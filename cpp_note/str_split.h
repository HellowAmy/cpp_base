#ifndef STR_SPLIT_H
#define STR_SPLIT_H

#include <string.h>
#include <stdlib.h>

//! 功能:查找字符串中的子串
//! 参数1：查找的字符串
//! 参数2：子串
//! 参数3：开始查找的下标
//! 返回值：找到子串的下标
int find_c(const char* str,const char* flg,int index)
{
    size_t count = 0;
    size_t len_s = strlen(str);
    size_t len_f = strlen(flg);
    if(len_s < len_f) return -1;

    for(size_t i=index;i<len_s;i++)
    {
        for(size_t j=0;j<len_f;j++)
        {
            if(flg[j] == str[j+i])
            {
                count++;
                if(count == len_f) return i;
            }
            else count = 0;
        }
    }
    return -1;
}

//!功能:通过分隔符分割字符串,可以指定范围
//!参数1：需要切割的字符串
//!参数2：分割符
//!参数3：开始下标
//!参数4：结束下标
//!返回值:分割的字符串（找不到时为NULL）
//!使用:("123##456",[0:123]##[1:456]) (##qwe##asd##,[0:][1:qwe][2:asd][3:])
char* split_ac(const char *str,const char *flg, int begin,int end)
{
    if(begin > end) return NULL;
    const size_t len_f = strlen(flg);
    const size_t len_s = strlen(str);
    int pos_end = -1;
    int pos_begin = -1;

    //前标定位
    {
        int pos_go = 0;
        for(int i=0;i<end+1;i++)
        {
            if(i != 0) pos_go += len_f;
            pos_go = find_c(str,flg,pos_go);
            if(pos_go == -1)
            {
                if(i == end)
                {
                    pos_end = len_s;
                }
                break;
            }
        }
        if(pos_end != (int)len_s) pos_end = pos_go;
    }

    //后标定位
    {
        int pos_go = 0;
        for(int i=0;i<begin;i++)
        {
            pos_go = find_c(str,flg,pos_go);
            pos_go += len_f;
        }
        pos_begin = pos_go;
    }
    if(pos_end == -1 || pos_begin == -1 || (pos_end - pos_begin <= 0)) return NULL;

    //获取前标和后标之间的内容
    size_t len = pos_end - pos_begin;
    char *buf = (char*)malloc(len+1);
    buf[len] = '\0';
    for(size_t i=0;i< len;i++)
    {
        buf[i] = str[pos_begin+i];
    }
    return buf;
}

#endif // STR_SPLIT_H
