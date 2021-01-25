#include "kinematics.h"
#include <iostream>

Kinematics::Kinematics()
{
    DH_parame<<227.0<<173.0<<455.0<<25.0<<25.0<<240.0<<205.0<<72.0;
    toollength=18;
    a1=25;
    d1=400;
    a2=455;
    a3=25;
    d4=420;
    d6=90;
    parame<<0<<-90<<90<<0<<0<<0;
    simulationTimer = new QTimer(this);
    time_step = 0.01;
    speedrate=1;
    
    currenttimepoint = 0;
    connect(simulationTimer,SIGNAL(timeout()),this,SLOT(sendVirtualJointPosition()));
    
    line= new LineGeometry;
    orientation=new OrientationGeometry;
    linetrajp=new STrajectoryParameter;
    
    circlearc=new CircleArcGeometry;
    arc_orientation=new OrientationGeometry;
    circlearctrajp=new STrajectoryParameter;
    
}

Kinematics::~Kinematics()
{
    
}

void Kinematics::fkcall(QVector<float> jointvalue, QVector<float> &mrpy)
{
    if(jointvalue.size()<JOINT_NUM)
        return;
    float a1,a2,a3,a4,d1,d4,d5,d6;
    a1=DH_parame.at(0);//227
    a2=DH_parame.at(1);//173
    a3=DH_parame.at(2);//455
    a4=DH_parame.at(3);//25
    d1=DH_parame.at(4);//25
    
    d4=DH_parame.at(5);//240
    d5=DH_parame.at(6);//205
    d6=DH_parame.at(7);//72
    
    QMatrix4x4 joint1,joint2,joint3,joint4,joint5,joint6;
    joint1.setToIdentity();
    joint2.setToIdentity();
    joint3.setToIdentity();
    joint4.setToIdentity();
    joint5.setToIdentity();
    joint6.setToIdentity();
    
    joint1.translate(QVector3D(0,0,a1));
    joint1.rotate(jointvalue.at(0),QVector3D(0,0,1));
    //joint1.translate(QVector3D(0,0,a2));
    
    joint2.translate(QVector3D(d1,0,a2));
    joint2.rotate(-90,QVector3D(1,0,0));
    joint2.rotate(jointvalue.at(1)-90,QVector3D(0,0,1));
    //joint2.rotate(jointvalue.at(1),QVector3D(0,1,0));
    
    joint3.translate(QVector3D(a3,0,0));
    //joint3.rotate(90,QVector3D(1,0,0));
    //joint3.rotate(jointvalue.at(2)-90,QVector3D(0,0,1));
    joint3.rotate(jointvalue.at(2),QVector3D(0,0,1));
    
    joint4.translate(QVector3D(a4,d4,0));
    joint4.rotate(-90,QVector3D(1,0,0));
    //joint4.rotate(-90,QVector3D(0,0,1));
    joint4.rotate(jointvalue.at(3),QVector3D(0,0,1));
    //joint4.rotate(jointvalue.at(3),QVector3D(1,0,0));
    
    joint5.translate(QVector3D(0,0,d5));
    //    joint5.rotate(90,QVector3D(0,1,0));
    joint5.rotate(90,QVector3D(1,0,0));
    joint5.rotate(jointvalue.at(4),QVector3D(0,0,1));
    
    //joint5.rotate(jointvalue.at(4),QVector3D(0,1,0));
    
    
    joint6.translate(QVector3D(0,(d6+toollength),0));
    //    joint6.rotate(90,QVector3D(0,1,0));
    joint6.rotate(-90,QVector3D(1,0,0));
    //    joint6.rotate(jointvalue.at(5),QVector3D(0,0,1));
    joint6.rotate(jointvalue.at(5), QVector3D(0,0,1));
    
    
    //    a1=25;a2=455;d1=400;a3=25;d4=420;d6=90;
    //    joint1=getTjoint(jointvalue.at(0),a1,90,d1,0);
    //    joint2=getTjoint(jointvalue.at(1),a2,0,0,jointvalue.at(0));
    //    joint3=getTjoint(jointvalue.at(2),a3,90,0,jointvalue.at(1));
    //    joint4=getTjoint(jointvalue.at(3),0,-90,d4,jointvalue.at(2));
    //    joint5=getTjoint(jointvalue.at(4),0,90,0,jointvalue.at(3));
    //    joint6=getTjoint(jointvalue.at(5),0,0,d6,jointvalue.at(4));
    
    
    QMatrix4x4 trans=joint1*joint2*joint3*joint4*joint5*joint6;
    
    qDebug()<<"fkcall is\n"<<trans<<joint1<<joint2<<joint3<<joint4<<joint5<<joint6;
    QVector<float> rpy;
    rpy.push_back(trans.row(0).w());
    rpy.push_back(trans.row(1).w());
    rpy.push_back(trans.row(2).w());
    getrpy(trans,rpy);
    qDebug()<<"rpy is"<<rpy;
    mrpy=rpy;
}

void Kinematics::fkcall2(const QVector<float> jointvalue, QVector<float> &rpy)
{
    if(jointvalue.size()<6) return;
    float j1,j2,j3,j4,j5,j6;
    j1=jointvalue.at(0)*PI/180;
    j2=jointvalue.at(1)*PI/180;
    j3=jointvalue.at(2)*PI/180;
    j4=jointvalue.at(3)*PI/180;
    j5=jointvalue.at(4)*PI/180;
    j6=jointvalue.at(5)*PI/180;
    
    QMatrix4x4 T10,T21,T32,T43,T54,T65;
    T10=getTjoint(j1,a1,-PI/2,d1);
    //qDebug()<<"T10 is"<<T10;
    T21=getTjoint(j2,a2,0,0);
    //qDebug()<<"T21 is"<<T21;
    T32=getTjoint(j3-PI/2,a3,PI/2,0);
    //qDebug()<<"T32 is"<<T32;
    T43=getTjoint(j4,0,-PI/2,-d4);
    //qDebug()<<"T43 is"<<T43;
    T54=getTjoint(j5,0,PI/2,0);
    //qDebug()<<"T54 is"<<T54;
    T65=getTjoint(j6,0,PI,-d6);
    //qDebug()<<"T65 is"<<T65;
    
    QMatrix4x4 Tr=T10*T21*T32*T43*T54*T65;
    
    //qDebug()<<"last T is"<<Tr;
    
    QVector<float> temprpy;
    temprpy.push_back(iszero(Tr.row(0).w()));
    temprpy.push_back(iszero(Tr.row(1).w()));
    temprpy.push_back(iszero(Tr.row(2).w()));
    getrpy(Tr,temprpy);
    
    rpy=temprpy;
    //qDebug()<<"rpy is"<<rpy;
}

bool Kinematics::ikcals(const QVector<float> fpos, const QVector<float> pos, QVector<float> &jointvalue)
{
    //qDebug()<<fpos<<pos;
    if(fpos.size()<6||pos.size()<6)
        return  false;
    QVector<QVector<float> > sloutions;
    QMatrix4x4 mts;
    mts.setToIdentity();
    getmatrix(pos,mts);
    float nx,ny,nz,ox,oy,oz,ax,ay,az,px,py,pz;
    nx=mts(0,0);ox=mts(0,1);ax=mts(0,2);px=mts(0,3);
    ny=mts(1,0);oy=mts(1,1);ay=mts(1,2);py=mts(1,3);
    nz=mts(2,0);oz=mts(2,1);az=mts(2,2);pz=mts(2,3);
    
    QVector3D p04={px-d6*ax,py-d6*ay,pz-d6*az};
    
    float joint1;
    if(!ikjoint1(mts,joint1))
    {
        qDebug()<<"can not get joint1 solution";
        return false;
    }
    QVector<float> sloution1=QVector<float>(6,0);
    sloution1[0]=joint1;
    //qDebug()<<"joint1"<<joint1*180/PI;
    
    float joint3,joint31;
    if(!ikjoint3(joint1,mts,joint3,joint31))
    {
        qDebug()<<"can not get joint3 solution";
        return  false;
    }
    QVector<float> sloution2=sloution1;
    if(joint3*180/PI>-207 && joint3*180/PI<66)
        sloution1[2]=joint3;
    else
        sloution1.clear();
    if(joint31*180/PI>-207 && joint31*180/PI<66)
        sloution2[2]=joint31;
    else
        sloution2.clear();
    
    //qDebug()<<"joint3"<<joint3*180/PI<<joint31*180/PI;
    
    float joint2,joint21;
    if(!ikjoint2(p04,joint1,joint2,joint21))
    {
        qDebug()<<"can not get joint2 solution";
        return  false;
    }
    
    //qDebug()<<"joint2"<<joint2*180/PI<<joint21*180/PI;
    QVector3D N60=QVector3D(ax,ay,az);
    float joint5,joint51;
    if(!ikjoint5(joint1,joint2,joint3,N60,joint5,joint51))
    {
        qDebug()<<"can not get joint5 solution";
        return false;
    }
    //qDebug()<<"joint5"<<joint5*180/PI<<joint51*180/PI;
    
    float joint4;
    if(!ikjoint4(joint1,joint2,joint3,mts,joint4))
    {
        qDebug()<<"can not get joint4 solution";
        return  false;
    }
    
    float joint6;
    if(!ikjoint6(joint1,joint2,joint3,mts,joint6))
    {
        qDebug()<<"can not get joint6 solution";
        return false;
    }
    //qDebug()<<"joint4"<<joint4*180/PI;
    //qDebug()<<"joint6"<<joint6*180/PI;
    jointvalue.clear();
    jointvalue.push_back(joint1*180/PI);
    jointvalue.push_back(joint2*180/PI);
    jointvalue.push_back(joint3*180/PI);
    jointvalue.push_back(joint4*180/PI);
    jointvalue.push_back(joint5*180/PI);
    jointvalue.push_back(joint6*180/PI);
    for(int i=0;i<DOF;++i)
    {
        if(qAbs(fpos.at(i)-jointvalue.at(i)) >= offset)
            return false;
    }
    return  true;
}

