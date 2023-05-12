#ifndef CTIMEL_H
#define CTIMEL_H

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


/*
class ctimel
{
public:
    ctimel() { _begin = steady_clock::now(); }
    ~ctimel() { show(); }

    inline string to_str()
    {
        //顺序 [纳秒|微秒|毫秒|秒]
        auto loss = steady_clock::now() - _begin;
        string str =
                "[nan: " + std::to_string(loss.count()) +
                "|mic: " + std::to_string(duration_cast<microseconds>(loss).count()) +
                "|mil: " + std::to_string(duration_cast<milliseconds>(loss).count()) +
                "|sec: " + std::to_string(duration_cast<seconds>(loss).count()) +
                "]\n";
        return str;
    }

    inline void show_str(const string str) { cout<<str<<endl; }
    inline void show() { show_str(to_str()); }
    inline void update() { _begin = steady_clock::now(); }

protected:
    time_point<steady_clock,nanoseconds> _begin;
};
*/



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

/*
class ctimes
{
public:
    //现在的时间点-可视化
    string date_now(const string &format = "%Y-%m-%d %H:%M:%S")
    {
        char ret[1024];
        time_t t = system_clock::to_time_t(system_clock::now());
        strftime(ret,sizeof(ret),format.c_str(),localtime(&t));
        return ret;
    }

    //格式化时间-可视化
    string to_format(const time_t &t, const string &format = "%Y-%m-%d %H:%M:%S")
    {
        char ret[1024];
        strftime(ret,sizeof(ret),format.c_str(),localtime(&t));
        return ret;
    }
    time_t time_now(){ time_t t; return time(&t); };//当前时间
};
*/
//===== ctimel =====

#endif // CTIMEL_H
