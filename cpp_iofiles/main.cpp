//!
//! C++基础：C++与C风格文件读写方式比对
//!
//! ===== 文件流简介 =====
//! 我认为输入输出流是一种非常难理解的概念，可能主要归功于翻译的锅，
//!     如输出/输入的写法：
//!         cout<<buf : 内存buf输出到设备屏幕
//!         cin>>buf : 键盘输入到内存buf中
//!     这种流的理解导致初学者在对设备与内存，文件的读写都存在误差，
//!         不利于快速掌握与运用，其中对文件的操作也采用了流的概念，
//!         将输入输出流与文件结合在一起
//!
//!     对于文件为什么是IO流，而不是其他的称呼，我的猜想是设计者希望文件时一串数据，
//!         不管文件有多长，只需要用一个字符串和一个文件指针就能表示整个文件，
//!         以及文件的进度
//!     都知道文件占用的磁盘很大，往往数据存储的实际物理空间并不是连续的，
//!         只是散列在磁盘中各个角落的二进制数据，而设计成连续为一条直线的二进制串，
//!         有利于对文件的操作而不用关心文件是如何保存的
//!     在下面的测试中，对流的描述不会关心是如何流入流出的，而是采用读取与写入的描述方式
//! ===== 文件流简介 =====
//!
//!
//! ===== C++文件读写 =====
//! ofstream : 默认写文件类
//! ifstream : 默认读文件类
//! fstream  : 读写文件类--无默认模式，通过参数设置（常用）
//!
//! fstream类同时继承ofstream与ifstream，通常构造的时间成本更大，
//!     但是功能更多，操作灵活
//!
//! 类操作:
//! 文件打开方式: 构造函数/open函数
//! 读取一行内容: >>重载/getline函数
//! ===== C++文件读写 =====
//!
//!
//! ===== ios 参数 =====
//! open函数的ios参数:
//! （参数可通过按位或的方式启动多个模式 -- "ios:in|ios:out" 同时启动读与写模式）
//!     ios::app     : 追加模式--写入内容追加到文件末尾
//!     ios::in      : 只读模式--读文件时使用
//!     ios::out     : 只写模式--写文件时使用
//!     ios::ate     : 停留模式--打开文件时将文件指针偏移到文件末尾
//!     ios::trunc   : 截断模式--重置为新文件
//!     ios::binary  : 字节模式--二进制模式进行读写，默认是字符读写
//!
//! seekg函数的ios参数:
//!     ios::beg     : 文件头部
//!     ios::cur     : 当前位置
//!     ios::end     : 文件尾部
//! ===== ios 参数 =====
//!
//!
//! ===== C风格文件读写 =====
//! fopen函数选项:
//!     r : 只读模式
//!     w : 只写模式
//!     a : 追加模式
//!     b : 字节模式
//!     + : 强化模式
//! fopen函数的模式选项非常多，可以随意组合，其中 "+" 强化之后会有变化，
//!     组合非常多，而C库的文件操作并不是特别友好，除了网络读写方便之外，
//!     对字符的操作不如fstream方便
//! C库的文件读写的好处在于，读写会回返成功读写的字节数，
//!     没有状态标志位，方便对文件指针进行管理，方便指针转跳，更接近Linux系统调用，
//!     与网络套接字相互匹配，可以与系统调用描述符相互转换
//!
//! 常用模式组合:
//!     a   : 追加
//!     w+  : 截断写
//!     r+  : 读写
//!     rb+ : 二进制读写
//!
//! fseek函数的文件偏移:
//! 例子: fseek(fd,0,SEEK_SET);
//!     参数1:文件指针,参数2:偏移方向，同fstream,参数3:偏移定位
//!
//!偏移定位:
//!     SEEK_SET : 文件头部
//!     SEEK_CUR : 当前位置
//!     SEEK_END : 文件尾部
//! ===== C风格文件读写 =====
//!
//!
//! 结束语:
//!     对于字符串文件的读写C++的fstream有方便的输入输出重载，
//!         C库的文件流因为可以更灵活的操作文件指针，
//!         则更适合对二进制读取与网络字节的兼容
//!
#include <iostream>

using namespace std;

//===== C++文件读写测试 =====
#include <fstream>
using namespace std;

//显示文件内容
void show_file(const string &filename)
{
    cout<<"== show file =="<<endl;
    ifstream ifs(filename);
    if(ifs.is_open())
    {
        string str;
        while (getline(ifs,str)) { cout<<str<<endl; }
        ifs.close();
    }
    cout<<"== show end =="<<endl;
}


