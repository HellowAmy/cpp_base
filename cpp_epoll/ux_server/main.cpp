//!
//! C++网络:IO复用epoll服务器-附带网络聊天室代码实例
//!
//! ===== IO复用简介 =====
//! 众所周知,在LINUX中有一切皆文件的说法,将文件视为一种IO流,网络也被抽象为IO流,
//!     网络数据的操作与文件文件操作一致
//! 如socket等进行TCP连接时返回的套接字,对socket的读写都会堵塞线程等待返回,
//!     此时一个线程只能对一个socket进行监控读写,也就是只能对一个IO口进行操作,
//!     如果采用非堵塞的方式就可以对多个IO口进行监控,因为非堵塞下读写IO口会立刻返回,
//!     不管是否成功读写到数据
//! 如果将多个成功连接的socket存储起来,并采用非堵塞模式定时对IO口循环扫描,
//!     这样就可以知道那一个IO口有数据可读,这就是IO复用的基本原理
//! 目前LINUX下IO复用主流的监控方式有select/poll/epoll,其中select/poll为循环扫描所有IO口,
//!     不管该IO口是否有读写操作,在监控的IO口数量多时会拉低扫描速度,通常的监控上限建议是1024个IO口
//! epoll则采用事件触发的方式,即扫描IO口无读写事件时不会将IO可加入到处理队列,
//!     所以epoll不存在监控上限,所以在大数量连接上epoll可以完全取代select/poll
//! ===== IO复用简介 =====
//!
//!
//! ===== epoll简介 =====
//! 事件触发:
//!     epoll的使用特点就是,使用一个epool事件数组放入epoll_wait函数,当有IO口存在可读事件,
//!         epoll_wait函数就会返回,并将事件放入epool事件数组中,
//!         让使用者在循环的对epool事件数组中的IO口进行操作
//! 触发方式:
//!     epoll存在水平触发(LT)/边缘触发(ET),两种触发方式
//!         水平触发:只要epool事件数组中还存在IO口未读完的数据就会让epoll_wait函数返回并进入事件处理循环
//!         边缘触发:epool事件数组中每一次数据变化只会触发一次,不读取数据会一直留在IO口
//!     水平触发是epoll的默认触发模式，边沿触发则属于高速性能模式，两则的性能差距尚未可知，
//!         但边沿触发的读写处理往往更加复杂且容易引发错误
//! ===== epoll简介 =====
//!
//!
//! ===== 任务介绍 =====
//! 使用epoll写一个网络聊天室的服务器:
//!     场景描述:
//!         聊天室在命令行中运行,可以在登陆时输入昵称,有群聊和私聊的功能,
//!             并可以查看所有已经登陆的用户
//! ===== 任务介绍 =====
//!
//!
//! ===== 服务器实现流程 =====
//! 1.将监听socket设置为堵塞模式,保证所有新连接能成功接入服务器
//! 2.将新连接的读写socket设置为非堵塞模式,并在主线程完成读操作
//! 3.TCP的socket是字节流,在应用层会出现粘包问题,需要在应用层实现字节流的发送协议
//!         (该服务器的协议为:[内容长度:内容]模式的拆包协议,发送与接收接口为string)
//! 4.客户端与服务器的传输协议使用结构体转换进行传输
//!         (发送方:struct->string ,网络传输:通过网络发送string到接收方 ,接收方:string->struct)
//! 5.启动线程池,多线程处理拆包协议与回调任务处理函数
//! 6.拆包时在子线程完成,且并为每次拆包都可以拿到一整个包的数据,数据不足时需要存储数据包
//! 7.解析出数据包触发任务回调函数,需要提供channel管道,保证反馈到客户端的封包协议一致
//! 8.提供读写回调函数接口给使用者
//! ===== 服务器实现流程 =====
//!
//!
//! ===== 客户端实现流程 =====
//! 1.客户端保证使用的封包/拆包协议与服务器一致,发送的请求协议与服务器协议一致
//! 2.不需要线程池,但需要启动子线程对IO口进行堵塞读,IO口的堵塞写操作不能与读操作在同一个线程
//! 3.客户端需要提供一个命令处理操作函数,对不同的命令向服务器发出不同请求
//! ===== 客户端实现流程 =====
//!
//!
//! ===== 客户端使用方式 =====
//! 输入命令规则:(三种命令输入方式)
//!          [文字]:直接发送--公开模式群发
//!                  例子:     大家好,我是小黄
//!          ID:[文字]:用冒号分割,前面是登陆ID,后面是发送内容
//!                  例子:     3:你好阿!3号,我是小黄
//!          [show]:查看所有的已登陆用户
//!                  例子:     show
//!                      返回内容:
//!                          <<<< system:
//!                          1:小名
//!                          2:小黄
//!                          3:小虎
//!                           >>>>
//!
//! == 输出显示 ==
//!     <<<< system: [登陆] [ID:3] [昵称:小花] >>>>
//!     <<<< {public: 小黄 <1>} [大家好] >>>>
//!     <<<< {public: 小毛 <2>} [小黄你好阿] >>>>
//!     <<<< {private: 小毛 <2>} [小花,你看到了吗,是新朋友] >>>>
//!     <<<< system: [退出] [ID: 2] [name: 小毛] >>>>
//! == 输出显示 ==
//! ===== 客户端使用方式 =====
//!
//!
//! ===== 代码段展示顺序 =====
//! 1.ux_epoll.h
//! 2.ux_server:main.cpp
//! 3.ux_client:main.cpp
//! ===== 代码段展示顺序 =====
//!
//! 结束语:
//!     该服务器的ux_epoll.h文件实现了一个轻量线程池,简单的TCP拆包协议,
//!         发送数据的string接口,结构体转string等,可以完全满足小数量的简易服务器部署,
//!         且无需任务依赖,只要支持C++11即可携带ux_epoll.h头文件在LINUX环境下到处部署IO复用服务器
//!
//!
//! ux_server:main.cpp
//! ===== 服务端代码 =====
//!
#include "../../include/ux_epoll.h"
#include <cstring>
#include <algorithm>


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

