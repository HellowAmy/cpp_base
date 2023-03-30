//!
//! C++代码详解: 线程池实现以及细节详解
//!
//! ===== 线程池简介 =====
//! 线程池是并发任务中比较关键的一环，如服务器等需要多线程并发的任务环境，
//!     通常情况下会先提前建立线程池以防止内存不足时建立子线程出现失败的情况，
//!     建立线程池不仅可以避免子线程启动失败，也减少了需要时再建立的启动时间，
//!     提高对任务处理的响应速度
//!
//! 该线程池的实现是借鉴github高人气项目ThreadPool的线程池实现，
//!     并对代码进行了一点修改，提高代码可读性
//! ===== 线程池简介 =====
//!
//!
//! ===== 实现概念 =====
//! 该线程池只提供了创建线程池、插入任务、释放线程池三个功能，是固定数量线程池，
//!     用vector容器装载thread线程类，queue队列容器对外部任务函数进行排队，
//!     mutex互斥量让多线程进入排队状态，condition_variable条件变量通知睡眠线程。
//!
//! 任务流程：
//!     当线程池创建时，所有的线程都会执行create_func函数，进入wait的休眠状态，
//!     当有任务加入时，外部函数会被打包成function类，
//!         然后将function类对象加入队列，并通知线程函数，
//!         线程函数从wait函数的休眠点被唤醒，然后从队列中取出被打包function对象的任务，并执行，
//!         到此外部函数已经被线程执行完毕
//!     当调用add_work函数时该函数会立刻返回，外部通过返回future对象获取任务返回值，
//!         future类的get函数会堵塞线程，等待任务函数被执行完毕，
//!         通常不建议主线程等待返回值，否则主线程依旧会堵塞，此时多线程无意义（注意get函数不能多次调用）
//!     当有线程池触发析构函数时，会等待所有的线程执行玩全部任务之后，退出线程函数，
//!         完成对线程池的释放
//! ===== 实现概念 =====
//!
//!
//! ===== 前置知识 =====
//! 函数未知的类型返回值：auto返回值
//!     例子：
//!         auto func_name() -> int
//!     此时func_name函数的返回类型是int，等价 int func_name()，
//!
//! 类型推导：result_of推导类
//!     例子：
//!         函数声明：int func_func() { return 0; }
//!         类型推导：result_of<func_name()>::type
//!     result_of会根据func_func的类型int推导到type上，
//!     此时result_of::type的推导结果就是int类型
//!
//! 完美转发：forward转发函数
//!     forward函数就是将保留了当前函数参数的左值或者右值，
//!     将这个参数的属性转发到下一个函数的参数中，保留其左右值属性
//!     左值、右值的概念不好理解，是函数参数传递时的参数属性，
//!     参数属性会影响到编译器对重载函数的优先级选择
//!
//! 参数打包：bind打包函数
//!     将函数地址与参数打包为function对象，用于未来的执行，
//!     执行时调用前提准备好的参数，无需再次传入参数
//!
//! 打包函数：packaged_task包装类
//!     用于打包bind函数将其转为future类，准备在将来获取返回值
//!
//! 创建工厂：make_shared创建函数
//!     用于创建shared_ptr智能指针的函数，传入等待构建对象的参数
//! ===== 前置知识 =====
//!
//!
//! ThreadPool项目链接 : https://github.com/progschj/ThreadPool
//!
//! 结束语：文章下方会附带一份可直接调用的头文件代码
//!
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

using namespace std;

