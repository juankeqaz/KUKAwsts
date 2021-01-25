#include "pipes.h"

Pipes::Pipes(const gp_Ax2 &Axes, const Standard_Real Rmax, const Standard_Real Rmin, const Standard_Real _H, const Standard_Real Angle)
{
    if(Rmax<Rmin)
        _pipes=NULL;

    BRepPrimAPI_MakeCylinder *cy1 = new BRepPrimAPI_MakeCylinder(Axes,Rmax,_H,Angle);
    BRepPrimAPI_MakeCylinder *cy2 = new BRepPrimAPI_MakeCylinder(Axes,Rmin,_H,Angle);
    _pipes=new BRepAlgoAPI_Cut(cy1->Shape(),cy2->Shape());
}

Pipes::Pipes(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real Rmax, const Standard_Real Rmin,const Standard_Real _H)
{
    gp_Ax2 Axes;
    Axes.SetLocation(gp_Pnt(x,y,z));
    BRepPrimAPI_MakeCylinder *cy1 = new BRepPrimAPI_MakeCylinder(Axes,Rmax,_H);
    BRepPrimAPI_MakeCylinder *cy2 = new BRepPrimAPI_MakeCylinder(Axes,Rmin,_H);
    _pipes=new BRepAlgoAPI_Cut(cy1->Shape(),cy2->Shape());
}

Pipes::Pipes(const gp_Ax2 &Axes, const Standard_Real Rmax, const Standard_Real Rmin, const Standard_Real _H)
{
    if(Rmax<Rmin)
        _pipes=NULL;

    BRepPrimAPI_MakeCylinder *cy1 = new BRepPrimAPI_MakeCylinder(Axes,Rmax,_H);
    BRepPrimAPI_MakeCylinder *cy2 = new BRepPrimAPI_MakeCylinder(Axes,Rmin,_H);
    _pipes=new BRepAlgoAPI_Cut(cy1->Shape(),cy2->Shape());
}
