//!
//! 随笔:Linux下获取文件与目录
//!
//! == 随笔介绍 ==
//! 提供一个用于在Linux下获取文件名和目录的类，有两种方式，
//!     一种是将文件与目录分别归类到容器中,另一种是将文件与目录放入同一个容器，
//!     但带有描述结构的信息
//! == 随笔介绍 ==
//!
//!
//! == 内容展示 ==
//! 1.main()中的使用测试
//! 2.scan_files.h头文件代码
//! 3.测试结果显示
//! == 内容展示 ==
//!
//!
#include "scan_files.h"
#include <iostream>
using namespace std;

void test_1()
{
    cout<<endl<<"== test_1 =="<<endl;

    string path = "/home/red/open/save_test/test_dir/";
    vector<string> vec_dir;
    vector<string> vec_files;
    bool ok = scan_files::get_files(path,vec_files,vec_dir);

    if(ok)
    {
        cout<<"== 所有目录 =="<<endl;
        for(auto a:vec_dir)
        {
            cout<<a<<endl;
        }
        cout<<"== 所有文件 =="<<endl;
        for(auto a:vec_files)
        {
            cout<<a<<endl;
        }
        cout<<"目录数: "<<vec_dir.size()<<" | 文件数: "<<vec_files.size()<<endl;
    }
}

void test_2()
{
    cout<<endl<<"== test_2 =="<<endl;

    string path = "/home/red/open/save_test/test_dir";
    vector<scan_files::tree> vec_tree;
    bool ok = scan_files::get_tree_dir(path,vec_tree);
    if(ok)
    {
        for(auto a:vec_tree)
        {
            cout<<a.deep<<"|"<<a.type<<"|"<<a.name<<endl;
            cout<<a.path_abs<<endl;
        }
        cout<<"目录与文件数(加根节点): "<<vec_tree.size()<<endl;
    }
}

int main()
{
    cout<<"===== begin ====="<<endl;

    test_1();
    test_2();

    cout<<"===== end ====="<<endl;
    return 0;
}

/*
 * 测试结果展示如下：
 * 1.ree命令的数结构
 * 2.test_1的输出
 * 2.test_2的输出
*/

/*
 * tree 的树结构
 *
.
├── t1
│   ├── word1.txt
│   ├── word2.txt
│   └── word3.txt
├── t2
│   ├── class1.mp4
│   ├── class2.mp4
│   ├── class3.mp4
│   ├── pic
│   │   ├── newpic
│   │   │   ├── jk.jpeg
│   │   │   ├── mv.jpeg
│   │   │   └── open.jpeg
│   │   ├── p1.jpg
│   │   ├── p2.jpg
│   │   └── p3.jpg
│   ├── pic1.png
│   ├── pic2.png
│   └── word
└── t3
    ├── d1
    │   ├── h1.txt
    │   └── h2.txt
    ├── d2
    │   ├── r1.txt
    │   └── r2.txt
    └── d3
        ├── p1.txt
        └── p2.txt

9 directories, 20 files

*/

