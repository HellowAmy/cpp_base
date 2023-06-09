//!
//! Tsingle.h
//!
#ifndef TSINGLE_H
#define TSINGLE_H

#include <mutex>

//单例模板限制宏
#define TSINGLE(class,single)           \
    public:                             \
    friend single<class>;               \
    class() = default;                  \
    ~class() = default;                 \
    class(const class &) = delete;      \
    private:

//静态模板
template<class T>
class Tsingle_s
{
public:
    static T* get(){ return _obj; }

private:
    static T *_obj;
    friend T;
    Tsingle_s() { static exit_class exit; }
    ~Tsingle_s() = default;
    Tsingle_s(const Tsingle_s &) = delete ;

    struct exit_class { ~exit_class() { delete _obj; } };
};

//注意头文件的多重定义
template<class T> T *Tsingle_s<T>::_obj = new T;

//动态模板
template<class T>
class Tsingle_d
{
public:
    static T* get()
    {
        if(_obj == nullptr)
        {
            _mut.lock();
            if(_obj == nullptr)
            { _obj = new T; }
            _mut.unlock();
        }
        return _obj;
    }

    void clean()
    {
        _mut.lock();
        delete _obj;
        _obj = nullptr;
        _mut.unlock();
    }

private:
    static T *_obj;
    static std::mutex _mut;
    friend T;
    Tsingle_d() = default;
    ~Tsingle_d() = default;
    Tsingle_d(const Tsingle_d &) = delete ;
};

//注意头文件的多重定义
template<class T> T *Tsingle_d<T>::_obj = nullptr;
template<class T> std::mutex Tsingle_d<T>::_mut;

#endif // TSINGLE_H
