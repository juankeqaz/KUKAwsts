#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QMesh>


class SceneModifier:public QObject
{
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier();

public slots:
    void addtorus(QString objname,float radius,float minorradius,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color);
    void addcone(QString objname,float topradius,float bottomradius,float length,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color);
    void addclinder(QString objname,float radius,float length,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color);
    void addcuboid(QString objname, float x, float y, float z, float scanle, QVector3D axis, float angle, QVector3D trans, QColor color);
    void addplant(QString objname, float width, float height, float scanle, QVector3D axis, float angle, QVector3D trans, QColor color);
    void addsphere(QString objname, float radius, int rings, int slices, float scanle, QVector3D trans, QColor color);
    void hidegeometri(QString objname, bool enable);
    bool hasobjshape(QString objname);
    void deleteshape(QString objname);
    void loadbase();
    void addaxis(int length);

private:
    Qt3DCore::QEntity *m_rootEntity;
    QList<Qt3DCore::QEntity*> listmap;

};

#endif // SCENEMODIFIER_H
