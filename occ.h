#ifndef OCC_H
#define OCC_H

#include <AIS_InteractiveContext.hxx>
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

#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Prs3d_Drawer.hxx>

#include <TopAbs_Orientation.hxx>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <StlAPI_Reader.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XSControl.hxx>
#include <XSControl_WorkSession.hxx>
#include <Transfer_TransientProcess.hxx>
#include <IFSelect_ReturnStatus.hxx>


#include <QVector>
#include <QList>
#include <QVector3D>

#include <Eigen/Eigen>
#include <QDebug>

static Standard_Real distansall(QVector<gp_Pnt> vec)
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
static void splitpath(const QVector<gp_Pnt> vec, const int num, QVector<gp_Pnt> &endvec)
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
        endvec.push_back(pi);
    }
}


static void pathpointformface(TopoDS_Face face,QVector<gp_Pnt> pointvec)
{
    QVector<gp_Pnt> vecline1;
    QVector<gp_Pnt> vecline2;
    if(face.IsNull())
        return;
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
            if(vecline1.last().X()==strpnt.X()&&vecline1.last().Y()==strpnt.Y()&&vecline1.last().Z()==strpnt.Z())
            {
                vecline1.push_back(endpnt);
            }
            else if(vecline2.size()==0)
            {
                vecline2.push_front(endpnt);
                vecline2.push_front(strpnt);
            }else if(vecline2.first().X()==endpnt.X()&&vecline2.first().Y()==endpnt.Y()&&vecline2.first().Z()==endpnt.Z())
            {
                vecline2.push_front(strpnt);
            }
        }

    }
    if(vecline1.first().X()==vecline2.first().X()&&vecline1.first().Y()==vecline2.first().Y()&&vecline1.first().Z()==vecline2.first().Z())
        vecline2.pop_front();
    if(vecline1.last().X()==vecline2.last().X()&&vecline1.last().Y()==vecline2.last().Y()&&vecline1.last().Z()==vecline2.last().Z())
        vecline1.pop_back();
    Standard_Real d1=distansall(vecline1);
    Standard_Real d2=distansall(vecline2);
    Standard_Real step=5;

    int st=0;
    if(d1 >= d2)
        st=ceil(d1 / step);
    else
        st=ceil(d2 / step);
    splitpath(vecline1,st,pointvec);
    splitpath(vecline2,st,pointvec);
}
static bool isequal(const gp_Pnt pnt1,gp_Pnt pnt2)
{
    if(pnt1.X()==pnt2.X() && pnt1.Y()==pnt2.Y() && pnt1.Z()==pnt2.Z())
    {
        return  true;
    }else
        return  false;
}
static gp_Pnt getnearstpoint(const gp_Pnt org,const QVector<gp_Pnt> array)
{
    gp_Pnt temp;
    if(array.size()==0) return  org;
    Standard_Real Dis=1000;
    for(int i=0;i<array.size();++i)
    {
        Standard_Real D=org.Distance(array.at(i));
        if(D<Dis)
        {
            Dis=D;
            temp=array.at(i);
        }
    }
    return  temp;
}
static gp_Pnt getfarststpoint(const gp_Pnt org,const QVector<gp_Pnt> array)
{
    gp_Pnt temp;
    if(array.size()==0) return  org;
    Standard_Real Dis=0;
    for(int i=0;i<array.size();++i)
    {
        Standard_Real D=org.Distance(array.at(i));
        if(D>Dis)
        {
            Dis=D;
            temp=array.at(i);
        }
    }
    return  temp;
}
static void de_duplication(QVector<gp_Pnt>& vec)
{
    QList<QVector3D> list;
    if(vec.size()>0)
    {
        for(int i=0;i<vec.size();++i)
        {
            QVector3D p=QVector3D(vec.at(i).X(),vec.at(i).Y(),vec.at(i).Z());
            if(!list.contains(p))
            {
                list.push_back(p);
            }
        }
    }

    vec.clear();
    for(int i=0;i<list.size();++i)
    {
        vec.push_back(gp_Pnt(list.at(i).x(),list.at(i).y(),list.at(i).z()));
    }
}
static void gppnttoqvector3d(const gp_Pnt &p,QVector3D &vec)
{
    vec.setX(p.X());
    vec.setY(p.Y());
    vec.setZ(p.Z());
}
static int getminangle(const gp_Pnt &op,const gp_Vec &vx,const QVector<gp_Pnt> &array)
{
    float a=100.0;
    int num=0;
    for(int i=0;i<array.size();i++)
    {
        gp_Pnt pd=array.at(i);
        if(isequal(op,pd))
            continue;
        gp_Vec vec=gp_Vec(op,pd);
        float angle=vec.Angle(vx);
        if(angle<a)
        {
            a=angle;
            num=i;
        }
    }
    return num;
}
static void sortgppntlist(const gp_Pnt &org,const gp_Vec vx,QVector<gp_Pnt> &point)
{
    gp_Pnt op=getnearstpoint(org,point);
    QVector<gp_Pnt> pt;
    pt.push_back(op);
    for(int i=0;i<point.size();++i)
    {
        gp_Pnt pd=point.at(i);
        if(isequal(op,pd))
        {
            continue;
        }
        else
        {
            int num = getminangle(op,vx,point);
            pt.push_back(point.at(num));
            point.remove(num);
            i--;
        }
    }
    point.clear();
    for(int j=0;j<pt.size();++j)
    {
        point.push_back(pt.at(j));
    }
}
static QVector3D getRPYfromVec(const QVector3D &vec,const QVector3D &uec)//由法线求RPY角，其中vec为法线，uec为切线
{

    gp_Vec zdir=gp_Vec(vec.x(),vec.z(),vec.z());
    gp_Dir zdir2=zdir.Normalized();
    gp_Vec xdir=gp_Vec(uec.x(),uec.y(),uec.z()).Normalized();
    if(zdir.Angle(xdir)< 1e-5)
        return QVector3D(180,0,0);
    gp_Vec ydir=zdir2.Crossed(xdir);

//    Eigen::AngleAxisd R_vec(1,Eigen::Vector3d(vec.x(),vec.y(),vec.z()));
//    Eigen::Matrix3d rmat=Eigen::Matrix3d::Identity();
//    rmat=R_vec.toRotationMatrix();
//    Eigen::Vector3d euler_angle=rmat.eulerAngles(2,1,0);
    Eigen::Matrix3d R_mat=Eigen::Matrix3d::Identity();
    R_mat(0,0)=xdir.X();
    R_mat(1,0)=xdir.Y();
    R_mat(2,0)=xdir.Z();
    R_mat(0,1)=ydir.X();
    R_mat(1,1)=ydir.Y();
    R_mat(2,1)=ydir.Z();
    R_mat(0,2)=zdir2.X();
    R_mat(1,2)=zdir2.Y();
    R_mat(2,2)=zdir2.Z();
    Eigen::Vector3d euler_angle=R_mat.eulerAngles(2,1,0);
    QVector3D euler;
    euler.setX(euler_angle.x()*180/M_PI);
    euler.setY(euler_angle.y()*180/M_PI);
    euler.setZ(euler_angle.z()*180/M_PI);
    return euler;

}
static QVector3D eulerformpoint(gp_Pnt p0,gp_Pnt p1,gp_Pnt p2)//三个点求欧拉角
{
    gp_Vec va=gp_Vec(p1,p0).Normalized();
    gp_Vec vb=gp_Vec(p2,p0).Normalized();
    gp_Vec vz=vb.Crossed(va).Normalized();
    gp_Vec vx=va;
    gp_Vec vy=vz.Crossed(vx).Normalized();
    float r[3][3];
    float rx,ry,rz;
    r[0][0]=vx.X();r[0][1]=vy.X();r[0][2]=vz.X();
    r[1][0]=vx.Y();r[1][1]=vy.Y();r[1][2]=vz.Y();
    r[2][0]=vx.Z();r[2][1]=vy.Z();r[2][2]=vz.Z();

//    qDebug()<<vx.X()<<vy.X()<<vz.X();
//    qDebug()<<vx.Y()<<vy.Y()<<vz.Y();
//    qDebug()<<vx.Z()<<vy.Z()<<vz.Z();

    float sy=sqrt(r[2][1]*r[2][1]+r[2][2]*r[2][2]);
    if(sy>=1e-6)
    {
        rx=atan2(r[2][1],r[2][2]);
        ry=atan2(-r[2][0],sy);
        rz=atan2(r[1][0],r[0][0]);
    }else
    {
        rx=atan2(-r[1][2],r[1][1]);
        ry=atan2(-r[2][0],sy);
        rz=0;
    }
    return QVector3D(rx*180/M_PI,ry*180/M_PI,rz*180/M_PI);
}
static QVector3D eulerformzox(gp_Pnt pz,gp_Pnt po,gp_Pnt px)
{
    gp_Vec vx=gp_Vec(px,po).Normalized();
    gp_Vec vz=gp_Vec(pz,po).Normalized();
    gp_Vec vy=vz.Crossed(vx).Normalized();
//    QVector3D norm=eulerformpoint(po,px,gp_Pnt(vy.X(),vy.Y(),vy.Z()));
//    return norm;
    //gp_Vec vx=vy.Crossed(vz).Normalized();
    float r[3][3];
    float rx,ry,rz;
    r[0][0]=vx.X();r[0][1]=vy.X();r[0][2]=vz.X();
    r[1][0]=vx.Y();r[1][1]=vy.Y();r[1][2]=vz.Y();
    r[2][0]=vx.Z();r[2][1]=vy.Z();r[2][2]=vz.Z();

    qDebug()<<vx.X()<<vy.X()<<vz.X();
    qDebug()<<vx.Y()<<vy.Y()<<vz.Y();
    qDebug()<<vx.Z()<<vy.Z()<<vz.Z();
    float sy=sqrt(r[2][1]*r[2][1]+r[2][2]*r[2][2]);
    if(sy>=1e-6)
    {
        rx=atan2(r[2][1],r[2][2]);
        ry=atan2(-r[2][0],sy);
        rz=atan2(r[1][0],r[0][0]);
    }else
    {
        rx=atan2(-r[1][2],r[1][1]);
        ry=atan2(-r[2][0],sy);
        rz=0;
    }
    return QVector3D(rx*180/M_PI,ry*180/M_PI,rz*180/M_PI);
}
#endif // OCC_H
