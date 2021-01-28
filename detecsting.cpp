#include "detecsting.h"
#include "ui_detecsting.h"
#include <QDebug>

DetecSting* DetecSting::self=0;

DetecSting::DetecSting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetecSting)
{
    ui->setupUi(this);
    initForms();
    ui->groupBox_xoy->hide();
    ui->groupBox_xdir->hide();
    ui->widget_setting->setEnabled(false);
}

DetecSting::~DetecSting()
{
    delete ui;
}

void DetecSting::on_pushButton_tool_axis_clicked()
{
    QString orgx,orgy,orgz,xdirx,xdiry,xdirz,plantx,planty,plantz;
    orgx=ui->lineEdit_tool_org_x->text();
    orgy=ui->lineEdit_tool_org_y->text();
    orgz=ui->lineEdit_tool_org_z->text();
    //    xdirx=ui->lineEdit_tool_xdir_x->text();
    //    xdiry=ui->lineEdit_tool_xdir_y->text();
    //    xdirz=ui->lineEdit_tool_xdir_z->text();
    //    plantx=ui->lineEdit_tool_xoy_x->text();
    //    planty=ui->lineEdit_tool_xoy_y->text();
    //    plantz=ui->lineEdit_tool_xoy_z->text();
    if(orgx.isEmpty()||orgy.isEmpty()||orgz.isEmpty()) return;

    myoccview->reset();
    basecoord=NULL;
    _toolcd=NULL;
    showshapes(orgx.toFloat(),orgy.toFloat(),orgz.toFloat(),orgx.toFloat()+1,orgy.toFloat(),orgz.toFloat(),orgx.toFloat(),orgy.toFloat()+1,orgz.toFloat());
    workshape=_toolcd->getshape();

    orgpoint.setX(_toolcd->getorg().X());
    orgpoint.setY(_toolcd->getorg().Y());
    orgpoint.setZ(_toolcd->getorg().Z());
    toolx.setX(_toolcd->getxdir().X());
    toolx.setY(_toolcd->getxdir().Y());
    toolx.setZ(_toolcd->getxdir().Z());
    tooly.setX(_toolcd->getydir().X());
    tooly.setY(_toolcd->getydir().Y());
    tooly.setZ(_toolcd->getydir().Z());
    toolz.setX(_toolcd->getzdir().X());
    toolz.setY(_toolcd->getzdir().Y());
    toolz.setZ(_toolcd->getzdir().Z());
    ui->widget_setting->setEnabled(true);
    emit buildtoolcoord();

}

void DetecSting::on_pushButton_tool_trans_clicked()
{
    QString dx,dy,dz,angle;
    float transx,transy,transz,rotangle;
    char axis;
    dx=ui->lineEdit_tool_trans_x->text();
    dy=ui->lineEdit_tool_trans_y->text();
    dz=ui->lineEdit_tool_trans_z->text();
    angle=ui->lineEdit__tool_rot_x->text();
    transx = dx.isEmpty()? 0.0:dx.toFloat();
    transy = dy.isEmpty()? 0.0:dy.toFloat();
    transz = dz.isEmpty()? 0.0:dz.toFloat();
    rotangle= angle.isEmpty()? 0.0 :angle.toFloat();
    if(ui->radioButton_x->isChecked())
        axis='x';
    else if(ui->radioButton_y->isChecked())
        axis='y';
    else if(ui->radioButton_z->isChecked())
        axis='z';
    if(_toolcd==NULL)
        return;
    TopoDS_Shape shape=currentshap;
    _toolcd->removeshape();
    _toolcd->addshapes(shape);
    _toolcd->shaperotation(gp_Vec(transx,transy,transz),axis,rotangle*M_PI/180);
    workshape=_toolcd->getshape();
}

