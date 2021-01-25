#ifndef STLDATA_H
#define STLDATA_H
#include "kinematics_interface.h"
#include <QTimer>
#include <QReadWriteLock>

//extern status* robstatus;
#include "LBRIII_trajectory_planning/LBRIII_kinematics.h"

extern "C"{
#include "interpolationfunction.h"
#include "LBRIII_trajectory_planning/trajectory_planning.h"
#include "LBRIII_trajectory_planning/mathtype.h"
}
#ifndef RAD2DEG
#define RAD2DEG (180.0/M_PI)
#define DEG2RAD (M_PI/180.0)
#endif
#define LINE_V 0.5
#define LINE_A 10
#define ARC_V 2
#define ARC_A 20


enum MOVEMENT_MODE{
    COMMON_MODE,
    LINE_MODE,
    ARC_MODE
};

enum GeometryShape{
    LINE,
    CIRCLE
};

struct GeometryMetaData
{
    void* shapegeometry;
    GeometryShape shape;

};

class stldata:public QObject
{
    Q_OBJECT
public:
    explicit stldata(QObject *parent=0);
    ~stldata();
signals:
    void dataforsimulate(QVector<float> data);
    void sendwaypointsignal(QString waypoint);
    void currentcommandsignal();
    void getjointparamfilishsignal();
    void initparamfinished();
    void calcujointparam(QVector<float> curvec,QVector<float> targetvec);
    void movepnoresult();
    void fkcalsignal(const QVector<float> &vector);
    void resultoffkcacul(const QVector<float> &vector);
    void ikcaculresulut(const QVector<float> &result);
    void positionnotreachable();
    void sendsimulateposition(const QVector<QVector<float> > crt);
    void setiostatusignal(int port,int st);
public slots:
    void getmoveflag(int flag);
    void getspeedmodelflag(int flag);
    void getjointparam(QVector<float> paramet);
    void sendVirtualJointPosition();
    void getwaypoint();
    void getspeedratevalue(int value);
    void resultofcacul(bool isnon,QVector<float> result);
    void resultoffkcaculslot(const QVector<float> &vector);
    void positionmodel();
    void speedmodel();
    void actualrun(QVector<float> pos);
    void vectortoparam();
    bool isactualrun();
    void movelq(QVector<float> end);
    void movecq(QVector<float> pass,QVector<float> end,int flag);
    void trajectory_display();
    void changerobstatus(int p);
    void getposparaminfor(QVector<float> paramets);
    void jointrotateslot(int joint,float value);
    void speedrotateslot(int joint,float speed,int timelength);
    void jointmovtoangleslot(int joint, float value);
    void gotohomeslot();
    void homeposition();
    void getjointparams(std::vector<std::vector<float> > wayline);
    void stoprun();
    void startrun();
    void getcurpostions();
    void getspacecurpostions();
    void changerobparamet(QStringList paramelist);
    void virtualrobcommand(QStringList paramlist, int p);
    void timeoutslots();
    void atendpoint();
    void movecp(QVector<float> pass,QVector<float> end,int flag);

private:
    void translate();
    void cyclerun();
    float timeDistance(const std::vector<float> &from, const std::vector<float> &to);
    void calculateTimedPath(const std::vector<std::vector<float> > &path, std::vector<float> &time);
    void currentposition(QVector<float> paramet);
    void runteachwaypoint();
    float checkdata(float data)
    {
        if(data<10e-4)
            return 0;
        else return data;
    }
    void getcommonmodeposition();
    QVector<float> checkjointvalue(QVector<float> para);
    void cleargeometrymetadataset();
    void sendsimulatedate();
public:
       float speedrate;
private:
    std::vector<int> jointVariables;
    QVector<float> virtualJointVariables;
    std::vector<std::vector<float> > virtual_interpolated_points;
    std::vector<std::vector<float> > jointparavariables;
    std::vector<std::vector<float> > waypoint;
    std::vector<std::vector<float> > teachpoint;
    QVector<float> curspace;
    QVector<float> jointVelocityLimits;
    QVector<float> parame;
    QVector<float> curparame;
    bool is_need_calculat_coefficient;
    int stepnum;
    bool isstep;
    float time_step;
    QTimer* simulationTimer;
    QTimer* positiontmodelTimer;
    QTimer* speedmodelTimer;

    unsigned int currentVirtualJointPositionIndex;
    //FiveOrderInter FiveCal;


    FiveOrderPolyCoefficients** allcoefficients;
    Waypoint* waypoints;
    float* timepoints;
    unsigned int waypointsnum;
    float tmax;
    float currenttimepoint;
    bool stopflag;

    KInematics_Interface *cacul;
    //QThread *cacuthread;

    int curpoint;
    bool isteachway;
    float rotstep;
    int currjoint;
    bool isjointrot;

    int moveflag;//模型进行位置模式标识
    int speedmodelflag;//模型进行速度模式标识
    QVector<float> spacevec;
    bool isspeedmodel;
    QTimer *lbrupdatetimer;
    QReadWriteLock lock;
    int movement_mode;
    VTYPE startq[6];

    LineGeometry *line;
    OrientationGeometry *orientation;
    STrajectoryParameter *linetrajp;
    VTYPE orientationtrajp[6];

    CircleArcGeometry *circlearc;
    OrientationGeometry* arc_orientation;
    STrajectoryParameter* circlearctrajp;

    bool isatendpoint;
    bool isdrwn;
    QVector<QVector<float> > movewayline;
    QVector<GeometryMetaData> geometrymatadataset;
    bool issimulate;

    QTimer *waittime;
    int funflag;
    bool runfunc;
};

#endif // STLDATA_H
