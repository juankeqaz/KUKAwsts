#include "pathplan.h"

PathPlan::PathPlan()
{
    simulationTimer=new QTimer(this);
    connect(simulationTimer,SIGNAL(timeout()),this,SLOT(sendVirtualJointPosition()));

    movement_mode=0;
    speedrate=1;
    //rotstep=8;
    //isjointrot=false;
    time_step = 0.01;
    currenttimepoint = 0;

    isatendpoint=false;

    parame<<0<<-90<<90<<0<<0<<0;

    linetrajp=new STrajectoryParameter;
    orientation=new OrientationGeometry;
    line=new LineGeometry;


}

PathPlan::~PathPlan()
{
    free(linetrajp);
    free(orientation);
}

void PathPlan::setpathpoint(QVector<QVector3D> point)
{

}

void PathPlan::setpathpoint(float width, float height, float length)
{

}

void PathPlan::setpathpoint(QVector3D start, QVector3D end)
{

}

void PathPlan::sendVirtualJointPosition()
{
    switch (movement_mode)
    {
    case 0:
        getcommonmodeposition();
        break;
    case 1:
    {
        currenttimepoint += speedrate*time_step;
        if(currenttimepoint >= linetrajp->T)
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
//    case 2:
//    {
//        currenttimepoint += speedrate*time_step;
//        if(currenttimepoint>=circlearctrajp->T)
//        {
//            currenttimepoint = circlearctrajp->T;
//            isatendpoint = true;
//        }
//        float tempparam[6];
//        VTYPE currentq[6];
//        for(unsigned int i=0;i<6;++i)
//        {
//            currentq[i] = parame.at(i);
//        }
//        caculate_next_arc_point(circlearc,arc_orientation,circlearctrajp,orientationtrajp,currenttimepoint,currentq,tempparam);
//        for(int i=0;i<6;++i)
//        {
//            parame[i]=tempparam[i];
//        }
//    }
//        break;
//    default:
//        break;
    }
//    if(isatendpoint)
//    {
//        //stoprun();
//        atendpoint();
//        currenttimepoint =0;
//        isatendpoint=false;
//        Q_EMIT currentcommandsignal();
//    }
//    getspacecurpostions();

//    Q_EMIT dataforsimulate(parame);
}

void PathPlan::getcommonmodeposition()
{
    QVector<float> currentJointPosition;
    currenttimepoint += speedrate*time_step;//当前时间点，即时间步长累加
    for(unsigned int j=0;j<6;++j)
    {
        unsigned int index = 0;
        FiveOrderPolyCoefficients* coefficients;
        if((tmax-currenttimepoint)<=0)
        {
//            if(isteachway)
//                runteachwaypoint();
//            else
//            {
                currenttimepoint=tmax;
                isatendpoint=true;
                //stoprun();
                atendpoint();
//            }
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
    if(currentJointPosition.size() < DOF)
        return;
    else
    {
        parame=currentJointPosition;
    }
}

void PathPlan::atendpoint()
{
    if(simulationTimer->isActive())
        simulationTimer->stop();
}

float PathPlan::cal_position(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    return  polynomialvalue(5,coefficients->data,t);
}

void PathPlan::calculat_next_lq_point(const LineGeometry *line, const OrientationGeometry *oritentation, const STrajectoryParameter *linetrajp, const float orientationtrajp[], const float time, const float currentq[], float result[])
{

}
