//===== for =====
//功能:直接打印(包括换行)或者提供快捷的for版本
//      提供对象版本和指针版本,后缀_p为指针版本
//for_show : 根据迭代器打印
//for_it : 返回迭代器自行操作
//for_i : 返回类型size_t的变量自行操作
//for_n : 制定执行循环次数,不希望返回迭代器
#define for_show(arr) \
    for(auto it=arr.begin();it!=arr.end();it++) \
    { std::cout<<*it<<std::endl; }

#define for_show_p(arr) \
    for(auto it=arr->begin();it!=arr->end();it++) \
    { std::cout<<*it<<std::endl; }

#define for_it(it,arr) \
    for(auto it=arr.begin();it!=arr.end();it++)

#define for_it_p(it,arr) \
    for(auto it=arr->begin();it!=arr->end();it++)

#define for_i(i,arr) \
    for(size_t i=0;i<arr.size();i++)

#define for_n(n) \
    for(size_t index_it_for=0;index_it_for<n;index_it_for++)
//===== for =====

#include <iostream>
#include <vector>
#include <list>
using namespace std;


int main()
{
//    list<int> ls;
//    ls.size();
    cout<<"===== begin ====="<<endl;

    cout<<"===== stm ====="<<endl;
    string a1 = "1-22-333-4444-55555";
    string a2 = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";

    cout<<"ret = "<<stm(a1)("-",0,2)<<endl;                     //一次切割，1-22-333
    cout<<"ret = "<<stm("1-22-333-4444-55555")("-",0,2)<<endl;  //支持直接赋值
    cout<<"ret = "<<stm(a1)("-",-1,-1)<<endl;                   //切割末尾段，必须反响切割，55555
    cout<<"ret = "<<stm(a1)("-",-3,-1)<<endl;                   //反响切割，333-4444-55555
    cout<<"ret = "<<stm(a2)(" ",1,1,"<<",-1,-1,">>",0,0)<<endl; //两次切割同一字符串，1991-12-3
    cout<<"===== stm =====\n"<<endl;

    cout<<"===== stmv =====\n"<<endl;
    string a = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";
    string b = "--11--22--33ashgdas pas<<1991-12-3>>da asdg ausd";
    string c = "11--22--33ashgdas pas<<1991-12-3>>da asdg ausd--";
    string d = "--11--22--33ashgdas pas<<1991-12-3>>da asdg ausd ";
    string e = "ajkwjfajknfjabcwjiabfjawabcabwfiagalkfabc";

    vector<string> vec1 = stmv(a)("--");        //只有一个分割符时
    vector<string> vec2 = stmv(b)("--"," ");    //传入多个分割符时
    vector<string> vec3 = stmv(c)("--"," ","<<");
    vector<string> vec4 = stmv(d)("--"," ","<<",">>");
    vector<string> vec5 = stmv(e)("abc","ab");

    cout<<"1========"<<endl;
    for_show(vec1);

    cout<<"2========"<<endl;
    for_show(vec2);

    cout<<"3========"<<endl;
    for_show(vec3);

    cout<<"4========"<<endl;
    for_show(vec4);

    cout<<"5========"<<endl;
    for_show(vec5);
    cout<<"===== stmv =====\n"<<endl;

    cout<<"===== end ====="<<endl;
    return 0;
}

/*
 * 测试结果：
 *
===== begin =====
===== stm =====
ret = 1-22-333
ret = 1-22-333
ret = 55555
ret = 333-4444-55555
ret = 1991-12-3
===== stm =====

===== stmv =====

1========
11
22
33ashgdas pas<<1991-12-3>>da asdg ausd
2========
11
22
33ashgdas
pas<<1991-12-3>>da
asdg
ausd
3========
11
22
33ashgdas
pas
1991-12-3>>da
asdg
ausd
4========
11
22
33ashgdas
pas
1991-12-3
da
asdg
ausd
5========
ajkwjfajknfj
wji
fjaw
wfiagalkf
===== stmv =====

===== end =====
*/

