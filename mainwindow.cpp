#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mlbg=new QLabel(ui->page_start);
    mlbg->setPixmap(QPixmap(":/image/starsky.jpg"));
    mlbg->setScaledContents(true);
    mlbg->lower();
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_sub->setCurrentIndex(0);
    initform();
    homesetingpage=new Homeseting();
    homesetingpage->hide();
    qRegisterMetaType<Robot_Path> ("Robot_Path");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initform()
{
    IconHelper::Instance()->setIcon(ui->pushButton_showset, 0xf053, 15);

    ttext<<" "<<"连接设置"<<"工件设置"<<"超声设置"<<"检测设置"<<"检测执行"<<"数据分析";
    title =new MainTitle(ttext);
    connect(title,SIGNAL(clicked(int)),this,SLOT(getcurrentindex(int)));

    QHBoxLayout *ttlayout=new QHBoxLayout;
    ttlayout->addWidget(title);
    ttlayout->setMargin(0);
    ttlayout->setSpacing(0);
    ui->widget_list->setLayout(ttlayout);

    ipinput=IPInputbox::Instance();
    anysis=DataAnysis::Instance();
    workpiece=WorkPiece::Instance();
    scanner=scannersetting::Instance();
    Detec=DetecSting::Instance();
    runpage=new Runpage();

    QVBoxLayout *layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(ipinput);
    ui->widget_connectsetting->setLayout(layout);
    ui->pushButton_showset->hide();

    connect(ipinput,SIGNAL(hidesignal()),this,SLOT(hideipsettingwidget()));
    layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(workpiece);
    ui->widget_toolset->setLayout(layout);

    layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(scanner);
    ui->widget_csset->setLayout(layout);

    layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(Detec);
    ui->widget_testset->setLayout(layout);

    layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(runpage);
    ui->widget_runset->setLayout(layout);

    layout=new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(anysis);
    ui->widget_anysis->setLayout(layout);



    timer=new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(updatatime()));
    timer->start(1000);

    QList<int> heights;
    heights<<30<<10;
    ui->splitter_V->setSizes(heights);


    QList<int> widths;
    widths<<60<<1024;
    ui->splitter_H->setSizes(widths);
    connect(workpiece,SIGNAL(definedshape(TopoDS_Shape&)),Detec,SLOT(getshapepara(TopoDS_Shape&)));
    ui->pushButton_zc->hide();
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    QSettings settings("users.ini", QSettings::IniFormat);

    for(int i=0;;i++)
    {
        QString usernum="user"+QString::number(i);
        if(settings.value(usernum)==QVariant::Invalid)
            break;
        QString str= settings.value(usernum).toString();
        QStringList list=str.split("/");
        userlist.push_back(list);
    }
    bool isremeberme=settings.value("isremember").toBool();

    if(isremeberme)
    {
        QString str=settings.value("currentuser").toString();
        QStringList list=str.split("/");
        ui->lineEdit_username->setText(list.first());
        ui->lineEdit_password->setText(list.last());
        ui->checkBox->setChecked(true);
    }else
    {
        ui->checkBox->setChecked(false);
    }
    connect(Detec,SIGNAL(buildtoolcoord()),runpage,SLOT(toolcoordchanged()));
    connect(Detec,SIGNAL(sendpathpoint(QVector<Robot_Path>)),runpage,SLOT(getpathplant(QVector<Robot_Path>)));
    connect(runpage,SIGNAL(robotposition(QVector<float>,int)),anysis,SLOT(showcurrentpath(QVector<float>,int)));
    connect(runpage,SIGNAL(startrunsignal()),anysis,SLOT(clearpoint()));

}

void MainWindow::initmainwidget()
{

}

void MainWindow::backstartpage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::savecurrentuserlist()
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    mlbg->resize(ui->centralwidget->size());
    ui->widget_login->resize(ui->centralwidget->width()/4,ui->centralwidget->height()/4);
    ui->widget_login->setGeometry((ui->centralwidget->width()-ui->widget_login->width())/2,(ui->centralwidget->height()-ui->widget_login->height())/2,
                                  ui->centralwidget->width()/4,ui->centralwidget->height()/4);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}
void MainWindow::on_pushButton_clicked()
{
    /**

      用户登录
    */
    QString user=ui->lineEdit_username->text();
    QString password=ui->lineEdit_password->text();
    bool isok=false;
    foreach(QStringList list,userlist)
    {
        if(list.first()==user||list.last()==password)
        {
            isok=true;
            ui->stackedWidget->setCurrentIndex(1);
            getcurrentindex(1);
            //title->checkstatus(0);
            currentuser=user+"/"+password;
            on_checkBox_clicked();

            break;
        }
    }
    if(!isok)
    {
        QUIWidget::showMessageBoxError("用户名或密码错误");
    }
    if(!ui->pushButton_zc->isHidden())
        ui->pushButton_zc->hide();


}

void MainWindow::updatatime()
{
    QDateTime currenttime=QDateTime::currentDateTime();
    QString tmstr= currenttime.toString("hh:mm:ss");
    ui->label_time->setText(tmstr);
    checkstatus();
}

