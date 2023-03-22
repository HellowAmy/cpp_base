//!
//! C++容器: 线性容器[vector - array]
//!
//! == 容器简介 ==
//! 本章讲解的是C++ STL中的线性容器，所谓线性容器就容器底层只会分配一块连续内存，
//!     在C++11 STL标准容器中只有array与vector容器算的上是线性容器，
//!     它们都是只分配一块连续内存作为数据的存储。
//!
//! array容器特点：与C语言数组几乎一致，但被封装成C++容器接口，是类对象而不是指针
//! vector容器特点：动态数组，会动态的规划内存，使用者无需关心内存分配问题
//! == 容器简介 ==
//!
//!
//! == array ==
//! 模板声明：
//!     template<class T,std::size_t N> struct array;
//!
//! array容器与其他的STL标准容器不同，不存在分配器，
//!     使用std::size_t N参数在编译期指定分配内存的数量，
//!     不能在运行时重新规划内存大小，是类对象而不是指针，
//!     所以不用担心作为返回值时内存释放问题
//! == array ==
//!
//!
//! == vector ==
//! 模板声明：
//! template<class T,class Allocator = std::allocator<T>> class vector;
//!
//! vector容器其他STL标准容器一样，带有std::allocator<T>分配器，
//!     可自动管理内存的分配与释放
//! vector容器存在总容量与已使用容量的概念，当已使用容量超出总容量，
//!     会开辟新控件进行隐式扩容，通常新的总容量是旧的2倍
//! vector容器扩容的时间成本很高，扩容问题是所有线性容器的通病
//! == vector ==
//!
//!
//! == 对比数组 ==
//! array容器：可以完全平替C风格数组，无副作用，也可以与C风格数组相互转换，
//!                 array容器内置STL容器的常用操作接口，
//!                 支持迭代器，可以与STL算法结合使用，
//!                 所以array容器更佳方便操作，作为接口传参更友好
//!
//! vector容器：C++风格代码在连续空间的内存选择上，
//!                 通常vector容器比C风格数组与array数组的出场率都要高的多，
//!                 所以vector容器是线程数组的常用选择。
//!             优点：可以自动扩容，对于小量且多次的内存分配更快，无需考虑数组大小，更方便
//!             缺点：存在总空间与使用空间的概念，会消耗额外的空间
//! == 对比数组 ==
//!
//!
//! == 容器操作 ==
//!     1.vector容器扩容测试
//!     2.vector容器常用操作
//!     3.C风格与array容器数组操作对比
//!     4.vector应用场景
//! == 容器操作 ==
//!
//!
//! 结束语：
//!     因为STL算法的加持，C++风格代码更推荐使用array代替C风格数组，
//!         如果非对内存管理不严格，建议使用vector代替array以得到更多灵活性，
//!         所以C++风格代码对数组的优先选择应该是vector容器
//!
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <list>
#include <ctime>

using namespace std;

//===== vector容器扩容测试  =====
void test_1()
{
    cout<<endl<<endl<<"== 默认总空间下追加 =="<<endl;
    vector<int> vec;

    cout<<"总空间容量: "<<vec.capacity()<<endl;
    for(size_t i=0;i<10000;i++)
    {
        vec.push_back(i);
        if(vec.size() == vec.capacity())
            cout<<"size = "<<i<<"  |"<<vec.size()<<"|"<<vec.capacity()<<endl;
    }
    cout<<"退出时总空间容量: "<<vec.capacity()<<" | 已使用:"<<vec.size()<<endl;
}

void test_2()
{
    cout<<endl<<endl<<"== 预分配下追加: resize[扩容使用空间] =="<<endl;
    vector<int> vec;
    vec.resize(50);

    cout<<"预分配 50,总空间容量: "<<vec.capacity()<<endl;
    for(size_t i=0;i<10000;i++)
    {
        vec.push_back(i);
        if(vec.size() == vec.capacity())
            cout<<"size = "<<i<<"  |"<<vec.size()<<"|"<<vec.capacity()<<endl;
    }
    cout<<"退出时总空间容量: "<<vec.capacity()<<" | 已使用:"<<vec.size()<<endl;
}

