#ifndef RUNPAGE_H
#define RUNPAGE_H

#include <QDialog>
//#include "occview.h"
//#include "myview.h"
#include <QHBoxLayout>
#include <QTimer>
#include <stlmodels.h>
//#include <stlmodule.h>
#include "modelview.h"
//#include "QQuickWidget"
#include "kinematics.h"
//#include "toolcoord.h"
#include "LBRIII_trajectory_planning/LBRIII_kinematics.h"

//#include "KR6ArmKinematics.h"
//using namespace kr6_arm_kinematics;

#include "globalvar.h"
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>


namespace Ui {
class Runpage;
}

class Runpage : public QDialog
{
    Q_OBJECT

public:
    explicit Runpage(QWidget *parent = nullptr);
    ~Runpage();
    void testik();
signals:
    void showmodels();
    void robotposition(const QVector<float> &currentjoint,const int &flag);
    void startrunsignal();
private slots:
    void initForm();
    void initTreeWidget();
    void initMenu();
    void doMenu();
    void on_pushButton_min_clicked();
    void on_pushButton_max_clicked();
    void on_pushButton_close_clicked();
    void init3d();
    void on_toolButton_start_clicked();
    void on_toolButton_sigle_clicked();

    void on_checkBox_showmodel_clicked();

    void on_toolButton_start_triggered(QAction *arg1);

    void on_toolButton_stop_clicked();

    void on_toolButton_suspend_clicked();

public slots:
    void showrobot();
    void testmodel();
    void toolcoordchanged();
    void getpathplant(QVector<Robot_Path> path);
    void gettreePros();
    void singuarpoint();


private:
    void updatajointdata(QVector<float> dts);
    void checkprogramlist();
    QVector3D subcoordpoint(const QVector3D &point);
    void printpath();
    void jointvecformpath(const QVector<float> currentjoint, Robot_Path &path);

private:
    Ui::Runpage *ui;
    bool max;
    QRect location;
    //OccView *myview;
    //myview *myview;
    QTimer *timer;    
    //STLModule *model;
    //stlmodels *model;
    modelview *model;
    //QQuickWidget *view;
    Kinematics *kinematics;
    //ToolCoord *toolcd;
    QVector<Robot_Path> pathvec;
    int  runflag=0;
    bool isloop;
    QVector<float> currentposition;


};

#endif // RUNPAGE_H