void DetecSting::on_pushButton_tool_rot_clicked()
{
    if(_toolcd==NULL) return;
    ui->lineEdit_tool_trans_x->setText(QString::number(0.0));
    ui->lineEdit_tool_trans_y->setText(QString::number(0.0));
    ui->lineEdit_tool_trans_z->setText(QString::number(0.0));
    ui->lineEdit__tool_rot_x->setText(QString::number(0.0));
    TopoDS_Shape shape=currentshap;
    _toolcd->removeshape();
    _toolcd->addshapes(shape);
    workshape=_toolcd->getshape();
    myoccview->showgrounds(true);
    myoccview->fitAll();
}

void DetecSting::on_pushButton_tool_plan_clicked()
{
    if(pathpackge.size()<1)
        return;
    emit sendpathpoint(pathpackge);

}

void DetecSting::showshape(QStringList &list)
{
    //qDebug()<<list;
    if(list.size()<1) return;

    //myview->reset();
    //myview->showcycground();
    //myview->makexyz();

    QString type=list.at(0);
    QVector<float> para;
    para.clear();
    for(int i=1;i<list.size();++i)
    {
        para<<QString(list.at(i)).toFloat();
    }
    if(type=="box")
    {
        //myview->drawcube(0,0,0,para.at(0),para.at(1),para.at(2));

    }else if(type=="cycler")
    {
        //myview->drawcylinder(0,0,0,para.at(0),para.at(1));
    }
    else if(type=="pipe")
    {
        //myview->drawpipe(0,0,0,para.at(0),para.at(1),para.at(2));
    }else if(type=="line")
    {
        //myview->drawLine(0,0,0,para.at(0));
    }else if(type=="arc")
    {
        //myview->drawarc(0,0,0,para.at(0));
    }else if(type=="wedge")
    {
        //myview->drawwedge(para.at(0),para.at(1),para.at(2),para.at(3),para.at(4),para.at(5),para.at(6));
    }else if(type=="cyclerfan")
    {
        //myview->drawcylinderfan(0,0,0,para.at(0),para.at(1),para.at(2));

    }else if(type=="pipefan")
    {
        //myview->drawpipefan(0,0,0,para.at(0),para.at(1),para.at(2),para.at(3));
    }else
    {
        return;
    }
}

void DetecSting::getshapepara(TopoDS_Shape &list)
{
    //qDebug()<<list;
    //if(list.size()<1) return;
    currentshap=list;
    workshape=list;
    hasmodelshape=true;
    //qDebug()<<"DetecSting::getshapepara"<<currentshapetype;
    ui->stackedWidget_path->setCurrentIndex(currentshapetype);
}

