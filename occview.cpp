/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : OccView.cpp
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-08 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Qt widget for OpenCASCADE viewer.
*/

#include <OpenGl_GraphicDriver.hxx>

#include "occview.h"

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>

#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <QDebug>

#ifdef WNT
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#include <Xw_Window.hxx>
#endif


static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}

OccView::OccView(QWidget* parent )
    : QGLWidget(parent),
      myXmin(0),
      myYmin(0),
      myXmax(0),
      myYmax(0),
      myCurrentMode(CurAction3d_DynamicRotation),
      myDegenerateModeIsOn(Standard_True),
      myRectBand(NULL)
{
    // No Background
    setBackgroundRole( QPalette::NoRole );

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );
    robangle={0,0,0,0,0,0};
    showrangeline=false;
    //readkukamodel();

}

void OccView::axisposition(int p)
{
    //myView->TriedronErase();

    switch (p) {
    case 0:
        myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
        break;
    case 1:
        myView->TriedronDisplay(Aspect_TOTP_CENTER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
        break;
    case 2:
        myView->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
        break;
    case 3:
        myView->TriedronDisplay(Aspect_TOTP_RIGHT_UPPER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
        break;
    case 4:
        myView->TriedronDisplay(Aspect_TOTP_LEFT_UPPER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
        break;

    }

}

TopoDS_Shape OccView::make_cube(Standard_Real x,Standard_Real y,Standard_Real z,Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_box = BRepPrimAPI_MakeBox(anAxis,_dx,_dy,_dz).Shape();
    return  t_topo_box;
}

TopoDS_Shape OccView::make_cube(gp_Pnt org, gp_Vec vec, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);

    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_box = BRepPrimAPI_MakeBox(anAxis,_dx,_dy,_dz).Shape();
    return  t_topo_box;
}

TopoDS_Shape OccView::make_sphere(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_Sphere=BRepPrimAPI_MakeSphere(anAxis, _r);
    return t_topo_Sphere;
}

TopoDS_Shape OccView::make_cylinder(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _R, Standard_Real _H)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _R,_H);
    return t_topo_cylinder;
}

TopoDS_Shape OccView::make_cylinder(gp_Pnt org, gp_Vec vec, Standard_Real _R, Standard_Real _H)
{
    gp_Ax2 anAxis;
    //gp_Vec vecx(vec.X()-org.X(),vec.Y()-org.Y(),vec.Z()-org.Z());
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _R,_H);
    return t_topo_cylinder;
}

TopoDS_Shape OccView::make_cone(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1, Standard_Real r2, Standard_Real _h)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    anAxis.SetDirection(gp_Dir(0,0,1));
    TopoDS_Shape t_topo_cone = BRepPrimAPI_MakeCone(anAxis,_r1,r2,_h).Shape();
    return  t_topo_cone;
}

TopoDS_Shape OccView::make_cone(gp_Pnt org, gp_Vec vec, Standard_Real _r1, Standard_Real r2, Standard_Real _h)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_cone = BRepPrimAPI_MakeCone(anAxis,_r1,r2,_h).Shape();
    return  t_topo_cone;
}

TopoDS_Shape OccView::make_torus(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1, Standard_Real _r2)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_torus = BRepPrimAPI_MakeTorus(anAxis, _r1,_r2);
    return  t_topo_torus;
}

TopoDS_Shape OccView::make_torus(gp_Pnt org, gp_Vec vec, Standard_Real _r1, Standard_Real _r2)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_torus = BRepPrimAPI_MakeTorus(anAxis, _r1,_r2);
    return  t_topo_torus;
}

TopoDS_Shape OccView::make_wedge(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax, Standard_Real _zmin)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape edge=BRepPrimAPI_MakeWedge(x,y,z,_dmin,_zmin,_dmax,_zmax);
    return  edge;
}

TopoDS_Shape OccView::make_wedge(gp_Pnt org, gp_Vec vec,Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax, Standard_Real _zmin)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape edge=BRepPrimAPI_MakeWedge(anAxis,x,y,z,_dmin,_zmin,_dmax,_zmax);
    return  edge;
}

TopoDS_Shape OccView::make_cylinder_fan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _R, Standard_Real _H, Standard_Real _angle)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _R,_H,_angle*PI/180);
    return  t_topo_cylinder;
}

TopoDS_Shape OccView::make_cylinder_fan(gp_Pnt org, gp_Vec vec, Standard_Real _R, Standard_Real _H, Standard_Real _angle)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _R,_H,_angle*PI/180);
    return  t_topo_cylinder;
}

TopoDS_Shape OccView::make_pipe(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax,Standard_Real _Rmin, Standard_Real _H)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _Rmax,_H);
    TopoDS_Shape t_topo_cylinder2=BRepPrimAPI_MakeCylinder(anAxis, _Rmin,_H);
    TopoDS_Shape t_topo_pipe=BRepAlgoAPI_Cut(t_topo_cylinder,t_topo_cylinder2);
    return  t_topo_pipe;

}

TopoDS_Shape OccView::make_pipe(gp_Pnt org, gp_Vec vec, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _Rmax,_H);
    TopoDS_Shape t_topo_cylinder2=BRepPrimAPI_MakeCylinder(anAxis, _Rmin,_H);
    TopoDS_Shape t_topo_pipe=BRepAlgoAPI_Cut(t_topo_cylinder,t_topo_cylinder2);
    return  t_topo_pipe;
}

TopoDS_Shape OccView::make_pipe_fan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H, Standard_Real _angle)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _Rmax,_H,_angle*PI/180);
    TopoDS_Shape t_topo_cylinder2=BRepPrimAPI_MakeCylinder(anAxis, _Rmin,_H);
    TopoDS_Shape t_topo_pipe=BRepAlgoAPI_Cut(t_topo_cylinder,t_topo_cylinder2);
    return  t_topo_pipe;
}

TopoDS_Shape OccView::make_pipe_fan(gp_Pnt org, gp_Vec vec, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H, Standard_Real _angle)
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    TopoDS_Shape t_topo_cylinder=BRepPrimAPI_MakeCylinder(anAxis, _Rmax,_H,_angle*PI/180);
    TopoDS_Shape t_topo_cylinder2=BRepPrimAPI_MakeCylinder(anAxis, _Rmin,_H);
    TopoDS_Shape t_topo_pipe=BRepAlgoAPI_Cut(t_topo_cylinder,t_topo_cylinder2);
    return  t_topo_pipe;
}

TopoDS_Shape OccView::make_line(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _H)
{
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, z));
    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, _H));
    return  aPrismVertex;
}

TopoDS_Shape OccView::make_line(gp_Pnt org, gp_Vec vec, Standard_Real _H)
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

    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, vf);
    return  aPrismVertex;
}

TopoDS_Shape OccView::make_arc(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real radius,Standard_Real _angle)
{
    gp_Ax1 anAxis(gp_Pnt(0,0,0),gp_Dir(1,1,1));


    // revol a vertex result is an edge.
    anAxis.SetLocation(gp_Pnt(x, y, z));
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(radius+x, y, z));
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis,_angle*PI/180);
    return  aRevolVertex;
}

TopoDS_Shape OccView::make_arc(gp_Pnt org, gp_Vec vec, Standard_Real radius, Standard_Real _angle)
{
    gp_Ax1 anAxis;

    anAxis.SetLocation(org);
    anAxis.SetDirection(gp_Dir(vec.Normalized()));
    gp_Pnt pn=gp_Pnt(org.X()+radius,org.Y(),org.Z());


    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(pn);
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis,_angle*PI/180);
    return  aRevolVertex;
}

