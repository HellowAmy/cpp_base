#include "ctimel.h"
#include "../include/stm.h"

#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <map>
#include <atomic>
using namespace std;

//===== for =====
//功能:直接打印(包括换行)或者提供快捷的for版本
//      提供对象版本和指针版本,后缀_p为指针版本
//for_show : 根据迭代器打印
//for_it : 返回迭代器自行操作
//for_i : 返回类型size_t的变量自行操作
//for_n : 制定执行循环次数,不希望返回迭代器
#define for_show(arr) \
    for(auto it=arr.begin();it!=arr.end();it++) \
    { std::cout<<*it<<std::endl; }

#define for_show_p(arr) \
    for(auto it=arr->begin();it!=arr->end();it++) \
    { std::cout<<*it<<std::endl; }

#define for_it(it,arr) \
    for(auto it=arr.begin();it!=arr.end();it++)

#define for_it_p(it,arr) \
    for(auto it=arr->begin();it!=arr->end();it++)

#define for_i(i,arr) \
    for(size_t i=0;i<arr.size();i++)

#define for_n(n) \
    for(size_t index_it_for=0;index_it_for<n;index_it_for++)
//===== for =====


void func()
{
    for(int i=0;i<10000;i++)
        for(int i=0;i<10000;i++){}
}

void test_1()
{
    cout<<"===== test_1 ====="<<endl;

    auto func = [](){
        for(int i=0;i<10000;i++)
            for(int i=0;i<10000;i++){}
    };

    //创建临时区域，等待退出时打印
    {
        cout<<"===== 一次计算 ====="<<endl;
        ctimel tm;
        func();
    }
    {
        cout<<"===== 更新时间 ====="<<endl;
        ctimel tm;

        func();
        tm.show();
        tm.update();

        func();
        tm.show();
        tm.update();

        func();
        tm.show();
        tm.update();
    }
    {
        cout<<"===== 保存时间点等待未来打印 ====="<<endl;
        ctimel tm;

        func();
        auto s1 = tm.to_point();

        func();
        auto s2 = tm.to_point();

        cout<<"paint: "<<tm.to_string(s1)<<endl;
        cout<<"paint: "<<tm.to_string(s2)<<endl;
    }
    {
        cout<<"===== 多点时间测试 ====="<<endl;
        ctimel tm;

        func();
        tm.add_point();

        func();
        tm.add_point();

        func();
        tm.add_point();

        func();
        tm.add_point();

        tm.show_vec();
    }
    {
        cout<<"===== 多点时间测试并每次重新计时 ====="<<endl;
        ctimel tm;

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        tm.show_vec();
    }
    {
        cout<<"===== 人类可视化时间:ctimes ====="<<endl;
        ctimes ct;
        cout<<ct.to_now()<<endl;
        cout<<ct.to_now("%H:%M:%S")<<endl; //自定义打印格式

        //保留时间点并在未来打印
        time_t t1 = ct.time_now();
        cout<<ct.to_format(t1)<<endl;
    }
}

