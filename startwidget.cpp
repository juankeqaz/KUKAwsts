#include "startwidget.h"
#include "ui_startwidget.h"
#include <QHBoxLayout>
#include <qtextcodec.h>
#include <QDebug>


startwidget::startwidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startwidget)
{
    ui->setupUi(this);

    mlbg=new QLabel(ui->widget_ct);
    mlbg->setPixmap(QPixmap(":/image/starsky.jpg"));
    mlbg->setScaledContents(true);
    mlbg->lower();
    int xcenter=width()/2;
    tbtn1=new imagebutton(this);
    tbtn2=new imagebutton(this);
    tbtn3=new imagebutton(this);
    tbtn3->setopbase(0.8);
    tbtn4=new imagebutton(this);
    tbtn5=new imagebutton(this);
    runpage=new Runpage();

    ipinput=IPInputbox::Instance();
    anysis=DataAnysis::Instance();
    workpiece=WorkPiece::Instance();
    scanner=scannersetting::Instance();
    Detec=DetecSting::Instance();


    resizecards();
    inittoolbtn();


    ui->widget_list->move((width()-ui->widget_list->width())/2,height()-ui->widget_list->height()-40);
    ui->widget_flag->move(xcenter-ui->widget_flag->width()/2,ui->widget_list->y()-ui->widget_flag->height());

    connect(workpiece,SIGNAL(definedshape(QStringList&)),Detec,SLOT(getshapepara(QStringList&)));


}

startwidget::~startwidget()
{
    delete ui;
    delete runpage;
    delete ipinput;
}

void startwidget::switchcards()
{
    imagebutton *bt = qobject_cast<imagebutton*>(sender());
    QString names=bt->gettext();
    risecard(names);

}

void startwidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int wd=this->width();
    int ht=this->height();

    ui->widget_list->move((wd-ui->widget_list->width())/2,ht-ui->widget_list->height()-40);
    ui->widget_flag->move((wd-ui->widget_flag->width())/2,ui->widget_list->y()-ui->widget_flag->height());
    ui->widget_ct->resize(wd,ui->widget_flag->y());
    mlbg->resize(ui->widget_ct->size());
    resizecards();
}

void startwidget::inittoolbtn()
{
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    btns.append(qMakePair(codec->toUnicode("连接设置"),QString(":/connect.ico")));
//    btns.append(qMakePair(codec->toUnicode("工件设置"),QString(":/connect.ico")));
//    btns.append(qMakePair(codec->toUnicode("超声设置"),QString(":/connect.ico")));
//    btns.append(qMakePair(codec->toUnicode("检测设置"),QString(":/connect.ico")));
//    btns.append(qMakePair(codec->toUnicode("检测执行"),QString(":/connect.ico")));
//    btns.append(qMakePair(codec->toUnicode("数据分析"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("连接设置"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("工件设置"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("超声设置"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("检测设置"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("检测执行"),QString(":/connect.ico")));
    btns.append(qMakePair(QString("数据分析"),QString(":/connect.ico")));

    toolbuttonlist<<tbtn1<<tbtn2<<tbtn3<<tbtn4<<tbtn5;
    buttonlist<<ui->pushButton_1<<ui->pushButton_2<<ui->pushButton_3<<ui->pushButton_4<<ui->pushButton_5<<ui->pushButton_6;

    for(int i=0;i<toolbuttonlist.size();++i)
    {
        imagebutton *bt=toolbuttonlist.at(i);
        bt->seticon(btns.at(i).second);
        bt->settext(btns.at(i).first);
        connect(bt,SIGNAL(clicked()),this,SLOT(switchcards()));
    }
    for(int i=0;i<buttonlist.size();i++)
    {
        connect(buttonlist.at(i),SIGNAL(clicked()),this,SLOT(onpushButtonclicked()));
    }
}

void startwidget::resizecards()
{
    tbtn3->setFixedSize(QSize(190,360));
    tbtn2->setFixedSize(QSize(160,290));
    tbtn1->setFixedSize(QSize(130,240));
    tbtn4->setFixedSize(tbtn2->size());
    tbtn5->setFixedSize(tbtn1->size());
    tbtn3->move((this->width()-tbtn3->width())/2,(ui->widget_flag->y()-tbtn3->height())/2);
    tbtn2->move(tbtn3->x()-tbtn2->width(),(ui->widget_flag->y()-tbtn2->height())/2);
    tbtn1->move(tbtn2->x()-tbtn1->width(),(ui->widget_flag->y()-tbtn1->height())/2);
    tbtn4->move(tbtn3->x()+tbtn3->width(),tbtn2->y());
    tbtn5->move(tbtn4->x()+tbtn4->width(),tbtn1->y());
}

void startwidget::risecard(QString names)
{
    int index=-1;
    for(int i=0;i<btns.size();i++)
    {
        if(btns.at(i).first==names)
        {
            index=i;
            break;
        }
    }
    if(index<0) return;
    //qDebug()<<names<<index;
    QList<QPair<QString,QString> >subpair;
    subpair.clear();
    int p=-1;
    for(int i=0;i<5;i++)
    {
        if(index-2+i<0)
        {
            p = index-2+i+btns.size();
        }
        else if(index-2+i>=btns.size())
        {
            p=(index-2+i)%btns.size();
        }
        else
        {
            p=index-2+i;
        }
        subpair.push_back(btns.at(p));
    }
    for(int i=0;i<5;++i)
    {
        toolbuttonlist.at(i)->seticon(subpair.at(i).second);
        toolbuttonlist.at(i)->settext(subpair.at(i).first);
    }
    showbynames(index);
}

void startwidget::showbynames(int index)
{
    qDebug()<<index;
    switch (index) {
    case 0:
        ipinput->show();
        break;
    case 1:
        workpiece->show();
        break;
    case 2:
        scanner->show();
        break;
    case 3:
        Detec->show();
        break;
    case 4:
        runpage->show();
        break;
    case 5:
        anysis->show();
        break;
    default:
        break;
    }
}

void startwidget::onpushButtonclicked()
{
    QPushButton *bt= qobject_cast<QPushButton*> (sender());

    QString names=bt->text();
    risecard(names);
}
