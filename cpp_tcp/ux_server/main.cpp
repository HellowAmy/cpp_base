//!
//! C++网络：TCP点对点通信-附带文件传输代码实例
//!
//! ===== TCP简介 =====
//! TCP作为传输层通信协议，因为存在三次握手四次挥手等保证了数据的可靠性，
//!     已经成为了目前主流的通信手段。
//!
//! TCP连接的建立需要调用固定函数流程，实现服务器与客户端的连接，
//!     通常这些调用是固定的，连接成功之后服务器与客户端都会拿到套接字，
//!     套接字sock与文件描述符fd的用户几乎是一致的，
//!     支持read，write，close等文件操作函数，
//!     其中套接字sock额外支持网络读写的recv和send函数
//! ===== TCP简介 =====
//!
//!
//! ===== TCP连接 =====
//! 服务器：
//!     socket(创建套接字)，bind(绑定端口号)，accept(等待客户端连接)
//!
//! 客户端：
//!     socket(创建套接字)，connect(主动连接服务器)
//!
//! 发送与接收：
//!     write/send : 将buf写入缓冲区，等待TCP发送，send的区别为添加第四参数
//!     read/recv : 从套接字缓冲区内读内容到buf并返回读取字节，recv的区别为添加第四参数
//!
//! 第四参数的通常用处：
//!     1.不查路由：在本机发送数据时提高速度
//!     2.保持缓存：套接字读取buf时不会清除缓冲区，可以反复读取，用于查看
//!     3.等待内容：套接字读取是不会提前返回，等待发送方的字节同步，可用于保证发送与接收次数一致
//!     4.立即返回：非堵塞模式，可以快速响应读写后的操作
//!     5.紧急数据：额外发送一字节的外带数据，且立即发送，基本是鸡肋功能
//!
//! send/recv提供第四参数附带的功能，如果不需要这些功能也不希望提高复杂度，
//!     可将sock看做fd即可，对网络数据的操作可以像对文件的操作一样简单
//! ===== TCP连接 =====
//!
//!
//! ===== 任务简介 =====
//! 建立TCP连接点对点通信，实现文字通信与文件传输功能：
//!     点对点通信即一个客户端与一个服务器相连接，一旦连接成功立刻退出等待，
//!         服务器不再与新的客户端建立连接
//!     由于需要发送信息与文件传输两种传输类型，需要定义一个简单协议，
//!         对文字与文件进行区分并做出不同处理
//! ===== 任务简介 =====
//!
//!
//! ===== 代码流程 =====
//! ux_tcp.h        : 可实现点对点连接，连接成功立刻返回通信管道
//! ux_protocol.h   : 简单的文件传输机制，可实现传输功能，但不可用，没有对错误与发送超时处理
//! ux_server main  : 服务器代码
//! ux_client main  : 客户端代码
//! ===== 代码流程 =====
//!
//! 结束语：
//!     本次实现的TCP连接只考虑了点对点，因为是点对点连接，
//!         服务器与客户端都采用了连接后立刻返回的处理，
//!         这是为了减少二者的使用差距，意图在于建立一种两个进程的数据管道，
//!         而不是为了区分服务器与客户端的请求响应机制
//!     如果希望服务器一对多，可以使用IO复用技术，另一篇文章有介绍
//!
//!
//! ux_server.h : main
//!
#include "ux_tcp.h"
#include "ux_protocol.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    const int port = 5005;
    string ip;
    bool is_run = true;
    ux_tcp server;

    server.sock_close = [&](){
        cout<<"sock_close"<<endl;
        is_run = false;
    };

    server.sock_read = [=](const string &msg){
        parse_msg(msg);
    };

    cout<<"server: 5005"<<endl;
    auto sock = server.open_tcp(port,&ip);
    if(sock == nullptr) { cout<<"open_tcp err"<<endl; return -1; }

    cout<<"connect: in "<<ip<<endl;
    while (is_run)
    {
        string str;
        cin>>str;
        if(str == "exit" || is_run == false) break;
        is_run = parse_cmd(str,sock);
    }

    cout<<"===== end ====="<<endl;
    return 0;
}

/*
 * 对话测试：
 *
//== 服务端 ==
server: 5005
connect: in 127.0.0.1
HellowAmy
read: 你好
我发一份软件给你，记得处理
file:qtapp.run
send file: qtapp.run
send finish: qtapp.run
read: 收到了
OK


//== 客户端 ==
client: 127.0.0.1 | 5005
connect: in
read: HellowAmy
你好
read: 我发一份软件给你，记得处理
begin recv file: qtapp.run
recv file finish: qtapp.run
收到了
read: OK
sock_close
*/
