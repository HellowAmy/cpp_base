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
        cout<<"== sock_read =="<<endl;
        parse_msg(msg);
//        cout<<"sock_read: "<<msg<<endl;
    };

    cout<<"server: 5005"<<endl;
    auto sock = server.open_tcp(port,&ip);
    if(sock == nullptr) { cout<<"open_tcp err"<<endl; return -1; }

    cout<<"connect: in "<<ip<<endl;
    while (is_run)
    {
        string str;
        cin>>str;
        if(is_run == false) break;
        is_run = parse_cmd(str,sock);
//        is_run = sock->send_msg(str);
    }

    cout<<"===== end ====="<<endl;
    return 0;
}
