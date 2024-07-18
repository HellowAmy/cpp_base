//!
//! C++工具：C++日志模板--提供调试日志和文件日志
//!
//! == 日志模板简介 ==
//! 日志打印是C++程序调试的关键一环，可惜C++并没有提供标准库日志，
//!     本文将提供一个简单轻便的调试日志，使用方式更接近C++风格
//! 日志模板包含控制台调试日志，写入文件日志两种常见日志，
//!     通常调试日志不会跟随程序发行，发行时采用文件日志
//! 该日志在Linux环境下适用，但基本采用C++标准，
//!     如需要在Windows下使用需微调代码
//! == 日志模板简介 ==
//!
//!
//! == 调试日志 ==
//! 调试日志：提供日志开关宏，控制整体日志的打印输出
//!         提供日志级别，可屏蔽低级日志
//!         提供日志级别模板，可自定义日志级别
//!         提供颜色打印开关，控制不同级别高亮打印
//!         提供标准容器打印模板，快速打印容器内容
//!
//! 文件日志：提供日志级别，可屏蔽低级日志
//!         提供日志级别模板，可自定义日志级别
//!         提供文件限制最大长度，文件数量循环复写模式
//!         提供无限文件数量追加写入模式
//! == 调试日志 ==
//!
//! 结束语：
//!     从性能测试结果来看，调试日志的颜色打印与常规打印性能无差距，
//!         调试日志的时间消耗是cout/paintf打印时间的一倍，
//!         时间消耗主要由重载符<<产生，为便捷性牺牲速度
//!     文件日志写入文件的性能比cout/paintf打印到屏幕更快，
//!         文件日志不错的性能与便捷性，可以在简单项目中快速使用
//!
#define VLOG_COLOR  //需要在Tvlog.h文件之前使用
//#define VLOG_CLOSE  //需要在Tvlog.h文件之前使用
#include "../include/Tvlog.h"
#include "../include/format.h"
#include <vector>
#include <list>

#include <chrono>
#include <iostream>
#include <string>
#include <cstring>
using std::string;
using std::vector;
using std::list;
using std::to_string;
using namespace std::chrono;

//!
//! 说明：主要用于计算函数运行的时间，测试函数性能
//!         steady_clock时间：精确到纳秒的均速时间
//!
//! 例子：
//!     {
//!         ctimel tm;
//!         func();
//!     }//到这里析构退出并打印时间
//!
//! 原理：构建对象时开始计时，析构时打印时间
//!
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

    inline void show_str(const string str) { std::cout<<str<<std::endl; }
    inline void show() { show_str(to_str()); }
    inline void update() { _begin = steady_clock::now(); }

protected:
    time_point<steady_clock,nanoseconds> _begin;
};


void test_1()
{
    Tvlogs::get()->set_level(vlevel4::e_info);    //设置最低显示日志级别

    //普通打印
    vloge("== 普通打印 ==");
    vlogi("e_info level");
    vlogd("this template log");
    vlogd("10+20 ret: " << 10+20);
    vlogw("PI: "<<3.14);
    vloge("2^4 calculate: "<<2*2<<" * "<<2*2<<" = "<<4*4);
    vloge("2^4 calculate:" $(2*2) $(2*2) "=" $(4*4));

    //快速打印结果
    vloge("== 快速打印 ==");
    int count = 0;
    for(int i=0;i<=100;i++) count += i;
    string str = "hello world";
    int ret = 10*60;
    vlogd($(str) $(ret) $(count));

    //容器快速打印
    vector<string> vec;
    list<string> ls;
    for(int i=0;i<10;i++)
    {
        vec.push_back("vec:"+to_string(i));
        ls.push_back("ls:"+to_string(i));
    }

    vloge("== 容器打印--模板 ==");
    {
        vector<int> vec;
        for(int i=0;i<25;i++)
        {
            vec.push_back(i);
        }
        vlogc(vec);
        vlogc(vec,5);
        vlogc(vec,5,"][");
        vlogc(vec.begin(),vec.end(),5);
        vlogc(vec.begin(),vec.end(),5,".");
    }

    int ifor = 3;

#if 1
    {
        ctimel c;
        for(int i=0;i<ifor;i++)
        {
            vlogd($(i) "hellow world");
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            vlogd($(i) $(value) $(s));
        }
        string s2 = c.to_str();
    }
#endif

#if 1
    {
        ctimel c;
        for(int i=0;i<ifor;i++)
        {
            std::cout<<i<<": hellow world"<<std::endl;
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            std::cout<<i<<": value:"<<value<<" | s:"<<s<<std::endl;
        }
        string s2 = c.to_str();
    }
#endif

#if 1
    {
        ctimel c;
        for(int i=0;i<ifor;i++)
        {
            printf("%d: hellow world\n",i);
        }
        string s1 = c.to_str();
        int value=100;
        const char *s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            printf("%d: value: %d| s: %s\n",i,value,s);
        }
        string s2 = c.to_str();
    }
