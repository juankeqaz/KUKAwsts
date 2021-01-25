#include "stlmodule.h"
#include <QVector>
//#include "fiveorderinter.h"
#include <unordered_map>

STLModule::STLModule(QGLWidget *parent)
    : QGLWidget(parent)
{

    parame<<0<<90<<0<<90<<0<<0;
    slidervalue=40;
    STLPath=QCoreApplication::applicationDirPath()+"/../../KUKA_wsts/kukamodel";//windows用该路径
    TexPath=QCoreApplication::applicationDirPath()+"/../../KUKA_wsts/Texture";//windows用该路径
    //STLPath=QCoreApplication::applicationDirPath()+"/../robotcontroler/Geometry";//linux用该路径
    //TexPath=QCoreApplication::applicationDirPath()+"/../robotcontroler/Texture";//linux用该路径
    //qDebug()<<TexPath;
    rotationX=-90.0f;
    rotationY=0.0f;
    rotationZ=-90.0f;
    translationX=0.0f;
    translationY=-0.6f;
    translationZ=-6.0f;
    //genTex(TexPath);
    //glGenTextures(5,&texture[0]);
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    paramet={0.119,0.49969,0.39602,-0.14257,0.11243,0.058};
    getSTLFile(STLPath);
    genTex(TexPath);
//    for(int i=0;i<6;++i)
//    {
//        paramet.push_back((float)DH_paramet[i]);
//    }
    //paramet={0.119,0.38,0.33,-0.143,0.111,0.058};
    //simudata=new stldata;
    //connect(simudata,SIGNAL(setiostatusignal(int,int)),this,SIGNAL(setiostatussignal(int,int)));
    //connect(this,SIGNAL(simucommandsignal(QStringList,int)),simudata,SLOT(virtualrobcommand(QStringList,int)));
    //connect(simudata,SIGNAL(currentcommandsignal()),this,SIGNAL(runnextcommandsignal()));
    //connect(simudata,SIGNAL(dataforsimulate(QVector<float>)),this,SLOT(actualrun(QVector<float>)));
    //connect(simudata,SIGNAL(movepnoresult()),this,SIGNAL(positionnotreachable()));
    //connect(simudata,SIGNAL(positionnotreachable()),this,SIGNAL(positionnotreachable()));
}

