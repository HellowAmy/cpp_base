//!
//! C++容器: 索引容器[map - set]
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
//! == 容器变体 ==
//! multi 前缀 : 重复key版本，可以同时存放多个key，如果数据的key不唯一，且需要使用索引容器，
//!                 可以使用该版本，通常选用容器的确认顺序是，
//!                 考虑到必须使用索引容器之后，在考虑选择哪个版本的索引容器
//!
//! unordered 前缀: 哈希散列表版本,通常比红黑树版本拥有2-3倍的微妙级速度提升，
//!                     不过可能存在最坏情况，会比红黑树慢2-3倍甚至更多，
//!                     红黑树版本更稳定，内存占用更小，推荐使用
//!
//! 何种前缀的容器在使用上并没有什么本质区别，根据应用场景选择才是最优选，
//!     C++11容器接口基本一致，只需要掌握map容器与其他类型容器的区别，
//!     便可掌握所有索引容器的使用
//! == 容器变体 ==
//!
//!
//! == map ==
//! 模板声明：
//!     template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
//!     typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
//!     class map
//!
//! 模板参数 _Key : key参数类型
//! 模板参数 _Tp : value参数类型
//! 模板参数 _Compare : key的比较函数，用于排序key顺序
//! 模板参数 _Alloc : 分配器，采用pair键值对分配元素内存
//!
//! map容器作为索引容器最常见的容器之一，几乎包揽了C++11标准库中所有需要查询的任务，
//!     相比于字典之类标准库之外的索引容器，map提供了稳定和快速两个版本，
//!     unordered_map 与 hash_map 指的都是同一种散列表实现方式
//!
//! 在性能上hash_map总是在大多数时候胜出，一般为map速度的2-3倍，虽然都在微妙级别，
//!     实际上它们的差距并不能达到肉眼可见的地步，
//!     在微妙级别即时相差百倍对于一般场景来说也毫无影响
//! == map ==
//!
//!
//! == set ==
//! 模板声明：
//!     template<typename _Key, typename _Compare = std::less<_Key>,
//!     typename _Alloc = std::allocator<_Key> >
//!     class set
//!
//! 模板参数 _Key : key参数类型
//! 模板参数 _Compare : key的比较函数，用于排序key顺序
//! 模板参数 _Alloc : 普通分配器
//!
//! set容器是我认为最不知道该用在什么地方的容器之一，它的行为总是可以被map替代，
//!     因为没有[key-value]不能被当前正常的索引容器来使用，它本身就是二者的结合，
//!     大多数时候的作用只是被当做一个可以快速索引的vector容器来使用，
//!     因为set不可以重复，并不能充当完全的vector容器，因此set的出场率并不高，
//!     反倒是unordered_set符合这一特点
//! unordered_set容器是一个类似带索引的vector容器，如果当数据不考虑存放的顺序，
//!     且需要频繁索引，此时unordered_set容器将是一个很好的选择
//! == set ==
//!
//!
//! == 测试顺序 ==
//!     1. map常规操作测试
//!     2. multimap特殊用法
//!     3.索引容器应用场景
//!     4.使用实例
//!     5.性能测试
//! == 测试顺序 ==
//!
//!
//! 结束语:
//!     C++11标准容器接口大差不差，关键是理解容器数据结构以及容器使用场景，
//!         各类容器优缺点等才算是掌握，标准容器的使用只需要遵循规范即可
//!
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

    cout<<"显示容器内容:"<<endl;
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

    cout<<"容器变化:"<<endl;
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
//===== multimap特殊用法 =====

template<class T>
void show_vec(T vec,bool is_nl = true)
{
    cout<<"===== show_vec ====="<<endl;
    if(is_nl) { for(auto a:vec) { cout<<a<<" "; }  }
    else { for(auto a:vec) { cout<<a<<endl; }  }
    cout<<endl<<"===== end ====="<<endl;
}

