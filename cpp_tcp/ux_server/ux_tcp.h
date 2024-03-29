//!
//! ux_tcp.h
//!
#ifndef UX_TCP_H
#define UX_TCP_H

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <future>
#include <functional>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::placeholders;


//===== 日志宏 =====
#define vv(value) "["#value": "<<value<<"] "
#define vloge(...) std::cout<<"\033[31m[Err] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogw(...) std::cout<<"\033[33m[War] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogd(...) std::cout<<"\033[32m[Deb] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogf(...) std::cout<<"[Inf] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<endl
//===== 日志宏 =====


//== 字符串类型转换 ==
template<typename T>
string to_string(const T& t)
{ ostringstream os; os<<t; return os.str(); }

template<typename T>
T from_string(const string& str)
{ T t; istringstream iss(str); iss>>t; return t; }
//== 字符串类型转换 ==


//===== 结构体转换string函数 =====
//结构体转string
//      语法解析：(char*)&ct ，由&ct获取结构体地址，在由该地址(char*)转为char*类型的指针
//      根据string构造函数，参数1：char*地址，参数2：长度，可从地址内存中复制二进制内容
template <class T_ct>
static string ct_s(T_ct ct)
{ return string((char*)&ct,sizeof(T_ct)); }

//string转结构体
//      语法解析：*(T_ct*)str.c_str() ，由str.c_str()从string类获取const char*指针，
//      由const char*指针转为T_ct*指针，再*（T_ct*）从指针中获取值，从而返回值
template <class T_ct>
static T_ct st_c(const string &str)
{ T_ct ct = *(T_ct*)str.c_str(); return ct; }
//===== 结构体转换string函数 =====


//===== 数据管道 =====
class channel
{
public:
    channel(int fd) : _fd(fd){}
    int get_fd() const { return _fd; }

    //发送string字符串，带锁
    bool send_msg(const string &msg)
    {
        unique_lock<mutex> lock(_mutex);
        if(send_msg(_fd,msg,NULL) == false)
        { if(close_cb) {close_cb(_fd);} return false; }
        else return true;
    }

    //读取反馈信息--线程启动
    void read_string_th(int fd,function<void(string)> read_cb,function<void()> close_cb)
    {
        size_t all_len = 0;
        string all_content;
        while(true)
        {
            char buf[1024];
            memset(buf,0,sizeof(buf));
            size_t size = read(fd,&buf,sizeof(buf));
            if(size <= 0) { if(close_cb){close_cb();} return; }

            //加入新内容（可能存在上一次的人剩余信息）
            all_len += size;
            all_content += string(buf,size);

            while(true)
            {
                //超过八个字节（判断是否能完整读出头部大小）
                if(all_len > sizeof(all_len))
                {
                    //解析出ct_msg结构体的信息--长度
                    size_t con_len = *(size_t*)string(all_content,0,sizeof(con_len)).c_str();

                    //判断目前剩余量是否大于等于一个包的长度
                    if((all_len - sizeof(all_len)) >= con_len)
                    {
                        //解析的内容
                        string buf_content(all_content,sizeof(all_len),con_len);
                        if(read_cb) read_cb(buf_content);//解析出完整包后触发回调

                        //存放剩余的内容
                        all_len -= sizeof(all_len) + con_len;
                        all_content = string(all_content.begin() +
                                        sizeof(all_len) + con_len,all_content.end());
                    }
                    else break;
                }
                else break;
            }
        }
    }

    //指定发送N个字节的数据
    size_t writen(int sock,const void *buf,size_t len) const
    {
        size_t all = len;
        const char *pos = (const char *)buf;
        while (all > 0)
        {
            size_t res = write (sock,pos,all);
            if (res <= 0){ if (errno == EINTR){res = 0;} else{return -1;} }
            pos += res; all -= res;
        }
        return len;
    }

    //发送string字符串
    bool send_msg(int sock,const string &msg,size_t *all)
    {
        size_t len = msg.size();
        string buf;
        buf += string((char*)&len,sizeof(len));
        buf += msg;

        size_t ret = writen(sock,buf.c_str(),buf.size());
        if(all != nullptr) *all = ret;
        return ret != -1u;
    }

