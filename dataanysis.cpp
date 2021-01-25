#include "dataanysis.h"
#include "ui_dataanysis.h"

DataAnysis *DataAnysis::self=0;

DataAnysis::DataAnysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataAnysis)
{
    ui->setupUi(this);
    this->setWindowTitle("数据分析");
    initform();
    init3D();

}

DataAnysis::~DataAnysis()
{
    delete ui;
}

void DataAnysis::on_pushButton_min_clicked()
{
    showMinimized();
}

void DataAnysis::on_pushButton_max_clicked()
{
    if(max){
        this->setGeometry(location);
        this->setProperty("canmove",true);
    }else{
        location=this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        this->setProperty("canmove",false);
    }
    max=!max;
}

void DataAnysis::on_pushButton_close_clicked()
{
    close();
}

void DataAnysis::showcurrentpath(QVector<float> path,int index)
{
    if(index/10==0)
        return;
    else
    {
        QVector<float> p;
        Kinematics km;
        km.fkcall2(path,p);
        p.push_back(index);
        if(!ui->radioButton_pathshow->isChecked())
        {
            point.push_back(p);
        }
        else
            showpath();
    }
}

void DataAnysis::getballposition()
{
    int pt=occview->selectshapeposition();
    if(pt==-1)
    {
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
    }else
    {
        ui->lineEdit->setText(QString::number(point.at(pt).at(0)));
        ui->lineEdit_2->setText(QString::number(point.at(pt).at(1)));
        ui->lineEdit_3->setText(QString::number(point.at(pt).at(2)));
    }
}

void DataAnysis::clearpoint()
{
    point.clear();
    count=0;
}

void DataAnysis::initform()
{
    this->max=false;
    this->location=this->geometry();
    this->setProperty("form",true);
    this->setProperty("canmove",true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    IconHelper::Instance()->setIcon(ui->label_icon,QChar(0xF099),30);
    IconHelper::Instance()->setIcon(ui->pushButton_min,QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->pushButton_max,QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->pushButton_close,QChar(0xf00d));
    ui->widgetTitle->setProperty("form","title");
    ui->widgettop->setProperty("nav","top");
    ui->label_text->setText("数据分析");
    ui->label_text->setFont(QFont("Microsoft YaHei",20));
    this->setWindowTitle(ui->label_text->text());
    QSize iconsize(32,32);
    int icowidth=85;
    QList<QToolButton*> tbtns=ui->widgettop->findChildren<QToolButton* >();
    foreach(QToolButton *btn,tbtns)
    {
        btn->setIconSize(iconsize);
        btn->setMinimumWidth(icowidth);
    }

    QList<int> widths;
    widths<<170<<650;
    ui->splitterH->setSizes(widths);
    QList<int> heights;
    heights<<500<<100;
    ui->splitterV->setSizes(heights);
    ui->pushButton_max->hide();
    ui->pushButton_min->hide();
    ui->pushButton_close->hide();

}

void DataAnysis::init3D()
{
    //myview=new OccView(this);
    occview=new myview(this);
    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(occview);
    ui->frame_3D->setLayout(layout);
    connect(occview,SIGNAL(selectionChanged()),this,SLOT(getballposition()));

}

void DataAnysis::showpath()
{
    occview->showgrounds(true);
    for(int i=count;i<point.size();++i)
    {
        QVector<float> path=point.at(i);
        gp_Pnt center=gp_Pnt(path.at(0),path.at(1),path.at(2));
        TopoDS_Shape ball=BRepPrimAPI_MakeSphere(center,toolradius).Shape();
        float index=path.last();
        int p=(int)index%10;
        if(occview)
            occview->addshapetocontent(ball,p);
        count++;
    }
}

void DataAnysis::mousePressEvent(QMouseEvent *event)
{
    if(max)
    {
        return;
    }else
    {
        ispress=true;
        startpos=event->globalPos();
    }
    return QWidget::mousePressEvent(event);
}

void DataAnysis::mouseMoveEvent(QMouseEvent *event)
{
    if(ispress)
    {
        QPoint movepoint=event->globalPos()-startpos;
        QPoint wdpos=this->pos();
        startpos=event->globalPos();
        //this->move(wdpos.x()+movepoint.x(),wdpos.y()+movepoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void DataAnysis::mouseReleaseEvent(QMouseEvent *event)
{
    ispress=false;
    return QWidget::mouseReleaseEvent(event);
}

void DataAnysis::on_pushButton_clicked()
{
    //occview->showgrounds(true);
    if(point.size()==0) return;
    QString x,y,z;
    x=ui->lineEdit->text();
    y=ui->lineEdit_2->text();
    z=ui->lineEdit_3->text();
    
    if(x.isEmpty()||y.isEmpty()||z.isEmpty())
    {
        QMessageBox::warning(this,"warning","input data error!");
        return;
    }
    float dist=10000;
    gp_Pnt p=gp_Pnt(x.toFloat(),y.toFloat(),z.toFloat());
    int index=-1;
    QVector<float> nearstpoint;
    for(int i=0;i<point.size();++i)
    {
        gp_Pnt pend=gp_Pnt(point.at(i).at(0),point.at(i).at(1),point.at(i).at(2));
        if(p.Distance(pend)<dist)
        {
            dist=p.Distance(pend);
            index=i;
        }
    }
    if(index==(point.size()-1))
    {
        nearstpoint=point.at(index);
    }
    else
    {
        gp_Pnt ps=gp_Pnt(point.at(index).at(0),point.at(index).at(1),point.at(index).at(2));
        gp_Pnt pe=gp_Pnt(point.at(index+1).at(0),point.at(index+1).at(1),point.at(index+1).at(2));
        
        float persend=p.Distance(ps)/ps.Distance(pe);
        nearstpoint.clear();
        nearstpoint.push_back(x.toFloat());
        nearstpoint.push_back(y.toFloat());
        nearstpoint.push_back(z.toFloat());
        nearstpoint.push_back(point.at(index).at(3)+(point.at(index+1).at(3)-point.at(index).at(3))*persend);
        nearstpoint.push_back(point.at(index).at(4)+(point.at(index+1).at(4)-point.at(index).at(4))*persend);
        nearstpoint.push_back(point.at(index).at(5)+(point.at(index+1).at(5)-point.at(index).at(5))*persend);
    }
    ui->textEdit->append(QString("robot position is: %1,%2,%3,%4,%5,%6").arg(nearstpoint.at(0)).arg(nearstpoint.at(1)).arg(nearstpoint.at(2)).arg(nearstpoint.at(3)).arg(nearstpoint.at(4)).arg(nearstpoint.at(5)));

}

void DataAnysis::on_pushButton_2_clicked()
{
    //occview->showgrounds(false);
    ui->lineEdit->setText(QString::number(orgpoint.x()));
    ui->lineEdit_2->setText(QString::number(orgpoint.y()));
    ui->lineEdit_3->setText(QString::number(orgpoint.z()));
}

void DataAnysis::on_toolButton_clear_clicked()
{
    clearpoint();
    occview->reset();
}


void DataAnysis::on_toolButton_start_clicked()
{
//    occview->showgrounds(true);
//    QVector<float> path={100,212,34};
//    showcurrentpath(path, 0);
//    QVector<float> path2={200,112,134};
//    showcurrentpath(path2, 1);
//    QVector<float> path3={20,11,23};
//    showcurrentpath(path3, 2);
}
