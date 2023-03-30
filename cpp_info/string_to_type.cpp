//!
//! C++代码详解: string的赋值与C风格字符串转换
//!
//! ===== string简介 =====
//! string是C++里默认的字符串容器，用于代替C风格的字符串指针与字符串数组，
//!     不过C++字符串有时需要与C风格代码转换，
//!     本文简单介绍string的几种转换方法与运用场景
//! ===== string简介 =====
//!
//!
//! ===== 本章内容 =====
//!     1、string与C风格字符串相互转换，主要采用string构造函数完成
//!     2、string与结构体相互转换，主要用于传输网络
//!     3、string与基本类型相互转换，主要用于string与数字互转
//!     4、string与各类型拼接，主要用于格式化不同类型为字符串拼接
//! ===== 本章内容 =====
//!
//!
//! ===== 格式化转字符串拼接 =====
//! 目前C++中string类并没有类似cout那种<<左移运算符拼接的重载，
//!     也没有类型.arg(T)等可以链式编程的函数接口，
//!     如果想将各种类型转为字符串拼接到string中，
//!     需要借助snprintf函数将字符格式化到char[]中，
//!     再由char[]转为string
//! ===== 格式化转字符串拼接 =====
//!
//!
//! ===== 类型转换特点 =====
//! 字符串类型转换ostringstream/istringstream特点：
//!     如C函数转换stoi、stoll等函数，转换失败会抛异常，
//!         而C++的转换类转换失败时不会抛异常，而是返回0，
//!         如果关心是否转换失败，可以提前赋值，或者采用C函数方法
//! ===== 类型转换特点 =====
//!
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;


//== 字符串类型转换 ==
template<typename T>
string to_string(const T& t){ ostringstream os; os<<t; return os.str(); }

template<typename T>
T from_string(const string& str){ T t; istringstream iss(str); iss>>t; return t; }
//== 字符串类型转换 ==


//===== 结构体转换string函数 =====
//结构体转string
//      语法解析：(char*)&ct ，由&ct获取结构体地址，在由该地址(char*)转为char*类型的指针
//      根据string构造函数，参数1：char*地址，参数2：长度，可从地址内存中复制二进制内容
template <class T_ct>
static string ct_s(T_ct ct)
{ return string((char*)&ct,sizeof(T_ct)); }

//string转结构体
//      语法解析：*(T_ct*)str.c_str() ，由str.c_str()从string类获取const char*指针，
//      由const char*指针转为T_ct*指针，再*（T_ct*）从指针中获取值，从而返回值
template <class T_ct>
static T_ct st_c(const string &str)
{ return *(T_ct*)str.c_str(); }
//===== 结构体转换string函数 =====



//方位结构体
struct dire
{
    int x;
    int y;
    int z;
};

//定长消息包
struct msg
{
    bool flg;
    bool is_ssh;
    size_t now_size;
    size_t buf_size;
    char buf[2048];
};

void show_dire(dire di)
{ cout<<"["<<di.x<<"|"<<di.y<<"|"<<di.z<<"]"<<endl; }

void show_msg(msg ms)
{ cout<<"["<<ms.flg<<"|"<<ms.is_ssh<<"|"<<ms.now_size<<"|"<<ms.buf_size<<"|"<<ms.buf<<"]"<<endl; }



