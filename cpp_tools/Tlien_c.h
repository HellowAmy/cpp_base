//===== 测试文件部分 =====
#if 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//===== Tlien_c =====
//遍历宏: 参数为带数据的链表指针
#define Tlist_for(it,head) for(Tlist_c *it = head.next;it != NULL;it = it->next)
#define Tlist_for_p(it,head) for(Tlist_c *it = head->next;it != NULL;it = it->next)


//typedef struct dire dire;
//struct dire
//{
//    int a;
//    int b;
//    int c;
//    char cd[100];
//};


typedef struct msg msg;
struct msg
{
    int len;
    char *type;
    char data[100];
};

#endif




#if 0

//!
//! Tlien_c.h
//!
#ifndef TLIEN_C_H
#define TLIEN_C_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//===== Tlien_c =====
#define Tlist_for(head,it,Tapi) for(Tlist_##Tapi *it = head.next;it != NULL;it = it->next)
#define Tlist_for_p(head,it,Tapi) for(Tlist_##Tapi *it = head->next;it != NULL;it = it->next)

#define Tlist_create(Tapi,Tvalue)                                                          \
typedef struct Tlist_##Tapi Tlist_##Tapi;                                                   \
struct Tlist_##Tapi                                                                         \
{                                                                                           \
    Tvalue value;                                                                           \
    size_t size;                                                                            \
    Tlist_##Tapi *prev;                                                                     \
    Tlist_##Tapi *next;                                                                     \
};                                                                                          \
                                                                                            \
void list_init_##Tapi(Tlist_##Tapi *head)                                                   \
{                                                                                           \
    if(head == NULL) return;                                                                \
    head->size = 0;                                                                         \
    head->prev = NULL;                                                                      \
    head->next = NULL;                                                                      \
}                                                                                           \
                                                                                            \
void list_push_back_##Tapi(Tlist_##Tapi *head,Tvalue value)                                 \
{                                                                                           \
    if(head == NULL) return;                                                                \
    Tlist_##Tapi *new_value = (Tlist_##Tapi *)malloc(sizeof(Tlist_##Tapi));                 \
    list_init_##Tapi(new_value);                                                            \
                                                                                            \
    if(head->size == 0)                                                                     \
    {                                                                                       \
        head->prev = new_value;                                                             \
        head->next = new_value;                                                             \
        new_value->prev = head;                                                             \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        new_value->prev = head->prev;                                                       \
        head->prev->next = new_value;                                                       \
        head->prev = new_value;                                                             \
    }                                                                                       \
    head->size += 1;                                                                        \
    new_value->value = value;                                                               \
}                                                                                           \
                                                                                            \
void list_push_front_##Tapi(Tlist_##Tapi *head,Tvalue value)                                \
{                                                                                           \
    if(head == NULL) return;                                                                \
    Tlist_##Tapi *new_value = (Tlist_##Tapi *)malloc(sizeof(Tlist_##Tapi));                 \
    list_init_##Tapi(new_value);                                                            \
                                                                                            \
    if(head->size == 0)                                                                     \
    {                                                                                       \
        head->prev = new_value;                                                             \
        head->next = new_value;                                                             \
        new_value->prev = head;                                                             \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        head->next->prev = new_value;                                                       \
        new_value->next = head->next;                                                       \
        new_value->prev = head;                                                             \
        head->next = new_value;                                                             \
    }                                                                                       \
    head->size += 1;                                                                        \
    new_value->value = value;                                                               \
}                                                                                           \
                                                                                            \
int list_insert_##Tapi(Tlist_##Tapi *head,size_t pos,Tvalue value)                          \
{                                                                                           \
    if(head == NULL || pos >= head->size) return 0;                                         \
    Tlist_##Tapi *new_value = (Tlist_##Tapi *)malloc(sizeof(Tlist_##Tapi));                 \
    list_init_##Tapi(new_value);                                                            \
                                                                                            \
    size_t count = 0;                                                                       \
    Tlist_##Tapi *it = head->next;                                                          \
    for(;it != NULL;it = it->next)                                                          \
    {                                                                                       \
        if(count == pos) break;                                                             \
        count++;                                                                            \
    }                                                                                       \
                                                                                            \
    new_value->next = it;                                                                   \
    new_value->prev = it->prev;                                                             \
    it->prev->next = new_value;                                                             \
    it->prev = new_value;                                                                   \
                                                                                            \
    head->size += 1;                                                                        \
    new_value->value = value;                                                               \
    return 1;                                                                               \
}                                                                                           \
                                                                                            \
