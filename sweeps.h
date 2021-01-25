#ifndef SWEEPS_H
#define SWEEPS_H
#include "occ.h"
#include <QVector>
#include <QMap>

typedef struct SUBWIRE
{
    QString type;
    QVector<gp_Pnt> vertex;
    bool getvertex(gp_Pnt point);
}subwire;



class Sweeps
{
public:
    Sweeps(QVector<subwire> wire);
    void make_sweeps(Standard_Real wx, Standard_Real wy, Standard_Real wz);
    void make_sweeps(const gp_Dir &dir,const Standard_Real &length);

    TopoDS_Wire sweepwire();
    TopoDS_Shape getshape()
    {
        return  _sweeps->Shape();
    }
private:
    BRepPrimAPI_MakePrism *_sweeps=NULL;
    QVector<subwire> sweeps_line_point;
};

#endif // SWEEPS_H