template<class T>
void show_it(T it1,T it2)
{
    cout<<"===== show_it ====="<<endl;
    for(auto it = it1;it!=it2;it++)
    { cout<<*it<<" "; }
    cout<<endl<<"===== end ====="<<endl;
}

//===== 索引容器应用场景 =====
void test_3()
{
    cout<<endl<<endl<<"== 索引容器应用场景 =="<<endl;
    vector<int> vec1{1,1,1,2,3,5,5,6,7,8,8,9,9,9};
    cout<<"去重前: "<<endl;
    show_vec(vec1);


    //== 数据去重 ==
    set<int> set1;
    for(const auto &a:vec1)
    { set1.emplace(a); }

    vector<int> vec2;
    vec2.clear();
    for(const auto &a:set1)
    { vec2.push_back(a); }

    cout<<endl<<"去重后: "<<endl;
    show_vec(vec2);


    //== 重复个数统计 ==
    map<int,int> map1;
    for(const auto &a:vec1)
    { map1[a]++; }
    cout<<endl<<"数据重复统计结果: ";
    show_map(map1);


    //== 数据查重 ==
    cout<<endl<<"数据查重: "<<endl;
    show_it(set1.begin(),set1.end());
    auto p1 = set1.emplace(3);    //重复
    auto p2 = set1.emplace(4);    //新数据
    auto p3 = set1.emplace(12);   //新数据
    cout<<"p1: "<<p1.second<<endl;
    cout<<"p2: "<<p2.second<<endl;
    cout<<"p3: "<<p3.second<<endl;
    show_it(set1.begin(),set1.end());


    //== map双向索引 ==
    //通过两个容器复制一份反转的[key-value]达到相互索引的作用，
    //      可以加快索引速度，但会消耗大量内存，要求是[key-value]都没有重复值
    //例子：通过手机号码查询银行卡，反之查手机号码
    using u64 = unsigned long long;
    map<u64,u64> map2_1;
    map2_1.emplace(157734453486,567584657366723467);
    map2_1.emplace(152464456345,345124657366723452);
    map2_1.emplace(157804456745,672224657366767234);
    map2_1.emplace(156445451352,657362465766723434);
    map2_1.emplace(157358344798,367824673667265736);
    map2_1.emplace(159623445454,956767234366726723);

    map<u64,u64> map2_2;
    for(const auto &a:map2_1)
    { map2_2.emplace(a.second,a.first); }

    cout<<"map双向索引: "<<endl;
    show_map(map2_1);
    auto it21 = map2_2.find(672224657366767234);
    auto it22 = map2_1.find(it21->second);

    cout<<"|"<<it21->first<<"|"<<it21->second<<"|"<<endl;
    cout<<"|"<<it22->first<<"|"<<it22->second<<"|"<<endl;
    //== map双向索引 ==


    //== map索引函数调用 ==
    cout<<endl<<"map索引函数调用: "<<endl;
    map<int,function<void()>> map3;

    map3.emplace(1,[](){
        cout<<"执行函数 [1]"<<endl;
    });
    map3.emplace(11,[](){
        cout<<"执行函数 [11]"<<endl;
    });
    map3.emplace(3,[](){
        cout<<"执行函数 [3]"<<endl;
    });

    auto f1 = map3.find(1);
    if(f1 != map3.end()) (f1->second());
    else cout<<"not find func"<<endl;

    auto f2 = map3.find(2);
    if(f2 != map3.end()) (f2->second());
    else cout<<"not find func"<<endl;

    auto f3 = map3.find(3);
    if(f3 != map3.end()) (f3->second());
    else cout<<"not find func"<<endl;
}
//===== 索引容器应用场景 =====


//== 学生信息 ==
struct ct_student
{
    string name_class;          //班级名字
    string name_student;        //学生名字
    long long num_class;        //班级内序号
    long long num_school;       //学校内序号
};

ostream& operator<<(ostream& os,const ct_student &st)
{
    return os<<"[nc: "<<st.name_class<<"|ns:"
            <<st.name_student<<"|num c:"
           <<st.num_class<<"|num s: "
          <<st.num_school
         <<"] ";
}

