#ifndef CONE_H
#define CONE_H
#include "occ.h"

class Cone
{
public:
  Cone (const Standard_Real R1, const Standard_Real R2, const Standard_Real H);
  Cone(const Standard_Real R1, const Standard_Real R2, const Standard_Real H, const Standard_Real angle);
  Cone (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real H);
  Cone (const gp_Ax2 &Axes, const Standard_Real R1, const Standard_Real R2, const Standard_Real H, const Standard_Real angle);

  Standard_Real getraidus1(){
    return this->_radius1;
  }
  Standard_Real getraidus2(){
    return this->_radius2;
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
    return  _cone->Shape();
  }
  TopoDS_Face getface()
  {
    return  _cone->Face();
  }

private:
  BRepPrimAPI_MakeCone *_cone=NULL;
  gp_Pnt _position=gp_Pnt(0,0,0);
  gp_Dir _dir=gp_Dir(0,0,1);
  Standard_Real _radius1=0;
  Standard_Real _radius2=0;
  Standard_Real _height=0;
  Standard_Real _angle=2*M_PI;

};

#endif // CONE_H
