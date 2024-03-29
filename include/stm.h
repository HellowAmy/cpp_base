//!
//! stm.h
//!
#ifndef STM_H
#define STM_H

#include <string>
#include <vector>
#include <iostream>

//===== stm =====
//! 功能:字符串切割,主要针对单字符串的多重切割,支持不同切割符号,支持正反双向切割,
//!       无法切割到分割符最后一段字符串，如果需要最后一段字符可以反向切割
//! 算法:查找标记位置,向前取值,舍弃标记末尾数据
//!
//! 例子：
//!     string str = "123-456"
//!     string ret = stm(str)("-",0,0);
//!
struct stm
{
    std::string v_str;
    stm(const std::string &str) : v_str(str){}

    template<class ...Tarr> std::string split_t(){ return v_str; }
    template<class ...Tarr>
    std::string split_t(const std::string &flg,int begin,int end,const Tarr &...arg)
    {
        if((begin < 0) && (end < 0)) v_str = split_back(v_str,flg,begin,end);
        else v_str = split_go(v_str,flg,begin,end);
        return split_t(arg...);
    }

    template<class ...Tarr>
    std::string operator()(const Tarr &...arg) { return split_t(arg...); }

    //反向切割:反向参数传递
    std::string split_back(const std::string &str,const std::string &flg, int end,int begin)
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
        return std::string (str.begin()+pos_end,str.begin()+pos_begin);
    }

    //正向切割:正向参数传递
    std::string split_go(const std::string &str,const std::string &flg, int begin,int end)
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
        return std::string(str.begin()+pos_begin,str.begin()+pos_end);
    }
};
//===== stm =====


//===== stmv =====
//! 功能:字符串切割,按分隔符将字符串切割到数组
//! 算法:利用vector<bool>生成与字符串一样长的标记位
//!       切割算法扫描到切割符时将vector<bool>对应标记位置1(切割符占领位)
//!       然后将连续0段加入结果数组
//!
//! 例子：
//!     string str = "123-456##789_aa"
//!     vector<string> ret = stm(str)("-","_","##");
struct stmv
{
    std::string v_str;
    std::vector<std::string> vec_flg;
    std::vector<bool> vec_bit;

    stmv(const std::string &str) : v_str(str) { vec_bit.resize(str.size(),false); }
    template<class ...Tarr>
    std::vector<std::string> operator()(const Tarr &...arg) { return push_flg(arg...); }

    //获取切割符
    template<class ...Tarr> std::vector<std::string> push_flg()
    { return split_value(v_str,vec_flg); }
    template<class ...Tarr>
    std::vector<std::string> push_flg(const std::string &flg,Tarr ...arg)
    { vec_flg.push_back(flg); return push_flg(arg...); };

    //根据标记切割字符串
    std::vector<std::string> split_value
        (const std::string &in_str,const std::vector<std::string> &in_flg)
    {
        //标记数循环
        std::vector<std::string> vec;
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
        std::string str;
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

#endif // STM_H
