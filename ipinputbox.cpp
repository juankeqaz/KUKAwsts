#include "ipinputbox.h"

IPInputbox *IPInputbox::self=0;

IPInputbox::IPInputbox(QWidget *parent):QDialog(parent)
{
    this->initControl();
    this->initForm();
}

IPInputbox::~IPInputbox()
{
    delete widgetMain;
}

void IPInputbox::closeEvent(QCloseEvent *)
{
    closeSec = 0;
    currentSec = 0;
}

bool IPInputbox::eventFilter(QObject *obj, QEvent *evt)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - this->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            //this->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void IPInputbox::initControl()
{
    this->setObjectName(QString::fromUtf8("QUIInputBox"));

//#ifdef __arm__
//    this->resize(300, 160);
//#else
    this->resize(280, 150);
//#endif

    verticalLayout1 = new QVBoxLayout(this);
    verticalLayout1->setSpacing(0);
    verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
    verticalLayout1->setContentsMargins(1, 1, 1, 1);
    widgetTitle = new QWidget(this);
    widgetTitle->setObjectName(QString::fromUtf8("widgetTitle"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);//
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(widgetTitle->sizePolicy().hasHeightForWidth());
    widgetTitle->setSizePolicy(sizePolicy);
    widgetTitle->setMinimumSize(QSize(0, 30));
    horizontalLayout1 = new QHBoxLayout(widgetTitle);
    horizontalLayout1->setSpacing(0);
    horizontalLayout1->setObjectName(QString::fromUtf8("horizontalLayout1"));
    horizontalLayout1->setContentsMargins(0, 0, 0, 0);
    labIco = new QLabel(widgetTitle);
    labIco->setObjectName(QString::fromUtf8("labIco"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(labIco->sizePolicy().hasHeightForWidth());
    labIco->setSizePolicy(sizePolicy1);
    labIco->setMinimumSize(QSize(30, 0));
    labIco->setAlignment(Qt::AlignCenter);

    horizontalLayout1->addWidget(labIco);

    labTitle = new QLabel(widgetTitle);
    labTitle->setObjectName(QString::fromUtf8("labTitle"));
    labTitle->setStyleSheet(QString::fromUtf8(""));
    labTitle->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

    horizontalLayout1->addWidget(labTitle);

    labTime = new QLabel(widgetTitle);
    labTime->setObjectName(QString::fromUtf8("labTime"));
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(labTime->sizePolicy().hasHeightForWidth());
    labTime->setSizePolicy(sizePolicy2);
    labTime->setAlignment(Qt::AlignCenter);

    horizontalLayout1->addWidget(labTime);

    widgetMenu = new QWidget(widgetTitle);
    widgetMenu->setObjectName(QString::fromUtf8("widgetMenu"));
    sizePolicy1.setHeightForWidth(widgetMenu->sizePolicy().hasHeightForWidth());
    widgetMenu->setSizePolicy(sizePolicy1);
    horizontalLayout2 = new QHBoxLayout(widgetMenu);
    horizontalLayout2->setSpacing(0);
    horizontalLayout2->setObjectName(QString::fromUtf8("horizontalLayout2"));
    horizontalLayout2->setContentsMargins(0, 0, 0, 0);
    btnMenu_Close = new QPushButton(widgetMenu);
    btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
    QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
    btnMenu_Close->setSizePolicy(sizePolicy3);
    btnMenu_Close->setMinimumSize(QSize(30, 0));
    btnMenu_Close->setMaximumSize(QSize(30, 16777215));
    btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
    btnMenu_Close->setFocusPolicy(Qt::NoFocus);
    btnMenu_Close->setFlat(true);

    horizontalLayout2->addWidget(btnMenu_Close);
    horizontalLayout1->addWidget(widgetMenu);
    verticalLayout1->addWidget(widgetTitle);

    widgetMain = new QWidget(this);
    widgetMain->setObjectName(QString::fromUtf8("widgetMain"));
    widgetMain->setStyleSheet(QString::fromUtf8(""));
    verticalLayout2 = new QVBoxLayout(widgetMain);
    verticalLayout2->setSpacing(5);
    verticalLayout2->setObjectName(QString::fromUtf8("verticalLayout2"));
    verticalLayout2->setContentsMargins(5, 5, 5, 5);
    frame = new QFrame(widgetMain);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Sunken);
    verticalLayout3 = new QVBoxLayout(frame);
    verticalLayout3->setObjectName(QString::fromUtf8("verticalLayout3"));
    labInfo = new QLabel(frame);
    labInfo->setObjectName(QString::fromUtf8("labInfo"));
    labInfo->setScaledContents(false);
    labInfo->setWordWrap(true);
    labInfo->setText("机械臂IP设置：");
    verticalLayout3->addWidget(labInfo);

    QWidget *ipinput=new QWidget(frame);
    txtValue = new QLineEdit(ipinput);
    txtValue->setObjectName(QString::fromUtf8("txtValue"));
    QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    txtValue->setValidator(new QRegExpValidator(rx));
    //txtValue->setInputMask(QString("000.000.000.000;0"));
    QLabel *iplabel=new QLabel(ipinput);
    iplabel->setText("IP:");
    QHBoxLayout *iplayout= new QHBoxLayout();
    iplayout->addWidget(iplabel);
    iplayout->addWidget(txtValue);
    ipinput->setLayout(iplayout);
    verticalLayout3->addWidget(ipinput);

    QWidget *portinput=new QWidget(frame);
    portValue = new QLineEdit(portinput);
    portValue->setObjectName(QString::fromUtf8("portValue"));
    QRegExp portreg("[0-9]+$");
    portValue->setValidator(new QRegExpValidator(portreg));
    QLabel *portlabel=new QLabel();
    portlabel->setText("PORT:");
    QHBoxLayout *portlayout=new QHBoxLayout();
    portlayout->addWidget(portlabel);
    portlayout->addWidget(portValue);
    portinput->setLayout(portlayout);
    verticalLayout3->addWidget(portinput);

    QLabel *lableinfo2=new QLabel(frame);
    lableinfo2->setObjectName(QString::fromUtf8("labInfo2"));
    lableinfo2->setScaledContents(false);
    lableinfo2->setWordWrap(true);
    lableinfo2->setText("超声控制IP设置：");
    verticalLayout3->addWidget(lableinfo2);

    QWidget *ipinput2=new QWidget(frame);
    txtValue2 = new QLineEdit(ipinput2);
    txtValue2->setObjectName(QString::fromUtf8("txtValue2"));
    txtValue2->setValidator(new QRegExpValidator(rx));
    //txtValue2->setInputMask(QString("000.000.000.000;0"));

    QLabel *iplabel2=new QLabel(ipinput2);
    iplabel2->setText("IP:");
    QHBoxLayout *iplayout2= new QHBoxLayout();
    iplayout2->addWidget(iplabel2);
    iplayout2->addWidget(txtValue2);
    ipinput2->setLayout(iplayout2);
    verticalLayout3->addWidget(ipinput2);

    QWidget *portinput2=new QWidget(frame);
    portValue2 = new QLineEdit(portinput);
    portValue2->setObjectName(QString::fromUtf8("portValue"));
    portValue2->setValidator(new QRegExpValidator(portreg));
    QLabel *portlabel2=new QLabel();
    portlabel2->setText("PORT:");
    QHBoxLayout *portlayout2=new QHBoxLayout();
    portlayout2->addWidget(portlabel2);
    portlayout2->addWidget(portValue2);
    portinput2->setLayout(portlayout2);
    verticalLayout3->addWidget(portinput2);


    cboxValue = new QComboBox();
    //cboxValue->setObjectName(QString::fromUtf8("cboxValue"));

//    verticalLayout3->addWidget(cboxValue);

    lay = new QHBoxLayout();
    lay->setObjectName(QString::fromUtf8("lay"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    lay->addItem(horizontalSpacer);

    btnOk = new QPushButton(frame);
    btnOk->setObjectName(QString::fromUtf8("btnOk"));
    btnOk->setMinimumSize(QSize(80, 0));
    btnOk->setStyleSheet(QString::fromUtf8(""));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/image/btn_ok.png"), QSize(), QIcon::Normal, QIcon::Off);
    btnOk->setIcon(icon);

    lay->addWidget(btnOk);

    btnCancel = new QPushButton(frame);
    btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
    btnCancel->setMinimumSize(QSize(80, 0));
    btnCancel->setStyleSheet(QString::fromUtf8(""));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/image/btn_close.png"), QSize(), QIcon::Normal, QIcon::Off);
    btnCancel->setIcon(icon1);

    lay->addWidget(btnCancel);
    verticalLayout3->addLayout(lay);
    verticalLayout2->addWidget(frame);
    verticalLayout1->addWidget(widgetMain);

    setTabOrder(txtValue, btnOk);
    setTabOrder(btnOk, btnCancel);
    this->labTitle->setText("连接设置");
    this->btnOk->setText("确定");
    this->btnCancel->setText("取消");

    connect(this->btnOk, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
    connect(this->btnMenu_Close, SIGNAL(clicked(bool)), this, SLOT(on_btnMenu_Close_clicked()));
    connect(this->btnCancel, SIGNAL(clicked()), this, SLOT(on_btnMenu_Close_clicked()));
}

void IPInputbox::initForm()
{
    IconHelper::Instance()->setIcon(labIco, QUIConfig::IconMain, QUIConfig::FontSize + 2);
    IconHelper::Instance()->setIcon(btnMenu_Close, 0xf054, QUIConfig::FontSize);

    this->setProperty("form", true);
    this->widgetTitle->setProperty("form", "title");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);
    this->resize(280, 300);
    this->setWindowTitle(this->labTitle->text());

    int width = 80;
    int iconWidth = 18;
    int iconHeight = 18;

    QList<QPushButton *> btns  = this->frame->findChildren<QPushButton *>();

    foreach (QPushButton *btn, btns) {
        btn->setMinimumWidth(width);
        btn->setIconSize(QSize(iconWidth, iconHeight));
    }

    closeSec = 0;
    currentSec = 0;

    QTimer *timer = new QTimer(this);
    //timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkSec()));
    //timer->start();

    this->installEventFilter(this);

    QSettings settings("connectsetting.ini",QSettings::IniFormat);
    bool isfoever=settings.value("isforever").toBool();
    if(isfoever)
    {
        txtValue->setText(settings.value("robot_ip").toString());
        txtValue2->setText(settings.value("UItrasonic_control_ip").toString());
        portValue->setText(settings.value("robot_port").toString());
        portValue2->setText(settings.value("UItrasonic_control_port").toString());
    }
}

void IPInputbox::checkSec()
{
    if (closeSec == 0) {
        return;
    }

    if (currentSec < closeSec) {
        currentSec++;
    } else {
        //this->close();
        return;
    }

    QString str = QString("关闭倒计时 %1 s").arg(closeSec - currentSec + 1);
    this->labTime->setText(str);
}

void IPInputbox::saveipinfor()
{
    QString ip1=txtValue->text();
    QString port1=portValue->text();
    QString ip2=txtValue2->text();
    QString port2=portValue2->text();
    qDebug()<<"current ip is"<<ip1<<port1<<ip2<<port2;
    QSettings settings("connectsetting.ini",QSettings::IniFormat);
    settings.setValue("robot_ip",ip1);
    settings.setValue("robot_port",port1);
    settings.setValue("UItrasonic_control_ip",ip2);
    settings.setValue("UItrasonic_control_port",port2);
    settings.setValue("isforever",true);
}

QString IPInputbox::getip1()
{
    return txtValue->text();
}

QString IPInputbox::getip2()
{
    return txtValue2->text();
}

QString IPInputbox::getport1()
{
    return  portValue->text();
}

QString IPInputbox::getport2()
{
    return portValue2->text();
}

void IPInputbox::on_btnOk_clicked()
{
    QSettings settings("connectsetting.ini",QSettings::IniFormat);
    settings.setValue("isforever",false);
    QString ip1=txtValue->text();
    QString port1=portValue->text();
    QString ip2=txtValue2->text();
    QString port2=portValue2->text();
    settings.setValue("robot_ip",ip1);
    settings.setValue("robot_port",port1);
    settings.setValue("UItrasonic_control_ip",ip2);
    settings.setValue("UItrasonic_control_port",port2);

    //close();
}

void IPInputbox::on_btnMenu_Close_clicked()
{
    //close();
    emit hidesignal();
}

QString IPInputbox::getValue() const
{
    return this->value;
}

void IPInputbox::setIconMain(QChar str, quint32 size)
{
    IconHelper::Instance()->setIcon(this->labIco, str, size);
}

void IPInputbox::setParameter(const QString &title, int type, int closeSec, QString defaultValue, bool pwd)
{
    this->closeSec = closeSec;
    this->currentSec = 0;
    this->labTime->clear();
    this->labInfo->setText(title);

    checkSec();

    if (type == 0) {
        this->cboxValue->setVisible(false);
        this->txtValue->setPlaceholderText(defaultValue);

        if (pwd) {
            this->txtValue->setEchoMode(QLineEdit::Password);
        }
    } else if (type == 1) {
        this->txtValue->setVisible(false);
        this->cboxValue->addItems(defaultValue.split("|"));
    }
}
