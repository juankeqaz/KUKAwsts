#include "maintitle.h"

MainTitle::MainTitle(QList<QString> list, QWidget *parent):
    QWidget(parent)
{
    this->titletexts=list;
    w=this->width();
    h=this->height();
    hh=h/2;
    buttonhome=new MyButton(1,this);
    buttonhome->setFixedWidth(100);
    buttonhome->setFixedHeight(60);
    homeicon=new QLabel(buttonhome);
    homeicon->move(20,10);
    homeicon->resize(40,40);
    QImage *img=new QImage;
    img->load(":/image/Home.png");
    *img=img->scaled(40,40,Qt::KeepAspectRatio);
    homeicon->setPixmap(QPixmap::fromImage(*img));

    buttonconnect=new MyButton(2,this);
    buttonwpice=new MyButton(2,this);
    buttoncsset=new MyButton(2,this);
    buttontest=new MyButton(2,this);
    buttonrun=new MyButton(2,this);
    buttonanysis=new MyButton(3,this);
    bt<<buttonhome<<buttonconnect<<buttonwpice<<buttoncsset<<buttontest<<buttonrun<<buttonanysis;
    //initforms();
    QImage *uncheckedimg=new QImage;
    uncheckedimg->load(":/image/msg_error.png");
    uncheckedimg->scaled(20,20);
    QImage *checkedimg=new QImage;
    checkedimg->load(":/image/msg_info.png");
    checkedimg->scaled(20,20);

    QString checkstyle="QLabel{spacing: 2px;}QCheckBox::indicator{width: 15px;height: 15px;} \
            QCheckBox::indicator:unchecked{image:url(:/image/msg_error.png);} \
            QCheckBox::indicator:checked{image:url(:/image/msg_info.png);}";
    for(int i=0;i<bt.size();++i)
    {
        if(i>0)
        {
            QLabel *box=new QLabel(bt.at(i));
            bt_check<<box;
            box->resize(20,20);
            box->move(40,(60-box->height())/2);
            box->setStyleSheet("border-image: url(:/image/msg_error.png);");
            //box->setPixmap(QPixmap::fromImage(*uncheckedimg));
            QLabel* textbox = new QLabel(bt.at(i));
            textbox->setFont(QFont("微软雅黑",16));
            textbox->setText(QString(titletexts.at(i)));
            textbox->setAlignment(Qt::AlignVCenter);
            textbox->setFixedHeight(60);
            textbox->setFixedWidth(100);
            textbox->move(65,0);
            textbox->lower();
            bt_text<<textbox;
        }
        connect(bt.at(i),SIGNAL(btnclicked()),this,SLOT(btisclicked()));
    }


}
MainTitle::~MainTitle()
{

}

void MainTitle::initforms()
{   
    buttonhome->move(0,0);
    int wd=(w-buttonhome->width()+6*h/2)/(titletexts.size()-1);
    buttonconnect->resize(wd,h);
    buttonconnect->move(buttonhome->width()-h/2,0);
    buttonwpice->resize(wd,h);
    buttonwpice->move(buttonhome->width()+wd-2*h/2,0);
    buttoncsset->resize(wd,h);
    buttoncsset->move(buttonhome->width()+2*wd-3*h/2,0);
    buttontest->resize(wd,h);
    buttontest->move(buttonhome->width()+3*wd-4*h/2,0);
    buttonrun->resize(wd,h);
    buttonrun->move(buttonhome->width()+4*wd-5*h/2,0);
    buttonanysis->resize(wd,h);
    buttonanysis->move(buttonhome->width()+5*wd-6*h/2,0);
}

void MainTitle::btisclicked()
{
    for(int i=0;i<bt.size();++i)
    {
        MyButton *cbt=bt.at(i);
        if(sender()==cbt)
        {
            cbt->setcheck(true);
            emit clicked(i);
        }else
            cbt->setcheck(false);
    }
}

void MainTitle::checkstatus(int flag)
{
    QLabel *box =bt_check.at(flag);
    box->setStyleSheet("border-image: url(:/image/msg_info.png);");
}

void MainTitle::uncheckstatus(int flag)
{
    QLabel *box =bt_check.at(flag);
    box->setStyleSheet("border-image: url(:/image/msg_error.png);");
}

void MainTitle::resizeEvent(QResizeEvent *event)
{

    w=event->size().width();
    h=event->size().height();
    hh=h/2;
    initforms();
    for(int i=1;i<bt.size();++i)
    {
        QLabel *box=bt_check.at(i-1);
        QLabel *textbox=bt_text.at(i-1);
        int wd=bt.at(i)->width();
        box->move((wd-120+30)/2,(60-box->height())/2);
        textbox->move((wd-100+55)/2,0);
    }

}

void MainTitle::paintEvent(QPaintEvent *)
{
    //    w=width();
    //    h=height();
    //    hh=h/2;
    //    initforms();
}
