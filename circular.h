#ifndef CIRCULAR_H
#define CIRCULAR_H
#include "occ.h"

class circular
{
public:
  circular(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real radius,const Standard_Real _angle);
  circular(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real radius);
  circular(const gp_Pnt position, const gp_Dir dir, const Standard_Real radius);
  circular(const gp_Pnt position, const gp_Dir dir, const Standard_Real radius,const Standard_Real _angle);
  circular(const gp_Pnt p1,const gp_Pnt p2,gp_Pnt p3);

  Standard_Real getraidus(){
    return this->_radius;
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
    return  _cirular->Shape();
  }

private:
  void calculearcparamet(const gp_Pnt p1,const gp_Pnt p2,gp_Pnt p3);
  void calculearcparamet(const gp_Pnt position,const gp_Dir dir,const Standard_Real radius,gp_Pnt &point);


private:
  BRepPrimAPI_MakeRevol *_cirular;
  Standard_Real _radius=0;
  Standard_Real _angle=2*M_PI;
  gp_Pnt _position=gp_Pnt(0,0,0);
  gp_Dir _dir=gp_Dir(0,0,1);



};

#endif // CIRCULAR_H