void test_3()
{
    cout<<endl<<endl<<"== 预分配下追加: reserve[扩容总空间] =="<<endl;
    vector<int> vec;
    vec.reserve(50);

    cout<<"预分配 50,总空间容量: "<<vec.capacity()<<endl;
    for(size_t i=0;i<10000;i++)
    {
        vec.push_back(i);
        if(vec.size() == vec.capacity())
            cout<<"size = "<<i<<"  |"<<vec.size()<<"|"<<vec.capacity()<<endl;
    }
    cout<<"退出时总空间容量: "<<vec.capacity()<<" | 已使用:"<<vec.size()<<endl;
}
//===== vector容器扩容测试  =====



//===== vector容器常用操作  =====
void test_4()
{
    cout<<endl<<endl<<"===== vector容器常用操作  ====="<<endl;

    //初始化
    size_t size = 10;
    vector<int> vec;

    //添加元素
    for(size_t i=0;i<size;i++) { vec.push_back(i); }

    //切片
    vector<int> vec2(vec.begin(),vec.begin()+size/2);
    cout<<endl<<"迭代器切片大小: "<<vec2.size()<<endl;


    //== 遍历 ==
    //遍历：at()函数
    cout<<endl<<"== 正常遍历 =="<<endl;
    cout<<"at()函数: "<<endl;
    try {
        for(size_t i=0;i<size;i++) { cout<<vec.at(i)<<" "; }
        cout<<endl;
    } catch (...) { cout<<"越界"<<endl; }

    //遍历：下标
    cout<<"下标: "<<endl;
    for(size_t i=0;i<size;i++) { cout<<vec[i]<<" "; }
    cout<<endl;

    //遍历：迭代器
    cout<<"迭代器: "<<endl;
    for(auto it = vec.begin();it!=vec.end();it++){ cout<<*it<<" "; }
    cout<<endl;

    //遍历：快速for
    cout<<"快速for: "<<endl;
    for(const auto &a :vec) { cout<<a<<" "; }
    cout<<endl;

    //越界遍历:
    //  使用下标遍历可能会出现越界现象，
    //      但是一个合格的程序员几乎不可能出现数组越界的情况，
    //      使用哪一种遍历方式完全由个人风格决定，
    //      如果是接口函数，优先选择迭代器可以适配不同容器
    cout<<endl<<"== 越界遍历 =="<<endl;
    cout<<"at()函数: "<<endl;
    try {
        for(size_t i=0;i<size +10;i++) { cout<<vec.at(i)<<" "; }
        cout<<endl;
    } catch (...) { cout<<"越界"<<endl; }

    //遍历：下标
    cout<<"下标: "<<endl;
    for(size_t i=0;i<size +10;i++) { cout<<vec[i]<<" "; }
    cout<<endl;

    vec.clear();
    cout<<"清空后大小: "<<vec.size()<<" | 总容量: "<<vec.capacity()<<endl;
    //== 遍历 ==


    //== 插入方式 ==
    //  push_back插入是vector最常用的方式之一，
    //      emplace_back与push_back的插入方式的却别在于，
    //      如果vector内存储的是类，emplace_back无需提前声明类，
    //      传入的参数就可以让容器原地构造对象并插入到容器内，
    //      push_back则需要提前声明对象，再插入到容器
    //  insert插入可以指定元素插入位置，并将位置之后的元素后推，非常消耗时间
    //
    //尾插，不会重排
    cout<<endl<<"== 插入与移除 =="<<endl;
    for(size_t i=0;i<size;i++) { vec.push_back(i); }
    cout<<"插入方式:push_back[尾插值] | "<<vec.size()<<endl;

    //尾插，不会重排，类似push_back
    for(size_t i=0;i<size;i++) { vec.emplace_back(i); }
    cout<<"插入方式:emplace_back[尾插值] | "<<vec.size()<<endl;

    //迭代器位置插入元素，会重排，重排时将迭代器之后的元素整体向后推
    for(size_t i=0;i<size;i++) { vec.insert(vec.begin(),i); }
    cout<<"插入方式:insert[位置迭代器 + 值] | "<<vec.size()<<endl;

    //从迭代器位置插入其他迭代器范围内的元素，不限容器类型,只需要支持迭代器
    vec.insert(vec.end(),vec2.begin(),vec2.end());
    list<int> ls{1,2,3,4,5};
    vec.insert(vec.end(),ls.begin(),ls.end());
    cout<<"插入方式:insert[位置迭代器 + 开始迭代器 + 结束迭代器] | "<<vec.size()<<endl;
    //== 插入方式 ==


    //== 移除方式 ==
    //移除最后一个元素，不会重排，时间最快
    for(size_t i=0;i<size/2;i++) { vec.pop_back(); }
    cout<<endl<<"移除方式:pop_back | "<<vec.size()<<endl;

    //移除迭代器指定元素的下标，会重排，数组的通病
    for(size_t i=0;i<size/2;i++) { vec.erase(vec.begin()); }
    cout<<"移除方式:erase [位置迭代器]| "<<vec.size()<<endl;

    //移除迭代器指定范围的元素，会重排
    vec.erase(vec.begin(),vec.begin() + vec.size()/2);
    cout<<"移除方式:erase [开始迭代器 + 结束迭代器]| "<<vec.size()<<endl;
    //== 移除方式 ==


    //== 迭代器失效 ==
    //  迭代器失效往往会造成结果的错误，或者程序崩溃
    //      所以在操作STL容器时应该要时刻注意迭代器的位置，
    //      因为STL不提供任何范围矫正
    //  通常来说只要对容器的值做了任何的修改、删除、添加都要及时矫正迭代器，
    //      尽管不是所有的操作都会影响到迭代器的移动或者失效，
    //      但随时保持矫正迭代器的习惯总是能避免出错
    cout<<endl<<"== 迭代器失效 =="<<endl;
    cout<<"容器使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;
    vector<int>::iterator it = vec.begin() + vec.size()/2;
    cout<<"迭代器位置: "<<it - vec.begin() <<"|值: "<<*it<<endl;

    vec.insert(vec.begin(),0);
    cout<<"迭代器位置前方插入数值，迭代器位置[已移动]: "<<it - vec.begin() <<"|值: "<<*it<<endl;

    vector<int> vec3;
    vec3.resize(1000);
    vec.insert(vec.begin(),vec3.begin(),vec3.end());
    cout<<"插入大量数据: "<<vec3.size()<<endl;
    cout<<"容器开辟新空间，新空间容器，使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;
    cout<<"迭代器位置[已失效]: "<<it - vec.begin()<<"|"<<*it<<endl;

    cout<<endl<<"== 迭代器失效 : 矫正 =="<<endl;
    cout<<"容器使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;
    vector<int>::iterator it2 = vec.begin() + vec.size()/2;
    cout<<"迭代器位置: "<<it2 - vec.begin() <<"|值: "<<*it2<<endl;

    vec.insert(vec.begin(),0);
    it2 = vec.begin() + vec.size()/2;
    cout<<"迭代器位置[修正移动]: "<<it2 - vec.begin() <<"|值: "<<*it2<<endl;

    vector<int> vec4;
    vec4.resize(3000);
    vec.insert(vec.begin(),vec4.begin(),vec4.end());
    cout<<"插入大量数据: "<<vec4.size()<<endl;
    cout<<"容器开辟新空间，新空间容器，使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;

    it2 = vec.begin() + vec.size()/2;
    cout<<"迭代器位置[修正失效]: "<<it2 - vec.begin()<<"|"<<*it<<endl;
    //== 迭代器失效 ==


    //== 空间回收 ==
    cout<<endl<<"== 空间回收 =="<<endl;
    vec.erase(vec.begin(),vec.begin() + vec.size()/2);
    cout<<"[回收前] 使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;

    //vector容器在内存分配上只增不减，总容量永远保存峰值状态，即时不使用
    //      当想回收内存时，可以利用栈空间自动释放的原理，释放多余空间
    {
        auto tm = vec;
        vec.swap(tm);
    }
    cout<<"[回收后] 使用空间: "<<vec.size()<<"|总容量: "<<vec.capacity()<<endl;
    //== 空间回收 ==
}
//===== vector容器常用操作  =====



