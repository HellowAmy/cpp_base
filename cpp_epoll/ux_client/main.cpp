//!
//! ux_client:main.cpp
//! ===== 客户端代码 =====
//!
#include "../ux_server/ux_epoll.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>
#include <functional>
#include <cstring>
#include <vector>

using namespace std;


//===== stmv =====
//功能:字符串切割,按分隔符将字符串切割到数组
//算法:利用vector<bool>生成与字符串一样长的标记位
//      切割算法扫描到切割符时将vector<bool>对应标记位置1(切割符占领位)
//      然后将连续0段加入结果数组
//用法示例:
//      [1]
//      string a = "11--22--33";
//      string b = "11--22++33";
//      string c = "11 22 33 44++55--66";
//      vector<string> vec = vts::stmv(a)("--");
//      [ret = 11,22,33]
//      vector<string> vec1 = vts::stmv(b)("--");
//      [ret = 11,22++33]
//      vector<string> vec2 = vts::stmv(c)(" ","++","--");
//      [ret = 11,22,33,44,55,66]
//
struct stmv
{
    string v_str;
    vector<string> vec_flg;
    vector<bool> vec_bit;

    stmv(const string &str) : v_str(str) { vec_bit.resize(str.size(),false); }

    template<class ...Tarr>
    vector<string> operator()(const Tarr &...arg) { return push_flg(arg...); }

    //获取切割符
    template<class ...Tarr> vector<string> push_flg()
    { return split_value(v_str,vec_flg); }
    template<class ...Tarr>
    vector<string> push_flg(const string &flg,Tarr ...arg)
    { vec_flg.push_back(flg); return push_flg(arg...); };

    //根据标记切割字符串
    vector<string> split_value(const string &in_str,const vector<string> &in_flg)
    {
        vector<string> vec;

        //标记数循环
        for(size_t iflg=0;iflg<in_flg.size();iflg++)
        {
            //字符串标记排查,存在用bit标记
            size_t pos_begin = 0;
            while(true)
            {
                pos_begin = in_str.find(in_flg[iflg],pos_begin);
                if(pos_begin != in_str.npos)
                {
                    for(size_t il=0;il<in_flg[iflg].size();il++)
                    { vec_bit[pos_begin+il]=1; }
                    pos_begin+=1;
                }
                else break;
            }
        }

        //根据0/1状态获取字符串,加入返回结果
        string str;
        for(size_t i=0;i<vec_bit.size();i++)
        {
            if(vec_bit[i] == false)
            {
                if(i>0 && (vec_bit[i-1] == true)) str.clear();
                str+=in_str[i];
            }
            else if(i>0 && (vec_bit[i-1] == false)) vec.push_back(str);
        }

        //末尾无状态转跳时加入结果
        if(vec_bit[vec_bit.size()-1] == false)
        { vec.push_back(str); }

        return vec;
    }
};
//===== stmv =====


//===== 消息处理结构体 =====
//== 请求类型 ==
enum en_transmit
{
    e_login,    //登陆
    e_swap,     //交换
    e_notify,   //通知
};

//== 消息权限 ==
enum en_msg
{
    e_public,   //群发
    e_private,  //私聊
};

//== 服务器解析聊天消息交互协议 ==
struct ct_msg_swap
{
    en_transmit et;     //用于判断是登陆还是发送信息
    en_msg em;          //用于判断是私聊还是群发
    size_t number_to;   //用于判断私聊时的目标ID
    size_t number_from; //用于存储发送者ID
    char name[64];      //发送者的昵称
    char buf[2048];     //存放发送的内容(登陆时:昵称,转发时:信息)
};
//===== 消息处理结构体 =====