void Kinematics::LBR_fk(const float Q[6], Transformation *T)
{
    float q1,q2,q3,q4,q5,q6;
    q1 = Q[0]*M_PI/180; q2 = Q[1]*M_PI/180; q3 = Q[2]*M_PI/180;
    q4 = Q[3]*M_PI/180; q5 = Q[4]*M_PI/180; q6 = Q[5]*M_PI/180;
    QMatrix4x4 T01,T12,T23,T34,T45,T56,T06;
    DH2T(q1,d1,a1,-M_PI/2,T01);
    DH2T(q2,0,a2,0,T12);
    DH2T(q3-M_PI/2,0,a3,M_PI/2,T23);
    DH2T(q4,-d4,0,-M_PI/2,T34);
    DH2T(q5,0,0,M_PI/2,T45);
    DH2T(q6,-d6,0,M_PI,T56);
    T06=T01*T12*T23*T34*T45*T56;
    
    //qDebug()<<T06;
    
    T->rotation[0][0] = T06(0,0);
    T->rotation[1][0] = T06(1,0);
    T->rotation[2][0] = T06(2,0);
    T->rotation[0][1] = T06(0,1);
    T->rotation[1][1] = T06(1,1);
    T->rotation[2][1] = T06(2,1);
    T->rotation[0][2] = T06(0,2);
    T->rotation[1][2] = T06(1,2);
    T->rotation[2][2] = T06(2,2);
    T->translation[0] = T06(0,3);
    T->translation[1] = T06(1,3);
    T->translation[2] = T06(2,3);
}

void Kinematics::LBR_ik(const Transformation *T, const QVector<float> currentq, unsigned int *solutionnum, float inversesolutions[8][6])
{
    unsigned int currentsolutionnum = 0;
    float nx,ny,nz,ox,oy,oz,ax,ay,az,px,py,pz;
    nx = T->rotation[0][0]; ny = T->rotation[1][0]; nz = T->rotation[2][0];
    ox = T->rotation[0][1]; oy = T->rotation[1][1]; oz = T->rotation[2][1];
    ax = T->rotation[0][2]; ay = T->rotation[1][2]; az = T->rotation[2][2];
    px = T->translation[0]; py = T->translation[1]; pz = T->translation[2];
    
    QVector3D a=QVector3D(ax,ay,az);
    QVector3D p=QVector3D(px,py,pz);
    
    float solution[6] = {0,0,0,0,0,0};// pre_solution[6] = {10000,0,0,0,0,0};//d_Q = 10000;
    float sinq1,sinq2,sinq3,sinq5,sinq4,sinq6;
    float cosq1,cosq2,cosq3,cosq5,cosq4,cosq6;
    
    // ---------- solve q1 ----------
    QVector3D p04=p-a*d6;
    float q1_sin_cos_pair[2][2];
    unsigned char q1_num = phase_shift_eq(q1_sin_cos_pair,-p04.x(),p04.y(),0);
    
    
    if(!q1_num)
    {
        printf("no q1 solution\n");
        *solutionnum = 0;
        return; // no inverse solution
    }
    
    for(unsigned char i=0;i<q1_num;++i)
    {
        sinq1 = q1_sin_cos_pair[i][0];
        cosq1 = q1_sin_cos_pair[i][1];
        //----------solve q3------------
        QVector3D p01=QVector3D(a1*cosq1,a1*sinq1,d1);
        QVector3D p14=p04-p01;
        float q3_sin_cos_pair[2][2];
        unsigned char q3_num = phase_shift_eq(q3_sin_cos_pair,2*a2*a3,2*a2*d4,p14.lengthSquared()-a2*a2-a3*a3-d4*d4);
        if(!q3_num)
        {
            continue;
        }
        
        for(unsigned char j=0;j<q3_num;++j)
        {
            sinq3=q3_sin_cos_pair[j][0];
            cosq3=q3_sin_cos_pair[j][1];
            
            QVector3D p14_1=QVector3D((px-ax*d6)*cosq1+(py-ay*d6)*sinq1-a1,az*d6+d1-pz,(ax*d6-px)*sinq1+(py-ay*d6)*cosq1);
            float M=d4*cosq3+a3*sinq3+a2;
            float N=a3*cosq3-d4*sinq3;
            float b1=p14_1.x();
            float b2=p14_1.y();
            sinq2=(N*b1+M*b2)/(M*M+N*N);
            cosq2=(M*b1-N*b2)/(M*M+N*N);
            
            cosq5=-(((-cosq1*ax-sinq1*ay)*cosq3+sinq3*az)*cosq2+(az*cosq3+sinq3*(cosq1*ax+sinq1*ay))*sinq2);
            sincosround(&cosq5);
            sinq5=sqrt(1-cosq5*cosq5);
            float q5_sin_cos_pair[2][2];
            q5_sin_cos_pair[0][0]=sinq5;
            q5_sin_cos_pair[1][0]=-sinq5;
            q5_sin_cos_pair[0][1]=cosq5;
            q5_sin_cos_pair[1][1]=cosq5;
            for(unsigned char k=0;k<2;++k)
            {
                sinq5=q5_sin_cos_pair[k][0];
                cosq5=q5_sin_cos_pair[k][1];
                if(abs(sinq5)<1e-4)
                {
                    sinq4=sin(currentq.at(3)*M_PI/180);
                    cosq4=cos(currentq.at(3)*M_PI/180);
                    float b1=-sinq1*nx+cosq1*ny;
                    float b2=-sinq1*ox+cosq1*oy;
                    sinq6=b1*cosq4+b2*sinq4*cosq5;
                    cosq6=b1*sinq4*cosq5-b2*cosq4;
                    if(cosq6<-1||cosq6>1||sinq6>1||sinq6<-1)
                    {
                        continue;
                    }
                    
                }else
                {
                    float r13=(az*cosq3+sinq3*(cosq1*ax+sinq1*ay))*cosq2+sinq2*((cosq1*ax+sinq1*ay)*cosq3-sinq3*az);
                    float r23=cosq1*ay-sinq1*ax;
                    float r31=((-cosq1*nx-sinq1*ny)*cosq3+sinq3*nz)*cosq2+(nz*cosq3+sinq3*(cosq1*nx+sinq1*ny))*sinq2;
                    float r32=((-cosq1*ox-sinq1*oy)*cosq3+sinq3*oz)*cosq2+(oz*cosq3+sinq3*(cosq1*ox+sinq1*oy))*sinq2;
                    sinq4=-r23/sinq5;
                    cosq4=-r13/sinq5;
                    sinq6=-r32/sinq5;
                    cosq6=-r31/sinq5;
                    sincosround(&sinq4);
                    sincosround(&cosq4);
                    sincosround(&sinq6);
                    sincosround(&cosq6);
                }
                solution[0] = atan2(sinq1,cosq1)*180/M_PI;
                solution[1] = atan2(sinq2,cosq2)*180/M_PI;
                solution[2] = atan2(sinq3,cosq3)*180/M_PI;
                solution[3] = atan2(sinq4,cosq4)*180/M_PI;
                solution[4] = atan2(sinq5,cosq5)*180/M_PI;
                solution[5] = atan2(sinq6,cosq6)*180/M_PI;
                for(unsigned int m=0;m<DOF;++m)
                {
                    //                    if(currentq[m]<-180 && abs(currentq[m]+solution[m])-360<5)
                    //                        inversesolutions[currentsolutionnum][m] = solution[m]-180;
                    //                    else if(currentq[m]>180 && abs(currentq[m]+solution[m])-360<5)
                    //                        inversesolutions[currentsolutionnum][m] = solution[m]+180;
                    //                    else
                    inversesolutions[currentsolutionnum][m] = solution[m];
                }
                currentsolutionnum++;
            }
            
        }
    }
    *solutionnum = currentsolutionnum;
}


