//!
//! ux_epoll.h <ux_epoll>
//!
#ifndef UX_EPOLL_H
#define UX_EPOLL_H

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <sstream>
#include <map>
#include <queue>
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


//===== 线程池 =====
class vpool_th
{
public:
    //创建线程池
    vpool_th(size_t number)
    {
        //准备一个循环函数--给线程池内的线程[等待任务/执行任务]
        auto create_func = [=](){
            while(true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_mutex);              //独占锁--获取队列任务
                    while (_tasks.empty() && _run) { _cond.wait(lock); }    //假唤醒--退出且队列为空
                    if(_run == false && _tasks.empty()) { return; }         //等待队列任务完成并退出任务
                    task = std::move(_tasks.front()); _tasks.pop();         //取任务
                }
                task(); //执行从队列获取的任务函数
            }
        };
        for(size_t i = 0;i<number;i++) { _workers.emplace_back(create_func); }
    }

    //释放线程池
    ~vpool_th()
    {
        { std::unique_lock<std::mutex> lock(_mutex); _run = false; }    //这里锁的用处--add_work执行时不给释放
        _cond.notify_all();                                             //唤醒所有线程准备退出
        for(std::thread &worker: _workers) { worker.join(); }           //等待所有线程完成任务后释放
    }

    //加入任务函数
    //  解析: typename std::result_of<Tfunc(Targs...)>::type -- 获取外部函数的返回值类型
    template<class Tfunc, class... Targs>
    auto add_work(Tfunc&& func, Targs&&... args)
        -> std::future<typename std::result_of<Tfunc(Targs...)>::type>
    {
        using ret_type = typename std::result_of<Tfunc(Targs...)>::type;                //任务函数的返回类型
        auto pack = std::bind(std::forward<Tfunc>(func), std::forward<Targs>(args)...); //任务函数打包
        auto task = std::make_shared<std::packaged_task<ret_type()>>(pack);             //打包为连接future类
        auto res = task->get_future();                                                  //从future类获取函数返回值
        {
            std::unique_lock<std::mutex> lock(_mutex);              //锁住并准备将任务插入队列
            std::function<void()> func = [task](){ (*task)(); };    //包装外部任务函数到function
            if(_run) { _tasks.emplace(func); }                      //插入function到队列
        }
        _cond.notify_one(); //通知一个线程去完成任务
        return res;
    }

private:
    bool _run = true;                           //运行标记
    std::vector<std::thread> _workers;          //线程容器
    std::mutex _mutex;                          //线程池锁
    std::queue<std::function<void()>> _tasks;   //任务队列
    std::condition_variable _cond;              //条件变量
};
//===== 线程池 =====


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
    function<int(int)> close_cb = nullptr;  //发送失败时触发回调--用于服务器

private:
    int _fd;            //连接套接字
    std::mutex _mutex;  //互斥锁--发送

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
};
//===== 数据管道 =====


//===== epoll事件循环 =====
class ux_epoll
{
public:
    //初始化epoll,启动线程池与拆包线程,拆包线程会永久占用一个线程
    ux_epoll()
    {
        _pool = new vpool_th(10);                       //线程池初始化
        _pool->add_work(&ux_epoll::work_parse_th,this); //启动拆包函数线程
    }
    ~ux_epoll() { delete _pool; }