//===== 发送协议 =====
//size_t readn(int fd, void *buf, size_t len)
//{
//    size_t all = len;
//    char *pos = (char *)buf;
//    while (all > 0)
//    {
//        size_t size = read(fd,pos,all);
//        if (size == -1u)
//        {
//            if (errno == EINTR) size = 0;
//            else return -1;
//        }
//        else if (size == 0) return 0;
//        pos += size;
//        all -= size;
//    }
//    return len - all;
//}

//size_t writen(int sock,const void *buf,size_t len)
//{
//    size_t all = len;
//    const char *pos = (const char *)buf;
//    while (all > 0)
//    {
//        size_t res = write (sock,pos,all);
//        if (res <= 0)
//        {
//            if (errno == EINTR) res = 0;
//            else return -1;
//        }
//        pos += res;
//        all -= res;
//    }
//    return len;
//}
//===== 发送协议 =====


//解析命令内容
string pares_send_cmd(const string &cmd,const string &name,size_t *number)
{
    ct_msg_swap ct_swap;
    memset(&ct_swap,0,sizeof(ct_swap));

    //查询所有用户信息
    if(cmd == "show") { ct_swap.et = e_notify; }
    else
    {
        string content = cmd;
        ct_swap.et = e_swap;
        ct_swap.em = e_public;

        //私聊,使用stmv按照分割副切割出容器,如果容器存在数据则标识为私聊
        vector<string> vec = stmv(cmd)(":");
        if(vec.size() >= 2)
        {
            ct_swap.em = e_private;
            ct_swap.number_to = from_string<size_t>(vec[0]);
            content = vec[1];
        }

        ct_swap.number_from = *number;
        strncpy(ct_swap.name,name.c_str(),sizeof(ct_swap.buf));
        strncpy(ct_swap.buf,content.c_str(),sizeof(ct_swap.buf));
    }
    return ct_s(ct_swap);
}


int main()
{
    bool is_run = true;
    size_t number = -1;
    ux_client client;

    string name;
    cout<<"please input your name: "<<endl; cin>>name;

    client.sock_new = [&](){
        //连接成功并登陆
        ct_msg_swap ct_login;
        memset(&ct_login,0,sizeof(ct_login));
        ct_login.et = e_login;
        strncpy(ct_login.buf,name.c_str(),sizeof(ct_login.buf));
        client.send_msg(ct_s(ct_login)); //发送数据
    };

    client.sock_close = [&](){
        is_run = false;
        cout<<"== sock_close =="<<endl;
    };

    client.sock_read = [&](const string &msg){
        //解析字符串成结构体
        ct_msg_swap ct = st_c<ct_msg_swap>(msg);

        //登陆反馈信息
        if(ct.et == e_login)
        {
            number = from_string<size_t>(ct.buf);
            if(number != -1u)
            { cout<<"<<<< login ID:"<<number<<" >>>>"<<endl; }
            else { cout<<"login err"<<number<<endl; exit(-1); }
        }

        //消息转发信息
        else if(ct.et == e_swap)
        {
            string stc_ms = "private";
            if(ct.em == e_public) stc_ms = "public";
            char buf[4096];
            snprintf(buf,sizeof(buf),"<<<< {%s: %s <%ld>} [%s] >>>>",
                     stc_ms.c_str(),ct.name,ct.number_from,ct.buf);
            cout<<buf<<endl;
        }

        //系统通知信息
        else if(ct.et == e_notify)
        {
            char buf[4096];
            snprintf(buf,sizeof(buf),"<<<< system: %s >>>>",ct.buf);
            cout<<buf<<endl;
        }
    };

    if(client.open_connect("127.0.0.1",5005) < 0)
    { cout<<"open_connect err"<<endl; return -1; }

    //循环用户的信息输入
    while (is_run)
    {
        string str;
        cin>>str;
        if(str == "exit") { break; }

        str = pares_send_cmd(str,name,&number); //解析输入命令
        if(client.send_msg(str) == false)
        { cout<<"== send err =="<<endl; }
    }
    client.close_connect();

    cout<<"===== end ====="<<endl;
    return 0;
}


