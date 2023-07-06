#ifndef QMOVE_POS_H
#define QMOVE_POS_H

#include <QtMath>
#include <QObject>
#include <QWidget>
#include <QVector>
#include <QApplication>
#include <QDesktopWidget>

class qmove_pos : public QObject
{
    Q_OBJECT
public:
    explicit qmove_pos(QObject *parent = nullptr);

    //== 位移操作 ==
    //添加到管理容器，然后通过移动布局函数控制加入的控件
    void add_wid(QWidget *wid);

    //x轴移动布局 (封装: move_group)
    QSize move_x(QPoint pos = QPoint(0,0),int space = 0);

    //y轴移动布局 (封装: move_group)
    QSize move_y(QPoint pos = QPoint(0,0),int space = 0);

    //矩阵布局，超过数量则换行，可指定方向
    //参数:[pos: 起点][space: 每个控件的间隔][row : 一排最大数量][column : 一列最大数量]
    //返回值: [QSize: 矩阵占领的最大边缘范围]
    //注意: row 和 column 必须有一个值为 -1，默认时取最接近正方形的布局
    QSize move_group(QPoint pos = QPoint(0,0),int space = 0,int row = -1,int column = -1);
    //== 位移操作 ==

    //相对于屏幕窗口的居中
    static void move_center(QWidget *wid);

private:
    QVector<QWidget *> _vec;
};

#endif // QMOVE_POS_H
