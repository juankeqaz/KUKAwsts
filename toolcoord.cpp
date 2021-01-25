#include "toolcoord.h"
#include <QDebug>

bool ToolCoord::ToolCoordParame(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
{
  gp_Pnt p0=gp_Pnt(x0,y0,z0);
  gp_Pnt p1=gp_Pnt(x1,y1,z1);
  gp_Pnt p2=gp_Pnt(x2,y2,z2);
  if(p0.Distance(p1)<=0.0001||p0.Distance(p2)<=0.0001||p1.Distance(p2)<=0.0001)
    {
      qDebug()<< "point paramet error";
      return false;
    }
  gp_Vec v01=gp_Vec(p0,p1);
  gp_Vec v02=gp_Vec(p0,p2);
  Standard_Real angle=v01.Angle(v02);
  if(angle <= 0.0001|| qAbs(angle-M_PI) <= 0.0001)
    {
      qDebug()<<"three point in a lines";
      return false;
    }else
    {
      suborg=p0;
      subvx=v01.Normalized();
      subvz=subvx.Crossed(v02);
      subvy=subvz.Crossed(subvx);
    }
  toolcood.SetValues(subvx.X(),subvy.X(),subvz.X(),suborg.X(),subvx.Y(),subvy.Y(),subvz.Y(),suborg.Y(),subvx.Z(),subvy.Z(),subvz.Z(),suborg.Z());
  return  true;
  //maketoolxyz(200);
}

bool ToolCoord::ToolCoordParame(gp_Pnt org, gp_Dir xdir, gp_Dir ydir)
{
  suborg=org;
  subvx=xdir;
  Standard_Real angle=xdir.Angle(ydir);
  if(abs(angle)<1e-4)
    return false;
  subvz = xdir.Crossed(ydir);
  subvy = subvz.Crossed(xdir);
  toolcood.SetValues(subvx.X(),subvy.X(),subvz.X(),suborg.X(),subvx.Y(),subvy.Y(),subvz.Y(),suborg.Y(),subvx.Z(),subvy.Z(),subvz.Z(),suborg.Z());
  return true;
}

void ToolCoord::maketoolxyz(Standard_Real length)
{
  TopoDS_Shape center=BRepPrimAPI_MakeSphere(suborg,1.5);
  TopoDS_Shape xaxis=BRepPrimAPI_MakeCylinder(gp_Ax2(suborg,subvx),0.8,length);

  gp_Pnt pn=suborg.Translated(subvx.Normalized()*length);
  TopoDS_Shape xheader = BRepPrimAPI_MakeCone(gp_Ax2(pn,subvx),1.5,0.0,5.0);
  xaxis = BRepAlgoAPI_Fuse(xaxis,xheader);

  TopoDS_Shape yaxis=BRepPrimAPI_MakeCylinder(gp_Ax2(suborg,subvy),0.8,length);
  gp_Pnt py=suborg.Translated(subvy.Normalized()*length);
  TopoDS_Shape yheader = BRepPrimAPI_MakeCone(gp_Ax2(py,subvy),1.5,0.0,5.0);
  yaxis = BRepAlgoAPI_Fuse(yaxis,yheader);

  TopoDS_Shape zaxis=BRepPrimAPI_MakeCylinder(gp_Ax2(suborg,subvz),0.8,length);
  gp_Pnt pz=suborg.Translated(subvz.Normalized()*length);
  TopoDS_Shape zheader = BRepPrimAPI_MakeCone(gp_Ax2(pz,subvz),2.0,0.0,5.0);
  zaxis = BRepAlgoAPI_Fuse(zaxis,zheader);


  Handle(AIS_Shape) ais_xaxis = new AIS_Shape(xaxis);
  Handle(AIS_Shape) ais_center = new AIS_Shape(center);
  Handle(AIS_Shape) ais_yaxis = new AIS_Shape(yaxis);
  Handle(AIS_Shape) ais_zaxis = new AIS_Shape(zaxis);



  myContext->SetColor(ais_xaxis,Quantity_NOC_RED1,Standard_False);
  myContext->SetColor(ais_yaxis,Quantity_NOC_GREEN1,Standard_False);
  myContext->SetColor(ais_zaxis,Quantity_NOC_BLUE2,Standard_False);
  myContext->SetColor(ais_center,Quantity_NOC_GRAY,Standard_False);

  myContext->Display(ais_xaxis,Standard_True);
  myContext->Display(ais_yaxis,Standard_True);
  myContext->Display(ais_zaxis,Standard_True);
  myContext->Display(ais_center,Standard_True);
}

void ToolCoord::addshapes(TopoDS_Shape &shape)
{
  //toolcood.Transforms(suborg.X(),suborg.Y(),suborg.Z());
  TopLoc_Location location=shape.Location();
  gp_Trsf trsf=location.Transformation();
  trsf=toolcood*trsf;
  location=trsf;
  shape.Move(location);
  plantmatrix=getshapetransformation(shape);
  workpicematrix.setToIdentity();
  Handle(AIS_Shape) ais_shape = new AIS_Shape(shape);
  _shape=ais_shape;
  myContext->Display(ais_shape,Standard_True);
}

void ToolCoord::removeshape()
{
    myContext->Remove(_shape,true);
}

void ToolCoord::shapetransform()
{

}

void ToolCoord::shaperotation(const gp_Vec trans, const char axis, const Standard_Real angle)
{

    if(_shape.IsNull()) return;
    myContext->Remove(_shape,true);
    TopoDS_Shape shape=_shape->Shape();

    gp_Trsf trsf;//=shape.Location().Transformation();
    shape.Move(trsf);

    gp_Ax1 p1;
    p1.SetLocation(suborg);
    if(axis=='x')
    {
        p1.SetDirection(subvx);
    }else if(axis=='y')
    {
        p1.SetDirection(subvy);
    }else if(axis=='z')
    {
        p1.SetDirection(subvz);
    }else
        return;
    trsf.SetRotation(p1,angle);
    //trsf.SetTranslationPart(trans);
    TopLoc_Location location;
    location=trsf;
    shape.Move(location);
    shape=shapetranslation(shape,'x',trans.X());
    shape=shapetranslation(shape,'y',trans.Y());
    shape=shapetranslation(shape,'z',trans.Z());
    //workpicematrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(p1.Direction().X(),p1.Direction().Y(),p1.Direction().Z()),angle*M_PI/180));
    Handle(AIS_Shape) ais_shape=new AIS_Shape(shape);
    _shape=ais_shape;
    myContext->Display(ais_shape,Standard_True);
    QMatrix4x4 qmat=getshapetransformation(shape);
    qmat(0,3)=trans.X();
    qmat(1,3)=trans.Y();
    qmat(2,3)=trans.Z();
    workpicematrix=qmat;
    //qDebug()<<"ToolCoord::shaperotation:"<<workpicematrix;
}

QMatrix4x4 ToolCoord::getshapetransformation(TopoDS_Shape &shape)
{
    gp_Trsf trsf=shape.Location();
    NCollection_Mat4<Standard_Real> mat4;
    mat4.InitIdentity();
    trsf.GetMat4(mat4);
    QMatrix4x4 qmat;
    qmat.setToIdentity();
    for(int i=0;i<4;++i)
    {
        for(int j=0;j<4;++j)
        {
            qmat(i,j)=mat4.GetValue(i,j);
        }
    }
    return qmat;

}

TopoDS_Shape ToolCoord::shapetranslation(TopoDS_Shape shape, const char f, const Standard_Real dist)
{
    gp_Trsf trsf=shape.Location();
    gp_Vec vx=dist*(subvx.Normalized());
    gp_Vec vy=dist*(subvy.Normalized());
    gp_Vec vz=dist*(subvz.Normalized());
    if(f=='x')
    {
        trsf.SetTranslation(vx);
    }
    else if(f=='y')
    {
        trsf.SetTranslation(vy);
    }else if(f=='z')
    {
        trsf.SetTranslation(vz);
    }else
        return shape;
    TopLoc_Location location;
    location=trsf;
    shape.Move(location);
    return shape;
}