    //! 启动epoll服务器
    //! 返回值：
    //!      0：正常退出
    //!     -1：监听失败
    //!     -2：创建epoll失败 epoll_wait
    //!     -3：epoll_wait失败
    //!     -4：监听套接字加入epoll失败
    //!
    int open_epoll(int port)
    {
        int listensock = init_port(port);//获取套接字--监听
        if(listensock <= 0) { return -1; }

        vlogd("启动epoll成功:" vv(port));

        //创建一个epoll描述符
        //参数1：无效数，不过要求必须大于0
        _fd_epoll = epoll_create(1);
        if(_fd_epoll <= 0) { return -2; }

        if(epoll_add(listensock) != 0) { return -4; }//将监听套接字加入epoll
        struct epoll_event events[_size_event];//存放epool事件的数组

        //epoll进入监听状态
        while (true)
        {
            //等待监视的socket有事件发生 | 参数4设置超时时间,-1为无限制
            //参数1：epoll描述符，参数2：epoll事件数组，
            //      参数3：同时处理的fd数量，参数4：超时（-1则无视超时时间）
            int infds = epoll_wait(_fd_epoll, events, _size_event, -1);
            if (infds < 0) { return -3; }

            //遍历有事件发生的结构数组--事件处理循环
            for (int i = 0; i < infds; i++)
            {
                //EPOLLIN事件：（1）新连接 （2）有数据可读 （3）连接正常关闭
                //（1）新连接,响应套接字等于监听套接字listensock
                if ((events[i].data.fd == listensock)
                        && (events[i].events & EPOLLIN))
                {
                    //建立请求，接收客户端的套接字（accept返回之后双方套接字可通信）
                    struct sockaddr_in client;
                    socklen_t len = sizeof(client);
                    int clientsock = accept(listensock,(struct sockaddr *)&client, &len);
                    set_non_block(clientsock);

                    //新连接进入
                    if (clientsock != -1)
                    {
                        //把新的客户端添加到epoll中
                        if(epoll_add(clientsock) == 0)
                        {
                            //触发新连接回调
                            string str_ip = inet_ntoa(client.sin_addr);
                            if(sock_new) _pool->add_work
                                    (sock_new,make_shared<channel>(clientsock),str_ip);
                        }
                        else vlogw("新连接--加入epoll失败");
                    }
                    else vlogw("新连接--接入失败");
                }

                //事件触发:有数据可读,或者连接断开
                else if (events[i].events & EPOLLIN)
                {
                    char buf[_size_buf];
                    memset(buf,0,sizeof(buf));
                    size_t size = read(events[i].data.fd,&buf,sizeof(buf));

                    //发生了错误或socket被对方关闭
                    if(size <= 0)
                    {
                        //把已断开的客户端从epoll中删除
                        if(epoll_del(events[i].data.fd) == 0)
                        {

                            //触发关闭回调
                            int fd = events[i].data.fd;
                            if(sock_close) _pool->add_work
                                    (sock_close,make_shared<channel>(fd));
                        }
                        else vlogw("连接断开：epoll删除失败");
                    }
                    else
                    {
                        //子线程解析（需要将字符串复制一份而不是引用,因为地址会反复填充新内容）
                        int fd = events[i].data.fd;
                        add_work_parse(bind(&ux_epoll::parse_buf_th,this,fd,string(buf,size)));

                        //原地拆包解析(无需多线程，但是可能降低IO遍历的能力)
                        //parse_buf(events[i].data.fd,buf,size);
                    }

                }//<<事件触发
            }//<<遍历有事件发生的结构数组
        }//<<epoll进入监听状态
        return 0;
    }

    //新连接
    function<void(const shared_ptr<channel> &pch,const string &ip)>
            sock_new = nullptr;
    //关闭连接
    function<void(const shared_ptr<channel> &pch)>
            sock_close = nullptr;
    //读取数据
    function<void(const shared_ptr<channel> &pch,const string &msg)>
            sock_read = nullptr;
protected:
    int _size_event = 1024;     //单次IO扫描最大事件数
    int _size_buf = 4096;       //接收数据缓冲区大小
    int _fd_epoll = 0;          //epoll描述符
    mutex _mutex;               //互斥锁
    vpool_th *_pool;            //线程池
    mutex _mutex_parse;         //互斥锁--用于拆包解析函数
    condition_variable _cond;   //条件变量
    queue<function<void()>> _queue_task;    //解析任务队列
    map<int,string> _map_save_read;         //存储fd拆包剩余数据

    //设置为非阻塞套接字
    int set_non_block(int fd)
    {
        //将O_NONBLOCK无堵塞选项设置到fd中
        int old_op = fcntl(fd, F_GETFL);
        int new_op = old_op | O_NONBLOCK;
        fcntl(fd,F_SETFL,new_op);
        return old_op;
    }

    //从epoll移除套接字
    int epoll_del(int fd)
    {
        //从epoll移除fd，根据结构体信息
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        int ret = epoll_ctl(_fd_epoll, EPOLL_CTL_DEL, fd, &ev);
        if(ret != 0) close(fd);
        return ret;
    }

    //套接字加入epoll
    int epoll_add(int fd)
    {
        //将临时结构体加入到epoll--水平触发
        struct epoll_event ev;
        memset(&ev,0,sizeof(struct epoll_event));
        ev.data.fd = fd;
        ev.events = EPOLLIN;
        return epoll_ctl(_fd_epoll,EPOLL_CTL_ADD,fd,&ev);
    }

