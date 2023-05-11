#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "Tlien_c.h"
#include "Tline_ac.h"

//==
//void list_malloc_Tcharp(Tlist_c *head,Tvalue value)
//{
//    head->value = (char*)malloc(strlen(value) +1);
//    strncpy(head->value,value,strlen(value));
//    head->value[strlen(value)] = '\0';
//}

//void list_free_Tcharp(Tlist_c *head)
//{
//    free(head->value);
//}


//void list_malloc_Tbase(Tlist_c *head,Tvalue value)
//{
//    head->value = value;
//}

//void list_free_Tbase(Tlist_c *head)
//{

//}
//==






//static void Tmalloc_Tbase(Tls_Tint *head,int value)
//{ head->value = value; }
//static void Tfree_Tbase(Tls_Tint *head)
//{ }
//static void Tmalloc_Tcharp(Tls_Tcharp *head,const char* value)
//{
//    head->value = (char*)malloc(strlen(value) +1);
//    strncpy(head->value,value,strlen(value));
//    head->value[strlen(value)] = '\0';
//}
//static void Tfree_Tcharp(Tls_Tcharp *head)
//{ free(head->value); }




#define Tlist_for(it,head) for(Tlist_base_Tint *it = head.next;it != NULL;it = it->next)
#define Tlist_for_p(it,head) for(Tlist_base_Tint *it = head->next;it != NULL;it = it->next)


//#define rt


//int p = 0;

//#ifdef rt


//#define qwe 9
//#endif

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


void test_1()
{
    //== 基础类型测试 ==
    {
        Tlist_Tchar *ls = Topen_Tchar(Tmalloc_Tchar,Tfree_Tchar);
        const char t1 = 'a';
        char t2 = 'b';
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,'c');
        Tclose_Tchar(ls);
    }
    {
        Tlist_Tshort *ls = Topen_Tshort(Tmalloc_Tshort,Tfree_Tshort);
        const short t1 = 10;
        int t2 = 20;
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,30);
        Tclose_Tshort(ls);
    }
    {
        Tlist_Tint *ls = Topen_Tint(Tmalloc_Tint,Tfree_Tint);
        const int t1 = 100;
        int t2 = 200;
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,300);
        Tclose_Tint(ls);
    }
    {
        Tlist_Tlonglong *ls = Topen_Tlonglong(Tmalloc_Tlonglong,Tfree_Tlonglong);
        const int t1 = 100000;
        int t2 = 200000;
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,300000);
        Tclose_Tlonglong(ls);
    }
    //== 基础类型测试 ==


    //== 字符串测试 ==
    {
        Tlist_Tcharp *ls = Topen_Tcharp(Tmalloc_Tcharp,Tfree_Tcharp);
        const char* t1 = "a1";
        char* t2 = "a2";
        ls->push_back(ls,t1);
        ls->push_back(ls,t2);
        ls->push_back(ls,"a3");
        Tclose_Tcharp(ls);
    }
    //== 字符串测试 ==


    //== 自定义测试 ==
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
        Tclose_Tmeg(ls);
    }
    {
        Tlist_Tmegp *ls = Topen_Tmegp(Tmalloc_Tmegp,Tfree_Tmegp);
        {
//            const meg* t1 = {0,"00","000"};
            meg* t2 = (meg*)malloc(sizeof(meg));
            t2->len = 1;
            t2->type = (char *)malloc(100);
            strcpy(t2->type,"11");
            strcpy(t2->data,"111");
//            ls->push_back(ls,t1);
            ls->push_back(ls,t2);

            //使用完了需要释放
            free(t2->type);
        }
        Tclose_Tmegp(ls);
    }
//    {
//        Tlist_Tdirep *ls = Topen_Tdirep(Tmalloc_Tdirep,Tfree_Tdirep);
//        {
//            //使用默认分配器时，刚分配加入链表的变量不要释放
//            dire *t1 = (dire *)malloc(sizeof(dire));
//            t1->x = 1;
//            t1->y = 2;
//            t1->z = 3;
//            t1->data = (char *)malloc(100);
//            strcpy(t1->data,"4");
//            ls->push_back(ls,t1);

//            dire *t2 = (dire *)malloc(sizeof(dire));
//            t2->x = 11;
//            t2->y = 22;
//            t2->z = 33;
//            t2->data = (char *)malloc(100);
//            strcpy(t1->data,"44");
//            ls->push_back(ls,t2);
//        }
//        //使用默认分配器时，关闭链表时需要手动释放元素的内存
//        //for
//        Tclose_Tdirep(ls);
//    }
    //== 自定义测试 ==
}


int main()
{

    test_1();

    Tlist_Tint *ls = Topen_Tint(Tmalloc_Tint,Tfree_Tint);

    const int hu = 100;
    int hu2 = 200;
    ls->push_back(ls,hu);
    ls->push_back(ls,hu2);

    ls->push_back(ls,555);
    hu2 = 300;
    ls->push_back(ls,hu);

    for(Tls_Tint *it = ls->head->next;it != NULL;it = it->next)
    {
        printf("%d\n",it->value);
    }


    printf("== begin ==\n");

    Tclose_Tint(ls);




    {
        Tlist_Tcharp *ls = Topen_Tcharp(Tmalloc_Tcharp,Tfree_Tcharp);
        ls->push_back(ls,"124");
        ls->push_back(ls,"124111");
        ls->push_back(ls,"124www");

        for(Tls_Tcharp *it = ls->head->next;it != NULL;it = it->next)
        {
            printf("%s\n",it->value);
        }

        Tclose_Tcharp(ls);
    }

    printf("== end ==\n");
    return 0;
}
