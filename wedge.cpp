#include "wedge.h"

Wedge::Wedge(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real ltx)
{
  _wedge=new BRepPrimAPI_MakeWedge(dx,dy,dz,ltx);
}

Wedge::Wedge(const gp_Ax2 &Axes, const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real ltx)
{
   _wedge=new BRepPrimAPI_MakeWedge(Axes, dx,dy,dz,ltx);
}

Wedge::Wedge(const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real xmin, const Standard_Real zmin, const Standard_Real xmax, const Standard_Real zmax)
{
   _wedge=new BRepPrimAPI_MakeWedge(dx,dy,dz,xmin,zmin,xmax,zmax);
}

Wedge::Wedge(const gp_Ax2 &Axes, const Standard_Real dx, const Standard_Real dy, const Standard_Real dz, const Standard_Real xmin, const Standard_Real zmin, const Standard_Real xmax, const Standard_Real zmax)
{
  _wedge=new BRepPrimAPI_MakeWedge(Axes,dx,dy,dz,xmin,zmin,xmax,zmax);
}
