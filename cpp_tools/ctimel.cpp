#include <iostream>
using namespace std;

void func()
{
    for(int i=0;i<10000;i++)
        for(int i=0;i<10000;i++){}
}

#include <queue>
#include "queue_th.h"

int main()
{

    cout<<"===== 1 ====="<<endl;
    queue<int> pott;
//    queue<int> po1(pott);
//    queue<int> po;
//    po = po1;
//    po.pop();
//    po.back();
//    po.push(123);
//    po.swap(pott);
//    po.front();
//    po.size();
//    po.empty();
//    po.emplace(123);
//    po.operator=(pott);
//    po.operator=()

//    std::deque<int> de1;
//    queue_th<int> p3(de1);
//    queue_th<int> p4(p3);
    queue_th<int> p2;
    cout<<"========"<<endl;
//    p2 = p3;
//    p2.pop();
//    p2.back();
    p2.push(12113);
    cout<<"========"<<endl;
    cout<<"========"<<p2.size()<<endl;
//    p2.swap(pott);
//    p2.front();
//    p2.size();
//    p2.empty();
    p2.emplace(1234);
    cout<<p2.front()<<endl;
    p2.pop();
    cout<<p2.front()<<endl;

//    po.operator=(pott);
//    p2.

    return 0;



    cout<<"===== begin ====="<<endl;

//    //创建临时区域，等待退出时打印
//    {
//        ctimel tm;
//        func();
//    }

//    ctimel tm;
//    func();
//    func();
//    tm.show();

//    //更新时间为现在的时间点
//    cout<<"== update =="<<endl;
//    tm.update();
//    func();
//    tm.show();


//    cout<<"== ctimes =="<<endl;
//    ctimes ct;
//    cout<<ct.to_now()<<endl;
//    cout<<ct.to_now("%H:%M:%S")<<endl;

//    //保留时间点并在未来打印
//    time_t t1 = ct.time_now();
//    cout<<ct.to_format(t1)<<endl;

//    cout<<"===== end ====="<<endl;
//    return 0;
}
