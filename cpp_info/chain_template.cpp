//!
//! C++代码详解：模板化职责链
//!
//! ===== 模板化职责链简介 =====
//! 模板化职责链到目前我并未发现有任何项目存在先例，几乎属于独创，
//!     该思想是我在开发项目时，发现项目接口过于死板，由于该项目建立在旧项目之上，
//!     且都未使用面向对象的设计方式。于是开始构思并改进这个接口问题，
//!     在项目代码在不做大规模调整的情况下，重新设计接口的灵活性，
//!     在较劲脑汁之后，偶然发现了tuple容器与职责链的妙用，
//!     成功的在无类继承的情况下实现了模板化职责链
//! ===== 模板化职责链简介 =====
//!
//!
//!
//! ===== 任务实现 =====
//! 在正式进入实例代码之前，请掌握一些前置知识：
//!     1.职责链
//!     2.模板化编程
//!     2.模板类特化
//!     3.可变参数模板的展开方式
//!     4.tuple容器的使用
//!
//! 接下来通过实现一个成功等级查询的例子简单介绍模板化职责链的具体实现：
//!     通过输入姓名与分数判断该成绩属于什么水平的成绩等级。
//! ===== 任务实现 =====
//!
//!
//! 结束语:
//!     模板化职责链的使用与职责链差不多，使用模板化职责链相对复杂，
//!     也并没有什么其他好处，所以没有特殊需求还是老老实实用其他方式去实现。
//!
#include <iostream>
#include <tuple>

using namespace std;

//== 等级排名 ==
//任务类声明区别：
//模板化职责链的使用与经典职责链一样，
//      需要至少存在【任务函数】与【职责判断】两个函数（这里分别对应show、is_ok函数）
//      模板化职责链的不同之处是无需建立类继承关系，所有的任务类都是独立的
class A
{
public:
    void show(const string &name)
    { cout<<name<<" 同学的成绩为 "<<_number<<" --属于 A 等级"<<endl; }
    bool is_ok(int number)
    {
        _number = number;
        if(_number >= 90 && _number < 100) return true;
        return false;
    }
    int _number = 0;
};

class B
{
public:
    void show(const string &name)
    { cout<<name<<" 同学的成绩为 "<<_number<<" --属于 B 等级"<<endl; }
    bool is_ok(int number)
    {
        _number = number;
        if(_number >= 60 && _number < 90) return true;
        return false;
    }
    int _number = 0;
};

class C
{
public:
    void show(const string &name)
    { cout<<name<<" 同学的成绩为 "<<_number<<" --属于 C 等级"<<endl; }
    bool is_ok(int number)
    {
        _number = number;
        if(_number >= 0 && _number < 60) return true;
        return false;
    }
    int _number = 0;
};
//== 等级排名 ==



//===== 可变参数职责链 =====
//再声明模板化职责链时，需要四个部分组成，分别是：
//      class Twork             :调用模板，控制职责链的调用与调用成功后是否继续传递
//      class Twork<Ttuple,0>   :结束模板，特化的调用模板，用于结束tuple容器的递归解参数
//      class Twork_list        :链接模板，用于将任务类链接到tuple容器内，等待职责链的执行
//      class inter_Twork       :接口模板，控制具体的任务类的连接流程，并提供对外接口
//
//== 任务函数调用模板 ==
//注意该模板的声明，第二参数size_t size是用于解开tuple容器的模板参数，
//      因为tuple容器只支持编译时展开，不支持运行时展开，
//      所以不能使用for(size_t i)的方式展开tuple容器，
//      只能通过递归展开等模板化的特殊展开方式
template<class Ttuple,size_t size>
class Twork
{
public:
    //action函数用于执行职责任务类的任务函数，其中Ttuple tuple参数是必须的，
    //      其他参数需要根据实际的任务类参数需求进去定制，
    //      所以Twork模板类是定制的，并不能通用
    //      该函数的写法与经典职责链写法一直
    static bool action(Ttuple tuple,const string &name, int _number)
    {
        auto it = std::get<size -1>(tuple);
        if(it.is_ok(_number)) { it.show(name); return true; }

        //如果不在范围，则返回下一个tuple展开的结果，语法为展开tuple容器的下一个任务类
        return Twork<Ttuple,size -1>::action(tuple,name,_number);
    }
};

//模板结束特化
//注意该模板第二参数为0，是Twork类的特化模板，tuple容器展开结束时为容器个数为0进入结束特化，
//      遇到结束时结束特化会返回false表示任务未能在职责链上执行对应任务，
//      结束特化需要声明与Twork类的action函数一样的函数，通常该函数不实现任务
template<class Ttuple>
class Twork<Ttuple,0>
{
public:
    static bool action(Ttuple,const string &,int) { return false; }
};

//任务类链接模板
//该模板类是用tuple容器链接所有声明的任务函数，并将容器与任务函数参数一起传入Twork类等待执行，
//      通常的实现为add_work函数通过make_tuple函数添加任务类对象到tuple容器，
//      start函数通知Twork类执行具体任务流程，
//      Twork_list类的实现通常是不变的，可以通用
template<class ...Tarr_class>
class Twork_list
{
public:
    void add_work(Tarr_class ...arg)
    { _tuple = make_tuple(arg...); }

    //start函数触发Twork类的action函数可以说相当复杂
    //      静态调用Twork类的action函数:
    //          【Twork<tuple<Tarr_class...>,tuple_size<tuple<Tarr_class...>>::value>::action】
    //      参数1：tuple<Tarr_class...> 为_tuple容器的声明
    //      参数2：tuple_size<tuple<Tarr_class...>>::value 获取为_tuple容器的数量，代表任务类传入的数量
    template<class ...Tarr_arg>
    bool start(Tarr_arg ...arg)
    { return Twork<tuple<Tarr_class...>,tuple_size<tuple<Tarr_class...>>::value>::action(_tuple,arg...); }
private:
    tuple<Tarr_class...> _tuple;
};

//== 接口函数 ==
class inter_Twork
{
public:
    inter_Twork()
    {
        //声明对应顺序的职责链模板对象
        Twork_list<A,B,C> *work = new Twork_list<A,B,C>;
        work->add_work(A(),B(),C()); //装载匿名职责任务类
        _work = work; //保持指针等待调用，防止多次调用接口时重复创建对象
    }
    ~inter_Twork() { delete _work; }

    //触发借口，调用启动函数
    template<class ...Tarr>
    bool start_work(Tarr ...arg)
    { return _work->start(arg...); }

protected:
    Twork_list<A,B,C> *_work;
};
//===== 可变参数职责链 =====



//== 测试 ==
void test()
{
    inter_Twork work;

    if(work.start_work("小明",97) == false)
    { cout<<"不存在成绩范围"<<endl; }

    if(work.start_work("小芳",87) == false)
    { cout<<"不存在成绩范围"<<endl; }

    if(work.start_work("小虎",67) == false)
    { cout<<"不存在成绩范围"<<endl; }

    if(work.start_work("小甲",33) == false)
    { cout<<"不存在成绩范围"<<endl; }

    if(work.start_work("小全",-18) == false)
    { cout<<"不存在成绩范围"<<endl; }

    if(work.start_work("小天",124) == false)
    { cout<<"不存在成绩范围"<<endl; }
}



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

    test();

    cout<<"== end =="<<endl;
    return 0;
}

int main()
{
    m_main();
    return 0;
}
