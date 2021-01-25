#include "circular.h"
#include <QDebug>

circular::circular(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real radius, const Standard_Real _angle)
{
  this->_radius=radius;
  this->_angle=_angle;
  this->_position=gp_Pnt(x, y, z);
  gp_Ax1 anAxis;
  anAxis.SetLocation(gp_Pnt(x, y, z));
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(radius+x, y, z));
  _cirular = new BRepPrimAPI_MakeRevol(aVertex, anAxis,_angle);
}

circular::circular(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real radius)
{
  this->_radius=radius;
  this->_position=gp_Pnt(x, y, z);
  gp_Ax1 anAxis;
  anAxis.SetLocation(gp_Pnt(x, y, z));
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(radius+x, y, z));
  _cirular = new BRepPrimAPI_MakeRevol(aVertex, anAxis);
}

circular::circular(const gp_Pnt position, const gp_Dir dir, const Standard_Real radius)
{
  this->_radius=radius;
  this->_position=position;
  this->_dir=dir;

  gp_Ax1 anAxis(position,dir);
  gp_Pnt point;
  calculearcparamet(position,dir,radius,point);

  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(point);
  _cirular = new BRepPrimAPI_MakeRevol(aVertex, anAxis);
}

circular::circular(const gp_Pnt position, const gp_Dir dir, const Standard_Real radius, const Standard_Real _angle)//error
{
  this->_radius=radius;
  this->_position=position;
  this->_dir=dir;
  this->_angle=_angle;
  gp_Ax1 anAxis(position,dir);
  gp_Pnt point;
  calculearcparamet(position,dir,radius,point);
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(point);
  _cirular = new BRepPrimAPI_MakeRevol(aVertex, anAxis,_angle);
}

circular::circular(const gp_Pnt p1, const gp_Pnt p2, gp_Pnt p3)
{
  calculearcparamet(p1,p2,p3);
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(p1);
  gp_Ax1 anAxis(_position,_dir);
  _cirular = new BRepPrimAPI_MakeRevol(aVertex, anAxis);
}

void circular::calculearcparamet(const gp_Pnt p1, const gp_Pnt p2, gp_Pnt p3)
{
  double  k11, k12, k13, k14;
   k11 = (p1.X() - p3.X())*(p2.Z() - p3.Z()) - (p2.Y() - p3.Y())*(p1.Z() - p3.Z());
   k12 = (p2.X() - p3.X())*(p1.Z() - p3.Z()) - (p1.X() - p3.X())*(p2.Z() - p3.Z());
   k13 = (p1.X() - p3.X())*(p2.Y() - p3.Y()) - (p2.X() - p3.X())*(p1.Y() - p3.Y());
   k14 = -(k11*p3.X() + k12*p3.Y() + p3.Z());

   double  k21, k22, k23, k24;
   k21 = p2.X() - p1.X();
   k22 = p2.Y() - p1.Y();
   k23 = p2.Z() - p1.Z();
   k24 = ((pow(p2.X(), 2) - pow(p1.X(), 2) + pow(p2.Y(), 2) - pow(p1.Y(), 2) + pow(p2.Z(), 2) - pow(p1.Z(), 2))) / 2.0;

   double  k31, k32, k33, k34;
   k31 = p3.X() - p2.X();
   k32 = p3.Y() - p2.Y();
   k33 = p3.Z() - p2.Z();
   k34 = ((pow(p3.X(), 2) - pow(p2.X(), 2) + pow(p3.Y(), 2) - pow(p2.Y(), 2) + pow(p3.Z(), 2) - pow(p2.Z(), 2))) / 2.0;
   //求圆心半径
   double x0, y0, z0, r;
   x0 = (-k14*k22*k33 - k24*k32*k13 - k34*k12*k23 + k13*k22*k34 + k33*k12*k24 + k23*k32*k14) / (k11*k22*k33 + k12*k23*k31 + k21*k32*k13 - k13*k22*k31 - k12*k21*k33 - k23*k31*k11);
   y0 = (-k11*k24*k33 - k21*k34*k13 - k14*k23*k31 + k13*k24*k31 + k23*k34*k11 + k14*k21*k33) / (k11*k22*k33 + k12*k23*k31 + k21*k32*k13 - k13*k22*k31 - k12*k21*k33 - k23*k31*k11);
   z0 = (-k11*k22*k34 - k21*k32*k14 - k12*k24*k31 + k14*k22*k31 + k12*k21*k34 + k24*k32*k11) / (k11*k22*k33 + k12*k23*k31 + k21*k32*k13 - k13*k22*k31 - k12*k21*k33 - k23*k31*k11);
   gp_Pnt P0(x0, y0, z0);
   r = sqrt(pow(p1.X() - P0.X(), 2) + pow(p1.Y() - P0.Y(), 2) + (pow(p1.Z() - P0.Z(), 2)));
   this->_position=P0;
   this->_radius=r;
   gp_Vec v1(p1,p2);
   gp_Vec v2(p1,p3);

   gp_Vec v3=v1.Crossed(v2);

   this->_dir=v3.Normalized();


}

void circular::calculearcparamet(const gp_Pnt position, const gp_Dir dir, const Standard_Real radius, gp_Pnt &point)
{
  //法线，点求平面方程 （x-x1）a+（y-y1）b+（z-z1）c=0；

  //球面方程
  Standard_Real vx,vy,vz;
  dir.Coord(vx,vy,vz);

  gp_Pnt pn=position;

  if(vx==0)
    pn.SetX(pn.X()+radius);
  else if(vy==0)
    pn.SetY(pn.Y()+radius);
  else if(vz==0)
    pn.SetZ(pn.Z()+radius);
  else
  {
   pn.SetX(pn.X()+radius);
   pn.SetZ(pn.Z()-radius*vx/vz);
  }
    gp_Dir norm(pn.X()-position.X(),pn.Y()-position.Y(),pn.Z()-position.Z());
    gp_Vec vec= gp_Vec(norm).Normalized()*radius;
     pn=gp_Pnt(position.X()+vec.X(),position.Y()+vec.Y(),position.Z()+vec.Z());

    //Standard_Real angle= dir.Angle(norm);
    //qDebug()<<angle;
    //qDebug()<<"norm"<<pn.X()<<pn.Y()<<pn.Z();
     point=pn;




}
