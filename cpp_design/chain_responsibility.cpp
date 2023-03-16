//!
//! C++设计模式: 职责链-代码实例以及其他代替方案
//!
//! == 职责链简介 ==
//!     职责链主要用于一个接收用户的一个请求，然后在处理方层层传递，
//!         最终处理完成将结果返回
//!     如果职责链如果没有找到到对应的处理响应方，则该请求不处理
//!     职责链主要用于将请求与处理解耦，使得请求方无需关系任务是如何处理的，
//!         只需要关注处理方式提供的处理接口
//!     职责链在处理流程上非常灵活，任务可以是多处理，
//!         即出现响应函数时继续传递任务，由多个处理函数依次处理，
//!         也可以是单处理并退出。
//!     职责链可以将大量统一的任务细分到几个处理类中，减轻处理函数的体量与复杂度，
//!         或者用于优化出现的大量if-else代码段。
//! == 职责链简介 ==
//!
//!
//! == 职责链作用以及代替方案 ==
//!     职责链模式常用来替代if-else结构语句，因为大量的if-else会增加后期维护，
//!         且修改往往需要改动框架代码，违反开闭原则
//!
//! 职责链代替方案：
//!     1.map回调（利用map索引到回调函数）
//!     2.模板职责链（无需继承即可实现职责链功能）
//! == 作用以及代替方案 ==
//!
//!
//! //== 任务说明 ==
//!     处理CS:GO游戏中玩家按下键盘的操作，
//!         当玩家按下WSAD与起跳、下蹲按键时调用对应的处理函数
//! //== 任务说明 ==
//!
//!
//! //== 实现方式 ==
//!     1、if-else处理方式
//!     2、经典职责链处理方式
//!     3、map回调处理方式 (推荐--适用无任务顺序要求，快速定位请求处理)
//!     4、递归模板职责链
//!     5、可变参数职责链 (推荐--适用无继承关系，任务需求与经典职责链要求一致)
//!
//! //== 实现方式 ==
//!
#include <iostream>
#include <map>
#include <functional>
#include <tuple>

using namespace std::placeholders;
using namespace std;

//===== 任务数据 =====
//== 坐标 ==
struct ct_point
{
    int x;
    int y;
    int z;
    ct_point operator=(const ct_point &pos)
    { this->x = pos.x; this->y = pos.y; this->z = pos.z;  return *this; }
};
ostream & operator<<(ostream &out,const ct_point &pos)
{ return out<<"["<<pos.x<<"."<<pos.y<<"."<<pos.z<<"] "; }

//== 指令类型 ==
enum en_key
{
    e_W,
    e_S,
    e_A,
    e_D,
    e_Ctrl,
    e_Spacebar,
    e_None,
};

//== 翻译函数 ==
en_key to_enmu(const string &op)
{
    en_key key = e_None;
    if(op == "W") { key = e_W; }
    else if(op == "S") { key = e_S; }
    else if(op == "A") { key = e_A; }
    else if(op == "D") { key = e_D; }
    else if(op == "Spacebar") { key = e_Spacebar; }
    else if(op == "Ctrl") { key = e_Ctrl; }
    return key;
}

//== 按键按下时的指标处理 ==
void move_go(ct_point &pos) { pos.x += 1; }
void move_back(ct_point &pos) { pos.x -= 1; }
void move_left(ct_point &pos) { pos.y -= 1; }
void move_right(ct_point &pos) { pos.y += 1; }
void move_jump(ct_point &pos) { pos.z += 1; }
void move_crouch(ct_point &pos) { pos.z -= 1; }
//===== 任务数据 =====



