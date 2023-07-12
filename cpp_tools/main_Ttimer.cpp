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

#include "../include/Ttimer.h"

using namespace std;

void test_1()
{
    cout<<"===== test ====="<<endl;
    cout<<"in time_level  : "<<Ttimer<>::time_now().count()<<endl;
    cout<<"in ctime       : "<<(int)time(NULL)<<endl;

    auto fn = [=](){
        static int i=0;
        cout<<endl;
        cout<<"count       : "<<++i<<endl;
        cout<<"thread      : "<<this_thread::get_id()<<endl;
        cout<<"nanoseconds : "<<steady_clock::now().time_since_epoch().count()<<endl;
        cout<<"time_level  : "<<Ttimer<>::time_now().count()<<endl;
        cout<<"ctime       : "<<(int)time(NULL)<<endl;
    };

    //== 单独测试 ==
    int in = 1;
    switch (in) {
    case 1:
    {
        //默认精度 (检查：25ms)(触发：100ms)
        {
            cout<<"== test 1 =="<<endl;
            Ttimer<> tr;
            tr.add_task(ms(100),fn,-1);
            sleep(5);
        }
    }
    break;
    case 2:
    {
        //高精度 (检查：10us)(触发：200us)
        {
            cout<<"== test 2 =="<<endl;
            Ttimer<us,10> tr;
            tr.add_task(us(200),fn,-1);
            sleep(5);
        }
    }
    break;
    case 3:
    {
        //低精度 (检查：200ms)(触发：1s)
        {
            cout<<"== test 3 =="<<endl;
            Ttimer<ms,200> tr;
            tr.add_task(ss(1),fn,-1);
            while(true){ sleep(10); }
        }
    }
    break;
    case 4:
    {
        //短延时任务 (检查：10s)(触发：1minu)
        {
            cout<<"== test 4 =="<<endl;
            Ttimer<ss,10> tr;
            tr.add_task(minu(1),fn,3);
            while(true){ sleep(10); }
        }
    }
    break;
    case 5:
    {
        //长延时任务 (检查：1minu)(触发：1hour)
        {
            cout<<"== test 4 =="<<endl;
            Ttimer<minu,1> tr;
            tr.add_task(hour(1),fn,3);
            while(true){ sleep(10); }
        }
    }
    break;
    }
}


void test_2()
{
    cout<<"===== test_2 ====="<<endl;
    cout<<"in time_level  : "<<Ttimer<>::time_now().count()<<endl;
    cout<<"in ctime       : "<<(int)time(NULL)<<endl;

    auto fn = [=](){
        static int i=0;
        cout<<endl;
        cout<<"count       : "<<++i<<endl;
        cout<<"thread      : "<<this_thread::get_id()<<endl;
        cout<<"nanoseconds : "<<steady_clock::now().time_since_epoch().count()<<endl;
        cout<<"time_level  : "<<Ttimer<>::time_now().count()<<endl;
        cout<<"ctime       : "<<(int)time(NULL)<<endl;
    };

    //== 单独测试 ==
    int in = 1;
    switch (in) {
    case 1:
    {
        //默认方式使用
        {
            cout<<"== test 1 =="<<endl;
            Ttimer<> tr;
            tr.add_task(ms(100),fn,-1);
            sleep(5);
        }
    }
    break;
    case 2:
    {
        //单线程方式使用 (启用两个线程，保证事件循环和回调线程可运行)
        {
            cout<<"== test 2 =="<<endl;
            typedef Ttimer<milliseconds,25,2> Ttimer_sig;

            Ttimer_sig tr;
            tr.add_task(ms(100),fn,-1);
            sleep(5);
        }
    }
    break;
    case 3:
    {
        //取消定时任务与停止定时器
        {
            cout<<"== test 3 =="<<endl;
            Ttimer<> tr;

            auto fn1 = [=](){
                static int i=0;
                cout<<endl;
                cout<<"count       : "<<++i<<endl;
                cout<<"== fn1 =="<<endl;
            };

            auto fn2 = [=](){
                static int i=0;
                cout<<endl;
                cout<<"count       : "<<++i<<endl;
                cout<<"== fn2 =="<<endl;
            };

            int id1 = tr.add_task(ms(500),fn1,-1);
            int id2 = tr.add_task(ss(1),fn2,-1);
            sleep(3);
            cout<<"size: "<<tr.get_task_size()<<endl;

            cout<<"移除任务： id1"<<endl;
            tr.remove_task(id1);
            sleep(2);
            cout<<"size: "<<tr.get_task_size()<<endl;

            cout<<"关闭定时器"<<endl;
            tr.stop_timer();
            sleep(1);
            cout<<"size: "<<tr.get_task_size()<<endl;
        }
    }
    break;
    case 4:
    {
        //大量定时器启动 (最好分配足够的线程处理任务，否则会导致定时任务超时)
        {
            cout<<"== test 4 =="<<endl;
            Ttimer<ms,25,16> tr;    //16线程
            mutex mut;
            int sum = 0;
            auto fn1 = [&](int num){
                lock_guard<mutex> lock(mut);
                sum++;
                static int i=0;
                cout<<endl;
                cout<<"thread      : "<<this_thread::get_id()<<endl;
                cout<<"count       : "<<++i<<endl;
                cout<<"num         : "<<num<<endl;
                cout<<"ctime       : "<<(int)time(NULL)<<endl;
                cout<<"sum: "<<sum<<endl;
            };

            for(int i=0;i<100;i++)
            {
                tr.add_task(ms(200),bind(fn1,i),-1);
                cout<<"=="<<endl;
            }
            sleep(5);
        }
    }
    break;
    }
}