//===== 使用实例 =====
void test_4()
{
    cout<<endl<<endl<<"== 使用实例 =="<<endl;

    //===== 学生数据统计 =====
    //学生数据模板
    ct_student tmepale;
    tmepale.name_class = "last class ";
    tmepale.name_student = "Amy_";
    tmepale.num_school = 563;
    tmepale.num_class = 1;

    int count_school = tmepale.num_school;

    //== 准备数据 ==
    cout<<endl<<"== 一班数据 =="<<endl;
    vector<ct_student> vecs1;
    for(int i=0;i<11;i++)
    {
        count_school++;
        ct_student tm = tmepale;
        tm.name_class += "1";
        tm.name_student += 'a'+i;
        tm.num_class += i;
        tm.num_school = count_school;

        vecs1.push_back(tm);
    }
    show_vec(vecs1,false);


    cout<<endl<<"== 二班数据 =="<<endl;
    vector<ct_student> vecs2;
    tmepale.name_student = "Tom_";
    for(int i=0;i<16;i++)
    {
        count_school++;
        ct_student tm = tmepale;
        tm.name_class += "2";
        tm.name_student += 'a'+i;
        tm.num_class += i;
        tm.num_school = count_school;

        vecs2.push_back(tm);
    }
    show_vec(vecs2,false);


    cout<<endl<<"== 三班数据 =="<<endl;
    vector<ct_student> vecs3;
    tmepale.name_student = "Jack_";
    for(int i=0;i<22;i++)
    {
        count_school++;
        ct_student tm = tmepale;
        tm.name_class += "3";
        tm.name_student += 'a'+i;
        tm.num_class += i;
        tm.num_school = count_school;

        vecs3.push_back(tm);
    }
    show_vec(vecs3,false);
    //== 准备数据 ==


    //== 加入数据到容器：以班级为索引 ==
    //以班级为索引，学生为数据
    multimap<string,ct_student> map1; //班级+学生信息
    for(auto a:vecs1)
    { map1.emplace(a.name_class,a); }

    for(auto a:vecs2)
    { map1.emplace(a.name_class,a); }

    for(auto a:vecs3)
    { map1.emplace(a.name_class,a); }

    //统计3班人数
    size_t count_3 = map1.count("last class 3");
    cout<<"统计三班人数: "<<count_3<<endl;

    //查看二班信息
    cout<<endl<<"查看二班信息: "<<endl;
    auto it1 = map1.equal_range("last class 2");
    map<string,ct_student> map2; //姓名+学生信息
    for(auto it=it1.first;it!=it1.second;it++)
    { map2.emplace(it->second.name_student,it->second); }
    show_map(map2); //显示信息

    //查看二班信息Tom_k的信息
    auto itname1 = map2.find("Tom_k");
    if(itname1 != map2.end()) cout<<endl<<"Tom_k信息: "<<itname1->second<<endl;

    //查看一班班级序号为5的学生信息
    auto itns=map1.find("last class 1");
    for(int i=0;i<7-1;i++) itns++;
    cout<<endl<<"一班序号为7的学生信息: "<<itns->second<<endl;
    //== 加入数据到容器：以班级为索引 ==


    //== 加入数据到容器：以学校序号为索引 ==
    map<long long,ct_student> map3; //学校序号+学生信息
    for(const auto &a:map1)
    { map3.emplace(a.second.num_school,a.second); }

    //查看学校序号为611的学生
    auto itns1 = map3.find(611);
    if(itns1 != map3.end()) cout<<endl<<"学校序号为611的信息: "<<itns1->second<<endl;
}
//===== 使用实例 =====


