#ifndef TORUS_H
#define TORUS_H
#include "occ.h"

class Torus
{
public:
  Torus(const Standard_Real R1, const Standard_Real R2);
  Torus (const Standard_Real R1, const Standard_Real R2, const Standard_Real angle);
  Torus (const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2);
  Torus (const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle);
  Torus (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2);
  Torus (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle);
  Torus (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2);
  Torus (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real angle1, const Standard_Real angle2, const Standard_Real angle);

  Standard_Real getraidus1(){
    return this->_radius1;
  }
  Standard_Real getraidus2(){
    return this->_radius2;
  }
  Standard_Real getheight(){
    return  this->_height;
  }
  Standard_Real getangle1(){
    return  this->_angle1;
  }
  gp_Pnt getpostion(){
    return this->_position;
  }
  gp_Dir getdiraction(){
    return  this->_dir;
  }
  TopoDS_Shape getshape(){
    return  _torus->Shape();
  }
  TopoDS_Face getface()
  {
    return  _torus->Face();
  }
private:
  BRepPrimAPI_MakeTorus *_torus=NULL;
  gp_Pnt _position=gp_Pnt(0,0,0);
  gp_Dir _dir=gp_Dir(0,0,1);
  Standard_Real _radius1=0;
  Standard_Real _radius2=0;
  Standard_Real _height=0;
  Standard_Real _angle1=2*M_PI;
  Standard_Real _angle2=2*M_PI;
  Standard_Real _angle3=2*M_PI;

};

#endif // TORUS_H
