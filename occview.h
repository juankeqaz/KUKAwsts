/*
*    Copyright (c) 2018 Shing Liu All Rights Reserved.
*
*           File : OccView.h
*         Author : Shing Liu(eryar@163.com)
*           Date : 2018-01-09 21:00
*        Version : OpenCASCADE7.2.0 & Qt5.7.1
*
*    Description : Adapte OpenCASCADE view for Qt.
*/

#ifndef _OCCVIEW_H_
#define _OCCVIEW_H_

#include <QGLWidget>

#include <AIS_InteractiveContext.hxx>
#include <QDesktopWidget>
#include <QApplication>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>

#include <AIS_Shape.hxx>
#include "STEPCAFControl_Reader.hxx"
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <TopoDS_Compound.hxx>

#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <Geom_Plane.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <BRepLib.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <AIS_AnimationObject.hxx>
#include <AIS_Animation.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeSweep.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <GProp_PEquation.hxx>
#include <QStringList>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <Poly_Triangulation.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>

#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomLProp_SLProps.hxx>




#include <QVector>


#include "globalvar.h"

class QMenu;
class QRubberBand;
#if defined (qApp)
#undef qApp
#endif
#define qApp (static_cast<QApplication*> (QCoreApplication::instance()))
//! adapted a QGLWidget for OpenCASCADE viewer.

#define PI 3.14159265



class OccView : public QGLWidget
{
    Q_OBJECT

public:
    //! mouse actions.
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    };

public:
    //! constructor.
    OccView(QWidget* parent=0);

    const Handle(AIS_InteractiveContext)& getContext() const;
    void axisposition(int p);

signals:
    void selectionChanged(void);

public slots:
    //! operations for the view.
    void pan(void);
    void fitAll(void);
    void reset(void);
    void zoom(void);
    void rotate(void);
    //void removeshape();
    void readkukamodel();
    TopoDS_Shape getshapes(Standard_CString url);
    void updatemodel();
    TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
    void setrobangle(QVector<float> angle);
    void showruanges(bool isshow);
    QVector<float> getcurrentangle();
    void animationmodel();
    void showgrounds(bool isshow);
    void showcycground();
    void clearsweeppoint();
    void addpoints(QString type, Standard_Real x, Standard_Real y, Standard_Real z);
    QVector<QPair<QString,QVector<gp_Pnt> > > getsweepline();
    void showshape(TopoDS_Shape &shape);
    void showshapefromlist(QStringList list);
    void showshapefromlist(QStringList list, bool issubcoord, Standard_Real transx=0, Standard_Real transy=0, Standard_Real transz=0, int axis=0, Standard_Real angle=0);

    void drawcube(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz);
    void drawsphere(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r);
    void drawpipe(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H);
    void drawLine(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _H);
    void drawLine(Standard_Real orgx,Standard_Real org_y,Standard_Real org_z,Standard_Real dir_x,Standard_Real dir_y,Standard_Real dir_z,Standard_Real norm_x,Standard_Real norm_y,Standard_Real norm_z,Standard_Real _H);
    void drawarc(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real radius,Standard_Real _angle=0);
    void drawwedge(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax,Standard_Real _zmin);
    void drawsweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz);
    void drawcylinder(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _R,Standard_Real _H);

    void drawcylinderfan(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _R,Standard_Real _H,Standard_Real _angle);
    void drawcone(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1,Standard_Real r2,Standard_Real _h);
    void drawtorus(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1,Standard_Real _r2);
    void drawpipefan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H,Standard_Real _angle);

    void makexyz(Standard_Real x=0,Standard_Real y=0,Standard_Real z=0);
    void maketoolxyz(Standard_Real length=50);
    void makecoord(float x0,float y0,float z0,float x1,float y1,float z1,float x2,float y2,float z2);
    void getedgepoint(QString dir);
    TopoDS_Wire pathplan(TopoDS_Face face);
    QVector<gp_Pnt> pathpoint(TopoDS_Wire awire);
    QVector<gp_Pnt> pathpoint(TopoDS_Face aface);
    Standard_Real distansall(QVector<gp_Pnt> vec);
    void splitpath(const QVector<gp_Pnt> vec,const int num,QVector<gp_Pnt> &endvec);

    QVector<gp_Pnt> getpathpoint();
    void getnormal(TopoDS_Face &face);



    bool hasshapes();




protected:
    // Paint events.
    virtual void paintEvent(QPaintEvent* theEvent);
    virtual void resizeEvent(QResizeEvent* theEvent);

    // Mouse events.
    virtual void mousePressEvent(QMouseEvent* theEvent);
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent * theEvent);
    virtual void wheelEvent(QWheelEvent * theEvent);

    // Button events.
    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
    virtual void onLButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onRButtonUp(const int theFlags, const QPoint thePoint);
    virtual void onMouseMove(const int theFlags, const QPoint thePoint);

    // Popup menu.
    virtual void addItemInPopup(QMenu* theMenu);


protected:
    void init(void);
    void popup(const int x, const int y);
    void dragEvent(const int x, const int y);
    void inputEvent(const int x, const int y);
    void moveEvent(const int x, const int y);
    void multiMoveEvent(const int x, const int y);
    void multiDragEvent(const int x, const int y);
    void multiInputEvent(const int x, const int y);
    void drawRubberBand(const int minX, const int minY, const int maxX, const int maxY);
    void panByMiddleButton(const QPoint& thePoint);
    gp_Trsf getcoordT(gp_Trsf T,gp_Vec trans,gp_Dir dir,Standard_Real angle);
    void initT(QVector<float> robangles);
    void startanimation(QVector<float> angle);
