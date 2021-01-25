#include "kinematics_interface.h"
#include <QDebug>
#include <QThread>

KInematics_Interface::KInematics_Interface()
{
    //paramet=paramete.toStdVector();
}

KInematics_Interface::~KInematics_Interface()
{
}

//void KInematics_Interface::updataparamete(QStringList paramelist)
//{
//    QString str;
//    QStringList list;
//    for(int i=0;i<paramelist.size();++i)
//    {
//        str=paramelist.at(i);
//        if(str.startsWith("S_paramet"))
//        {
//            list=str.split(" ");
//            break;
//        }
//    }
//    if(list.size()<6) return;

//    paramet.clear();
//    for(int j=1;j<list.size();++j)
//    {
//        str=list.at(j);
//        paramet.push_back(str.toFloat());
//    }
//}


void KInematics_Interface::fKCal(const QVector<float> &CurrentJointsPosition, float *ToolPose)
{
    float q1 = CurrentJointsPosition.at(0)*DEG2RAD;
    float q2 = CurrentJointsPosition.at(1)*DEG2RAD;
    float q3 = CurrentJointsPosition.at(2)*DEG2RAD;
    float q4 = CurrentJointsPosition.at(3)*DEG2RAD;
    float q5 = CurrentJointsPosition.at(4)*DEG2RAD;
    float q6 = CurrentJointsPosition.at(5)*DEG2RAD;
    std::vector<float> Q={q1,q2,q3,q4,q5,q6};
    CIRT_LBR::Transform T = CIRT_LBR::LBR_fk(Q);
    std::vector<float> rpy = T.RPY();
        *ToolPose=T.px*1000;
        *(ToolPose+1)=T.py*1000;
        *(ToolPose+2)=T.pz*1000;
        *(ToolPose+3)=rpy[0];
        *(ToolPose+4)=rpy[1];
        *(ToolPose+5)=rpy[2];
}

void KInematics_Interface::IKCal(const QVector<float> &CurrentJointsPosition, const QVector<float> &TargetPose)
{
    std::vector<float> poses;
    poses=CIRT_LBR::trans(TargetPose.toStdVector());
    qDebug()<<"currentjointposition"<<CurrentJointsPosition.at(0)<<CurrentJointsPosition.at(1)<<CurrentJointsPosition.at(2);
    float nx = poses.at(0);float ny = poses.at(4);float nz = poses.at(8);
    float ox = poses.at(1);float oy = poses.at(5);float oz = poses.at(9);
    float ax = poses.at(2);float ay = poses.at(6);float az = poses.at(10);
    float px = poses.at(3)/1000.0;float py = poses.at(7)/1000.0;float pz = poses.at(11)/1000.0;
    CIRT_LBR::Transform T(nx,ny,nz,ox,oy,oz,ax,ay,az,px,py,pz);
    std::vector<std::vector<float> > Q_set=CIRT_LBR::LBR_ik(T);
    bool isSolutionNotFound=true;
    QVector<float> slt;
    float stddist=100;
    if(Q_set.size()>0)
    {
        for(unsigned int j=0;j<Q_set.size();++j)
        {
            float dist=CIRT_LBR::distance(CurrentJointsPosition,Q_set.at(j));
            if(dist < stddist)
            {
                slt.clear();
                for(unsigned int i=0;i<Q_set.at(j).size();++i)
                {
                    slt.push_back(Q_set[j][i]);
                }
                stddist=dist;
                isSolutionNotFound=false;
                break;
            }
        }
    }else
        isSolutionNotFound=true;


    emit IKResults(isSolutionNotFound,slt);
}