//===== ctimel =====
//!
//! 功能：打印时间间隔
//!
//! 说明：主要用于计算函数运行的时间，测试函数性能
//!         steady_clock时间：精确到纳秒的均速时间
//! 原理：构建对象时开始计时，析构时打印时间
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

    inline string to_string()
    {
        //顺序 [纳秒|微秒|毫秒|秒]
        auto loss = steady_clock::now() - _begin;
        return string("[nan: " + std::to_string(loss.count()) +
                      "|mic: " + std::to_string(duration_cast<microseconds>(loss).count()) +
                      "|mil: " + std::to_string(duration_cast<milliseconds>(loss).count())  +
                      "|sec: " + std::to_string(duration_cast<seconds>(loss).count()) + "]\n");
    }

    inline void update() { _begin = steady_clock::now(); }

protected:
    time_point<steady_clock,nanoseconds> _begin;
};


//随机数生成函数
long long rand_number(int count = 10)
{
    long long account = 0;
    if(count < 1) return account;

    while(true)
    {
        char buf[count + 1];
        buf[count] = '\0';
        for(int i=0;i<count;i++) buf[i] = '0' + rand() % 10;

        if(buf[0] != '0')
        { account = atoll(buf); break; }
    }
    return account;
}

//===== 性能测试 =====
void test_5()
{
    cout<<endl<<endl<<"== 性能测试 =="<<endl;

    srand(time(NULL));//随机数初始化

    //一白万个11位数的数字模拟手机号长度
    vector<long long> vec;
    for(size_t i=0;i<1000000;i++)//
    { vec.push_back(rand_number(11)); }

    map<long long,int> map;
    unordered_map<long long,int> hash_map;

    //!
    //! 进行十次性能测试结论：
    //!     1.五个分布点查找hash_map快3-4倍
    //!     2.四百次查找hash_map快2-3倍
    //!     3.五百次删除hash_map快3-5倍
    //!     4.一千次查找hash_map快3-4倍
    //!
    //! 在本次测试数据上hash_map基本在微妙级速度上全面领先map，
    //!     且多次测试并未出现最坏情况，可能测试数据并不合理
    //! 在一千万的数据量下，速度的比例与一百万不相上下
    //!
    for(int i=0;i<10;i++)
    {
        //插入数据到容器
        for(auto a:vec)
        { map.insert(pair<long long,int>(a,1)); }
        for(auto a:map)
        { hash_map.insert(pair<long long,int>(a.first,1)); }
        cout<<endl<<endl<<map.size()<<"|"<<hash_map.size()<<endl;

        //时间点测试
        cout<<endl<<"== map find =="<<endl;
        {
            ctimel ct;

            //五个分布点查找
            long long g1 = map.find(vec[0])->first;
            long long g2 = map.find(vec[vec.size()/2])->first;
            long long g3 = map.find(vec[vec.size()/4])->first;
            long long g4 = map.find(vec[vec.size()/2+vec.size()/4])->first;
            long long g5 = map.find(vec[vec.size()-100])->first;
            string ss1 = ct.to_string();
            ct.update();

            //四百次集中查找
            int count1 = 0;
            for(int i=0;i<400;i++)
            {
                auto it = map.find(vec[10000+i]);
                if(it != map.end()) count1++;
            }
            string ss2 = ct.to_string();
            ct.update();

            //五百次分布点删除
            int count2 = 0;
            for(int i=0;i<500;i++)
            { count2 += map.erase(vec[1990*i]); }
            string ss3 = ct.to_string();
            ct.update();


            //一千次分布点查找
            int count3 = 0;
            for(int i=0;i<1000;i++)
            {
                auto it = map.find(vec[888*i]);
                if(it != map.end()) count3++;
            }
            string ss4 = ct.to_string();
            ct.update();

            cout<<"["<<g1<<g2<<g3<<g4<<g5<<"]"<<count1<<count2<<count3<<endl; //防止汇编优化
            cout<<endl<<ss1<<ss2<<ss3<<ss4<<endl;
        }

        cout<<endl<<"== hash_map find =="<<endl;
        {
            ctimel ct;

            //五个分布点查找
            long long g1 = hash_map.find(vec[0])->first;
            long long g2 = hash_map.find(vec[vec.size()/2])->first;
            long long g3 = hash_map.find(vec[vec.size()/4])->first;
            long long g4 = hash_map.find(vec[vec.size()/2+vec.size()/4])->first;
            long long g5 = hash_map.find(vec[vec.size()-100])->first;
            string ss1 = ct.to_string();
            ct.update();

            //四百次集中查找
            int count1 = 0;
            for(int i=0;i<400;i++)
            {
                auto it = hash_map.find(vec[10000+i]);
                if(it != hash_map.end()) count1++;
            }
            string ss2 = ct.to_string();
            ct.update();

            //五百次分布点删除
            int count2 = 0;
            for(int i=0;i<500;i++)
            { count2 += hash_map.erase(vec[1990*i]); }
            string ss3 = ct.to_string();
            ct.update();


            //一千次分布点查找
            int count3 = 0;
            for(int i=0;i<1000;i++)
            {
                auto it = hash_map.find(vec[888*i]);
                if(it != hash_map.end()) count3++;
            }
            string ss4 = ct.to_string();
            ct.update();

            cout<<"["<<g1<<g2<<g3<<g4<<g5<<"]"<<count1<<count2<<count3<<endl; //防止汇编优化
            cout<<endl<<ss1<<ss2<<ss3<<ss4<<endl;
        }
    }
}
//===== 性能测试 =====


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
    test_1();
    test_2();
    test_3();
    test_4();