void Kinematics::getjointparam(QVector<float> paramet)
{
    if(waypoint.size()==0)
    {
        waypoint.push_back(parame.toStdVector());
        waypoint.push_back(paramet.toStdVector());
    }else
    {
        //qDebug()<<"seconed";
        waypoint.clear();
        waypoint.push_back(parame.toStdVector());
        waypoint.push_back(paramet.toStdVector());
    }
    //is_need_calculat_coefficient=true;
    cyclerun();
    movement_mode=0;
    isatendpoint=false;
    //getspacecurpostions();
    startrun();
}

void Kinematics::getjointparam(QVector<QVector<float> > paramet)
{
    //qDebug()<<"Kinematics::getjointparam";
    waypoint.clear();
    float dist=distansabout2point(parame,paramet.first());
    //qDebug()<<"getjointparam to parame"<<dist;
    if(dist>1e-1)
    {
        waypoint.push_back(parame.toStdVector());
        dist=distansabout2point(parame,homeposition);
        //qDebug()<<"getjointparam to home"<<dist;
        if(dist>1e-1)
        {
            waypoint.push_back(homeposition.toStdVector());
        }
        waypoint.push_back(paramet.first().toStdVector());
        setspeedratevalue(100);
        isatpath=false;
        cyclerun();
        movement_mode=0;
        isatendpoint=false;
        startrun();
    }else
    {
        for(int i=1;i<paramet.size();++i)
        {
            waypoint.push_back(paramet[i].toStdVector());
        }
        //setspeedratevalue(20);
        isatpath=true;
        movelqindex=0;
        movelq(QVector<float>::fromStdVector(waypoint.at(movelqindex)));
    }
}

void Kinematics::getrobotpath(QVector<Robot_Path> path)
{
    if(simulationTimer->isActive())
        stoprun();
    
    currentpath.clear();
    currentpath=path;
    pathflag=0;
    QVector<QVector<float>> wp;
    isatpath=true;
    if(runonepath(wp))
        getjointparam(wp);
    else
    {
        ispathready=false;
    }
}

void Kinematics::movelq(QVector<float> end)
{
    float Q1[6];
    for(int i=0;i<6;++i)
    {
        startq[i]=(float)parame.at(i);
        Q1[i]=(float)end.at(i);
    }
    
    if(islinefeasible(startq,Q1,LINE_V,LINE_A))
    {
        //qDebug()<<"islinefeasible is true";
        GeometryMetaData geometrymetadata;
        calculat_line_Geometry(startq,Q1,line,orientation);
        calculat_line_Trajectory(line,orientation,LINE_V,LINE_A,linetrajp,orientationtrajp);
        movement_mode=1;
        //isdrwn=true;
        LineGeometry* tem = new LineGeometry;
        V_assign(line->direction,tem->direction,3);
        tem->length = line->length;
        V_assign(line->startpoint,tem->startpoint,3);
        geometrymetadata.shapegeometry = (void*)tem;
        geometrymetadata.shape = LINE;
        geometrymatadataset.push_back(geometrymetadata);
        isatendpoint = false;
        //Q_EMIT getjointparamfilishsignal();
        startrun();
    }else
        return;
    //Q_EMIT positionnotreachable();
}

void Kinematics::stoprun()
{
    if(simulationTimer->isActive())
        simulationTimer->stop();
}

void Kinematics::setspeedratevalue(int value)
{
    speedrate=value/100.0;
}

void Kinematics::getrpy(const QMatrix4x4 mt, QVector<float> &rpy)
{
    float rx,ry,rz;
    float cs=mt.row(0).x();
    float si=mt.row(1).x();
    float sy= sqrt(cs*cs+si*si);
    bool singular= sy < 1e-6;
    
    if(!singular)
    {
        rx=iszero(atan2(mt.row(2).y(),mt.row(2).z()));
        ry=iszero(atan2(-mt.row(2).x(),sy));
        rz=iszero(atan2(mt.row(1).x(),mt.row(0).x()));
    }else
    {
        rx=iszero(atan2(-mt.row(1).z(),mt.row(1).y()));
        ry=iszero(atan2(-mt.row(2).x(),sy));
        rz=0;
    }
    /*
    float rx,ry,rz;
    double nx,ny,nz,ox,oy,oz,az;
    nx=mt.column(0).x();
    ny=mt.column(0).y();
    nz=mt.column(0).z();
    ox=mt.column(1).x();
    oy=mt.column(1).y();
    oz=mt.column(1).z();
    az=mt.column(2).z();
    float sy= sqrt(nx*nx+ny*ny);
    bool singular= sy < 1e-6;
    
//    if(!singular)
//    {
//        rx=atan2(mt.row(2).y(),mt.row(2).z());
//        ry=atan2(-mt.row(2).x(),sy);
//        rz=atan2(mt.row(1).x(),mt.row(0).x());
        ry=iszero(atan2(ny,nx));
        rx=iszero(atan2(az,oz));
        rz=iszero(atan2(sqrt(nx*nx+ny*ny),-nz));
//    }else
//    {
//        rx=atan2(ny,nx);
//        rz=atan2(sy,-nz);
//        ry=0;
//    }
*/
    
    rpy.push_back(rx*180/PI);
    rpy.push_back(ry*180/PI);
    rpy.push_back(rz*180/PI);
}

void Kinematics::getmatrix(const QVector<float> pos, QMatrix4x4 &mt)
{
    
    if(pos.size()<6) return;
    mt(0,3)=pos.at(0);
    mt(1,3)=pos.at(1);
    mt(2,3)=pos.at(2);
    
    float rx=pos.at(3)*PI/180;
    float ry=pos.at(4)*PI/180;
    float rz=pos.at(5)*PI/180;
    QMatrix3x3 matx,maty,matz;
    matx.setToIdentity();
    matx(1,1)=cos(rx);
    matx(1,2)=-sin(rx);
    matx(2,1)=sin(rx);
    matx(2,2)=cos(rx);
    maty.setToIdentity();
    maty(0,0)=cos(ry);
    maty(0,2)=sin(ry);
    maty(2,0)=-sin(ry);
    maty(2,2)=cos(ry);
    matz.setToIdentity();
    matz(0,0)= cos(rz);
    matz(0,1)= -sin(rz);
    matz(1,0)= sin(rz);
    matz(1,1)= cos(rz);
    QMatrix3x3 R=matz*maty*matx;
    mt(0,0)=R(0,0);
    mt(0,1)=R(0,1);
    mt(0,2)=R(0,2);
    mt(1,0)=R(1,0);
    mt(1,1)=R(1,1);
    mt(1,2)=R(1,2);
    mt(2,0)=R(2,0);
    mt(2,1)=R(2,1);
    mt(2,2)=R(2,2);
    /*
    double aph,beta,gamma;
    aph =pos.at(3)*PI/180;
    beta =pos.at(4)*PI/180;
    gamma =pos.at(5)*PI/180;
    mt(0,0)=cos(aph)*cos(beta);
    mt(1,0)=sin(aph)*cos(beta);
    mt(2,0)=-sin(beta);
    mt(0,1)=cos(aph)*sin(beta)*sin(gamma)-sin(aph)*cos(gamma);
    mt(1,1)=sin(aph)*sin(beta)*sin(gamma)+cos(aph)*cos(gamma);
    mt(2,1)=cos(beta)*sin(gamma);
    mt(0,2)=cos(aph)*sin(beta)*cos(gamma)+sin(aph)*sin(gamma);
    mt(1,2)=sin(aph)*sin(beta)*cos(gamma)-cos(aph)*sin(gamma);
    mt(2,2)=cos(beta)*cos(gamma);
    */
    
}

