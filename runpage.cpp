#include "runpage.h"
#include "ui_runpage.h"
#include "quiwidget.h"


Runpage::Runpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Runpage)
{
    ui->setupUi(this);
    initForm();
    //initTreeWidget();
    model=new modelview(ui->frame_3D);
    //model->addbase();
    QUIWidget::setFormInCenter(this);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(testmodel()));
    timer->start(100);

    //kinematics=new kr6_arm_kinematics::Kinematics(minJointAngles,maxJointAngles);
    kinematics=new Kinematics();
    //connect(kinematics,SIGNAL(currentcommandsignal()),this,SLOT(on_toolButton_start_clicked()));
    connect(kinematics,SIGNAL(dataforsimulate(QVector<float>,int)),model,SLOT(updatepositon(QVector<float>,int )));
    connect(kinematics,SIGNAL(singularpointinline()),this,SLOT(singuarpoint()));
    //toolcd=new Toolcoord();

}

Runpage::~Runpage()
{
    //model->destroy();
    delete ui;
    delete kinematics;
    //delete toolcd;
}

void Runpage::testik()
{
    //    QVector<float> p={0,-90,90,0,0,140};

    //    model->updatepositon(p);
    //    Kinematics km;
    //    QVector<float> pos;
    //    km.fkcall2(p,pos);
    //    //qDebug()<<"*************************";
    //    //={185.608, -7.61738, 1064.02, -128.708, -19.3669, -158.689};
    //    QVector<float> jointpos;
    //    km.ikcals(p,pos,jointpos);
    //    JointValues joint;
    //    for(int i=0;i<DOF;++i)
    //    {
    //        joint(i)=p.at(i)*M_PI/180;
    //    }
    //    Pose pos;
    //    //Vector3d pos= kinematics->FK(joint);
    //    kinematics->FK(joint,pos);

    //    qDebug()<<pos.position(0)<<pos.position(1)<<pos.position(2)<<pos.orientation(0)<<pos.orientation(1)<<pos.orientation(2);
    //    std::vector<JointValues> sloutions;
    //    int resut=kinematics->getAllIKSolutions(pos,sloutions);
    //    if(resut==2)
    //    {
    //        qDebug()<<"no sloutions fond";
    //    }
    //    JointValues sol;
    //    for(int i=0;i < sloutions.size();++i)
    //    {
    //        sol=sloutions.at(i);
    //        qDebug()<<"angle is"<<sol(0)<<sol(1)<<sol(2)<<sol(3)<<sol(4)<<sol(5);

    //    }

}

void Runpage::initForm()
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
    ui->label_text->setText("检测执行");
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

void Runpage::initTreeWidget()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("程序列表");
    QTreeWidgetItem *group1=new QTreeWidgetItem(ui->treeWidget);
    group1->setText(0,"程序1");
    group1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    group1->setCheckState(0,Qt::Checked);
    QTreeWidgetItem *subitem11=new QTreeWidgetItem(group1);
    subitem11->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subitem11->setText(0,"函数1");
    subitem11->setCheckState(0,Qt::Checked);
    QTreeWidgetItem *subitem12=new QTreeWidgetItem(group1);
    subitem12->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subitem12->setText(0,"函数2");
    subitem12->setCheckState(0,Qt::Checked);

    QTreeWidgetItem *group2=new QTreeWidgetItem(ui->treeWidget);
    group2->setText(0,"程序2");
    group2->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    group2->setCheckState(0,Qt::Unchecked);
    QTreeWidgetItem *subitem21=new QTreeWidgetItem(group2);
    subitem21->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subitem21->setText(0,"函数1");
    subitem21->setCheckState(0,Qt::Unchecked);
    QTreeWidgetItem *subitem22=new QTreeWidgetItem(group2);
    subitem22->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subitem22->setText(0,"函数2");
    subitem22->setCheckState(0,Qt::Unchecked);
}

void Runpage::initMenu()
{

}

void Runpage::doMenu()
{

}

void Runpage::on_pushButton_min_clicked()
{
    showMinimized();
}

void Runpage::on_pushButton_max_clicked()
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

void Runpage::on_pushButton_close_clicked()
{
    close();
}

void Runpage::init3d()
{

}

