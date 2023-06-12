#include "qbutt_col.h"

qbutt_col::qbutt_col(QWidget *parent) : QWidget(parent)
{

}

bool qbutt_col::set_pic(QString pic,int w,int h)
{
    _load_pic = _pix.load(pic);
    if(_load_pic)
    {
        //载入图片，保存透明度像素点
        if(w != 0 || h != 0) _pix = _pix.scaled(w,h); //设置缩放
        QImage img = _pix.toImage();
        _vec_alpha.resize(img.width());
        for(int w=0;w<img.width();w++)
        {
            _vec_alpha[w].resize(img.height());
            for(int h=0;h<img.height();h++)
            {
                _vec_alpha[w][h] = img.pixelColor(w, h).alpha();
            }
        }
        this->resize(img.size()); //重置大小到图片尺寸
    }
    return _load_pic;
}

void qbutt_col::set_col_foreground(status col)
{
    _paint_fore = true;
    _fore = col;
    ch_col_point(_fore.normal,_back.normal);
}

void qbutt_col::set_col_background(status col)
{
    _paint_back = true;
    _back = col;
    ch_col_point(_fore.normal,_back.normal);
}

void qbutt_col::set_keep_col(bool keep)
{
    _keep_col = keep;
}

void qbutt_col::set_frame(bool frame,QPen pen)
{
    _paint_frame = frame;
    _pen_frame = pen;
}

void qbutt_col::paintEvent(QPaintEvent *event)
{
    if(_load_pic)
    {
        QPainter *show = new QPainter(this);
        show->drawPixmap(0,0,_pix);

        if(_paint_frame)
        {
            show->setPen(_pen_frame);
            show->drawRect(this->rect());
        }
        show->end();
    }
    else QWidget::paintEvent(event);
}

void qbutt_col::enterEvent(QEvent *event)
{
    ch_col_point(_fore.enter,_back.enter);
    QWidget::enterEvent(event);
}

void qbutt_col::mousePressEvent(QMouseEvent *event)
{
    ch_col_point(_fore.press,_back.press);
    QWidget::mousePressEvent(event);
}

void qbutt_col::leaveEvent(QEvent *event)
{
    if(_keep_col == false) { ch_col_point(_fore.normal,_back.normal); }
    QWidget::leaveEvent(event);
}

void qbutt_col::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->rect().contains(event->pos())) { emit sn_clicked(); }
    ch_col_point(_fore.enter,_back.enter);
    QWidget::mouseReleaseEvent(event);
}

void qbutt_col::ch_col_point(QColor col_fore,QColor col_back)
{
    if(_paint_fore == false) return;
    QImage img = _pix.toImage(); //载入
    for (int w=0;w<img.width();w++)
    {
        for (int h=0;h<img.height();h++)
        {
            if(_vec_alpha[w][h] > 0 && _paint_fore)
            {
                col_fore.setAlpha(_vec_alpha[w][h]);
                img.setPixelColor(w,h,col_fore);
            }
            else if(_paint_back) { img.setPixelColor(w,h,col_back); }
        }
    }
    _pix = QPixmap::fromImage(img); //保存
    this->update();
}
