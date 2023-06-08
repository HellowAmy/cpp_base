#include "../include/queue_th.h"
#include "../include/vector_th.h"
#include "../include/map_th.h"

#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

void test_3()
{
    //基本类型操作测试
    cout<<endl<<"===== queue_th ====="<<endl;
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
    cout<<endl<<"===== vector_th ====="<<endl;
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

void test_5()
{
    cout<<endl<<"===== map_th ====="<<endl;
    int v2=20;
    int v3=30;
    int v5=50;
    int v6=60;

    //插入测试
    map_th<int,int> mm1;
    mm1.insert(pair<int,int>(1,10));
    mm1.insert(pair<int,int>(2,v2));
    mm1.insert(pair<int,int>(3,std::move(v3)));
    mm1.emplace(4,40);
    mm1.emplace(5,v5);
    mm1.emplace(6,std::move(v6));
    mm1.emplace_hint(mm1.begin(),pair<int,int>(7,70));

    for(const auto &a : mm1)
    {
        cout<<a.first<<"|"<<a.second<<endl;
    }
    cout<<mm1.size()<<endl;

    //赋值测试
    map_th<int,int> mm2(mm1);
    map_th<int,int> mm3(std::move(mm1));

    cout<<mm2.size()<<endl;
    cout<<mm3.size()<<endl;
    cout<<mm1.size()<<endl;

    auto it = mm2.begin();
    for(int i=0;i<3;i++) it++;
    mm2.erase(mm2.begin(),it);
    cout<<mm2.size()<<endl;

    //多线程测试
    map_th<int,string> ms1;
    ms1.emplace(11,"ss1");
    ms1.emplace(12,"ss2");
    ms1.emplace(13,"ss3");
    ms1.emplace(14,"ss4");

    atomic<int> count {0};
    auto func = [&](){
        for(int i=100;i<1000;i++)
        {
            count++;
            ms1.emplace(i,"push");
        }
    };

    thread t1(func);
    thread t2(func);
    thread t3(func);
    t1.join();
    t2.join();
    t3.join();

    cout<<ms1.size()<<endl;
    cout<<count<<endl;
}

#include <unistd.h>
void test_6()
{
    //迭代器易位测试
    cout<<endl<<"== 迭代器易位测试 =="<<endl;
    vector_th<string> v1;
    v1.reserve(10);
    v1.push_back("word_1");
    v1.push_back("word_2");
    v1.push_back("word_3");

    //初始化的数据量
    cout<<v1.capacity()<<"|"<<v1.size()<<endl;

    //拿到迭代器,延时时候使用(模拟多线程下被时间切片打断操作)
    auto func1 = [&](){
        auto it1 = v1.begin();
        cout<<"对比点: "<<*it1<<endl;
        sleep(1);   //延时使得多线程下一定被其他线程插入操作
        it1++;
        cout<<"对比点: "<<*it1<<endl; //多线程操作迭代器之后,迭代器易位,打印非法数据
    };

    //往容器添加数据(模拟多线程下迭代器失效)
    auto func2 = [&](){
        v1.insert(v1.begin()+1,"add word"); //其他线程插入操作
    };

    cout<<"单线程操作迭代器"<<endl;
    func1();
    func2();

    cout<<"===== show ====="<<endl;
    for(auto a:v1)
    {
        cout<<a<<endl;
    }

    v1.clear();
    v1.reserve(10);
    v1.push_back("word_1");
    v1.push_back("word_2");
    v1.push_back("word_3");

    cout<<endl<<"多线程操作迭代器"<<endl;
    thread th1(func1);
    thread th2(func2);
    th1.join();
    th2.join();

    cout<<"===== show ====="<<endl;
    for(auto a:v1)
    {
        cout<<a<<endl;
    }

    //多线程添加数据之后的数据量
    cout<<v1.capacity()<<"|"<<v1.size()<<endl;
}

int main()
{
    cout<<"===== begin ====="<<endl;
    test_3();
    test_4();
    test_5();
    test_6();
    cout<<"===== end ====="<<endl;
    return 0;
}

/*
 * 测试结果：
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