void MainWindow::getcurrentindex(int index)
{
    switch (index) {
    case 0:
        backstartpage();
        break;
    case 1:
        ui->stackedWidget_sub->setCurrentIndex(0);
        break;
    case 2:
        ui->stackedWidget_sub->setCurrentIndex(1);
        break;
    case 3:
        ui->stackedWidget_sub->setCurrentIndex(2);
        break;
    case 4:
        ui->stackedWidget_sub->setCurrentIndex(3);
        break;
    case 5:
        ui->stackedWidget_sub->setCurrentIndex(4);
        break;
    case 6:
        ui->stackedWidget_sub->setCurrentIndex(5);
        break;
    default:
        break;

    }
    ui->label_subtitle->setText(ttext.at(index));
}

void MainWindow::hideipsettingwidget()
{
    ui->widget_connectsetting->hide();
    ui->pushButton_showset->show();
}

void MainWindow::on_pushButton_showset_clicked()
{
    ui->pushButton_showset->hide();
    ui->widget_connectsetting->show();
}

void MainWindow::on_pushButton_zc_clicked()
{
    QSettings settings("users.ini",QSettings::IniFormat);
    QString username=ui->lineEdit_username->text();
    QString Password=ui->lineEdit_password->text();
    if(username.isEmpty()||Password.isEmpty()) return;
    int p=userlist.size();
    QString str="user"+QString::number(p);
    settings.setValue(str,username+"/"+Password);
    QStringList list;
    list<<username<<Password;
    userlist.push_back(list);
}

void MainWindow::on_lineEdit_password_textChanged(const QString &arg1)
{
    if(arg1=="cirt")
        ui->pushButton_zc->show();
}

void MainWindow::on_checkBox_clicked()
{
    QSettings setting("users.ini",QSettings::IniFormat);
    if(ui->checkBox->isChecked())
    {
         setting.setValue("isremember",true);
    }else
         setting.setValue("isremember",false);
    setting.setValue("currentuser",currentuser);
}

void MainWindow::on_pushButton_saveip_clicked()
{
    ipinput->saveipinfor();
}

void MainWindow::checkstatus()
{
    if(isrobotconnected)
    {
        ui->label_connectlight1->setPixmap(QPixmap(":/image/gree.png"));
        ui->label_connectlight1->setScaledContents(true);
    }else
    {
        ui->label_connectlight1->setPixmap(QPixmap(":/image/redcolor.png"));
        ui->label_connectlight1->setScaledContents(true);
    }
    if(isUIstraconnected)
    {
        ui->label_connectlight2->setPixmap(QPixmap(":/image/gree.png"));
        ui->label_connectlight2->setScaledContents(true);
    }else
    {
        ui->label_connectlight2->setPixmap(QPixmap(":/image/redcolor.png"));
        ui->label_connectlight2->setScaledContents(true);
    }

    if(isrobotconnected && isUIstraconnected)
    {
        title->checkstatus(0);
    }else
    {
        title->uncheckstatus(0);
    }

    if(hasmodelshape)
    {
        title->checkstatus(1);
    }else
        title->uncheckstatus(1);
    if(isUItrasonicsetted)
    {
        title->checkstatus(2);
    }else
        title->uncheckstatus(2);
    if(ispromodelbuild)
    {
        title->checkstatus(3);
    }else
        title->uncheckstatus(3);
    ispathready? title->checkstatus(4):title->uncheckstatus(4);
}

void MainWindow::on_pushButton_robotip_clicked()
{
    if(robserver==nullptr)
    {
        QString ip="192.168.1.20";
        int port;
        QString ip2=ipinput->getip1();
        port=ipinput->getport1().toInt();
        QThread *td=new QThread(this);
        robserver=new TcpServer(this,ip,port);
        robserver->moveToThread(td);
        td->start();
        ui->pushButton_robotip->setText("等待中。。。。。");
        connect(robserver,&TcpServer::newClientInfo,this,&MainWindow::robotconnected);
        connect(robserver,&TcpServer::deleteClient,this,&MainWindow::robotdisconnected);

    }else
    {
        return;
    }
}

void MainWindow::robotconnected()
{
    isrobotconnected=true;
    qDebug()<<"robotconnected()";
    ui->pushButton_robotip->setText("机械臂已连接");
}

void MainWindow::robotdisconnected()
{
    isrobotconnected=false;
    ui->pushButton_robotip->setText("等待中。。。。。");
}

void MainWindow::plantconnected()
{
    isUIstraconnected =true;
    ui->pushButton_toolplantip->setText("超声设备已连接");
}

void MainWindow::plantdisconnected()
{
    isUIstraconnected=false;
    ui->pushButton_toolplantip->setText("等待中。。。。。");
}

void MainWindow::on_pushButton_toolplantip_clicked()
{
    if(robserver==nullptr)
    {
        QString ip="192.168.1.20";
        int port;
        QString ip2=ipinput->getip2();
        port=ipinput->getport2().toInt();
        UIrserver=new TcpServer(this,ip,port);
        QThread *td=new QThread(this);
        UIrserver=new TcpServer(this,ip,port);
        UIrserver->moveToThread(td);
        td->start();
        ui->pushButton_robotip->setText("等待中。。。。。");
        connect(UIrserver,&TcpServer::newClientInfo,this,&MainWindow::plantconnected);
        connect(UIrserver,&TcpServer::deleteClient,this,&MainWindow::plantdisconnected);

    }else
    {
        return;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    runpage->testik();
}


void MainWindow::on_pushButton_Home_clicked()
{
    homesetingpage->show();
}

void MainWindow::on_pushButton_New_clicked()
{
    hasmodelshape=false;
}
