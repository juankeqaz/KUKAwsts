#ifndef PLANNER_H
#define PLANNER_H

#include <QObject>
#include <QMap>
#include <QMenu>
#include <QVector3D>
typedef enum ED_TYPE{
    edgeline_L,
    edgeline_R,
    scanpath
}edgetpey;
typedef enum LineShape{
    point,
    line,
    arc,
    spline
}lineshape;
typedef struct LineData{
  QList<QPair<lineshape,QVector<QVector3D> > > subline; 
  float length;
}linedata; 

class planner:public QObject
{
    Q_OBJECT
public:
    planner();
    planner(QMap<ED_TYPE,LineData> linepath);
    void getpath(ED_TYPE type,LineData linepath);
    void update();
    
signals:
    void erroroccous(QString errorstring);
    
private:
    void clear();
    void getlength(linedata &ld);
    float getarclength(QVector3D p0, QVector3D p1, QVector3D p2);
public:
    QMap<ED_TYPE,LineData> _pathshapeline;
    
private:
    linedata edge1;
    linedata edge2;
    linedata lines;
};

#endif // PLANNER_H
