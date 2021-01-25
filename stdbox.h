#ifndef STDBOX_H
#define STDBOX_H
#include <TopoDS_Shape.hxx>

class StdBox:public TopoDS_Shape
{
public:
    StdBox(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz);


private:
    Standard_Real x;
    Standard_Real y;
    Standard_Real z;
    Standard_Real _dx;
    Standard_Real _dy;
    Standard_Real _dz;
};

#endif // STDBOX_H