void OccView::make_wires()
{
    if(sweeps_line_point.size()<=0) return;
    QVector<TopoDS_Edge> edgevector;
    for(int i=0;i<sweeps_line_point.size();i++)
    {
        QPair<QString,QVector<gp_Pnt> > pair=sweeps_line_point.at(i);
        QVector<gp_Pnt> pnt=pair.second;
        TopoDS_Edge edge;
        if(pair.first=="line"&& pnt.size()==2)
        {
            gp_Pnt aPnt1,aPnt2;
            aPnt1=pnt.at(0);
            aPnt2=pnt.at(1);
            Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
            edgevector.push_back(BRepBuilderAPI_MakeEdge(aSegment1));


        }else if(pair.first=="arc"&& pnt.size()==3)
        {
            gp_Pnt aPnt2,aPnt3,aPnt4;
            aPnt2=pnt.at(0);
            aPnt3=pnt.at(1);
            aPnt4=pnt.at(2);
            Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
            edgevector.push_back(BRepBuilderAPI_MakeEdge(anArcOfCircle));
        }
    }
    TopoDS_Wire aWire;
    for(int p=0;p<edgevector.size();p++)
    {
        aWire=BRepBuilderAPI_MakeWire(aWire, edgevector.at(p));
    }
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(aWire);
    myContext->Display(anAisRevolVertex, Standard_True);
    myView->FitAll();
}

TopoDS_Shape OccView::make_sweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz)
{
    TopoDS_Shape myFaceProfile;
    if(sweeps_line_point.size()<=0) return myFaceProfile;
    QVector<TopoDS_Edge> edgevector;
    for(int i=0;i<sweeps_line_point.size();i++)
    {
        QPair<QString,QVector<gp_Pnt> > pair=sweeps_line_point.at(i);
        QVector<gp_Pnt> pnt=pair.second;
        TopoDS_Edge edge;
        if(pair.first=="line"&& pnt.size()==2)
        {
            gp_Pnt aPnt1,aPnt2;
            aPnt1=pnt.at(0);
            aPnt2=pnt.at(1);
            Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
            edgevector.push_back(BRepBuilderAPI_MakeEdge(aSegment1));


        }else if(pair.first=="arc"&& pnt.size()==3)
        {
            gp_Pnt aPnt2,aPnt3,aPnt4;
            aPnt2=pnt.at(0);
            aPnt3=pnt.at(1);
            aPnt4=pnt.at(2);
            Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
            edgevector.push_back(BRepBuilderAPI_MakeEdge(anArcOfCircle));
        }
    }
    TopoDS_Wire aWire;
    for(int p=0;p<edgevector.size();p++)
    {
        aWire=BRepBuilderAPI_MakeWire(aWire, edgevector.at(p)).Wire();
    }

//    gp_Pnt p1=sweeps_line_point.first().second.first();
//    gp_Pnt p2=gp_Pnt(wx,wy,wz);
//    gp_Pnt p3=gp_Pnt(hx,hy,hz);

//    gp_Pnt p4=gp_Pnt(wx-hx+p1.X(),wy-hy+p1.Y(),hz-wz+p1.Z());
//    //gp_Pnt p4=sweeps_line_point.last().second.last();
//    Handle(Geom_TrimmedCurve) facedge1 = GC_MakeSegment(p1, p2);
//    Handle(Geom_TrimmedCurve) facedge2 = GC_MakeSegment(p2, p4);
//    Handle(Geom_TrimmedCurve) facedge3 = GC_MakeSegment(p4, p3);
//    Handle(Geom_TrimmedCurve) facedge4 = GC_MakeSegment(p3, p1);
//    TopoDS_Edge Edge1 = BRepBuilderAPI_MakeEdge(facedge1);
//    TopoDS_Edge Edge2 = BRepBuilderAPI_MakeEdge(facedge2);
//    TopoDS_Edge Edge3 = BRepBuilderAPI_MakeEdge(facedge3);
//    TopoDS_Edge Edge4 = BRepBuilderAPI_MakeEdge(facedge4);
//    TopoDS_Wire Wire1=BRepBuilderAPI_MakeWire(Edge1, Edge2, Edge3,Edge4);
    myFaceProfile = BRepPrimAPI_MakePrism(aWire,gp_Vec(wx,wy,wz),false,false).Shape();
    return  myFaceProfile;
}

TopoDS_Face OccView::getupface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=-1;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);



            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.Z();
            if(aZ > zMax){
                zMax = aZ;
                face=aFace;

                //qDebug()<<"istopface"<<count;
            }
        }
    }
    return  face;
}

TopoDS_Face OccView::getdownface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=10000;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            //qDebug()<<"down"<<aPnt.X()<<aPnt.Y()<<aPnt.Z()<<aPlane->get_type_name();
            Standard_Real aZ   = aPnt.Z();
            if(count == 5){
                zMax = aZ;
                face=aFace;
                //qDebug()<<"isface"<<count<<zMax;
                //gp_Pnt d0;
                //gp_Vec dr1;
                //gp_Vec dr2;
                //aPlane->D1(1,1,d0,dr1,dr2);
                //qDebug()<<"d0 is"<<d0.X()<<d0.Y()<<d0.Z()<<dr1.X()<<dr1.Y()<<dr1.Z()<<dr2.X()<<dr2.Y()<<dr2.Z();
            }
        }
    }
    return  face;
}

TopoDS_Face OccView::getfrontface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=-1;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.X();
            if(count==3){
                zMax = aZ;
                face=aFace;
                //qDebug()<<"istopface"<<count;
            }
        }
    }
    return  face;
}

TopoDS_Face OccView::getbackface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=1000;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.X();
            if(count==4){
                zMax = aZ;
                face=aFace;
            }
        }
    }
    return  face;
}

TopoDS_Face OccView::getleftface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=1000;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.Y();
            if(count==1){
                zMax = aZ;
                face=aFace;
                //qDebug()<<"istopface"<<count;
            }
        }
    }
    return  face;
}

TopoDS_Face OccView::getrightface(TopoDS_Shape shape)
{
    TopoDS_Face face;
    int count=0;int zMax=-1;
    for(TopExp_Explorer ex(shape,TopAbs_FACE);ex.More();ex.Next())
    {
        TopoDS_Face aFace = TopoDS::Face(ex.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        count++;
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.Y();
            if(count==2){
                zMax = aZ;
                face=aFace;
            }
        }
    }
    return  face;
}

void OccView::printpnt(const gp_Pnt pnt)
{
    qDebug()<< pnt.X()<<pnt.Y()<<pnt.Z();
}

bool OccView::isequal(const gp_Pnt pnt1, gp_Pnt pnt2)
{
    if(pnt1.X()==pnt2.X() && pnt1.Y()==pnt2.Y() && pnt1.Z()==pnt2.Z())
    {
        return  true;
    }else
        return  false;
}