QMatrix4x4 Kinematics::getTjoint(double angle, double ai, double aph, double di)
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.setRow(0,QVector4D(cos(angle),-sin(angle)*cos(aph),sin(angle)*sin(aph),ai*cos(angle)));
    matrix.setRow(1,QVector4D(sin(angle),cos(angle)*cos(aph),-cos(angle)*sin(aph),ai*sin(angle)));
    matrix.setRow(2,QVector4D(0,sin(aph),cos(aph),di));
    matrix.setRow(3,QVector4D(0,0,0,1));
    
    
    
    //    matrix.setRow(0,QVector4D(cos(angle),-sin(angle),0,ai));
    //    matrix.setRow(1,QVector4D(sin(angle)*cos(aph),cos(angle)*cos(aph),-sin(aph),-di*sin(aph)));
    //    matrix.setRow(2,QVector4D(sin(angle)*sin(aph),cos(angle)*sin(aph),cos(aph),di*cos(aph)));
    //    matrix.setRow(3,QVector4D(0,0,0,1));
    //qDebug()<<matrix;
    return  matrix;
}

bool Kinematics::ikjoint1(const QMatrix4x4 mts,float &joint1)
{
    float ax,ay,az,px,py,pz;
    ax=mts(0,2);px=mts(0,3);
    ay=mts(1,2);py=mts(1,3);
    az=mts(2,2);pz=mts(2,3);
    QVector3D p04={px-d6*ax,py-d6*ay,pz-d6*az};
    
    joint1=atan2(p04.y(),p04.x());
    //qDebug()<<p04<<joint1*180/PI;
    if(isnan(joint1))
        return  false;
    if(joint1<minJointAngles.at(0)||joint1>maxJointAngles.at(0))
    {
        return false;
    }
    
    if(p04.y()<1e-3 && p04.y()>-1e-3)
    {
        p04.setY(0);
        if(p04.x()>0)
            joint1=0;
        else {
            joint1=PI;
        }
    }
    return true;
}

bool Kinematics::ikjoint2(const QVector3D p04, const float joint1, float &joint2,float &joint21)
{
    QMatrix4x4 R01;
    R01=getTjoint(joint1,a1,-PI/2,d1);
    R01=R01.inverted();
    QVector3D p01={a1*cos(joint1),a1*sin(joint1),d1};
    QVector3D p14= p04-p01;
    QVector4D inp14=QVector4D(p14);
    inp14 = R01*inp14;
    float l4=sqrt(a3*a3+d4*d4);
    float beta1 =atan2(inp14.x(),inp14.y());
    //float beta1=acos(inp14.x()/inp14.z());
    float beta2=acos((a2*a2+p14.lengthSquared()-l4*l4)/(2*a2*(p14.length())));
    //qDebug()<<"ikjoint2"<<beta1*180/PI<<beta2*180/PI<<inp14;
    joint2=PI/2-(beta1+beta2);
    joint21=beta1-beta2;
    
    if(isnan(joint2))
        return  false;
    if(joint2<minJointAngles.at(1)||joint2>maxJointAngles.at(1))
    {
        return false;
    }
    
    if(joint2>-1e-3 && joint2<1e-3)
        joint2=0;
    
    return  true;
}

bool Kinematics::ikjoint3(const float joint1, const QMatrix4x4 mts, float &joint3, float &joint31)
{
    QMatrix4x4 p06=mts;
    //qDebug()<<p06;
    QVector3D p04={p06(0,3)-d6*p06(0,2),p06(1,3)-d6*p06(1,2),p06(2,3)-d6*p06(2,2)};
    QVector3D p01={a1*cos(joint1),a1*sin(joint1),d1};
    QVector3D p14= p04-p01;
    
    float l4=sqrt(a3*a3+d4*d4);
    float oumg=acos(a3/l4);
    
    float fi=acos((a2*a2+l4*l4-p14.lengthSquared())/(2*a2*l4));
    //float oumg=atan2(a3,d4);
    joint31=-1*(fi-oumg);
    joint3=(3*PI/2-fi-oumg);
    if(isnan(joint3))
        return  false;
    if(joint3<minJointAngles.at(2)||joint3>maxJointAngles.at(2))
    {
        return false;
    }
    
    if(joint3<1e-4&&joint3>-1e-4)
        joint3=0;
    return  true;
}

bool Kinematics::ikjoint4(const float joint1, const float joint2, const float joint3, const QMatrix4x4 mts, float &joint4)
{
    QMatrix4x4 T10,T21,T32,T40;
    T10=getTjoint(joint1,a1,-PI/2,d1);
    T21=getTjoint(joint2,a2,0,0);
    T32=getTjoint(joint3,a3,PI/2,0);
    T40=T10*T21;
    QMatrix4x4 T63=T40.inverted()*mts;
    //qDebug()<<T63;
    joint4=atan2(-T63(1,2),T63(0,2))+PI/2;
    if(isnan(joint4))
        return  false;
    if(joint4<minJointAngles.at(3)||joint4>maxJointAngles.at(3))
    {
        return false;
    }
    
    if(joint4<1e-3&&joint4>-1e-3)
        joint4=0;
    return true;
}

bool Kinematics::ikjoint5(const float joint1,const float joint2,const float joint3,const QVector3D N60,float &joint5,float &joint51)
{
    QMatrix4x4 T10,T21,T32,T40;
    T10=getTjoint(joint1,a1,-PI/2,d1);
    T21=getTjoint(joint2-PI/2,a2,0,0);
    T32=getTjoint(joint3,a3,PI/2,0);
    T40=T10*T21*T32;
    QVector3D N40=QVector3D(T40(0,2),T40(1,2),T40(2,2));
    //QVector3D N60=QVector3D(ax,ay,az);
    //qDebug()<<N40<<N60;
    
    joint5=PI - acos(QVector3D::dotProduct(N40,N60));
    
    joint51=PI+acos((N40.x()*N60.x()+N40.y()*N60.y()+N40.z()*N60.z()));
    if(isnan(joint5))
        return  false;
    if(joint5<minJointAngles.at(4)||joint5>maxJointAngles.at(4))
    {
        return false;
    }
    
    
    
    if(joint5>-1e-3&&joint5<1e-3)
        joint5=0;
    if(joint51>-1e-3&&joint51<1e-3)
        joint51=0;
    return  true;
}

bool Kinematics::ikjoint6(const float joint1, const float joint2, const float joint3, const QMatrix4x4 mts, float &joint6)
{
    QMatrix4x4 T10,T21,T32,T40;
    T10=getTjoint(joint1,a1,-PI/2,d1);
    T21=getTjoint(joint2-PI/2,a2,0,0);
    T32=getTjoint(joint3,a3,PI/2,0);
    T40=T10*T21;
    QMatrix4x4 T63=T40.inverted()*mts;
    
    joint6=PI/2-atan2(-T63(2,1),T63(2,0));
    
    if(isnan(joint6))
        return  false;
    if(joint6<minJointAngles.at(5)||joint6>maxJointAngles.at(5))
    {
        return false;
    }
    
    if(joint6<1e-3&&joint6>-1e-3)
        joint6=0;
    return true;
}
double Kinematics::iszero(double value)
{
    if(value>-1e-4 && value<1e-4)
        return 0.0;
    else
        return value;
}

