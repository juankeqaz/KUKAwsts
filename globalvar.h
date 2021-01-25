#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include <QVector3D>
#include <QVector>
#include <QMatrix4x4>

extern bool isrobotconnected;
extern bool isUIstraconnected;
extern bool hasmodelshape;
extern bool ispromodelbuild;
extern bool isUItrasonicsetted;
extern bool ispathready;
extern int currentshapetype;
extern QVector3D orgpoint;
extern QVector3D toolx;
extern QVector3D tooly;
extern QVector3D toolz;
extern QMatrix4x4 plantmatrix;
extern QMatrix4x4 workpicematrix;

extern int scannersize;

extern QVector<float> homeposition;
extern QVector<float> jointposition;
extern QVector<float> spaceposition;


extern QVector<float> currentshapeparame;

typedef struct Robot_Path{
    QString type;
    QVector<QVector3D> point;
    QVector<QVector3D> norml;
    QVector<QVector<float> > joint;
    float length;
    bool runflag=false;
    bool isneedscan=false;
    

}robotpath;



class globalvar
{
public:
    globalvar();
};

#endif // GLOBALVAR_H