void DetecSting::initForms()
{
    this->max=false;
    this->location=this->geometry();
    this->setProperty("form",true);
    this->setProperty("canmove",true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    IconHelper::Instance()->setIcon(ui->label_icon,QChar(0xF099),30);
    IconHelper::Instance()->setIcon(ui->pushButton_min,QChar(0xf068));
    IconHelper::Instance()->setIcon(ui->pushButton_max,QChar(0xf067));
    IconHelper::Instance()->setIcon(ui->pushButton_close,QChar(0xf00d));
    ui->widgetTitle->setProperty("form","title");
    ui->widgettop->setProperty("nav","top");
    ui->label_text->setText("检测设置");
    ui->label_text->setFont(QFont("Microsoft YaHei",20));
    this->setWindowTitle(ui->label_text->text());
    QSize iconsize(32,32);
    int icowidth=85;
    QList<QToolButton*> tbtns=ui->widgettop->findChildren<QToolButton* >();
    foreach(QToolButton *btn,tbtns)
    {
        btn->setIconSize(iconsize);
        btn->setMinimumWidth(icowidth);
    }

    QList<int> widths;
    widths<<650<<170;
    ui->splitterH->setSizes(widths);
    QList<int> heights;
    heights<<500<<100;
    ui->splitterV->setSizes(heights);

    //myview=new OccView(this);
    myoccview=new myview();
    QVBoxLayout *layout=new QVBoxLayout();
    //layout->addWidget(myview);
    layout->addWidget(myoccview);
    ui->frame_3d->setLayout(layout);
    ui->widgetTitle->hide();
    ui->stackedWidget_path->setCurrentIndex(currentshapetype);


}

void DetecSting::showshapes(const float ox, const float oy, const float oz, const float x, const float y, const float z, const float px, const float py, const float pz)
{
    if(basecoord==NULL)
    {
        basecoord=new ToolCoord(myoccview->m_get_context());
        basecoord->ToolCoordParame(0,0,0,1,0,0,0,1,0);
        basecoord->maketoolxyz(200);
    }
    if(_toolcd==NULL)
    {
        _toolcd=new ToolCoord(myoccview->m_get_context());
        _toolcd->ToolCoordParame(ox,oy,oz,x,y,z,px,py,pz);
        _toolcd->maketoolxyz(100);
    }
    TopoDS_Shape shape=currentshap;
    _toolcd->addshapes(shape);

    myoccview->showgrounds(true);
    myoccview->fitAll();
}

void DetecSting::printpathpoint()
{
    if(pathpoint.size()<1)
    {
        qDebug()<<"can not get path point";
        return;
    }
    qDebug()<<"DetecSting::printpathpoint";
    for(int i=0;i<pathpoint.size();++i)
    {
        qDebug()<<pathpoint.at(i).X()<<pathpoint.at(i).Y()<<pathpoint.at(i).Z();
    }
}

float DetecSting::getlength(const QVector<gp_Pnt> &array)
{
    if(array.size()<1) return 0;
    gp_Pnt p1=array.at(0);
    float dis=0;
    for(int i=1;i<array.size();++i)
    {
        dis+=p1.Distance(array.at(i));
        p1=array.at(i);
    }
    return  dis;
}

Robot_Path DetecSting::setpathformvec(QVector<gp_Pnt> array,const QVector3D qnorm, const QString str)
{
    Robot_Path subpath;
    subpath.type=str;
    for(int i=0;i<array.size();++i)
    {
        subpath.point.push_back(QVector3D(array.at(i).X(),array.at(i).Y(),array.at(i).Z()));
        subpath.norml.push_back(qnorm);
    }
    subpath.length=getlength(array);
    return subpath;
}

QVector3D DetecSting::rpyformnormal(gp_Vec norm, gp_Vec px)
{
    gp_Vec vz=-norm;
    gp_Vec vy=-px.Normalized();
    gp_Vec vx=vy.Crossed(vx);
    float r[3][3];
    float rx,ry,rz;
    r[0][0]=vx.X();r[0][1]=vy.X();r[0][2]=vz.X();
    r[1][0]=vx.Y();r[1][1]=vy.Y();r[1][2]=vz.Y();
    r[2][0]=vx.Z();r[2][1]=vy.Z();r[2][2]=vz.Z();

    qDebug()<<vx.X()<<vy.X()<<vz.X();
    qDebug()<<vx.Y()<<vy.Y()<<vz.Y();
    qDebug()<<vx.Z()<<vy.Z()<<vz.Z();
    float sy=sqrt(r[2][1]*r[2][1]+r[2][2]*r[2][2]);
    //float sy=sqrt(r[0][0]*r[0][0]+r[1][0]*r[1][0]);
    if(sy>=1e-6)
    {
        rx=atan2(r[2][1],r[2][2]);
        ry=atan2(-r[2][0],sy);
        rz=atan2(r[1][0],r[0][0]);
    }else
    {
        rx=atan2(-r[1][2],r[1][1]);
        ry=atan2(-r[2][0],sy);
        rz=0;
    }
    //    double sin2=vz.X();
    //    double cos21=sqrt(1-pow(vz.X(),2));
    //    double cos22=-sqrt(1-pow(vz.X(),2));
    //    double cos2;
    //    if(cos21-cos22>1e-6)
    //        cos2=cos21;
    //    else
    //        cos2=cos22;

    //    ry=atan2(sin2,cos2);
    //    double sin1=-vz.Y()/cos2;
    //    double cos1=vz.Z()/cos2;
    //    rx=atan2(sin1,cos1);
    //    double sin3=-vx.X()/cos2;
    //    double cos3=vy.X()/cos2;
    //    rz=atan2(sin3,cos3);



    QVector3D rpy=QVector3D(rx*180/M_PI,ry*180/M_PI,rz*180/M_PI);
    qDebug()<<"DetecSting::rpyformnormal"<<rpy;
    return rpy;
}

bool DetecSting::isrotationmatrix(QMatrix4x4 &R)
{
    QMatrix4x4 Rt;
    Rt=R;

    return true;

}

void DetecSting::setfacepathformvec(const QVector3D qnorm)
{
    pathpackge.clear();
    gp_Pnt pstart,pend;
    pstart=pathpoint.first();
    int sz=pathpoint.size()/2;
    for(int i=1;i<sz;++i)//路径按照线段进行打包
    {
        QVector<gp_Pnt> subpath;
        if(i%2==1)
        {
            pend=pathpoint.at(sz+i-1);
            getalinepath(pstart,pend,qnorm,true);
            pstart=pend;
            pend=pathpoint.at(sz+i);
            getalinepath(pstart,pend,qnorm,false);
            pstart=pend;
        }
        if(i%2==0)
        {
            pend=pathpoint.at(i-1);
            getalinepath(pstart,pend,qnorm,true);
            pstart=pend;
            pend=pathpoint.at(i);
            getalinepath(pstart,pend,qnorm,false);
            pstart=pend;
        }
        if(i==sz-1)
        {
            pend=pathpoint.at(i);
            getalinepath(pstart,pend,qnorm,false);
            pstart=pend;
        }
    }
}

TopoDS_Face DetecSting::getshapeface(TopoDS_Shape &_shape, int index)//index order is left right front back bottom up
{
    TopoDS_Face face;
    int count=0;int zMax=-1;
    TopoDS_Shape shape=_shape;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);



            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.Z();
            if(count==index){
                zMax = aZ;
                face=aFace;
            }
        }
    }
    return  face;
}

