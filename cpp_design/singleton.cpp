//!
//! C++设计模式: 单例模式-提供线程安全的单例模板类
//!
//! == 单例模式简介 ==
//! 单例模式出现的原因是一个类在整个程序运行时，最多只能存在一个对象，
//!     单例类本身会构造出唯一的对象，并返回指针给使用者，
//!     为了防止使用者自己构造对象，单例类需要将构造函数限制在私有区域
//! 单例类的特性是一个程序只存在一个对象，且可以跨文件提供给使用者，
//!     这意味着单例类与全局变量一样，没有作用域的限制
//! == 单例模式简介 ==
//!
//!
//! == 单例类特点 ==
//! 单例类存在静态创建与动态创建两种方式：
//!     静态创建：在main函数执行前创建，在main函数结束后释放，无论是否使用都会创建对象
//!     动态创建：首次使用时创建，可以手动释放资源，不使用则不创建
//! 动态创建由于创建实际延时到调用时机，会出现多线程问题，需要线程同步，
//!     实际使用默认推荐静态创建，因为如果不使用该单例对象的话，就不应该存在这个文件
//! 如果存在多选一判断的单例类，即多个单例类中最后使用的结果未知，且只使用其中一个，
//!     推荐动态创建，以节约全局资源
//!
//! 单例模板类在使用上分为真单例与伪单例两种用法：
//!     真单例：构造函数私有，外部无法在创建新的对象
//!     伪单例：使用默认构造函数，提供单例对象接口，但外部可以创建对象
//! 当使用者不是自己是，往往需要提供真单例给其他人使用，以防止误操作，
//!     但如果创建与使用单例的但是自己，则推荐伪单例，因为明确知道类的使用方法，
//!     则不强制限制构造函数，非单例但当作单例使用，以达到快速开发的目的
//! == 单例类特点 ==
//!
//!
//! == 应用场景 ==
//! 单例类的唯一对象特性非常适合用于对资源对接，如存在唯一数据库，
//!     或者唯一日志类时的接口封装，且由于没有作用域的限制，
//!     使得被单例类控制的资源可以在全范围使用
//! 单例类无限制作用域的特点是一把双刃剑，即十分灵活也打破了模块封装的壁垒，
//!     如资源需要限制范围时，需谨慎使用单例模式
//! == 应用场景 ==
//!
//! 结束语：
//!     文章在最后提供Tsingle.h文件的单例模板，可供快速建立类，
//!         文章的测试代码直接运行，会导致打印内容混乱，
//!         最后的测试结果已经归纳好了单个test函数的输出结果
//!
#include "../include/Tsingle.h"
#include <iostream>
#include <mutex>
#include <vector>
#include <fstream>

using namespace std;

//===== 单例模式写法 =====
//== 静态创建 ==
//new singleton_s 的静态指针会在进入main函数之前执行，
//      exit_class 类的静态对象的析构函数会在main函数执行结束之后调用，
//      利用exit_class析构函数释放singleton_s指针内配的内存
class singleton_s
{
public:
    static singleton_s* get() { return _obj; }

    void print(string log)
    {
        cout<<log<<endl;
    }

private:
    singleton_s()
    {
        static exit_class exit;
        cout<<"in singleton_s"<<endl;
    }

    ~singleton_s()
    {
        cout<<"out singleton_s"<<endl;
    }

    struct exit_class
    {
        exit_class()
        {
            cout<<"in exit_class"<<endl;
        }
        ~exit_class()
        {
            delete _obj;
            cout<<"out exit_class"<<endl;
        }
    };

    static singleton_s *_obj;
};
singleton_s *singleton_s::_obj = new singleton_s;
//== 静态创建 ==


//== 动态创建 ==
//防止多线程创建需要加锁，保证多线程安全
//动态创建通常是为了节约内存，不必要不创建，既提供创建也提供回收
class singleton_d
{
public:
    static singleton_d* get()
    {
        //双重判断加锁提速
        if(_obj == nullptr)
        {
            _mut.lock();
            if(_obj == nullptr)
            {
                _obj = new singleton_d;
            }
            _mut.unlock();
        }
        return _obj;
    }

    void clear()
    {
        _mut.lock();
        delete _obj;
        _obj = nullptr;
        _mut.unlock();
    }

    void print(string log)
    {
        cout<<log<<endl;
    }

private:
    singleton_d()
    {
        cout<<"in singleton_d"<<endl;
    }

    ~singleton_d()
    {
        cout<<"out singleton_d"<<endl;
    }

    static singleton_d *_obj;
    static std::mutex _mut;
};
singleton_d *singleton_d::_obj = nullptr;
std::mutex singleton_d::_mut;
//===== 单例模式写法 =====



//静态创建与销毁时机测试
void test_1()
{
    cout<<"== test_1 =="<<endl;
    singleton_s::get()->print("<< print singleton_s >>");
}

//动态创建与销毁时机测试
void test_2()
{
    cout<<"== test_2 =="<<endl;
    singleton_d::get()->print("<< print singleton_d >>");
    singleton_d::get()->clear();
}

