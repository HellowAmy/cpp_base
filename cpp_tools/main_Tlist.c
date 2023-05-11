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



int main()
{
    printf("== begin ==\n");

    Tlist_ac *ls = Tlist_open(list_malloc_Tcharp,list_free_Tcharp);
//    Tlist_ac *ls = Tlist_open(list_malloc_Tbase,list_free_Tbase);

    ls->push_back(ls,"123");
    ls->push_back(ls,"456");
    ls->push_back(ls,"789");
    Tlist_for_p(it,ls->head)
    {
        printf("%s\n",it->value);
    }





//    ls->push_back(ls,123);
//    char bu[100] = "gggg";
//    int hu = 444;
//    ls->push_back(ls,hu);
//    strcpy(bu,"pppp");
//    hu = 999;
//    ls->push_back(ls,hu);
//    Tlist_for_p(it,ls->head)
//    {
//        printf("%d\n",it->value);
//    }

    Tlist_close(ls);

    printf("== end ==\n");
    return 0;
}
