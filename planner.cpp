#include "planner.h"
#include <QDebug>

planner::planner()
{
    _pathshapeline.clear();
}

planner::planner(QMap<ED_TYPE, LineData> linepath)
{
    clear();
    _pathshapeline=linepath;
    if(_pathshapeline.contains(edgeline_L))
        edge1=_pathshapeline[edgeline_L];
    if(_pathshapeline.contains(edgeline_R))
        edge2=_pathshapeline[edgeline_R];
    if(_pathshapeline.contains(scanpath))
        lines=_pathshapeline[scanpath];
    
}

void planner::getpath(ED_TYPE type, LineData linepath)
{
    if(_pathshapeline.contains(type))
    {
        for(int i=0;i<linepath.subline.size();++i)
        {
            _pathshapeline[type].subline.push_back(linepath.subline.at(i));
        }
    }
    else
        _pathshapeline[type]=linepath;
}

void planner::update()
{
    if(edge1.subline.size()!=0)
    {
        
    }
}

void planner::clear()
{
    edge1.subline.clear();
    edge1.length=0;
    edge2.subline.clear();
    edge2.length=0;
    lines.subline.clear();
    lines.length=0;
    _pathshapeline.clear();
}

void planner::getlength(linedata &ld)
{
    float dist=0;
    QList<QPair<lineshape,QVector<QVector3D> > > list=ld.subline;
    if(list.size()==0)
        ld.length=0;
    else
    {
        for(int i=0;i<list.size();++i)
        {
            QPair<lineshape,QVector<QVector3D> > pair=list.at(i);
            switch (pair.first) {
            case point:
            {
               dist+=0; 
            }
                break;
            case line:
            {
                QVector<QVector3D> linedate=pair.second;
                if(linedate.size()==0)
                    dist+=0;
                QVector3D start=linedate.first(),end;
                for(int j=1;j<linedate.size();++j)
                {
                    end=linedate.at(j);
                    dist+=start.distanceToPoint(end);
                    start=end;
                }
            }
                break;
            case arc:
            {
                QVector<QVector3D> arcdate=pair.second;
                if((arcdate.size()-1)%2 != 0)
                {
                    qDebug()<<"error about arc point count;";
                    emit erroroccous("error about arc point count;");
                    dist+=0;
                    return;
                }
                QVector3D start=arcdate.first(),pass,end;
                for(int j=1;j<arcdate.size();j+=2)
                {
                    pass=arcdate.at(j);
                    end=arcdate.at(j+1);
                    dist+=getarclength(start,pass,end);
                    start=end;
                }
            }
                break;
            case spline:
            {
                
            }
                break;
            default:
                break;
            
            }
        }
    }
    
}

float planner::getarclength(QVector3D p0, QVector3D p1, QVector3D p2)
{
    float a1, b1, c1, d1;
    float a2, b2, c2, d2;
    float a3, b3, c3, d3, d4;

    float x1 = p0[0], y1 = p0[1], z1 = p0[2];
    float x2 = p1[0], y2 = p1[1], z2 = p1[2];
    float x3 = p2[0], y3 = p2[1], z3 = p2[2];

    a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
    b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
    c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

    a2 = 2 * (x2 - x1);
    b2 = 2 * (y2 - y1);
    c2 = 2 * (z2 - z1);
    d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

    a3 = 2 * (x3 - x1);
    b3 = 2 * (y3 - y1);
    c3 = 2 * (z3 - z1);
    d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

    d4 = 1/(a1*(b2*c3 - b3*c2) - a2*(b1*c3 - b3*c1) + a3*(b1*c2 - b2*c1));
    //圆心
    float A[3],B[3],C[3];
    A[0]=x1-x2;
    A[1]=y1-y2;
    A[2]=z1-z2;
    B[0]=x1-x3;
    B[1]=y1-y3;
    B[2]=z1-z3;
    C[0]=A[1]*B[2]-A[2]*B[1];
    C[1]=A[2]*B[0]-A[0]*B[2];
    C[2]=A[0]*B[1]-A[1]*B[0];
    double dc=sqrt(C[0]*C[0]+C[1]*C[1]+C[2]*C[2]);
    if(dc<10e-6)
    {
        emit erroroccous("three points art coliner");
        qDebug()<<"three points art coliner\n";
        return 0;
    }
    double o0 = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)*d4;
    double o1 =  (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)*d4;
    double o2 = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)*d4;
    double r0 = sqrt((x1-o0)*(x1-o0)+(y1-o1)*(y1-o1)+(z1-o2)*(z1-o2));//半径
    double cit1=(x3-x1)*(x3-x1)+(y3-y1)*(y3-y1)+(z3-z1)*(z3-z1);//弦长的平方
    double cosct=1-cit1/(2*r0*r0);//圆心角余弦
    double cit0=acos(cosct);
    return cit0*r0;
}