    function<void(int)> close_cb = nullptr;  //发送失败时触发回调--用于服务器

private:
    int _fd;            //连接套接字
    std::mutex _mutex;  //互斥锁--发送
};
//===== 数据管道 =====


//===== TCP服务器 =====
class ux_tcp
{
public:
    //建立连接
    shared_ptr<channel> open_tcp(int port,string *in_ip)
    {
        int listen = init_port(port);
        if(listen < 0) { vloge("init port err"); return nullptr; }

        //建立请求，接收客户端的套接字（accept返回之后双方套接字可通信）
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int fd = accept(listen,(struct sockaddr *)&client, &len);
        close(listen); //断开监听套接字，防止新的客户端连接

        //新连接进入
        if (fd == -1) { vloge("accept err"); return nullptr; }
        if(in_ip) *in_ip = inet_ntoa(client.sin_addr);

        _pch = make_shared<channel>(fd);
        thread(&channel::read_string_th,_pch,fd,sock_read,
               bind(&ux_tcp::close_connect,this)).detach();
        return _pch;
    }

    //关闭连接
    void close_connect()
    { close(_pch->get_fd()); if(sock_close) sock_close(); }

    function<void()> sock_close = nullptr;                  //关闭连接
    function<void(const string &msg)> sock_read = nullptr;  //读取数据

private:
    shared_ptr<channel> _pch; //数据管道

    //! 初始化监听端口，返回套接字
    //! 返回值：
    //!     -1：socket打开失败
    //!     -2：bind建立失败
    //!     sock：返回成功，建立的套接字
    //!
    int init_port(int port)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0); //设置TCP连接模式
        if (sock < 0) { return -1; }

        int opt = 1;
        unsigned int len = sizeof(opt);
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len); //打开复用
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, len); //打开心跳

        //设置网络连接模式
        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;				  //TCP协议族
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //监听所有
        servaddr.sin_port = htons(port);			  //兼容端口

        //监听或绑定失败返回错误 | listen函数 参数2：正在连接的队列容量(点对点时为1)
        if (bind(sock, (struct sockaddr *)&servaddr,
                 sizeof(servaddr)) < 0 || listen(sock,1) != 0)
        { close(sock); return -2; }

        return sock;
    }
};
//===== TCP服务器 =====


//===== TCP客户端 =====
class ux_client
{
public:
    //建立连接
    shared_ptr<channel> open_connect(const string &ip,int port)
    {
        int fd = init_connect(ip,port);
        if(fd < 0) { return nullptr; }

        _pch = make_shared<channel>(fd);
        thread(&channel::read_string_th,_pch,fd,sock_read,
               bind(&ux_client::close_connect,this)).detach();
        return _pch;
    }

    //关闭连接
    void close_connect()
    { close(_pch->get_fd()); if(sock_close) sock_close(); }

    function<void()> sock_close = nullptr;                  //关闭连接
    function<void(const string &msg)> sock_read = nullptr;  //读取数据

private:
    shared_ptr<channel> _pch; //数据管道

    //! 网络连接初始化
    //! 返回值：
    //!     -1：socket打开失败
    //!     -2：IP转换失败
    //!     -3：connect连接失败
    //!     sock：返回成功，建立的套接字
    //!
    int init_connect(const string &ip,int port)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0); //设置TCP连接模式
        if (sock < 0) { return -1; }

        int opt = 1;
        unsigned int len = sizeof(opt);
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len); //打开复用
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, len); //打开心跳

        //设置网络连接模式
        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;				  // TCP协议族
        servaddr.sin_port = htons(port);			  //兼容端口

        //IP转换
        if(inet_pton(AF_INET,ip.c_str(), &servaddr.sin_addr) <=0 )
        { return -2; }

        //建立连接
        if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
        { return -3; }

        return sock;
    }
};
//===== TCP客户端 =====

#endif // UX_TCP_H