bool Kinematics::islinefeasible(const float Q0[6], const float Q1[6], const float v, const float a)
{
    int initsign; // start point det jacobian sign
    bool isclosetozero;
    int sign;
    detjacobianinfo(Q0,&isclosetozero,&initsign);
    if(isclosetozero)
    {
        printf("line start point is singular\n");
        emit singularpointinline();
        return false;
    }
    
    detjacobianinfo(Q1,&isclosetozero,&sign);
    if(isclosetozero)
    {
        printf("line end point is singular\n");
        emit singularpointinline();
        return false;
    }
    if(sign!=initsign)
    {
        printf("singular point on the line\n");
        emit singularpointinline();
        return false;
    }
    
    Transformation T0,T1;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    float p0[3],p1[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T1,R1);
    // position trajectory
    LineGeometry* line = (LineGeometry*)malloc(sizeof(LineGeometry));
    linepath(p0,p1,line);
    STrajectoryParameter* linetrajp = (STrajectoryParameter*)malloc(sizeof(STrajectoryParameter));
    strajectory(line->length,v,a,linetrajp);
    // orientation trajectory
    OrientationGeometry* orientation = (OrientationGeometry*)malloc(sizeof(OrientationGeometry));
    orientationpath(R0,R1,orientation);
    float orientationtrajp[6];
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,linetrajp->T,orientationtrajp);
    
    float timestep = 0.05,time = timestep;
    
    float targetp[3],targetR[3][3],currentq[6];
    V_assign(Q0,currentq,6);
    Transformation* targetT = (Transformation*)malloc(sizeof(Transformation));
    float inversesolutions[8][6];
    unsigned int solutionnum = 0;
    float distance,currentdistance;
    unsigned int minindex;
    while(time<linetrajp->T)
    {
        // cartesian interpolation
        lineinterpolation(line,linetrajp,time,targetp);
        orientationinterpolation(orientation,orientationtrajp,time,targetR);
        // inverse kinematics
        T_from_RP(targetR,targetp,targetT);
        LBR_ik(targetT,parame,&solutionnum,inversesolutions);
        if(!solutionnum)
        {
            printf("not reachable point on the line\n");
            return false;
        }
        // find closet configuration
        minindex = 0;
        currentdistance = caldistance(inversesolutions[0],currentq);
        distance = currentdistance;
        for(unsigned int j=1;j<solutionnum;++j)
        {
            currentdistance = caldistance(inversesolutions[j],currentq);
            if(currentdistance < distance)
            {
                distance = currentdistance;
                minindex = j;
            }
        }
        V_assign(inversesolutions[minindex], currentq, JOINT_NUM);
        // check
        detjacobianinfo(currentq,&isclosetozero,&sign);
        if(isclosetozero)
        {
            printf("singular point on the line\n");
            V_print(currentq,6);
            return false;
        }
        if(sign!=initsign)
        {
            printf("middle point on the line is singular\n");
            return false;
        }
        
        time += timestep;
    }
    free(line);
    free(linetrajp);
    free(orientation);
    free(targetT);
    return true;
}

void Kinematics::linepath(const float p0[], const float p1[], LineGeometry *line)
{
    float delta[3];
    V_sub_V(p1,p0,delta,3);           // delta = p1 - p0;
    V_assign(p0,line->startpoint,3);  // p0 is the start point
    V_dir(delta,line->direction,3);   // delta/norm(delta) is the direction unit
    line->length = V_norm(delta,3);   // norm(delta) is length
}

void Kinematics::strajectory(const float l, const float v, const float a, STrajectoryParameter *trajp)
{
    float a_inv = 1/a;
    
    float omega,t1,s1,t2,s2,T;
    // -- is the given velocity can be reached? ---
    float maxv = sqrt(M_2_PI*l*a);
    
    if(v>maxv)
    {
        //printf("v is greater than maxv\n");
        // only sinusoidal acceleration and deceleration
        trajp->v = maxv;
        omega = 2*a/maxv;
        t1 = M_PI_2*maxv*a_inv;
        s1 = 0.5*l;
        t2 = t1;
        s2 = 0;
        T = 2*t1;
    }
    else
    {
        //printf("v is less than maxv\n");
        trajp->v = v;
        float v_inv = 1/v;
        omega = 2*a*v_inv;
        t1 = M_PI_2*v*a_inv;
        s1 = 0.5*v*t1;
        t2 = l*v_inv;
        s2 = l - v*t1;
        T = t1 + t2;
    }
    trajp->a = a;
    trajp->omega = omega;
    trajp->t1 = t1;
    trajp->t2 = t2;
    trajp->T = T;
    trajp->s1 = s1;
    trajp->s2 = s2;
}

void Kinematics::orientationpath(const float R0[3][3], const float R1[3][3], OrientationGeometry *orientation)
{
    float temp1[3][3],temp2[3][3];
    R_inv(R0,temp1);
    R_mul_R(temp1,R1,temp2);
    //printf("temp2:\n");
    //R_print(temp2);
    R_assign(R0,orientation->R0);
    R_to_AA(temp2,&orientation->AA);
    //    printf("oritentation info:\n");
    //    printf("R0:\n");
    //    R_print(orientation->R0);
    //    printf("AA:\n");
    //    AA_print(&orientation->AA);
}

void Kinematics::fiveordertrajectory(const float p0, const float v0, const float a0, const float p1, const float v1, const float a1, const float T, float coefficient[6])
{
    float h = p1 - p0;
    coefficient[0] = 1/(2*pow(T,5))*(12*h-6*(v0+v1)*T+(a1-a0)*T*T);
    coefficient[1] = 1/(2*pow(T,4))*(-30*h+(14*v1+16*v0)*T+(3*a0-2*a1)*T*T);
    coefficient[2] = 1/(2*pow(T,3))*(20*h-(8*v1+12*v0)*T-(3*a0-a1)*T*T);
    coefficient[3] = 0.5*a0;
    coefficient[4] = v0;
    coefficient[5] = p0;
}

void Kinematics::calculat_line_Geometry(const float Q0[], const float Q1[], LineGeometry *line, OrientationGeometry *orientation)
{
    //printf("11111111111111");
    Transformation T0,T1;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    float p0[3],p1[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T1,R1);
    
    linepath(p0,p1,line);
    orientationpath(R0,R1,orientation);
}

void Kinematics::calculat_line_Trajectory(const LineGeometry *line, const OrientationGeometry *orientation, const float v, const float a, STrajectoryParameter *linetrajp, float orientationtrajp[])
{
    //printf("222222222222222222");
    strajectory(line->length,v,a,linetrajp);
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,linetrajp->T,orientationtrajp);
}

bool Kinematics::isatruange(const float solution[6])
{
    for(int i=0;i<DOF;++i)
    {
        if(solution[i]*180/M_PI>maxjointangle.at(i)||solution[i]*180/M_PI<minjointangle.at(i))
        {
            return false;
        }
    }
    return true;
}

bool Kinematics::runonepath(QVector<QVector<float> > &wp)
{
    if((unsigned int)currentpath.size()<=pathflag)
    {
        ispathready=false;
        return false;
    }
    Robot_Path path;
    if(isatpath)
    {
        for(int i=pathflag;i<currentpath.size();++i)
        {
            pathflag++;
            //qDebug()<<"runonepath"<<pathflag<<i<<currentpath.at(i).runflag;
            if(currentpath.at(i).runflag)
            {
                path=currentpath.at(i);
                break;
            }
        }
    }else
    {
        path=currentpath.at(pathflag-1);
    }
    
    if(path.joint.size()==0)
    {
        ispathready=false;
        return false;
    }
    wp=path.joint;
    bescan=path.isneedscan;
    
    ispathready=true;
    return true;
    
}

void Kinematics::speedcontrol(const QVector<float> &pa)
{
    QVector<float> starp=currentpath.at(pathflag-1).joint.first();
    float dist1=0;
    dist1=distansabout2point(pa,starp);
    if(dist1<1)
    {
        setspeedratevalue(20);
    }
}

float Kinematics::distansabout2point(const QVector<float> &p1, const QVector<float> &p2)
{
    float dist1=0;
    dist1+=(qAbs(p1.at(0)-p2.at(0))+qAbs(p1.at(1)-p2.at(1))+qAbs(p1.at(2)-p2.at(2))+qAbs(p1.at(3)-p2.at(3))+qAbs(p1.at(4)-p2.at(4))+qAbs(p1.at(5)-p2.at(5)));
    return dist1;
}

