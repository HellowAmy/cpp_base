//!
//! C++工具：C++高精度定时器--提供自定义精度回调定时器
//!
//! == 高精度定时器简介 ==
//! 定时器是网络编程中常见的组件之一，通常用于处理超时任务，
//!     定时器在普通程序中虽不常用，但需要时却无从下手
//! 本次提供支持自定义精度的定时器，采用多线程回调触发定时任务，
//!     采用了C++ chrono库进行计时，精度达到纳秒级别
//! 定时器的事件循环与任务回调都在子线程中完成，不影响主线程执行流程，
//!     但需要注意任务回调时的多线程问题
//! 该定时器工具在使用上依旧遵从简单便捷的原则
//! == 高精度定时器简介 ==
//!
//!
//! == 使用说明 ==
//! 类声明：
//!     template<class time_level = milliseconds,int time_len = 25,int th_size = 4>
//!     class Ttimer
//!
//! 参数：
//!     time_level : 时间精度
//!     time_len   : 间隔长度
//!     th_size    : 线程数量
//!
//! 类说明：
//!     time_level 和 time_len 是控制事件循环的扫描精度，由C++ chrono库控制，
//!         th_size 参数为线程池创建数量，三个参数都有默认值，
//!         Ttimer类在构造函数即启动事件循环
//!     Ttimer类的任务队列使用链表，按任务触发时间升序排列模拟最小堆，在取任务时有 O1 性能，
//!         但插入时未优化达到 On 级别，如有需求可采用二分法快速插入
//!
//! 快捷使用：
//!     默认创建：
//!         Ttimer<> tr;  //milliseconds精度（毫秒）,25ms间隔，4线程
//!
//!     创建定时任务：
//!         //定时200毫秒 ，无限次触发，重复加入任务队列
//!         tr.add_task(ms(200),[](){
//!             cout<<"hellow world"<<endl;
//!         },-1);
//!
//!         //定时5秒 ，3次触发后移出任务队列
//!         tr.add_task(ss(5),[](){
//!             cout<<"hellow world"<<endl;
//!         },3);
//! == 使用说明 ==
//!
//!
//! == 精度定义 ==
//! C++ chrono库命名：
//!     using nanoseconds	= duration<_GLIBCXX_CHRONO_INT64_T, nano>;
//!     using microseconds	= duration<_GLIBCXX_CHRONO_INT64_T, micro>;
//!     using milliseconds	= duration<_GLIBCXX_CHRONO_INT64_T, milli>;
//!     using seconds	= duration<_GLIBCXX_CHRONO_INT64_T>;
//!     using minutes	= duration<_GLIBCXX_CHRONO_INT64_T, ratio< 60>>;
//!     using hours		= duration<_GLIBCXX_CHRONO_INT64_T, ratio<3600>>;
//!
//! 快速命名：
//!     using nan = nanoseconds;
//!     using mic = microseconds;
//!     using mil = milliseconds;
//!     using sec = seconds;
//!     using minu = minutes;
//!     using hour = hours;
//!     using us = microseconds;
//!     using ms = milliseconds;
//!     using ss = seconds;
//! == 精度定义 ==
//!
//!
//! == 展示顺序 ==
//! 1.使用测试
//! 2.Ttimer.h 文件
//! 3.部分测试结果
//! == 展示顺序 ==
//!
//! 结束语：
//!     定时器在插入时算出时间并时间按顺序插入到容器中，事件循环定时扫描容器的首位数据，
//!         达到时间之后放入线程池中运行
//!     虽然扫描精度可达到纳米级别，但触发时间达不到该精度，出现大量定时任务时，
//!         任务执行定时会被线程池拖慢，需要保证定时任务能快速执行并退出
//!     值得注意的是，定时器提供扫描精度的自定义，扫描精度越低，占用的性能越小，
//!         但执行任务的误差时间就越大，扫描精度与定时时长通常在 50 ~ 100 倍之间
//!
//!
#include <iostream>
#include <unistd.h>

#include "../include/format.h"
#include "../include/ctimel.h"
//#include "../include/Ttimer.h"

using namespace std;