//===== 测试准备 =====
//静态模板: 限制构造函数--真单例
class vclog : public Tsingle_s<vclog>
{
    TSINGLE(vclog,Tsingle_s)
public:
    void print(string log)
    {
        cout<<"vclog: "<<log<<endl;
    }
};

//动态模板: 限制构造函数--真单例
class dclog : public Tsingle_d<dclog>
{
    TSINGLE(dclog,Tsingle_d)
public:
    void print(string log)
    {
        cout<<"dclog: "<<log<<endl;
    }
};

//不限制构造函数--伪单例（可以通过构造函数创建对象）
class cclog
{
public:
    void print(string log)
    {
        cout<<"cclog: "<<log<<endl;
    }
};
//===== 测试准备 =====

//单例模板测试
void test_3()
{
    cout<<"== test_3 =="<<endl;

    //静态模板
    vclog::get()->print("123");             //真单例推荐用法
    Tsingle_s<cclog>::get()->print("456");  //伪单例推荐用法

    //动态模板
    dclog::get()->print("123qwe");             //真单例推荐用法
    Tsingle_s<dclog>::get()->print("456qwe");  //伪单例推荐用法
}


//===== 简单日志-单例 =====
#define cl ""<<end
#define okc ok_c && op_c && oc_c
#define okf ok_f && op_f && oc_f

class vlog
{
public:
    enum level{ e_error,e_warning,e_debug,e_info };
    static vlog* instance() { if(obj==nullptr){obj=new vlog;} return obj; };
    void set_level(level elc) { el_c=elc; }
    void set_status(bool oc) { oc_c=oc; }

    bool init(level el = e_info)
    {
        if(op_c) { return op_c; }
        el_c = el; op_c = true; return op_c;
    }

    template<class T>
    vlog& operator<<(const T &txt)
    { if(okc) std::cout<<txt; return *this; };
    vlog& operator<<(std::ostream& (*end)(std::ostream&))
    { if(okc) std::cout<<cl; return *this; };
    vlog& operator<<(level el)
    {
        if(el <= el_c) ok_c = true; else ok_c = false;
        if(okc){ std::cout<<vec[el]<<"] "; }
        return *this;
    };

    explicit vlog()
    {
        vec.push_back("[Err");
        vec.push_back("[War");
        vec.push_back("[Deb");
        vec.push_back("[Inf");
    }

private:
    bool oc_c = true;
    bool op_c = false;
    bool ok_c = false;
    level el_c;

    static vlog* obj;
    std::vector<std::string> vec;
    std::string v_filename;

    std::string get_time()
    {
        time_t t; time(&t); char tmp[32];
        strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&t));
        return tmp;
    }
    class mcv { ~mcv(){delete obj; obj = nullptr;} }; static mcv t;
};
vlog* vlog::obj = nullptr;

#define vloge(...) \
    (*Tsingle_s<vlog>::get())<<vlog::e_error\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogw(...) \
    (*Tsingle_s<vlog>::get())<<vlog::e_warning\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogd(...) \
    (*Tsingle_s<vlog>::get())<<vlog::e_debug\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogf(...) \
    (*Tsingle_s<vlog>::get())<<vlog::e_info\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

//===== 简单日志-单例 =====

//简单日志测试
void test_4()
{
    cout<<"== test_4 =="<<endl;

    Tsingle_s<vlog>::get()->init(vlog::e_debug); //设置打印等级

    //原生接口
    (*Tsingle_s<vlog>::get())<<vlog::e_info<<123<<endl;
    (*Tsingle_s<vlog>::get())<<vlog::e_debug<<456<<endl;
    (*Tsingle_s<vlog>::get())<<vlog::e_warning<<"qwe"<<endl;
    (*Tsingle_s<vlog>::get())<<vlog::e_error<<"asd"<<endl;

    //宏接口
    vlogf("100");
    vlogd("200");
    vlogw("300");
    vloge("400");
}


int main()
{
    cout<<"== begin =="<<endl;

    //实际应用
    test_1();
    test_2();
    test_3();
    test_4();

    cout<<"== end =="<<endl;
    return 0;
}

/*
 * 1.静态创建与销毁时机测试
 *
in exit_class
in singleton_s
== begin ==
== test_1 ==
<< print txt >>
== end ==
out singleton_s
out exit_class


 *
 * 2.动态创建与销毁时机测试
 *
== begin ==
== test_2 ==
in exit_class
in singleton_d
<< print singleton_d >>
out singleton_d
== end ==


 *
 * 3.模板单例模式测试
 *
== begin ==
== test_3 ==
vclog: 123
cclog: 456
dclog: 123qwe
dclog: 456qwe
== end ==


 *
 * 4.简单日志模板测试
 *
== begin ==
== test_4 ==
[Deb] 456
[War] qwe
[Err] asd
[Deb] [../cpp_design/singleton.cpp:<304>] <<<< 200
[War] [../cpp_design/singleton.cpp:<305>] <<<< 300
[Err] [../cpp_design/singleton.cpp:<306>] <<<< 400
== end ==

*/

