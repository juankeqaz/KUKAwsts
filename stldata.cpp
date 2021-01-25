#include "stldata.h"
#include <QDebug>
#include <QTimer>
#include "programloader.h"

stldata::stldata(QObject *parent):
    QObject(parent)
{
    jointVariables={0,1,2,3,4,5};
    jointVelocityLimits<<20<<20<<20<<20<<20<<20;
    parame<<0<<90<<0<<90<<0<<0;
    curparame=parame;
    stepnum=0;
    isstep=true;
    speedrate=1;
    rotstep=8;
    isjointrot=false;
    time_step = 0.01;
    qRegisterMetaType<QVector<float> >("QVector<float>");
    simulationTimer = new QTimer(this);
    connect(simulationTimer,SIGNAL(timeout()),this,SLOT(sendVirtualJointPosition()));

    currenttimepoint = 0;
    positiontmodelTimer=new QTimer(this);
    //positiontmodelTimer->setSingleShot(true);
    connect(positiontmodelTimer,SIGNAL(timeout()),this,SLOT(positionmodel()));
    speedmodelTimer=new QTimer(this);
    connect(speedmodelTimer,SIGNAL(timeout()),this,SLOT(speedmodel()));

    spacevec={0,0,0,0,0,0};

    //QVector<float> paramet={0.119,0.49969,0.39602,-0.14257,0.11243,0.058};//三代机械臂

    //QVector<float> paramet={0.119,0.38,0.33,-0.143,0.111,0.058};//四代机械臂
    float spacearray[6];
    cacul=new KInematics_Interface;
    cacul->fKCal(parame,spacearray);
    for(int i=0;i<6;++i)
    {
        spacevec[i]=*(spacearray+i);
    }
    connect(cacul,SIGNAL(ikresults(bool,QVector<float>)),this,SLOT(resultofcacul(bool,QVector<float>)));
    //connect(this,SIGNAL(calcujointparam(QVector<float>,QVector<float>)),cacul,SLOT(IKCal(QVector<float>,QVector<float>)));
    //connect(cacul,SIGNAL(IKResults(bool,QVector<float>)),this,SLOT(resultofcacul(bool,QVector<float>)));

    lbrupdatetimer = new QTimer(this);
    connect(lbrupdatetimer,SIGNAL(timeout()),this,SLOT(vectortoparam()));
    movement_mode=0;
    is_need_calculat_coefficient=false;
    issimulate=false;
    line= new LineGeometry;
    orientation=new OrientationGeometry;
    linetrajp=new STrajectoryParameter;

    circlearc=new CircleArcGeometry;
    arc_orientation=new OrientationGeometry;
    circlearctrajp=new STrajectoryParameter;
    isdrwn=false;
    isteachway=false;
    isspeedmodel=true;
    waittime=new QTimer(this);
    waittime->setSingleShot(true);
    connect(waittime,SIGNAL(timeout()),this,SLOT(timeoutslots()));
    getspacecurpostions();
    runfunc=false;
    funflag=-1;
}
stldata::~stldata()
{
    cleargeometrymetadataset();
    free(line);
    free(linetrajp);
    free(orientation);
    cacul->deleteLater();
}

void stldata::getjointparam(QVector<float> paramet)
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
    //    if(!isjointrot)
    //Q_EMIT getjointparamfilishsignal();
}

void stldata::getjointparams(std::vector<std::vector<float> > wayline)
{
    isteachway=true;
    teachpoint=wayline;
    curpoint=0;
    runteachwaypoint();
}

void stldata::vectortoparam()
{
    //lock.lockForRead();
    for(int i=0;i<parame.size();++i)
    {
        parame[i]=position[i];
    }
    //lock.unlock();

    //updateGL();
}

bool stldata::isactualrun()
{
    if(lbrupdatetimer->isActive())
        return true;
    else
        return false;
}

