//!
//!
//!
//!
//!
//!
#include "../qmove_pos.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>

#include <QDebug>
#define qout qDebug()

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.show();

    qmove_pos::move_center(&w);

    auto fn_bu = [&](QString txt){
        QPushButton *p = new QPushButton(&w);
        p->resize(50,50);
        p->show();
        p->setText(txt);
        return p;
    };

    {
        qmove_pos move;
        for(int i=1;i<=5;i++)
        {
            move.add_wid(fn_bu(QString::number(i)));
        }
        auto size = move.move_x(QPoint(100,0));
        qout<<size;
    }
    {
        qmove_pos move;
        for(int i=1;i<=5;i++)
        {
            move.add_wid(fn_bu(QString::number(i)));
        }
        auto size = move.move_y(QPoint(0,0),10);
        qout<<size;
    }
    {
        qmove_pos move;
        for(int i=1;i<=25;i++)
        {
            move.add_wid(fn_bu(QString::number(i)));
        }

        auto size = move.move_group(QPoint(100,100),10);
        qout<<size;
    }

    return a.exec();
}
