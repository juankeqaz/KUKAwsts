#include "cone.h"

Cone::Cone(const Standard_Real R1, const Standard_Real R2, const Standard_Real H)
{
  this->_radius1=R1;
  this->_radius2=R2;
  this->_height=H;
  _cone=new BRepPrimAPI_MakeCone(R1,R2,H);

}

Cone::Cone(const Standard_Real R1, const Standard_Real R2, const Standard_Real H, const Standard_Real angle)
{
  this->_radius1=R1;
  this->_radius2=R2;
  this->_height=H;
  this->_angle=angle;
  _cone=new BRepPrimAPI_MakeCone(R1,R2,H,angle);
}

Cone::Cone(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real H)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  this->_height=H;
  _cone=new BRepPrimAPI_MakeCone( Axes,R1,R2,H);
}

Cone::Cone(const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real H, const Standard_Real angle)
{
  this->_position=Axes.Location();
  this->_dir=Axes.Direction();
  this->_radius1=R1;
  this->_radius2=R2;
  this->_height=H;
  this->_angle=angle;
  _cone=new BRepPrimAPI_MakeCone( Axes,R1,R2,H,angle);
}
