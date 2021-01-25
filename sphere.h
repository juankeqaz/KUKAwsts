#ifndef SPHERE_H
#define SPHERE_H
#include "occ.h"

class sphere
{
public:
  sphere(const Standard_Real R);

  sphere(const Standard_Real R, const Standard_Real angle);
  sphere(const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2);
  sphere(const gp_Pnt &Center, const Standard_Real R);
  sphere(const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3);
  sphere(const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle);
  sphere(const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2);
  sphere (const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3);
  sphere (const gp_Ax2 &Axis, const Standard_Real R);
  sphere (const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle);
  sphere (const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2);
  sphere (const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3);

  TopoDS_Shape getshape(){
    return  _sphere->Shape();
  }
private:
  BRepPrimAPI_MakeSphere *_sphere=NULL;
};

#endif // SPHERE_H