void stldata::movelq(QVector<float> end)
{
    //qDebug()<<"movelq is run";
    VTYPE Q1[6];
    for(int i=0;i<6;++i)
    {
        startq[i]=(VTYPE)parame.at(i);
        Q1[i]=(VTYPE)end.at(i);
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
        Q_EMIT positionnotreachable();

}

void stldata::movecq(QVector<float> pass, QVector<float> end, int flag)
{
    VTYPE Q1[6],Q2[6];
    bool flg;
    for(int i=0;i<6;++i)
    {
        startq[i]=(VTYPE)parame.at(i);
        Q2[i]=(VTYPE)pass.at(i);
        Q1[i]=(VTYPE)end.at(i);
    }
    if(flag==0)
        flg=false;
    else
        flg=true;
    if(iscirclearcfeasible(startq,Q2,Q1,ARC_V,ARC_A,flg))
    {
        //qDebug()<<"iscirclearcfeasible is true";
        calculate_arc_Geometry(startq,Q2,Q1,flg,circlearc,arc_orientation);
        caculate_arc_Trajectory(circlearc,arc_orientation,ARC_V,ARC_A,circlearctrajp,orientationtrajp);
        //qDebug()<<"total time is"<<circlearctrajp->T;
        movement_mode=2;
        GeometryMetaData geometrymetadata;
        CircleArcGeometry* temp = new CircleArcGeometry;
        V_assign(circlearc->center,temp->center,3);
        temp->maxtheta = circlearc->maxtheta;
        temp->radius = circlearc->radius;
        V_assign(circlearc->u,temp->u,3);
        V_assign(circlearc->v,temp->v,3);

        geometrymetadata.shapegeometry = (void*)temp;
        geometrymetadata.shape = CIRCLE;
        geometrymatadataset.push_back(geometrymetadata);

        isatendpoint = false;
        //Q_EMIT getjointparamfilishsignal();
        startrun();
    }else
    {
        //qDebug()<<"not reachable";
        Q_EMIT positionnotreachable();
    }
}

void stldata::trajectory_display()
{
    if(isdrwn)
        isdrwn=false;
    else
        isdrwn=true;
    //updateGL();
}

void stldata::changerobstatus(int p)
{
    if(p==200)
        issimulate=true;
    else if(p==201)
        issimulate=false;
    else
        return;
}

void stldata::positionmodel()
{
    int step;
    int t=0;
    switch (moveflag) {
    case 1:
        step=10;
        t=0;
        spacevec[t] += step;
        break;
    case 2:
        step=-10;
        t=0;
        spacevec[t] += step;
        break;
    case 3:
        step=10;
        t=1;
        spacevec[t] += step;
        break;
    case 4:
        step=-10;
        t=1;
        spacevec[t] += step;
        break;
    case 5:
        step=10;
        t=2;
        spacevec[t] += step;
        break;
    case 6:
        step=-10;
        t=2;
        spacevec[t] += step;
        break;
    case 7:
        step=1;
        t=3;
        spacevec[t] += step*0.3;
        break;
    case 8:
        step=-1;
        t=3;
        spacevec[t] += step*0.3;
        break;
    case 9:
        step=1;
        t=4;
        spacevec[t] += step*0.3;
        break;
    case 10:
        step=-1;
        t=4;
        spacevec[t] += step*0.3;
        break;
    case 11:
        step=1;
        t=5;
        spacevec[t] += step*0.3;
        break;
    case 12:
        step=-1;
        t=5;
        spacevec[t] += step*0.3;
        break;
    default:
        break;
    }
    QVector<float> currvec;
    currvec.push_back(parame.at(0));
    currvec.push_back(parame.at(1));
    currvec.push_back(parame.at(2));
    currvec.push_back(parame.at(3));
    currvec.push_back(parame.at(4));
    currvec.push_back(parame.at(5));
    //qDebug()<<"spacevec is"<<spacevec.at(0)<<spacevec.at(1)<<spacevec.at(2)<<spacevec.at(3)<<spacevec.at(4)<<spacevec.at(5);
    //qDebug()<<"parame is"<<parame.at(0)<<parame.at(1)<<parame.at(2)<<parame.at(3)<<parame.at(4)<<parame.at(5);
    cacul->IKCal(currvec,spacevec);
}

void stldata::speedmodel()
{
    switch (speedmodelflag) {
    case 1:
        //currvec[0]+=1*speedrate;
        parame[0]+=1*speedrate;
        break;
    case 2:
        //currvec[0]-=1*speedrate;
        parame[0]-=1*speedrate;
        break;
    case 3:
        //currvec[1]+=1*speedrate;
        parame[1]+=1*speedrate;
        break;
    case 4:
        //currvec[1]-=1*speedrate;
        parame[1]-=1*speedrate;
        break;
    case 5:
        //currvec[2]+=1*speedrate;
        parame[2]+=1*speedrate;

        break;
    case 6:
        //currvec[2]-=1*speedrate;
        parame[2]-=1*speedrate;
        break;
    case 7:
        //currvec[3]+=1*speedrate;
        parame[3]+=1*speedrate;
        break;
    case 8:
        //currvec[3]-=1*speedrate;
        parame[3]-=1*speedrate;
        break;
    case 9:
        //currvec[4]+=1*speedrate;
        parame[4]+=1*speedrate;
        break;
    case 10:
        //currvec[4]-=1*speedrate;
        parame[4]-=1*speedrate;
        break;
    case 11:
        //currvec[5]+=1*speedrate;
        parame[5]+=1*speedrate;
        break;
    case 12:
        //currvec[5]-=1*speedrate;
        parame[5]-=1*speedrate;
        break;
    default:
        break;
    }
    //parame=currvec;
    getspacecurpostions();
    //updateGL();
    //vectortoparam(currvec);
    Q_EMIT dataforsimulate(parame);
}

void stldata::actualrun(QVector<float> pos)
{
    //lbrupdatetimer->start(87);
    parame=pos;
    //updateGL();
}

void stldata::getmoveflag(int flag)//获取位置模式运动标识
{

    isspeedmodel=false;
    moveflag=flag;
    spacevec.clear();
    for(int i=0;i<6;++i)
    {
        spacevec.push_back(simspaceposition[i]);
    }
    //qDebug()<<"spacevec is"<<spacevec.at(0)<<spacevec.at(1)<<spacevec.at(2)<<spacevec.at(3)<<spacevec.at(4)<<spacevec.at(5);
    positiontmodelTimer->start(100);
}

void stldata::getspeedmodelflag(int flag)
{
    //qDebug()<<"simulation speed model is run!";
    isspeedmodel=true;
    speedmodelflag=flag;
    speedmodelTimer->start(10);
}
void stldata::translate()
{
    jointparavariables.clear();
    //waypoint.push_back(parame.getvectorparame());
    unsigned int index1 = waypoint.at(0).size();
    unsigned int index2 = waypoint.size();
    for(unsigned int i=0;i<index1;++i)
    {
        std::vector<float> vec;
        for(unsigned int j=0;j<index2;++j)
        {
            vec.push_back(waypoint[j][i]);
        }
        jointparavariables.push_back(vec);
    }
    //cyclerun();
    //qDebug()<<jointparavariables[0][0]<<jointparavariables[0][1];
}

void stldata::stoprun()
{
    stopflag=true;
    isjointrot=false;
    if(simulationTimer->isActive())
        simulationTimer->stop();
    if(positiontmodelTimer->isActive())
        positiontmodelTimer->stop();
    if(speedmodelTimer->isActive())
        speedmodelTimer->stop();
    if(lbrupdatetimer->isActive())
        lbrupdatetimer->stop();

}

void stldata::startrun()
{
    //qDebug()<<"simulation is start";
    stopflag=false;
    if(simulationTimer->isActive())
        simulationTimer->stop();

    simulationTimer->start(time_step*1000);
}

void stldata::getcurpostions()
{
    lock.lockForWrite();
    for(int i=0;i<6;++i)
    {
        simposition[i]=curparame[i];
    }
    lock.unlock();
}

void stldata::getspacecurpostions()
{
    curparame=parame;
    float tempvec[6];
//    tempvec=(float*)malloc(6);
        cacul->fKCal(curparame,tempvec);
        lock.lockForWrite();
        for(int i=0;i<6;++i)
        {
            simposition[i]=parame.at(i);
//            if(i<3)
            simspaceposition[i]=tempvec[i];
//            else
//                simspaceposition[i]=tempvec[i]*RAD2DEG;
        }
        lock.unlock();
}

void stldata::changerobparamet(QStringList paramelist)
{
    QString str;
    QStringList list;
    VTYPE p[6];
    for(int i=0;i<paramelist.size();++i)
    {
        str=paramelist.at(i);
        if(str.startsWith("S_paramet"))
        {
            list=str.split(" ");
            break;
        }
    }
    if(list.size()!=7) return;

    for(int j=1;j<list.size();++j)
    {
        str=list.at(j);
        p[j-1]=str.toFloat();
    }
    int t=changeparame(p);
    qDebug()<<"set parame success"<<t;
}

void stldata::virtualrobcommand(QStringList paramlist, int p)
{
    QVector<float> paramvec;
    for(int i=0;i<paramlist.size();++i)
    {
        paramvec.push_back(QString(paramlist.at(i)).toFloat());
    }
    funflag=p;
    switch (p) {
    case 0:
        if(paramvec.size()==6)
            getjointparam(paramvec);
        break;
    case 1:
        if(paramvec.size()==6)
        {
            runfunc=true;
            getposparaminfor(paramvec);
        }
        break;
    case 2:
        if(paramvec.size()==2)
        {
            int port=(int)paramvec.at(0);
            int st=(int)paramvec.at(1);
            Q_EMIT setiostatusignal(port,st);
            waittime->start(1000);
        }
        break;
    case 3:
        if(paramvec.size()==3)
        {
            int t= (int)paramvec.at(0);
            float speed= paramvec.at(1);
            int timelength=(int)paramvec.at(2);
            speedrotateslot(t,speed,timelength);
        }
        break;
    case 4:
        gotohomeslot();
        break;
    case 5:
        if(paramvec.size()==1)
        {
            int t= paramvec.at(0);
            waittime->start(t);
        }
        break;
    case 6:
        if(paramvec.size()==2)
        {
            int t=(int)paramvec.at(0);
            float p=paramvec.at(1);
            jointrotateslot(t,p);
        }
        break;
    case 7:
        if(paramvec.size()==2)
        {
            int t=(int)paramvec.at(0);
            float p = paramvec.at(1);
            jointmovtoangleslot(t,p);
        }
        break;
    case 8:
        if(paramvec.size()==6)
        {
            movelq(paramvec);
        }
        break;
    case 9:
        if(paramvec.size()==13)
        {
            QVector<float> pass;
            QVector<float> end;
            for(int i=0;i<paramvec.size()-1;++i)
            {
                if(i<6)
                    pass.push_back(paramvec.at(i));
                else
                    end.push_back(paramvec.at(i));
            }
            int flag=(int)paramvec.at(12);
            movecq(pass,end,flag);
        }
        break;
    case 10:
        if(paramvec.size()==6)
        {
            runfunc=true;
            getposparaminfor(paramvec);
        }
        break;
    case 11:
        if(paramvec.size()==13)
        {
            QVector<float> pass;
            QVector<float> end;
            for(int i=0;i<paramvec.size()-1;++i)
            {
                if(i<6)
                    pass.push_back(paramvec.at(i));
                else
                    end.push_back(paramvec.at(i));
            }
            int flag=(int)paramvec.at(12);
            movecp(pass,end,flag);
        }
        break;
    default:
        break;
    }
}

void stldata::timeoutslots()
{
    if(!stopflag)
    {
        Q_EMIT currentcommandsignal();
    }
}

void stldata::atendpoint()
{
    isjointrot=false;
    if(simulationTimer->isActive())
        simulationTimer->stop();
    if(positiontmodelTimer->isActive())
        positiontmodelTimer->stop();
    if(speedmodelTimer->isActive())
        speedmodelTimer->stop();
    if(lbrupdatetimer->isActive())
        lbrupdatetimer->stop();

}

void stldata::movecp(QVector<float> pass, QVector<float> end, int flag)
{
    QVector<float> jpass,jend;
    if(cacul->getjointpos(pass,parame,&jpass))
    {
//        for(int i=0;i<jpass.size();++i)
//        {
//            qDebug()<<"pass joint value is"<<jpass.at(i);
//        }
        if(cacul->getjointpos(end,jpass,&jend))
        {
//            for(int i=0;i<jend.size();++i)
//            {
//                qDebug()<<"end joint value is"<<jend.at(i);
//            }
        }else
            return;
    }else
        return;
    movecq(jpass,jend,flag);
}

void stldata::cyclerun()
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
    float velocitylimit[6] = {120,120,120,120,120,120};
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

void stldata::sendVirtualJointPosition()
{
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
        VTYPE currentq[6];
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
        VTYPE currentq[6];
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
        currenttimepoint =0;
        isatendpoint=false;
        Q_EMIT currentcommandsignal();
    }
    //curparame=parame;
    //getcurpostions();
    getspacecurpostions();
    //    lock.lockForWrite();
    //    for(int i=0;i<parame.size();++i)
    //    {
    //        simposition[i]=parame.at(i);
    //    }
    //    lock.unlock();

    Q_EMIT dataforsimulate(parame);
}