void test_2()
{
    cout<<"===== stm ====="<<endl;
    string a1 = "1-22-333-4444-55555";
    string a2 = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";

    cout<<"ret = "<<stm(a1)("-",0,2)<<endl;                     //一次切割，1-22-333
    cout<<"ret = "<<stm("1-22-333-4444-55555")("-",0,2)<<endl;  //支持直接赋值
    cout<<"ret = "<<stm(a1)("-",-1,-1)<<endl;                   //切割末尾段，必须反响切割，55555
    cout<<"ret = "<<stm(a1)("-",-3,-1)<<endl;                   //反响切割，333-4444-55555
    cout<<"ret = "<<stm(a2)(" ",1,1,"<<",-1,-1,">>",0,0)<<endl; //两次切割同一字符串，1991-12-3
    cout<<"===== stm =====\n"<<endl;

    cout<<"===== stmv =====\n"<<endl;
    string a = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";
    string b = "--11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";
    string c = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd--";
    string d = "--11--22--33ashgdas pas<<1991-12-3>>da asdg ausd ";
    string e = "ajkwjfajknfjabcwjiabfjawabcabwfiagalkfabc";

    vector<string> vec1 = stmv(a)("--");        //只有一个分割符时
    vector<string> vec2 = stmv(b)("--"," ");    //传入多个分割符时
    vector<string> vec3 = stmv(c)("--"," ","<<");
    vector<string> vec4 = stmv(d)("--"," ","<<",">>");
    vector<string> vec5 = stmv(e)("abc","ab");

    cout<<"1========"<<endl;
    for_show(vec1);

    cout<<"2========"<<endl;
    for_show(vec2);

    cout<<"3========"<<endl;
    for_show(vec3);

    cout<<"4========"<<endl;
    for_show(vec4);

    cout<<"5========"<<endl;
    for_show(vec5);
    cout<<"===== stmv =====\n"<<endl;
}

int main()
{
    cout<<"===== begin ====="<<endl;
    test_1();
//    test_2();

    cout<<"===== end ====="<<endl;
    return 0;
}


/*
 * 多线程容器接口测试
 *

===== begin =====

===== queue_th =====
=====
11
12
13
14
15
=====
s11
s22
s33
s44
s55
s66
s77
=====
600
=====
600
600
600
swap front: 0
swap after: 600

===== vector_th =====
5
5
5
=====
v1
v2
v3
v4
v5
=====
v1
v2
v3
v4
v5
=====
v3
v4
=====
capacity: 8
size: 6
=====
v0
v1
v2
v3
v4
v5
=====
30000

===== map_th =====
1|10
2|20
3|30
4|40
5|50
6|60
7|70
7
7
7
0
4
904
2700

== 迭代器易位测试 ==
10|3
单线程操作迭代器
对比点: word_1
对比点: word_2
===== show =====
word_1
add word
word_2
word_3

多线程操作迭代器
对比点: word_1
对比点: add word
===== show =====
word_1
add word
word_2
word_3
10|4
===== end =====

*/


/*
 * 计算函数时间测试结果：
 *
===== begin =====
===== test_1 =====
===== 一次计算 =====
[nan: 288974927|mic: 288974|mil: 288|sec: 0]
===== 更新时间 =====
[nan: 291067824|mic: 291067|mil: 291|sec: 0]
[nan: 287929075|mic: 287929|mil: 287|sec: 0]
[nan: 287614887|mic: 287614|mil: 287|sec: 0]
[nan: 866809334|mic: 866809|mil: 866|sec: 0]
===== 保存时间点等待未来打印 =====
paint: [nan: 288384552|mic: 288384|mil: 288|sec: 0]
paint: [nan: 576289512|mic: 576289|mil: 576|sec: 0]
[nan: 576362262|mic: 576362|mil: 576|sec: 0]
===== 多点时间测试 =====
[nan: 304235552|mic: 304235|mil: 304|sec: 0]
[nan: 591549148|mic: 591549|mil: 591|sec: 0]
[nan: 878842238|mic: 878842|mil: 878|sec: 0]
[nan: 1166150134|mic: 1166150|mil: 1166|sec: 1]
[nan: 1166241424|mic: 1166241|mil: 1166|sec: 1]
===== 多点时间测试并每次重新计时 =====
[nan: 287540339|mic: 287540|mil: 287|sec: 0]
[nan: 286082889|mic: 286082|mil: 286|sec: 0]
[nan: 286608348|mic: 286608|mil: 286|sec: 0]
[nan: 286342874|mic: 286342|mil: 286|sec: 0]
[nan: 1146695529|mic: 1146695|mil: 1146|sec: 1]
===== 人类可视化时间:ctimes =====
2023-06-08 21:01:01
21:01:01
2023-06-08 21:01:01
===== end =====
*/
