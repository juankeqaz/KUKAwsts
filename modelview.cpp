#include "modelview.h"
#include <QtGui/QScreen>
#include <QHBoxLayout>


modelview::modelview(QWidget * windows)
{

    this->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    this->containner = QWidget::createWindowContainer(this);

    QSize screensize=this->screen()->size();
    containner->setMinimumSize(QSize(20,10));
    containner->setMaximumSize(screensize);
    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(containner);
    windows->setLayout(layout);
    rotcenter=QVector3D(0,0,0);
    paramet={230,170,460,240,200,17};
    scaler=0.1f;
    jointpos={0,-90,90,0,0,0};
    initcamera();


    //plantEntity->addComponent(planttransform);

    robot=new robotmodel(methEntity);
    robottransform=new Qt3DCore::QTransform();

    robot->addComponent(robottransform);
    joint1=new robotmodel(robot);
    joint1transform=new Qt3DCore::QTransform();
    //joint1transform->setTranslation()
    joint1->addComponent(joint1transform);
    joint2=new robotmodel(robot);
    joint2transform=new Qt3DCore::QTransform();
    joint2->addComponent(joint2transform);
    joint3=new robotmodel(robot);
    joint3transform=new Qt3DCore::QTransform();
    joint3->addComponent(joint3transform);
    joint4=new robotmodel(robot);
    joint4transform=new Qt3DCore::QTransform();
    joint4->addComponent(joint4transform);
    joint5=new robotmodel(robot);
    joint5transform=new Qt3DCore::QTransform();
    joint5->addComponent(joint5transform);
    joint6=new robotmodel(robot);
    joint6transform=new Qt3DCore::QTransform();
    joint6->addComponent(joint6transform);
    tools=new Qt3DCore::QEntity(robot);
    tooltransform=new Qt3DCore::QTransform();
    tools->addComponent(tooltransform);
    toolmodifier=new SceneModifier(tools);

    plantEntity=new Qt3DCore::QEntity(robot);
    planttransform = new Qt3DCore::QTransform();
    plantEntity->addComponent(planttransform);
    modifier = new SceneModifier(plantEntity);


    updatepositon(jointpos,0);
    addbase();
    workpeiceEntity=new Qt3DCore::QEntity(plantEntity);
    Qt3DExtras::QPhongMaterial *meterial=new Qt3DExtras::QPhongMaterial();
    workpeiceEntity->addComponent(meterial);
    workpiecetransform=new Qt3DCore::QTransform();
    workpeiceEntity->addComponent(workpiecetransform);
}

modelview::~modelview()
{
    delete containner;
    delete roottransform;
    delete rootEntity;
    delete modifier;
}

void modelview::addbase()
{
    //robottransform->setScale(0.2f);
    robot->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/base.stl"));
    Qt3DExtras::QDiffuseMapMaterial * diffuse=new Qt3DExtras::QDiffuseMapMaterial();
    diffuse->setSpecular(QColor::fromRgbF(0.5f,0.5f,0.5f,1.0f));
    diffuse->setShininess(2.0f);
    Qt3DRender::QTextureImage *image=new Qt3DRender::QTextureImage();
    image->setSource(QUrl::fromLocalFile("../KUKA_wsts/Texture/grey.png"));
    diffuse->diffuse()->addTextureImage(image);
    robot->addComponent(diffuse);

    joint1->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint1.stl"));
    joint1->addComponent(diffuse);

    joint2->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint2.stl"));
    joint2->addComponent(diffuse);
    joint3->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint3.stl"));
    joint3->addComponent(diffuse);
    joint4->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint4.stl"));
    joint4->addComponent(diffuse);
    joint5->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint5.stl"));
    joint5->addComponent(diffuse);
    joint6->mesh()->setSource(QUrl::fromLocalFile("../KUKA_wsts/kukamodel/joint6.stl"));
    joint6->addComponent(diffuse);

}

void modelview::updatepositon(QVector<float> position,int p)
{
    Q_UNUSED(p);
    jointpos=position;

    robottransform->setScale(0.2f);
    //QMatrix4x4 j1matrix=getTjoint(jointpos.at(0),a1,-M_PI/2,d1);
    //joint1transform->setMatrix(j1matrix);
    joint1transform->setRotationZ(jointpos.at(0));
    QMatrix4x4 j1matrix=joint1transform->matrix();

    QMatrix4x4 j2matrix;
    j2matrix.translate(a1,0,d1);
    j2matrix.rotate(jointpos.at(1)+90,QVector3D(0,1,0));
    j2matrix=j1matrix * j2matrix;
    j2matrix.translate(-a1,0,-d1);

    //joint2transform->setRotationZ(jointpos.at(1));
    joint2transform->setMatrix(j2matrix);

    QMatrix4x4 j3matrix;
    j3matrix.translate(a1,0,a2+d1);
    j3matrix.rotate(jointpos.at(2)-90,QVector3D(0,1,0));
    j3matrix=j2matrix*j3matrix;
    j3matrix.translate(-a1,0,-a2-d1);
    joint3transform->setMatrix(j3matrix);

    QMatrix4x4 j4matrix;
    j4matrix.translate(236,0,880);
    j4matrix.rotate(jointpos.at(3),QVector3D(1,0,0));
    j4matrix=j3matrix*j4matrix;
    j4matrix.translate(-236,0,-880);
    joint4transform->setMatrix(j4matrix);

    QMatrix4x4 j5matrix;
    j5matrix.translate(445,0,880);
    j5matrix.rotate(jointpos.at(4),QVector3D(0,1,0));
    j5matrix=j4matrix*j5matrix;
    j5matrix.translate(-445,0,-880);
    joint5transform->setMatrix(j5matrix);

    QMatrix4x4 j6matrix;
    j6matrix.translate(517,0,880);
    j6matrix.rotate(jointpos.at(5),QVector3D(1,0,0));
    j6matrix=j5matrix*j6matrix;
    j6matrix.translate(-517,0,-880);
    joint6transform->setMatrix(j6matrix);

    QMatrix4x4 toolmatrix;
    toolmatrix.translate(535,0,880);
    toolmatrix=j6matrix*toolmatrix;
    tooltransform->setMatrix(toolmatrix);
}