//===== vpool_th线程池 =====
class vpool_th
{
public:
    //创建线程池
    vpool_th(size_t number)
    {
        //准备一个循环函数--给线程池内的线程[等待任务/执行任务]
        auto create_func = [=](){
            while(true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_mutex);              //独占锁--获取队列任务
                    while (_tasks.empty() && _run) { _cond.wait(lock); }    //假唤醒--退出且队列为空
                    if(_run == false && _tasks.empty()) { return; }         //等待队列任务完成并退出任务
                    task = std::move(_tasks.front()); _tasks.pop();         //取任务
                }
                task(); //执行从队列获取的任务函数
            }
        };
        for(size_t i = 0;i<number;i++) { _workers.emplace_back(create_func); }
    }

    //释放线程池
    ~vpool_th()
    {
        { std::unique_lock<std::mutex> lock(_mutex); _run = false; }    //这里锁的用处--add_work执行时不给释放
        _cond.notify_all();                                             //唤醒所有线程准备退出
        for(std::thread &worker: _workers) { worker.join(); }           //等待所有线程完成任务后释放
    }

    //加入任务函数
    //      typename std::result_of<Tfunc(Targs...)>::type -- 获取外部函数的返回值类型
    template<class Tfunc, class... Targs>
    auto add_work(Tfunc&& func, Targs&&... args)
        -> std::future<typename std::result_of<Tfunc(Targs...)>::type>
    {
        using ret_type = typename std::result_of<Tfunc(Targs...)>::type;                //任务函数的返回类型
        auto pack = std::bind(std::forward<Tfunc>(func), std::forward<Targs>(args)...); //任务函数打包
        auto task = std::make_shared<std::packaged_task<ret_type()>>(pack);             //打包为连接future类
        auto res = task->get_future();                                                  //从future类获取函数返回值
        {
            std::unique_lock<std::mutex> lock(_mutex);              //锁住并准备将任务插入队列
            std::function<void()> func = [task](){ (*task)(); };    //包装外部任务函数到function
            if(_run) { _tasks.emplace(func); }                      //插入function到队列
        }
        _cond.notify_one(); //通知一个线程去完成任务
        return res;
    }

private:
    bool _run = true;                           //运行标记
    std::vector<std::thread> _workers;          //线程容器
    std::mutex _mutex;                          //线程池锁
    std::queue<std::function<void()>> _tasks;   //任务队列
    std::condition_variable _cond;              //条件变量
};
//===== vpool_th线程池 =====


#include <windows.h>

int work(int i,int j)
{
    cout<<endl<<"th:"<<this_thread::get_id()<<endl;
    cout<<"ret:"<<i<<"+"<<j<<endl;
    return i+j;
}

void work_sleep(int time)
{
    cout<<endl<<"th:"<<this_thread::get_id()<<endl;
    cout<<"time:"<<time<<endl;
    Sleep(time);
}

class test_c
{
public:
    int work(int i,int j)
    {
        cout<<endl<<"th:"<<this_thread::get_id()<<endl;
        cout<<"ret:"<<i<<"+"<<j<<endl;
        return i+j;
    }

    void work_sleep(int time)
    {
        cout<<endl<<"th:"<<this_thread::get_id()<<endl;
        cout<<"time:"<<time<<endl;
        Sleep(time);
    }
};

//void test()
//{

//    //===== 测试 =====
//    cout << "== begin ==" << endl;
//    future<int> fp;//外部获取返回值,销毁线程池后显示
//    int sum = 0;

//    //创建临时区
//    {
//        test_c t;
//        vpool_th po(3);
//        auto p1 = po.add_work(work,1,2);
//        auto p2 = po.add_work(work,10,20);
//        auto p3 = po.add_work(work_sleep,1);
//        auto p4 = po.add_work(work_sleep,2);
//        fp = po.add_work(&test_c::work,t,100,20);
//        auto p5 = po.add_work(&test_c::work_sleep,t,1);

//        //获取返回值
//        int k1 = p1.get();
//        int k2 = p2.get();

//        cout<<"p:"<<k1<<"|"<<k2<<endl;
//        sum += k1 + k2;
//    }
//    cout<<"sum:"<<sum<<endl;
//    cout<<"fp:"<<fp.get()<<endl;

//    //累加线程池的返回值
//    cout << "== too work ==" << endl;
//    vpool_th po(50);
//    vector<future<int>> vec_sum;
//    for(int i=0;i<10000;i++)
//    {
//        vec_sum.push_back(po.add_work(work,100,100));
//    }

//    int sum2 = 0;
//    for(size_t i=0;i<vec_sum.size();i++)
//    {
//        sum2 += vec_sum[i].get();
//    }
//    cout<<"sum2: "<<sum2<<endl;
//    cout << "== end ==" << endl;
//    //===== 测试 =====
//}

//int m_main()
//{
//    cout<<"== begin =="<<endl;

//    test();

//    cout<<"== end =="<<endl;
//    return 0;
//}

//int main()
//{
//    m_main();
//    return 0;
//}

