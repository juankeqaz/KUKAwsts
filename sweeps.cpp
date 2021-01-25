#include "sweeps.h"
#include <QDebug>

Sweeps::Sweeps(QVector<subwire> wire):sweeps_line_point(wire)
{

}

void Sweeps::make_sweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz)
{
    if(sweeps_line_point.size()<=0) return;
    TopoDS_Wire aWire=sweepwire();

    _sweeps = new BRepPrimAPI_MakePrism(aWire,gp_Vec(wx,wy,wz),false,false);
}

void Sweeps::make_sweeps(const gp_Dir &dir, const Standard_Real &length)
{
    if(sweeps_line_point.size()<=0) return;
    gp_Pnt pstart=sweeps_line_point.first().vertex.first();
    gp_Vec vec=dir;
    vec=vec*length;
    gp_Vec vf=gp_Vec(pstart.X(),pstart.Y(),pstart.Z());
    gp_Vec pend=vf.Added(vec);
    pend=gp_Vec(pstart,gp_Pnt(pend.X(),pend.Y(),pend.Z()));
    TopoDS_Wire aWire=sweepwire();
    //qDebug()<<pend.X()<<pend.Y()<<pend.Z();
    _sweeps = new BRepPrimAPI_MakePrism(aWire,pend,false,false);
}

TopoDS_Wire Sweeps::sweepwire()
{
    QVector<TopoDS_Edge> edgevector;
    for(int i=0;i<sweeps_line_point.size();i++)
    {
        QString type=sweeps_line_point.at(i).type;
        QVector<gp_Pnt> pnt=sweeps_line_point.at(i).vertex;
        TopoDS_Edge edge;
        if(type=="line"&& pnt.size()==2)
        {
            gp_Pnt aPnt1,aPnt2;
            aPnt1=pnt.at(0);
            aPnt2=pnt.at(1);
            Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
            edgevector.push_back(BRepBuilderAPI_MakeEdge(aSegment1));


        }else if(type=="arc"&& pnt.size()==3)
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
    return  aWire;
}

bool SUBWIRE::getvertex(gp_Pnt point)
{
    if(type=="line")
    {
        int p=vertex.size();
        if(p<2)
        {
            vertex.push_back(point);
            return  true;
        }
    }
    if(type=="arc")
    {
        int p=vertex.size();
        if(p<3)
        {
            vertex.push_back(point);
            return  true;
        }
    }
    return false;
}
