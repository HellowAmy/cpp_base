//!
//! 随笔:Qt使用QWidget绘制三态按钮
//!
//! ===== 简介 =====
//! 当Qt的控件不能瞒住需求是，往往需要自绘控件，
//!     本次提供的三态按钮只是一种思路，并无实际使用意义，
//!     主要用于学习思路的延展
//! 按钮的触发依赖QWidget的事件，在不同事件触发时记录状态，
//!     据不同状态绘制对应的图形
//! ===== 简介 =====
//!
#ifndef QT_BUTT_TREE_H
#define QT_BUTT_TREE_H

#ifndef MYBUT_H
#define MYBUT_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>
#define qout qDebug()

class mybut : public QWidget
{
    Q_OBJECT
public:
    enum status
    {
        en_normal,
        en_enter,
        en_cilck,
    };
public:
    explicit mybut(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    //===== 重写绘制函数 =====
    void paintEvent(QPaintEvent *event) override
    {
        QPainter *show = new QPainter(this);
        switch(_s)
        {
        case en_normal: paint_normal(show); break;
        case en_enter: paint_enter(show); break;
        case en_cilck: paint_cilck(show); break;
        }
        show->end();
    }
    //===== 重写绘制函数 =====


    //===== 重写事件触发 =====
    //鼠标进入时触发
    void enterEvent(QEvent *event) override
    {
        qout<<"enterEvent";
        _s = en_enter;                  //改变状态
        this->update();                 //更新UI--触发paintEvent函数
        QWidget::enterEvent(event);     //交给系统（默认操作，否则导致无事件可用）
    }

    //鼠标离开时触发
    void leaveEvent(QEvent *event) override
    {
        qout<<"moveEvent";
        _s = en_normal;
        this->update();
        QWidget::leaveEvent(event);
    }

    //鼠标按下立刻触发
    void mousePressEvent(QMouseEvent *event) override
    {
        qout<<"mousePressEvent";
        _s = en_cilck;
        this->update();
        QWidget::mousePressEvent(event);
    }

    //鼠标松开立刻触发
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        qout<<"mouseReleaseEvent";
        _s = en_enter;
        this->update();
        QWidget::mouseReleaseEvent(event);
    }
    //===== 重写事件触发 =====

private:
    status _s = en_normal;  //记录当前状态，用于绘制切换
    int _space = 5;         //内外边框的边距

    //===== 三态切换绘制 =====
    void paint_normal(QPainter *show)
    {
        //绘制外边框
        show->setPen(QColor(Qt::red));
        QRect r = this->rect();
        sub_rect(r,QRect(0,0,1,1));
        show->drawRect(r);
    }

    void paint_enter(QPainter *show)
    {
        show->setPen(QColor(Qt::red));
        QRect r = this->rect();
        sub_rect(r,QRect(0,0,1,1));
        show->drawRect(r); //绘制外边框

        sub_rect(r,QRect(_space,_space,_space,_space));
        show->drawRect(r); //绘制内边框
    }

    void paint_cilck(QPainter *show)
    {
        paint_normal(show); //绘制外边框

        show->setBrush(QColor(Qt::red));
        QPainterPath path;
        path.addRect(_space,_space,this->width()-_space*2 -1,this->height()-_space*2 -1);
        show->drawPath(path); //绘制区域颜色
    }
    //===== 三态切换绘制 =====

    //范围修正（不修正会出现边缘溢出）
    void sub_rect(QRect &r1,QRect r2)
    {
        r1.setX(r1.x() + r2.x());
        r1.setY(r1.y() + r2.y());
        r1.setWidth(r1.width() - r2.width());
        r1.setHeight(r1.height() - r2.height());
    }
};

#endif // MYBUT_H


#endif // QT_BUTT_TREE_H