/*
 * test_1
 *
== test_1 ==
== 所有目录 ==
/home/red/open/save_test/test_dir/t1
/home/red/open/save_test/test_dir/t2
/home/red/open/save_test/test_dir/t3
/home/red/open/save_test/test_dir/t2/word
/home/red/open/save_test/test_dir/t2/pic
/home/red/open/save_test/test_dir/t2/pic/newpic
/home/red/open/save_test/test_dir/t3/d3
/home/red/open/save_test/test_dir/t3/d1
/home/red/open/save_test/test_dir/t3/d2
== 所有文件 ==
/home/red/open/save_test/test_dir/t1/word3.txt
/home/red/open/save_test/test_dir/t1/word1.txt
/home/red/open/save_test/test_dir/t1/word2.txt
/home/red/open/save_test/test_dir/t2/pic1.png
/home/red/open/save_test/test_dir/t2/class1.mp4
/home/red/open/save_test/test_dir/t2/class2.mp4
/home/red/open/save_test/test_dir/t2/class3.mp4
/home/red/open/save_test/test_dir/t2/pic2.png
/home/red/open/save_test/test_dir/t2/pic/p1.jpg
/home/red/open/save_test/test_dir/t2/pic/p2.jpg
/home/red/open/save_test/test_dir/t2/pic/p3.jpg
/home/red/open/save_test/test_dir/t2/pic/newpic/jk.jpeg
/home/red/open/save_test/test_dir/t2/pic/newpic/mv.jpeg
/home/red/open/save_test/test_dir/t2/pic/newpic/open.jpeg
/home/red/open/save_test/test_dir/t3/d3/p1.txt
/home/red/open/save_test/test_dir/t3/d3/p2.txt
/home/red/open/save_test/test_dir/t3/d1/h1.txt
/home/red/open/save_test/test_dir/t3/d1/h2.txt
/home/red/open/save_test/test_dir/t3/d2/r2.txt
/home/red/open/save_test/test_dir/t3/d2/r1.txt
目录数: 9 | 文件数: 20
*/

/*
 * test_2
 *
== test_2 ==
0|DIR|.
/home/red/open/save_test/test_dir
1|DIR|t1
/home/red/open/save_test/test_dir/t1
2|REG|word3.txt
/home/red/open/save_test/test_dir/t1/word3.txt
2|REG|word1.txt
/home/red/open/save_test/test_dir/t1/word1.txt
2|REG|word2.txt
/home/red/open/save_test/test_dir/t1/word2.txt
1|DIR|t2
/home/red/open/save_test/test_dir/t2
2|REG|pic1.png
/home/red/open/save_test/test_dir/t2/pic1.png
2|REG|class1.mp4
/home/red/open/save_test/test_dir/t2/class1.mp4
2|REG|class2.mp4
/home/red/open/save_test/test_dir/t2/class2.mp4
2|REG|class3.mp4
/home/red/open/save_test/test_dir/t2/class3.mp4
2|REG|pic2.png
/home/red/open/save_test/test_dir/t2/pic2.png
2|DIR|word
/home/red/open/save_test/test_dir/t2/word
2|DIR|pic
/home/red/open/save_test/test_dir/t2/pic
3|REG|p1.jpg
/home/red/open/save_test/test_dir/t2/pic/p1.jpg
3|REG|p2.jpg
/home/red/open/save_test/test_dir/t2/pic/p2.jpg
3|REG|p3.jpg
/home/red/open/save_test/test_dir/t2/pic/p3.jpg
3|DIR|newpic
/home/red/open/save_test/test_dir/t2/pic/newpic
4|REG|jk.jpeg
/home/red/open/save_test/test_dir/t2/pic/newpic/jk.jpeg
4|REG|mv.jpeg
/home/red/open/save_test/test_dir/t2/pic/newpic/mv.jpeg
4|REG|open.jpeg
/home/red/open/save_test/test_dir/t2/pic/newpic/open.jpeg
1|DIR|t3
/home/red/open/save_test/test_dir/t3
2|DIR|d3
/home/red/open/save_test/test_dir/t3/d3
3|REG|p1.txt
/home/red/open/save_test/test_dir/t3/d3/p1.txt
3|REG|p2.txt
/home/red/open/save_test/test_dir/t3/d3/p2.txt
2|DIR|d1
/home/red/open/save_test/test_dir/t3/d1
3|REG|h1.txt
/home/red/open/save_test/test_dir/t3/d1/h1.txt
3|REG|h2.txt
/home/red/open/save_test/test_dir/t3/d1/h2.txt
2|DIR|d2
/home/red/open/save_test/test_dir/t3/d2
3|REG|r2.txt
/home/red/open/save_test/test_dir/t3/d2/r2.txt
3|REG|r1.txt
/home/red/open/save_test/test_dir/t3/d2/r1.txt
目录与文件数(加根节点): 30
*/