#endif

}

void test_2()
{
    Tflogs::get()->init("Tflog.log",false);     //设置日志文件和追加模式
    Tflogs::get()->set_level(vlevel4::e_debug); //设置最低显示日志级别
    Tflogs::get()->set_limit(5);                //设置日志数量（启动循环覆盖）--不设置则默认无限制
    Tflogs::get()->set_length(1024*1024*10);    //设置最大日志长度（10M）--不设置默认64M

    //普通打印
    floge("== 普通打印 ==");
    flogi("e_info level");
    flogd("this template log");
    flogd("10+20 ret: " << 10+20);
    flogw("PI: "<<3.14);
    floge("2^4 calculate: "<<2*2<<" * "<<2*2<<" = "<<4*4);
    floge("2^4 calculate:" $(2*2) $(2*2) "=" $(4*4));

    //快速打印结果
    floge("== 快速打印 ==");
    int count = 0;
    for(int i=0;i<=100;i++) count += i;
    string str = "hello world";
    int ret = 10*60;
    flogd($(str) $(ret) $(count));

    int ifor = 3000000;

#if 1
    {
        ctimel c;
        for(int i=0;i<ifor;i++)
        {
            flogw($(i) "hellow world");
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            flogw($(i) $(value) $(s));
        }
        string s2 = c.to_str();
        std::cout<<"=======: "<<s1<<std::endl;
        std::cout<<"=======: "<<s2<<std::endl;
    }
#endif
}

void test_3()
{
    Talogs::get()->init("Talog.log",false);     //设置日志文件和追加模式
    Talogs::get()->set_level(vlevel4::e_debug); //设置最低显示日志级别
    Talogs::get()->set_limit(5);                //设置日志数量（启动循环覆盖）--不设置则默认无限制
    Talogs::get()->set_length(1024*1024*10);    //设置最大日志长度（10M）--不设置默认64M

    //普通打印
    aloge("== 普通打印 ==");
    alogi("e_info level");
    alogd("this template log");
    alogd("10+20 ret: " << 10+20);
    alogw("PI: "<<3.14);
    aloge("2^4 calculate: "<<2*2<<" * "<<2*2<<" = "<<4*4);
    aloge("2^4 calculate:" $(2*2) $(2*2) "=" $(4*4));

    //快速打印结果
    aloge("== 快速打印 ==");
    int count = 0;
    for(int i=0;i<=100;i++) count += i;
    string str = "hello world";
    int ret = 10*60;
    alogd($(str) $(ret) $(count));

    int ifor = 3000000;

#if 1
    {
        ctimel c;
        for(int i=0;i<ifor;i++)
        {
            alogw($(i) "hellow world");
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            alogw($(i) $(value) $(s));
        }
        string s2 = c.to_str();
        std::cout<<"=======: "<<s1<<std::endl;
        std::cout<<"=======: "<<s2<<std::endl;
    }
#endif
}


class trs
{
public:
    template<class T>
    inline trs& operator<<(const T &txt)
    { _buf += Tto_string(txt); return *this; };

    inline trs& operator<<(std::ostream& (*end)(std::ostream&))
    { std::cout<<_buf<<std::endl; _buf = ""; return *this; };

    inline void show_buf(const std::string &buf)
    {
        std::cout<<buf<<std::endl;
    }

    std::string _buf;
};

