#include "sphere.h"

sphere::sphere(const Standard_Real R)
{
  _sphere=new BRepPrimAPI_MakeSphere(R);
}

sphere::sphere(const Standard_Real R, const Standard_Real angle)
{
  _sphere=new BRepPrimAPI_MakeSphere(R,angle);
}

sphere::sphere(const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2)
{
  _sphere=new BRepPrimAPI_MakeSphere(R,angle1,angle2);
}

sphere::sphere(const gp_Pnt &Center, const Standard_Real R)
{
  _sphere=new BRepPrimAPI_MakeSphere(Center,R);
}

sphere::sphere(const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3)
{
  _sphere=new BRepPrimAPI_MakeSphere(R,angle1,angle2,angle3);
}

sphere::sphere(const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle)
{
  _sphere=new BRepPrimAPI_MakeSphere(Center,R,angle);
}

sphere::sphere(const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2)
{
  _sphere=new BRepPrimAPI_MakeSphere(Center,R,angle1,angle2);
}

sphere::sphere(const gp_Pnt &Center, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3)
{
  _sphere=new BRepPrimAPI_MakeSphere(Center,R,angle1,angle2,angle3);
}

sphere::sphere(const gp_Ax2 &Axis, const Standard_Real R)
{
  _sphere=new BRepPrimAPI_MakeSphere(Axis,R);
}

sphere::sphere(const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle)
{
  _sphere=new BRepPrimAPI_MakeSphere(Axis,R,angle);
}

sphere::sphere(const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2)
{
  _sphere=new BRepPrimAPI_MakeSphere(Axis,R,angle1,angle2);
}

sphere::sphere(const gp_Ax2 &Axis, const Standard_Real R, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle3)
{
  _sphere=new BRepPrimAPI_MakeSphere(Axis,R,angle1,angle2,angle3);
}
