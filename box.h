#ifndef BOX_H
#define BOX_H
#include "occ.h"

class box
{
public:
  box(const TopoDS_Shape &shape);
  box(const Standard_Real dx,const Standard_Real dy,const Standard_Real dz);
  box(const Standard_Real x,const Standard_Real y,const Standard_Real z,const Standard_Real _dx, const Standard_Real _dy,const  Standard_Real _dz);//给定第一个角位置和长宽高，创建box
  box(const gp_Pnt location,const Standard_Real _dx, const Standard_Real _dy, const Standard_Real _dz);//给定第一个角位置点，创建box
  box(const gp_Pnt location,const gp_Pnt corner);//根据两个对角坐标创建box
  box(const gp_Ax2 axes,const Standard_Real _dx,const  Standard_Real _dy, const Standard_Real _dz);//根据角的位置和方向，创建box，其中第一条边将沿着给定的轴方向；
  TopoDS_Shape getshape(){
    return _box->Shape();
  }
  TopoDS_Face topface()
  {
    return  _box->TopFace();
  }
  TopoDS_Face frontface()
  {
    return  _box->FrontFace();
  }
  TopoDS_Face backface()
  {
    return  _box->BackFace();
  }
  TopoDS_Face bottomface()
  {
    return  _box->BottomFace();
  }
  TopoDS_Face rightface()
  {
    return  _box->RightFace();
  }
  TopoDS_Face leftface()
  {
    return  _box->LeftFace();
  }
  TopoDS_Wire getedge(TopoDS_Face face);
  QVector<gp_Pnt> pathpoint(TopoDS_Wire wire);
  QVector<gp_Pnt> pathpoint(TopoDS_Face face);
  gp_Vec getnormal(TopoDS_Face face);
private:
  void getboxcorner(TopoDS_Shape &shape,Standard_Real &p1,Standard_Real &p2,Standard_Real &p3,gp_Ax2 &axis);
private:
  BRepPrimAPI_MakeBox *_box=NULL;






};

#endif // BOX_H
