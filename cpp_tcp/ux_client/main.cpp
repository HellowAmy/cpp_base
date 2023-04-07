//!
//! ux_client.h : main
//!
#include "../ux_server/ux_tcp.h"
#include "../ux_server/ux_protocol.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main()
{
    const string ip = "127.0.0.1";
    const int port = 5005;
    bool is_run = true;
    ux_client client;

    client.sock_close = [&](){
        cout<<"sock_close"<<endl;
        is_run = false;
    };

    client.sock_read = [=](const string &msg){
        parse_msg(msg);
    };

    cout<<"client: "<<ip<<" | "<<port<<endl;
    auto sock = client.open_connect(ip,port);
    if(sock == nullptr) { cout<<"open_tcp err"<<endl; return -1; }

    cout<<"connect: in"<<endl;
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
