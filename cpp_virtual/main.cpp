//!
//! 为什么使用多态？
//!     实现多态往往是因为需要在一个公共接口上使用父类指指向子类的实现，
//!         多态可以将父类的调用留给子类 重新实现，
//!         留给子类重新实现的设计方式坚持了开闭原则还保留了代码的灵活性。
//!
//! 为什么不使用多态？
//!     使用多态并不是没有缺点，虚表的延时绑定导致调用多态函数时需要多次加载内存，
//!         C++在高性能场景不会使用多态而是用模板代替，
//!         用编译时间换多态运行时间绑定带来的性能开销
//!
//! 下列代码使用多态以及代替多态实现的方案，代码顺序分别是：
//!     1、经典类多态
//!     2、C++回调
//!     3、C风格指针回调
//!     4、模板化编程
//!
//! 结束语：在四种C++实现中，我更推荐使用模板编程
//!
#include <iostream>
#include <functional>

using namespace std;

//===== 多态 =====
//抽象类
class abs_calc
{
public:
    abs_calc(size_t num1,size_t num2) : _num1(num1),_num2(num2) {}
    virtual size_t get_ret() = 0;

protected:
    size_t _num1;
    size_t _num2;
};

//实现类：加法
class add_calc : public abs_calc
{
public:
    add_calc(size_t num1,size_t num2) : abs_calc(num1,num2) {}
    size_t get_ret() override { return _num1 + _num2; }
};

//实现类：减法
class sub_calc : public abs_calc
{
public:
    sub_calc(size_t num1,size_t num2) : abs_calc(num1,num2) {}
    size_t get_ret() override { return _num1 - _num2; }
};

//父类虚函数调用的公共接口
void show_ret(abs_calc *obj)
{ cout << "show ret: " <<obj->get_ret() << endl; }

//多态测试
void test_1()
{
    //用多态实现计算器计算流程：经典方式 (测试用例，未释放已分配的内存)
    cout << "add_calc: ";
    show_ret(new add_calc(10,20));

    cout << "sub_calc: ";
    show_ret(new sub_calc(100,40));
    cout<<endl;
}
//===== 多态 =====



//===== 回调 =====
//回调函数的公共接口
void show_ret_func(function<size_t()> func)
{ cout << "show ret: " << func() << endl; }

//实现函数：加法
size_t add_func(size_t num1,size_t num2) { return num1 + num2; };

//实现函数：减法
size_t sub_func(size_t num1,size_t num2) { return num1 - num2; };

//回调测试
void test_2()
{
    //代替多态的方法之一：回调式
    cout << "add_func: ";
    show_ret_func(bind(&add_func,10,20));

    cout << "sub_func: ";
    show_ret_func(bind(&sub_func,100,40));
    cout<<endl;
}
//===== 回调 =====



//===== 指针回调 =====
//指针回调的公共接口
void show_ret_p
    (size_t(*func)(size_t num1,size_t num2),size_t num1,size_t num2)
{  cout << "show ret: " <<func(num1,num2) << endl; }

//加法实现
size_t add_c(size_t num1,size_t num2) { return num1 + num2; }

//减法实现
size_t sub_c(size_t num1,size_t num2) { return num1 - num2; }

//指针回调测试
void test_3()
{
    //代替多态的方法之一：指针回调 (C风格回调)
    cout << "add_p: ";
    show_ret_p(&add_c,10,20);

    cout << "sub_p: ";
    show_ret_p(&sub_c,100,40);
    cout<<endl;
}
//===== 指针回调 =====



//===== 模板 =====
//操作模型类
template<class Tclass>
class Tabs_cala
{
public:
    Tabs_cala(size_t num1,size_t num2) : _num1(num1),_num2(num2){}
    size_t get_ret(){ return Tfunc.get_ret(_num1,_num2); }; //鸭子函数

private:
    Tclass Tfunc;
    size_t _num1;
    size_t _num2;
};

//加法实现类
class Tadd_cala
{
public:
    size_t get_ret(size_t num1,size_t num2) { return num1 + num2; };
};

//减法实现类
class Tsub_cala
{
public:
    size_t get_ret(size_t num1,size_t num2) { return num1 - num2; };
};

//模板化调用的公共接口
template<class Tfunc>
void show_ret_template(Tabs_cala<Tfunc> obj)
{ cout << "show ret: " << obj.get_ret() << endl; }

//模板测试
void test_4()
{
    //代替多态的方法之一：模板式
    cout << "add_T: ";
    show_ret_template(Tabs_cala<Tadd_cala>(10,20));

    cout << "sub_T: ";
    show_ret_template(Tabs_cala<Tsub_cala>(100,40));
}
//===== 模板 =====



int main()
{
    cout<<"add: 10 + 20"<<endl;
    cout<<"sub: 100 - 40"<<endl;
    cout<<"===== begin ====="<<endl;
    test_1();//经典类多态
    test_2();//C++函数回调
    test_3();//C风格指针回调
    test_4();//模板化编程
    cout<<"===== end ====="<<endl;
    return 0;
}

/*
执行结果如下:
add: 10 + 20
sub: 100 - 40
===== begin =====
add_calc: show ret: 30
sub_calc: show ret: 60
add_func: show ret: 30
sub_func: show ret: 60
add_p: show ret: 30
sub_p: show ret: 60
add_T: show ret: 30
sub_T: show ret: 60
===== end =====
*/
