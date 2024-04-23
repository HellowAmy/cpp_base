#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QMenu>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QProgressBar>
#include <QTextEdit>
#include <QItemSelectionModel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVBoxLayout *lay_main = new QVBoxLayout(this);
    {
        QTextEdit *edit = new QTextEdit(this);
        lay_main->addWidget(edit);
    }
    {
        QString sty =
R"(
QScrollBar::handle {
    background-color: #daf108;
    margin: 2px;
}
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    border-radius: 5px;
    background-color: #15bb1d;
}
)";
        QListWidget *tab = new QListWidget(this);
        tab->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tab->setStyleSheet(sty);
        for(int i=0;i<10;i++)
        {
            tab->addItem(QString("a: %1").arg(i));
        }
        lay_main->addWidget(tab);
    }
    {
        QString sty =
            R"(
QScrollBar::handle {
    background-color: #daf108;
    margin: 2px;
}
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    border-radius: 5px;
    background-color: #15bb1d;
}
)";

        QTableWidget *tab = new QTableWidget(this);
        tab->setStyleSheet(sty);
        tab->setHorizontalHeaderLabels(QStringList()<<"a1"<<"b2"<<"c3"<<"d4");
        tab->setRowCount(10);
        tab->setColumnCount(4);
        tab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for(int i=0;i<4;i++)
        {
            for(int a=0;a<10;a++)
            {
                tab->setItem(a,i,new QTableWidgetItem(QString("item : [%1 : %2]").arg(a).arg(i)));
            }
        }
        lay_main->addWidget(tab);
    }

    {
        QHBoxLayout *lay = new QHBoxLayout;
        for(int i=0;i<3;i++)
        {
            auto prog = new QProgressBar(this);
            prog->setValue(50);
            prog->setRange(0,100);
            if(i == 0)
            {
                prog->setObjectName("ProgressBarTest");
            }
            lay->addWidget(prog);
        }
        lay_main->addLayout(lay);
    }
    {
        QHBoxLayout *lay = new QHBoxLayout;
        for(int i=0;i<3;i++)
        {
            auto butt = new QPushButton(QString("abc: %1").arg(i),this);
            butt->setToolTip("hellow world !!!");
            if(i == 0)
            {
                QMenu *mu = new QMenu(this);
                mu->addAction(new QAction("123"));
                mu->addAction(new QAction("456"));

                butt->setObjectName("PushButtonTest");
                butt->setToolTip(" ");
                butt->setMenu(mu);
            }
            lay->addWidget(butt);
        }
        lay_main->addLayout(lay);
    }

    {
        QHBoxLayout *lay = new QHBoxLayout;
        for(int i=0;i<3;i++)
        {
            auto butt = new QLineEdit(this);
            if(i == 0)
            {
                butt->setProperty("read_only",true);
            }
            lay->addWidget(butt);
        }
        lay_main->addLayout(lay);
    }
    {
        QWidget *wid = new QWidget(this);
        QScrollArea *area = new QScrollArea(this);
        area->resize(this->width(),300);
        area->setWidget(wid);
        wid->resize(wid->width(),2000);
        lay_main->addWidget(area);
    }
    {
        QWidget *wid = new QWidget(this);
        QScrollArea *area = new QScrollArea(this);
        area->resize(this->width()/2,100);
        area->setWidget(wid);
        wid->resize(this->width(),1000);
        lay_main->addWidget(area);

        QVBoxLayout *lay_wid = new QVBoxLayout(wid);
        lay_wid->setAlignment(Qt::AlignTop);
        {
            QHBoxLayout *lay = new QHBoxLayout;
            for(int i=0;i<3;i++)
            {
                auto *box = new QCheckBox(QString("123: %1").arg(i),wid);
                lay->addWidget(box);
            }
            lay_wid->addLayout(lay);
        }
        {
            QHBoxLayout *lay = new QHBoxLayout;
            for(int i=0;i<3;i++)
            {
                auto *box = new QRadioButton(QString("999: %1").arg(i),wid);
                lay->addWidget(box);
            }
            lay_wid->addLayout(lay);
        }

        {
            QHBoxLayout *lay = new QHBoxLayout;
            for(int i=0;i<3;i++)
            {
                auto *box = new QComboBox(wid);
                for(int a=0;a<5;a++)
                {
                    box->addItem(QString("abc: %1").arg(a));
                }

                lay->addWidget(box);
            }
            lay_wid->addLayout(lay);
        }
    }






}

Widget::~Widget()
{
    delete ui;
}