void test_1()
{
    int a = 25;
    double b = 3.14;
    string c = "hellow world !";

    cout<<endl<<"== 常用格式化 == "<<endl;
    {
        string str = sformat("[{0} {1} {2}]")(100,3.1415926,"how are you");
        cout<<str<<endl;
    }
    {
        string str = sformat("[{0} {1} {2}]")(a,b,c);
        cout<<str<<endl;
    }
    {
        string str = sformat("{0}{1}{2}")(a,b,c);
        cout<<str<<endl;
    }
    {
        string str = sformat("{0}--{1}--{2}")(a,b,c);
        cout<<str<<endl;
    }

    cout<<endl<<"== 反复格式化 == "<<endl;
    {
        string str = sformat("{0}--{0}--{0}--{1}--{2}")(a,b,c);
        cout<<str<<endl;
    }
    {
        string str = sformat("({2}) ({2}) ({1}) ({0})")(a,b,c);
        cout<<str<<endl;
    }

    cout<<endl<<"== 性能测试: sformat == "<<endl;
    int count = 1000000;
    {
        ctimel ct;
        for(int i=0;i<count;i++)
        {
            string str = sformat("{0}--{0}--{0}--{1}--{2}")(a,b,c);
        }
        ct.show();
    }

    cout<<endl<<"== 性能测试: sprintf == "<<endl;
    {
        ctimel ct;
        char buf[1024];
        for(int i=0;i<count;i++)
        {
            sprintf(buf,"%d--%d--%d--%f--%s",a,a,a,b,c);
        }
        ct.show();
    }

}

template<class... T>
std::string formatf(const char *fmt, const T&...t)
{
    const auto len = snprintf(nullptr, 0, fmt, t...);
    std::string r;
    r.resize(static_cast<size_t>(len) + 1);
    snprintf(&r.front(), len + 1, fmt, t...);  // Bad boy
    r.resize(static_cast<size_t>(len));
 
    return r;
}

void test_2()
{
    int a = 25;
    double b = 3.14;
    string c = "hellow world !";

    cout<<endl<<"== 常用格式化 == "<<endl;
    {
        string str = fformat("[{} {} {}]")(100,3.1415926,"how are you");
        cout<<str<<endl;
    }
    {
        string str = fformat("[{} {} {}]")(a,b,c);
        cout<<str<<endl;
    }
    {
        string str = fformat("{}{}{}")(a,b,c);
        cout<<str<<endl;
    }
    {
        string str = fformat("{}--{}--{}")(a,b,c);
        cout<<str<<endl;
    }
}

void test_3()
{
    // int count = 50000000;
    int count = 10000000;
    // int count = 1000000;
    // int count = 1;
    {
        ctimel c;
        for(int i=0;i<count;i++)
        {   
            char buf[64];
            sprintf(buf,"<<t1:%dt2:%st3:%ft4:%dt5:%st6:%d>>",i+1,"==",i+3.3,i+4,"==",i+6);
            std::string s(buf);
            // std::cout<<s<<std::endl;
        }
        c.add_point();
        c.show_vec();
    }
    {
        ctimel c;
        for(int i=0;i<count;i++)
        {   
            std::string s = formatf("<<t1:%dt2:%st3:%ft4:%dt5:%st6:%d>>",i+1,"==",i+3.3,i+4,"==",i+6);
            // std::cout<<s<<std::endl;
        }
        c.add_point();
        c.show_vec();
    }
    {
        ctimel c;
        for(int i=0;i<count;i++)
        {   
            std::string s = fformat("<<t1:{}t2:{}t3:{}t4:{}t5:{}t6:{}>>")(i+1,"==",i+3.3,i+4,"==",i+6);
            // std::cout<<s<<std::endl;
        }
        c.add_point();
        c.show_vec();
    }
    {
        ctimel c;
        for(int i=0;i<count;i++)
        {   
            std::string s = sformat("<<t1:{0}t2:{1}t3:{2}t4:{3}t5:{4}t6:{5}>>")(i+1,"==",i+3.3,i+4,"==",i+6);
            // std::cout<<s<<std::endl;
        }
        c.add_point();
        c.show_vec();
    }
}


int main()
{
    cout<<"== begin =="<<endl;

        // test_1();
    //    test_2();
       test_3();

    cout<<"== end =="<<endl;
    return 0;
}