void OccView::makexyz(Standard_Real x, Standard_Real y, Standard_Real z)
{

    TopoDS_Shape center=make_sphere(x,y,z,2);
    Handle(AIS_Shape) ais_center = new AIS_Shape(center);
    myContext->SetColor(ais_center,Quantity_NOC_BLACK,Standard_False);
    myContext->SetTransparency(ais_center,0.0f,Standard_False);
    myContext->Display(ais_center, Standard_True);

    TopoDS_Shape xaxis=make_cylinder(0,0,0,1,100);
    gp_Trsf Tx;
    Tx.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(0,1,0)),90*PI/180);
    BRepBuilderAPI_Transform traf1(Tx);
    traf1.Perform(xaxis);

    TopoDS_Shape xheader = make_cone(0,0,100,2.0,0.0,5.0);
    gp_Trsf txh;
    txh.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(0,1,0)),90*PI/180);
    BRepBuilderAPI_Transform trafh1(txh);
    trafh1.Perform(xheader);

    xaxis = BRepAlgoAPI_Fuse(traf1.Shape(),trafh1.Shape());
    //xaxis.Located(pos);

    Handle(AIS_Shape) ais_xaxis = new AIS_Shape(xaxis);
    myContext->SetColor(ais_xaxis,Quantity_NOC_RED,Standard_False);
    myContext->SetTransparency(ais_xaxis,0.4f,Standard_True);
    myContext->Display(ais_xaxis, Standard_True);

    TopoDS_Shape yaxis=make_cylinder(0,0,0,1,100);
    gp_Trsf Ty;
    Ty.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(1,0,0)),-90*PI/180);
    BRepBuilderAPI_Transform traf2(Ty);
    traf2.Perform(yaxis);

    TopoDS_Shape yheader = make_cone(0,0,100,2.0,0.0,5.0);
    gp_Trsf tyh;
    tyh.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(1,0,0)),-90*PI/180);
    BRepBuilderAPI_Transform trafh2(tyh);
    trafh2.Perform(yheader);

    yaxis = BRepAlgoAPI_Fuse(traf2.Shape(),trafh2.Shape());
    //yaxis.Located(pos);
    Handle(AIS_Shape) ais_yaxis = new AIS_Shape(yaxis);
    myContext->SetColor(ais_yaxis,Quantity_NOC_GREEN,Standard_False);
    myContext->SetTransparency(ais_yaxis,0.4f,Standard_True);
    myContext->Display(ais_yaxis, Standard_True);

    TopoDS_Shape zaxis=make_cylinder(0,0,0,1,100);
    gp_Trsf Tz;
    Ty.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(0,0,1)),90*PI/180);
    BRepBuilderAPI_Transform traf3(Tz);
    traf3.Perform(zaxis);
    TopoDS_Shape zheader = make_cone(0,0,100,2.0,0.0,5.0);
    gp_Trsf tzh;
    tzh.SetRotation(gp_Ax1(gp_Pnt(0,0,0),gp_Dir(0,0,1)),90*PI/180);
    BRepBuilderAPI_Transform trafh3(tzh);
    trafh3.Perform(zheader);

    zaxis = BRepAlgoAPI_Fuse(traf3.Shape(),trafh3.Shape());
    //zaxis.Located(pos);
    Handle(AIS_Shape) ais_zaxis = new AIS_Shape(zaxis);
    myContext->SetColor(ais_zaxis,Quantity_NOC_BLUE1,Standard_False);
    myContext->SetTransparency(ais_zaxis,0.4f,Standard_True);
    myContext->Display(ais_zaxis, Standard_True);
    myView->FitAll();
}

void OccView::maketoolxyz(Standard_Real length)
{
    if(!canmaketool) return;
    TopoDS_Shape center=make_sphere(suborg.X(),suborg.Y(),suborg.Z(),1.5);
    TopoDS_Shape xaxis=make_cylinder(suborg,subvx,0.8,length);

    gp_Pnt pn=suborg.Translated(subvx.Normalized()*length);
    TopoDS_Shape xheader = make_cone(pn,subvx,1.5,0.0,5.0);
    xaxis = BRepAlgoAPI_Fuse(xaxis,xheader);

    TopoDS_Shape yaxis=make_cylinder(suborg,subvy,0.8,length);
    gp_Pnt py=suborg.Translated(subvy.Normalized()*length);
    TopoDS_Shape yheader = make_cone(py,subvy,1.5,0.0,5.0);
    yaxis = BRepAlgoAPI_Fuse(yaxis,yheader);

    TopoDS_Shape zaxis=make_cylinder(suborg,subvz,0.8,length);
    gp_Pnt pz=suborg.Translated(subvz.Normalized()*length);
    TopoDS_Shape zheader = make_cone(pz,subvz,2.0,0.0,5.0);
    zaxis = BRepAlgoAPI_Fuse(zaxis,zheader);

    Handle(AIS_Shape) ais_xaxis = new AIS_Shape(xaxis);
    Handle(AIS_Shape) ais_center = new AIS_Shape(center);
    Handle(AIS_Shape) ais_yaxis = new AIS_Shape(yaxis);
    Handle(AIS_Shape) ais_zaxis = new AIS_Shape(zaxis);

    myContext->SetColor(ais_xaxis,Quantity_NOC_RED1,Standard_False);
    myContext->SetColor(ais_yaxis,Quantity_NOC_GREEN1,Standard_False);
    myContext->SetColor(ais_zaxis,Quantity_NOC_BLUE2,Standard_False);
    myContext->SetColor(ais_center,Quantity_NOC_GRAY,Standard_False);

    myContext->Display(ais_xaxis,Standard_True);
    myContext->Display(ais_yaxis,Standard_True);
    myContext->Display(ais_zaxis,Standard_True);
    myContext->Display(ais_center,Standard_True);
    myView->FitAll();



}

void OccView::makecoord(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2)
{
    gp_Pnt p0=gp_Pnt(x0,y0,z0);
    gp_Pnt p1=gp_Pnt(x1,y1,z1);
    gp_Pnt p2=gp_Pnt(x2,y2,z2);
    if(p0.Distance(p1)<=0.0001||p0.Distance(p2)<=0.0001||p1.Distance(p2)<=0.0001)
    {
        qDebug()<< "point paramet error";
        return;
    }
    gp_Vec v01=gp_Vec(p0,p1);
    gp_Vec v02=gp_Vec(p0,p2);
    Standard_Real angle=v01.Angle(v02);
    if(angle <= 0.0001|| qAbs(angle-PI) <= 0.0001)
    {
        qDebug()<<"three point in a lines";
        canmaketool=false;
        return;
    }else
    {
        suborg=p0;
        subvx=v01.Normalized();
        subvz=v01;
        subvz.Cross(v02);
        subvy=subvz;
        subvy.Cross(subvx);
        canmaketool=true;
        subvy=subvy.Normalized();
        subvz=subvz.Normalized();
        toolcood.SetValues(subvx.X(),subvy.X(),subvz.X(),suborg.X(),subvx.Y(),subvy.Y(),subvz.Y(),suborg.Y(),subvx.Z(),subvy.Z(),subvz.Z(),suborg.Z());
        modelcood=toolcood;
    }
}

void OccView::getedgepoint(QString dir)
{
    TopoDS_Face face;
    TopoDS_Wire awire;

    if(dir=="upface")
    {
        face=getupface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="downface")
    {
        face=getdownface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="frontface")
    {
        face=getfrontface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="backface")
    {
        face=getbackface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="leftface")
    {
        face=getleftface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="rightface")
    {
        face=getrightface(currentshape);
        pathp=pathpoint(face);
        getnormal(face);
        showshape(face);
    }else if(dir=="upedge")
    {
        face=getupface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);
        showshape(awire);

    }else if(dir=="downedge")
    {
        face=getdownface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);

        showshape(awire);
    }else  if(dir=="frontedge")
    {
        face=getfrontface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);
        showshape(awire);

    }else if(dir=="backedge")
    {
        face=getbackface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);
        showshape(awire);

    }else  if(dir=="leftedge")
    {
        face=getleftface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);
        showshape(awire);

    }else if(dir=="rightedge")
    {
        face=getrightface(currentshape);
        getnormal(face);
        awire=pathplan(face);
        pathp=pathpoint(awire);
        showshape(awire);

    }else
        return;
}

