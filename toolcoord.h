#ifndef TOOLCOORD_H
#define TOOLCOORD_H
#include "occ.h"
#include "myview.h"
#include <QVector4D>
#include <QMatrix4x4>
#include "globalvar.h"

class ToolCoord
{
public:
  ToolCoord(const Handle(AIS_InteractiveContext) m_context):myContext(m_context){

  };
  bool ToolCoordParame(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2);
  bool ToolCoordParame(gp_Pnt org,gp_Dir xdir,gp_Dir ydir);
  void maketoolxyz(Standard_Real length);
  void addshapes(TopoDS_Shape &shape);
  void removeshape();
  void shapetransform();
  void shaperotation(const gp_Vec trans,const char axis,const Standard_Real angle);
  QMatrix4x4 getshapetransformation(TopoDS_Shape &shape);

  TopoDS_Shape getshape()
  {
      return  _shape->Shape();
  }
  gp_Pnt getorg()
  {
      return  suborg;
  }
  gp_Vec getxdir()
  {
      return  subvx;
  }
  gp_Vec getydir()
  {
      return  subvy;
  }
  gp_Vec getzdir()
  {
      return  subvz;
  }
private:
  TopoDS_Shape shapetranslation(TopoDS_Shape shape,const char f,const Standard_Real dist);

private:
  gp_Vec subvx=gp_Vec(1,0,0);
  gp_Vec subvy=gp_Vec(0,1,0);
  gp_Vec subvz=gp_Vec(0,0,1);
  gp_Pnt suborg=gp_Pnt(0,0,0);
  gp_Trsf toolcood;
  Handle(AIS_InteractiveContext) myContext=NULL;
  Handle(AIS_Shape) _shape;


};

#endif // TOOLCOORD_H