void test_1()
{
    const string filename = "test1.txt"; //测试文件名

    //== 显示文章 ==
    const string str_show =
R"(明月几时有？
把酒问青天。
不知天上宫阙，今夕是何年。
我欲乘风归去，又恐琼楼玉宇，高处不胜寒。
起舞弄清影，何似在人间。
转朱阁，低绮户，照无眠。
不应有恨，何事长向别时圆？
人有悲欢离合，月有阴晴圆缺，此事古难全。
但愿人长久，千里共婵娟。)";
    //== 显示文章 ==


    //== 写入文字 ==
    //ofstream 使用构造函数方式打开文件
    ofstream ofs(filename);
    if(ofs.is_open())
    {
        ofs<<str_show; //写入内容到文件
        ofs.close();
    }
    //== 写入文字 ==


    //== 读取文字 ==
    //ifstream 使用open函数打开文件
    ifstream ifs;
    ifs.open(filename);
    if(ifs.is_open())
    {
        string str;
        ifs>>str;   //使用 >> 重载读一行内容
        cout<<"ifs: "<<str<<endl<<endl;
        ifs.close();
    }
    //== 读取文字 ==


    //== 追加文字 ==
    fstream fs1(filename,ios::app);
    if(fs1.is_open())
    {
        //文件漂移在末尾
        cout<<"app偏移: "<<fs1.tellg()<<endl; //获取当前的文件指针偏移
        fs1<<endl<<"作者 苏轼";               //需要手动追加换行符
        fs1<<endl<<"时间 宋代";
        cout<<"app偏移: "<<fs1.tellg()<<endl;
        fs1.close();
    }
    //== 追加文字 ==


    //== 读模式尾部定位 ==
    fstream fs2(filename,ios::in | ios::ate);
    if(fs2.is_open())
    {
        //!例子: seekg(0,ios::beg),参数1:偏移量，参数2:ios
        //!偏移量:
        //!    负 : 文件头部偏移
        //!    正 : 文件尾部偏移
        //!    零 : 不偏移
        cout<<"ate偏移: "<<fs2.tellg()<<endl;
        fs2.seekg(-6,ios::cur); //向前偏移6个字节（三个字节等于一个汉字）
        string str;
        fs2>>str;
        cout<<"读两个汉字: "<<str<<endl;
        fs2.close();
    }
    //== 读模式尾部定位 ==


    //== 字节读取与写入 ==
    fstream fs3(filename,ios::in | ios::out);
    if(fs3.is_open())
    {
        //四个函数
        char buf[12];
        fs3.read(buf,sizeof(buf));
        cout<<"gcount: "<<fs3.gcount()<<endl; //一般只能用于返回read读取的个数，write不行
        cout<<"buf: "<<buf<<endl;

        char bufw[] = "会出来";
        fs3.write(bufw,sizeof(bufw));
        cout<<"bufw: "<<bufw<<endl;
        fs3.close();
    }
    //== 字节读取与写入 ==


    //== 标记位状态 ==
    fstream fs5(filename,ios::in);
    if(fs5.is_open())
    {
        cout<<endl<<"====="<<endl;
        string str;
        fs5>>str;
        cout<<"成功读一行: "<<str<<endl;
        cout<<"good: "<<fs5.good()<<"| fail: "<<fs5.fail()<<endl;
        fs5.close();

        //文件指针到结尾后读一行读不到内容，但不出错
        while(fs5.eof()){ fs5>>str; }
        str.clear();
        cout<<"文件指针到末尾后读一行: "<<str<<endl;
        cout<<"good: "<<fs5.good()<<"| fail: "<<fs5.fail()<<endl;

        //流关闭后读文件fail位标记失败
        str.clear();
        fs5>>str;
        cout<<"文件关闭后读一行: "<<str<<endl;
        cout<<"good: "<<fs5.good()<<"| fail: "<<fs5.fail()<<endl;
        cout<<"====="<<endl;
    }
    //== 标记位状态 ==


    //===== 常规操作三部曲 =====
    //文件操作三部曲：打开，读写，关闭
    //  文件打开时需要判断是否打开成功，负责可能后续操作会导致程序崩溃
    fstream fs6(filename,ios::in | ios::out); //打开
    if(fs6.is_open())
    {
        //== 读写 ==
        int count = 0; string str;
        cout<<endl<<"====="<<endl;
        while (getline(fs6,str)) { cout<<str<<endl; count++; }
        cout<<"====="<<endl;
        cout<<"行数："<<count<<endl<<endl;
        //== 读写 ==

        fs6.close(); //关闭
    }
    else cout<<"open err"<<endl;
    //===== 常规操作三部曲 =====


    //===== 截断文件 =====
    //定位到文件头部，内容还在
    //  ios::trunc 截断模式只有在同时读写时才有意义，因为ios::out也会截断文件
    cout<<endl<<"===== 非截断 ====="<<endl;
    fstream fs7(filename,ios::out | ios::in);
    if(fs7.is_open())
    {
        fs7<<"苏轼：今天写水调歌头给皇帝交差，明月及时会出来。。。算了被贬再说\n";
        fs7.close();
    }
    show_file(filename);

    //定位到文件头部，截断内容为空
    cout<<endl<<"===== 截断 ====="<<endl;
    fstream fs8(filename,ios::out | ios::in |ios::trunc);
    if(fs8.is_open())
    {
        fs8<<"苏轼：今天写水调歌头给皇帝交差，明月及时会出来。。。算了被贬再说\n";
        fs8.close();
    }
    show_file(filename);
    //===== 截断文件 =====


    //===== 二进制读写 =====
    //二进制必须用read/write函数，否则非字符服务读取，
    //      gcount函数记录read读取成功的字节数
    cout<<endl<<"===== 二进制读写 ====="<<endl;
    fstream fs9(filename,ios::out | ios::in | ios::binary);
    if(fs9.is_open())
    {
        size_t len_file = 0;
        string str_all;
        char buf[20];

        //获取文件长度，偏移到末尾记录长度，再回到文件头部
        fs9.seekg(0,ios::end);
        len_file = fs9.tellg();
        fs9.seekg(0,ios::beg);

        while(fs9.eof() == false)
        {
            fs9.read(buf,sizeof(buf));
            str_all.append(buf,fs9.gcount());
        }
        cout<<"str_all size: "<<str_all.size() <<"| len_file: "<<len_file<<endl;

        //清空标志位，定位到文件头部重新读取
        fs9.clear();
        fs9.seekg(0,ios::beg);

        while(fs9.eof() == false)
        {
            fs9.read(buf,sizeof(buf));
            str_all.append(buf,fs9.gcount());
        }
        cout<<"str_all size: "<<str_all.size() <<"| len_file: "<<len_file<<endl;

        //清空标志位，将前两次读到的二进制追加到原文件中
        fs9.clear();
        fs9.seekg(0,ios::end);
        str_all = "新内容："+str_all;
        fs9.write(str_all.c_str(),str_all.size());
        fs9.close();
    }
    show_file(filename);
    //===== 二进制读写 =====
}
//===== C++文件读写测试 =====



