//!
//! C++容器: 管道容器[stack - queue]
//!
//! == 管道容器简介 ==
//! 本章讲解的是C++ STL中的管道容器，管道容器指的是容器像管子一样，只能看到两端，
//!     看不到中间的数据，具有封闭性
//! 在STL中，存在stack和queue两种管道容器
//!     容器特点：
//!         stack容器：先进后出，入口即是出口，除了出口之外无法看到任何数据
//!         queue容器：先进先出，入口和出口分别是管道的两端，只有管道端点可以看到数据
//! stack容器与queue容器都继承于deque容器，是deque容器的简单封装，
//!     deque容器是一种由中控器控制的连续容器，使用上类似线性容器，但内存不连续
//! == 管道容器简介 ==
//!
//!
//! == stack ==
//! 模板声明：
//!     template<typename _Tp, typename _Sequence = deque<_Tp> >
//!     class stack
//! 模板参数 _Tp ： 数据类型
//! 模板参数 _Sequence ： 分配器（deque容器）
//!
//! stack容器作为一种限制很强的容器类型，其使用场景并不多，
//!     由于是先进后出的特点，主要用于单向出入口，
//!     如应用场景中模拟枪械弹夹，第一颗压入的子弹总是最后一颗发射
//! == stack ==
//!
//!
//! == queue ==
//! 模板声明：
//!     template<typename _Tp, typename _Sequence = deque<_Tp> >
//!     class queue
//! 模板参数 _Tp ： 数据类型
//! 模板参数 _Sequence ： 分配器（deque容器）
//!
//! queue容器与stack的差别不多，由单出入口变为双出口，一般是一边进一边出，
//!     不过先进先出的特点多用于排队，而排队现象十分常见，使用也相对频繁，
//!     如应用场景中饭店就餐，先排号再根据排队叫好去取餐窗口领取
//! == queue ==
//!
//!
//! == deque ==
//! 模板声明：
//!     template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
//!     class deque : protected _Deque_base<_Tp, _Alloc>
//! 模板参数 _Tp ： 数据类型
//! 模板参数 _Sequence ： 标准容器分配器
//!
//! deque容器放在管道容器中讲述是因为其是stack与queue的数据分配器，
//!     但deque容器本身是数据开放的，即可以从任意下标取数据，而非管道容器
//! deque容器在使用上十分类似vector容器，不过采用了分段内存存储数据的方式，
//!     解决了vector容器前插数据性能很差的问题，支持前后两端的快速插入，
//!     不过随机插入速度依旧不佳
//! == deque ==
//!
//!
//! == 内存模型 ==
//! stack:                      |
//!     |^^^^^^^^^^             |
//!     |           >> / <<     | 同侧出入
//!     |__________             |
//!                             |
//! queue:
//!      ^^^^^^^^^^             |
//!     <<         <<           | 单向出入
//!      __________             |
//!                             |
//! deque:
//!          !                  |
//!     # -> =====              |
//!     # -> =====              | 无向数据
//!     # -> =====              |
//!     # -> =====              |
//!              ^              |
//! 符号解析：
//!     >>  ： 数据流向
//!     <<  ： 数据流向
//!     #   ： 数组元素
//!     # -> ===== ：数组元素里存放指针，指针指向另一块数组
//!     ! ： begin()迭代器位置
//!     ^ ： end()迭代器位置
//! == 内存模型 ==
//!
//!
//! == 测试顺序 ==
//!     1.stack与queue的接口使用
//!     2.queue容器 -- 生产消费者模型测试
//!     3.stack容器 -- 计算弹夹测试
//! == 测试顺序 ==
//!
//! 结束语：
//!     三个容器中属queue容器最常见，可以有效的保护数据顺序，
//!         防止误操作带来的循序变化，用deque分配内存可以保证头插尾插的性能，
//!         stack和deque容器则比较鸡肋，基本用不到，因为总是有更合适的容器代替
//!
#include <iostream>
#include <stack>
#include <queue>
#include <deque>

using namespace std;

//== stack与queue的接口使用 ==
void test_1()
{
    cout<<"===== test_1 ====="<<endl;

    stack<int> st;
    cout<<"== stack : 压入顺序 =="<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<i<<endl;
        st.push(i);
    }
    cout<<"== stack : 压入顺序 =="<<endl;

    cout<<endl<<"== stack : 弹出顺序 =="<<endl;
    while(st.empty() == false)
    {
        auto value = st.top();  //获取最顶的元素（唯一获取接口）
        st.pop();               //弹出顶部元素，否则下一个无法获取
        cout<<value<<endl;
    }
    cout<<"== stack : 弹出顺序 =="<<endl;

    queue<int> qu;
    cout<<endl<<"== queue : 压入顺序 =="<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<i<<endl;
        qu.push(i);
    }
    cout<<"== queue : 压入顺序 =="<<endl;

    cout<<endl<<"== queue : 弹出顺序 =="<<endl;
    while(qu.empty() == false)
    {
        auto value = qu.front();    //获取第一个压入的元素
        qu.pop();                   //弹出第一个元素，否则下一个无法获取
        cout<<value<<"  |back:" <<qu.back()<<endl;
    }
    cout<<"== queue : 弹出顺序 =="<<endl;
}
//== stack与queue的接口使用 ==


//== queue容器 -- 生产消费者模型测试 ==
#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include <condition_variable>

struct ct_data
{
    int id;
    int data;
};

