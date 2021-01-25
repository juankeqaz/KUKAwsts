#ifndef STLMODULE_H
#define STLMODULE_H
#include <QtOpenGL>
#include <QGLWidget>
//#include "stldata.h"
//#include <glut.h>

#define LINE_V 0.5
#define LINE_A 10
#define ARC_V 2
#define ARC_A 20

//static QVector<float> paramet={0.119,0.49969,0.39602,-0.14257,0.11243,0.058};//三代机械臂

//static QVector<float> paramet={0.119,0.38,0.33,-0.143,0.111,0.058};//四代机械臂

class STLModule:public QGLWidget
{
    Q_OBJECT
public:
    explicit STLModule(QGLWidget *parent=0);
    ~STLModule();
    void paintModel(QVector<float> &jointVariables);

signals:
    void zoomvaluechanged(int value);
    void setiostatussignal(int port,int st);
    void simucommandsignal(QStringList paramlist,int p);
    void runnextcommandsignal();
    void positionnotreachable();
    void speedchanged();
public slots:
    void zoom(int value);
    void zoomIn();
    void zoomOut();
    void actualrun(QVector<float> pos);
    void upparamet(QStringList paramelist);
    void simucommand(QStringList paramlist,int p);
    void stopsimu();
    void speedmodel(int flag);
    void posmodel(int flag);
    void gohome();
    void changespeed(int value);
    QVector<float> getcurrentangle();

//    QVector<float> getspacecurpostions();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width,int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    void drawTriangle(const QVector<QVector<float> >& vec);
    void getSTLFile(const QString &stlFilePath);
    void getSTLData(const QString &stlFilePath,const QString &stlFileName);
    void genTex(const QString &texFilePath);
private:
    QPoint lastPos;
    GLfloat rotationX,rotationY,rotationZ;
    GLfloat translationX,translationY,translationZ;
    QString STLPath;
    QString TexPath;
    QVector<float> parame;
    QVector<float> curparame;
    QVector<QVector<float> > vectorsOfLink1_1;
    QVector<QVector<float> > vectorsOfLink1_2;
    QVector<QVector<float> > vectorsOfLink2_1;
    QVector<QVector<float> > vectorsOfLink2_2;
    QVector<QVector<float> > vectorsOfLink3_1;
    QVector<QVector<float> > vectorsOfLink3_2;
    QVector<QVector<float> > vectorsOfLink4_1;
    QVector<QVector<float> > vectorsOfLink4_2;
    QVector<QVector<float> > vectorsOfLink5_1;
    QVector<QVector<float> > vectorsOfLink5_2;
    QVector<QVector<float> > vectorsOfLink6;
    QVector<QVector<float> > vectorsOfBase;
    QVector<QVector<float> > vectorsOfTable;
    QVector<QVector<float> > base;
    QVector<QVector<float> > joint1;
    QVector<QVector<float> > joint2;
    QVector<QVector<float> > joint3;
    QVector<QVector<float> > joint4;
    QVector<QVector<float> > joint5;
    QVector<QVector<float> > joint6;

    GLuint texture[5];
    int slidervalue;
    //QVector<float> paramet;
    GLfloat lightAmbient[4]={0.5,0.5,0.5,1.0};
    GLfloat lightDiffuse[4]={1.0,1.0,1.0,1.0};
    GLfloat lightPosition[4]={0.0,0.0,2.0,1.0};
    GLfloat lightSpecular[4]={1.0,1.0,1.0,1.0};
    //QVector<float> paramet={0.119,0.49969,0.39602,-0.14257,0.11243,0.058};//三代机械臂
    QVector<float> paramet;//={0.119,0.38,0.33,-0.143,0.111,0.058};//四代机械臂

    //stldata *simudata;
};

#endif // STLMODULE_H