void test_4()
{
    int count = 10000000;

    // {
    //     ctimel c;
    //     trs ts;
    //     for(int i=0;i<count;i++)
    //     {
    //         ts<<"t1:"<<i+1<<"t2:"<<i+2<<"t3:"<<i+3<<"t4:"<<i+4<<"t5:"<<i+5<<"t6:"<<i+6<<std::endl;
    //     }
    //     std::cout<<"count1: "<<c.to_str()<<std::endl; 
    // }

    {
        ctimel c;
        trs ts;
        char buf[64];
        for(int i=0;i<count;i++)
        {   
            memset(buf,0,sizeof(buf));
            sprintf(buf,"t1:%dt2:%dt3:%dt4:%dt5:%dt6:%d",i+1,i+2,i+3,i+4,i+5,i+6);
            std::string s(buf);
        }
        std::cout<<"count1: "<<c.to_str()<<std::endl; 
    }
    {
        ctimel c;
        trs ts;
        for(int i=0;i<count;i++)
        {   
            std::string s = sformat("t1:{0}t2:{1}t3:{2}t4:{3}t5:{4}t6:{5}")(i+1,i+2,i+3,i+4,i+5,i+6);
            // ts.show_buf(sformat("t1:{0}t2:{1}t3:{2}t4:{3}t5:{4}t6:{5}")(i+1,i+2,i+3,i+4,i+5,i+6));
        }
        std::cout<<"count2: "<<c.to_str()<<std::endl; 
    }
    


}

int main()
{
    printf("== begin ==\n");

    // test_1();
    // test_2();
    // test_3();
    test_4();

    printf("== end ==\n");
    return 0;
}