STLModule::~STLModule()
{
    //simudata->deleteLater();
}
void STLModule::initializeGL()
{
    //qDebug()<<"initializegl is used!";
    //getSTLFile(STLPath);
    //genTex(TexPath);

    qglClearColor(Qt::white);
    glShadeModel(GL_FLAT);//glShadeModel函数用于控制opengl中绘制指定两点间其他点颜色的过渡模式,参数一般为GL_SMOOTH（默认）,GL_FLAT
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);//启用了之后，OpenGL在绘制的时候就会检查，当前像素前面是否有别的像素，如果别的像素挡道了它，那它就不会绘制，也就是说，OpenGL就只绘制最前面的一层
    glDepthFunc(GL_LEQUAL);//如果输入的深度值等于参考值，则通过
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);/*指定颜色和纹理坐标的差值质量。
    //如果OpenGL不能有效的支持透视修正参数差值，那么GL_DONT_CARE 和 GL_FASTEST可以执行颜色、纹理坐标的简单线性差值计算
    //GL_POLYGON_SMOOTH_HINT：指定反走样多边形的采样质量，如果应用较大的滤波函数，GL_NICEST在光栅化期间可以生成更多的像素段。
    //GL_NICEST：选择最高质量选项。
    //*/
    glEnable(GL_CULL_FACE);//开启剔除操作效果,无论如何位置变化，我们都只能看到构成其组成的多边形的某一面时，可使用该函数
    glEnable(GL_TEXTURE_2D);//绑定到GL_TEXTURE_2D目标，表示二维纹理
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

    glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);//创建指定的光源,light可以是GL_LIGHT0、GL_LIGHT1，pname定义的是光源的属性，它指定了一个命名参数。params表示表示pname属性将要被设置的值
    glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,lightPosition);
    //glLightfv(GL_LIGHT1,GL_SPECULAR,lightSpecular);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);


}
void STLModule::resizeGL(int width, int height)
{
    //qDebug()<<"resizeGl param"<<width<<height;
    glViewport(0,0,(GLint)width,(GLint)height);
    //glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);//将当前矩阵指定为投影矩阵
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵
    GLfloat x=GLfloat(width) / height;
    glFrustum(-x/3,+x/3,-0.4,+0.4,3.2,13.0);//矩阵与一个透视矩阵相乘，把当前矩阵转变成透视矩阵
    glMatrixMode(GL_MODELVIEW);//对模型视景矩阵操作

}
void STLModule::paintGL()
{
    //qDebug()<<"paintGL is used";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    paintModel(parame);
    //currentposition(parame);
}
void STLModule::paintModel(QVector<float> &parames)
{
    //glClearColor(1,1,1,1);
    if(parames.size()!=6)
        return;
    //qDebug()<<"parames is"<<parames.at(0)<<parames.at(1)<<parames.at(2)<<parames.at(3)<<parames.at(4)<<parames.at(5);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_MODELVIEW);
    curparame=parames;

    glLoadIdentity();
    glTranslatef(translationX, translationY, translationZ);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(base);
    //glFlush();

    //Catia World frame1 to Base
    //glTranslatef(0.0f,0.0380f,-0.003f);
    //glRotatef(99.8469f,0.3822f,0.8414f,0.3822f);

    //Movej1
    //glRotatef(0.0f, 1.0f, 0.0f, 0.0f);//Rx
    //glTranslatef(0.0f, 0.0f, 0.0f);//Tx
    glRotatef(parames.at(0), 0.0f, 0.0f, 1.0f);//Rz
    glTranslatef(0.0f, 0.0f, paramet.at(0));//Tz
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-1.0 * paramet.at(0));
    //glRotatef(99.8469, -0.3822f, -0.8414f, -0.3822f);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint1);
    glPopMatrix();

    //move j2
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);//Rx
    //glTranslatef(0.0f, 0.0f, 0.0f);//Tx
    glRotatef(parames.at(1), 0.0f, 0.0f, 1.0f);//Rz
    //glTranslatef(0.0f, 0.0f, 0.0f);//Tz
    glPushMatrix();
    glTranslatef(0.0f,-1.0 * paramet.at(0),0.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint2);
    glPopMatrix();

    //move j3
    //glRotatef(0.0f, 1.0f, 0.0f, 0.0f);//Rx
    glTranslatef(paramet.at(1), 0.0f, 0.0f);//Tx
    glRotatef(parames.at(2), 0.0f, 0.0f, 1.0f);//Rz
    //glTranslatef(0.0f, 0.0f, 0.0f);//Tz
    glPushMatrix();
    glTranslatef(-1.0 * paramet.at(1),-1*paramet.at(0),0.0f);
    glRotatef(-90.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint3);
    //drawTriangle(vectorsOfLink3_3);
    glPopMatrix();

    //move j4
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);//Rx
    glTranslatef(paramet.at(2), 0.0f, 0.0f);//Tx
    glRotatef(parames.at(3), 0.0f, 0.0f, 1.0f);//Rz
    glTranslatef(0.0f, 0.0f, paramet.at(3));//Tz
    glPushMatrix();
    glTranslatef(-1.0 * (paramet.at(2)+paramet.at(1)),paramet.at(0),-1 * paramet.at(3));
    glRotatef(90.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint4);

    glPopMatrix();

    //move j5
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);//Rx
    //glTranslatef(0.0f, 0.0f, 0.0f);//Tx
    glRotatef(parames.at(4), 0.0f, 0.0f, 1.0f);//Rz
    glTranslatef(0.0f, 0.0f, paramet.at(4));//Tz
    glPushMatrix();
    glTranslatef(-1.0 * (paramet.at(2)+paramet.at(1)),-1 * paramet.at(3),-1*(paramet.at(0)+paramet.at(4)));
    //glRotatef(99.8469f, -0.3822f,-0.8414f,-0.3822f);
    //drawTriangle(vectorsOfLink5);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint5);
    glPopMatrix();
    //move j6
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);     //Rx
    //glTranslatef(0.0f, 0.0f, 0.0f);           //Tx
    glRotatef(parames.at(5), 0.0f, 0.0f, 1.0f);     //Rz
    glTranslatef(0.0f, 0.0f, paramet.at(5));     //Tz
    glPushMatrix();
    glTranslatef(-1.0 * (paramet.at(2)+paramet.at(1)),-1*(paramet.at(0)+paramet.at(4)),paramet.at(3)-paramet.at(5));
    //glTranslatef(-0.71f,-0.23f,-0.201f);
    glRotatef(-90.0f, 1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint6);
    glPopMatrix();

}

