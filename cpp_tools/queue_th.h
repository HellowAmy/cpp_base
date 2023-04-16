//!
//! C++工具：STL容器多线程接口封装--多线程接口安全
//!
//! ===== 多线程容器简介 =====
//! 多线程安全容器指的是在多个线程同时操作同一个容器时,总是保证数据同步的,
//!     但C++STL标准容器中不提供线程安全的容器,保证线程安全需要消耗更多的时间等待操作,
//!     本章封装了STL容器中的部分容器,采用保护继承的方式尽最大可能保留原始STL容器接口
//! 主要封装方式为对所有接口加锁,保证容器接口线程安全,但不保证迭代器的线程安全,
//!     多线程操作迭代器存在迭代器易位情况, 如果需要操作迭代器需要自行加锁
//! 因为STL容器存在可保存的迭代器直接指向内存地址,导致很难保证线程安全,
//!     如果需要实现线程安全的容器,迭代器不应该保留,如此以来也失去了操作数据的灵活性
//! ===== 多线程容器简介 =====
//!
//!
//! ===== 提供的封装容器 =====
//!     queue_th    : queue  管道容器代表
//!     map_th      : map    索引容器代表
//!     vector_th   : vector 线性容器代表
//! ===== 提供的封装容器 =====
//!
//!
//! ===== 迭代器易位 =====
//! 迭代器易位的场景,如vector容器在多线程下扩容,导致其他线程的迭代器全部失效,
//!     或者容器数据改变,导致迭代器的数据顺序改变,如map的insert函数被连续调用,
//!     导致其他线程提前保存的begin迭代器可能改变位置,迭代器失效
//! ===== 迭代器易位 =====
//!
//!
//! ===== 内容展示顺序 =====
//! 1.queue_th.h
//! 2.map_th.h
//! 3.vector_th.h
//! 4.main.h (包含容器测试函数)
//! ===== 内容展示顺序 =====
//!
//! 结束语:
//!     可以提供简易的多线程容器应用场景,但是不保证所有操作的安全性,
//!         如果需要大量多线程的场景请选择真正线程安全的容器
//!
#ifndef QUEUE_TH_H
#define QUEUE_TH_H

#include <queue>
#include <deque>
#include <mutex>
#include <memory>

//!
//! 功能: 对queue容器加锁,保证多线程下调用接口安全,不保证迭代器安全
//! 原理:保护继承queue容器,实现大部分函数接口, 使用继承构造获取大部分父类构造函数
//!
template<typename _Tp, typename _Sequence = std::deque<_Tp> >
class queue_th : protected std::queue<_Tp,_Sequence>
{
public:
    //C++STD
    typedef typename	_Sequence::value_type		value_type;
    typedef typename	_Sequence::reference		reference;
    typedef typename	_Sequence::size_type		size_type;
    //this class
    typedef std::queue<_Tp,_Sequence>       parent;
    typedef std::lock_guard<std::mutex>     lock_t;

public:
    using std::queue<_Tp,_Sequence>::queue;

    //拷贝底层deque容器的数据到queue_th,需要覆盖父类的拷贝构造和赋值重载
    explicit queue_th(const queue_th& __q) : parent(__q) {}
    explicit queue_th(queue_th&& __q) : parent(std::move(__q)) {}
    void operator=(const queue_th& __q) { this->c = __q.c; }
    void operator=(queue_th&& __q) { this->c = std::move(__q.c); }

    void pop() { lock_t lock(_mutex); this->parent::pop(); }
    reference back() { lock_t lock(_mutex); return this->parent::back(); }
    reference front() { lock_t lock(_mutex); return this->parent::front(); }
    size_type size() { lock_t lock(_mutex); return this->parent::size(); }
    bool empty() { lock_t lock(_mutex); return this->parent::empty(); }

    void swap(queue_th& __q)
    { lock_t lock(_mutex); std::swap(this->c, __q.c); }

    void push(const value_type& __x)
    { lock_t lock(_mutex);  this->parent::push(__x); }

    void push(value_type&& __x)
    { lock_t lock(_mutex);  this->parent::push(std::move(__x)); }

    template<typename... _Args>
    void emplace(_Args&&... __args)
    { lock_t lock(_mutex); this->parent::emplace(std::forward<_Args>(__args)...); }

protected:
    std::mutex _mutex;
};
#endif // QUEUE_TH_H
