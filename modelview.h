#ifndef MODELVIEW_H
#define MODELVIEW_H
#include <QGLWidget>
#include <QWidget>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include "scenemodifier.h"
#include <Qt3DExtras/qabstractcameracontroller.h>
#include <QVector3D>
#include <QWheelEvent>
#include <Qt3DRender/QObjectPicker>
#include "robotmodel.h"
#include <Qt3DExtras/QDiffuseMapMaterial>
#include <Qt3DRender/QTextureImage>
#include <QQuaternion>
#include <globalvar.h>
#include <kinematics.h>



class modelview:public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT
public:
    explicit modelview(QWidget * windows=nullptr);
    ~modelview();
    void addbase();
public slots:

    void updatepositon(QVector<float> position, int p);
    QVector<float> getcurrentjointposition();
    void plantcoordchanged(QMatrix4x4 matrix);
    void makeabox();
    void removemodel();
    void getendposition();

private:

    QWidget* containner;
    Qt3DCore::QTransform *roottransform;
    QVector3D rotcenter;
    Qt3DCore::QEntity *rootEntity;
    float scaler;
    SceneModifier *modifier;
    Qt3DCore::QEntity *methEntity;
    Qt3DCore::QEntity *workpeiceEntity;
    Qt3DCore::QEntity *plantEntity;
    QPoint lastPos;
    QVector<float> paramet;
    QVector<float> jointpos;
    robotmodel *robot;
    robotmodel *joint1;
    robotmodel *joint2;
    robotmodel *joint3;
    robotmodel *joint4;
    robotmodel *joint5;
    robotmodel *joint6;
    Qt3DCore::QEntity* tools;
    Qt3DCore::QTransform *robottransform;
    Qt3DCore::QTransform *joint1transform;
    Qt3DCore::QTransform *joint2transform;
    Qt3DCore::QTransform *joint3transform;
    Qt3DCore::QTransform *joint4transform;
    Qt3DCore::QTransform *joint5transform;
    Qt3DCore::QTransform *joint6transform;
    Qt3DCore::QTransform *tooltransform;
    Qt3DCore::QTransform *planttransform;
    Qt3DCore::QTransform *workpiecetransform;
    SceneModifier *toolmodifier;
    QVector<QVector<float> > jointpath;
    Qt3DCore::QComponent *currentcomp=nullptr;
    float a1=25;
    float d1=400;
    float a2=455;
    float a3=25;
    float d4=445;
    float d6=90;

private:
    QMatrix4x4 movematrix(Qt3DCore::QTransform *joint1, QVector3D trans, QVector3D dir, float angle);
    QMatrix4x4 getTjoint(float j,float a,float af,float d);

protected:
    void initcamera();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

};

#endif // MODELVIEW_H
