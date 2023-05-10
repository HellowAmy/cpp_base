//!
//! 随笔:C语言字符串分割函数
//!
//! == 简介 ==
//! C语言没有提供随手的字符串分割函数，本次提供C语言的字符串分割函数，
//!     切割的字符从分割符左侧开始，第一个下标为0,
//!     该函数会分配内存，需要外部释放
//!
//!     例子：("123##456",[0:123]##[1:456]) (##qwe##asd##,[0:][1:qwe][2:asd][3:])
//!         char *str = split_ac("123##456","##",0,0);
//!         free(str);
//! == 简介 ==
//!
//! 结束语：
//!     文章后面附带str_split.h文件
//!
#include <stdio.h>

#include "str_split.h"

void test_1()
{
    //无分割符的边界测试
    {
        char *str = split_ac("123##456","##",0,0);
        printf("[123##456] 1:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("123##456","##",1,1);
        printf("[123##456] 2:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("123##456","##",2,2);
        printf("[123##456] 3:std: %s\n",str);
        free(str);
    }

    //双头分割符测试
    {
        char *str = split_ac("##123##456##","##",0,0);
        printf("[##123##456##] 1:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("##123##456##","##",1,1);
        printf("[##123##456##] 2:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("##123##456##","##",2,2);
        printf("[##123##456##] 3:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("##123##456##","##",3,3);
        printf("[##123##456##] 4:std: %s\n",str);
        free(str);
    }

    //连续分割测试
    {
        char *str = split_ac("a:b:c:d:e:f",":",0,5);
        printf("[a:b:c:d:e:f] 1:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("a:b:c:d:e:f",":",0,2);
        printf("[a:b:c:d:e:f] 2:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("a:b:c:d:e:f",":",2,5);
        printf("[a:b:c:d:e:f] 3:std: %s\n",str);
        free(str);
    }
    {
        char *str = split_ac("a:b:c:d:e:f",":",0,6);
        printf("[a:b:c:d:e:f] 4:std: %s\n",str);
        free(str);
    }
}

int main()
{
    printf("== begin ==\n");
    test_1();
    printf("== end ==\n");
    return 0;
}

/*
 * 测试结果
 *
== begin ==
[123##456] 1:std: 123
[123##456] 2:std: 456
[123##456] 3:std: (null)
[##123##456##] 1:std: (null)
[##123##456##] 2:std: 123
[##123##456##] 3:std: 456
[##123##456##] 4:std: (null)
[a:b:c:d:e:f] 1:std: a:b:c:d:e:f
[a:b:c:d:e:f] 2:std: a:b:c
[a:b:c:d:e:f] 3:std: c:d:e:f
[a:b:c:d:e:f] 4:std: (null)
== end ==
*/