//===== if-else处理方式 =====
//常见的if_else或switch处理方法
//      可以看出，不管是if_else或switch,如果想再新增一个功能的时候，
//      都必须要改动func_if_else函数的实现
bool func_if_else(const string &op, ct_point &pos)
{
    //== if-else ==
    en_key key = to_enmu(op);
    if(key == e_W) { move_go(pos); }
    else if(key == e_S) { move_back(pos); }
    else if(key == e_A) { move_left(pos); }
    else if(key == e_D) { move_right(pos); }
    else if(key == e_Spacebar) { move_jump(pos); }
    else if(key == e_Ctrl) { move_crouch(pos); }
    else { return false; }
    return true;

#if 0
    //== switch ==
    switch (to_enmu(op))
    {
    case e_W : move_go(pos); break;
    case e_S : move_back(pos); break;
    case e_A : move_left(pos); break;
    case e_D : move_right(pos); break;
    case e_Spacebar : move_jump(pos); break;
    case e_Ctrl : move_crouch(pos); break;
    case e_None : default: return false; break;
    }
    return true;
#endif
}
//===== if-else处理方式 =====



//===== 经典职责链处理方式 =====
class task_list
{
public:
    virtual ~task_list(){}; //如果子类多态分配内存则需要重写，否则内存泄露
    void set_next(task_list *next) { _next = next; }
    bool start_task(const string &op,ct_point &pos)
    {
        if(is_ok(op)) return request(pos);              //可完成直接返回
        if(_next) { return _next->start_task(op,pos); } //不在范围，交给下一个
        return false;                                   //无人处理，返回失败
    }

protected:
    virtual bool request(ct_point &pos) = 0;
    virtual bool is_ok(const string &op) = 0;
    task_list *_next = nullptr;
};

class task_go : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_go(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "W") return true; else return false; }
};

class task_back : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_back(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "S") return true; else return false; }
};

class task_left : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_left(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "A") return true; else return false; }
};

class task_right : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_right(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "D") return true; else return false; }
};

class task_jump : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_jump(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "Spacebar") return true; else return false; }
};

class task_crouch : public task_list
{
protected:
    bool request(ct_point &pos) override
    { move_crouch(pos); return true; }
    bool is_ok(const string &op) override
    { if(op == "Ctrl") return true; else return false; }
};

//职责链的对外接口类：（该类通常分文件写，添加其他职责类时只需单独修改本接口构造函数）
//      只需要添加处理类与修改接口的连接点，就可以调整控制流程
//职责链相对与if-else来说代码相当冗余与复杂，如果不是为了接口的统一与类解耦，
//      其实并不推荐使用，我更推荐使用map回调的处理方式，接下来展示map回调
class inter_task_list
{
public:
    inter_task_list()
    {
        t1 = new task_go;
        t2 = new task_back;
        t3 = new task_left;
        t4 = new task_right;
        t5 = new task_jump;
        t6 = new task_crouch;

        t1->set_next(t2);
        t2->set_next(t3);
        t3->set_next(t4);
        t4->set_next(t5);
        t5->set_next(t6);
    }

    ~inter_task_list()
    {
        delete t1;
        delete t2;
        delete t3;
        delete t4;
        delete t5;
        delete t6;
    }

    bool start_list(const string &op, ct_point &pos)
    { return t1->start_task(op,pos); }

private:
    task_list *t1;
    task_list *t2;
    task_list *t3;
    task_list *t4;
    task_list *t5;
    task_list *t6;
};
//===== 经典职责链处理方式 =====



//===== map回调处理方式 =====
//可以看出，map回调机制是利用map的索引直接索取到对应的调用函数上执行，
//      与职责链相比不仅可以获得实现职责链的好处，还能不受任务数量的影响。
//map回调不会因为数量增加而降低速度，而且可以用更少的代码便可实现功能，
//      还拥有更好的代码代码可读性。
//不过map回调有一个缺点重大，就是无法想职责链一样依次执行任务链，
//      而且执行结束之后不能将任务继续传递
//map回调只适合一次性执行且不要求执行顺序的任务场景，
//      接下来展示可顺序执行的模板职责链
class inter_map_cb
{
public:
    inter_map_cb()
    {
        map_task.insert(pair<string,
            function<void (ct_point &)>>("W",bind(&move_go,_1)));
        map_task.insert(pair<string,
            function<void (ct_point &)>>("S",bind(&move_back,_1)));
        map_task.insert(pair<string,
            function<void (ct_point &)>>("A",bind(&move_left,_1)));
        map_task.insert(pair<string,
            function<void (ct_point &)>>("D",bind(&move_right,_1)));
        map_task.insert(pair<string,
            function<void (ct_point &)>>("Spacebar",bind(&move_jump,_1)));
        map_task.insert(pair<string,
            function<void (ct_point &)>>("Ctrl",bind(&move_crouch,_1)));
    }
    bool start_map_cb(const string &op, ct_point &pos)
    {
        auto it = map_task.find(op);
        if(it != map_task.end())
        { (it->second(pos)); return true; }
        else return false;
    }

protected:
    map<string,function<void (ct_point &)>> map_task;
};
//===== map回调处理方式 =====