void STLModule::zoom(int value)
{
    if(value>slidervalue)
        translationZ+=0.1;
    else translationZ+=(-1)*0.1;
    slidervalue=value;
    updateGL();
    Q_EMIT zoomvaluechanged(slidervalue);
}

void STLModule::actualrun(QVector<float> pos)
{
    //qDebug()<<"run a position"<<pos.at(0);
    parame=pos;
    updateGL();
}

void STLModule::upparamet(QStringList paramelist)
{
    QString str;
    QStringList list;
    for(int i=0;i<paramelist.size();++i)
    {
        str=paramelist.at(i);
        if(str.startsWith("S_paramet"))
        {
            list=str.split(" ");
            break;
        }
    }
    if(list.size()<6) return;

    paramet.clear();
    for(int j=1;j<list.size();++j)
    {
        str=list.at(j);
        paramet.push_back(str.toFloat());
    }
    //qDebug()<<"paramet"<<paramet.at(0)<<paramet.at(1)<<paramet.at(2)<<paramet.at(3)<<paramet.at(4)<<paramet.at(5);
}

void STLModule::simucommand(QStringList paramlist, int p)
{
    Q_EMIT simucommandsignal(paramlist,p);
}

void STLModule::stopsimu()
{
    //simudata->stoprun();
}

void STLModule::speedmodel(int flag)
{
    //simudata->getspeedmodelflag(flag);
}

void STLModule::posmodel(int flag)
{
    //simudata->getmoveflag(flag);
}

void STLModule::gohome()
{

    //simudata->gotohomeslot();
}

void STLModule::changespeed(int value)
{
    //simudata->getspeedratevalue(value);
    Q_EMIT speedchanged();
}

QVector<float> STLModule::getcurrentangle()
{
    return  parame;
}



void STLModule::drawTriangle(const QVector<QVector<float> > &vec)
{
    glBegin(GL_TRIANGLES);
    for(int i=0; i<vec.size(); i+=4)
    {

        glColor3f(1.0, 1.0, 1.0);
        GLfloat v01=vec[i][0];
        GLfloat v02=vec[i][1];
        GLfloat v03=vec[i][2];
        glNormal3f(v01,v02,v03);

        GLfloat v11=vec[i+1][0];
        GLfloat v12=vec[i+1][1];
        GLfloat v13=vec[i+1][2];
        glVertex3f(v11,v12,v13);

        GLfloat v21=vec[i+2][0];
        GLfloat v22=vec[i+2][1];
        GLfloat v23=vec[i+2][2];
        glVertex3f(v21,v22,v23);

        GLfloat v31=vec[i+3][0];
        GLfloat v32=vec[i+3][1];
        GLfloat v33=vec[i+3][2];
        glVertex3f(v31,v32,v33);

    }
    glEnd();
}
void STLModule::mousePressEvent(QMouseEvent *event)
{
    lastPos=event->pos();
}
void STLModule::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx=GLfloat(event->x()-lastPos.x())/width();
    GLfloat dy=GLfloat(event->y()-lastPos.y())/height();
    if(event->buttons()&Qt::LeftButton)
    {
        rotationX+=180*dy;
        rotationY+=180*dx;
        updateGL();
    }else if(event->buttons()&Qt::RightButton)
    {
        translationX+= dx;
        translationY+= -dy;
        updateGL();
    }
    lastPos=event->pos();
}
void STLModule::wheelEvent(QWheelEvent *event)
{
    translationZ+=(event->delta()/120)*0.1;
    updateGL();
    slidervalue=(translationZ+13)*8;
    Q_EMIT zoomvaluechanged(slidervalue);
}