//===== C风格读写测试 =====
#include <stdio.h>
#include <string.h>
void test_2()
{
    const string filename = "test2.txt"; //测试文件名

    //== 显示文章 ==
    const string str_show =
R"(金樽清酒斗十千，玉盘珍羞直万钱。
停杯投箸不能食，拔剑四顾心茫然。
欲渡黄河冰塞川，将登太行雪满山。
闲来垂钓碧溪上，忽复乘舟梦日边。
行路难，行路难，多歧路，今安在？
长风破浪会有时，直挂云帆济沧海。)";
    //== 显示文章 ==

    cout<<endl<<endl<<"===== C风格读写 ====="<<endl;

    //== 写模式 ==
    //fopen函数，参数1：文件名，参数2：打开模式
    FILE* fd1 = fopen(filename.c_str(),"w");
    if(fd1)
    {
        //写入文章到文件
        //  参数1：buf地址，参数2：buf长度，参数3：buf要写入的个数，参数4：文件指针
        //  返回值：返回成功写入的buf个数，一般与参数3相等
        size_t size = fwrite(str_show.c_str(),str_show.size(),1,fd1);
        if(size != 1) cout<<"fwrite err"<<endl;
        cout<<"ftell: "<<ftell(fd1)<<endl; //获取当前文件偏移
        fclose(fd1);
    }

    //== 读模式 ==
    FILE* fd2 = fopen(filename.c_str(),"r");
    if(fd2)
    {
        //读全部
        char buf[1024];
        while(fgets(buf,sizeof(buf),fd2) != nullptr)
            { cout<<buf; }
        cout<<endl<<"ftell: "<<ftell(fd1)<<endl;

        //读一行
        fseek(fd2,0,SEEK_SET); //偏移文件指针，定位到文件头部
        if(fgets(buf,sizeof(buf),fd2) != nullptr)
            { cout<<buf; }
        fclose(fd2);
    }

    //== 二进制读写 ==
    FILE* fd3 = fopen(filename.c_str(),"rb+");
    if(fd3)
    {
        //读四个字符
        char buf[13];
        buf[12] = '\0';

        //fread读取buf，参数与fwrite相同
        if(fread(buf,sizeof(buf)-1,1,fd3) != 1)
            { cout<<"fwrite err"<<endl; }
        cout<<"fread: "<<buf<<endl;

        //写四个字符--覆盖原文件前四个字节
        fseek(fd3,0,SEEK_SET);
        char buf2[12];
        strncpy(buf2,"破杯清水",sizeof(buf));
        fwrite(buf2,sizeof(buf2),1,fd3);
        cout<<"buf2: "<<"破杯清水"<<endl;

        fclose(fd3);
    }
    cout<<endl;
    show_file(filename);

    //== 文件截断 ==
    FILE* fd4 = fopen(filename.c_str(),"w");
    if(fd4)
    {
        string str = "没有东西了";
        fwrite(str.c_str(),str.size(),1,fd1);
        fclose(fd4);
    }
    cout<<endl;
    show_file(filename);
}
//===== C风格读写测试 =====



int main()
{
    cout<<"===== begin ====="<<endl;
    test_1();
    test_2();

    cout<<"===== end ====="<<endl;
    return 0;
}