/*
 * 测试结果：各个版本的运行时间
 *
//flog:
[nan: 3999236206|mic: 3999236|mil: 3999|sec: 3]
[nan: 3981404537|mic: 3981404|mil: 3981|sec: 3]
[nan: 3943364553|mic: 3943364|mil: 3943|sec: 3]

//vlog-color:
[nan: 9553316241|mic: 9553316|mil: 9553|sec: 9]
[nan: 9509730052|mic: 9509730|mil: 9509|sec: 9]
[nan: 9444099080|mic: 9444099|mil: 9444|sec: 9]

//vlog:
[nan: 9377207274|mic: 9377207|mil: 9377|sec: 9]
[nan: 9237833979|mic: 9237833|mil: 9237|sec: 9]
[nan: 9389804775|mic: 9389804|mil: 9389|sec: 9]

//cout:
[nan: 5033779586|mic: 5033779|mil: 5033|sec: 5]
[nan: 5081468378|mic: 5081468|mil: 5081|sec: 5]
[nan: 5098394323|mic: 5098394|mil: 5098|sec: 5]

//print:
[nan: 4815928537|mic: 4815928|mil: 4815|sec: 4]
[nan: 4650109190|mic: 4650109|mil: 4650|sec: 4]
[nan: 4881491074|mic: 4881491|mil: 4881|sec: 4]


 *
 * 测试结果：各个版本的打印格式
 *
//flog:
[2023-06-07 22:30:44] [War] <<<< [i: 0] hellow world [../cpp_tools/main_Tvlog.cpp:<227>]
[2023-06-07 22:30:44] [War] <<<< [i: 1] hellow world [../cpp_tools/main_Tvlog.cpp:<227>]
[2023-06-07 22:30:44] [War] <<<< [i: 2] hellow world [../cpp_tools/main_Tvlog.cpp:<227>]
[2023-06-07 22:30:44] [War] <<<< [i: 0] [value: 100] [s: hellow world]  [../cpp_tools/main_Tvlog.cpp:<234>]
[2023-06-07 22:30:44] [War] <<<< [i: 1] [value: 100] [s: hellow world]  [../cpp_tools/main_Tvlog.cpp:<234>]
[2023-06-07 22:30:44] [War] <<<< [i: 2] [value: 100] [s: hellow world]  [../cpp_tools/main_Tvlog.cpp:<234>]

//vlog:
[Deb][../cpp_tools/main_Tvlog.cpp:<146>] <<<< [i: 0] hellow world
[Deb][../cpp_tools/main_Tvlog.cpp:<146>] <<<< [i: 1] hellow world
[Deb][../cpp_tools/main_Tvlog.cpp:<146>] <<<< [i: 2] hellow world
[Deb][../cpp_tools/main_Tvlog.cpp:<153>] <<<< [i: 0] [value: 100] [s: hellow world]
[Deb][../cpp_tools/main_Tvlog.cpp:<153>] <<<< [i: 1] [value: 100] [s: hellow world]
[Deb][../cpp_tools/main_Tvlog.cpp:<153>] <<<< [i: 2] [value: 100] [s: hellow world]

//cout:
0: hellow world
1: hellow world
2: hellow world
0: value:100 | s:hellow world
1: value:100 | s:hellow world
2: value:100 | s:hellow world

//print:
0: hellow world
1: hellow world
2: hellow world
0: value: 100| s: hellow world
1: value: 100| s: hellow world
2: value: 100| s: hellow world


 *
 * 测试结果：test_1函数的打印结果
 *
== begin ==
[Err][../cpp_tools/main_Tvlog.cpp:<95>] <<<< == 普通打印 ==
[Inf][../cpp_tools/main_Tvlog.cpp:<96>] <<<< e_info level
[Deb][../cpp_tools/main_Tvlog.cpp:<97>] <<<< this template log
[Deb][../cpp_tools/main_Tvlog.cpp:<98>] <<<< 10+20 ret: 30
[War][../cpp_tools/main_Tvlog.cpp:<99>] <<<< PI: 3.14
[Err][../cpp_tools/main_Tvlog.cpp:<100>] <<<< 2^4 calculate: 4 * 4 = 16
[Err][../cpp_tools/main_Tvlog.cpp:<101>] <<<< 2^4 calculate:[2*2: 4] [2*2: 4] =[4*4: 16]
[Err][../cpp_tools/main_Tvlog.cpp:<104>] <<<< == 快速打印 ==
[Deb][../cpp_tools/main_Tvlog.cpp:<109>] <<<< [str: hello world] [ret: 600] [count: 5050]
[Err][../cpp_tools/main_Tvlog.cpp:<125>] <<<< == 容器打印--模板 ==
| [../cpp_tools/main_Tvlog.cpp:<132>]
| size: 25
| 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
| [../cpp_tools/main_Tvlog.cpp:<133>]
| size: 25
| 0 1 2 3 4
| 5 6 7 8 9
| 10 11 12 13 14
| 15 16 17 18 19
| 20 21 22 23 24
| [../cpp_tools/main_Tvlog.cpp:<134>]
| size: 25
| 0][1][2][3][4][
| 5][6][7][8][9][
| 10][11][12][13][14][
| 15][16][17][18][19][
| 20][21][22][23][24][
| [../cpp_tools/main_Tvlog.cpp:<135>]
| size: 25
| 0 1 2 3 4
| 5 6 7 8 9
| 10 11 12 13 14
| 15 16 17 18 19
| 20 21 22 23 24
| [../cpp_tools/main_Tvlog.cpp:<136>]
| size: 25
| 0.1.2.3.4.
| 5.6.7.8.9.
| 10.11.12.13.14.
| 15.16.17.18.19.
| 20.21.22.23.24.
== end ==

 *
 * 测试结果：test_2函数的打印结果
 *
[2023-06-07 22:34:36] [Err] <<<< == 普通打印 == [../cpp_tools/main_Tvlog.cpp:<204>]
[2023-06-07 22:34:36] [Deb] <<<< this template log [../cpp_tools/main_Tvlog.cpp:<206>]
[2023-06-07 22:34:36] [Deb] <<<< 10+20 ret: 30 [../cpp_tools/main_Tvlog.cpp:<207>]
[2023-06-07 22:34:36] [War] <<<< PI: 3.14 [../cpp_tools/main_Tvlog.cpp:<208>]
[2023-06-07 22:34:36] [Err] <<<< 2^4 calculate: 4 * 4 = 16 [../cpp_tools/main_Tvlog.cpp:<209>]
[2023-06-07 22:34:36] [Err] <<<< 2^4 calculate:[2*2: 4] [2*2: 4] =[4*4: 16]  [../cpp_tools/main_Tvlog.cpp:<210>]
[2023-06-07 22:34:36] [Err] <<<< == 快速打印 == [../cpp_tools/main_Tvlog.cpp:<213>]
[2023-06-07 22:34:36] [Deb] <<<< [str: hello world] [ret: 600] [count: 5050]  [../cpp_tools/main_Tvlog.cpp:<218>]
*/