//    test_5();
    cout<<"===== end ====="<<endl;
    return 0;
}

/*
 * 一千万级别速度测试：某一次结果
 *
9999076|9999076

== map find ==
[9654012102813233300242136390485673455108531997672722299]400500998

[nan: 26500|mic: 26|mil: 0|sec: 0]
[nan: 1402500|mic: 1402|mil: 1|sec: 0]
[nan: 2421000|mic: 2421|mil: 2|sec: 0]
[nan: 3252700|mic: 3252|mil: 3|sec: 0]

[nan: 12393800|mic: 12393|mil: 12|sec: 0]

== hash_map find ==
[9654012102813233300242136390485673455108531997672722299]400500998

[nan: 9100|mic: 9|mil: 0|sec: 0]
[nan: 396800|mic: 396|mil: 0|sec: 0]
[nan: 367200|mic: 367|mil: 0|sec: 0]
[nan: 787300|mic: 787|mil: 0|sec: 0]

[nan: 13096300|mic: 13096|mil: 13|sec: 0]


 *
 * 一百万级别速度测试：某一次结果
 *
999986|999986

== map find ==
[1875558382075612418622641309969445788761424032805514112]400500998

[nan: 25900|mic: 25|mil: 0|sec: 0]
[nan: 1166300|mic: 1166|mil: 1|sec: 0]
[nan: 2196900|mic: 2196|mil: 2|sec: 0]
[nan: 2688000|mic: 2688|mil: 2|sec: 0]

[nan: 3932600|mic: 3932|mil: 3|sec: 0]

== hash_map find ==
[1875558382075612418622641309969445788761424032805514112]400500998

[nan: 8900|mic: 8|mil: 0|sec: 0]
[nan: 330600|mic: 330|mil: 0|sec: 0]
[nan: 825400|mic: 825|mil: 0|sec: 0]
[nan: 768700|mic: 768|mil: 0|sec: 0]

[nan: 8585900|mic: 8585|mil: 8|sec: 0]

*/