TopoDS_Wire OccView::pathplan(TopoDS_Face face)
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

QVector<gp_Pnt> OccView::pathpoint(TopoDS_Wire awire)
{
    QVector<gp_Pnt> pvec;
    TopExp_Explorer ex(awire,TopAbs_EDGE);
    for(;ex.More();ex.Next())
    {
        TopoDS_Edge edge=TopoDS::Edge(ex.Current());
        Standard_Real first;
        Standard_Real end;
        BRep_Tool::Range(edge,first,end);
        Handle(Geom_Curve) cur=BRep_Tool::Curve(edge,first,end);
        for(Standard_Integer i=first;i<end;++i)
        {
            gp_Pnt pi;
            cur->D0(i,pi);
            pvec.push_back(pi);
            //qDebug()<<"point "<<i<<"is"<<pi.X()<<pi.Y()<<pi.Z();
        }
    }
    return  pvec;
}

QVector<gp_Pnt> OccView::pathpoint(TopoDS_Face aface)
{

    QVector<gp_Pnt> vec;
    QVector<gp_Pnt> vecline1;
    QVector<gp_Pnt> vecline2;
    if(aface.IsNull())
        return vec;
    for(TopExp_Explorer Ex(aface,TopAbs_EDGE);Ex.More();Ex.Next())
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

Standard_Real OccView::distansall(QVector<gp_Pnt> vec)
{
    Standard_Real dist=0;
    if(vec.size()<2) return  0;
    else
    {
        gp_Pnt p1,p2;
        p1=vec.at(0);
        for(int i=1;i<vec.size();++i)
        {
            p2=vec.at(i);
            dist += sqrt((p2.X()-p1.X())*(p2.X()-p1.X())+(p2.Y()-p1.Y())*(p2.Y()-p1.Y())+(p2.Z()-p1.Z())*(p2.Z()-p1.Z()));
            p1=p2;
        }

    }
    return  dist;
}

void OccView::splitpath(const QVector<gp_Pnt> vec, const int num, QVector<gp_Pnt> &endvec)
{
    if(vec.size()<=0) return;
    TColgp_Array1OfPnt array(1,vec.size());
    for(int i=0;i<vec.size();++i)
    {
        array.SetValue(i+1,vec.at(i));
    }
    Handle(Geom_BSplineCurve) curve = GeomAPI_PointsToBSpline(array);
    Standard_Real firstParamer=curve->FirstParameter();
    Standard_Real lastParamer=curve->LastParameter();
    for(Standard_Integer i=0;i<num+1;++i)
    {
        Standard_Real ui=i*(lastParamer-firstParamer)/num;
        gp_Pnt pi;
        gp_Vec veci;
        curve->D1(ui,pi,veci);
        //qDebug()<<"splitpath pos"<<pi.X()<<pi.Y()<<pi.Z();
        //qDebug()<<"splitpath vec"<<veci.X()<<veci.Y()<<veci.Z();
        endvec.push_back(pi);
    }
}

QVector<gp_Pnt> OccView::getpathpoint()
{
    return  pathp;
}

void OccView::getnormal(TopoDS_Face &face)
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
        curfacenormal=props.Normal();
        if(face.Orientation()==TopAbs_REVERSED)
        {
            curfacenormal=curfacenormal.Reversed();
        }
        qDebug()<<"normal is "<<curfacenormal.X()<<curfacenormal.Y()<<curfacenormal.Z();
//    }
}

bool OccView::hasshapes()
{
    int p=myContext->NbSelected();
    qDebug()<<"OccView::hasshapes()"<<p;
    if(p==0)
        return false;
    else
        return true;
}

void OccView::showshape(TopoDS_Shape &shape)
{
    viewshape=shape;
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(shape);
    myContext->Display(anAisRevolVertex, Standard_True);
    myView->FitAll();

}

void OccView::showshapefromlist(QStringList list)
{
    if(list.size()<1) return;
    QString type=list.at(0);
    QVector<float> para;
    TopoDS_Shape shape;
    para.clear();
    for(int i=1;i<list.size();++i)
    {
        para<<QString(list.at(i)).toFloat();
    }
    if(type=="box")
    {
        drawcube(0,0,0,para.at(0),para.at(1),para.at(2));

    }else if(type=="cycler")
    {
        drawcylinder(0,0,0,para.at(0),para.at(1));
    }
    else if(type=="pipe")
    {
        drawpipe(0,0,0,para.at(0),para.at(1),para.at(2));
    }else if(type=="line")
    {
        if(para.size()<10)
            drawLine(0,0,0,para.at(0));
        else
            drawLine(para.at(0),para.at(1),para.at(2),para.at(3),para.at(4),para.at(5),para.at(6),para.at(7),para.at(8),para.at(9));
    }else if(type=="arc")
    {
        drawarc(0,0,0,para.at(0));
    }else if(type=="wedge")
    {
        drawwedge(para.at(0),para.at(1),para.at(2),para.at(3),para.at(4),para.at(5),para.at(6));
    }else if(type=="cyclerfan")
    {
        drawcylinderfan(0,0,0,para.at(0),para.at(1),para.at(2));

    }else if(type=="pipefan")
    {
        drawpipefan(0,0,0,para.at(0),para.at(1),para.at(2),para.at(3));
    }else
    {
        return;
    }

}

void OccView::showshapefromlist(QStringList list, bool issubcoord,Standard_Real transx,Standard_Real transy,Standard_Real transz,int axis,Standard_Real angle)
{
    if(issubcoord)
    {
        if(canmaketool)
        {
            if(list.size()<1) return;
            QString type=list.at(0);
            QVector<float> para;
            TopoDS_Shape shape;
            para.clear();
            for(int i=1;i<list.size();++i)
            {
                para<<QString(list.at(i)).toFloat();
            }
            gp_Trsf T=toolcood;
            T.SetTranslation(gp_Vec(suborg.X()+transx,suborg.Y()+transy,suborg.Z()+transz));
            gp_Pnt suborg1=T.TranslationPart();
            gp_Ax1 ax;
            switch (axis) {
            case 0:
                break;
            case 1:
                ax=gp_Ax1(suborg,gp_Dir(subvx.Normalized()));
                break;
            case 2:
                ax=gp_Ax1(suborg,gp_Dir(subvy.Normalized()));
                break;
            case 3:
                ax=gp_Ax1(suborg,gp_Dir(subvz.Normalized()));
                break;
            default:
                break;
            }
            T.SetRotation(ax,angle*PI/180);
            modelcood=T;
            gp_Vec subvz1=gp_Vec(T.Value(1,3),T.Value(2,3),T.Value(3,3)).Normalized();

            if(type=="box")
            {
                shape=make_cube(suborg1,subvz1,para.at(0),para.at(1),para.at(2));


            }else if(type=="cycler")
            {
                shape=make_cylinder(suborg1,subvz1,para.at(0),para.at(1));
            }
            else if(type=="pipe")
            {
                shape=make_pipe(suborg1,subvz1,para.at(0),para.at(1),para.at(2));
            }else if(type=="line")
            {
                if(para.size()<9)
                    shape=make_line(suborg1,subvz1,para.at(0));
                else
                {
                    gp_Pnt pnt(para.at(0)+suborg1.X(),para.at(1)+suborg1.Y(),para.at(2)+suborg1.Z());
                    gp_Vec vec(para.at(3)+subvz1.X(),para.at(4)+subvz1.Y(),para.at(5)+subvz1.Z());
                    curfacenormal=gp_Vec(para.at(6)+subvz1.X(),para.at(7)+subvz1.X(),para.at(8)+subvz1.X()).Normalized();
                    shape=make_line(pnt,vec,para.at(9));
                }

            }else if(type=="arc")
            {
                shape=make_arc(suborg1,subvz1,para.at(0));
            }else if(type=="wedge")
            {
                shape=make_wedge(suborg1,subvz1,para.at(0),para.at(1),para.at(2),para.at(3),para.at(4),para.at(5),para.at(6));
            }else if(type=="cyclerfan")
            {
                shape=make_cylinder_fan(suborg1,subvz1,para.at(0),para.at(1),para.at(2));

            }else if(type=="pipefan")
            {
                shape=make_pipe_fan(suborg1,subvz1,para.at(0),para.at(1),para.at(2),para.at(3));
            }else
            {
                return;
            }
            currentshape=shape;
            showshape(shape);

        }else
            return;

    }else
    {
        showshapefromlist(list);
    }
}


