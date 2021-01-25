#ifndef STLMODELS_H
#define STLMODELS_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QVector>
//#include <glut.h>

#define GLUT_DISABLE_ATEXIT_HACK

class stlmodels:public QGLWidget
{
    Q_OBJECT
public:
    stlmodels(QGLWidget *parent=0);
    ~stlmodels();
    void paintModel(QVector<float> &parames);
public slots:
    void setcurrentangle();
    void addcube(QVector3D pos, float cubelength, float cubewidth, float cubeheight);
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
    void axis(GLfloat length);
    void glprint(const char *pstr);
    void buildfont();
private:
    GLfloat lightAmbient[4]={0.5f,0.5f,0.5f,1.0f};
    GLfloat lightDiffuse[4]={1.0,1.0,1.0,1.0};
    GLfloat lightPosition[4]={0.0,0.0,2.0,1.0};
    GLfloat lightSpecular[4]={1.0,1.0,1.0,1.0};
    QString STLPath;
    QString TexPath;
    QVector<QVector<float> > base;
    QVector<QVector<float> > joint1;
    QVector<QVector<float> > joint2;
    QVector<QVector<float> > joint3;
    QVector<QVector<float> > joint4;
    QVector<QVector<float> > joint5;
    QVector<QVector<float> > joint6;
    QVector<QVector<float> > ruangeline;
    QPoint lastPos;
    GLfloat rotationX,rotationY,rotationZ;
    GLfloat translationX,translationY,translationZ;
    QVector<float> parame;
    GLuint texture[5];
    QVector<float> paramet;
    GLint fontbase;
};

#endif // STLMODELS_H
