#ifndef PIPES_H
#define PIPES_H
#include "occ.h"

class Pipes
{
public:
    Pipes(const gp_Ax2 &Axes, const Standard_Real Rmax, const Standard_Real Rmin, const Standard_Real _H, const Standard_Real Angle);
    Pipes(const Standard_Real x,const Standard_Real y,const Standard_Real z,const Standard_Real Rmax,const Standard_Real Rmin,const Standard_Real _H);
    Pipes(const gp_Ax2 &Axes, const Standard_Real Rmax, const Standard_Real Rmin, const Standard_Real _H);


    TopoDS_Shape getshape()
    {
        return _pipes->Shape();
    }

private:
    BRepAlgoAPI_Cut *_pipes;

};

#endif // PIPES_H
