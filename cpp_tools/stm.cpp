//!
//! C++工具：字符串切割工具--根据分割符切割
//!
//! ===== 工具简介 =====
//! C++标准库中缺少对字符串的处理工具，使得很多时候对字符串处理都相当麻烦，
//!     除了手写之外必须依赖庞大的外部库
//! 本次提供的是用法简单的字符串切割工具类：
//!     stm  :  该类传入字符串完成切割后返回一个字符串，可对同一字符串多次切割
//!                 切割方式：传入分割符和切割的区间（可多次传入）
//!     stmv :  该类传入字符串完成切割后返回vector数组，根据切割符将内容填入容器
//!                 切割方式：传入分割符（可多次传入）
//! ===== 工具简介 =====
//!
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//===== stm =====
//功能:字符串切割,主要针对单字符串的多重切割,支持不同切割符号,支持正反双向切割,
//      无法切割到分割符最后一段字符串，如果需要最后一段字符可以反向切割
//算法:查找标记位置,向前取值,舍弃标记末尾数据
struct stm
{
    string v_str;
    stm(const string &str) : v_str(str){}

    template<class ...Tarr> string split_t(){ return v_str; }
    template<class ...Tarr>
    string split_t(const string &flg,int begin,int end,const Tarr &...arg)
    {
        if((begin < 0) && (end < 0)) v_str = split_back(v_str,flg,begin,end);
        else v_str = split_go(v_str,flg,begin,end);
        return split_t(arg...);
    }

    template<class ...Tarr>
    string operator()(const Tarr &...arg) { return split_t(arg...); }

    //反向切割:反向参数传递
    string split_back(const string &str,const string &flg, int end,int begin)
    {
        begin *= -1; begin -= 1; end *= -1;
        int len = end - begin;
        if((begin < 0) || (len <= 0)) return "";

        size_t pos_begin = str.size();
        for(int i=0;i<begin;i++)
        {
            if((pos_begin = str.rfind(flg,pos_begin)) == str.npos) return "";
            pos_begin -= 1;
        }

        size_t pos_end = pos_begin;
        for(int i=0;i<len;i++)
        {
            if((pos_end = str.rfind(flg,pos_end)) == str.npos) return "";
            pos_end -= 1;
        }

        if(pos_begin != str.size()) pos_begin += 1;
        pos_end += flg.size() +1;

        if(pos_begin <= pos_end) return "";
        return string (str.begin()+pos_end,str.begin()+pos_begin);
    }

    //正向切割:正向参数传递
    string split_go(const string &str,const string &flg, int begin,int end)
    {
        int len = (end - begin) + 1;
        if((begin < 0) || (len <= 0)) return "";

        size_t pos_begin = 0;
        for(int i=0;i<begin;i++)
        {
            if((pos_begin = str.find(flg,pos_begin)) == str.npos) return "";
            pos_begin += flg.size();
        }

        size_t pos_end = pos_begin;
        for(int i=0;i<len;i++)
        {
            if((pos_end = str.find(flg,pos_end)) == str.npos) return "";
            pos_end += flg.size();
        }

        pos_end -= flg.size();
        if(pos_begin >= pos_end) return "";
        return string(str.begin()+pos_begin,str.begin()+pos_end);
    }
};
//===== stm =====


//===== stmv =====
//功能:字符串切割,按分隔符将字符串切割到数组
//算法:利用vector<bool>生成与字符串一样长的标记位
//      切割算法扫描到切割符时将vector<bool>对应标记位置1(切割符占领位)
//      然后将连续0段加入结果数组
struct stmv
{
    string v_str;
    vector<string> vec_flg;
    vector<bool> vec_bit;

    stmv(const string &str) : v_str(str) { vec_bit.resize(str.size(),false); }
    template<class ...Tarr>
    vector<string> operator()(const Tarr &...arg) { return push_flg(arg...); }

    //获取切割符
    template<class ...Tarr> vector<string> push_flg()
    { return split_value(v_str,vec_flg); }
    template<class ...Tarr>
    vector<string> push_flg(const string &flg,Tarr ...arg)
    { vec_flg.push_back(flg); return push_flg(arg...); };

    //根据标记切割字符串
    vector<string> split_value(const string &in_str,const vector<string> &in_flg)
    {
        //标记数循环
        vector<string> vec;
        for(size_t iflg=0;iflg<in_flg.size();iflg++)
        {
            //字符串标记排查,存在用bit标记
            size_t pos_begin = 0;
            while(true)
            {
                pos_begin = in_str.find(in_flg[iflg],pos_begin);
                if(pos_begin != in_str.npos)
                {
                    for(size_t il=0;il<in_flg[iflg].size();il++)
                    { vec_bit[pos_begin+il]=1; }
                    pos_begin+=1;
                }
                else break;
            }
        }

        //根据0/1状态获取字符串,加入返回结果
        string str;
        for(size_t i=0;i<vec_bit.size();i++)
        {
            if(vec_bit[i] == false)
            {
                if(i>0 && (vec_bit[i-1] == true)) str.clear();
                str+=in_str[i];
            }
            else if(i>0 && (vec_bit[i-1] == false)) vec.push_back(str);
        }

        //末尾无状态转跳时加入结果
        if(vec_bit[vec_bit.size()-1] == false)
        { vec.push_back(str); }
        return vec;
    }
};
//===== stmv =====


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


int main()
{
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

