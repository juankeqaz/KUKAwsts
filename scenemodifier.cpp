#include "scenemodifier.h"
#include <QtCore/QDebug>

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{
    listmap.clear();
    addaxis(200);


}

SceneModifier::~SceneModifier()
{
}

void SceneModifier::addtorus(QString objname,float radius,float minorradius,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color)
{
    Qt3DExtras::QTorusMesh *m_torus = new Qt3DExtras::QTorusMesh();
    m_torus->setRadius(radius);
    m_torus->setMinorRadius(minorradius);
    m_torus->setRings(rings);
    m_torus->setSlices(slices);
    //! [0]
    // TorusMesh Transform
    //! [1]
    Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform();
    torusTransform->setScale(scanle);
    torusTransform->setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
    torusTransform->setTranslation(trans);
    //! [1]

    //! [2]
    Qt3DExtras::QPhongMaterial *torusMaterial = new Qt3DExtras::QPhongMaterial();
    torusMaterial->setDiffuse(color);
    //! [2]

    // Torus
    //! [3]
    Qt3DCore::QEntity* m_torusEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_torusEntity->addComponent(m_torus);
    m_torusEntity->addComponent(torusMaterial);
    m_torusEntity->addComponent(torusTransform);
    m_torusEntity->setObjectName(objname);
    listmap<<m_torusEntity;
}

void SceneModifier::addcone(QString objname,float topradius,float bottomradius,float length,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color)
{
    Qt3DExtras::QConeMesh *cone = new Qt3DExtras::QConeMesh();
    cone->setTopRadius(topradius);
    cone->setBottomRadius(bottomradius);
    cone->setLength(length);
    cone->setRings(rings);
    cone->setSlices(slices);

    // ConeMesh Transform
    Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
    coneTransform->setScale(scanle);
    coneTransform->setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
    coneTransform->setTranslation(trans);
    Qt3DExtras::QPhongMaterial *coneMaterial = new Qt3DExtras::QPhongMaterial();
    coneMaterial->setDiffuse(color);

    // Cone
    Qt3DCore::QEntity* m_coneEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_coneEntity->addComponent(cone);
    m_coneEntity->addComponent(coneMaterial);
    m_coneEntity->addComponent(coneTransform);
    m_coneEntity->setObjectName(objname);
    listmap<<m_coneEntity;
}

void SceneModifier::addclinder(QString objname,float radius,float length,int rings,int slices,float scanle,QVector3D axis,float angle,QVector3D trans,QColor color)
{
    Qt3DExtras::QCylinderMesh *cylinder = new Qt3DExtras::QCylinderMesh();
    cylinder->setRadius(radius);
    cylinder->setLength(length);
    cylinder->setRings(rings);
    cylinder->setSlices(slices);

    //trans.setZ(trans.z()+length/2);
    // CylinderMesh Transform
    Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform();
    cylinderTransform->setScale(scanle);
    cylinderTransform->setTranslation(trans);
    cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(axis, angle));



    Qt3DExtras::QPhongMaterial *cylinderMaterial = new Qt3DExtras::QPhongMaterial();
    cylinderMaterial->setDiffuse(color);

    // Cylinder
    Qt3DCore::QEntity* m_cylinderEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_cylinderEntity->addComponent(cylinder);
    m_cylinderEntity->addComponent(cylinderMaterial);
    m_cylinderEntity->addComponent(cylinderTransform);
    m_cylinderEntity->setObjectName(objname);
    listmap<<m_cylinderEntity;
}

void SceneModifier::addcuboid(QString objname, float x, float y, float z, float scanle, QVector3D axis, float angle, QVector3D trans, QColor color)
{
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setXExtent(x);
    cuboid->setYExtent(y);
    cuboid->setZExtent(z);

    // CuboidMesh Transform
    Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(scanle);
    cuboidTransform->setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
    cuboidTransform->setTranslation(trans);

    Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(color);

    //Cuboid
    Qt3DCore::QEntity* m_cuboidEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_cuboidEntity->addComponent(cuboid);
    m_cuboidEntity->addComponent(cuboidMaterial);
    m_cuboidEntity->addComponent(cuboidTransform);
    m_cuboidEntity->setObjectName(objname);
    listmap<<m_cuboidEntity;
}