TopoDS_Wire DetecSting::getwirefromface(TopoDS_Face &face)
{
    ShapeAnalysis_FreeBounds afreebounds(face,Standard_False,Standard_True);
    TopoDS_Compound aclosedwires=afreebounds.GetClosedWires();
    TopTools_IndexedMapOfShape awires;
    TopExp::MapShapes(aclosedwires,TopAbs_WIRE,awires);
    TopoDS_Wire awire;
    Standard_Integer nbWires=awires.Extent();
    if(nbWires)
        awire=TopoDS::Wire(awires(1));
    return awire;
}

QVector<gp_Pnt> DetecSting::pathpointfromface(TopoDS_Face face, int step)
{
    QVector<gp_Pnt> vec;
    QVector<gp_Pnt> vecline1;
    QVector<gp_Pnt> vecline2;
    if(face.IsNull())
        return vec;
    for(TopExp_Explorer Ex(face,TopAbs_EDGE);Ex.More();Ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(Ex.Current());
        Standard_Real first,last;
        Handle(Geom_Curve) acure=BRep_Tool::Curve(edge,first,last);

        gp_Pnt strpnt=acure->Value(first);
        gp_Pnt endpnt=acure->Value(last);
        //printpnt(strpnt);
        //printpnt(endpnt);


        if(vecline1.size()==0)
        {
            vecline1.push_back(strpnt);
            vecline1.push_back(endpnt);
        }
        else
        {
            if(isequal(vecline1.last(),strpnt))
            {
                vecline1.push_back(endpnt);
            }
            else if(vecline2.size()==0)
            {
                vecline2.push_front(endpnt);
                vecline2.push_front(strpnt);
            }else if(isequal(vecline2.first(),endpnt))
            {
                vecline2.push_front(strpnt);
            }
        }

    }
    if(isequal(vecline1.first(),vecline2.first()))
        vecline2.pop_front();
    if(isequal(vecline1.last(),vecline2.last()))
        vecline1.pop_back();
    Standard_Real d1=distansall(vecline1);
    Standard_Real d2=distansall(vecline2);
    int st=0;
    if(d1 >= d2)
        st=ceil(d1 / step);
    else
        st=ceil(d2 / step);
    splitpath(vecline1,st,vec);
    splitpath(vecline2,st,vec);
    //qDebug()<<vec;
    return vec;
}