void Kinematics::sendVirtualJointPosition()
{
    speedcontrol(parame);
    switch (movement_mode)
    {
    case 0:
        getcommonmodeposition();
        break;
    case 1:
    {
        currenttimepoint += speedrate*time_step;
        if(currenttimepoint>=linetrajp->T)
        {
            currenttimepoint = linetrajp->T;
            isatendpoint = true;
        }
        float tempparam[6];
        float currentq[6];
        for(unsigned int i=0;i<6;++i)
        {
            currentq[i] = parame.at(i);
        }
        calculat_next_lq_point(line,orientation,linetrajp,orientationtrajp,currenttimepoint,currentq,tempparam);
        for(int i=0;i<6;++i)
        {
            parame[i]=tempparam[i];
        }
    }
        break;
    case 2:
    {
        currenttimepoint += speedrate*time_step;
        if(currenttimepoint>=circlearctrajp->T)
        {
            currenttimepoint = circlearctrajp->T;
            isatendpoint = true;
        }
        float tempparam[6];
        float currentq[6];
        for(unsigned int i=0;i<6;++i)
        {
            currentq[i] = parame.at(i);
        }
        caculate_next_arc_point(circlearc,arc_orientation,circlearctrajp,orientationtrajp,currenttimepoint,currentq,tempparam);
        for(int i=0;i<6;++i)
        {
            parame[i]=tempparam[i];
        }
    }
        break;
    default:
        break;
    }
    
    if(isatendpoint)
    {
        //stoprun();
        atendpoint();
        //qDebug()<<"at end"<<movement_mode;
        currenttimepoint =0;
        isatendpoint=false;
        if(movement_mode==1)
        {
            //qDebug()<<"movelqindex is"<<movelqindex<<waypoint.size();
            if(movelqindex < (waypoint.size()-1))
            {
                movelqindex++;
                movelq(QVector<float>::fromStdVector(waypoint.at(movelqindex)));
            }
            else
            {
                QVector<QVector<float> > wp;
                if(runonepath(wp))
                {
                    getjointparam(wp);
                }else
                {
                    Q_EMIT currentcommandsignal();
                }
            }
        }
        if(movement_mode==0)
        {
            QVector<QVector<float> > wp;
            if(runonepath(wp))
            {
                getjointparam(wp);
            }else
            {
                Q_EMIT currentcommandsignal();
            }
        }
    }
    //qDebug()<<"current parame"<<parame;
    int p=0;
    if(isatpath)
        p+=10;
    if(bescan)
        p+=1;
    emit dataforsimulate(parame,p);
}

void Kinematics::atendpoint()
{
    if(simulationTimer->isActive())
        simulationTimer->stop();
    setspeedratevalue(100);
}

float Kinematics::cal_position(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    return polynomialvalue(5,coefficients->data,t);
}

void Kinematics::getspeedratevalue(int value)
{
    speedrate=value/100.0;
}

void Kinematics::startrun()
{
    //stopflag=false;
    if(simulationTimer->isActive())
        simulationTimer->stop();
    simulationTimer->start(time_step*1000);
}

void Kinematics::getcommonmodeposition()
{
    QVector<float> currentJointPosition;
    currenttimepoint += speedrate*time_step;//当前时间点，即时间步长累加
    for(unsigned int j=0;j<6;++j)
    {
        unsigned int index = 0;
        FiveOrderPolyCoefficients* coefficients;
        if((tmax-currenttimepoint)<=0)
        {
            currenttimepoint=tmax;
            isatendpoint=true;
            atendpoint();
        }
        else
        {
            for(unsigned int i=0;i<waypointsnum-1;++i)
            {
                if(currenttimepoint<timepoints[i+1])
                {
                    index = i;
                    break;
                }else
                {
                    //qDebug()<<"*************************"<<i;
                }
            }
            
            coefficients = &(allcoefficients[index][j]);//当前使用系数
            float t = currenttimepoint-timepoints[index];
            currentJointPosition.push_back(cal_position(coefficients,t));
        }
        
    }
    if(currentJointPosition.size() < DOF)
        return;
    else
    {
        parame=currentJointPosition;
    }
    
}

void Kinematics::cyclerun()
{
    waypointsnum=waypoint.size();
    //qDebug()<<"waypointsnum="<<waypointsnum;
    waypoints = (Waypoint*)malloc(sizeof(Waypoint)* waypointsnum);
    for(unsigned int i=0;i<waypointsnum;++i)
    {
        for(unsigned int j=0;j<6;++j)
        {
            waypoints[i].data[j] = (float)waypoint[i][j];
        }
    }
    
    timepoints = (float*)malloc(sizeof(float)*waypointsnum);
    float velocitylimit[6] = {60,60,60,60,60,60};
    cal_timepoints(timepoints,waypoints,waypointsnum,velocitylimit);
    tmax = timepoints[waypointsnum-1];
    //printf("tmax: %lf",tmax);
    float vs[6] = {0,0,0,0,0,0};
    float ve[6] = {0,0,0,0,0,0};
    
    float as[6] = {0,0,0,0,0,0};
    float ae[6] = {0,0,0,0,0,0};
    
    BoundaryCondition start,end;
    for(unsigned int i=0;i<6;++i)
    {
        start.v.data[i] = vs[i];
        start.a.data[i] = as[i];
        end.v.data[i] = ve[i];
        end.a.data[i] = ae[i];
    }
    unsigned int intervalnum = waypointsnum-1;
    
    
    allcoefficients = (FiveOrderPolyCoefficients**)malloc(sizeof(FiveOrderPolyCoefficients*)*intervalnum);
    for(unsigned int i=0;i<intervalnum;++i)
    {
        allcoefficients[i] = (FiveOrderPolyCoefficients*)malloc(sizeof(FiveOrderPolyCoefficients)*6);
    }
    cal_coefficients_for_all_joints_all_intervals(allcoefficients,waypoints,waypointsnum,timepoints,&start,&end);
}

void Kinematics::cal_timepoints(float *timepoints, const Waypoint *waypoints, const unsigned int waypointsnum, const float *velocitylimit)
{
    // the first time point is always zero
    timepoints[0] = 0;
    for(unsigned int i=1;i<waypointsnum;++i)
    {
        timepoints[i] = timepoints[i-1] + cal_timeduraion_on_one_interval(&(waypoints[i-1]),&(waypoints[i]),velocitylimit);
    }
}

float Kinematics::cal_timeduraion_on_one_interval(const Waypoint *from, const Waypoint *to, const float *velocitylimit)
{
    
    float timeduration = 0;
    float temp = 0;
    for(unsigned int i=0;i<6;++i)
    {
        temp = fabs(from->data[i]-to->data[i])/velocitylimit[i]*2;
        timeduration = (temp>timeduration? temp:timeduration);
    }
    return timeduration;
}

void Kinematics::cal_coefficients_for_all_joints_all_intervals(FiveOrderPolyCoefficients **allcoefficients, const Waypoint *waypoints, const unsigned int waypointsnum, const float *timepoints, const BoundaryCondition *start, const BoundaryCondition *end)
{
    // only one interval
    if(waypointsnum == 2)
    {
        cal_coefficients_for_all_joints_one_interval(allcoefficients[0],
                timepoints[1]-timepoints[0],
                &waypoints[0],&waypoints[1],
                &(start->v),&(end->v),
                &(start->a),&(end->a));
        //printf("waypoints num =2 computed\n");
        return;
    }
    
    // other cases
    unsigned int index = 0;
    float timeduration01 = 0;
    float timeduration12 = 0;
    float timeduration23 = 0;
    Velocity v0,v1,v2,v01,v12,v23;
    Acceleration a0,a1,a01,a12;
    while(index<=waypointsnum-2)
    {
        // calculate velocity and acceleration
        cal_velocity_and_acceleration(waypoints,waypointsnum,timepoints,
                                      start,end,
                                      index,
                                      &timeduration01,&timeduration12,&timeduration23,
                                      &v0,&v1,&v2,&v01,&v12,&v23,
                                      &a0,&a1,&a01,&a12);
        cal_coefficients_for_all_joints_one_interval(allcoefficients[index],timeduration01,
                                                     &waypoints[index],&waypoints[index+1],&v0,&v1,&a0,&a1);
        ++index;
    }
    //    for(int i=0;i<waypointsnum-1;i++)
    //    {
    //        std::cout<<"this is"<<i<<"seg"<<std::endl;
    //        for(int j=0;j<6;j++)
    //        {
    //            std::cout<<"this is "<<j<<"joint"<<std::endl;
    //            for(int k=0;k<6;k++)
    //            {
    //                std::cout<<allcoefficients[i][j].data[k]<<" ";
    //            }
    //            std::cout<<std::endl;
    //        }
    //    }
}

void Kinematics::cal_coefficients_for_all_joints_one_interval(FiveOrderPolyCoefficients *jointscoefficients, const float timeduration, const Waypoint *p0, const Waypoint *p1, const Velocity *v0, const Velocity *v1, const Acceleration *a0, const Acceleration *a1)
{
    for(unsigned int i=0;i<6;++i)
    {
        //printf("claculating %d joint coefficient\n",i);
        //printf("p0 %lf,v0 %lf,a0 %lf\n",p0->data[i],v0->data[i],a0->data[i]);
        //printf("p1 %lf,v1 %lf,a1 %lf\n",p1->data[i],v1->data[i],a1->data[i]);
        cal_coefficients_for_one_joint_one_interval(jointscoefficients+i,timeduration,
                                                    p0->data[i],p1->data[i],v0->data[i],v1->data[i],a0->data[i],a1->data[i]);
    }
}

