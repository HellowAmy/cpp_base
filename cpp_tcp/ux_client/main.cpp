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
        cout<<"== sock_read =="<<endl;
        parse_msg(msg);
//        cout<<"sock_read: "<<msg<<endl;
    };

    cout<<"client: "<<ip<<" | "<<port<<endl;
    auto sock = client.open_connect(ip,port);
    if(sock == nullptr) { cout<<"open_tcp err"<<endl; return -1; }

    cout<<"connect: in"<<endl;
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