void OccView::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection =
            new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId) winId();

    // Create appropriate window for platform
#ifdef WNT
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
#else
    Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window) window_handle);
#endif

    // Create V3dViewer and V3d_View
    myViewer = new V3d_Viewer(GetGraphicDriver(), Standard_ExtString("viewer3d"));
    myView = myViewer->CreateView();
    myView->SetWindow(wind);
    if (!wind->IsMapped())
        wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    // Set up lights etc
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myView->SetBackgroundColor(Quantity_NOC_GRAY60);
    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    myContext->SetDisplayMode(AIS_Shaded, Standard_True);

    Handle(Prs3d_Drawer) t_hilight_style= myContext->HighlightStyle();
    t_hilight_style->SetMethod(Aspect_TOHM_COLOR);
    t_hilight_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);
    t_hilight_style->SetDisplayMode(1);
    t_hilight_style->SetTransparency(0.4f);
    myView->SetZoom(100);
}

const Handle(AIS_InteractiveContext)& OccView::getContext() const
{
    return myContext;
}

void OccView::paintEvent( QPaintEvent* /*theEvent*/ )
{

    if (myContext.IsNull())
    {
        init();
    }
    myView->Redraw();
}

void OccView::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void OccView::fitAll( void )
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void OccView::reset( void )
{
    myContext->EraseAll(true);
}

void OccView::pan( void )
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void OccView::zoom( void )
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void OccView::rotate( void )
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void OccView::readkukamodel()
{   
    base=getshapes("D:\\kukamodel\\base.step");
    TopLoc_Location pos=base.Location();
    joint1=getshapes("D:\\kukamodel\\joint1.step");
    joint2=getshapes("D:\\kukamodel\\joint2.step");
    joint3=getshapes("D:\\kukamodel\\joint3.step");
    joint4=getshapes("D:\\kukamodel\\joint4.step");
    joint5=getshapes("D:\\kukamodel\\joint5.step");
    joint6=getshapes("D:\\kukamodel\\joint6.step");
    ruangeline=getshapes("D:\\kukamodel\\ruangeline.step");
    joint1.Located(pos);
    joint2.Located(pos);
    joint3.Located(pos);
    joint4.Located(pos);
    joint5.Located(pos);
    joint6.Located(pos);
    ruangeline.Located(pos);

}

TopoDS_Shape OccView::getshapes(Standard_CString url)
{
    STEPControl_Reader reader;
    reader.ReadFile(url);

    reader.NbRootsForTransfer();
    // gets the number of transferable roots
    reader.TransferRoots();
    // translates all transferable roots, and returns the number of //successful translations
    TopoDS_Shape aShape;

    aShape= reader.OneShape();
    return  aShape;
}

void OccView::mousePressEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
        //myContext->MoveTo(theEvent->x(),theEvent->y(),myView,Standard_True);
        AIS_StatusOfPick t_pick_status=AIS_SOP_NothingSelected;
        if(qApp->keyboardModifiers()==Qt::ControlModifier)
        {
            t_pick_status= myContext->ShiftSelect(true);
        }
        else {
            t_pick_status=myContext->Select(true);
        }

    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    //    if((theEvent->button()&Qt::LeftButton)&&(theEvent->button()&Qt::RightButton))
    //    {
    //        myXmax=theEvent->x();
    //        myYmax=theEvent->y();
    //    }
    //    else if(theEvent->button()&Qt::LeftButton)
    //    {
    //        myContext->MoveTo(theEvent->x(),theEvent->y(),myView,Standard_True);
    //        AIS_StatusOfPick t_pick_status=AIS_SOP_NothingSelected;
    //        if(qApp->keyboardModifiers()==Qt::ControlModifier)
    //        {
    //            t_pick_status= myContext->ShiftSelect(true);

    //        }
    //        else {
    //            t_pick_status=myContext->Select(true);
    //        }
    //    }
    //    else if(theEvent->button()&Qt::MidButton)
    //    {
    //        myXmax=theEvent->x();
    //        myYmax=theEvent->y();
    //        myView->StartRotation(theEvent->x(),theEvent->y());
    //    }

}

void OccView::mouseReleaseEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MidButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    //myContext->MoveTo(theEvent->pos().x(),theEvent->pos().y(),myView,Standard_True);
}

void OccView::mouseMoveEvent( QMouseEvent * theEvent )
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
    //    if((theEvent->button()&Qt::LeftButton)&&(theEvent->button()&Qt::RightButton))
    //    {
    //        myView->Pan(theEvent->pos().x()-myXmax,myYmax - theEvent->pos().y());
    //        myXmax=theEvent->x();
    //        myYmax=theEvent->y();
    //    }
    //    else if(theEvent->button()&Qt::LeftButton)
    //    {
    //        if(qApp->keyboardModifiers()==Qt::ShiftModifier)
    //        {
    //            myView->Pan(theEvent->pos().x()-myXmax,myYmax - theEvent->pos().y());
    //            myXmax=theEvent->x();
    //            myYmax=theEvent->y();

    //        }
    //        else {
    //            myView->Rotation(theEvent->x(),theEvent->y());
    //        }
    //    }else
    //    {
    //        myContext->MoveTo(theEvent->pos().x(),theEvent->pos().y(),myView,Standard_True);
    //    }
}

void OccView::wheelEvent( QWheelEvent * theEvent )
{
    //onMouseWheel(theEvent->buttons(), theEvent->delta(), theEvent->pos());
    myView->StartZoomAtPoint(theEvent->pos().x(),theEvent->pos().y());
    myView->ZoomAtPoint(0,0,theEvent->angleDelta().y(),0);
}


void OccView::onLButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

}

void OccView::onMButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void OccView::onRButtonDown( const int /*theFlags*/, const QPoint /*thePoint*/ )
{

}

void OccView::onMouseWheel( const int /*theFlags*/, const int theDelta, const QPoint thePoint )
{
    Standard_Integer aFactor = 10;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }
    //qDebug()<<thePoint.x()<<thePoint.y()<<aX<<aY;
    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}

void OccView::addItemInPopup( QMenu* /*theMenu*/ )
{
}

void OccView::popup( const int /*x*/, const int /*y*/ )
{
}

void OccView::onLButtonUp( const int theFlags, const QPoint thePoint )
{
    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void OccView::onMButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void OccView::onRButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    popup(thePoint.x(), thePoint.y());
}

void OccView::onMouseMove( const int theFlags, const QPoint thePoint )
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MidButton)
    {
        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            //qDebug()<<"rotation"<<thePoint.x()<<thePoint.y();
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            //myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;

        default:
            break;
        }
    }

}