//void STLModule::resizeEvent(QResizeEvent *size)
//{
//    Q_UNUSED(size);
//    resizeGL(this->width(),this->height());
//    paintGL();
//}
void STLModule::getSTLFile(const QString &stlFilePath)
{
    QStringList filter;
    filter << "*.stl";
    QDir *dir = new QDir(stlFilePath);
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));

    for(int i=0; i<fileInfo->size(); ++i)
    {
        QString totalPath = fileInfo->at(i).absoluteFilePath();
        QString name = fileInfo->at(i).baseName();
        getSTLData(totalPath,name);
    }
    //qDebug()<<"fileinfo size is"<<fileInfo->size();
}
void STLModule::getSTLData(const QString &stlFilePath, const QString &stlFileName)
{
    QFile file(stlFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {   return;     }

    QTextStream in(&file);
    QString str;
    QVector<QVector<float> > vectorOfTriangle;

    while(!in.atEnd())
    {
        str = in.readLine();
        if(str.isEmpty())
            continue;

        QStringList list = str.split(" ",QString::SkipEmptyParts);
        QVector<float> vec;
        if(list.contains("facet"))
        {
            vec.push_back(list[2].toFloat()/1000);
            vec.push_back(list[3].toFloat()/1000);
            vec.push_back(list[4].toFloat()/1000);
            list.clear();
        }
        else if(list.contains("vertex"))
        {
            vec.push_back(list[1].toFloat()/1000);
            vec.push_back(list[2].toFloat()/1000);
            vec.push_back(list[3].toFloat()/1000);
            list.clear();
        }
        else
        {
            list.clear();
            continue;
        }
        vectorOfTriangle.push_back(vec);
    }

    if(stlFileName=="basea")
        base = vectorOfTriangle;
    else if(stlFileName=="joint1")
        joint1 = vectorOfTriangle;
    else if(stlFileName=="joint2")
        joint2 = vectorOfTriangle;
    else if(stlFileName=="joint3")
        joint3 = vectorOfTriangle;
    else if(stlFileName=="joint4")
        joint4 = vectorOfTriangle;
    else if(stlFileName=="joint5")
        joint5 = vectorOfTriangle;
    else if(stlFileName=="joint6")
        joint6 = vectorOfTriangle;
    vectorOfTriangle.clear();
    file.close();
}
void STLModule::genTex(const QString &texFilePath)
{
    QDir *dir = new QDir(texFilePath);
    QStringList filter;
    filter<<"*.png";
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    QImage buf,tex;
    int size = (fileInfo->size()<5?fileInfo->size():5);
    glGenTextures(size,texture);//生成指定数目的纹理对象，size表示生成的纹理数量，texture表示纹理对象的索引
    for(int i=0; i<size; ++i)
    {
        if(buf.load(fileInfo->at(i).absoluteFilePath()))//获取文件信息
        {
            tex = QGLWidget::convertToGLFormat(buf);//将Qt图片的格式buf转换成opengl的图片格式tex
            //glGenTextures(1,texture[i]);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
            glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        }
    }
}
void STLModule::zoomIn()
{
    if(translationZ<-4.5f)
    {
        translationZ += 0.1;
        updateGL();
    }
}
void STLModule::zoomOut()
{
    if(translationZ>-7.5f)
    {
        translationZ -= 0.1;
        update();
    }
}
//QVector<float> STLModule::getcurpostions()
//{
//    return curparame;
//}
//QVector<float> STLModule::getspacecurpostions()
//{
////    QVector<float> tempvec;
////    if(isspeedmodel)
////        tempvec=cacul->fKCal(curparame,spacevec);
////    else
////        tempvec=spacevec;
////    return tempvec;
//}
