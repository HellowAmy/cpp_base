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
//! == 调试日志 ==
//!
//! 结束语：
//!     日志模板的无颜色调试下，是cout/paintf打印时间的翻倍，
//!         时间性能测试结果，是三者分别打印一百万条简单日志得出，
//!         日志模板的时间消耗主要是由重载符<<追加数据引起的，
//!         是为了使用便利而牺牲性能
//!
#define VLOG_COLOR  //需要在Tvlog.h文件之前使用
//#define VLOG_CLOSE  //需要在Tvlog.h文件之前使用
#include "Tvlog.h"

#include <vector>
#include <list>
using namespace std;

void test_1()
{
    Tsvlog::get()->set_level(level4::level::e_info); //日志等级

    //普通打印
    vloge("== 普通打印 ==");
    vlogi("e_info level");
    vlogd("this template log");
    vlogd("10+20 ret: " << 10+20);
    vlogw("PI: "<<3.14);
    vloge("2^4 calculate: "<<2*2<<" * "<<2*2<<" = "<<4*4);
    vloge("2^4 calculate:" $1(2*2) $1(2*2) "=" $1(4*4));

    //快速打印结果
    vloge("== 快速打印 ==");
    int count = 0;
    for(int i=0;i<=100;i++) count += i;
    string str = "hello world";
    int ret = 10*60;
    vlogd($1(str) $1(ret) $1(count));

    //容器打印
    vector<string> vec;
    list<string> ls;
    for(int i=0;i<10;i++)
    {
        vec.push_back("vec:"+to_string(i));
        ls.push_back("ls:"+to_string(i));
    }
    vloge("== 容器打印--宏 ==");
    vlogc($v1,ls);                              //打印容器
    vlogc($v2,ls,4);                            //打印容器，带换行
    vlogc($v3,vec,vec.begin()+2,vec.end()-2);   //打印容器，指定迭代器范围
    vlogc($v4,vec,3,"[","]");                   //打印容器，制定分界符

    vloge("== 容器打印--模板 ==");
    {
        vector<int> vec;
        for(int i=0;i<25;i++)
        {
            vec.push_back(i);
        }
        vlogp(vec);
        vlogp(vec,5);
        vlogp(vec,5,"][");
        vlogp(vec.begin(),vec.end(),5);
        vlogp(vec.begin(),vec.end(),5,".");
    }

    int ifor = 1000000;

#if 0
    {
        vts::ctimel c;
        for(int i=0;i<ifor;i++)
        {
            vlogi($1(i) "hellow world");
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            vlogi($1(i) $1(value) $1(s));
        }
        string s2 = c.to_str();
    }
#endif

#if 0
    {
        vts::ctimel c;
        for(int i=0;i<ifor;i++)
        {
            cout<<i<<": hellow world"<<endl;
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            cout<<i<<": value:"<<value<<" | s:"<<s<<endl;
        }
        string s2 = c.to_str();
    }
#endif

#if 0
    {
        vts::ctimel c;
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
    Tsflog::get()->init("Tflog.log",false);
    Tsflog::get()->set_level(level4::level::e_debug);
    Tsflog::get()->set_limit(5);

    //普通打印
    floge("== 普通打印 ==");
    flogi("e_info level");
    flogd("this template log");
    flogd("10+20 ret: " << 10+20);
    flogw("PI: "<<3.14);
    floge("2^4 calculate: "<<2*2<<" * "<<2*2<<" = "<<4*4);
    floge("2^4 calculate:" $1(2*2) $1(2*2) "=" $1(4*4));

    //快速打印结果
    floge("== 快速打印 ==");
    int count = 0;
    for(int i=0;i<=100;i++) count += i;
    string str = "hello world";
    int ret = 10*60;
    flogd($1(str) $1(ret) $1(count));

    int ifor = 1000000;

#if 0
    {
        vts::ctimel c;
        for(int i=0;i<ifor;i++)
        {
            flogw($1(i) "hellow world");
        }
        string s1 = c.to_str();
        int value=100;
        string s = "hellow world";
        for(int i=0;i<ifor;i++)
        {
            flogw($1(i) $1(value) $1(s));
        }
        string s2 = c.to_str();
    }
#endif
}

int main()
{
    printf("== begin ==\n");

    test_1();
//    test_2();

    printf("== end ==\n");
    return 0;
}

/*
 *
 *
//vlog:
//[nan: 7189842803|mic: 7189842|mil: 7189|sec: 7]
//[nan: 7165359528|mic: 7165359|mil: 7165|sec: 7]
//[nan: 7169156501|mic: 7169156|mil: 7169|sec: 7]

//cout:
//[nan: 4834558776|mic: 4834558|mil: 4834|sec: 4]
//[nan: 4898849502|mic: 4898849|mil: 4898|sec: 4]
//[nan: 4793082020|mic: 4793082|mil: 4793|sec: 4]

//print:
//[nan: 4714220664|mic: 4714220|mil: 4714|sec: 4]
//[nan: 4750219105|mic: 4750219|mil: 4750|sec: 4]
//[nan: 4736075373|mic: 4736075|mil: 4736|sec: 4]

*/