bool Kinematics::cal_coefficients_for_one_joint_one_interval(FiveOrderPolyCoefficients *jointcoefficients, const float t, const float p0, const float p1, const float v0, const float v1, const float a0, const float a1)
{
    // coefficients order is
    // a5 a4 a3 a2 a1 a0
    float h = p1 - p0;
    jointcoefficients->data[0] = 1/(2*pow(t,5))*
            (12*h-6*(v0+v1)*t+(a1-a0)*t*t);
    jointcoefficients->data[1] = 1/(2*pow(t,4))*
            (-30*h+(14*v1+16*v0)*t+(3*a0-2*a1)*t*t);
    jointcoefficients->data[2] = 1/(2*pow(t,3))*
            (20*h-(8*v1+12*v0)*t-(3*a0-a1)*t*t);
    jointcoefficients->data[3] = 0.5*a0;
    jointcoefficients->data[4] = v0;
    jointcoefficients->data[5] = p0;
    return true;
}

void Kinematics::cal_velocity_and_acceleration(const Waypoint *waypoints, const unsigned int waypointsnum, const float *timepoints, const BoundaryCondition *start, const BoundaryCondition *end, const unsigned int index, float *timeduration01, float *timeduration12, float *timeduration23, Velocity *v0, Velocity *v1, Velocity *v2, Velocity *v01, Velocity *v12, Velocity *v23, Acceleration *a0, Acceleration *a1, Acceleration *a01, Acceleration *a12)
{
    if(index == 0 && waypointsnum-3!=index) // the first interval and total points numer is not 3
    {
        //printf("the first interval of over 3 points\n");
        float t01 = timepoints[index+1]-timepoints[index];
        float t12 = timepoints[index+2]-timepoints[index+1];
        float t23 = timepoints[index+3]-timepoints[index+2];
        *timeduration01 = t01;
        *timeduration12 = t12;
        *timeduration23 = t23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = start->v.data[i];
            a0->data[i] = start->a.data[i];
            float v01i = (waypoints[index+1].data[i]-waypoints[index].data[i])/t01;
            float v12i = (waypoints[index+2].data[i]-waypoints[index+1].data[i])/t12;
            float v23i = (waypoints[index+3].data[i]-waypoints[index+2].data[i])/t23;
            float v1i = cal_velocity_at_mid_point_one_joint(v01i,v12i);
            float v2i = cal_velocity_at_mid_point_one_joint(v12i,v23i);
            float a01i = (v1i-v0->data[i])/t01;
            float a12i = (v2i-v1i)/t12;
            float a1i = cal_acceleration_at_mid_point_one_joint(a01i,a12i);
            v01->data[i] = v01i;
            v12->data[i] = v12i;
            v23->data[i] = v23i;
            v1->data[i] = v1i;
            v2->data[i] = v2i;
            a01->data[i] = a01i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(index == 0 && waypointsnum == 3) // the firse interval and total point number is 3
    {
        //printf("the first interval of 3 points\n");
        float t01 = timepoints[index+1]-timepoints[index];
        float t12 = timepoints[index+2]-timepoints[index+1];
        *timeduration01 = t01;
        *timeduration12 = t12;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = start->v.data[i];
            a0->data[i] = start->a.data[i];
            float v01i = (waypoints[index+1].data[i]-waypoints[index].data[i])/t01;
            float v12i = (waypoints[index+2].data[i]-waypoints[index+1].data[i])/t12;
            float v1i = cal_velocity_at_mid_point_one_joint(v01i,v12i);
            float a01i = (v1i-v0->data[i])/t01;
            float a12i = (end->v.data[i]-v1i)/t12;
            float a1i = cal_acceleration_at_mid_point_one_joint(a01i,a12i);
            v01->data[i] = v01i;
            v12->data[i] = v12i;
            v1->data[i] = v1i;
            a01->data[i] = a01i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(index!=0 && waypointsnum-3 == index) // the second last interval
    {
        //printf("the second lase interval of over 3 points\n");
        *timeduration01 = *timeduration12;
        *timeduration12 = *timeduration23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            v1->data[i] = v2->data[i];
            a0->data[i] = a1->data[i];
            a01->data[i] = a12->data[i];
            float a12i = (end->v.data[i]-v1->data[i])/(*timeduration12);
            float a1i = cal_acceleration_at_mid_point_one_joint(a01->data[i],a12i);
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(waypointsnum-2 == index) // the last interval
    {
        //printf("the last interval\n");
        *timeduration01 = *timeduration12;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            a0->data[i] = a1->data[i];
            v1->data[i] = end->v.data[i];
            a1->data[i] = end->a.data[i];
        }
    }
    else   // interval 1 to n-4
    {
        *timeduration01 = *timeduration12;
        *timeduration12 = *timeduration23;
        float t23 = timepoints[index+3]-timepoints[index+2];
        *timeduration23 = t23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            v1->data[i] = v2->data[i];
            v01->data[i] = v12->data[i];
            v12->data[i] = v23->data[i];
            a0->data[i] = a1->data[i];
            a01->data[i] = a12->data[i];
            float v23i = (waypoints[index+3].data[i]-waypoints[index+2].data[i])/t23;
            float v2i =cal_velocity_at_mid_point_one_joint(v12->data[i],v23i);
            float a12i = (v2i-v1->data[i])/(*timeduration12);
            float a1i = cal_acceleration_at_mid_point_one_joint(a01->data[i],a12i);
            v23->data[i] = v23i;
            v2->data[i] = v2i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
}

float Kinematics::cal_velocity_at_mid_point_one_joint(const float v01, const float v12)
{
    if(isgreaterthanzero(v01) == isgreaterthanzero(v12))
    {
        return 0.5*(v01+v12);
    }
    else
    {
        return 0;
    }
    
}

float Kinematics::cal_acceleration_at_mid_point_one_joint(const float a01, const float a12)
{
    // if pre and post average accelerations have the same sign
    // the mid point acceleration is the average of the two average accelerations
    if(isgreaterthanzero(a01) == isgreaterthanzero(a12))
    {
        return 0.5*(a01+a12);
    }
    //
    else
    {
        return 0;
    }
}

void Kinematics::calculat_next_lq_point(const LineGeometry *line, const OrientationGeometry *orientation, const STrajectoryParameter *linetrajp, const float orientationtrajp[], const float time, const float currentq[], float result[6])
{
    //1. transformation interpolation
    Transformation targetT;
    float inversesolutions[8][6];
    unsigned int solutionnum = 0;
    
    lineinterpolation(line,linetrajp,time,targetT.translation);
    orientationinterpolation(orientation,orientationtrajp,time,targetT.rotation);
    
    // 2. inverse kinematics
    LBR_ik(&targetT,parame,&solutionnum,inversesolutions);
    if(!solutionnum)
    {
        printf("not reachable point\n");
        return;
    }
    // 3. find closet configuration
    closestq(inversesolutions,solutionnum,currentq,result);
}

void Kinematics::lineinterpolation(const LineGeometry *line, const STrajectoryParameter *trajp, const float t, float p[])
{
    // -- calculate scale corresponding to t --             s(t)
    float s = strajectoryscale(trajp,t);
    // -- calculate point onlth line corresponding to s --  p = p0 + s*(p1 - p0)/norm(p1 - p0)
    float temp[3];
    V_mul_S(line->direction,s,temp,3);
    V_add_V(line->startpoint,temp,p,3);
}

float Kinematics::strajectoryscale(const STrajectoryParameter *trajp, const float t)
{
    if(t<trajp->t1)      // sinusoidal accelaration
    {
        float omega_inv = 1/trajp->omega;
        return trajp->a*omega_inv*(t-sin(trajp->omega*t)*omega_inv);
    }
    else if(t<trajp->t2) // const velocity
    {
        return trajp->s1 + trajp->v*(t-trajp->t1);
    }
    else                    // sinusoidal decelaration
    {
        float omega_inv = 1/trajp->omega, tp = t - trajp->t2;
        return trajp->s1 + trajp->s2 + trajp->a*omega_inv*(tp+sin(trajp->omega*tp)*omega_inv);
    }
}

void Kinematics::caculate_next_arc_point(const CircleArcGeometry *circlearc, const OrientationGeometry *orientation, const STrajectoryParameter *circlearctrajp, const float orientationtrajp[], const float time, const float currentq[], float result[])
{
    float targetp[3],targetR[3][3];;
    //V_assign(Q0,currentq,6);
    Transformation targetT;
    float inversesolutions[8][6];
    unsigned int solutionnum = 0;
    //printf("time num is: %d\n",timenum);
    
    circlearcinterpolation(circlearc,circlearctrajp,time,targetp);
    orientationinterpolation(orientation,orientationtrajp,time,targetR);
    // inverse kinematics
    //T_from_RP(targetR,targetp,&targetT);
    LBR_ik(&targetT,parame,&solutionnum,inversesolutions);
    if(!solutionnum)
    {
        printf("not reachable point\n");
        return;
    }
    // find closet configuration
    closestq(inversesolutions,solutionnum,currentq,result);
}

void Kinematics::orientationinterpolation(const OrientationGeometry *orientation, const float coefficient[], const float t, float r[3][3])
{
    float s = fiveordertrajectoryscale(coefficient,t); // theta
    float temp[3][3];
    R_from_axis_angle(orientation->AA.axis,s,temp);
    R_mul_R(orientation->R0,temp,r);
}

float Kinematics::fiveordertrajectoryscale(const float coefficient[6], const float t)
{
    float result = coefficient[0];
    for(unsigned int i=1;i<6;++i)
    {
        result = result*t + coefficient[i];
    }
    return result;
}

void Kinematics::circlearcinterpolation(const CircleArcGeometry *circlearc, const STrajectoryParameter *trajp, const float t, float p[3])
{
    
    // -- calculate scale corresponding to t --              s(t)
    float s = strajectoryscale(trajp,t);
    // -- calculate point onlth line corresponding to s --   p = center + radius*(u*cos(s)+v*sin(s))
    float uc = circlearc->radius*cos(s),
            vc = circlearc->radius*sin(s),
            temp1[3],temp2[3],temp3[3];
    V_mul_S(circlearc->u,uc,temp1,3);
    V_mul_S(circlearc->v,vc,temp2,3);
    V_add_V(temp1,temp2,temp3,3);
    V_add_V(circlearc->center,temp3,p,3);
}

void Kinematics::DH2T(const float theta, const float d, const float a, const float alpha, QMatrix4x4 &matrix)
{
    float ct=cos(theta);
    float ca=cos(alpha);
    float st=sin(theta);
    float sa=sin(alpha);
    matrix.setToIdentity();
    matrix(0,0)=ct;
    matrix(0,1)=-st*ca;
    matrix(0,2)=st*sa;
    matrix(0,3)=a*ct;
    matrix(1,0)=st;
    matrix(1,1)=ct*ca;
    matrix(1,2)=-ct*sa;
    matrix(1,3)=a*st;
    matrix(2,0)=0;
    matrix(2,1)=sa;
    matrix(2,2)=ca;
    matrix(2,3)=d;
}

unsigned char Kinematics::phase_shift_eq(float sin_cos_pairs[2][2], const float a, const float b, const float c)
{
    float denominator = a*a+b*b;
    float delta = denominator-c*c;
    if(fabs(delta)<1e-5)
    {
        float sinx = a*c/denominator;
        float cosx = b*c/denominator;
        sincosround(&sinx);
        sincosround(&cosx);
        sin_cos_pairs[0][0] = sinx;
        sin_cos_pairs[0][1] = cosx;
        //printf("one solution\n");
        return 1;
    }
    
    if(delta<0)
    {
        if(fabs(a-c)<1e-5)
        {
            sin_cos_pairs[0][0] = 1;
            sin_cos_pairs[0][1] = 0;
            return 1;
        }
        if(fabs(a+c)<1e-5)
        {
            sin_cos_pairs[0][0] = -1;
            sin_cos_pairs[0][1] = 0;
            return 1;
        }
        if(fabs(b-c)<1e-5)
        {
            sin_cos_pairs[0][0] = 0;
            sin_cos_pairs[0][1] = 1;
            return 1;
        }
        if(fabs(b+c)<1e-5)
        {
            sin_cos_pairs[0][0] = 0;
            sin_cos_pairs[0][1] = -1;
            return 1;
        }
        //printf("no solution\n");
        return 0;
    }
    
    float sqrt_delta = sqrt(delta);
    float sinx1,cosx1,sinx2,cosx2;
    sinx1 = (a*c+b*sqrt_delta)/denominator;
    cosx1 = (b*c-a*sqrt_delta)/denominator;
    sinx2 = (a*c-b*sqrt_delta)/denominator;
    cosx2 = (b*c+a*sqrt_delta)/denominator;
    sincosround(&sinx1); sincosround(&cosx1); sincosround(&sinx2); sincosround(&cosx2);
    sin_cos_pairs[0][0] = sinx1;
    sin_cos_pairs[0][1] = cosx1;
    sin_cos_pairs[1][0] = sinx2;
    sin_cos_pairs[1][1] = cosx2;
    //printf("two solution\n");
    return 2;
}

bool Kinematics::closestq(const float inversesolutions[8][6], const unsigned int solutionnum, const float currentq[6], float closestsolution[6])
{
    if(solutionnum==0) return false;
    unsigned int minindex = 0;
    float currentdistance = caldistance(inversesolutions[0],currentq);
    float distance = currentdistance;
    for(unsigned int i=1;i<solutionnum;++i)
    {
        if(!isatruange(inversesolutions[i]))
            continue;
        currentdistance = caldistance(inversesolutions[i],currentq);
        if(currentdistance < distance)
        {
            distance = currentdistance;
            minindex = i;
        }
        
    }
    
    //    if(distance>5)
    //    {
    //        return false;
    //    }
    for(int i=0;i<DOF;++i)
    {
        closestsolution[i]=inversesolutions[minindex][i];
        if(abs(closestsolution[i]-currentq[i])>360-offset && abs(closestsolution[i]-currentq[i])<360+offset)
        {
            if(currentq[i]>0)
                closestsolution[i]+=360;
            if(currentq[i]<0)
                closestsolution[i]-=360;
            //可添加关节限位；
        }
        
    }
    return true;
}

bool Kinematics::closestq(const float inversesolutions[8][6], const unsigned int solutionnum, const QVector<float> currentq, QVector<float> &solution)
{
    float tqarray[6],solutionarray[6];
    for(int i=0;i<currentq.size();++i)
    {
        tqarray[i]=currentq.at(i);
    }
    if(closestq(inversesolutions,solutionnum,tqarray,solutionarray))
    {
        for(int i=0;i<DOF;++i)
        {
            solution.push_back(solutionarray[i]);
        }
        return true;
    }
    return false;
    
}

float Kinematics::caldistance(const float Q1[], const float Q2[])
{
    float delta = fabs(Q1[0]-Q2[0]);
    //    if(delta>=360)
    //        delta-=360;
    float d = delta;
    
    for(unsigned int i=1;i<DOF;++i)
    {
        delta = fabs(Q1[i]-Q2[i]);
        //        if(delta>=360)
        //            delta-=360;
        //        if(delta>d)
        //        {
        //            d = delta;
        //        }
        d+=delta;
    }
    return d;
}

void Kinematics::detjacobianinfo(const float Q[6], bool *isclosetozero, int *sign)
{
    *isclosetozero = false;
    float q2 = Q[1]*M_PI/180,q3 = Q[2]*M_PI/180, q5 = Q[4]*M_PI/180;
    float factor2 = sin(q5),
            factor3 = ((-2*cos(q2)*a3*d4+(-a3*a3+d4*d4)*sin(q2))*pow(cos(q3),2)+(((-a3*a3+d4*d4)*sin(q3)-a3*a2)\
                                                                                 *cos(q2)-a3*(-2*sin(q2)*sin(q3)*d4+a1))*cos(q3)+((a2*sin(q3)+a3)*cos(q2)+a1*sin(q3)-sin(q2)*d4)*d4);
    if(fabs(factor2)<1e-5 || fabs(factor3)<1e-5)
    {
        *isclosetozero = true;
    }
    int tempsign = -1;
    if(factor2 < 0)
    {
        tempsign *= -1;
    }
    if(factor3 < 0)
    {
        tempsign *= -1;
    }
    *sign = tempsign;
}