void Runpage::showrobot()
{

}

void Runpage::testmodel()
{
    currentposition = kinematics->getcurrentposition();
    //currentposition[1]-=90;
    //currentposition[2]+=90;

    updatajointdata(currentposition);
    int p=kinematics->getflags();
    emit robotposition(currentposition,p);

}

void Runpage::toolcoordchanged()
{
    //QMatrix4x4 matrix = toolcd->getsubcoord(orgpoint,toolx,toolplant);
    //model->plantcoordchanged(matrix);

}

void Runpage::getpathplant(QVector<Robot_Path> path)
{
    QVector<float> currentjoint=currentposition;
    pathvec.clear();

    for(int i=0;i<path.size();++i)
    {
        Robot_Path ph=path.at(i);
        jointvecformpath(currentjoint,ph);
        currentjoint=ph.joint.last();
    }
    gettreePros();
    ispromodelbuild=true;
}

void Runpage::gettreePros()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("程序列表");
    QTreeWidgetItem *group1=new QTreeWidgetItem(ui->treeWidget);
    group1->setText(0,"basicshape");
    group1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    group1->setCheckState(0,Qt::Unchecked);
    QString str=pathvec.first().type;
    if(str=="face")
    {
        QTreeWidgetItem *subitem11=new QTreeWidgetItem(group1);
        subitem11->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        subitem11->setText(0,str);
        subitem11->setCheckState(0,Qt::Unchecked);
    }else
    {
        for(int i=0;i<pathvec.size();++i)
        {
            QString type=pathvec.at(i).type;
            QTreeWidgetItem *subitem11=new QTreeWidgetItem(group1);
            subitem11->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            subitem11->setText(0,QString::number(i)+"_"+type);
            subitem11->setCheckState(0,Qt::Unchecked);
        }
    }
}

void Runpage::singuarpoint()
{
    QMessageBox::warning(this,"warning","路径上存在奇异点，无法达到！");
}
void Runpage::updatajointdata(QVector<float> dts)
{
    if(dts.size() != 6) return;
    QVector<float> dt=dts;
    ui->label_joint1->setText(QString::number(dt.at(0),'f',3));
    ui->label_joint2->setText(QString::number(dt.at(1),'f',3));
    ui->label_joint3->setText(QString::number(dt.at(2),'f',3));
    ui->label_joint4->setText(QString::number(dt.at(3),'f',3));
    ui->label_joint5->setText(QString::number(dt.at(4),'f',3));
    ui->label_joint6->setText(QString::number(dt.at(5),'f',3));
    float sppos[6];
    for(int i=0;i<DOF;++i)
    {
        sppos[i]=dt.at(i);
    }
    Transformation T;
    kinematics->LBR_fk(sppos,&T);
    SPose pose;
    kinematics->T_to_Pose(&T,&pose);
    ui->label_spx->setText(QString::number(pose.x,'f',3));
    ui->label_spy->setText(QString::number(pose.y,'f',3));
    ui->label_spz->setText(QString::number(pose.z,'f',3));
    ui->label_sprx->setText(QString::number(pose.alpha*180/M_PI,'f',3));
    ui->label_spry->setText(QString::number(pose.beta*180/M_PI,'f',3));
    ui->label_sprz->setText(QString::number(pose.gamma*180/M_PI,'f',3));

    QVector3D toolp=subcoordpoint(QVector3D(pose.x,pose.y,pose.z));
    ui->label_tx->setText(QString::number(toolp.x(),'f',3));
    ui->label_ty->setText(QString::number(toolp.y(),'f',3));
    ui->label_tz->setText(QString::number(toolp.z(),'f',3));
}

