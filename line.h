#ifndef LINE_H
#define LINE_H

#include "occ.h"

class Line
{
public:
  Line(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real _H);
  Line(const gp_Pnt org, const gp_Vec vec, const Standard_Real _H);
  Line(const gp_Pnt org,const gp_Pnt end);

  void setdirection(gp_Vec vec)
  {
      this->_normal=vec;
  }

  TopoDS_Shape getshape(){
    return _line->Shape();
  }
  gp_Vec getdir()
  {
      return  _normal;
  }

private:
        BRepPrimAPI_MakePrism *_line;
        gp_Pnt _firstpoint;
        gp_Pnt _lastpoint;
        gp_Vec _normal;
};

#endif // LINE_H