void list_pop_back_##Tapi(Tlist_##Tapi *head)                                               \
{                                                                                           \
    if(head == NULL || head->size == 0) return ;                                            \
    head->size -= 1;                                                                        \
                                                                                            \
    Tlist_##Tapi *end = head->prev;                                                         \
    head->prev = end->prev;                                                                 \
    end->prev->next = NULL;                                                                 \
    free(end);                                                                              \
}                                                                                           \
                                                                                            \
void list_pop_front_##Tapi(Tlist_##Tapi *head)                                              \
{                                                                                           \
    if(head == NULL || head->size == 0) return ;                                            \
    head->size -= 1;                                                                        \
                                                                                            \
    if(head->size == 0)                                                                     \
    {                                                                                       \
        Tlist_##Tapi *begin = head->next;                                                   \
        head->next = NULL;                                                                  \
        free(begin);                                                                        \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        Tlist_##Tapi *begin = head->next;                                                   \
        head->next = begin->next;                                                           \
        begin->next->prev = head;                                                           \
        free(begin);                                                                        \
    }                                                                                       \
}                                                                                           \
                                                                                            \
int list_erase_##Tapi(Tlist_##Tapi *head,size_t pos)                                        \
{                                                                                           \
    if(head == NULL || head->size == 0 || pos >= head->size) return 0;                      \
    head->size -= 1;                                                                        \
                                                                                            \
    size_t count = 0;                                                                       \
    Tlist_##Tapi *it = head->next;                                                          \
    for(;it != NULL;it = it->next)                                                          \
    {                                                                                       \
        if(count == pos) break;                                                             \
        count++;                                                                            \
    }                                                                                       \
                                                                                            \
    if(pos == head->size || head->size == 0)                                                \
    {                                                                                       \
        Tlist_##Tapi *now = it;                                                             \
        it->prev->next = NULL;                                                              \
        head->prev = it->prev;                                                              \
        free(now);                                                                          \
        return 1;                                                                           \
    }                                                                                       \
                                                                                            \
    Tlist_##Tapi *now = it;                                                                 \
    it->prev->next = it->next;                                                              \
    it->next->prev = it->prev;                                                              \
    free(now);                                                                              \
    return 1;                                                                               \
}                                                                                           \
                                                                                            \
void list_clear_##Tapi(Tlist_##Tapi *head)                                                  \
{                                                                                           \
    if(head == NULL) return;                                                                \
    while(1)                                                                                \
    {                                                                                       \
        Tlist_##Tapi *begin = head->next;                                                   \
        if(begin == NULL) break;                                                            \
        head->next = head->next->next;                                                      \
        free(begin);                                                                        \
    }                                                                                       \
    head->size = 0;                                                                         \
}                                                                                                                                                                                \
//===== Tlien_c =====

////== 模板生成(放在头文件会重复包含) ==
////基本类型
//Tlist_create(Tchar,char)
//Tlist_create(Tshort,short)
//Tlist_create(Tint,int)
//Tlist_create(Tlong,long)
//Tlist_create(Tlonglong,long long)
//Tlist_create(Tfloat,float)
//Tlist_create(Tdouble,double)
//Tlist_create(Tcharp,char *) //字符串
////== 模板生成 ==

#endif // TLIEN_C_H

#endif



//===== 测试文件部分 =====
#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//===== Tlien_c =====
//遍历宏: 参数为带数据的链表指针
#define Tlist_for(it,head) for(Tlist_c *it = head.next;it != NULL;it = it->next)
#define Tlist_for_p(it,head) for(Tlist_c *it = head->next;it != NULL;it = it->next)

//类型宏
#define Tvalue char *

typedef struct Tlist_c Tlist_c;
struct Tlist_c
{
    Tvalue value;
    size_t size;
    Tlist_c *prev;
    Tlist_c *next;
};

void list_init(Tlist_c *head)
{
    if(head == NULL) return;
    head->size = 0;
    head->prev = NULL;
    head->next = NULL;
}

