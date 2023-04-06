#ifndef UX_PROTOCOL_H
#define UX_PROTOCOL_H

#include "ux_tcp.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


//===== stmv =====
//功能:字符串切割,按分隔符将字符串切割到数组
//算法:利用vector<bool>生成与字符串一样长的标记位
//      切割算法扫描到切割符时将vector<bool>对应标记位置1(切割符占领位)
//      然后将连续0段加入结果数组
//用法示例:
//      [1]
//      string a = "11--22--33";
//      string b = "11--22++33";
//      string c = "11 22 33 44++55--66";
//      vector<string> vec = vts::stmv(a)("--");
//      [ret = 11,22,33]
//      vector<string> vec1 = vts::stmv(b)("--");
//      [ret = 11,22++33]
//      vector<string> vec2 = vts::stmv(c)(" ","++","--");
//      [ret = 11,22,33,44,55,66]
//
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
        vector<string> vec;

        //标记数循环
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


//== 文件传输结构体 ==
struct ct_msg
{
    bool is_file;       //判断是否为文件（发送文件或者发送信息）
    bool is_begin;      //首次发送
    bool is_end;        //发送结束
    size_t size_buf;    //本次发送的buf真实长度
    size_t size_file;   //文件总长度，首次发送时附带
    char filename[256]; //文件名，首次发送时附带
    char buf[4096];     //文件内容或者信息内容
};
//== 文件传输结构体 ==


//== 发送文件 ==
bool send_file(const string &filename,shared_ptr<channel> pch)
{
    ct_msg ct;
    memset(&ct,0,sizeof(ct));
    ct.is_file = true;

    fstream ofs(filename,ios::in);
    if(ofs.is_open())
    {
        //偏移到文件末尾获取文件总长度并返回起点
        ofs.seekg(0,ios::end);
        ct.size_file = ofs.tellg();
        ofs.seekg(0,ios::beg);

        //首次发送
        ct.is_begin = true;
        strncpy(ct.filename,filename.c_str(),sizeof(ct.filename));
        pch->send_msg(ct_s<ct_msg>(ct));

        //发送文件内容
        ct.is_begin = false;
        ct.is_end = false;
        while(ofs.eof() == false)
        {
            //读取内容到buf与记录buf字节数
            ofs.read(ct.buf,sizeof(ct.buf));
            ct.size_buf = ofs.gcount();
            pch->send_msg(ct_s<ct_msg>(ct));
        }
        ofs.close();

        //最后一次发送
        if(ofs.eof()) { ct.is_end = true; }
        pch->send_msg(ct_s<ct_msg>(ct));
    }
    else return false;
    return true;
}


//== 发送消息 ==
bool send_txt(const string &str,shared_ptr<channel> pch)
{
    ct_msg ct;
    memset(&ct,0,sizeof(ct));
    ct.is_file = false;

    strncpy(ct.buf,str.c_str(),sizeof(ct.buf));
    return pch->send_msg(ct_s<ct_msg>(ct));
}

//== 解析传输内容 ==
void parse_msg(const string &msg)
{
    ct_msg ct = st_c<ct_msg>(msg);
    if(ct.is_file)
    {
        static fstream ofs;
        static size_t size_len = 0;

        //首次接收
        if(ct.is_begin)
        {
            vlogd("== begin ==");
            ofs.open(string(ct.filename),ios::out);
            if(ofs.is_open() == false) { vlogw("== open err =="); }
            return; //提前返回
        }

        //最后一次
        if(ct.is_end)
        {
            vlogd("== end ==");
            ofs.close();
            size_t tm = size_len;
            size_len = 0;
            if(tm != ct.size_file) { vlogw("not equal: " vv(tm) vv(ct.size_file)); }
            return; //提前返回
        }

        //发送中
        if(ofs.is_open())
        {
            ofs.write(ct.buf,ct.size_buf);
            size_len += ofs.tellg();
        }
    }
    else { cout<<"read: "<<ct.buf<<endl;}
}

//== 解析命令 ==
bool parse_cmd(const string &cmd,shared_ptr<channel> pch)
{
    ct_msg ct;
    memset(&ct,0,sizeof(ct));
    vector<string> vec = stmv(cmd)(":"); //解析出分割符内容

    if(vec.size() < 2) { return send_txt(cmd,pch); }            //发送信息
    else if(vec[0] == "file") { return send_file(vec[1],pch); } //发送文件
    else return false;
}

#endif // UX_PROTOCOL_H
