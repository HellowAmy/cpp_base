//!
//! C++工具：仿C++STL接口的C风格链表--支持函数泛型
//!
//! == 工具简介 ==
//! 提供纯C风格的带头结点链表链表，操作接口类似C++STL容器，提供泛型宏接口，统一各类型操作
//! 	模板 Tlist_create(Tapi,Tvalue) Tapi是接口定义，Tvalue是具体类型
//! 		模板生成例子：
//! 			Tlist_create(Tint,int)  	->  Tlist_Tint head;
//! 			Tlist_create(Tcharp,char *)  	->  Tlist_Tcharp head;
//! 			Tlist_Tint head;
//! == 工具简介 ==
//!
//!
//! == 展示顺序 ==
//! 1.基本类型与接口测试 main测试
//! 2.字符串测试
//! 3.自定义类型测试
//! 4.Tlien_c.h 头文件
//! 5.参考头文件（用于新功能添加与测试）
//! == 展示顺序 ==
//!
#include "Tlien_c.h"

typedef struct dire dire;
struct dire
{
    int x;
    int y;
    int z;
};

//== 模板生成 ==
//基本类型
Tlist_create(Tchar,char)
Tlist_create(Tshort,short)
Tlist_create(Tint,int)
Tlist_create(Tlong,long)
Tlist_create(Tlonglong,long long)
Tlist_create(Tfloat,float)
Tlist_create(Tdouble,double)
Tlist_create(Tcharp,char *) //字符串
Tlist_create(Tdire,dire) //自定义类型
//== 模板生成 ==

void list_show_Tint(Tlist_Tint *head)
{
    printf("== size: %ld ==\n",head->size);
    Tlist_for_p(head,it,Tint)
    {
        printf("%d\n",it->value);
    }
    printf("== end ==\n");
}

//基础类型
void test_1()
{
    printf("== test_1 ==\n");

    //提供类似C++STL容器接口的C风格链表，支持泛型
    Tlist_Tint head;
    list_init_Tint(&head); //需要初始化才能使用

    list_push_back_Tint(&head,1);
    list_push_back_Tint(&head,12);
    list_push_back_Tint(&head,123);
    printf("\n== list_push_back ==\n");
    list_show_Tint(&head);

    list_push_front_Tint(&head,100);
    list_push_front_Tint(&head,200);
    list_push_front_Tint(&head,300);
    printf("\n== list_push_front ==\n");
    list_show_Tint(&head);

    list_pop_front_Tint(&head);
    printf("\n== list_pop_front ==\n");
    list_show_Tint(&head);

    list_pop_back_Tint(&head);
    printf("\n== list_pop_back ==\n");
    list_show_Tint(&head);

    list_erase_Tint(&head,2);
    printf("\n== list_pop_back ==\n");
    list_show_Tint(&head);

    list_insert_Tint(&head,2,400);
    printf("\n== list_insert ==\n");
    list_show_Tint(&head);

    list_clear_Tint(&head);
    printf("\n== list_clear ==\n");
    list_show_Tint(&head);
}

void list_show_Tcharp(Tlist_Tcharp *head)
{
    printf("== size: %ld ==\n",head->size);
    Tlist_for_p(head,it,Tcharp)
    {
        printf("%s\n",it->value);
    }
    printf("== end ==\n");
}

//字符串
void test_2()
{
    printf("\n== test_2 ==\n");

    Tlist_Tcharp head;
    list_init_Tcharp(&head); //需要初始化才能使用

    list_push_back_Tcharp(&head,"100");
    list_push_back_Tcharp(&head,"101");
    list_push_back_Tcharp(&head,"111");
    list_push_front_Tcharp(&head,"999");
    list_push_front_Tcharp(&head,"888");

    list_show_Tcharp(&head);
}

dire build_dire(int x,int y,int z)
{
    dire tm;
    tm.x = x;
    tm.y = y;
    tm .z = z;
    return tm;
}

void list_show_Tdire(Tlist_Tdire *head)
{
    printf("== size: %ld ==\n",head->size);
    Tlist_for_p(head,it,Tdire)
    {
        printf("[%d:%d:%d]\n",it->value.x,it->value.y,it->value.z);
    }
    printf("== end ==\n");
}

//自定义类型
void test_3()
{
    printf("\n== test_3 ==\n");

    Tlist_Tdire head;
    list_init_Tdire(&head); //需要初始化才能使用

    list_push_back_Tdire(&head,build_dire(0,1,2));
    list_push_back_Tdire(&head,build_dire(1,2,3));
    list_push_back_Tdire(&head,build_dire(3,3,3));
    list_push_front_Tdire(&head,build_dire(9,0,0));
    list_push_front_Tdire(&head,build_dire(123,456,789));

    list_show_Tdire(&head);
}

int main()
{
    printf("== begin ==\n");
    test_1();
    test_2();
    test_3();
    printf("== end ==\n");
    return 0;
}
