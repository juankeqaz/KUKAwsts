#include "line.h"

Line::Line(const gp_Pnt org, const gp_Vec vec, const Standard_Real _H)
{
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(org);

  Standard_Real distanse=sqrt(vec.X()*vec.X()+vec.Y()*vec.Y()+vec.Z()*vec.Z());
  gp_Vec vf;

  if(distanse==_H)
  {
      vf=vec;
  }
  else
  {
      vf=vec.Normalized();
      vf=vf*_H;
  }
  this->_firstpoint=org;
  this->_lastpoint=gp_Pnt(vf.X(),vf.Y(),vf.Z());

  _line = new BRepPrimAPI_MakePrism(aVertex, vf);
  _normal=gp_Vec(vec.Normalized());
}

Line::Line(const gp_Pnt org, const gp_Pnt end)
{
  this->_firstpoint=org;
  this->_lastpoint=end;
  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(org);
  gp_Vec vf=gp_Vec(org,end);
  _line=new BRepPrimAPI_MakePrism(aVertex, vf);
  _normal=vf.Normalized();
}


Line::Line(const Standard_Real x, const Standard_Real y, const Standard_Real z, const Standard_Real _H)
{
  this->_firstpoint=gp_Pnt(x, y, z);
  this->_lastpoint=gp_Pnt(x, y, z+_H);

  TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, z));

  _line =new BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, _H));
  _normal=gp_Vec(0,0,1);
}