void Runpage::checkprogramlist()
{
    int index=0;
    QTreeWidgetItemIterator it(ui->treeWidget);
    if(pathvec.first().type=="face")
    {
        if((*it)->child(0)->checkState(0)==Qt::Checked)
        {
            for(int i=0;i<pathvec.size();++i)
            {
                pathvec[i].runflag=true;
            }
            ispathready=true;
        }else
        {
            for(int i=0;i<pathvec.size();++i)
            {
                pathvec[i].runflag=false;
            }
        }

    }else
    {

        while (*it) {
            if((*it)->childCount()>0)
            {
                for(int i=0;i<(*it)->childCount();++i)
                {
                    int itype=(*it)->child(i)->type();
                    //qDebug()<<itype;
                    switch(itype)
                    {
                    case 0:
                    case 1:
                    {
                        //qDebug()<<(*it)->child(i)->checkState(0);
                        if((*it)->child(i)->checkState(0)==Qt::Checked)
                        {
                            pathvec[index].runflag=true;
                            ispathready=true;
                        }else
                        {
                            pathvec[index].runflag=false;
                        }
                        index++;
                    }
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    }
                }
                it+=(*it)->childCount();
            }
            ++it;
        }
    }
}

QVector3D Runpage::subcoordpoint(const QVector3D &point)
{
    QVector4D po1=QVector4D(point,1);
    QMatrix4x4 omatrix;
    omatrix.setToIdentity();
    omatrix(0,0)=toolx.x();
    omatrix(1,0)=toolx.y();
    omatrix(2,0)=toolx.z();
    omatrix(0,1)=tooly.x();
    omatrix(1,1)=tooly.y();
    omatrix(2,1)=tooly.z();
    omatrix(0,2)=toolz.x();
    omatrix(1,2)=toolz.y();
    omatrix(2,2)=toolz.z();
    omatrix(0,3)=orgpoint.x();
    omatrix(1,3)=orgpoint.y();
    omatrix(2,3)=orgpoint.z();
    QMatrix4x4 invm=omatrix.inverted();
    QVector4D pt=invm*po1;
    return QVector3D(pt.x(),pt.y(),pt.z());
}

void Runpage::printpath()
{
    //qDebug()<<"Runpage::printpath";
    for(int i=0;i<pathvec.size();i++)
    {
        Robot_Path path=pathvec.at(i);
        for(int j=0;j<path.joint.size();++j)
        {
            qDebug()<<path.joint.at(j);
        }
    }
}

void Runpage::jointvecformpath(const QVector<float> currentjoint,Robot_Path &path)
{
    QVector<float> currentjp=currentjoint;
    path.joint.clear();
    for(int i=0;i<path.point.size();++i)
    {
        QVector<float> jp;
        //qDebug()<<"Runpage::getpathplant"<<currentjoint;
        float sloution[8][6];
        unsigned int sloutionnum=0;
        SPose pose;
        pose.x=path.point.at(i).x();
        pose.y=path.point.at(i).y();
        pose.z=path.point.at(i).z();
        pose.alpha=path.norml.at(i).x();
        pose.beta=path.norml.at(i).y();
        pose.gamma=path.norml.at(i).z();
        Transformation T;
        kinematics->Pose_to_T(&pose,&T);
        kinematics->LBR_ik(&T,currentjp,&sloutionnum,sloution);
        if(sloutionnum>0)
        {
            if(kinematics->closestq(sloution,sloutionnum,currentjp,jp))
            {
                //qDebug()<<jp;
                path.joint.push_back(jp);
                currentjp=jp;
            }else
            {
                QMessageBox::warning(this,"warning",QString("path error,point %1 can not reach").arg(i));
                qDebug()<<"Runpage::getpathplant"<<path.point.at(i)<<path.norml.at(i);
                return;
            }

        }else
        {
            QMessageBox::warning(this,"warning",QString("path error,point %1 has no LBR_IK solution").arg(i));
            return;
        }
    }
    pathvec.push_back(path);
}

void Runpage::on_toolButton_start_clicked()
{
    checkprogramlist();
    kinematics->getrobotpath(pathvec);
    emit startrunsignal();
}

void Runpage::on_toolButton_sigle_clicked()
{

}

void Runpage::on_checkBox_showmodel_clicked()
{
    if(ui->checkBox_showmodel->isChecked())
        model->makeabox();
    else
        model->removemodel();
}

void Runpage::on_toolButton_start_triggered(QAction *arg1)
{

}

void Runpage::on_toolButton_stop_clicked()
{
    kinematics->stoprun();
    model->getendposition();
}

void Runpage::on_toolButton_suspend_clicked()
{
    QMatrix4x4 mat;
    mat(0,3)=365;
    mat(1,3)=311;
    mat(2,3)=311;

}
