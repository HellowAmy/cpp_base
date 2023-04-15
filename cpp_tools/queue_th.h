#ifndef QUEUE_TH_H
#define QUEUE_TH_H

#include <queue>
#include <deque>
#include <mutex>
#include <memory>

//!
//! 功能: 对queue容器加锁,保证多线程安全
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