void test()
{
    cout<<endl<<"== string构造函数运用：C风格字符数组转string =="<<endl;

    //生成一个字符串数组
    char buf1[] = {'1','2','3','4','5','\0','a','b','c','d','e'};
    cout<<"buf1: "<<buf1<<" | "<<sizeof(buf1)<<endl;

    //从首地址截取字符串，遇0停止
    string str1(buf1);
    cout<<"str1: "<<str1<<" | "<<str1.size()<<endl;

    //从首地址开始，到整个字符串长度（全部获取）
    //  （参数2控制截取长度，多用于从二进制buf转string的复制）
    string str2(buf1,sizeof(buf1));
    cout<<"str2: "<<str2<<" | "<<str2.size()<<endl;

    //从下标0开始，遇0停止
    string str3(buf1,0,8);
    cout<<"str3: "<<str3<<" | "<<str3.size()<<endl;

    //从下标3开始，遇0停止
    string str4(buf1,3,5);
    cout<<"str4: "<<str4<<" | "<<str4.size()<<endl;



    cout<<endl<<"== string转C风格字符数组 =="<<endl;

    //初始化string字符串
    string sstr1 = "12345ABCDEabcde";
    cout<<"sstr1: "<<sstr1<<" | "<<sstr1.size()<<endl;

    //string转const char * 常量指针
    const char *bbuf1 = sstr1.c_str();
    cout<<"bbuf1: "<<bbuf1<<" | "<<sizeof(bbuf1)<<endl; //长度为指针长度

    //string转char * 变量指针（两种情况，malloc或者强转（char*））
    //malloc方式：
    char *bbuf2 = (char*)malloc(sstr1.size()+1); //加末尾符，否则可能遇到无\0结束符溢出情况
    memset(bbuf2,0,sstr1.size()+1);
    strncpy(bbuf2,sstr1.c_str(),sstr1.size());
    cout<<"bbuf2: "<<bbuf2<<" | "<<sizeof(bbuf2)<<endl; //长度为指针长度

    //强转（char*）
    char *bbuf3 = (char*)sstr1.c_str();
    cout<<"bbuf3: "<<bbuf2<<" | "<<sizeof(bbuf3)<<endl; //长度为指针长度

    //string转char[] 数组
    char bbuf4[sstr1.size()+1]; //加末尾符，否则可能遇到无\0结束符溢出情况
    memset(bbuf4,0,sstr1.size()+1);
    strncpy(bbuf4,sstr1.c_str(),sstr1.size());
    cout<<"bbuf3: "<<bbuf2<<" | "<<sizeof(bbuf4)<<endl;



    cout<<endl<<"== string转结构体 =="<<endl;
    cout<<"转换前: "<<endl;
    dire di1 {10,20,40};
    show_dire(di1);

    //结构体转为string
    string sdi1((char*)&di1,sizeof(di1));

    //解析：dire di2 = *(dire*)sdi1.c_str();
    //  等效：dire *di2 = (dire*)sdi1.c_str();
    //          dire di3 = *di2;
    //
    //string转回结构体
    cout<<"转换后: "<<endl;
    dire di2 = *(dire*)sdi1.c_str();
    show_dire(di2);

    //初始化结构体
    msg ms1;
    ms1.flg = false;
    ms1.is_ssh = true;
    ms1.buf_size = sizeof(ms1.buf);
    string tm = "hellow world";
    strncpy(ms1.buf,tm.c_str(),tm.size());
    ms1.now_size = tm.size();

    cout<<endl<<"转换前: "<<endl;
    show_msg(ms1);

    //模拟网络传输。。。 （结构体转为string -> 发送到网络 ->将string转回结构体）
    string strms1 = ct_s(ms1); //结构体转为string
    // ... 网络传输
    msg ms2 = st_c<msg>(strms1); //string转为结构体

    cout<<"转换后: "<<endl;
    show_msg(ms2);



    cout<<endl<<"== 基本类型与string互相转换 =="<<endl;
    int i = 11;
    size_t il = 1999;
    char c = 'f';

    cout<<"i 转 string: "<<to_string(i)<<endl;
    cout<<"size_t 转 string: "<<to_string(il)<<endl;
    cout<<"char 转 string: "<<to_string(c)<<endl;
    cout<<endl;

    string strto1 = "100";
    cout<<"string 转 int: "<<from_string<int>(strto1)<<endl;
    cout<<"string 转 size_t: "<<from_string<size_t>(strto1)<<endl;
    cout<<"string 转 c: "<<from_string<char>(strto1)<<endl;

    //结果：
    //  当string转数字时，首歌字符不为数组时立刻返回0,不会抛出异常，
    //      当string转char时，拿到第一个字母赋值给char
    cout<<endl<<"== 无数字情况 =="<<endl;
    string strto2 = "sgsdg";
    cout<<"string 转 int: "<<from_string<int>(strto2)<<endl;
    cout<<"string 转 size_t: "<<from_string<size_t>(strto2)<<endl;
    cout<<"string 转 c: "<<from_string<char>(strto2)<<endl;

    cout<<endl<<"== 中间掺杂数字情况 =="<<endl;
    string strto3 = "sg4235sdg";
    cout<<"string 转 int: "<<from_string<int>(strto3)<<endl;
    cout<<"string 转 size_t: "<<from_string<size_t>(strto3)<<endl;
    cout<<"string 转 c: "<<from_string<char>(strto3)<<endl;

    cout<<endl<<"== 格式化转字符串拼接 =="<<endl;
    string strto4;
    char buf[128]{0};
    int i4 = 95;
    const char *class4 = "超级一班";
    snprintf(buf,sizeof(buf),"小明的成绩：%d,小明的班级：%s \n",i4,class4);

    strto4.append(buf,strlen(buf)); //追加字符串到string
    cout<<"格式化转字符串拼接: "<<strto4<<"size: "<<strto4.size()<<endl;
}


//== windows中文显示 ==
#ifdef _WIN32
#include <windows.h>
#endif

int m_main()
{
    //== Windows控制台: UTF-8编码 ==
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    //== Windows控制台: UTF-8编码 ==

    cout<<"== begin =="<<endl;

    test();

    cout<<"== end =="<<endl;
    return 0;
}

int main()
{
    m_main();
    return 0;
}

/*
运行结果：
== begin ==

== string构造函数运用：C风格字符数组转string ==
buf1: 12345 | 11
str1: 12345 | 5
str2: 12345 abcde | 11
str3: 12345 | 5
str4: 45 | 2

== string转C风格字符数组 ==
sstr1: 12345ABCDEabcde | 15
bbuf1: 12345ABCDEabcde | 8
bbuf2: 12345ABCDEabcde | 8
bbuf3: 12345ABCDEabcde | 8
bbuf3: 12345ABCDEabcde | 16

== string转结构体 ==
转换前:
[10|20|40]
转换后:
[10|20|40]

转换前:
[0|1|12|2048|hellow world]
转换后:
[0|1|12|2048|hellow world]

== 基本类型与string互相转换 ==
i 转 string: 11
size_t 转 string: 1999
char 转 string: f

string 转 int: 100
string 转 size_t: 100
string 转 c: 1

== 无数字情况 ==
string 转 int: 0
string 转 size_t: 0
string 转 c: s

== 中间掺杂数字情况 ==
string 转 int: 0
string 转 size_t: 0
string 转 c: s

== 格式化转字符串拼接 ==
格式化转字符串拼接: 小明的成绩：95,小明的班级：超级一班
size: 53
== end ==

*/