QVector<gp_Pnt> DetecSting::pathpointfromwire(TopoDS_Wire wire)
{
    QVector<gp_Pnt> pvec;
    TopExp_Explorer ex(wire,TopAbs_EDGE);
    for(;ex.More();ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(ex.Current());
        Standard_Real first;
        Standard_Real end;
        Handle(Geom_Curve) acure=BRep_Tool::Curve(edge,first,end);
        gp_Pnt strpnt=acure->Value(first);
        gp_Pnt endpnt=acure->Value(end);

        pvec.push_back(strpnt);
        pvec.push_back(endpnt);

        //        BRep_Tool::Range(edge,first,end);
        //        Handle(Geom_Curve) cur=BRep_Tool::Curve(edge,first,end);
        //        for(Standard_Integer i=first;i<end;++i)
        //        {
        //            gp_Pnt pi;
        //            cur->D0(i,pi);
        //            pvec.push_back(pi);
        //            //qDebug()<<"point "<<i<<"is"<<pi.X()<<pi.Y()<<pi.Z();
        //        }
    }

    return  pvec;
}

void DetecSting::getalinepath(gp_Pnt start, gp_Pnt end,QVector3D qnorm,bool isneedscan)
{
    QVector<gp_Pnt> subpath;
    subpath.push_back(start);
    subpath.push_back(end);
    QString str=QString("face");
    Robot_Path tpath=setpathformvec(subpath,qnorm,str);
    tpath.isneedscan=isneedscan;
    pathpackge.push_back(tpath);
}

void DetecSting::mousePressEvent(QMouseEvent *event)
{
    if(max)
    {
        return;
    }else
    {
        ispress=true;
        startpos=event->globalPos();
    }
    return QWidget::mousePressEvent(event);
}