int main()
{
    cout<<"== begin =="<<endl;

//    test_1();
//    test_2();

    cout<<"== end =="<<endl;
    return 0;
}

/*
 * test_1 : 1
 *
== begin ==
===== test =====
in time_level  : 325978552
in ctime       : 1689066461
== test 1 ==

count       : 1
thread      : 139718514865856
nanoseconds : 325978655828058
time_level  : 325978655
ctime       : 1689066461

count       : 2
thread      : 139718506473152
nanoseconds : 325978757588587
time_level  : 325978757
ctime       : 1689066461

count       : 3
thread      : 139718523258560
nanoseconds : 325978859803724
time_level  : 325978859
ctime       : 1689066461
。。。。。。

 *
 * test_1 : 2
 *
== begin ==
===== test =====
in time_level  : 326060331
in ctime       : 1689066543
== test 2 ==

count       : 1
thread      : 140377117058752
nanoseconds : 326060333458621
time_level  : 326060333
ctime       : 1689066543

count       : 2
thread      : 140377125451456
nanoseconds : 326060333770524
time_level  : 326060333
ctime       : 1689066543

count       : 3
thread      : 140377133844160
nanoseconds : 326060334068244
time_level  : 326060334
ctime       : 1689066543
。。。。。。

 *
 * test_1 : 3
 *
== begin ==
===== test =====
in time_level  : 326111485
in ctime       : 1689066594
== test 3 ==

count       : 1
thread      : 140351928170176
nanoseconds : 326112489628221
time_level  : 326112489
ctime       : 1689066595

count       : 2
thread      : 140351919777472
nanoseconds : 326113492024694
time_level  : 326113492
ctime       : 1689066596

count       : 3
thread      : 140351936562880
nanoseconds : 326114495201979
time_level  : 326114495
ctime       : 1689066597
。。。。。。

 *
 * test_1 : 4
 *
== begin ==
===== test =====
in time_level  : 326176446
in ctime       : 1689066659
== test 4 ==

count       : 1
thread      : 140593960482496
nanoseconds : 326236451654969
time_level  : 326236451
ctime       : 1689066719

count       : 2
thread      : 140593952089792
nanoseconds : 326296455636278
time_level  : 326296455
ctime       : 1689066779

count       : 3
thread      : 140593943697088
nanoseconds : 326356461103726
time_level  : 326356461
ctime       : 1689066839
。。。。。。

 *
 * test_1 : 5
 *
== begin ==
===== test =====
in time_level  : 318480718
in ctime       : 1689058719
== test 4 ==

count       : 1
thread      : 139916395837120
nanoseconds : 322080757612433
time_level  : 322080757
ctime       : 1689062563

count       : 2
thread      : 139916387444416
nanoseconds : 325680800607580
time_level  : 325680800
ctime       : 1689066163
。。。。。。
*/