void test_2()
{
    cout<<endl<<"===== test_2 ====="<<endl;

    queue<ct_data> queue_out;   //进入容器--记录进入顺序
    queue<ct_data> queue_in;    //弹出容器--记录弹出顺序

    queue<ct_data> _queue_data; //线程同步容器--多线程排队容器
    condition_variable _cond;
    mutex _mut;

    //数据加入函数  数据弹出函数  数据打包函数 创建生产者线程函数 消费者工作函数
    auto func_push_data = [&](ct_data data){
        unique_lock<mutex> lock(_mut);
        _queue_data.push(data);
    };

    //数据弹出函数--消费者工作函数
    auto func_pop_data = [&](){
        while(_queue_data.empty() == false)
        {
            {
                unique_lock<mutex> lock(_mut);
                if(_queue_data.empty()) break;
                auto data = _queue_data.front(); _queue_data.pop();
                queue_out.push(data); //获取数据并加入弹出容器，用于对比
            }
        }
    };

    //数据打包函数
    auto func_make_data = [&](int id,int data){
        ct_data ct{id,data};
        return ct;
    };

    //创建生产者线程函数
    auto func_create_data = [&](int id){
        for(int i=0;i<1000;i++)
        { func_push_data(func_make_data(id,i)); }
    };

    //多线程生产
    thread th_producer_1(func_create_data,100);
    thread th_producer_2(func_create_data,200);
    thread th_producer_3(func_create_data,300);
    thread th_producer_4(func_create_data,400);

    th_producer_1.join();
    th_producer_2.join();
    th_producer_3.join();
    th_producer_4.join();

    //多线程生产完毕，赋值到加入容器，用于对比
    queue_in = _queue_data;

    thread th_consumer_1(func_pop_data);
    thread th_consumer_2(func_pop_data);

    th_consumer_1.join();
    th_consumer_2.join();

    //所有工作完成，对比两个容器大小
    cout<<"queue_out size: "<<queue_out.size()<<endl;
    cout<<"queue_in size: "<<queue_in.size()<<endl;

    //打印容器，对比数据
    while(true)
    {
        if(queue_out.size() != queue_in.size()) cout<<"err : size not equal"<<endl;
        if(queue_out.empty() || queue_in.empty()) break;

        auto in = queue_in.front(); queue_in.pop();
        auto out = queue_out.front(); queue_out.pop();

        //如果两者一致则达到多线程同步与排队效果
        cout<<"in:["<<in.id<<": "<<in.data<<"]"<<" out:["<<out.id<<": "<<out.data<<"]"<<endl;
    }
}
//== queue容器 -- 生产消费者模型测试 ==


void test_3()
{
    cout<<endl<<"===== test_3 ====="<<endl;

    stack<int> st;
    int size_bullet = 30;

    //去弹夹
    auto func_clear = [&](){
        while(st.empty() == false)
        { st.pop(); }
    };

    //上子弹
    auto func_add = [&](){
        cout<<endl<<"== func_add =="<<endl;
        func_clear();
        for(int i=1;i<=size_bullet;i++)
        {
            cout<<"["<<i<<"] ";
            st.push(i);
        }
        cout<<endl;
    };

    //开火
    auto func_fire = [&](int size){
        cout<<"== func_fire =="<<endl;
        for(int i=0;i<size;i++)
        {
            cout<<"["<<st.top()<<"] ";
            st.pop();
            if(st.empty()) break;
        }
        cout<<endl;
        cout<<"===="<<endl;
    };

    func_add();     //上子弹
    func_fire(15);  //开火子弹--压入是顺序

    func_add();
    func_fire(30);

    func_add();
    func_fire(4);
}

int main()
{
    cout<<"===== begin ====="<<endl;

    test_1();
    test_2();
    test_3();

    cout<<"===== end ====="<<endl;
    return 0;
}

/*
===== begin =====
===== test_1 =====
== stack : 压入顺序 ==
0
1
2
3
4
== stack : 压入顺序 ==

== stack : 弹出顺序 ==
4
3
2
1
0
== stack : 弹出顺序 ==

== queue : 压入顺序 ==
0
1
2
3
4
== queue : 压入顺序 ==

== queue : 弹出顺序 ==
0  |back:4
1  |back:4
2  |back:4
3  |back:4
4  |back:4
== queue : 弹出顺序 ==


===== test_2 =====
queue_out size: 4000
queue_in size: 4000
in:[100: 0] out:[100: 0]
in:[100: 1] out:[100: 1]
in:[100: 2] out:[100: 2]
in:[100: 3] out:[100: 3]
in:[100: 4] out:[100: 4]
in:[100: 5] out:[100: 5]
in:[100: 6] out:[100: 6]
in:[100: 7] out:[100: 7]
... //省略部分结果


===== test_3 =====

== func_add ==
[1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15]
[16] [17] [18] [19] [20] [21] [22] [23] [24] [25] [26] [27] [28] [29] [30]
== func_fire ==
[30] [29] [28] [27] [26] [25] [24] [23] [22] [21] [20] [19] [18] [17] [16]
====

== func_add ==
[1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15]
[16] [17] [18] [19] [20] [21] [22] [23] [24] [25] [26] [27] [28] [29] [30]
== func_fire ==
[30] [29] [28] [27] [26] [25] [24] [23] [22] [21] [20] [19] [18] [17] [16]
 [15] [14] [13] [12] [11] [10] [9] [8] [7] [6] [5] [4] [3] [2] [1]
====

== func_add ==
[1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15]
[16] [17] [18] [19] [20] [21] [22] [23] [24] [25] [26] [27] [28] [29] [30]
== func_fire ==
[30] [29] [28] [27]
====
===== end =====
*/
