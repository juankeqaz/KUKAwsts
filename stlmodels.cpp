#include "stlmodels.h"



stlmodels::stlmodels(QGLWidget *parent):QGLWidget(parent)
{
    STLPath=QCoreApplication::applicationDirPath()+"/../../KUKA_wsts/kukamodel";//windows用该路径
    TexPath=QCoreApplication::applicationDirPath()+"/../../KUKA_wsts/Texture";//windows用该路径
    rotationX=-90.0f;
    rotationY=0.0f;
    rotationZ=-90.0f;
    translationX=0.0f;
    translationY=-0.6f;
    translationZ=-6.0f;
    parame<<0<<0<<0<<0<<0<<0;
    paramet={0.2297,0.4,0.855,0.882,0.025,0.240,0.440,0.517,0.534};
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    getSTLFile(STLPath);
    genTex(TexPath);
}

stlmodels::~stlmodels()
{
    glDeleteLists(fontbase,256);
}

void stlmodels::paintModel(QVector<float> &parames)
{
    if(parames.size()!=6)
        return;
    //qDebug()<<"parames is"<<parames.at(0)<<parames.at(1)<<parames.at(2)<<parames.at(3)<<parames.at(4)<<parames.at(5);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translationX, translationY, translationZ);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    axis(1.0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    drawTriangle(base);

    //Movej1
    glPushMatrix();
    glTranslatef(0.0f,0.0f,paramet.at(0));
    glRotatef(parames.at(0), 0, 0, 1);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-paramet.at(0));
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint1);
    glPopMatrix();
    //move j2
    glPushMatrix();
    glTranslatef(0.0f,0.0f,(paramet.at(1)-paramet.at(0)));

    glRotatef(parames.at(1),0.0f,1.0f,0.0f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-1*paramet.at(1));
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint2);
    glPopMatrix();
    //axis(0.8);

    //move j3
    glPushMatrix();
    glTranslatef(0,0,(paramet.at(3)-paramet.at(1)));
    glRotatef(parames.at(2),0.0f,1.0f,0.0f);
    glPushMatrix();
    glTranslatef(0,0,-1*paramet.at(3));
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint3);
    glPopMatrix();
    //axis(0.8);
    //move j4
    glPushMatrix();
    glTranslatef(paramet.at(5),0,0);
    glRotatef(parames.at(3),1.0f,0.0f,0.0f);
    glPushMatrix();
    glTranslatef(-1*paramet.at(5),0,0);
    glTranslatef(0,0.0,-1*paramet.at(3));
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint4);
    glPopMatrix();
    //axis(0.8);
    //move j5
    glPushMatrix();
    glTranslatef(paramet.at(6)-paramet.at(5),0,0);
    glRotatef(parames.at(4), 0,1,0);
    glPushMatrix();
    glTranslatef(-1*paramet.at(6),0,0);
    glTranslatef(0,0.0,-1*paramet.at(3));
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    drawTriangle(joint5);
    glPopMatrix();


    //move j6
    glPushMatrix();
    glTranslatef(paramet.at(7)-paramet.at(6),0,0);
    glRotatef(parames.at(5), 1,0,0);
    glPushMatrix();
    glTranslatef(-1*paramet.at(7),0,0);
    glTranslatef(0,0.0,-1*paramet.at(3));
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    drawTriangle(joint6);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(paramet.at(8)-paramet.at(7),0,0);
    axis(0.2);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    addcube(QVector3D(0.5,0.5,0.0),0.2,0.2,0.1);


}

void stlmodels::setcurrentangle()
{

}

void stlmodels::addcube(QVector3D pos, float cubelength, float cubewidth, float cubeheight)
{
    float cx=cubelength;
    float cy=cubewidth;
    float cz=cubeheight;
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glPushMatrix();
    glTranslated(pos.x(),pos.y(),pos.z());

    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(cx,0,0);
    glVertex3f(cx,0,cz);
    glVertex3f(0,0,cz);

    glVertex3f(0,0,cz);
    glVertex3f(cx,0,cz);
    glVertex3f(cx,cy,cz);
    glVertex3f(0,cy,cz);

    glVertex3f(0,cy,cz);
    glVertex3f(cx,cy,cz);
    glVertex3f(cx,cy,0);
    glVertex3f(0,cy,0);

    glVertex3f(0,cy,0);
    glVertex3f(cx,cy,0);
    glVertex3f(cx,0,0);
    glVertex3f(0,0,0);

    glVertex3f(0,0,0);
    glVertex3f(0,0,cz);
    glVertex3f(0,cy,cz);
    glVertex3f(0,cy,0);

    glVertex3f(cx,0,0);
    glVertex3f(cx,0,cz);
    glVertex3f(cx,cy,cz);
    glVertex3f(cx,cy,0);

    glEnd();

    glPopMatrix();
}

