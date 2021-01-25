#include "cylinder.h"

cylinder::cylinder(const Standard_Real R, const Standard_Real H, const Standard_Real Angle)
{
  this->_angle=Angle;
  this->_radius=R;
  this->_height=H;
  _cylinder=new BRepPrimAPI_MakeCylinder(R,H,Angle);
}

cylinder::cylinder(const Standard_Real R, const Standard_Real H)
{
  this->_radius=R;
  this->_height=H;
  _cylinder=new BRepPrimAPI_MakeCylinder(R,H);
}

cylinder::cylinder(const gp_Ax2 &Axes, const Standard_Real R, const Standard_Real H)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius=R;
  this->_height=H;
  _cylinder=new BRepPrimAPI_MakeCylinder(Axes,R,H);
}

cylinder::cylinder(const gp_Ax2 &Axes, const Standard_Real R, const Standard_Real H, const Standard_Real Angle)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius=R;
  this->_height=H;
  this->_angle=Angle;
    _cylinder=new BRepPrimAPI_MakeCylinder(Axes,R,H,Angle);
}