public:
    TopoDS_Shape make_cube(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz);
    TopoDS_Shape make_cube(gp_Pnt org,gp_Vec vec, Standard_Real _dx, Standard_Real _dy, Standard_Real _dz);
    TopoDS_Shape make_sphere(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r);
    TopoDS_Shape make_cylinder(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _R,Standard_Real _H);
    TopoDS_Shape make_cylinder(gp_Pnt org,gp_Vec vec,Standard_Real _R,Standard_Real _H);
    TopoDS_Shape make_cone(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1,Standard_Real r2,Standard_Real _h);
    TopoDS_Shape make_cone(gp_Pnt org,gp_Vec vec,Standard_Real _r1,Standard_Real r2,Standard_Real _h);
    TopoDS_Shape make_torus(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _r1,Standard_Real _r2);
    TopoDS_Shape make_torus(gp_Pnt org,gp_Vec vec,Standard_Real _r1,Standard_Real _r2);
    TopoDS_Shape make_wedge(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax,Standard_Real _zmin);
    TopoDS_Shape make_wedge(gp_Pnt org,gp_Vec vec,Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _dmax, Standard_Real _dmin, Standard_Real _zmax,Standard_Real _zmin);
    TopoDS_Shape make_cylinder_fan(Standard_Real x, Standard_Real y, Standard_Real z,Standard_Real _R,Standard_Real _H,Standard_Real _angle);
    TopoDS_Shape make_cylinder_fan(gp_Pnt org,gp_Vec vec,Standard_Real _R,Standard_Real _H,Standard_Real _angle);
    TopoDS_Shape make_pipe(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H);
    TopoDS_Shape make_pipe(gp_Pnt org,gp_Vec vec, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H);
    TopoDS_Shape make_pipe_fan(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H,Standard_Real _angle);
    TopoDS_Shape make_pipe_fan(gp_Pnt org,gp_Vec vec, Standard_Real _Rmax, Standard_Real _Rmin, Standard_Real _H,Standard_Real _angle);
    TopoDS_Shape make_line(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real _H);
    TopoDS_Shape make_line(gp_Pnt org,gp_Vec vec, Standard_Real _H);
    TopoDS_Shape make_arc(Standard_Real x, Standard_Real y, Standard_Real z, Standard_Real radius,Standard_Real _angle=0);
    TopoDS_Shape make_arc(gp_Pnt org,gp_Vec vec, Standard_Real radius,Standard_Real _angle=0);
    void make_wires();
    TopoDS_Shape make_sweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz);

private:
    TopoDS_Face getupface(TopoDS_Shape shape);
    TopoDS_Face getdownface(TopoDS_Shape shape);
    TopoDS_Face getfrontface(TopoDS_Shape shape);
    TopoDS_Face getbackface(TopoDS_Shape shape);
    TopoDS_Face getleftface(TopoDS_Shape shape);
    TopoDS_Face getrightface(TopoDS_Shape shape);
    void printpnt(const gp_Pnt pnt);
    bool isequal(const gp_Pnt pnt1,gp_Pnt pnt2);


private:

    //! the occ viewer.
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) myView;

    //! the occ context.
    Handle(AIS_InteractiveContext) myContext;

    //! save the mouse position.
    Standard_Integer myXmin;
    Standard_Integer myYmin;
    Standard_Integer myXmax;
    Standard_Integer myYmax;

    //! the mouse current mode.
    CurrentAction3d myCurrentMode;

    //! save the degenerate mode state.
    Standard_Boolean myDegenerateModeIsOn;

    //! rubber rectangle for the mouse selection.
    QRubberBand* myRectBand;
    TopoDS_Shape base;
    TopoDS_Shape joint1;
    TopoDS_Shape joint2;
    TopoDS_Shape joint3;
    TopoDS_Shape joint4;
    TopoDS_Shape joint5;
    TopoDS_Shape joint6;
    TopoDS_Shape ruangeline;
    QVector<float> robangle;
    bool showrangeline;

    Handle(AIS_Shape) anAisModel_base;
    Handle(AIS_Shape) anAisModel_joint1;
    Handle(AIS_Shape) anAisModel_joint2;
    Handle(AIS_Shape) anAisModel_joint3;
    Handle(AIS_Shape) anAisModel_joint4;
    Handle(AIS_Shape) anAisModel_joint5;
    Handle(AIS_Shape) anAisModel_joint6;

    gp_Trsf T_base,T_joint1,T_joint2,T_joint3,T_joint4,T_joint5;

    bool showground=true;
    QVector<QPair<QString,QVector<gp_Pnt> > > sweeps_line_point;
    gp_Vec subvx=gp_Vec(1,0,0);
    gp_Vec subvy=gp_Vec(0,1,0);
    gp_Vec subvz=gp_Vec(0,0,1);
    gp_Pnt suborg=gp_Pnt(0,0,0);
    gp_Trsf toolcood;
    gp_Trsf modelcood;
    bool canmaketool=false;
    TopoDS_Shape currentshape;
    TopoDS_Shape viewshape;
    QVector<gp_Pnt> pathp;
    gp_Dir curfacenormal;
};

#endif // _OCCVIEW_H_
