#include <stdio.h>
#include <stdlib.h>

#include "Tline_ac.h"

int main()
{
    printf("== begin ==\n");

    Tlist_ac *ls2 = NULL;
    Tlist_ac *ls1 = Tlist_open(list_malloc_Tcharp,list_free_Tcharp);
    ls1->push_back(ls1,"s[1]");
    ls1->push_back(ls1,"s[2]");
    ls1->push_front(ls1,"s[-1]");
    ls1->push_front(ls1,"s[-2]");
    ls1->insert(ls1,2,"s[0.1]");
    ls1->insert(ls1,3,"s[0.2]");

    Tlist_for_p(it,ls1->head)
    {
        printf("%s\n",it->value);
    }
    printf("链表个数: %ld\n",ls1->size);

    {
        ls2 = Tlist_clone(ls1);
        if(ls2 == NULL) printf("克隆链表\n");
//        printf("克隆链表 : %d %ld\n",ok,ls2->size);

    }



    printf("\n==  移除元素 ==\n");
    ls1->pop_back(ls1);
    ls1->pop_front(ls1);
    ls1->erase(ls1,1);

    Tlist_for_p(it,ls1->head)
    {
        printf("%s\n",it->value);
    }

    printf("链表个数: %ld\n",ls1->size);

    ls1->clear(ls1); //清空链表
    printf("清空链表:链表个数: %ld\n",ls1->size);

    Tlist_close(ls1);



    Tlist_for_p(it,ls2->head)
    {
        printf("ls2: %s\n",it->value);
    }

    Tlist_close(ls2);
//    test_1();

    printf("== end ==\n");
    return 0;
}
