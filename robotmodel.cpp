#include "robotmodel.h"
robotmodel::robotmodel(Qt3DCore::QNode *parent):Qt3DCore::QEntity(parent)
  ,m_mesh(new Qt3DRender::QMesh())
  //,m_transform(new Qt3DCore::QTransform())
{
    addComponent(m_mesh);
    //addComponent(m_transform);
}

Qt3DRender::QMesh *robotmodel::mesh() const
{
    return  m_mesh;
}

void robotmodel::updatetransform(Qt3DCore::QTransform *transform)
{
}
