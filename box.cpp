#include "box.h"


box::box(const TopoDS_Shape &shape)
{
    TopoDS_Shape cshape=shape;
    Standard_Real p1,p2,p3;
    gp_Ax2 anAxis;
    getboxcorner(cshape,p1,p2,p3,anAxis);
    _box=new BRepPrimAPI_MakeBox(anAxis,p1,p2,p3);
}

box::box(Standard_Real dx, Standard_Real dy, Standard_Real dz)
{
  _box=new BRepPrimAPI_MakeBox(dx,dy,dz);
}

box::box(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
  gp_Ax2 anAxis;
  anAxis.SetLocation(gp_Pnt(x, y, z));
  _box=new BRepPrimAPI_MakeBox(anAxis,_dx,_dy,_dz);
}

box::box(gp_Pnt location, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
  gp_Ax2 anAxis;
  anAxis.SetLocation(location);
  _box=new BRepPrimAPI_MakeBox(anAxis,_dx,_dy,_dz);
}

box::box(gp_Pnt location, gp_Pnt corner)
{
  _box=new BRepPrimAPI_MakeBox(location,corner);
}

box::box(gp_Ax2 axes, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
    _box= new BRepPrimAPI_MakeBox(axes,_dx,_dy,_dz);
}

TopoDS_Wire box::getedge(TopoDS_Face face)
{
    ShapeAnalysis_FreeBounds afreebounds(face,Standard_False,Standard_True);
    TopoDS_Compound aclosedwires=afreebounds.GetClosedWires();
    TopTools_IndexedMapOfShape awires;
    TopExp::MapShapes(aclosedwires,TopAbs_WIRE,awires);
    TopoDS_Wire awire;
    Standard_Integer nbWires=awires.Extent();
    if(nbWires)
        awire=TopoDS::Wire(awires(1));
    return awire;
}

QVector<gp_Pnt> box::pathpoint(TopoDS_Wire wire)
{
    QVector<gp_Pnt> pvec;
    TopExp_Explorer ex(wire,TopAbs_EDGE);
    for(;ex.More();ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(ex.Current());
        Standard_Real first;
        Standard_Real end;
        Handle(Geom_Curve) acure=BRep_Tool::Curve(edge,first,end);
        gp_Pnt strpnt=acure->Value(first);
        gp_Pnt endpnt=acure->Value(end);

        pvec.push_back(strpnt);
        pvec.push_back(endpnt);

//        BRep_Tool::Range(edge,first,end);
//        Handle(Geom_Curve) cur=BRep_Tool::Curve(edge,first,end);
//        for(Standard_Integer i=first;i<end;++i)
//        {
//            gp_Pnt pi;
//            cur->D0(i,pi);
//            pvec.push_back(pi);
//            //qDebug()<<"point "<<i<<"is"<<pi.X()<<pi.Y()<<pi.Z();
//        }
    }

    return  pvec;
}

QVector<gp_Pnt> box::pathpoint(TopoDS_Face face)
{
    QVector<gp_Pnt> vec;
    QVector<gp_Pnt> vecline1;
    QVector<gp_Pnt> vecline2;
    if(face.IsNull())
        return vec;
    for(TopExp_Explorer Ex(face,TopAbs_EDGE);Ex.More();Ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(Ex.Current());
        Standard_Real first,last;
        Handle(Geom_Curve) acure=BRep_Tool::Curve(edge,first,last);

        gp_Pnt strpnt=acure->Value(first);
        gp_Pnt endpnt=acure->Value(last);
        //printpnt(strpnt);
        //printpnt(endpnt);


        if(vecline1.size()==0)
        {
            vecline1.push_back(strpnt);
            vecline1.push_back(endpnt);
        }
        else
        {
            if(isequal(vecline1.last(),strpnt))
            {
                vecline1.push_back(endpnt);
            }
            else if(vecline2.size()==0)
            {
                vecline2.push_front(endpnt);
                vecline2.push_front(strpnt);
            }else if(isequal(vecline2.first(),endpnt))
            {
                vecline2.push_front(strpnt);
            }
        }

    }
    if(isequal(vecline1.first(),vecline2.first()))
        vecline2.pop_front();
    if(isequal(vecline1.last(),vecline2.last()))
        vecline1.pop_back();
    Standard_Real d1=distansall(vecline1);
    Standard_Real d2=distansall(vecline2);
    Standard_Real step=5;

    int st=0;
    if(d1 >= d2)
        st=ceil(d1 / step);
    else
        st=ceil(d2 / step);
    splitpath(vecline1,st,vec);
    splitpath(vecline2,st,vec);
    //qDebug()<<vec;
    return vec;
}

gp_Vec box::getnormal(TopoDS_Face face)
{
    Handle_Geom_Surface hsurface=BRep_Tool::Surface(face);
    TopExp_Explorer expFace;
    expFace.Init(face,TopAbs_FACE);
//    for(expFace.Init(face,TopAbs_EDGE);expFace.More();expFace.Next())
//    {
        TopExp_Explorer VtxExp(face,TopAbs_VERTEX);
        TopoDS_Vertex testVtx=TopoDS::Vertex(VtxExp.Current());
        gp_Pnt testpt=BRep_Tool::Pnt(testVtx);
        GeomAPI_ProjectPointOnSurf projectpt(testpt,hsurface);
        Standard_Real U=0.0,V=0.0;
        projectpt.LowerDistanceParameters(U,V);
        GeomLProp_SLProps props(hsurface,U,V,1,Precision::Confusion());
        gp_Vec curfacenormal=props.Normal();
        if(face.Orientation()==TopAbs_REVERSED)
        {
            curfacenormal=curfacenormal.Reversed();
        }
        //qDebug()<<"normal is "<<curfacenormal.X()<<curfacenormal.Y()<<curfacenormal.Z();
        return curfacenormal;
}

void box::getboxcorner(TopoDS_Shape &shape, Standard_Real &p1, Standard_Real &p2, Standard_Real &p3, gp_Ax2 &axis)
{
    int count=0;
    gp_Vec vx,vy,vz;
    gp_Pnt porg;
    TopoDS_Shape cshape=shape;

    for(TopExp_Explorer ex(cshape,TopAbs_EDGE);ex.More();ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(ex.Current());
        Standard_Real first,last;
        Handle(Geom_Curve) acure=BRep_Tool::Curve(edge,first,last);
        count++;
        gp_Pnt strpnt=acure->Value(first);
        gp_Pnt endpnt=acure->Value(last);
        gp_Vec v=gp_Vec(strpnt,endpnt);
        if(count==1)
        {
            porg=strpnt;
            p3=strpnt.Distance(endpnt);
            vz=v;
        }else if(count==2)
        {
            p2=strpnt.Distance(endpnt);
            vy=v;
            vx=vy.Crossed(vz);
        }else
        {
            if(v.Angle(vx)<1e-3)
            {
                p1=strpnt.Distance(endpnt);
            }
        }
    }
        axis.SetLocation(porg);
        axis.SetXDirection(vx);
        axis.SetYDirection(vy);
}