QVector<float> modelview::getcurrentjointposition()
{
    return jointpos;
}

void modelview::plantcoordchanged(QMatrix4x4 matrix)
{
    planttransform->setMatrix(matrix);
}

void modelview::makeabox()
{

    if(currentcomp!=nullptr)
    {
        plantEntity->removeComponent(currentcomp);
        currentcomp=nullptr;
    }
    QMatrix4x4 pmat=plantmatrix;
    planttransform->setMatrix(pmat);
    QMatrix4x4 mmat=workpicematrix;
    //qDebug()<<"modelview::makeabox :"<<currentshapetype<<currentshapeparame;
    switch (currentshapetype) {
    case 0:
    {
        if(currentshapeparame.size()<3)
            return;
        Qt3DExtras::QCuboidMesh *mesh=new Qt3DExtras::QCuboidMesh();
        float x,y,z;
        x=currentshapeparame.at(0);
        y=currentshapeparame.at(1);
        z=currentshapeparame.at(2);
        mesh->setXExtent(x);
        mesh->setYExtent(y);
        mesh->setZExtent(z);
        currentcomp=mesh;
        workpeiceEntity->addComponent(currentcomp);
        mmat.translate(QVector3D(x/2,y/2,z/2));
        workpiecetransform->setMatrix(mmat);
    }
        break;
    default:
        break;

    }
}

void modelview::removemodel()
{
    if(currentcomp!=nullptr)
    {
        workpeiceEntity->removeComponent(currentcomp);
        currentcomp=nullptr;
    }
    QMatrix4x4 mat=planttransform->matrix();
    mat.setToIdentity();
    planttransform->setMatrix(mat);
}

void modelview::getendposition()
{

    qDebug()<<"modelview::getendposition"<<tooltransform->matrix();
}


QMatrix4x4 modelview::movematrix(Qt3DCore::QTransform *joint1,QVector3D trans,QVector3D dir,float angle)
{
    QMatrix4x4 j1matrix=joint1->matrix();
    QMatrix4x4 j2matrix;
    j2matrix.translate(trans);
    j2matrix.rotate(angle,dir);
    j2matrix=j1matrix * j2matrix;
    return  j2matrix;
}

QMatrix4x4 modelview::getTjoint(float j, float a, float af, float d)
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.setRow(0,QVector4D(cos(j),-sin(j)*cos(af),sin(j)*sin(af),a*cos(j)));
    matrix.setRow(1,QVector4D(sin(j),cos(j)*cos(af),-cos(j)*sin(af),a*sin(j)));
    matrix.setRow(2,QVector4D(0,sin(af),cos(af),d));
    matrix.setRow(3,QVector4D(0,0,0,1));
    return  matrix;
}

void modelview::initcamera()
{
    rootEntity = new Qt3DCore::QEntity();


    Qt3DRender::QCamera *cameraEntity = camera();

    cameraEntity->lens()->setPerspectiveProjection(60.0f, width()/height(), 0.01f, 10000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 200.0f));
    cameraEntity->setUpVector(QVector3D(0, 0, 1));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.5);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    //Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    //Qt3DExtras::QAbstractCameraController *camController = new Qt3DExtras::QAbstractCameraController();
    //camController->setCamera(cameraEntity);

    // Scenemodifier
    methEntity=new Qt3DCore::QEntity(rootEntity);



    roottransform= new Qt3DCore::QTransform();
    roottransform->setTranslation(QVector3D(0,-90,0));
    roottransform->setRotation(QQuaternion(scaler,rotcenter));
    roottransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -90));

    methEntity->addComponent(roottransform);

    //plantEntity->addComponent(roottransform);

    // Set root object of the scene
    setRootEntity(rootEntity);
}

void modelview::mousePressEvent(QMouseEvent *event)
{
    lastPos=event->pos();
}

void modelview::mouseMoveEvent(QMouseEvent *event)
{
    float dx = float(event->x()-lastPos.x())/width();
    float dy = float(event->y()-lastPos.y())/height();
    float tx= float(event->x()-lastPos.x())/10;
    float ty=float(event->y()-lastPos.y())/10;
    //qDebug()<<"dx,xy is"<<dx<<dy;
    if(event->buttons()&Qt::LeftButton)
    {
        roottransform->setRotationX(roottransform->rotationX()+180*dy);
        roottransform->setRotationY(roottransform->rotationY()+180*dx);
    }else if(event->buttons()&Qt::RightButton)
    {
        roottransform->setTranslation(QVector3D(roottransform->translation().x()+tx,roottransform->translation().y()-ty,roottransform->translation().z()));
    }
    lastPos=event->pos();
}

void modelview::wheelEvent(QWheelEvent *event)
{
    scaler=roottransform->scale();
    scaler += (event->delta()/120)*0.1;
    roottransform->setScale(scaler);
    //qDebug()<<"scaler is:"<<scaler;

}
