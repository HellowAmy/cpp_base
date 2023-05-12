//!
//! Tlist_Tapi.h
//!
#ifndef TLIST_TAPI_H
#define TLIST_TAPI_H

#include <stdlib.h>
#include <string.h>

//未使用变量警告去除
#define Tun(x) (void)(x)

//链表遍历宏
#define Tfor_each(ls,it,Tapi)  \
    for(Tls_##Tapi *it = ls->head->next;it!=NULL;it=it->next)

//定义函数代码生成宏
#define Tlist_Tbuild(Tapi,Tvalue)                                               \
typedef struct Tls_##Tapi Tls_##Tapi;                                           \
struct Tls_##Tapi                                                               \
{                                                                               \
    Tvalue  value;                                                              \
    Tls_##Tapi *prev;                                                           \
    Tls_##Tapi *next;                                                           \
};                                                                              \
                                                                                \
typedef struct Tlist_##Tapi Tlist_##Tapi;                                       \
struct Tlist_##Tapi                                                             \
{                                                                               \
    void    (*alloc)       (Tls_##Tapi *head,const Tvalue value);               \
    void    (*free)        (Tls_##Tapi *head);                                  \
    void    (*init)        (Tls_##Tapi *head);                                  \
    void    (*push_back)   (Tlist_##Tapi *head,const Tvalue value);             \
    void    (*push_front)  (Tlist_##Tapi *head,const Tvalue value);             \
    int     (*insert)      (Tlist_##Tapi *head,size_t pos,const Tvalue value);  \
    void    (*pop_back)    (Tlist_##Tapi *head);                                \
    void    (*pop_front)   (Tlist_##Tapi *head);                                \
    int     (*erase)       (Tlist_##Tapi *head,size_t pos);                     \
    void    (*clear)       (Tlist_##Tapi *head);                                \
    size_t  size;                                                               \
    Tls_##Tapi *head;                                                           \
};                                                                              \
                                                                                \
__attribute__((unused))                                                         \
static void list_init_##Tapi(Tls_##Tapi *head)                                  \
{                                                                               \
    if(head == NULL) return;                                                    \
    head->prev = NULL;                                                          \
    head->next = NULL;                                                          \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void list_push_back_##Tapi(Tlist_##Tapi *ls,const Tvalue value)          \
{                                                                               \
    if(ls->head == NULL) return;                                                \
    Tls_##Tapi *new_value = (Tls_##Tapi *)malloc(sizeof(Tls_##Tapi));           \
    list_init_##Tapi(new_value);                                                \
                                                                                \
    if(ls->size == 0)                                                           \
    {                                                                           \
        ls->head->prev = new_value;                                             \
        ls->head->next = new_value;                                             \
        new_value->prev = ls->head;                                             \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        new_value->prev = ls->head->prev;                                       \
        ls->head->prev->next = new_value;                                       \
        ls->head->prev = new_value;                                             \
    }                                                                           \
    ls->size += 1;                                                              \
    ls->alloc(new_value,value);                                                 \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void list_push_front_##Tapi(Tlist_##Tapi *ls,const Tvalue value)         \
{                                                                               \
    if(ls->head == NULL) return;                                                \
    Tls_##Tapi *new_value = (Tls_##Tapi *)malloc(sizeof(Tls_##Tapi));           \
    list_init_##Tapi(new_value);                                                \
                                                                                \
    if(ls->size == 0)                                                           \
    {                                                                           \
        ls->head->prev = new_value;                                             \
        ls->head->next = new_value;                                             \
        new_value->prev = ls->head;                                             \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        ls->head->next->prev = new_value;                                       \
        new_value->next = ls->head->next;                                       \
        new_value->prev = ls->head;                                             \
        ls->head->next = new_value;                                             \
    }                                                                           \
    ls->size += 1;                                                              \
    ls->alloc(new_value,value);                                                 \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static int list_insert_##Tapi(Tlist_##Tapi *ls,size_t pos,const Tvalue value)   \
{                                                                               \
    if(ls->head == NULL || pos >= ls->size) return 0;                           \
    Tls_##Tapi *new_value = (Tls_##Tapi *)malloc(sizeof(Tls_##Tapi));           \
    list_init_##Tapi(new_value);                                                \
                                                                                \
    size_t count = 0;                                                           \
    Tls_##Tapi *it = ls->head->next;                                            \
    for(;it != NULL;it = it->next)                                              \
    {                                                                           \
        if(count == pos) break;                                                 \
        count++;                                                                \
    }                                                                           \
                                                                                \
    new_value->next = it;                                                       \
    new_value->prev = it->prev;                                                 \
    it->prev->next = new_value;                                                 \
    it->prev = new_value;                                                       \
                                                                                \
    ls->size += 1;                                                              \
    ls->alloc(new_value,value);                                                 \
    return 1;                                                                   \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void list_pop_back_##Tapi(Tlist_##Tapi *ls)                              \
{                                                                               \
    if(ls->head == NULL || ls->size == 0) return ;                              \
    ls->size -= 1;                                                              \
                                                                                \
    Tls_##Tapi *end = ls->head->prev;                                           \
    ls->head->prev = end->prev;                                                 \
    end->prev->next = NULL;                                                     \
    ls->free(end);                                                              \
    free(end);                                                                  \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void list_pop_front_##Tapi(Tlist_##Tapi *ls)                             \
{                                                                               \
    if(ls->head == NULL || ls->size == 0) return ;                              \
    ls->size -= 1;                                                              \
                                                                                \
    Tls_##Tapi *begin = NULL;                                                   \
    if(ls->size == 0)                                                           \
    {                                                                           \
        begin = ls->head->next;                                                 \
        ls->head->next = NULL;                                                  \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        begin = ls->head->next;                                                 \
        ls->head->next = begin->next;                                           \
        begin->next->prev = ls->head;                                           \
    }                                                                           \
    ls->free(begin);                                                            \
    free(begin);                                                                \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static int list_erase_##Tapi(Tlist_##Tapi *ls,size_t pos)                       \
{                                                                               \
    if(ls->head == NULL || ls->size == 0 || pos >= ls->size) return 0;          \
    ls->size -= 1;                                                              \
                                                                                \
    size_t count = 0;                                                           \
    Tls_##Tapi *it = ls->head->next;                                            \
    for(;it != NULL;it = it->next)                                              \
    {                                                                           \
        if(count == pos) break;                                                 \
        count++;                                                                \
    }                                                                           \
                                                                                \
    Tls_##Tapi *now = NULL;                                                     \
    if(pos == ls->size || ls->size == 0)                                        \
    {                                                                           \
        now = it;                                                               \
        it->prev->next = NULL;                                                  \
        ls->head->prev = it->prev;                                              \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        now = it;                                                               \
        it->prev->next = it->next;                                              \
        it->next->prev = it->prev;                                              \
                                                                                \
    }                                                                           \
    ls->free(now);                                                              \
    free(now);                                                                  \
    return 1;                                                                   \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void list_clear_##Tapi(Tlist_##Tapi *ls)                                 \
{                                                                               \
    if(ls->head == NULL) return;                                                \
    while(1)                                                                    \
    {                                                                           \
        Tls_##Tapi *begin = ls->head->next;                                     \
        if(begin == NULL) break;                                                \
        ls->head->next = ls->head->next->next;                                  \
        ls->free(begin);                                                        \
        free(begin);                                                            \
    }                                                                           \
    ls->size = 0;                                                               \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static Tlist_##Tapi* Topen_##Tapi                                               \
    (void (*alloc)(Tls_##Tapi *head,const Tvalue value),                        \
     void (*free)(Tls_##Tapi *head))                                            \
{                                                                               \
    Tlist_##Tapi *ls = (Tlist_##Tapi *)malloc(sizeof(Tlist_##Tapi));            \
    ls->head = (Tls_##Tapi*)malloc(sizeof(Tls_##Tapi));                         \
                                                                                \
    ls->alloc       = alloc;                                                    \
    ls->free        = free;                                                     \
    ls->init        = list_init_##Tapi;                                         \
    ls->push_back   = list_push_back_##Tapi;                                    \
    ls->push_front  = list_push_front_##Tapi;                                   \
    ls->insert      = list_insert_##Tapi;                                       \
    ls->pop_back    = list_pop_back_##Tapi;                                     \
    ls->pop_front   = list_pop_front_##Tapi;                                    \
    ls->erase       = list_erase_##Tapi;                                        \
    ls->clear       = list_clear_##Tapi;                                        \
                                                                                \
    ls->size = 0;                                                               \
    ls->init(ls->head);                                                         \
    return ls;                                                                  \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static void Tclose_##Tapi(Tlist_##Tapi *ls)                                     \
{                                                                               \
    ls->clear(ls);                                                              \
    free(ls->head);                                                             \
    free(ls);                                                                   \
}                                                                               \
                                                                                \
__attribute__((unused))                                                         \
static Tlist_##Tapi* Tclone_##Tapi(Tlist_##Tapi *ls)                            \
{                                                                               \
    if(ls == NULL) return NULL;                                                 \
    Tlist_##Tapi *ls_new = Topen_##Tapi(ls->alloc,ls->free);                    \
                                                                                \
    for(Tls_##Tapi *it = ls->head->next;it!=NULL;it=it->next)                   \
    {                                                                           \
        ls_new->push_back(ls_new,it->value);                                    \
    }                                                                           \
    return ls_new;                                                              \
}                                                                               \


//定义生成浅拷贝分配器
#define Tac_Tdefault(Tapi,Tvalue)                                           \
__attribute__((unused))                                                     \
static void Tmalloc_##Tapi(Tls_##Tapi *head,const Tvalue value)             \
{ head->value = (Tvalue)value; }                                            \
__attribute__((unused))                                                     \
static void Tfree_##Tapi(Tls_##Tapi *head)                                  \
{ Tun(head); }                                                              \

//定义生成字符串分配器
#define Tac_Tcharp(Tapi)                                                    \
__attribute__((unused))                                                     \
static void Tmalloc_Tcharp(Tls_##Tapi *head,const char* value)              \
{                                                                           \
    head->value = (char*)malloc(strlen(value) +1);                          \
    strncpy(head->value,value,strlen(value));                               \
    head->value[strlen(value)] = '\0';                                      \
}                                                                           \
__attribute__((unused))                                                     \
static void Tfree_Tcharp(Tls_##Tapi *head)                                  \
{ free(head->value); }                                                      \


//== 定义生成对应类型代码 + 浅拷贝分配器 ==
#define Tls_Tbase(Tapi,Tvalue)                  \
    Tlist_Tbuild(Tapi,Tvalue)                   \
    Tac_Tdefault(Tapi,Tvalue)                   \

//== 定义生成对应类型代码 + 字符串分配器 ==
#define Tls_Tcharp()                            \
    Tlist_Tbuild(Tcharp,char *)                 \
    Tac_Tcharp(Tcharp)                          \

//===== 最终代码生成 =====
//基础类型(浅拷贝存储)
Tls_Tbase(Tchar,char)
Tls_Tbase(Tshort,short)
Tls_Tbase(Tint,int)
Tls_Tbase(Tlong,long)
Tls_Tbase(Tlonglong,long long)
Tls_Tbase(Tfloat,float)
Tls_Tbase(Tdouble,double)

//字符串（深拷贝特例）
Tls_Tcharp()
//===== 最终代码生成 =====

//== 快速构造的工厂宏 ==
#define Tlist_make(Tapi) Topen_##Tapi(Tmalloc_##Tapi,Tfree_##Tapi)

#endif // TLIST_TAPI_H
