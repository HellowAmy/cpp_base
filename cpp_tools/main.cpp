#include "ctimel.h"
#include "../include/stm.h"

#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <map>
#include <atomic>
using namespace std;



int main()
{
    cout<<"===== begin ====="<<endl;
    test_1();
//    test_2();

    cout<<"===== end ====="<<endl;
    return 0;
}


/*
 * 多线程容器接口测试
 *

===== begin =====

===== queue_th =====
=====
11
12
13
14
15
=====
s11
s22
s33
s44
s55
s66
s77
=====
600
=====
600
600
600
swap front: 0
swap after: 600

===== vector_th =====
5
5
5
=====
v1
v2
v3
v4
v5
=====
v1
v2
v3
v4
v5
=====
v3
v4
=====
capacity: 8
size: 6
=====
v0
v1
v2
v3
v4
v5
=====
30000

===== map_th =====
1|10
2|20
3|30
4|40
5|50
6|60
7|70
7
7
7
0
4
904
2700

== 迭代器易位测试 ==
10|3
单线程操作迭代器
对比点: word_1
对比点: word_2
===== show =====
word_1
add word
word_2
word_3

多线程操作迭代器
对比点: word_1
对比点: add word
===== show =====
word_1
add word
word_2
word_3
10|4
===== end =====

*/