void stldata::getwaypoint()
{
    //qDebug()<<"waypoint="<<waypoint[1][0];
    QString position;
    if(waypoint.size()==0)
    {
        return;
        //QMessageBox::critical(this, tr("Error"),"You need to set a position!");
    }else
    {
        for(unsigned int i=0;i<waypoint.size();i++)
        {
            for(unsigned int j=0;j<waypoint.at(i).size();j++)
            {
                position.append(QString::number(waypoint[i][j],10,6)).append(" ");
            }
            //position.append("\n");
            Q_EMIT sendwaypointsignal(position);
            position.clear();
        }
    }
}

void stldata::getspeedratevalue(int value)
{
    speedrate=value/100.0;
}

float stldata::timeDistance(const std::vector<float> &from, const std::vector<float> &to)
{
    float result = 0;
    for(unsigned i=0;i<from.size();++i)
    {
        result = qMax(result,qAbs((from.at(i)-to.at(i))/jointVelocityLimits.at(i)));
    }
    //round result unit 10ms
    result = ceil(result*100)/100;
    //qDebug()<<"speedrate is "<<speedrate;
    return result*1e6;

}
void stldata::calculateTimedPath(const std::vector<std::vector<float> > &path, std::vector<float> &time)
{
    //cal_timepoints(path,waypoints,waypoint.size(),jointVelocityLimits);

    time.clear();
    time.push_back(0);
    for(unsigned i=1;i<path.size();++i)
    {
        time.push_back(time.at(i-1)+timeDistance(path.at(i-1),path.at(i)));//maxtime
    }
}