void list_push_back(Tlist_c *head,Tvalue value)
{
    //分配空间
    if(head == NULL) return;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //连接到头节点
    if(head->size == 0)
    {
        //1.头节点的前后指针,指到新节点
        //2.新节点前指针,指到头节点
        head->prev = new_value;
        head->next = new_value;
        new_value->prev = head;
    }
    else
    {
        //1.新节点前节点,指向头节点前指针(头节点前指针等于尾节点)
        //2.尾节点的后指针,指到新节点
        //3.头节点前指针,指到新节点(等于将新节点当做尾节点)
        new_value->prev = head->prev;
        head->prev->next = new_value;
        head->prev = new_value;
    }
    head->size += 1;           //计数
    new_value->value = value;  //赋值
}

void list_push_front(Tlist_c *head,Tvalue value)
{
    //分配空间
    if(head == NULL) return;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //连接到头节点
    if(head->size == 0)
    {
        //参考: list_push_back
        head->prev = new_value;
        head->next = new_value;
        new_value->prev = head;
    }
    else
    {
        //1.第一个带数据节点的前指针,指向新节点
        //2.新节点的后指针,指向第一个带数据节点
        //3.新节点的前指针,指向头节点
        //4.头节点的后指针,指向新节点
        head->next->prev = new_value;
        new_value->next = head->next;
        new_value->prev = head;
        head->next = new_value;
    }
    head->size += 1;           //计数
    new_value->value = value;  //赋值
}

int list_insert(Tlist_c *head,size_t pos,Tvalue value)
{
    //分配空间
    if(head == NULL || pos >= head->size) return 0;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //获取指定的当前节点
    size_t count = 0;
    Tlist_c *it = head->next;
    for(;it != NULL;it = it->next)
    {
        if(count == pos) break;
        count++;
    }

    //1.新节点后指针,指向当前节点
    //2.新节点前指针,指向当前节点的前指针(等于当前节点的前节点)
    //3.当前节点的前节点的后指针,指向新节点
    //4.当前节点前指针,指向新节点
    new_value->next = it;
    new_value->prev = it->prev;
    it->prev->next = new_value;
    it->prev = new_value;

    head->size += 1;           //计数
    new_value->value = value;  //赋值
    return 1;
}

void list_pop_back(Tlist_c *head)
{
    if(head == NULL || head->size == 0) return ;
    head->size -= 1; //计数移除

    //1.保留尾节点
    //2.头节点的前指针,指向尾节点的前一个
    //3.尾节点的前一个的后指针,指向空(新的尾节点指向空)
    //4.释放尾节点
    Tlist_c *end = head->prev;
    head->prev = end->prev;
    end->prev->next = NULL;
    free(end);

}

void list_pop_front(Tlist_c *head)
{
    if(head == NULL || head->size == 0) return ;
    head->size -= 1; //计数移除

    //最后一个节点
    if(head->size == 0)
    {
        Tlist_c *begin = head->next;
        head->next = NULL;
        free(begin);
    }
    else
    {
        //1.保留第一个带数据节点
        //2.头节点的后指针,指向第一个带数据的后指针(等于第二个带数据节点)
        //3.第二个带数据的前指针,指向头节点
        //4.释放第一个带数据节点
        Tlist_c *begin = head->next;
        head->next = begin->next;
        begin->next->prev = head;
        free(begin);
    }
}

int list_erase(Tlist_c *head,size_t pos)
{
    if(head == NULL || head->size == 0 || pos >= head->size) return 0;
    head->size -= 1; //计数移除

    //获取指定的当前节点
    size_t count = 0;
    Tlist_c *it = head->next;
    for(;it != NULL;it = it->next)
    {
        if(count == pos) break;
        count++;
    }

    //删除末尾节点和最后一个元素时,需要特殊处理,避免操作空指针
    if(pos == head->size || head->size == 0)
    {
        //1.保留当前节点
        //2.前一个节点的后指针,指向空(新尾节点)
        //3.头节点前指针,指向新尾节点
        //4.释放当前指针
        Tlist_c *now = it;
        it->prev->next = NULL;
        head->prev = it->prev;
        free(now);
        return 1;
    }

    //1.保留当前节点
    //2.前一个节点的后指针,指向后节点
    //3.后节点的前指针,指向前节点
    //4.释放当前指针
    Tlist_c *now = it;
    it->prev->next = it->next;
    it->next->prev = it->prev;
    free(now);
    return 1;
}

void list_clear(Tlist_c *head)
{
    if(head == NULL) return;
    while(1)
    {
        //1.保存第一个节点,获取下一个,然后依次释放
        Tlist_c *begin = head->next;
        if(begin == NULL) break;
        head->next = head->next->next;
        free(begin);
    }
    head->size = 0; //清空计数
}
//===== Tlien_c =====
#endif

