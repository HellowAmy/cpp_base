//!
//! C++工具：C语言链表实现C++STL链表接口--支持面向对象与泛型数据结构
//!
//! == 工具简介 ==
//! 本文是上篇文章的加强版
//!     上篇文章: C++工具：仿C++STL接口的C风格链表--支持函数泛型
//!
//! 上篇文章实现了C语言链表的泛型与C++STL接口，但使用的是函数风格，
//!     不同类型需要用不同函数操作，略显麻烦,且不支持深拷贝数据结构，
//!     指针类型需要手动分配和循环链表释放内存
//!
//! 本章带来的加强版链表采用面向对象的思想，使用与接口与C++STL类似，
//! 	提供自定义内存分配器，支持自定义类型的深拷贝操作，
//! 	默认自动生成八大基本类型和字符串类型链表，
//! 	提供默认浅拷贝分配器和字符串分配器
//!
//! 加强版链表利用结构体封装回调指针实现接口的统一，
//!     利用宏泛型生成对应数据结构类型
//! == 工具简介 ==
//!
//!
//! == 使用说明 ==
//! 以字符串类型作为例子：
//!
//!     == 代码段 ==
//!     // Tlist_make宏的等价声明: Tlist_Tcharp *ls = Topen_Tcharp(Tmalloc_Tcharp,Tfree_Tcharp);
//!     Tlist_Tcharp *ls = Tlist_make(Tcharp);
//!     Tfor_each(ls,it,Tcharp)
//!     {
//!         printf("%s\n",it->value);
//!     }
//!     Tclose_Tcharp(ls);
//!     == 代码段 ==
//!
//! 1.Tlist_Tcharp为链表类型，类似list<char*>
//! 2.Topen_Tcharp函数类似构造函数，链表需要构造函数初始化才能使用
//! 3.Tmalloc_Tcharp是内存分配函数,Tfree_Tcharp是内存释放函数
//! 4.内存分配器可以自定义分配结构体的内存分配问题，和深拷贝问题
//! 5.Tfor_each是遍历宏,参数（1：链表对象，2：返回的迭代器，3：数据接口类型）
//! 6.Tclose_Tcharp函数类似析构函数，释放链表的元素内存与链表本身的内存
//! 7.自定义分配器时注意内存泄露问题，内存分配与释放函数需完全对应
//! 8.Tlist_make宏是快速声明宏，方便使用，如果是自定义分配器，必须符合函数签名格式
//! == 使用说明 ==
//!
//!
//! == 展示顺序 ==
//! 1.main测试代码
//! 2.Tlist_Tapi.h文件
//! 3.测试参考头文件
//! == 展示顺序 ==
//!
//!
//! 结束语：
//!     该链表的使用符合C++程序员在C语言的使用习惯，
//!         也解决了C语言下C++程序员无容器可用的烦恼,
//!         与C语言内核链表相比泛型容器可以降低学习成本
//!     本文提供的容器可能还有很多不足的地方，如果有发现问题请在评论区反馈,
//!         如果本文给你提供了C语言下其他C++泛型容器的设计思想，你已经实现并希望分享给大家，
//!         请在评论区留言并将代码提交到GIthub上
//!     GIthub网址：https://github.com/HellowAmy/cpp_base.git
//!
//!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tlist_Tapi.h"

//== 自定义类型:自定义内存分配:注意生成顺序 (类型定义 -> 宏代码生成 -> 自定义分配器) ==
typedef struct meg meg;
struct meg
{
    int len;
    char *type;
    char data[100];
};

//注意:
//  当链表调用push_back等需要分配内存的函数元素的函数时，调用Tmalloc
//  调用pop_back等移除元素时需要释放内存的函数时，调用Tfree
//对象
Tlist_Tbuild(Tmeg,meg)
static void Tmalloc_Tmeg(Tls_Tmeg *head,meg value)
{
    head->value.len = value.len;
    head->value.type = (char*)malloc(strlen(value.type) +1);
    strcpy(head->value.type,value.type);
    strcpy(head->value.data,value.data);
}
static void Tfree_Tmeg(Tls_Tmeg *head)
{
    free(head->value.type);
}

