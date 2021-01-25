#include "homeseting.h"
#include "ui_homeseting.h"

Homeseting::Homeseting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Homeseting)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    currenttype=0;
    setWindowTitle("");
    getspacepos();
}

Homeseting::~Homeseting()
{
    delete ui;
}

void Homeseting::on_tabWidget_currentChanged(int index)
{
    currenttype=index;
}

void Homeseting::getspacepos()
{
    Kinematics kmt;
    float jpos[6];
    jpos[0]=ui->lineEdit_j1->text().toFloat();
    jpos[1]=ui->lineEdit_j2->text().toFloat();
    jpos[2]=ui->lineEdit_j3->text().toFloat();
    jpos[3]=ui->lineEdit_j4->text().toFloat();
    jpos[4]=ui->lineEdit_j5->text().toFloat();
    jpos[5]=ui->lineEdit_j6->text().toFloat();
    Transformation T;
    kmt.LBR_fk(jpos,&T);
    SPose pose;
    kmt.T_to_Pose(&T,&pose);
    ui->lineEdit_s1->setText(QString::number(pose.x));
    ui->lineEdit_s2->setText(QString::number(pose.y));
    ui->lineEdit_s3->setText(QString::number(pose.z));
    ui->lineEdit_s4->setText(QString::number(pose.alpha*180/M_PI));
    ui->lineEdit_s5->setText(QString::number(pose.beta*180/M_PI));
    ui->lineEdit_s6->setText(QString::number(pose.gamma*180/M_PI));
    QVector<float> homep;
    homep<<jpos[0]<<jpos[1]<<jpos[2]<<jpos[3]<<jpos[4]<<jpos[5];
    homeposition=homep;
}

void Homeseting::getjointpos()
{
    Kinematics kmt;
    SPose pose;
    float *jpos;
    pose.x=ui->lineEdit_s1->text().toFloat();
    pose.y=ui->lineEdit_s2->text().toFloat();
    pose.z=ui->lineEdit_s3->text().toFloat();
    pose.alpha=ui->lineEdit_s4->text().toFloat();
    pose.beta=ui->lineEdit_s5->text().toFloat();
    pose.gamma=ui->lineEdit_s6->text().toFloat();

    Transformation T;
    QVector<float> pos={0,-90,90,0,0,0};
    kmt.Pose_to_T(&pose,&T);
    unsigned int solutionnum=0;
    float solution[8][6];
    kmt.LBR_ik(&T,pos,&solutionnum,solution);
    jpos=pos.data();

    float jointvalue[6];
    if(kmt.closestq(solution,solutionnum,jpos,jointvalue))
    {
        ui->lineEdit_j1->setText(QString::number(jointvalue[0]));
        ui->lineEdit_j2->setText(QString::number(jointvalue[1]));
        ui->lineEdit_j3->setText(QString::number(jointvalue[2]));
        ui->lineEdit_j4->setText(QString::number(jointvalue[3]));
        ui->lineEdit_j5->setText(QString::number(jointvalue[4]));
        ui->lineEdit_j6->setText(QString::number(jointvalue[5]));
    }
    QVector<float> homep;
    homep<<jointvalue[0]<<jointvalue[1]<<jointvalue[2]<<jointvalue[3]<<jointvalue[4]<<jointvalue[5];
    homeposition=homep;
}


void Homeseting::on_pushButton_ok_clicked()
{
    qDebug()<<currenttype;
    if(currenttype==0)
    {
        getspacepos();
    }else
    {
        getjointpos();
    }

}
