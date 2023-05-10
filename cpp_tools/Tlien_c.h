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

//类型宏
//#define Tvalue char *
#define Tvalue msg


//存放数据的链表
typedef struct Tlist_c Tlist_c;
struct Tlist_c
{
    Tvalue  value;
    size_t  size;
    Tlist_c *prev;
    Tlist_c *next;
};

//操作结构体--集合操作链表回调函数的数据结构
typedef struct Tlist_ac Tlist_ac;
struct Tlist_ac
{
    void    (*alloc)       (Tlist_c *head,Tvalue value);
    void    (*free)        (Tlist_c *head);
    void    (*init)        (Tlist_c *head);
    void    (*push_back)   (Tlist_ac *head,Tvalue value);
    void    (*push_front)  (Tlist_ac *head,Tvalue value);
    int     (*insert)      (Tlist_ac *head,size_t pos,Tvalue value);
    void    (*pop_back)    (Tlist_ac *head);
    void    (*pop_front)   (Tlist_ac *head);
    int     (*erase)       (Tlist_ac *head,size_t pos);
    void    (*clear)       (Tlist_ac *head);
    size_t  (*size)        (Tlist_ac *head);
    Tlist_c *head; //链表
};

//链表的初始化函数，使用链表前必须初始化
void list_init(Tlist_c *head)
{
    if(head == NULL) return;
    head->size = 0;
    head->prev = NULL;
    head->next = NULL;
}

void list_push_back(Tlist_ac *ls,Tvalue value)
{
    //分配空间
    if(ls->head == NULL) return;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //连接到头节点
    if(ls->head->size == 0)
    {
        //1.头节点的前后指针,指到新节点
        //2.新节点前指针,指到头节点
        ls->head->prev = new_value;
        ls->head->next = new_value;
        new_value->prev = ls->head;
    }
    else
    {
        //1.新节点前节点,指向头节点前指针(头节点前指针等于尾节点)
        //2.尾节点的后指针,指到新节点
        //3.头节点前指针,指到新节点(等于将新节点当做尾节点)
        new_value->prev = ls->head->prev;
        ls->head->prev->next = new_value;
        ls->head->prev = new_value;
    }
    ls->head->size += 1;                //计数
    ls->alloc(new_value,value);         //赋值
}

void list_push_front(Tlist_ac *ls,Tvalue value)
{
    //分配空间
    if(ls->head == NULL) return;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //连接到头节点
    if(ls->head->size == 0)
    {
        //参考: list_push_back
        ls->head->prev = new_value;
        ls->head->next = new_value;
        new_value->prev = ls->head;
    }
    else
    {
        //1.第一个带数据节点的前指针,指向新节点
        //2.新节点的后指针,指向第一个带数据节点
        //3.新节点的前指针,指向头节点
        //4.头节点的后指针,指向新节点
        ls->head->next->prev = new_value;
        new_value->next = ls->head->next;
        new_value->prev = ls->head;
        ls->head->next = new_value;
    }
    ls->head->size += 1;                //计数
    ls->alloc(new_value,value);         //赋值
}

int list_insert(Tlist_ac *ls,size_t pos,Tvalue value)
{
    //分配空间
    if(ls->head == NULL || pos >= ls->head->size) return 0;
    Tlist_c *new_value = (Tlist_c *)malloc(sizeof(Tlist_c));
    list_init(new_value);

    //获取指定的当前节点
    size_t count = 0;
    Tlist_c *it = ls->head->next;
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

    ls->head->size += 1;                //计数
    ls->alloc(new_value,value);         //赋值
    return 1;
}

void list_pop_back(Tlist_ac *ls)
{
    if(ls->head == NULL || ls->head->size == 0) return ;
    ls->head->size -= 1; //计数移除

    //1.保留尾节点
    //2.头节点的前指针,指向尾节点的前一个
    //3.尾节点的前一个的后指针,指向空(新的尾节点指向空)
    //4.释放尾节点
    Tlist_c *end = ls->head->prev;
    ls->head->prev = end->prev;
    end->prev->next = NULL;
    ls->free(end);
    free(end);

}