/*
 * ===== 部分测试结果截取 =====
 *
===== begin =====
显示容器内容:

== show_map ==
[a1 - 10]
[b2 - 20]
[c3 - 30]
[d4 - 40]
[e5 - 50]
[f6 - 60]
== end ==
f1: 0
f2: 20
it1: not find
it2: b2 | 20
容器变化:

== show_map ==
[a1 - 10]
[b2 - 20]
[c3 - 30]
[d4 - 40]
[e5 - 50]
[f6 - 60]
[f666 - 0]
== end ==
c1: 0
c2: 1
itret: c3|30

== show_map ==
[c3 - 30]
[d4 - 40]
[e5 - 50]
[f6 - 60]
[f666 - 0]
== end ==

== show_map ==
[f6 - 60]
[f666 - 0]
== end ==

== show_map ==
== end ==

== show_map ==
[a1 - 10]
[a2 - 20]
[a3 - 30]
[a4 - 40]
[a5 - 50]
[a6 - 60]
[a7 - 70]
== end ==
itf1: a3|30
itf2: a6|60

== show_map ==
[a1 - 10]
[a2 - 20]
[a6 - 60]
[a7 - 70]
== end ==

== show_map ==
[a1 - 10]
[a1 - 10]
[a1 - 20]
[a2 - 20]
[a3 - 30]
[a3 - 40]
[a4 - 50]
[a4 - 50]
== end ==

统计重复key数量: 3
== 遍历重复key的内容 ==
range: [a1-10]
range: [a1-10]
range: [a1-20]


== 索引容器应用场景 ==
去重前:
===== show_vec =====
1 1 1 2 3 5 5 6 7 8 8 9 9 9
===== end =====

去重后:
===== show_vec =====
1 2 3 5 6 7 8 9
===== end =====

数据重复统计结果:
== show_map ==
[1 - 3]
[2 - 1]
[3 - 1]
[5 - 2]
[6 - 1]
[7 - 1]
[8 - 2]
[9 - 3]
== end ==

数据查重:
===== show_it =====
1 2 3 5 6 7 8 9
===== end =====
p1: 0
p2: 1
p3: 1
===== show_it =====
1 2 3 4 5 6 7 8 9 12
===== end =====
map双向索引:

== show_map ==
[152464456345 - 345124657366723452]
[156445451352 - 657362465766723434]
[157358344798 - 367824673667265736]
[157734453486 - 567584657366723467]
[157804456745 - 672224657366767234]
[159623445454 - 956767234366726723]
== end ==
|672224657366767234|157804456745|
|157804456745|672224657366767234|

map索引函数调用:
执行函数 [1]
not find func
执行函数 [3]


== 使用实例 ==

== 一班数据 ==
== 二班数据 ==
== 三班数据 ==

统计三班人数: 22

查看二班信息:

== show_map ==
[Tom_a - [nc: last class 2|ns:Tom_a|num c:1|num s: 575] ]
[Tom_b - [nc: last class 2|ns:Tom_b|num c:2|num s: 576] ]
[Tom_c - [nc: last class 2|ns:Tom_c|num c:3|num s: 577] ]
[Tom_d - [nc: last class 2|ns:Tom_d|num c:4|num s: 578] ]
[Tom_e - [nc: last class 2|ns:Tom_e|num c:5|num s: 579] ]
[Tom_f - [nc: last class 2|ns:Tom_f|num c:6|num s: 580] ]
[Tom_g - [nc: last class 2|ns:Tom_g|num c:7|num s: 581] ]
[Tom_h - [nc: last class 2|ns:Tom_h|num c:8|num s: 582] ]
[Tom_i - [nc: last class 2|ns:Tom_i|num c:9|num s: 583] ]
[Tom_j - [nc: last class 2|ns:Tom_j|num c:10|num s: 584] ]
[Tom_k - [nc: last class 2|ns:Tom_k|num c:11|num s: 585] ]
[Tom_l - [nc: last class 2|ns:Tom_l|num c:12|num s: 586] ]
[Tom_m - [nc: last class 2|ns:Tom_m|num c:13|num s: 587] ]
[Tom_n - [nc: last class 2|ns:Tom_n|num c:14|num s: 588] ]
[Tom_o - [nc: last class 2|ns:Tom_o|num c:15|num s: 589] ]
[Tom_p - [nc: last class 2|ns:Tom_p|num c:16|num s: 590] ]
== end ==

Tom_k信息: [nc: last class 2|ns:Tom_k|num c:11|num s: 585]

一班序号为7的学生信息: [nc: last class 1|ns:Amy_g|num c:7|num s: 570]

学校序号为611的信息: [nc: last class 3|ns:Jack_u|num c:21|num s: 611]
===== end =====

*/

