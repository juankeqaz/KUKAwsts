#ifndef WEDGE_H
#define WEDGE_H
#include "occ.h"

class Wedge
{
public:
  Wedge(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real ltx);
  Wedge (const gp_Ax2 &Axes, const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real ltx);
  Wedge (const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real xmin, const Standard_Real zmin, const Standard_Real xmax, const Standard_Real zmax);
  Wedge (const gp_Ax2 &Axes, const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real xmin, const Standard_Real zmin, const Standard_Real xmax, const Standard_Real zmax);

  TopoDS_Shape getshape(){
    return _wedge->Shape();
  }

private:
  BRepPrimAPI_MakeWedge *_wedge;


};

#endif // WEDGE_H
