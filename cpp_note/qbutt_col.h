//!
//! qbutt_col.h
//!
#ifndef QBUTT_COL_H
#define QBUTT_COL_H

#include <QWidget>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>

//!
//! 功能：图片变色三态按钮
//! 用法：传入一张PNG图片（推荐黑白图片），传入三态RGB颜色，触发三态时根据RGB颜色改变图片颜色
//! 原理：保留PNG图片每个点位的颜色信息和透明度，如果点为不透明，根据三态RGB颜色改变点为颜色
//! 注意：前景指有颜色的像素点，背景指透明的像素点
//!
class qbutt_col : public QWidget
{
    Q_OBJECT
public:
    struct status
    {
        QColor normal;
        QColor enter;
        QColor press;
    };

public:
    explicit qbutt_col(QWidget *parent = nullptr);
    bool set_pic(QString pic,int w = 0,int h = 0);  //设置图片
    void set_col_foreground(status col);            //设置前景变化颜色
    void set_col_background(status col);            //设置背景变化颜色
    void set_keep_col(bool keep);                   //保持进入状态颜色
    void set_frame(bool frame,QPen pen);            //保持进入状态颜色 (QPen-width双倍数值可居中)

signals:
    emit void sn_clicked();

protected:
    //绘图事件
    void paintEvent(QPaintEvent *event) override;

    //触发事件
    void enterEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    //改变像素点颜色
    void ch_col_point(QColor col_fore,QColor col_back);

private:
    bool _keep_col = false;     //保存进入颜色
    bool _load_pic = false;     //加载图片
    bool _paint_frame = false;  //边框
    bool _paint_fore = false;   //前景
    bool _paint_back = false;   //背景
    status _fore;       //前景
    status _back;       //背景
    QPen _pen_frame;    //边框颜色
    QPixmap _pix;       //显示图片
    QVector<QVector<int>> _vec_alpha;   //记录像素点透明度
};

#endif // QBUTT_COL_H