//===== C风格与array容器数组操作对比 =====
void test_5()
{
    cout<<endl<<endl<<"===== C风格与array容器数组操作对比 ====="<<endl;

    //初始化
    const size_t size = 10;
    int arr_c[size];
    array<int,size> arr_cpp;

    //添加元素
    for(size_t i=0;i<size;i++) { arr_c[i] = i; }
    for(size_t i=0;i<size;i++) { arr_cpp[i] = i; }

    //遍历容器元素
    cout<<endl<<"== 遍历元素 =="<<endl;
    auto show = [](int *c,array<int,size>cpp){
        for(size_t i=0;i<size;i++)
        { cout<<c[i]<<"|"<<cpp[i]<<"  "; }
        cout<<endl;
    };
    show(arr_c,arr_cpp);

    //复制到新数组
    cout<<endl<<"== 复制到新数组 =="<<endl;
    array<int,size> arr_cpp2 = arr_cpp;

    int arr_c2[size];
    for(size_t i=0;i<size;i++) { arr_c2[i] = arr_c[i]; }
    show(arr_c2,arr_cpp2);

    //新容器内容反转
    cout<<endl<<"== 新容器内容反转 =="<<endl;
    std::reverse(arr_cpp2.begin(),arr_cpp2.end());

    for(size_t i=0;i<size/2;i++)
    { int tm = arr_c2[i]; arr_c2[i] = arr_c2[size -i -1]; arr_c2[size -i -1] = tm;}
    show(arr_c2,arr_cpp2);

    //与新容器交换内容
    cout<<endl<<"== 新容器内容反转 =="<<endl;
    arr_cpp.swap(arr_cpp2);

    for(size_t i=0;i<size;i++)
    { int tm = arr_c[i]; arr_c[i] = arr_c2[i]; arr_c2[i] = tm; }

    cout<<"旧容器: ";
    show(arr_c,arr_cpp);

    cout<<"新容器: ";
    show(arr_c2,arr_cpp2);
}
//===== C风格与array容器数组操作对比 =====



