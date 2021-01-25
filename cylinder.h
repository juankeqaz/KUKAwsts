#ifndef CYLINDER_H
#define CYLINDER_H
#include "occ.h"

class cylinder
{
public:
  cylinder(const Standard_Real R, const Standard_Real H, const Standard_Real Angle);
  cylinder(const Standard_Real 	R,  const Standard_Real H);
  cylinder (const gp_Ax2 &Axes, const Standard_Real R, const Standard_Real H);
  cylinder (const gp_Ax2 &Axes, const Standard_Real R, const Standard_Real H, const Standard_Real Angle);
  Standard_Real getraidus(){
    return this->_radius;
  }
  Standard_Real getheight(){
    return  this->_height;
  }
  Standard_Real getangle(){
    return  this->_angle;
  }
  gp_Pnt getpostion(){
    return this->_position;
  }
  gp_Dir getdiraction(){
    return  this->_dir;
  }
  TopoDS_Shape getshape(){
    return  _cylinder->Shape();
  }
  TopoDS_Face getface()
  {
    return  _cylinder->Face();
  }

private:
  BRepPrimAPI_MakeCylinder *_cylinder=NULL;
  gp_Pnt _position=gp_Pnt(0,0,0);
  gp_Dir _dir=gp_Dir(0,0,1);
  Standard_Real _radius=0;
  Standard_Real _height=0;
  Standard_Real _angle=2*M_PI;

};

#endif // CYLINDER_H
