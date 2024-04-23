//!
//!
//! 官方Qt样式表Demo展示:
//!     https://doc.qt.io/qt-5/stylesheet-examples.html
//!

#include "widget.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <iostream>

// 读取指定目录下的所有样式表
QString read_file_qss(QString path)
{
    QString style;
    QDir dir(path);
    auto ls = dir.entryList();
    for(auto &a:ls)
    {
        QFileInfo info(path + a);
        if(info.isFile() && info.suffix() == "qss")
        {
            QFile fs(info.absoluteFilePath());
            if(fs.open(QIODevice::ReadOnly))
            {
                style += fs.readAll();
                fs.close();
            }
        }
    }
    return style;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString style = read_file_qss("../test_qss/qss/");
//    qDebug()<<"style: "<<style;
    std::cout<<"style: "<<style.toStdString()<<std::endl;

    a.setStyleSheet(style);


    Widget w;
    w.show();
    return a.exec();
}