//指针
Tlist_Tbuild(Tmegp,meg *)
static void Tmalloc_Tmegp(Tls_Tmegp *head,const meg *value)
{
    //给链表存储的指针分配内存
    head->value = (meg *)malloc(sizeof(meg));

    //给结构体内的成员分配内存
    head->value->type = (char*)malloc(strlen(value->type) +1);
    head->value->type[strlen(value->type)] = '\0';

    head->value->len = value->len;
    strcpy(head->value->type,value->type);
    strcpy(head->value->data,value->data);
}
static void Tfree_Tmegp(Tls_Tmegp *head)
{
    //Tmalloc函数分配了多少Tfee就释放多少
    free(head->value->type);
    free(head->value);
}
//== 自定义类型:自定义内存分配:注意生成顺序 (类型定义 -> 宏代码生成 -> 自定义分配器) ==


//== 自定义类型:(使用默认分配器需要外部malloc分配内存，否则会浅拷贝问题) ==
typedef struct dire dire;
struct dire
{
    int x;
    int y;
    int z;
    char *data;
};
Tls_Tbase(Tdirep,dire *)
//== 自定义类型:(使用默认分配器需要外部malloc分配内存，否则会浅拷贝问题) ==


//各类型加入测试
void test_1()
{
    //== 基础类型测试 ==
    printf("== 基础类型测试 ==\n");
    {
        Tlist_Tchar *ls =  Tlist_make(Tchar); //等价下方声明
//        Tlist_Tchar *ls = Topen_Tchar(Tmalloc_Tchar,Tfree_Tchar);
        const char t1 = 'a';
        char t2 = 'b';
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,'c');

        Tfor_each(ls,it,Tchar)
        {
            printf("%c\n",it->value);
        }
        Tclose_Tchar(ls);
    }
    {
        Tlist_Tint *ls = Topen_Tint(Tmalloc_Tint,Tfree_Tint);
        const int t1 = 100;
        int t2 = 200;
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,300);

        Tfor_each(ls,it,Tint)
        {
            printf("%d\n",it->value);
        }
        Tclose_Tint(ls);
    }
    //== 基础类型测试 ==


    //== 字符串测试 ==
    printf("== 字符串测试 ==\n");
    {
        Tlist_Tcharp *ls = Topen_Tcharp(Tmalloc_Tcharp,Tfree_Tcharp);
        const char* t1 = "a1";
        char* t2 = "a2";
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,"a3");

        Tfor_each(ls,it,Tcharp)
        {
            printf("%s\n",it->value);
        }
        Tclose_Tcharp(ls);
    }
    {
        Tlist_Tcharp *ls = Tlist_make(Tcharp);
        const char* t1 = "a1";
        char* t2 = "a2";
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,"a3");

        Tfor_each(ls,it,Tcharp)
        {
            printf("%s\n",it->value);
        }
        Tclose_Tcharp(ls);
    }
    //== 字符串测试 ==


    //== 自定义测试 ==
    printf("== 自定义测试 ==\n");
    {
        Tlist_Tmeg *ls = Topen_Tmeg(Tmalloc_Tmeg,Tfree_Tmeg);
        {
            const meg t1 = {0,"00","000"};

            meg t2;
            t2.len = 1;
            t2.type = (char *)malloc(100);
            strcpy(t2.type,"11");
            strcpy(t2.data,"111");
            ls->push_back(ls,t1);
            ls->push_back(ls,t2);
        }
        Tfor_each(ls,it,Tmeg)
        {
            printf("p1:[%d : %s : %s]\n",it->value.len,it->value.data,it->value.type);
        }
        Tclose_Tmeg(ls);
    }
    {
        Tlist_Tmegp *ls = Topen_Tmegp(Tmalloc_Tmegp,Tfree_Tmegp);
        {
            meg* t2 = (meg*)malloc(sizeof(meg));
            t2->len = 1;
            t2->type = (char *)malloc(100);
            strcpy(t2->type,"11");
            strcpy(t2->data,"111");
            ls->push_back(ls,t2);

            //使用完了需要释放
            free(t2->type);
        }
        Tfor_each(ls,it,Tmegp)
        {
            printf("p2:[%d : %s : %s]\n",it->value->len,it->value->data,it->value->type);
        }
        Tclose_Tmegp(ls);
    }
    {
        Tlist_Tdirep *ls = Tlist_make(Tdirep);//等价下方声明
//        Tlist_Tdirep *ls = Topen_Tdirep(Tmalloc_Tdirep,Tfree_Tdirep);
        {
            //使用默认分配器时，刚分配加入链表的变量不要释放
            dire *t1 = (dire *)malloc(sizeof(dire));
            t1->x = 1;
            t1->y = 2;
            t1->z = 3;
            t1->data = (char *)malloc(100);
            strcpy(t1->data,"4");
            ls->push_back(ls,t1);

            dire *t2 = (dire *)malloc(sizeof(dire));
            t2->x = 11;
            t2->y = 22;
            t2->z = 33;
            t2->data = (char *)malloc(100);
            strcpy(t2->data,"44");
            ls->push_back(ls,t2);
        }
        //使用默认分配器时，关闭链表时需要手动释放元素的内存
        //for
        Tfor_each(ls,it,Tdirep)
        {
            printf("p3:[%d : %d : %d : %s]\n",it->value->x,it->value->y,it->value->z,it->value->data);
        }
        Tclose_Tdirep(ls);
    }
    //== 自定义测试 ==

    printf("== 测试结束 ==\n");
}