//===== vector应用场景 =====

class dog
{
public:
    dog(string name,int age) : _name(name),_age(age){}
    void show() { cout<<"dog: "<<_name<<"|"<<_age<<endl; }
private:
    string _name;
    int _age = 0;
};

void test_6()
{
    cout<<endl<<endl<<"===== vector应用场景 ====="<<endl;

    //== 场景1：容器嵌套 生成矩阵 10X10 ==
    srand(time(NULL));
    const int size = 10;
    vector<vector<int>> vec1(size);
    for(int x=0;x<size;x++)
    {
        vector<int> tm(size);
        for(int y=0;y<size;y++) { tm[y] = rand() % 100; }
        vec1[x] = tm;
    }

    cout<<endl<<"== 场景1：生成矩阵 10X10 =="<<endl;
    for(size_t x1=0;x1<vec1.size();x1++)
    {
        for(size_t y1=0;y1<vec1[x1].size();y1++) { cout<<vec1[x1][y1]<<"\t"; }
        cout<<endl;
    }


    //== 场景2：存储对象 10个 ==
    vector<dog> vec2;
    for(int i=0;i<size;i++)
    {
        string name = "小狗" + std::to_string(rand() % 100);
        vec2.emplace_back(name,rand() % 20); //直接构造对象到容器
    }

    cout<<endl<<"== 场景2：存储对象 =="<<endl;
    for(size_t i=0;i<vec2.size();i++)
    { vec2[i].show(); }


    //== 场景3：存储指针 10个 ==
    vector<dog*> vec3;
    for(int i=0;i<size;i++)
    {
        string name = "小狗" + std::to_string(rand() % 100);
        dog* p = new dog(name,rand() % 20);
        vec3.push_back(p);
    }

    cout<<endl<<"== 场景3：存储指针 =="<<endl;
    for(size_t i=0;i<vec3.size();i++)
    { vec3[i]->show(); }

    //释放
    for(size_t i=0;i<vec3.size();i++)
    { delete vec3[i]; }
}
//===== vector应用场景 =====



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

    cout<<"== begin =="<<endl;

    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();

    cout<<"== end =="<<endl;
    return 0;
}

int main()
{
    m_main();
    return 0;
}

