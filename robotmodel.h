#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H
#include <Qt3DCore>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>

class robotmodel:public Qt3DCore::QEntity
{
    Q_OBJECT
public:
    robotmodel(Qt3DCore::QNode *parent=nullptr);
    Qt3DRender::QMesh *mesh() const;
    void updatetransform(Qt3DCore::QTransform * transform);
private:
    Qt3DRender::QMesh *m_mesh;
    //Qt3DCore::QTransform *m_transform;


};

#endif // ROBOTMODEL_H
