//!
//! C++工具: 显示时间函数-计算函数运行时间
//!
//! ===== 工具简介 =====
//! 简单的时间显示工具，来之C++11的时间库
//!     ctimel : 打印时间间隔，主要用于计算函数运行的时间
//!     ctimes : 打印可视化时间，用于显示现在的时间，支持格式化输出
//! ===== 工具简介 =====
//!
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;


//===== ctimel =====
//!
//! 功能：打印时间间隔
//!
//! 说明：主要用于计算函数运行的时间，测试函数性能
//!         steady_clock时间：精确到纳秒的均速时间
//! 原理：构建对象时开始计时，析构时打印时间
//!
//! 例子：
//!     {
//!         ctimel tm;
//!         func();
//!     } //到这里析构退出并打印时间
//!
class ctimel
{
public:
    ctimel() { _begin = steady_clock::now(); }
    ~ctimel() { show(); }

    inline void show()
    {
        //顺序 [纳秒|微秒|毫秒|秒]
        auto loss = steady_clock::now() - _begin;
        std::cout<<"[nan: "<<loss.count()
                <<"|mic: "<<duration_cast<microseconds>(loss).count()
               <<"|mil: "<<duration_cast<milliseconds>(loss).count()
              <<"|sec: "<<duration_cast<seconds>(loss).count()
             <<"]"<<std::endl;
    }

    inline void update() { _begin = steady_clock::now(); }

protected:
    time_point<steady_clock,nanoseconds> _begin;
};

//!
//! 功能：显示可视化的时间
//!
class ctimes
{
public:
    //现在的时间点-可视化
    string to_now(const string &format = "%Y-%m-%d %H:%M:%S")//
    {
        char ret[1024];
        time_t t = system_clock::to_time_t(system_clock::now());
        strftime(ret,sizeof(ret),format.c_str(),localtime(&t));
        return ret;
    }

    //格式化时间-可视化
    string to_format(const time_t &t,
        const string &format = "%Y-%m-%d %H:%M:%S")
    {
        char ret[1024];
        strftime(ret,sizeof(ret),format.c_str(),localtime(&t));
        return ret;
    }

    //当前时间
    time_t time_now(){ time_t t; return time(&t); };
};
//===== ctimel =====


void func()
{
    for(int i=0;i<10000;i++)
        for(int i=0;i<10000;i++){}
}


int main()
{
    cout<<"===== begin ====="<<endl;

    //创建临时区域，等待退出时打印
    {
        ctimel tm;
        func();
    }

    ctimel tm;
    func();
    func();
    tm.show();

    //更新时间为现在的时间点
    cout<<"== update =="<<endl;
    tm.update();
    func();
    tm.show();


    cout<<"== ctimes =="<<endl;
    ctimes ct;
    cout<<ct.to_now()<<endl;
    cout<<ct.to_now("%H:%M:%S")<<endl;

    //保留时间点并在未来打印
    time_t t1 = ct.time_now();
    cout<<ct.to_format(t1)<<endl;

    cout<<"===== end ====="<<endl;
    return 0;
}