//== 接口测试 ==
void test_2()
{
    printf("\n== 接口测试 ==\n");
    printf("== 操作ls1 ==\n");

    //使用链表时，需要开启和关闭链表
    Tlist_Tcharp *ls1 = Topen_Tcharp(Tmalloc_Tcharp,Tfree_Tcharp);
    ls1->push_back(ls1,"s[1]");
    ls1->push_back(ls1,"s[2]");
    ls1->push_front(ls1,"s[-1]");
    ls1->push_front(ls1,"s[-2]");
    ls1->insert(ls1,2,"s[0.1]");
    ls1->insert(ls1,3,"s[0.2]");

    Tfor_each(ls1,it,Tcharp)
    {
        printf("%s\n",it->value);
    }
    printf("链表个数: %ld\n",ls1->size);

    Tlist_Tcharp *ls2 = Tclone_Tcharp(ls1);
    printf("克隆链表ls2: 链表个数: %ld\n",ls2->size);

    printf("\n==  移除元素 ==\n");
    ls1->pop_back(ls1);
    ls1->pop_front(ls1);
    ls1->erase(ls1,1);

    Tfor_each(ls1,it,Tcharp)
    {
        printf("%s\n",it->value);
    }

    printf("链表个数: %ld\n",ls1->size);

    ls1->clear(ls1); //清空链表
    printf("清空链表:链表个数: %ld\n",ls1->size);

    Tclose_Tcharp(ls1); //关闭链表


    //操作ls2
    printf("== 操作ls2 ==\n");
    Tfor_each(ls2,it,Tcharp)
    {
        printf("%s\n",it->value);
    }
    Tclose_Tcharp(ls2); //关闭链表

    printf("== 测试结束 ==\n");
}

int main()
{

    printf("== begin ==\n");

    test_1();
    test_2();

    printf("== end ==\n");
    return 0;
}

/*
 * 测试结果
 *
== begin ==
== 基础类型测试 ==
a
b
c
100
200
300
== 字符串测试 ==
a1
a2
a3
== 自定义测试 ==
p1:[0 : 000 : 00]
p1:[1 : 111 : 11]
p2:[1 : 111 : 11]
p3:[1 : 2 : 3 : 4]
p3:[11 : 22 : 33 : 44]
== 测试结束 ==

== 接口测试 ==
== 操作ls1 ==
s[-2]
s[-1]
s[0.1]
s[0.2]
s[1]
s[2]
链表个数: 6
克隆链表ls2: 链表个数: 6

==  移除元素 ==
s[-1]
s[0.2]
s[1]
链表个数: 3
清空链表:链表个数: 0
== 操作ls2 ==
s[-2]
s[-1]
s[0.1]
s[0.2]
s[1]
s[2]
== 测试结束 ==
== end ==

*/