void DetecSting::mouseMoveEvent(QMouseEvent *event)
{
    if(ispress)
    {
        QPoint movepoint=event->globalPos()-startpos;
        QPoint wdpos=this->pos();
        startpos=event->globalPos();
        //this->move(wdpos.x()+movepoint.x(),wdpos.y()+movepoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void DetecSting::mouseReleaseEvent(QMouseEvent *event)
{
    ispress=false;
    return QWidget::mouseReleaseEvent(event);
}

void DetecSting::on_toolButton_loop_clicked()
{
    //if(currentshap.size()<1)return;
    //myview->showshapefromlist(currentshap);
}

void DetecSting::on_pushButton_min_clicked()
{
    showMinimized();
}

void DetecSting::on_pushButton_max_clicked()
{
    if(max){
        this->setGeometry(location);
        this->setProperty("canmove",true);
    }else{
        location=this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        this->setProperty("canmove",false);
    }
    max=!max;
}

void DetecSting::on_pushButton_close_clicked()
{
    close();
}

void DetecSting::on_radioButton_up_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    TopoDS_Face face=getshapeface(workshape,6);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    setfacepathformvec(qnorm);
    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::radioButton_down_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    TopoDS_Face face=getshapeface(workshape,5);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    setfacepathformvec(qnorm);

    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::on_radioButton_right_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    TopoDS_Face face=getshapeface(workshape,2);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    qDebug()<<qnorm;
    setfacepathformvec(qnorm);
    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::on_radioButton_left_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    TopoDS_Face face=getshapeface(workshape,1);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    setfacepathformvec(qnorm);
    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::on_radioButton_front_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    TopoDS_Face face=getshapeface(workshape,3);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    setfacepathformvec(qnorm);
    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::on_radioButton_back_surface_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    TopoDS_Face face=getshapeface(workshape,4);
    pathpoint=pathpointfromface(face);
    QVector3D qnorm;
    qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    setfacepathformvec(qnorm);
    myoccview->removeshape();
    myoccview->addshapetocontent(face);
}

void DetecSting::on_radioButton_up_edge_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    TopoDS_Face face= getshapeface(workshape,6);
    TopoDS_Wire wire=getwirefromface(face);
    pathpoint=pathpointfromwire(wire);
    de_duplication(pathpoint);//去重
    sortgppntlist(_toolcd->getorg(),_toolcd->getxdir(),pathpoint);//排序，沿着x方向逆时针


    QVector3D qnorm;
    //qnorm=rpyformnormal(norm,gp_Vec(pathpoint.first(),pathpoint.at(1)));
    qnorm=eulerformpoint(pathpoint.at(0),pathpoint.at(1),pathpoint.last());//法线计算欧拉角
    pathpoint.push_back(pathpoint.first());//最后添加第一个点作为封闭曲线；
    //printpathpoint();
    pathpackge.clear();
    for(int i=0;i<pathpoint.size()-1;++i)//路径按照线段进行打包
    {
        QVector<gp_Pnt> subpath;
        subpath.push_back(pathpoint.at(i));
        subpath.push_back(pathpoint.at(i+1));
        QString str=QString("edge_%1").arg(i+1);
        Robot_Path tpath=setpathformvec(subpath,qnorm,str);
        //Robot_Path tpath=setpathformvec(subpath,_up_face_norm,str);
        tpath.isneedscan=true;
        //qDebug()<<"on_radioButton_up_edge_clicked"<<tpath.point;
        pathpackge.push_back(tpath);
    }
    myoccview->removeshape();
    myoccview->addshapetocontent(wire);
}

void DetecSting::on_radioButton_down_edge_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    box _box(workshape);
    _toolcd->removeshape();
    TopoDS_Face face=_box.bottomface();
    TopoDS_Wire wire=_box.getedge(face);
    pathpoint=_box.pathpoint(wire);
    de_duplication(pathpoint);//去重
    sortgppntlist(_toolcd->getorg(),_toolcd->getxdir(),pathpoint);//排序，沿着x方向逆时针
    printpathpoint();

    QVector3D qnorm;
    //gp_Vec norm=_box.getnormal(face);
    //qnorm=rpyformnormal(norm,gp_Vec(pathpoint.first(),pathpoint.at(1)));

    pathpoint.push_back(pathpoint.first());//最后添加第一个点作为封闭曲线；

    pathpackge.clear();
    for(int i=0;i<pathpoint.size()-1;++i)//路径按照线段进行打包
    {
        QVector<gp_Pnt> subpath;
        subpath.push_back(pathpoint.at(i));
        subpath.push_back(pathpoint.at(i+1));
        QString str=QString("edge_%1").arg(i+1);
        int p;
        if(i==0)
            p=3;
        else
            p=i-1;
        qnorm=rpyformnormal(gp_Vec(pathpoint.at(p),pathpoint.at(i)).Normalized(),gp_Vec(pathpoint.at(i),pathpoint.at(i+1)));
        //qnorm=eulerformzox(pathpoint.at(p),pathpoint.at(i),pathpoint.at(i+1));//法线计算欧拉角
        Robot_Path tpath=setpathformvec(subpath,qnorm,str);
        tpath.isneedscan=true;
        //qDebug()<<"on_radioButton_up_down_clicked"<<tpath.point;
        pathpackge.push_back(tpath);
    }
    myoccview->removeshape();
    myoccview->addshapetocontent(wire);
}

