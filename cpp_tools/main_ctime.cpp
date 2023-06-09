#include "../include/ctimel.h"
#include <iostream>

using namespace std;

void test_1()
{
    cout<<"===== test_1 ====="<<endl;

    auto func = [](){
        for(int i=0;i<10000;i++)
            for(int i=0;i<10000;i++){}
    };

    //创建临时区域，等待退出时打印
    {
        cout<<"===== 一次计算 ====="<<endl;
        ctimel tm;
        func();
    }
    {
        cout<<"===== 更新时间 ====="<<endl;
        ctimel tm;

        func();
        tm.show();
        tm.update();

        func();
        tm.show();
        tm.update();

        func();
        tm.show();
        tm.update();
    }
    {
        cout<<"===== 保存时间点等待未来打印 ====="<<endl;
        ctimel tm;

        func();
        auto s1 = tm.to_point();

        func();
        auto s2 = tm.to_point();

        cout<<"paint: "<<tm.to_string(s1)<<endl;
        cout<<"paint: "<<tm.to_string(s2)<<endl;
    }
    {
        cout<<"===== 多点时间测试 ====="<<endl;
        ctimel tm;

        func();
        tm.add_point();

        func();
        tm.add_point();

        func();
        tm.add_point();

        func();
        tm.add_point();

        tm.show_vec();
    }
    {
        cout<<"===== 多点时间测试并每次重新计时 ====="<<endl;
        ctimel tm;

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        func();
        tm.add_point_re();

        tm.show_vec();
    }
    {
        cout<<"===== 人类可视化时间:ctimes ====="<<endl;
        ctimes ct;
        cout<<ct.to_now()<<endl;
        cout<<ct.to_now("%H:%M:%S")<<endl; //自定义打印格式

        //保留时间点并在未来打印
        time_t t1 = ct.time_now();
        cout<<ct.to_format(t1)<<endl;
    }
}

int main()
{
    cout<<"===== begin ====="<<endl;
    test_1();

    cout<<"===== end ====="<<endl;
    return 0;
}


/*
 * 计算函数时间测试结果：
 *
===== begin =====
===== test_1 =====
===== 一次计算 =====
[nan: 288974927|mic: 288974|mil: 288|sec: 0]
===== 更新时间 =====
[nan: 291067824|mic: 291067|mil: 291|sec: 0]
[nan: 287929075|mic: 287929|mil: 287|sec: 0]
[nan: 287614887|mic: 287614|mil: 287|sec: 0]
[nan: 866809334|mic: 866809|mil: 866|sec: 0]
===== 保存时间点等待未来打印 =====
paint: [nan: 288384552|mic: 288384|mil: 288|sec: 0]
paint: [nan: 576289512|mic: 576289|mil: 576|sec: 0]
[nan: 576362262|mic: 576362|mil: 576|sec: 0]
===== 多点时间测试 =====
[nan: 304235552|mic: 304235|mil: 304|sec: 0]
[nan: 591549148|mic: 591549|mil: 591|sec: 0]
[nan: 878842238|mic: 878842|mil: 878|sec: 0]
[nan: 1166150134|mic: 1166150|mil: 1166|sec: 1]
[nan: 1166241424|mic: 1166241|mil: 1166|sec: 1]
===== 多点时间测试并每次重新计时 =====
[nan: 287540339|mic: 287540|mil: 287|sec: 0]
[nan: 286082889|mic: 286082|mil: 286|sec: 0]
[nan: 286608348|mic: 286608|mil: 286|sec: 0]
[nan: 286342874|mic: 286342|mil: 286|sec: 0]
[nan: 1146695529|mic: 1146695|mil: 1146|sec: 1]
===== 人类可视化时间:ctimes =====
2023-06-08 21:01:01
21:01:01
2023-06-08 21:01:01
===== end =====
*/