void list_pop_front(Tlist_ac *ls)
{
    if(ls->head == NULL || ls->head->size == 0) return ;
    ls->head->size -= 1; //计数移除

    //最后一个节点
    Tlist_c *begin = NULL;
    if(ls->head->size == 0)
    {
        begin = ls->head->next;
        ls->head->next = NULL;
    }
    else
    {
        //1.保留第一个带数据节点
        //2.头节点的后指针,指向第一个带数据的后指针(等于第二个带数据节点)
        //3.第二个带数据的前指针,指向头节点
        //4.释放第一个带数据节点
        begin = ls->head->next;
        ls->head->next = begin->next;
        begin->next->prev = ls->head;
    }
    ls->free(begin);
    free(begin);
}

int list_erase(Tlist_ac *ls,size_t pos)
{
    if(ls->head == NULL || ls->head->size == 0 || pos >= ls->head->size) return 0;
    ls->head->size -= 1; //计数移除

    //获取指定的当前节点
    size_t count = 0;
    Tlist_c *it = ls->head->next;
    for(;it != NULL;it = it->next)
    {
        if(count == pos) break;
        count++;
    }

    //删除末尾节点和最后一个元素时,需要特殊处理,避免操作空指针
    Tlist_c *now = NULL;
    if(pos == ls->head->size || ls->head->size == 0)
    {
        //1.保留当前节点
        //2.前一个节点的后指针,指向空(新尾节点)
        //3.头节点前指针,指向新尾节点
        //4.释放当前指针
        now = it;
        it->prev->next = NULL;
        ls->head->prev = it->prev;
    }
    else
    {
        //1.保留当前节点
        //2.前一个节点的后指针,指向后节点
        //3.后节点的前指针,指向前节点
        //4.释放当前指针
        now = it;
        it->prev->next = it->next;
        it->next->prev = it->prev;

    }
    ls->free(now);
    free(now);
    return 1;
}

void list_clear(Tlist_ac *ls)
{
    if(ls->head == NULL) return;
    while(1)
    {
        //1.保存第一个节点,获取下一个,然后依次释放
        Tlist_c *begin = ls->head->next;
        if(begin == NULL) break;
        ls->head->next = ls->head->next->next;
        ls->free(begin);
        free(begin);
    }
    ls->head->size = 0; //清空计数
}

size_t list_size(Tlist_ac *ls)
{
    if(ls->head == NULL) return 0;
    return ls->head->size;
}

//启动链表
Tlist_ac* Tlist_open(void (*alloc)(Tlist_c *head,Tvalue value),void (*free)(Tlist_c *head))
{
    //分配自身内存和链表的头节点链表
    Tlist_ac *ls = (Tlist_ac *)malloc(sizeof(Tlist_ac));
    ls->head = (Tlist_c*)malloc(sizeof(Tlist_c));

    //注册操作链表的回调函数
    //  alloc和free回调需要用使用者自定义
    //  自定义复杂结构内存的分配和释放
    ls->alloc       = alloc;
    ls->free        = free;
    ls->init        = list_init;
    ls->push_back   = list_push_back;
    ls->push_front  = list_push_front;
    ls->insert      = list_insert;
    ls->pop_back    = list_pop_back;
    ls->pop_front   = list_pop_front;
    ls->erase       = list_erase;
    ls->clear       = list_clear;
    ls->size        = list_size;

    ls->init(ls->head);//初始化链表
    return ls;
}

//关闭链表
void Tlist_close(Tlist_ac *ls)
{
    ls->clear(ls);  //清空带数据节点
    free(ls->head); //清空头节点
    free(ls);       //清空操作结构体
}
//===== Tlien_c =====
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