void stlmodels::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    //glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

    glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,lightPosition);
    //glLightfv(GL_LIGHT1,GL_SPECULAR,lightSpecular);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    buildfont();
}

void stlmodels::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    paintModel(parame);
}

void stlmodels::resizeGL(int width, int height)
{
    glViewport(0,0,(GLint)width,(GLint)height);
    glMatrixMode(GL_PROJECTION);//将当前矩阵指定为投影矩阵
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵
    GLfloat x=GLfloat(width) / height;
    glFrustum(-x/3,+x/3,-0.4,+0.4,3.2,13.0);//矩阵与一个透视矩阵相乘，把当前矩阵转变成透视矩阵
    glMatrixMode(GL_MODELVIEW);//对模型视景矩阵操作
}

void stlmodels::mousePressEvent(QMouseEvent *event)
{
    lastPos=event->pos();
}

void stlmodels::mouseMoveEvent(QMouseEvent *event)
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

void stlmodels::wheelEvent(QWheelEvent *event)
{
    translationZ+=(event->delta()/120)*0.1;
    updateGL();
}

void stlmodels::drawTriangle(const QVector<QVector<float> > &vec)
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

void stlmodels::getSTLFile(const QString &stlFilePath)
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
}

void stlmodels::getSTLData(const QString &stlFilePath, const QString &stlFileName)
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
            vec.push_back(list[2].toFloat());
            vec.push_back(list[3].toFloat());
            vec.push_back(list[4].toFloat());
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
    else if(stlFileName=="ruangeline")
        ruangeline = vectorOfTriangle;
    vectorOfTriangle.clear();
    file.close();
}

void stlmodels::genTex(const QString &texFilePath)
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
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
            glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        }
    }
}

void stlmodels::axis(GLfloat length)
{
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    glColor3f(0.0f,0.0f,1.0f);
    //glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(length,0.0,0.0);



    glVertex3f(length,0.0,0.0);
    glVertex3f(length-0.07,0.03,0.0);

    glVertex3f(length,0.0,0.0);
    glVertex3f(length-0.07,-0.03,0.0);

    glEnd();

    glPushMatrix();
    glTranslatef(length,0.0,0.0);
    glColor3f(0.0f,0.0f,1.0f);
    glRasterPos2d(0.04f,0.03f);
    glprint("X");
    glPopMatrix();

    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,length,0.0);
    glVertex3f(0.0,length,0.0);
    glVertex3f(0.03,length-0.07,0.0);
    glVertex3f(0.0,length,0.0);
    glVertex3f(-0.03,length-0.07,0.0);
    glEnd();
    glPushMatrix();
    glTranslatef(0.0,length,0.0);
    glColor3f(0.0f,1.0f,0.0f);
    glRasterPos2d(0.04f,0.03f);
    glprint("Y");
    glPopMatrix();

    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,length);
    glVertex3f(0.0,0.0,length);
    glVertex3f(0.0,0.03,length-0.07);
    glVertex3f(0.0,0.0,length);
    glVertex3f(0.0,-0.03,length-0.07);
    glEnd();
    glPushMatrix();
    glTranslatef(0.0,0.0,length);
    glColor3f(1.0f,0.0f,0.0f);
    glRasterPos2d(0.04f,0.03f);
    glprint("Z");
    glPopMatrix();


}

void stlmodels::glprint(const char *pstr)
{
    glPushAttrib(GL_LIST_BIT);
    glListBase(fontbase-0);
    glCallLists(strlen(pstr),GL_UNSIGNED_BYTE,pstr);
    glPopAttrib();


}

void stlmodels::buildfont()
{
    HFONT font;
    HFONT oldfont;
    fontbase=glGenLists(256);
    font= CreateFontA(-24,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,"Courier New");
    oldfont=(HFONT)SelectObject(wglGetCurrentDC(),font);
    wglUseFontBitmapsA(wglGetCurrentDC(),0,256,fontbase);
    SelectObject(wglGetCurrentDC(),oldfont);
    DeleteObject(font);

}