void DetecSting::on_radioButton_sweep_edge_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    //    TopoDS_Face face= getshapeface(workshape,0);
    //    TopoDS_Wire wire=getwirefromface(face);
    //    pathpoint=pathpointfromwire(wire);
    //    printpathpoint();
    QVector<gp_Pnt> vertexArray;
    BRepMesh_IncrementalMesh(workshape,0.001);
    gp_Pnt vertex1;
    gp_Pnt vertex2;
    gp_Pnt vertex3;
    TopExp_Explorer faceExplorer;
    for(faceExplorer.Init(workshape,TopAbs_FACE);faceExplorer.More();faceExplorer.Next())
    {
        TopLoc_Location loc;
        TopoDS_Face aFace=TopoDS::Face(faceExplorer.Current());
        QVector<QVector3D> pvec;

        TopoDS_Wire wire=getwirefromface(aFace);
        TopExp_Explorer ex(wire,TopAbs_EDGE);
        for(;ex.More();ex.Next())
        {
            TopoDS_Edge edge=TopoDS::Edge(ex.Current());
            Standard_Real first;
            Standard_Real end;
            BRep_Tool::Range(edge,first,end);
            Handle(Geom_Curve) cur=BRep_Tool::Curve(edge,first,end);
            if(cur->IsInstance("Geom_Line"))
            {
                gp_Pnt pi;
                pi=cur->Value(first);
                pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
                pi=cur->Value(end);
                pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
            }else
            {
                for(Standard_Integer i=first;i<end;i++)
                {
                    gp_Pnt pi;
                    cur->D0(i,pi);
                    pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
                }
            }
        }
        pathpoint=pathpointfromwire(wire);
        myoccview->addshapetocontent(wire);
    }
}

void DetecSting::on_radioButton_input_cheoice_clicked()
{
    if(workshape.IsNull())
    {
        return;
    }
    _toolcd->removeshape();
    TopoDS_Face face=getshapeface(workshape,6);
    //pathpoint=pathpointfromface(face);
    //printpathpoint();
    //QVector3D qnorm;
    //qnorm=eulerformpoint(pathpoint.at(1),pathpoint.at(0),pathpoint.last());//法线计算欧拉角
    //setfacepathformvec(qnorm);
//    int zmax=-1; int count=0;
//    for(TopExp_Explorer ex(workshape,TopAbs_WIRE);ex.More();ex.Next())
//    {
//        TopoDS_Wire awire=TopoDS::Wire(ex.Current());
//        count++;


//    }

    BRepMesh_IncrementalMesh(workshape,0.001);
    gp_Pnt vertex1;
    gp_Pnt vertex2;
    gp_Pnt vertex3;
    TopExp_Explorer faceExplorer;
    for(faceExplorer.Init(workshape,TopAbs_FACE);faceExplorer.More();faceExplorer.Next())
    {
        TopLoc_Location loc;
        TopoDS_Face aFace=TopoDS::Face(faceExplorer.Current());
        QVector<QVector3D> pvec;

        TopoDS_Wire wire=getwirefromface(aFace);
        TopExp_Explorer ex(wire,TopAbs_EDGE);
        for(;ex.More();ex.Next())
        {
            TopoDS_Edge edge=TopoDS::Edge(ex.Current());
            Standard_Real first;
            Standard_Real end;
            BRep_Tool::Range(edge,first,end);
            Handle(Geom_Curve) cur=BRep_Tool::Curve(edge,first,end);
            if(cur->IsInstance("Geom_Line"))
            {
                gp_Pnt pi;
                pi=cur->Value(first);
                pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
                pi=cur->Value(end);
                pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
            }else
            {
                for(Standard_Integer i=first;i<end;i++)
                {
                    gp_Pnt pi;
                    cur->D0(i,pi);
                    pvec.push_back(QVector3D(pi.X(),pi.Y(),pi.Z()));
                }
            }
        }
        qDebug()<<"pvec is ok!!!"<<pvec.size()<<"\n"<<pvec;

    }


    //myoccview->removeshape();
    //myoccview->addshapetocontent(aFace);

}
