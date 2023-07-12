//!
//! Ttimer.h
//!
#ifndef TTIMER_H
#define TTIMER_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

//== vpool_th ==
template<int th_size>
class vpool_th
{
public:
    //创建线程池
    vpool_th()
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
        for(size_t i = 0;i<th_size;i++) { _workers.emplace_back(create_func); }
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
//== vpool_th ==


#include <chrono>
#include <mutex>
#include <memory>
#include <list>

using namespace std::chrono;
using nan = nanoseconds;
using mic = microseconds;
using mil = milliseconds;
using sec = seconds;

using minu = minutes;
using hour = hours;

using us = microseconds;
using ms = milliseconds;
using ss = seconds;

//== Ttimer ==
template<class time_level = milliseconds,int time_len = 25,int th_size = 4>
class Ttimer
{
public:
    struct task
    {
        int id;                     //定时器任务ID
        int active;                 //活动数规定执行次数 [-1 : 无限次]
        time_level time_start;      //任务开始时间
        time_level time_delay;      //延时的时间长度
        std::function<void()> task; //回调函数
    };

public:
    Ttimer()
    {
        //处理任务
        auto fn_process_task = [=](time_level time){
            while(_ls_task.size() > 0) //获取多个超时任务
            {
                auto pack = _ls_task.begin();
                if(pack->time_start <= time)
                {
                    task ct = *pack;            //保存指针指向的数据
                    _pool.add_work(pack->task); //加入任务到线程执行
                    remove_task(pack);          //移除已执行任务

                    //从保留数据重新加入到定时任务
                    if(ct.active != 0)
                    {
                        if(ct.active > 0) ct.active -= 1;       //设置剩余活跃数
                        ct.time_start = time + ct.time_delay;   //重新计算延时时间
                        add_task_ls({ct.id,ct.active,ct.time_start,ct.time_delay,ct.task});
                    }
                }
                else break;
            }
        };

        //定时器事件循环
        auto fn_event_loop = [=](){
            while(_run)
            {
                auto now = time_now();  //更新时间
                fn_process_task(now);   //处理任务
                std::this_thread::sleep_for(time_level(time_len));
            }
            remove_task_all();
        };
        _pool.add_work(fn_event_loop); //启动事件循环
    }

    //加入任务
    template<class Ttime>
    inline int add_task(Ttime delay,std::function<void()> fn,int active = -1)
    {
        auto pos_start = time_now() + duration_cast<time_level>(delay);
        int id = _task_id++;
        if(active != -1) active -= 1;
        add_task_ls({id,active,pos_start,delay,fn});
        return id;
    }

    //移除定时任务
    void remove_task(int id)
    {
        std::lock_guard<std::mutex> lock(_mut_ls);
        for(auto it = _ls_task.begin();it != _ls_task.end();it++)
        {
            if(it->id == id)
            { _ls_task.erase(it); break; }
        }
    }

    //移除定时任务--迭代器重载
    void remove_task(typename std::list<task>::iterator it)
    {
        std::lock_guard<std::mutex> lock(_mut_ls);
        _ls_task.erase(it);
    }

    //移除定时任务--所有
    void remove_task_all()
    {
        std::lock_guard<std::mutex> lock(_mut_ls);
        _ls_task.clear();
    }

    //停止定时器
    void stop_timer() { _run = false; }

    //获取定时器排队任务数（仅代表当前任务长度，无限定时任务会重复加入）
    size_t get_task_size() { return _ls_task.size(); }

    //获取现在时间
    static inline time_level time_now()
    {
        nanoseconds now = steady_clock::now().time_since_epoch();
        return duration_cast<time_level>(now);
    }

private:
    bool _run = true;           //事件循环
    int _task_id = 1;           //任务ID
    std::mutex _mut_ls;         //定时器任务锁
    vpool_th<th_size> _pool;    //线程池
    std::list<task> _ls_task;   //定时器任务容器，保证从小到大排序

    //插入到定时任务，保证顺序：从小到大 （模拟最小堆数据结构）
    void add_task_ls(const task &pack)
    {
        std::lock_guard<std::mutex> lock(_mut_ls);
        for(auto it = _ls_task.begin();it != _ls_task.end();it++)
        {
            if(pack.time_start <= it->time_start)
            { _ls_task.insert(it,pack); return; }
        }
        _ls_task.push_back(pack);
    }
};
//== Ttimer ==

#endif // TTIMER_H