/*

== begin ==


== 默认总空间下追加 ==
总空间容量: 0
size = 0  |1|1
size = 1  |2|2
size = 3  |4|4
size = 7  |8|8
size = 15  |16|16
size = 31  |32|32
size = 63  |64|64
size = 127  |128|128
size = 255  |256|256
size = 511  |512|512
size = 1023  |1024|1024
size = 2047  |2048|2048
size = 4095  |4096|4096
size = 8191  |8192|8192
退出时总空间容量: 16384 | 已使用:10000


== 预分配下追加: resize[扩容使用空间] ==
预分配 50,总空间容量: 50
size = 49  |100|100
size = 149  |200|200
size = 349  |400|400
size = 749  |800|800
size = 1549  |1600|1600
size = 3149  |3200|3200
size = 6349  |6400|6400
退出时总空间容量: 12800 | 已使用:10050


== 预分配下追加: reserve[扩容总空间] ==
预分配 50,总空间容量: 50
size = 49  |50|50
size = 99  |100|100
size = 199  |200|200
size = 399  |400|400
size = 799  |800|800
size = 1599  |1600|1600
size = 3199  |3200|3200
size = 6399  |6400|6400
退出时总空间容量: 12800 | 已使用:10000


===== vector容器常用操作  =====

迭代器切片大小: 5

== 正常遍历 ==
at()函数:
0 1 2 3 4 5 6 7 8 9
下标:
0 1 2 3 4 5 6 7 8 9
迭代器:
0 1 2 3 4 5 6 7 8 9
快速for:
0 1 2 3 4 5 6 7 8 9

== 越界遍历 ==
at()函数:
0 1 2 3 4 5 6 7 8 9 越界
下标:
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 -1 670248936 32443
清空后大小: 0 | 总容量: 16

== 插入与移除 ==
插入方式:push_back[尾插值] | 10
插入方式:emplace_back[尾插值] | 20
插入方式:insert[位置迭代器 + 值] | 30
插入方式:insert[位置迭代器 + 开始迭代器 + 结束迭代器] | 40

移除方式:pop_back | 35
移除方式:erase [位置迭代器]| 30
移除方式:erase [开始迭代器 + 结束迭代器]| 15

== 迭代器失效 ==
容器使用空间: 15|总容量: 60
迭代器位置: 7|值: 7
迭代器位置前方插入数值，迭代器位置[已移动]: 7|值: 6
插入大量数据: 1000
容器开辟新空间，新空间容器，使用空间: 1016|总容量: 1016
迭代器位置[已失效]: -1061|6

== 迭代器失效 : 矫正 ==
容器使用空间: 1016|总容量: 1016
迭代器位置: 508|值: 0
迭代器位置[修正移动]: 508|值: 0
插入大量数据: 3000
容器开辟新空间，新空间容器，使用空间: 4017|总容量: 4017
迭代器位置[修正失效]: 2008|6

== 空间回收 ==
[回收前] 使用空间: 2009|总容量: 4017
[回收后] 使用空间: 2009|总容量: 2009


===== C风格与array容器数组操作对比 =====

== 遍历元素 ==
0|0  1|1  2|2  3|3  4|4  5|5  6|6  7|7  8|8  9|9

== 复制到新数组 ==
0|0  1|1  2|2  3|3  4|4  5|5  6|6  7|7  8|8  9|9

== 新容器内容反转 ==
9|9  8|8  7|7  6|6  5|5  4|4  3|3  2|2  1|1  0|0

== 新容器内容反转 ==
旧容器: 9|9  8|8  7|7  6|6  5|5  4|4  3|3  2|2  1|1  0|0
新容器: 0|0  1|1  2|2  3|3  4|4  5|5  6|6  7|7  8|8  9|9


===== vector应用场景 =====

== 场景1：生成矩阵 10X10 ==
75      18      83      4       23      27      50      30      81      94
60      5       1       20      54      6       4       72      53      23
15      43      66      54      14      57      36      5       9       40
13      42      57      7       13      25      70      89      57      5
47      20      11      72      71      83      51      65      11      32
49      26      40      30      55      37      74      2       98      35
69      60      69      12      27      67      42      13      38      55
59      43      7       45      22      47      79      76      84      55
49      89      68      20      29      20      24      64      36      50
20      2       91      74      53      71      32      48      28      7

== 场景2：存储对象 ==
dog: 小狗66|3
dog: 小狗32|1
dog: 小狗92|18
dog: 小狗33|15
dog: 小狗13|17
dog: 小狗37|6
dog: 小狗31|1
dog: 小狗80|0
dog: 小狗78|9
dog: 小狗15|19

== 场景3：存储指针 ==
dog: 小狗78|2
dog: 小狗34|13
dog: 小狗0|10
dog: 小狗66|8
dog: 小狗44|4
dog: 小狗56|16
dog: 小狗6|11
dog: 小狗55|10
dog: 小狗31|11
dog: 小狗32|4
== end ==

*/