void OccView::dragEvent( const int x, const int y )
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void OccView::multiDragEvent( const int x, const int y )
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();

}

void OccView::inputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(Standard_True);

    emit selectionChanged();
}

void OccView::multiInputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void OccView::moveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::multiMoveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void OccView::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}

void OccView::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

gp_Trsf OccView::getcoordT(gp_Trsf T, gp_Vec trans, gp_Dir dir, Standard_Real angle)
{
    gp_Trsf Td;
    Td.SetTranslation(trans);
    gp_Ax1 p2(gp_Pnt(trans.X(),trans.Y(),trans.Z()),dir);
    Td.SetRotation(p2, angle*M_PI/180.0);
    Td = T*Td;
    return  Td;
}

void OccView::initT(QVector<float> robangles)
{
    TopLoc_Location baseloc = base.Location();
    baseloc.Identity();
    T_base.SetTranslation(gp_Vec(0,0,229.7));
    T_base.SetRotation(gp_Ax1(gp_Pnt(0,0,229.7),gp_Dir(0,0,1)),robangles.at(0));
    T_joint1=getcoordT(T_base,gp_Vec(0,0,400),gp_Dir(0,1,0),robangles.at(1));
    T_joint2=getcoordT(T_joint1,gp_Vec(33,0,850),gp_Dir(0,1,0),robangles.at(2));
    T_joint3=getcoordT(T_joint2,gp_Vec(240,0,880),gp_Dir(1,0,0),robangles.at(3));
    T_joint4=getcoordT(T_joint3,gp_Vec(440,0,880),gp_Dir(0,1,0),robangles.at(4));
    T_joint5=getcoordT(T_joint4,gp_Vec(517,0,880),gp_Dir(1,0,0),robangles.at(5));


}

void OccView::startanimation(QVector<float> angle)
{
    gp_Trsf Tb=T_base;
    gp_Trsf Tj1=T_joint1;
    gp_Trsf Tj2=T_joint2;
    gp_Trsf Tj3=T_joint3;
    gp_Trsf Tj4=T_joint4;
    gp_Trsf Tj5=T_joint5;
    initT(angle);
    Handle(AIS_Animation) base_animation=new AIS_Animation("base");
    Handle(AIS_AnimationObject) base_ao=new AIS_AnimationObject("base",myContext,anAisModel_base,Tb,T_base);
    Handle(AIS_AnimationObject) base_j1=new AIS_AnimationObject("joint1",myContext,anAisModel_joint1,Tj1,T_joint1);
    //Handle(AIS_AnimationObject) base_j2=new AIS_AnimationObject("joint2",myContext,anAisModel_joint2,Tj2,T_joint2);
    //Handle(AIS_AnimationObject) base_j3=new AIS_AnimationObject("joint3",myContext,anAisModel_joint3,Tj3,T_joint3);
    //Handle(AIS_AnimationObject) base_j4=new AIS_AnimationObject("joint4",myContext,anAisModel_joint4,Tj4,T_joint4);
    //Handle(AIS_AnimationObject) base_j5=new AIS_AnimationObject("joint5",myContext,anAisModel_joint5,Tj5,T_joint5);

    base_animation->SetStartPts(0);
    base_animation->SetOwnDuration(10);
    base_animation->Add(base_ao);
    base_animation->Add(base_j1);
    //base_animation->Add(base_j2);
    //base_animation->Add(base_j3);
    //base_animation->Add(base_j4);
    //base_animation->Add(base_j5);

    //base_animation->SetOwnDuration(10);

    base_animation->StartTimer(0,1,true);
    //base_j1->StartTimer(0,1,true);

    while (!base_animation->IsStopped()) {
        base_animation->UpdateTimer();
        myContext->UpdateCurrentViewer();
    }
}
void OccView::updatemodel()
{
    if(showrangeline)
    {
        Handle(AIS_Shape) anAisModel0 = new AIS_Shape(ruangeline);
        anAisModel0->SetColor(Quantity_NOC_RED);
        anAisModel0->SetTransparency(0.1);
        myContext->Display(anAisModel0, Standard_True);
    }
    //base
    Handle(AIS_Shape) anAisModel = new AIS_Shape(base);
    //anAisModel->Set(joint1);
    anAisModel->SetColor(Quantity_NOC_GRAY40);
    anAisModel->SetTransparency(0.0);
    this->myContext->Display(anAisModel, Standard_True);
    //joint1
    gp_Trsf T1;
    TopLoc_Location baseloc = base.Location();
    baseloc.Identity();
    T1.SetTranslation(gp_Vec(0,0,229.7));
    T1.SetRotation(gp_Ax1(gp_Pnt(0,0,229.7),gp_Dir(0,0,1)),robangle.at(0));
    BRepBuilderAPI_Transform traf1(T1);
    traf1.Perform(joint1);
    Handle(AIS_Shape) anAisModel1 = new AIS_Shape(traf1.Shape());
    anAisModel1->SetColor(Quantity_NOC_GRAY70);
    anAisModel1->SetTransparency(0.0);
    myContext->Display(anAisModel1, Standard_True);
    //joint2
    gp_Trsf T2;
    T2=getcoordT(T1,gp_Vec(0,0,400),gp_Dir(0,1,0),robangle.at(1));
    BRepBuilderAPI_GTransform traf2(joint2,T2);
    Handle(AIS_Shape) anAisModel2 = new AIS_Shape(traf2.Shape());
    anAisModel2->SetColor(Quantity_NOC_GRAY70);
    anAisModel2->SetTransparency(0.0);
    myContext->Display(anAisModel2, Standard_True);

    //joint3
    gp_Trsf T3;

    T3=getcoordT(T2,gp_Vec(33,0,850),gp_Dir(0,1,0),robangle.at(2));
    BRepBuilderAPI_Transform traf3(T3);
    traf3.Perform(joint3);
    Handle(AIS_Shape) anAisModel3 = new AIS_Shape(traf3.Shape());
    anAisModel3->SetColor(Quantity_NOC_GRAY70);
    anAisModel3->SetTransparency(0.0);
    myContext->Display(anAisModel3, Standard_True);
    //joint4
    gp_Trsf T4;
    T4=getcoordT(T3,gp_Vec(240,0,880),gp_Dir(1,0,0),robangle.at(3));
    BRepBuilderAPI_Transform traf4(T4);
    traf4.Perform(joint4);
    Handle(AIS_Shape) anAisModel4 = new AIS_Shape(traf4.Shape());
    anAisModel4->SetColor(Quantity_NOC_GRAY70);
    anAisModel4->SetTransparency(0.0);
    myContext->Display(anAisModel4, Standard_True);
    //joint5
    gp_Trsf T5;
    T5=getcoordT(T4,gp_Vec(440,0,880),gp_Dir(0,1,0),robangle.at(4));
    BRepBuilderAPI_Transform traf5(T5);
    traf5.Perform(joint5);
    Handle(AIS_Shape) anAisModel5 = new AIS_Shape(traf5.Shape());
    anAisModel5->SetColor(Quantity_NOC_GRAY70);
    anAisModel5->SetTransparency(0.0);
    myContext->Display(anAisModel5, Standard_True);
    //joint6
    gp_Trsf T6;
    T6=getcoordT(T5,gp_Vec(517,0,880),gp_Dir(1,0,0),robangle.at(5));
    BRepBuilderAPI_Transform traf6(T6);
    traf6.Perform(joint6);
    Handle(AIS_Shape) anAisModel6 = new AIS_Shape(traf6.Shape());
    anAisModel6->SetColor(Quantity_NOC_GRAY40);
    anAisModel6->SetTransparency(0.0);
    myContext->Display(anAisModel6, Standard_True);
    fitAll();
}