void stldata::currentposition(QVector<float> paramet)
{
    int t=0;
    for(unsigned int i=1;i<waypoint.size();++i)
    {
        for(unsigned int j=0;j<waypoint.at(i).size();++j)
        {
            if(qAbs(paramet.at(j)-waypoint[i][j])<10e-4)
            {
                //simulationTimer->stop();
                //qDebug()<<paramet.getvectorparame().at(j);
                t++;
            }else {
                t=0;
            }
        }
        if(t==6)
        {
            //qDebug()<<"current i"<<i;
            if(isteachway)
                runteachwaypoint();
            else
            {
                //stoprun();
                atendpoint();
                //  qDebug()<<"222222222222222222222";
                Q_EMIT currentcommandsignal();
            }
        }
    }
}

void stldata::runteachwaypoint()//关节插值仿真
{
    if((unsigned int)curpoint<teachpoint.size())
    {
        //qDebug()<<"curpoint is"<<curpoint;
        waypoint.clear();
        currenttimepoint = 0;
        waypoint.push_back(parame.toStdVector());
        waypoint.push_back(teachpoint.at(curpoint));

        // qDebug()<<"parame is"<<parame.at(0)<<parame.at(1)<<parame.at(2)<<parame.at(3)<<parame.at(4)<<parame.at(5);
        // qDebug()<<"teachpoint is"<<teachpoint.at(curpoint).at(0)<<teachpoint.at(curpoint).at(1)<<teachpoint.at(curpoint).at(2)<<teachpoint.at(curpoint).at(3)\
        //        <<teachpoint.at(curpoint).at(4)<<teachpoint.at(curpoint).at(5);

        curpoint++;
        movement_mode=0;
        isatendpoint=false;
        //is_need_calculat_coefficient=true;
        cyclerun();
        //Q_EMIT getjointparamfilishsignal();
        startrun();
        //Q_EMIT getjointparamfilishsignal();
    }else
    {
        isteachway=false;
        curpoint=0;
        atendpoint();
        //stoprun();
        //Q_EMIT currentcommandsignal();
    }
}

