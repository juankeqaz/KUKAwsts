#ifndef PATHPLAN_H
#define PATHPLAN_H

#include <QObject>
#include <QVector3D>
#include "globalvar.h"
#include <QTimer>
#include "kinematics.h"


enum MOVEMENT_MODE{
    COMMON_MODE,
    LINE_MODE,
    ARC_MODE
};
typedef struct _FiveOrderPolyCoefficients
{
    float data[6];
} FiveOrderPolyCoefficients;
typedef  struct _StrajectoryParameter
{
    float v,a,omega;
    float t1,t2,T;
    float s1,s2;
}STrajectoryParameter;

typedef struct _AxisAngle
{
    float axis[3];
    float angle;
}AxisAngle;
typedef  struct _OrientationGeometry
{
    float R0[3][3];
    AxisAngle AA;
}OrientationGeometry;
typedef struct _LineGeometry
{
    float startpoint[3];
    float direction[3];
    float length;
}LineGeometry;




float polynomialvalue(const unsigned int order,const float *coefficients,const float t)
{
    float value=coefficients[0];
    for(unsigned int i=0;i<order;++i)
    {
        value=value*t+coefficients[i+1];
    }
    return value;
}

bool isgreaterthanzero(const float value)
{
    return value>0?true:false;
}



class PathPlan:public QObject
{
    Q_OBJECT
public:
    PathPlan();
    ~PathPlan();
public slots:
    void setpathpoint(QVector<QVector3D> point);

    void setpathpoint(float width,float height,float length);

    void setpathpoint(QVector3D start,QVector3D end);


    void sendVirtualJointPosition();
private:
    void getcommonmodeposition();
    void atendpoint();
    float cal_position(const FiveOrderPolyCoefficients *coefficients,const float t);
    void calculat_next_lq_point(const LineGeometry *line,const OrientationGeometry *oritentation,const STrajectoryParameter *linetrajp,\
                                const float orientationtrajp[6],const float time,const float currentq[6],float result[6]);
signals:
    void currentcommandsignal();
private:
    QTimer* simulationTimer;
    int movement_mode;
    float currenttimepoint;
    float speedrate;
    float time_step;
    float tmax;
    bool isatendpoint;

    unsigned int waypointsnum;
    float *timepoints;

    FiveOrderPolyCoefficients** allcoefficients;
    QVector<float> parame;

    STrajectoryParameter *linetrajp;
    OrientationGeometry *orientation;
    LineGeometry *line;
    float orientationtrajp[6];

};

#endif // PATHPLAN_H
