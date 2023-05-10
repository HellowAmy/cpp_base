#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tlien_c.h"



//typedef struct ddo ddo;
//struct ddo
//{
//    int a;
//    int b;
//    char *c;
//    char d[100];
//};


//void list_malloc(Tlist_c *head,Tvalue value)
//{
//    head->value = (Tvalue)malloc(strlen(value)+1);
//    strncpy(head->value,value,strlen(value)+1);
//}

//void list_free(Tlist_c *head)
//{
//    free(head->value);
//}


void list_malloc(Tlist_c *head,Tvalue value)
{
    head->value.len = value.len;
    head->value.type = (char*)malloc(strlen(value.type));
    strcpy(head->value.type,value.type);
    strcpy(head->value.data,value.data);

//    head->value = (Tvalue)malloc(strlen(value)+1);
//    strncpy(head->value,value,strlen(value)+1);
}

void list_free(Tlist_c *head)
{
    free(head->value.type);
}

int main()
{


    printf("== begin ==\n");

//    Tlist_c head;
//    list_init(&head);
//    list_push_back(&head,"123");
//    list_push_back(&head,"qwe");
//    list_push_back(&head,"asd");

//    char bu[100] = "111";
//    list_push_back(&head,bu);

//    strcpy(bu,"222");
//    list_push_back(&head,bu);

//    strcpy(bu,"333");
//    list_push_back(&head,bu);

//    Tlist_for(it,head)
//    {
//        printf("it: %s\n",it->value);
//    }

    Tlist_ac *ls = Tlist_open(list_malloc,list_free);


    msg p = {123,"123a","123b"};
//    printf("%d %s %s\n",p.len,p.type,p.data);
    ls->push_back(ls,p);

    msg p1 = {456,"456a","456b"};
    ls->push_back(ls,p1);

    msg p2 = {789,"789a","789b"};
    ls->push_back(ls,p2);

//    strcpy(buf,"qqqq");
//    ls->push_back(ls,buf);

//    strcpy(buf,"wwww");
//    ls->push_back(ls,buf);

    Tlist_for_p(it,ls->head)
    {
        printf("%d %s %s\n",it->value.len,it->value.type,it->value.data);
    }


//    ls->clear(ls);

//    Tlist_for_p(it,ls->head)
//    {
//        printf("%s\n",it->value);
//    }

//    ls->push_back(ls,"123");
//    ls->push_back(ls,"456");
//    ls->push_back(ls,"789");

//    Tlist_for_p(it,ls->head)
//    {
//        printf("%s\n",it->value);
//    }
//    ls->push_back(ls,"123");
//    ls->push_back(ls,"456");
//    ls->push_back(ls,"789");
//    Tlist_for_p(it,ls->head)
//    {
//        printf("%s\n",it->value);
//    }

    Tlist_close(ls);

//    printf("=======\n");
//    Tlist_for_p(it,ls->head)
//    {
//        printf("%s\n",it->value);
//    }

//    dire d1;
//    d1.a = 1;
//    d1.b = 2;
//    d1.c = 3;
////    d1.cd = "qwe";
//    Tlist_c head;
//    list_init(&head);
//    list_push_back(&head,&d1);

//    d1.a = 11;
//    d1.b = 21;
//    d1.c = 31;
//    char *po = (char*)malloc(123);

//    strcpy(po,"187t23qjwdjkqwd");
////    d1.cd = po;
//    list_push_back(&head,&d1);

//    d1.a = 13;
//    d1.b = 23;
//    d1.c = 33;
////    strcpy(d1.cd,"zx");
//    list_push_back(&head,&d1);
//    d1.a = 0;
//    d1.b = 0;
//    d1.c = 0;
////    strcpy(d1.cd,"0");
//    list_push_back(&head,&d1);

//    Tlist_for(it,head)
//    {
//        printf("it: %d %d %d %s\n",it->value->a,it->value->b,it->value->c,it->value->cd);
//    }
    printf("== end ==\n");

    return 0;


//    ddo d1;
//    d1.u = 10;
//    ddo *pd1 = te1(d1);
//    printf("te1: %d\n",pd1->u);

//    int *pd2 = te2(3);
//    printf("te2: %d\n",*pd2);

//    char **pd3 = te3("12345");
//    printf("te3: %s %ld\n",*pd3,strlen(*pd3));



//    ddo d1;

//    ddo *d = (ddo*)malloc(sizeof (ddo));
//    d = &d1;


//    char *po = (char *)malloc(100);
//    strcat(po,"123qweasd");
//    char * *pp1;
//    (pp1) = &po;


//    printf("%s %s \n",po,*pp1),
////    test_1();
////    test_2();
////    test_3();
    printf("== end ==\n");
    return 0;
}