void stldata::getcommonmodeposition()
{
    QVector<float> currentJointPosition;
    currenttimepoint += speedrate*time_step;//当前时间点，即时间步长累加
    for(unsigned int j=0;j<6;++j)
    {
        unsigned int index = 0;
        FiveOrderPolyCoefficients* coefficients;
        if((tmax-currenttimepoint)<=0)
        {
            if(isteachway)
                runteachwaypoint();
            else
            {
                currenttimepoint=tmax;
                isatendpoint=true;
                //stoprun();
                atendpoint();
            }
            //currenttimepoint = 0;
            //currenttimepoint = 0;
        }
        else
        {
            for(unsigned int i=0;i<waypointsnum-1;++i)
            {
                if(currenttimepoint<timepoints[i+1])
                {
                    index = i;
                    break;
                }
            }
            coefficients = &(allcoefficients[index][j]);//当前使用系数
            float t = currenttimepoint-timepoints[index];
            currentJointPosition.push_back(cal_position(coefficients,t));
        }
    }

    //qDebug()<<"___________________"<<currentJointPosition.size()<<jointVariables.size();
    if(currentJointPosition.size()<(int)jointVariables.size())
        return;
    else
    {
        parame=currentJointPosition;
    }
    //currentposition(parame);
    //updateGL();
}

