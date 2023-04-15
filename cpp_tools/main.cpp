#include "ctimel.h"
#include "stm.h"
#include "queue_th.h"
#include "vector_th.h"

#include <queue>
#include <vector>
#include <iostream>
#include <thread>
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

    cout<<"===== test_1 ====="<<endl;
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


void test_3()
{
    //基本类型操作测试
    cout<<"===== queue_th ====="<<endl;
    queue_th<int> thq1;
    thq1.push(11);
    thq1.push(12);
    thq1.push(13);
    thq1.push(14);
    thq1.push(15);

    queue_th<int> thq2(thq1);
    queue_th<int> thq3;
    thq3 = std::move(thq2);

    cout<<"====="<<endl;
    while(thq3.empty() == false)
    {
        cout<<thq3.front()<<endl;
        thq3.pop();
    }

    //类操作测试
    queue_th<string> th1;
    th1.push("s11");
    th1.push("s22");
    th1.push("s33");
    th1.push("s44");
    string s55 = "s55";
    th1.push(s55);
    th1.emplace("s66");
    string s77 = "s77";
    th1.emplace(s77);

    queue_th<string> th2(std::move(th1));
    queue_th<string> th3;
    th3 = th2;

    cout<<"====="<<endl;
    while(th2.size() > 0)
    {
        cout<<th2.front()<<endl;
        th2.pop();
    }

    //多线程测试
    cout<<"====="<<endl;
    queue_th<string> queue_th_1;
    auto func_th = [&](){
        for(int i=0;i<100;i++)
        {
            string str = "pp: " + to_string(i);
            queue_th_1.push(str);
            queue_th_1.push(std::move(str));
        }
    };

    thread thread_1(func_th);
    thread thread_2(func_th);
    thread thread_3(func_th);
    thread_1.join();
    thread_2.join();
    thread_3.join();
    cout<<queue_th_1.size()<<endl;

    //容器赋值与交换测试
    queue_th<string> queue_th_2(queue_th_1);
    queue_th<string> queue_th_3;
    queue_th_3 = queue_th_1;

    queue_th<string> queue_th_tm(queue_th_1);
    queue_th<string> queue_th_4(std::move(queue_th_tm));

    cout<<"====="<<endl;
    cout<<queue_th_2.size()<<endl;
    cout<<queue_th_3.size()<<endl;
    cout<<queue_th_4.size()<<endl;

    queue_th<string> queue_th_null;
    cout<<"swap front: "<<queue_th_null.size()<<endl;

    queue_th_null.swap(queue_th_4);
    cout<<"swap after: "<<queue_th_null.size()<<endl;
}

void test_4()
{
    vector_th<string> v1;
    v1.push_back("v1");
    v1.push_back("v2");
    string vv3 = "v3";
    v1.push_back(vv3);
    string vv4 = "v4";
    v1.push_back(std::move(vv4));
    v1.emplace_back("v5");

    vector_th<string> v2(v1);
    vector_th<string> vtm(v1);
    vector_th<string> v3(std::move(vtm));

    cout<<v1.size()<<endl;
    cout<<v2.size()<<endl;
    cout<<v3.size()<<endl;

    cout<<"====="<<endl;
    for(auto a:v2)
    {
        cout<<a<<endl;
    }

    cout<<"====="<<endl;
    for(auto it = v3.begin();it!=v3.end();it++)
    {
        cout<<*it<<endl;
    }

    cout<<"====="<<endl;
    cout<<v3.at(2)<<endl;
    cout<<v3[3]<<endl;

    cout<<"====="<<endl;
    v1.emplace(v1.begin(),"v0");
    cout<<"capacity: "<<v1.capacity()<<endl;
    cout<<"size: "<<v1.size()<<endl;

    cout<<"====="<<endl;
    for(auto it = v1.begin();it!=v1.end();it++)
    {
        cout<<*it<<endl;
    }

    cout<<"====="<<endl;
    vector_th<int> veci1;
    auto func_th = [&](){
        for(int i=0;i<10000;i++)
        {
            veci1.push_back(i);
        }
    };

    thread thread_1(func_th);
    thread thread_2(func_th);
    thread thread_3(func_th);
    thread_1.join();
    thread_2.join();
    thread_3.join();
    cout<<veci1.size()<<endl;
}

int main()
{
    cout<<"===== begin ====="<<endl;
//    test_1();
//    test_2();
//    test_3();
    test_4();
    cout<<"===== end ====="<<endl;
    return 0;
}
