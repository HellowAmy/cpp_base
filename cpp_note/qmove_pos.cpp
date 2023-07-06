#include "qmove_pos.h"

qmove_pos::qmove_pos(QObject *parent) : QObject(parent)
{

}

void qmove_pos::add_wid(QWidget *wid)
{
    _vec.push_back(wid);
}

QSize qmove_pos::move_x(QPoint pos, int space)
{
    return move_group(pos,space,-1,1);
}

QSize qmove_pos::move_y(QPoint pos, int space)
{
    return move_group(pos,space,1,-1);
}

QSize qmove_pos::move_group(QPoint pos,int space,int row, int column)
{
    if(row < 0 && column < 0)
    {
        //默认时尽可能形成正方形矩阵
        //计算方法：1.开平方获取一边 2.除以开平方获取另一条边 3.取两边平均数作为矩阵长度
        row = qSqrt(_vec.size());
        row += ((_vec.size()/row) -row) /2;
    }

    int max_wide = 0;
    int max_high = 0;
    int count_row = 0;
    int count_column = 0;
    QPoint pos_move = pos;
    for(auto &a:_vec)
    {
        //记录控件占领的最大区域
        a->move(pos_move);  //移动位置
        int now_wide = (pos_move.x() + a->width()) - pos.x();
        int now_high = (pos_move.y() + a->height()) - pos.y();
        if(max_wide < now_wide) max_wide = now_wide;
        if(max_high < now_high) max_high = now_high;

        if(row > 0) //row
        {
            //每次移动保留点，超距则修改保留点位置
            pos_move = QPoint(pos_move.x() + a->width() + space,pos_move.y());
            count_row++;
            if(count_row >= row)
            {
                pos_move.setX(pos.x());
                pos_move.setY(pos_move.y() + a->height() + space);
                count_row = 0;
            }
        }
        else if(column > 0) //column
        {
            pos_move = QPoint(pos_move.x(),pos_move.y() + a->height() + space);
            count_column++;
            if(count_column >= column)
            {
                pos_move.setX(pos_move.x() + a->width() + space);
                pos_move.setY(pos.y());
                count_column = 0;
            }
        }
    }

    return QSize(max_wide,max_high);
}

void qmove_pos::move_center(QWidget *wid)
{
    QDesktopWidget* desk = QApplication::desktop();
    wid->move((desk->width() - wid->width()) /2, (desk->height() - wid->height()) /2);
}
