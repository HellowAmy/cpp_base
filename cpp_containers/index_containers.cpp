//!
//! C++容器: 索引容器[map class - set class]
//!
//! == 索引容器简介 ==
//! 本章讲解的是C++ STL中的索引容器，所谓索引容器就容器通过key的形式快速定位内容，
//!     不管是map的[key-value]模式还是set的单[key]模式都是通过索引的方式快速定位，
//!     索引容器在查找速度上有着天然优势，几乎不会被数据的数量持续增大而直线下滑
//!
//! 本章列举C++11中已有的索引容器，其中包括：
//!     map类：[key-value]
//!         map                 : 经典map容器，红黑树底层，key不可重复
//!         multimap            : 多索引map，红黑树底层，可重复key
//!         unordered_map       : hash map，hash结构，key不可重复
//!         unordered_multimap  : 多索引hash map，hash结构，可重复key
//!
//!     set类：[key]
//!         set                 : 经典set容器，红黑树底层，key不可重复
//!         multiset            : 多索引set，红黑树底层，可重复key
//!         unordered_set       : hash set，hash结构，key不可重复
//!         unordered_multiset  : 多索引hash set，hash结构，可重复key
//!
//!     红黑树特点: 所有操作的时间上限是O(log n)，保证操作的时间稳定在一个值之内，主打稳定
//!     哈希表特点: 更快的查找，操作时间上限为O(1)常数级，但是存在O(n)的最坏情况，偶尔慢速
//! == 索引容器简介 ==
//!
//!
//! == map ==
//! 模板声明：
//!     template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
//!     typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
//!     class map
//! == map ==
//!
//!



#include <map>
#include <iostream>

using namespace std;

template<class T>
void show_map(T map)
{
    cout<<endl<<"== show_map =="<<endl;
    for(auto a:map)
    { cout<<"["<<a.first<<" - "<<a.second<<"]"<<endl; }
    cout<<"== end =="<<endl;
}

//===== map常规操作测试 =====
void test_1()
{
    //== map插入方式 ==
    //  1.下标插入，下标为key，将value赋值给key
    //  2.insert插入，需要pair声明键值，需要和map声明一致
    //  3.emplace插入，会在容器内声明键值对，只需直接赋值
    map<string,int> map1;
    map1["a1"] = 10;
    map1["b2"] = 20;
    map1.insert(pair<string,int>("c3",30));
    map1.insert(pair<string,int>("d4",40));
    map1.emplace("e5",50);
    map1.emplace("f6",60);

    show_map(map1); //显示容器内容

    //== map查找内容 ==
    //  1.下标查找，返回value，不存在则创建新的键值对
    //  2.find函数查找，返回目标迭代器，不存在返回容器末尾迭代器
    //  推荐使用find函数，可以知道键值对是否存在，且不会乱创建
    int f1 = map1["f666"];          //不存在
    int f2 = map1["b2"];            //存在

    auto it1 = map1.find("f777");   //不存在
    auto it2 = map1.find("b2");     //存在

    cout<<"f1: "<<f1<<endl;
    cout<<"f2: "<<f2<<endl;

    //迭代器如果不等于map.end()，表示存在值，
    //  迭代器的第一、第二参数与map键值对一致
    if(it1 != map1.end()) { cout<<"it1: "<<it1->first<<" | "<<it1->second<<endl; }
    else { cout<<"it1: not find"<<endl; }

    if(it2 != map1.end()) { cout<<"it2: "<<it2->first<<" | "<<it2->second<<endl; }
    else { cout<<"it2: not find"<<endl; }

    show_map(map1); //显示容器内容

    //== map删除方式 ==
    // 1.erase(key),直接删除key，返回值是成功删除的个数
    // 2.erase(it),利用迭代器删除，迭代器必须存在，否则程序崩溃
    size_t c1 = map1.erase("f888"); //不存在
    size_t c2 = map1.erase("a1");   //存在

    //存在，必须存在,返回下一个迭代器，一般没有用
    map<string,int>::iterator itret;
    auto itfind = map1.find("b2");
    if(itfind != map1.end()) itret = map1.erase(itfind);

    cout<<"c1: "<<c1<<endl;
    cout<<"c2: "<<c2<<endl;
    cout<<"itret: "<<itret->first<<"|"<<itret->second<<endl;

    show_map(map1); //显示容器内容

    //迭代器范围清除（前开后闭区间），清除前三个元素
    map<string,int>::iterator itret2 = map1.begin(); //获取头部迭代器
    for(int i=0;i<3;i++) itret2++; //迭代器向尾部移动三次
    map1.erase(map1.begin(),itret2);
    show_map(map1); //显示容器内容

    //清空所有元素
    map1.clear();
    show_map(map1); //显示容器内容


    //排除值范围
    map<string,int> map2;
    map2.emplace("a1",10);
    map2.emplace("a2",20);
    map2.emplace("a3",30);
    map2.emplace("a4",40);
    map2.emplace("a5",50);
    map2.emplace("a6",60);
    map2.emplace("a7",70);

    show_map(map2); //显示容器内容

    //模糊的范围清空：
    //  如果lower_bound找不到现有的key，则计算参数key与容器内key的关系，
    //      并试图返回前一个迭代器，计算失败可能会返回end()迭代器，
    //      upper_bound则返回后一个迭代器
    auto itf1 = map2.lower_bound("a29");
    auto itf2 = map2.upper_bound("a57");
    cout<<"itf1: "<<itf1->first<<"|"<<itf1->second<<endl;
    cout<<"itf2: "<<itf2->first<<"|"<<itf2->second<<endl;

    map2.erase(itf1,itf2);
    show_map(map2); //显示容器内容

//    auto itrange = map2.equal_range("a2");
//    for(auto it = itrange.first;it != itrange.second;it++)
//    {
//        cout<<"range: ["<<it->first<<"-"<<it->second<<"]"<<endl;
//    }


}
//===== map常规操作测试 =====


//===== multimap特殊用法 =====
void test_2()
{
    multimap<string,int> map1;
    map1.emplace("a1",10);
    map1.emplace("a1",10);
    map1.emplace("a1",20);
    map1.emplace("a2",20);
    map1.emplace("a3",30);
    map1.emplace("a3",40);
    map1.emplace("a4",50);
    map1.emplace("a4",50);

    show_map(map1); //显示容器内容
    cout<<endl<<"统计重复key数量: "<<map1.count("a1")<<endl;

    cout<<"== 遍历重复key的内容 =="<<endl;
    auto itrange = map1.equal_range("a1");

    for(auto it = itrange.first;it != itrange.second;it++)
    { cout<<"range: ["<<it->first<<"-"<<it->second<<"]"<<endl; }
}


//== windows中文显示 ==
#ifdef _WIN32
#include <windows.h>
#endif


int main()
{
    //== Windows控制台: UTF-8编码 ==
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif


    cout<<"===== begin ====="<<endl;
//    test_1();
    test_2();
    cout<<"===== end ====="<<endl;
    return 0;
}