//== 登陆信息记录 ==
struct ct_login_id
{
    int fd;                     //存放fd,用于转发信息时建立连接
    string name;                //存放连接用户的昵称
    shared_ptr<channel> pch;    //用于发送信息的接口
};
//===== 消息处理结构体 =====


//== 主函数 ==
int main()
{
    mutex mutex_read;                   //读锁
    map<size_t,ct_login_id> map_login;  //登陆的用户索引,用于查找
    size_t count_login = 0;             //分配ID
    ux_epoll server_epoll;              //epoll服务器

    //===== 回调区 =====
    //新连接
    server_epoll.sock_new = [&](const shared_ptr<channel> &pch,const string &ip){
        vlogd("sock_new: " vv(pch->get_fd()) vv(ip));
    };

    //读数据--服务器接收到数据
    server_epoll.sock_read = [&](const shared_ptr<channel> &pch,const string &msg){
        unique_lock<mutex> lock(mutex_read); //加锁是因为服务器读数据是多线程读取
        ct_msg_swap ct = st_c<ct_msg_swap>(msg); //字符串转结构体,无需引入json即可结构化数据

        //登陆的处理:分配ID,存储fd和昵称,反馈登陆ID,群发登陆用户信息
        if(ct.et == e_login)
        {
            //分配ID
            count_login++;

            //存储fd和昵称
            ct_login_id ct_id;
            ct_id.fd = pch->get_fd();
            ct_id.name = ct.buf;
            ct_id.pch = pch;
            map_login.insert(pair<size_t,ct_login_id>(count_login,ct_id));

            //反馈登陆ID
            ct.et = e_login;
            vlogd("count_login:" vv(to_string(count_login)));
            strncpy(ct.buf,to_string(count_login).c_str(),sizeof(ct.buf));
            pch->send_msg(ct_s(ct)); //结构体转string,并原路发送到客户端

            //群发登陆用户信息
            string content;
            ct.et = e_notify;
            content = "[登陆] [ID:" + to_string(count_login) +"] [昵称:"+ct_id.name+"]";
            strncpy(ct.buf,content.c_str(),sizeof(ct.buf));
            vlogd("群发:" vv(content) vv(ct_id.name) vv(count_login));
            for(const auto &a:map_login)
            { a.second.pch->send_msg(ct_s(ct)); }
        }

        //转发消息的处理:区分群发和私发,获取转发数据接口,转发数据
        else if(ct.et == e_swap)
        {
            if(ct.em == e_public)//群发
            {
                for(const auto &a:map_login)
                { a.second.pch->send_msg(ct_s(ct)); }
            }
            else if(ct.em == e_private)//私发
            {
                //查找并发送
                auto it = map_login.find(ct.number_to);
                if(it != map_login.end())
                    { it->second.pch->send_msg(ct_s(ct)); }
                else
                {
                    strncpy(ct.buf,"信息无法送达--请检查是否合理",sizeof(ct.buf));
                    ct.et = e_notify;
                    pch->send_msg(ct_s(ct));
                    vlogw("== number inexistence ==");
                }
            }
            else { vlogw("== en_msg inexistence =="); }
        }

        //所有用户信息请求:排队ID,通过ID获取昵称,原路反馈信息
        else if(ct.et == e_notify)
        {
            //排队ID
            vector<size_t> vec;
            for(const auto &a:map_login)
            { vec.push_back(a.first); }
            std::sort(vec.begin(),vec.end());

            //通过ID获取昵称
            string content = "\n";
            for(const auto &a:vec)
            {
                //查找并发送
                auto it = map_login.find(a);
                if(it != map_login.end())
                { content += to_string(it->first) + ":" + it->second.name + "\n"; }
            }

            //原路反馈信息
            ct.et = e_notify;
            strncpy(ct.buf,content.c_str(),sizeof(ct.buf));
            pch->send_msg(ct_s(ct));
        }
        else { vlogw("== en_transmit inexistence =="); }
    };

    //关闭连接--客户端主动关闭
    server_epoll.sock_close = [&](const shared_ptr<channel> &pch){
        unique_lock<mutex> lock(mutex_read);

        //群发通知信息有用户退出
        ct_msg_swap ct;
        memset(&ct,0,sizeof(ct));

        ct.et = e_notify;//通知类型
        for(auto a:map_login)
        {
            if(a.second.fd == pch->get_fd())
            {
                //记录退出信息
                string content = "[退出] [ID: "+to_string(a.first)+"] [昵称: "+a.second.name+"]";
                strncpy(ct.buf,content.c_str(),sizeof(ct.buf));
                map_login.erase(a.first);
                break;
            }
        }

        //群发通知
        for(const auto &a:map_login)
        { a.second.pch->send_msg(ct_s(ct));  }

        vlogd("channel 断开的fd :" vv(pch->get_fd()));
    };
    //===== 回调区 =====

    int ret = server_epoll.open_epoll(5005);    //启动服务器
    vlogd("open_epoll ret: " vv(ret));          //服务器退出

    printf("\n===== end =====\n");
    return 0;
}
