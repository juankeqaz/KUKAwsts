#include "torus.h"

Torus::Torus(const Standard_Real R1, const Standard_Real R2)
{
  this->_radius1=R1;
  this->_radius2=R2;
  _torus=new BRepPrimAPI_MakeTorus(R1,R2);
}

Torus::Torus(const Standard_Real R1, const Standard_Real R2, const Standard_Real angle)
{
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle;
  this->_angle2=angle;
  this->_angle3=angle;
  _torus=new BRepPrimAPI_MakeTorus(R1,R2,angle);
}

Torus::Torus(const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2)
{
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle1;
  this->_angle2=angle2;

  _torus=new BRepPrimAPI_MakeTorus(R1,R2,angle1,angle2);
}

Torus::Torus(const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle)
{
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle1;
  this->_angle2=angle2;
  this->_angle3=angle;
  _torus=new BRepPrimAPI_MakeTorus(R1,R2,angle1,angle2,angle);
}

Torus::Torus(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  _torus=new BRepPrimAPI_MakeTorus(Axes,R1,R2);
}

Torus::Torus(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle;
  _torus=new BRepPrimAPI_MakeTorus(Axes,R1,R2,angle);
}

Torus::Torus(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle1;
  this->_angle2=angle2;
  _torus=new BRepPrimAPI_MakeTorus(Axes,R1,R2,angle1,angle2);
}

Torus::Torus(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  this->_angle1=angle1;
  this->_angle2=angle2;
  this->_angle3=angle;
  _torus=new BRepPrimAPI_MakeTorus(Axes,R1,R2,angle1,angle2,angle);
}
