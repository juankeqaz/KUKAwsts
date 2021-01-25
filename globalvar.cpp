#include "globalvar.h"



bool isrobotconnected=false;
bool isUIstraconnected=false;
bool hasmodelshape=false;
bool ispromodelbuild=false;
bool isUItrasonicsetted=false;
bool ispathready=false;
QVector3D orgpoint={0,0,0};
QVector3D toolx={1,0,0};
QVector3D tooly={0,1,0};
QVector3D toolz={0,0,1};
QVector<float> homeposition={0,-90,90,0,0,0};
QVector<float> jointposition={0,-90,90,0,0,0};
QVector<float> spaceposition={0,0,0,0,0,0};
QVector<float> currentshapeparame={};
QMatrix4x4 workpicematrix={};
QMatrix4x4 plantmatrix={};
int currentshapetype=0;
int scannersize=2;


globalvar::globalvar()
{

}