TopoDS_Shape OccView::MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness)
{
    gp_Pnt aPnt1(-myWidth / 2., 0, 0);
    gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt3(0, -myThickness / 2., 0);
    gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt5(myWidth / 2., 0, 0);

    // Profile : Define the Geometry
    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);

    // Profile : Define the Topology
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire  = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);

    // Complete Profile
    gp_Ax1 xAxis = gp::OX();
    gp_Trsf aTrsf;

    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();

    // Body : Prism the Profile
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
    gp_Vec aPrismVec(0, 0, myHeight);
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);

    // Body : Apply Fillets
    BRepFilletAPI_MakeFillet mkFillet(myBody);
    TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
    while(anEdgeExplorer.More()){
        TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        //Add edge to fillet algorithm
        mkFillet.Add(myThickness / 12., anEdge);
        anEdgeExplorer.Next();
    }

    myBody = mkFillet.Shape();

    // Body : Add the Neck
    gp_Pnt neckLocation(0, 0, myHeight);
    gp_Dir neckAxis = gp::DZ();
    gp_Ax2 neckAx2(neckLocation, neckAxis);

    Standard_Real myNeckRadius = myThickness / 4.;
    Standard_Real myNeckHeight = myHeight / 10.;

    BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
    TopoDS_Shape myNeck = MKCylinder.Shape();

    myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

    // Body : Create a Hollowed Solid
    TopoDS_Face   faceToRemove;
    Standard_Real zMax = -1;

    for(TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()){
        TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
        // Check if <aFace> is the top face of the bottle抯 neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ   = aPnt.Z();
            if(aZ > zMax){
                zMax = aZ;
                faceToRemove = aFace;
            }
        }
    }

    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);
    BRepOffsetAPI_MakeThickSolid aSolidMaker;
    aSolidMaker.MakeThickSolidByJoin(myBody, facesToRemove, -myThickness / 50, 1.e-3);
    myBody = aSolidMaker.Shape();
    // Threading : Create Surfaces
    Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);

    // Threading : Define 2D Curves
    gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
    gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
    gp_Ax2d anAx2d(aPnt, aDir);

    Standard_Real aMajor = 2. * M_PI;
    Standard_Real aMinor = myNeckHeight / 10;

    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
    // Threading : Build Edges and Wires
    TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);

    // Create Threading
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);

    TopoDS_Shape myThreading = aTool.Shape();

    // Building the Resulting Compound
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound (aRes);
    aBuilder.Add (aRes, myBody);
    aBuilder.Add (aRes, myThreading);

    return aRes;
}

void OccView::setrobangle(QVector<float> angle)
{
    if(angle.size()!=6) return;
    for(int i=0;i<6;++i)
    {
        if(angle.at(i)> -0.00001 && angle.at(i)< 0.00001)
            robangle[i]=0;
        else
            robangle[i]=angle.at(i);
    }
    startanimation(robangle);


}

void OccView::showruanges(bool isshow)
{
    if(showrangeline==isshow) return;
    showrangeline=isshow;
}

QVector<float> OccView::getcurrentangle()
{
    return  robangle;
}

void OccView::animationmodel()
{
    if(showrangeline)
    {
        Handle(AIS_Shape) anAisModel0 = new AIS_Shape(ruangeline);
        anAisModel0->SetColor(Quantity_NOC_RED);
        anAisModel0->SetTransparency(0.1);
        myContext->Display(anAisModel0, Standard_True);
    }
    //base
    anAisModel_base = new AIS_Shape(base);
    //anAisModel->Set(joint1);
    anAisModel_base->SetColor(Quantity_NOC_GRAY40);
    anAisModel_base->SetTransparency(0.0);
    myContext->Display(anAisModel_base, Standard_True);
    //joint1
    anAisModel_joint1 = new AIS_Shape(joint1);
    anAisModel_joint1->SetColor(Quantity_NOC_GRAY70);
    anAisModel_joint1->SetTransparency(0.0);
    myContext->Display(anAisModel_joint1, Standard_True);
    //joint2
    anAisModel_joint2= new AIS_Shape(joint2);
    anAisModel_joint2->SetColor(Quantity_NOC_GRAY70);
    anAisModel_joint2->SetTransparency(0.0);
    myContext->Display(anAisModel_joint2, Standard_True);

    //joint3
    anAisModel_joint3 = new AIS_Shape(joint3);
    anAisModel_joint3->SetColor(Quantity_NOC_GRAY70);
    anAisModel_joint3->SetTransparency(0.0);
    myContext->Display(anAisModel_joint3, Standard_True);
    //joint4
    anAisModel_joint4 = new AIS_Shape(joint4);
    anAisModel_joint4->SetColor(Quantity_NOC_GRAY70);
    anAisModel_joint4->SetTransparency(0.0);
    myContext->Display(anAisModel_joint4, Standard_True);
    //joint5
    anAisModel_joint5 = new AIS_Shape(joint5);
    anAisModel_joint5->SetColor(Quantity_NOC_GRAY70);
    anAisModel_joint5->SetTransparency(0.0);
    myContext->Display(anAisModel_joint5, Standard_True);
    //joint6
    anAisModel_joint6 = new AIS_Shape(joint6);
    anAisModel_joint6->SetColor(Quantity_NOC_GRAY40);
    anAisModel_joint6->SetTransparency(0.0);
    myContext->Display(anAisModel_joint6, Standard_True);
    fitAll();
}

void OccView::showgrounds(bool isshow)
{
    showground=isshow;
    if(showground)
    {
        myViewer->SetRectangularGridValues(0,0,20,20,0);
        myViewer->SetRectangularGridGraphicValues(1000.01,1000.01,0);
        myViewer->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);
    }
    myView->Redraw();
}

void OccView::showcycground()
{
    myViewer->SetCircularGridValues(0,0,20,20,0);
    myViewer->SetCircularGridGraphicValues(500.01,0);
    myViewer->ActivateGrid(Aspect_GT_Circular,Aspect_GDM_Lines);
    myView->Redraw();
}

void OccView::clearsweeppoint()
{
    sweeps_line_point.clear();
}

void OccView::addpoints(QString type, Standard_Real x, Standard_Real y, Standard_Real z)
{
    QPair<QString,QVector<gp_Pnt> > pair;
    QPair<QString,QVector<gp_Pnt> > newpair;
    if(sweeps_line_point.size()==0)
    {
        pair.first=type;
        QVector<gp_Pnt> p;
        gp_Pnt point=gp_Pnt(x,y,z);
        p.push_back(point);
        pair.second=p;
        sweeps_line_point.push_back(pair);
    }else if(type=="line")
    {
        pair=sweeps_line_point.last();
        if(pair.first==type && pair.second.size()==2)
        {
            QVector<gp_Pnt> p;
            gp_Pnt point1=pair.second.at(1);
            gp_Pnt point2=gp_Pnt(x,y,z);
            p<<point1<<point2;
            newpair.first=type;
            newpair.second=p;
            sweeps_line_point.push_back(newpair);
        }else if(pair.first==type && pair.second.size()<2)
        {
            gp_Pnt point2=gp_Pnt(x,y,z);
            sweeps_line_point.last().second.push_back(point2);
        }
        else if(pair.first != type && pair.second.size()==3)
        {
            newpair.first=type;
            QVector<gp_Pnt> p;
            gp_Pnt point1=pair.second.last();
            gp_Pnt point2=gp_Pnt(x,y,z);
            p<<point1<<point2;
            newpair.second=p;
            sweeps_line_point.push_back(newpair);
        }
        else
        {
            qDebug()<<"error";
        }
    }else if(type=="arc")
    {
        pair=sweeps_line_point.last();
        if(pair.first==type && pair.second.size()==3)
        {
            QVector<gp_Pnt> p;
            gp_Pnt point1=pair.second.last();
            gp_Pnt point2=gp_Pnt(x,y,z);
            p<<point1<<point2;
            newpair.first=type;
            newpair.second=p;
            sweeps_line_point.push_back(newpair);
        }else if(pair.first==type && pair.second.size()<3)
        {
            gp_Pnt point2=gp_Pnt(x,y,z);
            sweeps_line_point.last().second.push_back(point2);
        }
        else if(pair.first != type && pair.second.size()==2)
        {
            newpair.first=type;
            QVector<gp_Pnt> p;
            gp_Pnt point1=pair.second.last();
            gp_Pnt point2=gp_Pnt(x,y,z);
            p<<point1<<point2;
            newpair.second=p;
            sweeps_line_point.push_back(newpair);
        }
        else
        {
            qDebug()<<"error";
        }
    }
    pair=sweeps_line_point.last();
    if((pair.first=="line"&&pair.second.size()==2)||(pair.first=="arc"&&pair.second.size()==3))
    {
        reset();
        make_wires();
    }
}

