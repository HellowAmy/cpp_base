//!
//! 随笔：Qt变色图像三态按钮
//!
//! == 控件简介 ==
//! 本次提供继承QWidget的按钮控件，提供显示图片功能，
//!     可根据图片透明度设置前景色和后景色，可添加边框和设置保持进入状态颜色，
//!     提供sn_clicked信号（鼠标抬起时发出）
//! == 控件简介 ==
//!
//! 结束语：
//!     文章后提供qbutt_col.h和qbutt_col.cpp文件
//!
#include "../qbutt_col.h"
#include "widget.h"
#include <iostream>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    qbutt_col bu1(&w);
    bu1.set_pic("/home/red/open/github/qt_hur/pic/m_one/m_2.png");
    bu1.set_col_foreground({0x808080,0x101010,0xff0000});  //前景色
    bu1.set_col_background({0xffaa44,0xff0000,0xffff00});  //背景色
    bu1.set_frame(true,QPen(QColor(0x010000),6));          //设置边框（宽度设置偶数可居中）
    bu1.show();

    w.connect(&bu1,&qbutt_col::sn_clicked,&w,[](){
        cout<<"sn_clicked"<<endl;
    });

    qbutt_col bu2(&w);
    bu2.move(bu1.width()+10,0);
    bu2.set_pic("/home/red/open/github/qt_hur/pic/m_one/m_2.png",100,200);
    bu2.set_col_foreground({0x808080,0x101010,0xff0000});  //前景色
    bu2.set_col_background({0xffaa44,0xff0000,0xffff00});  //背景色
    bu2.set_frame(true,QPen(QColor(0x010000),6));          //设置边框（宽度设置偶数可居中）
    bu2.set_keep_col(true); //保持进入颜色
    bu2.show();

    w.connect(&bu2,&qbutt_col::sn_clicked,&w,[](){
        cout<<"sn_clicked"<<endl;
    });

    return a.exec();
}