//===== 模板化任务数据 =====
class Ttask_go
{
public:
    bool request(ct_point &pos)
    { move_go(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "W") return true; else return false; }
};

class Ttask_back
{
public:
    bool request(ct_point &pos)
    { move_back(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "S") return true; else return false; }
};

class Ttask_left
{
public:
    bool request(ct_point &pos)
    { move_left(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "A") return true; else return false; }
};

class Ttask_right
{
public:
    bool request(ct_point &pos)
    { move_right(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "D") return true; else return false; }
};

class Ttask_jump
{
public:
    bool request(ct_point &pos)
    { move_jump(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "Spacebar") return true; else return false; }
};

class Ttask_crouch
{
public:
    bool request(ct_point &pos)
    { move_crouch(pos); return true; }
    bool is_ok(const string &op)
    { if(op == "Ctrl") return true; else return false; }
};
//===== 模板化任务数据 =====



//===== 递归模板职责链 =====
//结束模板:
//      相信熟悉模板递归调用的一定能很容易理解,
//      类似可变模板参数的参数解析一样需要在无参数是手动停止调用
class Twork_end
{
public:
    bool start(const string &, ct_point &)
    { return false; }
};

//递归模板:
//      这个调用了每个职责链类中的操作函数，与经典职责链一致，
//      拥有链式任务流程与下一个任务的连接点
template<class T,class Tlist>
class Twork
{
public:
    Twork(T func,Tlist next) : _func(func),_next(next){}
    bool start(const string &op, ct_point &pos) //调用鸭子函数（request and is_ok）
    {
        if(_func.is_ok(op)) { return _func.request(pos); }
        else  { return _next.start(op,pos); }
    }

private:
    T _func;        //当前任务
    Tlist _next;    //下一个任务
};

//递归模板职责链接口:
//      其实这个实现只是跟你们开一个玩笑，因为连接点可以说相当不友好，
//          不是不能用，但绝对不好用,只能说正常人都不会这样用
//      不管是声明方式1还是2都相当的麻烦，且容易出错，
//          相当不友好的连接点迫使我对这个递归职责链做出了研究并提供可行方案
//      接下来展示的模板化可变参数职责链才是真正可行的经典职责链替代方案
class inter_Twork
{
public:
    bool start_work(const string &op, ct_point &pos)
    {
        Ttask_go t1;
        Ttask_back t2;
        Ttask_left t3;
        Ttask_right t4;
        Ttask_jump t5;
        Ttask_crouch t6;

        //== 声明方式 1 ==
        Twork_end k0;
        typedef Twork_end def_t0;
        Twork<Ttask_go,def_t0> k1(t1,k0);
        typedef Twork<Ttask_go,def_t0> def_t1;
        Twork<Ttask_back,def_t1> k2(t2,k1);
        typedef Twork<Ttask_back,def_t1> def_t2;
        Twork<Ttask_left,def_t2> k3(t3,k2);
        typedef Twork<Ttask_left,def_t2> def_t3;
        Twork<Ttask_right,def_t3> k4(t4,k3);
        typedef Twork<Ttask_right,def_t3> def_t4;
        Twork<Ttask_jump,def_t4> k5(t5,k4);
        typedef Twork<Ttask_jump,def_t4> def_t5;
        Twork<Ttask_crouch,def_t5> k6(t6,k5);

#if 0
        //== 声明方式 2 ==
        Twork_end k0;
        Twork<Ttask_go,Twork_end>
                k1(t1,k0);
        Twork<Ttask_back,Twork<Ttask_go,Twork_end>>
                k2(t2,k1);
        Twork<Ttask_left,Twork<Ttask_back,Twork<Ttask_go,Twork_end>>>
                k3(t3,k2);
        Twork<Ttask_right,Twork<Ttask_left,Twork<Ttask_back,Twork<Ttask_go,Twork_end>>>>
                k4(t4,k3);
        Twork<Ttask_jump,Twork<Ttask_right,Twork<Ttask_left,Twork<Ttask_back,Twork<Ttask_go,Twork_end>>>>>
                k5(t5,k4);
        Twork<Ttask_crouch,Twork<Ttask_jump,Twork<Ttask_right,Twork<Ttask_left,Twork<Ttask_back,Twork<Ttask_go,Twork_end>>>>>>
                k6(t6,k5);
#endif
        return k6.start(op,pos);
    }
};
//===== 递归模板职责链 =====