QVector<QPair<QString, QVector<gp_Pnt> > > OccView::getsweepline()
{
    return  sweeps_line_point;
}

void OccView::drawcube(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz)
{
    TopoDS_Shape shape=make_cube(x,y,z,_dx,_dy,_dz);
    showshape(shape);
}

void OccView::drawsphere(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _r)
{
    TopoDS_Shape shape=make_sphere(x,y,z,_r);
    showshape(shape);
}

void OccView::drawpipe(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H)
{
    TopoDS_Shape shape=make_pipe(x,y,z,_Rmax,_Rmin,_H);
    showshape(shape);
}

void OccView::drawLine(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _H)
{
    TopoDS_Shape shape=make_line(x,y,z,_H);
    //TopoDS_Shape shape=make_line(gp_Pnt(1,1,10),gp_Vec(2,2,2),_H);
    showshape(shape);
}

void OccView::drawLine(Standard_Real orgx, Standard_Real org_y, Standard_Real org_z, Standard_Real dir_x, Standard_Real dir_y, Standard_Real dir_z, Standard_Real norm_x, Standard_Real norm_y, Standard_Real norm_z, Standard_Real _H)
{
    gp_Pnt org(orgx,org_y,org_z);
    gp_Vec vec(dir_x,dir_y,dir_z);

    TopoDS_Shape shape=make_line(org,vec,_H);
    gp_Vec norm(norm_x,norm_y,norm_z);
    curfacenormal=norm.Normalized();
    showshape(shape);
}

void OccView::drawarc(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real radius, Standard_Real _angle)
{
    TopoDS_Shape shape=make_arc(x,y,z,radius,_angle);
    //TopoDS_Shape shape=make_arc(gp_Pnt(1,1,30),gp_Vec(2,2,2),radius,_angle);
    showshape(shape);
}

void OccView::drawwedge(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax, Standard_Real _zmin)
{
    TopoDS_Shape shape=make_wedge(x,y,z,_dmax,_dmin,_zmax,_zmin);
    showshape(shape);
}

void OccView::drawsweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz)
{
    TopoDS_Shape shape=make_sweeps(wx,wy,wz);
    showshape(shape);
}

void OccView::drawcylinder(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _R, Standard_Real _H)
{
    TopoDS_Shape shape=make_cylinder(x,y,z,_R,_H);
    showshape(shape);
}

void OccView::drawcylinderfan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _R, Standard_Real _H, Standard_Real _angle)
{
    TopoDS_Shape shape=make_cylinder_fan(x,y,z,_R,_H,_angle);
    showshape(shape);
}

void OccView::drawcone(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _r1, Standard_Real r2, Standard_Real _h)
{
    TopoDS_Shape shape=make_cone(x,y,z,_r1,r2,_h);
    showshape(shape);
}

void OccView::drawtorus(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _r1, Standard_Real _r2)
{
    TopoDS_Shape shape=make_torus(x,y,z,_r1,_r2);
    showshape(shape);
}

void OccView::drawpipefan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H, Standard_Real _angle)
{
    TopoDS_Shape shape=make_pipe_fan(x,y,z,_Rmax,_Rmin,_H,_angle);
    showshape(shape);
}



/*
static Standard_Integer transform(Draw_Interpretor& ,Standard_Integer n,const char** a)
{
  if (n <= 1) return 1;

  gp_Trsf T;
  Standard_Integer last = n;
  const char* aName = a[0];

  Standard_Boolean isBasic = Standard_False;

  if (!strcmp(aName,"reset")) {
  }
  else {
    isBasic = (aName[0] == 'b');
    aName++;

    if (!strcmp(aName,"move")) {
      if (n < 3) return 1;
      TopoDS_Shape SL = DBRep::Get(a[n-1]);
      if (SL.IsNull()) return 0;
      T = SL.Location().Transformation();
      last = n-1;
    }
    else if (!strcmp(aName,"translate")) {
      if (n < 5) return 1;
      T.SetTranslation(gp_Vec(Draw::Atof(a[n-3]),Draw::Atof(a[n-2]),Draw::Atof(a[n-1])));
      last = n-3;
    }
    else if (!strcmp(aName,"rotate")) {
      if (n < 9) return 1;
      T.SetRotation(gp_Ax1(gp_Pnt(Draw::Atof(a[n-7]),Draw::Atof(a[n-6]),Draw::Atof(a[n-5])),
                    gp_Vec(Draw::Atof(a[n-4]),Draw::Atof(a[n-3]),Draw::Atof(a[n-2]))),
                    Draw::Atof(a[n-1])* (M_PI / 180.0));
      last = n-7;
    }
    else if (!strcmp(aName,"mirror")) {
      if (n < 8) return 1;
      T.SetMirror(gp_Ax2(gp_Pnt(Draw::Atof(a[n-6]),Draw::Atof(a[n-5]),Draw::Atof(a[n-4])),
                  gp_Vec(Draw::Atof(a[n-3]),Draw::Atof(a[n-2]),Draw::Atof(a[n-1]))));
      last = n-6;
    }
    else if (!strcmp(aName,"scale")) {
      if (n < 6) return 1;
      T.SetScale(gp_Pnt(Draw::Atof(a[n-4]),Draw::Atof(a[n-3]),Draw::Atof(a[n-2])),Draw::Atof(a[n-1]));
      last = n-4;
    }
  }

  if (T.Form() == gp_Identity || isBasic) {
    TopLoc_Location L(T);
    for (Standard_Integer i = 1; i < last; i++) {
      TopoDS_Shape S = DBRep::Get(a[i]);
      if (S.IsNull())
      {
        std::cerr << "Error: " << a[i] << " is not a valid shape\n";
        return 1;
      }
      else
        DBRep::Set(a[i],S.Located(L));
    }
  }
  else {
    BRepBuilderAPI_Transform trf(T);
    for (Standard_Integer i = 1; i < last; i++) {
      TopoDS_Shape S = DBRep::Get(a[i]);
      if (S.IsNull()) {
        std::cerr << "Error: " << a[i] << " is not a valid shape\n";
        return 1;
      }
      else {
        trf.Perform(S);
        if (!trf.IsDone())
          return 1;
        DBRep::Set(a[i],trf.Shape());
      }
    }
  }
  return 0;
}
*/