QVector<float> stldata::checkjointvalue(QVector<float> para)
{
    QVector<float> newpara;
    newpara.push_back(checkdata(para.at(0)));
    newpara.push_back(checkdata(para.at(1)));
    newpara.push_back(checkdata(para.at(2)));
    newpara.push_back(checkdata(para.at(3)));
    newpara.push_back(checkdata(para.at(4)));
    newpara.push_back(checkdata(para.at(5)));
    return newpara;
}

void stldata::cleargeometrymetadataset()
{
    for(int i=0;i<geometrymatadataset.size();++i)
    {
        switch(geometrymatadataset.at(i).shape)
        {
        case LINE:
            delete (LineGeometry*)geometrymatadataset.at(i).shapegeometry;
            break;
        case CIRCLE:
            delete (CircleArcGeometry*)geometrymatadataset.at(i).shapegeometry;
            break;
        default:
            break;
        }
    }
    geometrymatadataset.clear();
}

void stldata::sendsimulatedate()
{
    float *pos;
    pos=(float *)malloc(6);
    cacul->fKCal(parame,pos);
    QVector<QVector<float> > crt;
    crt.push_back(parame);
    QVector<float> temp(6);
    for(int i=0;i<6;++i)
    {
        if(i<3)
            temp[i]=pos[i];
        else
            temp[i]=pos[i]*RAD2DEG;
    }
    crt.push_back(temp);
    Q_EMIT sendsimulateposition(crt);
}