void SceneModifier::addplant(QString objname, float width, float height, float scanle, QVector3D axis, float angle, QVector3D trans, QColor color)
{
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(width);
    planeMesh->setHeight(height);


    // Plane mesh transform
    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setScale(scanle);
    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
    planeTransform->setTranslation(trans);

    Qt3DExtras::QPhongMaterial *planeMaterial = new Qt3DExtras::QPhongMaterial();
    planeMaterial->setDiffuse(color);

    // Plane
    Qt3DCore::QEntity* m_planeEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_planeEntity->addComponent(planeMesh);
    m_planeEntity->addComponent(planeMaterial);
    m_planeEntity->addComponent(planeTransform);
    m_planeEntity->setObjectName(objname);
    listmap<<m_planeEntity;
}

void SceneModifier::addsphere(QString objname, float radius, int rings, int slices, float scanle,QVector3D trans, QColor color)
{
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(rings);
    sphereMesh->setSlices(slices);
    sphereMesh->setRadius(radius);

    // Sphere mesh transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(scanle);
    sphereTransform->setTranslation(trans);

    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(color);

    // Sphere
    Qt3DCore::QEntity* m_sphereEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_sphereEntity->addComponent(sphereMesh);
    m_sphereEntity->addComponent(sphereMaterial);
    m_sphereEntity->addComponent(sphereTransform);
    m_sphereEntity->setObjectName(objname);
    listmap<<m_sphereEntity;
}

void SceneModifier::hidegeometri(QString objname,bool enable)
{
    if(listmap.size()<=0) return;
    foreach (Qt3DCore::QEntity *entity, listmap) {
        if(entity->objectName()==objname)
        {
            entity->setEnabled(enable);
        }
    }
}

bool SceneModifier::hasobjshape(QString objname)
{
    foreach (Qt3DCore::QEntity *entity, listmap) {
        if(entity->objectName()==objname)
        {
            return true;
        }
    }
    return  false;
}

void SceneModifier::deleteshape(QString objname)
{
    foreach (Qt3DCore::QEntity *entity, listmap) {
        if(entity->objectName()==objname)
        {
            listmap.removeOne(entity);
            return;
        }
    }
}

void SceneModifier::loadbase()
{
//    Qt3DRender::QMesh *mesh=new Qt3DRender::QMesh();

//    mesh->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/toyplane.obj"));
//    mesh->setMeshName("base");
//    mesh->setEnabled(true);

//    Qt3DCore::QTransform *baseTransform = new Qt3DCore::QTransform();
//    baseTransform->setScale(-1.0f);
//    baseTransform->setTranslation(QVector3D(0.0f,0.0f,0.0f));

//    Qt3DExtras::QPhongMaterial *basematerial=new Qt3DExtras::QPhongMaterial();
    //    basematerial->setDiffuse(QColor(QRgb(0x665432)));
}

void SceneModifier::addaxis(int length)
{
    addsphere("center",2,20,50,1.0,QVector3D(0.0,0.0,0.0),"#000000");
    addclinder("xaxis",1,length,20,50,1.0,QVector3D(0,0,1),90,QVector3D(length/2,0,0),"#ff0000");
    addcone("xcone",0,4,10,20,50,1.0,QVector3D(0,0,1),-90,QVector3D(length,0.0,0.0),"#ff0000");
    addclinder("yaxis",1,length,20,50,1.0,QVector3D(0,1,0),90,QVector3D(0.0,length/2,0),"#00ff00");
    addcone("ycone",0,4,10,20,50,1.0,QVector3D(0,1,0),90,QVector3D(0.0,length,0.0),"#00ff00");
    addclinder("zaxis",1,length,20,50,1.0,QVector3D(1,0,0),90,QVector3D(0.0,0,length/2),"#0000ff");
    addcone("ycone",0,4,10,20,50,1.0,QVector3D(1,0,0),90,QVector3D(0.0,0.0,length),"#0000ff");
}