//===== 可变参数职责链 =====
//任务函数调用模板
template<class Ttuple,size_t size>
class Twork_m
{
public:
    static bool action(Ttuple tuple,const string &op, ct_point &pos)
    {
        auto it = std::get<size -1>(tuple);
        if(it.is_ok(op)) { it.request(pos); return true; }
        return Twork_m<Ttuple,size -1>::action(tuple,op,pos);
    }
};

//模板结束特化
template<class Ttuple>
class Twork_m<Ttuple,0>
{
public:
    static bool action(Ttuple,const string &, ct_point &) { return false; }
};

//可变参数职责链组装类
//      该类将所有的职责任务类组装到tuple容器之内，
//          利用任务函数调用模板触发tuple容器的递归，
//          依次执行职责链上的任务函数
//      可变参数职责链与经典职责链的执行流程保持一致，
//          而且无需继承类，只需要实现同名接口即可调用，
//          在执行速度与灵活性上相比经典职责链都有明显优势，
//          且组装方式更加简单易用
template<class ...Tarr>
class Twork_list
{
public:
    void add_work(Tarr ...arg)
    { _tuple = make_tuple(arg...); }

    bool start(const string &op, ct_point &pos)
    { return Twork_m<tuple<Tarr...>,tuple_size<tuple<Tarr...>>::value>::action(_tuple,op,pos); }

private:
    tuple<Tarr...> _tuple;
};

//可变参数职责链接口
//      接口函数因为会被反复调用，
//          所以最好在初始化时保留可用指针，
//          而不是每次调用接口函数都重新生成一份对象,
//          递归模板职责链的接口 inter_Twork 类就是一个反面教程，
//          因为他每次调用都会生成一份新的职责链对象
class inter_Twork_list
{
public:
    inter_Twork_list()
    {
        //声明模板对象，需要对应每个职责任务
        Twork_list<Ttask_go,Ttask_back,Ttask_left,
                Ttask_right,Ttask_jump,Ttask_crouch> work;
        //装载匿名职责任务类
        work.add_work(Ttask_go(),Ttask_back(),Ttask_left(),
                    Ttask_right(),Ttask_jump(),Ttask_crouch());
        _work = &work;
    }
    bool start_work(const string &op, ct_point &pos)
    { return _work->start(op,pos); }
protected:
    Twork_list<Ttask_go,Ttask_back,Ttask_left,
            Ttask_right,Ttask_jump,Ttask_crouch> *_work;
};
//===== 可变参数职责链 =====