    //启动子线程执行拆包
    void work_parse_th()
    {
        //拆包子线程，单线程执行,自行运行一个循环防止退出，并永久占用一个线程池中的子线程
        while (true)
        {
            unique_lock<std::mutex> lock(_mutex_parse);     //此处单线程启动，无需锁，用于唤醒
            while(_queue_task.empty()){ _cond.wait(lock); } //假唤醒--退出且队列为空

            //取任务并执行
            function<void()> task = std::move(_queue_task.front());
            _queue_task.pop();
            task();
        }
    }

    //添加拆包内容
    void add_work_parse(function<void()> task)
    { _queue_task.push(task); _cond.notify_one(); }

    //epoll水平触发拆包函数--子线程
    void parse_buf_th(int fd,const string &buf)
    { parse_buf(fd,buf.c_str(),buf.size()); }

    //epoll水平触发拆包函数
    void parse_buf(int fd,const char *buf,size_t size)
    {
        size_t all_len = 0;
        string all_content;

        //查找是否有上次剩余部分
        auto it = _map_save_read.find(fd);
        if(it != _map_save_read.end())
        {
            all_len += it->second.size() + size;
            all_content += it->second + string(buf,size);
        }
        else
        {
            all_len = size;
            all_content = string(buf,size);
        }

        //循环解析，一次读取可能有多个任务包
        bool is_break = false;
        while (true)
        {
            //超过八个字节（判断是否能完整读出头部大小）
            if(all_len > sizeof(all_len))
            {
                //解析出ct_msg结构体的信息--长度
                size_t con_len = *(size_t*)string(all_content.begin(),
                                    all_content.begin()+sizeof(all_len)).c_str();

                //判断目前剩余量是否大于等于一个包的长度
                if((all_len - sizeof(all_len)) >= con_len)
                {
                    //解析的内容
                    string buf_content(all_content,sizeof(all_len),con_len);

                    //保存信息剩余量
                    all_len -= sizeof(all_len) + con_len;
                    all_content = string(all_content.begin() +
                                    sizeof(all_len) + con_len,all_content.end());
                    //触发读回调
                    auto pch = make_shared<channel>(fd);
                    pch->close_cb = bind(&ux_epoll::epoll_del,this,_1);
                    if(sock_read) _pool->add_work(sock_read,pch,buf_content);
                }
                else is_break = true;
            }
            else is_break = true;

            if(is_break)
            {
                //如果已经存在则插入剩余容器,不存在则新建,完成插入后退出循环
                if(it != _map_save_read.end())
                    { it->second = all_content; }
                else { _map_save_read.emplace(fd,all_content); }
                break;
            }
        }
    }

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

        //监听或绑定失败返回错误 | listen函数 参数2：正在连接的队列容量
        if (bind(sock, (struct sockaddr *)&servaddr,
                 sizeof(servaddr)) < 0 || listen(sock, _size_event) != 0)
        { close(sock); return -2; }

        return sock;
    }
};
//===== epoll事件循环 =====


//===== 客户端发送协议 =====
class ux_client
{
public:
    ux_client(){}
    ~ux_client() { if(pch != nullptr) { delete pch; pch = nullptr; } }

    int get_fd() { return _fd; }
    bool is_connect() { return _connect; }

    //建立连接
    int open_connect(const string &ip,int port)
    {
        _fd = init_connect(ip,port);
        if(_fd < 0) { if(sock_close){sock_close();} return _fd; }

        //建立发送管道
        pch = new channel(_fd);
        _connect = true;
        if(sock_new) sock_new();

        //建立读取子线程
        thread(&ux_client::read_string_th, this,_fd,sock_read,sock_close).detach();
        return _fd;
    }

    //关闭连接
    int close_connect()
    { _connect = false; return close(_fd); }

    //发送string字符串
    bool send_msg(const string &msg)
    {
        if(_connect)
        {
            bool ok = pch->send_msg(msg);
            if(ok == false)
            { _connect = false; if(sock_close) {sock_close();} }
            return ok;
        }
        else return false;
    }

    function<void()> sock_new = nullptr;                    //新连接
    function<void()> sock_close = nullptr;                  //关闭连接
    function<void(const string &msg)> sock_read = nullptr;  //读取数据

protected:
    int _fd = 0;
    bool _connect = false;
    channel *pch = nullptr;

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
};
//===== 客户端发送协议 =====

#endif // UX_EPOLL_H
