#ifndef DETECSTING_H
#define DETECSTING_H

#include <QDialog>
#include <QMutex>
#include "occview.h"
#include "quiwidget.h"
#include <QToolButton>
#include "globalvar.h"
#include "myview.h"
#include "toolcoord.h"
#include "box.h"

namespace Ui {
class DetecSting;
}

class DetecSting : public QDialog
{
    Q_OBJECT


public:
    explicit DetecSting(QWidget *parent = nullptr);
    ~DetecSting();
    static DetecSting *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new DetecSting;
            }
        }

        return self;
    }
signals:
    void buildtoolcoord();
    void sendpathpoint(QVector<Robot_Path> path);
private slots:
    void on_pushButton_tool_axis_clicked();

    void on_pushButton_tool_trans_clicked();

    void on_pushButton_tool_rot_clicked();

    void on_pushButton_tool_plan_clicked();

    void showshape(QStringList &list);
    void getshapepara(TopoDS_Shape &list);

    void on_toolButton_loop_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_close_clicked();

    void on_radioButton_up_surface_clicked();

    void radioButton_down_surface_clicked();

    void on_radioButton_right_surface_clicked();

    void on_radioButton_left_surface_clicked();

    void on_radioButton_front_surface_clicked();

    void on_radioButton_back_surface_clicked();

    void on_radioButton_up_edge_clicked();

    void on_radioButton_down_edge_clicked();

    void on_radioButton_sweep_edge_clicked();

    void on_radioButton_input_cheoice_clicked();

private:
    void initForms();
    void showshapes(const float ox,const float oy,const float oz,const float x,const float y,const float z,const float px,const float py,const float pz);
    void printpathpoint();
    float getlength(const QVector<gp_Pnt> &array);
    Robot_Path setpathformvec(QVector<gp_Pnt> array, const QVector3D qnorm, const QString str);
    QVector3D rpyformnormal(gp_Vec norm,gp_Vec px);
    bool isrotationmatrix(QMatrix4x4 &R);
    void setfacepathformvec(const QVector3D qnorm);
    TopoDS_Face getshapeface(TopoDS_Shape &_shape, int index);
    TopoDS_Wire getwirefromface(TopoDS_Face &face);
    QVector<gp_Pnt> pathpointfromface(TopoDS_Face face,int step=5);
    QVector<gp_Pnt> pathpointfromwire(TopoDS_Wire wire);
    void getalinepath(gp_Pnt start, gp_Pnt end, QVector3D qnorm, bool isneedscan);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::DetecSting *ui;
    static DetecSting* self;
    //OccView *myview;
    TopoDS_Shape currentshap;
    TopoDS_Shape workshape;
    bool max;
    QRect location;
    bool ispress=false;
    QPoint startpos;
    myview *myoccview;
    ToolCoord *basecoord=NULL;
    ToolCoord *_toolcd=NULL;
    QVector<gp_Pnt> pathpoint;
    //Robot_Path path;
    QVector<Robot_Path> pathpackge;
    const QVector3D _up_face_norm={180,0,180};
    const QVector3D _down_face_norm={0,0,-180};

};

#endif // DETECSTING_H