//===== 测试 =====
//if-else处理方式测试
void test_1()
{
    ct_point pos{0,0,0};
    cout<<"if-else: \n"<<pos<<endl;

    bool ok1 = func_if_else("W",pos);
    cout<<"ok1: "<<ok1<<" | "<<pos<<endl;

    bool ok2 = func_if_else("Ctrl",pos);
    cout<<"ok2: "<<ok2<<" | "<<pos<<endl;

    bool ok3 = func_if_else("NONO",pos);
    cout<<"ok3: "<<ok3<<" | "<<pos<<endl;

    cout<<endl<<endl;
}

//经典职责链处理方式测试
void test_2()
{
    inter_task_list inter;
    ct_point pos{0,0,0};
    cout<<"inter_task_list: \n"<<pos<<endl;

    bool ok1 = inter.start_list("W",pos);
    cout<<"ok1: "<<ok1<<" | "<<pos<<endl;

    bool ok2 = inter.start_list("Ctrl",pos);
    cout<<"ok2: "<<ok2<<" | "<<pos<<endl;

    bool ok3 = inter.start_list("NONO",pos);
    cout<<"ok3: "<<ok3<<" | "<<pos<<endl;

    cout<<endl<<endl;
}

//map回调处理方式测试
void test_3()
{
    inter_map_cb inter;
    ct_point pos{0,0,0};
    cout<<"inter_map_cb: \n"<<pos<<endl;

    bool ok1 = inter.start_map_cb("W",pos);
    cout<<"ok1: "<<ok1<<" | "<<pos<<endl;

    bool ok2 = inter.start_map_cb("Ctrl",pos);
    cout<<"ok2: "<<ok2<<" | "<<pos<<endl;

    bool ok3 = inter.start_map_cb("NONO",pos);
    cout<<"ok3: "<<ok3<<" | "<<pos<<endl;

    cout<<endl<<endl;
}

//递归模板职责链测试
void test_4()
{
    inter_Twork inter;
    ct_point pos{0,0,0};
    cout<<"inter_Twork: \n"<<pos<<endl;

    bool ok1 = inter.start_work("W",pos);
    cout<<"ok1: "<<ok1<<" | "<<pos<<endl;

    bool ok2 = inter.start_work("Ctrl",pos);
    cout<<"ok2: "<<ok2<<" | "<<pos<<endl;

    bool ok3 = inter.start_work("NONO",pos);
    cout<<"ok3: "<<ok3<<" | "<<pos<<endl;

    cout<<endl<<endl;
}

//可变参数职责链测试
void test_5()
{
    inter_Twork_list inter;
    ct_point pos{0,0,0};
    cout<<"inter_Twork_list: \n"<<pos<<endl;

    bool ok1 = inter.start_work("W",pos);
    cout<<"ok1: "<<ok1<<" | "<<pos<<endl;

    bool ok2 = inter.start_work("Ctrl",pos);
    cout<<"ok2: "<<ok2<<" | "<<pos<<endl;

    bool ok3 = inter.start_work("NONO",pos);
    cout<<"ok3: "<<ok3<<" | "<<pos<<endl;

    cout<<endl<<endl;
}
//===== 测试 =====

int m_main()
{
    cout<<"== begin =="<<endl;
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    cout<<"== end =="<<endl;
    return 0;
}

int main()
{
    m_main();
    return 0;
}

/*
 * 执行输入结构如下:
 *
== begin ==
if-else:
[0.0.0]
ok1: 1 | [1.0.0]
ok2: 1 | [1.0.-1]
ok3: 0 | [1.0.-1]


inter_task_list:
[0.0.0]
ok1: 1 | [1.0.0]
ok2: 1 | [1.0.-1]
ok3: 0 | [1.0.-1]


inter_map_cb:
[0.0.0]
ok1: 1 | [1.0.0]
ok2: 1 | [1.0.-1]
ok3: 0 | [1.0.-1]


inter_Twork:
[0.0.0]
ok1: 1 | [1.0.0]
ok2: 1 | [1.0.-1]
ok3: 0 | [1.0.-1]


inter_Twork_list:
[0.0.0]
ok1: 1 | [1.0.0]
ok2: 1 | [1.0.-1]
ok3: 0 | [1.0.-1]


== end ==
*/