void stldata::getposparaminfor(QVector<float> paramets)
{
    //std::vector<float> curvec=parame.toStdVector();
    QVector<float> currentvec;
    //currentvec.fromStdVector(curvec);
    for(int i=0;i<parame.size();++i)
    {
        currentvec.push_back(parame.at(i));
    }
    //qDebug()<<"currentvec is"<<currentvec.at(0)<<currentvec.at(1)<<currentvec.at(2)<<currentvec.at(3)<<currentvec.at(4)<<currentvec.at(5);
    QVector<float> spparam;
    for(int j=0;j<paramets.size();++j)
    {
        if(j<3)
            spparam.push_back(paramets.at(j));
        else
            spparam.push_back(paramets.at(j));
    }
    //qDebug()<<"paramets is"<<paramets.at(0)<<paramets.at(1)<<paramets.at(2)<<paramets.at(3)<<paramets.at(4)<<paramets.at(5);
    cacul->IKCal(currentvec,spparam);
}

void stldata::resultofcacul(bool isnon, QVector<float> result)
{
    //qDebug()<<"<<<<<<<<<<<<<<<<<<,,,,";
    //QVector<float> caculparam;
    if(isnon)
    {
        positiontmodelTimer->stop();
        Q_EMIT movepnoresult();
    }else
    {
        QVector<float> vec;
        for(int i=0;i<result.size();++i)
        {
            vec.push_back(result.at(i));
        }
        if(runfunc)
        {
            if(funflag==1)
            {
                getjointparam(vec);
            }
            if(funflag==10)
            {
                movelq(vec);
            }
          runfunc=false;
          return;
        }
        parame=vec;
        getspacecurpostions();
        Q_EMIT dataforsimulate(parame);
    }
}

void stldata::resultoffkcaculslot(const QVector<float> &vector)
{
    curspace.clear();
    for(int i=0;i<vector.size();++i)
    {
        curspace.push_back(vector.at(i));
        spaceposition[i]=vector.at(i);
    }
}

void stldata::jointrotateslot(int joint, float value)
{
    if(joint < 1)
        return;
    QVector<float> tempparam;
    for(int i=0;i<curparame.size();++i)
    {
        if(i==(joint-1))
            tempparam.push_back(curparame.at(i)+value);
        else
            tempparam.push_back(curparame.at(i));
    }
    getjointparam(tempparam);
}

void stldata::gotohomeslot()
{
    QVector<float> homepos={0,90,0,90,0,0};
    isspeedmodel=true;
    getjointparam(homepos);
    //startrun();
}

void stldata::homeposition()
{
    QVector<float> hvec={0,90,0,90,0,0};
    parame=hvec;
    getspacecurpostions();
    //Q_EMIT dataforsimulate(parame);
}

void stldata::jointmovtoangleslot(int joint, float value)
{
    QVector<float> tempparam;
    for(int i=0;i<curparame.size();++i)
    {
        if(i==joint)
            tempparam.push_back(value);
        else
            tempparam.push_back(curparame.at(i));
    }
    getjointparam(tempparam);
}

void stldata::speedrotateslot(int joint, float speed, int timelength)
{
    int value=speed*timelength/1000;
    jointrotateslot(joint,value);
}
