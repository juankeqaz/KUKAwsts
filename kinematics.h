#ifndef KINEMATICS_H
#define KINEMATICS_H
#include <QVector>
#include <QMatrix4x4>
#include <QObject>
#include <QMatrix3x3>
#include "KinematicConstants.h"
#include <QTimer>
#include <matrix/Matrix.hpp>

//const QVector<double> maxJointAngles = {2.96705972839, 0.78539816339, 2.72271363311, 3.22885911619, 2.09439510239, 6.10865238198};
//const QVector<double> minJointAngles = {-2.96705972839, -3.31612557879, -2.09439510239, -3.22885911619, -2.09439510239, -6.10865238198};
const float offset=20.0;
//const int DOF=6;
#include "globalvar.h"

#define JOINT_NUM 6
#define PI 3.1415926
#define LINE_V 500//单位毫米，用米时需换算
#define LINE_A 10000//单位毫米，用米时需换算
#define ARC_V 2//单位米，用毫米时需换算
#define ARC_A 20//单位米，用毫米时需换算






class Kinematics:public QObject
{
    Q_OBJECT
public:
    Kinematics();
    ~Kinematics();
    float polynomialvalue(const unsigned int order, const float *coefficients,const float t)
    {
        float value = coefficients[0];
        for(unsigned int i=0;i<order;++i)
        {
            value = value*t+coefficients[i+1];
        }
        return value;
    }

    bool isgreaterthanzero(const float value)
    {
        return value>0?true:false;
    }
    void V_mul_S(const float* V, const float s, float* des,const unsigned int dimension)
    {
        for(unsigned int i=0;i< dimension;++i)
        {
            des[i] = V[i] * s;
        }
    }
    void V_add_V(const float* V1, const float* V2, float* des, const unsigned int dimension)
    {
        for(unsigned int i=0;i<dimension;++i)
        {
            des[i] = V1[i] + V2[i];
        }
    }
    void R_from_axis_angle(const float axis[3], const float angle, float R[3][3])
    {
        float sinq = sin(angle), cosq = cos(angle);
        float one_cosq = 1-cosq;
        float kx = axis[0], ky = axis[1], kz = axis[2];
        float a = kx*one_cosq, b = ky*one_cosq, c = kz*one_cosq,
                d = kx*sinq, e = ky*sinq, f = kz*sinq;
        R[0][0] = kx*a + cosq;
        R[1][0] = ky*a + f;
        R[2][0] = kz*a - e;
        R[0][1] = kx*b - f;
        R[1][1] = ky*b + cosq;
        R[2][1] = kz*b + d;
        R[0][2] = kx*c + e;
        R[1][2] = ky*c - d;
        R[2][2] = kz*c + cosq;
    }
    void R_mul_R(const float R1[3][3], const float R2[3][3], float des[3][3])
    {

        for(unsigned int i=0;i<3;++i)
        {
            for(unsigned int j=0;j<3;++j)
            {
                float sum = 0;
                for(unsigned int k=0;k<3;++k)
                {
                    sum += R1[i][k] * R2[k][j];
                }
                des[i][j] = sum;
            }
        }
    }
    void T_to_Pose(const Transformation *T, SPose *pose)
    {
        pose->x = T->translation[0];
        pose->y = T->translation[1];
        pose->z = T->translation[2];
        R_to_EulerAngle(T->rotation,pose);
    }

    void R_to_EulerAngle(const float R[3][3], SPose *pose)
    {
        float rx,ry,rz;
        float sy = sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]);
        if(sy>=1e-6)
        {
            rx = atan2(R[2][1],R[2][2]);
            ry = atan2(-R[2][0],sy);
            rz = atan2(R[1][0],R[0][0]);
        }
        else
        {
            rx = atan2(-R[1][2],R[1][1]);
            ry = atan2(-R[2][0],sy);
            rz = 0;
        }
        pose->alpha = rx;
        pose->beta = ry;
        pose->gamma = rz;
    }
    void sincosround(float *value)
    {
        if(fabs(*value-1)<1e-5)
        {
            *value = 1;
            //printf("close 1\n");
            return;
        }
        if(fabs(*value+1)<1e-5)
        {
            *value = -1;
            //printf("close -1\n");
            return;
        }
        if(fabs(*value)<1e-5)
        {
            *value = 0;
            //printf("close 0\n");
            return;
        }
    }
    void V_assign(const float* src, float* des,const unsigned int dimension)
    {
        for(unsigned int i=0;i<dimension;++i)
        {
            des[i] = src[i];
        }
    }
    void R_assign(const float src[3][3], float des[3][3])
    {
        for(unsigned int i=0;i<3;++i)
        {
            for(unsigned int j=0;j<3;++j)
            {
                des[i][j] = src[i][j];
            }
        }
    }

    void T_to_R(const Transformation* T, float des[3][3])
    {
        R_assign(T->rotation,des);
    }

    void T_to_P(const Transformation* T, float des[3])
    {
        V_assign(T->translation,des,3);
    }
    void V_sub_V(const float* V1, const float* V2, float* des,const unsigned int dimension)
    {
        for(unsigned int i=0;i<dimension;++i)
        {
            des[i] = V1[i] - V2[i];
        }
    }
    float V_norm(const float* V,const unsigned int dimension)
    {
        float sum_square = 0;
        for(unsigned int i=0;i<dimension;++i)
        {
            sum_square += V[i] * V[i];
        }
        return sqrt(sum_square);
    }


    void V_dir(const float* V, float* des, const unsigned int dimension)
    {
        V_mul_S(V,1/V_norm(V,dimension),des, dimension);
    }

    void R_inv(const float src[3][3], float des[3][3])
    {
        for(unsigned int i=0;i<3;++i)
        {
            for(unsigned int j=0;j<3;++j)
            {
                des[i][j] = src[j][i];
            }
        }
    }
    void R_to_AA(const float R[3][3], AxisAngle* AA)
    {
        float cosq = 0.5*(R[0][0] + R[1][1] + R[2][2] - 1);
        if(cosq > 1)
            cosq = 1;
        if(cosq < -1)
            cosq = -1;
        AA->angle = acos(cosq);
        //printf("r to aa axis:\n");
        //printf("%lf,%lf,%lf\n",AA->axis[0],AA->axis[1],AA->axis[2]);
        if(fabs(AA->angle)<1e-5) // angle close to zero
        {
            //printf("angle close to 0\n");
            AA->angle = 0;
            for(unsigned int i=0;i<3;++i)
            {
                AA->axis[i] = 0;
            }
            return;
        }
        else if(fabs(AA->angle-M_PI)<1e-5) // angle close to 180 degree
        {
            //printf("angle close to pi\n");
            AA->angle = M_PI;
            float magnitude[3];
            for(unsigned int i=0;i<3;++i)
            {
                magnitude[i] = sqrt(0.5*(R[i][i]+1));
            }
            // determine signs
            unsigned int maxind = 0;
            if(magnitude[1] > magnitude[0])
                maxind = 1;
            if(magnitude[2] > magnitude[1])
                maxind = 2;
            if(maxind == 0)
            {
                float v1v2 = R[0][1] + R[1][0];
                float v1v3 = R[0][2] + R[2][0];
                if(v1v2 < 0.)
                    magnitude[1] *= -1;
                if(v1v3 < 0.)
                    magnitude[2] *= -1;
            }
            else if(maxind == 1)
            {
                float v1v2 = R[0][1] + R[1][0];
                float v2v3 = R[1][2] + R[2][1];
                if(v1v2 < 0.)
                    magnitude[0] *= -1;
                if(v2v3 < 0.)
                    magnitude[2] *= -1;
            }
            else if(maxind == 2){
                float v1v3 = R[0][2] + R[2][0];
                float v2v3 = R[1][2] + R[2][1];
                if(v1v3 < 0.)
                    magnitude[0] *= -1;
                if(v2v3 < 0.)
                    magnitude[1] *= -1;
            }
            V_assign(magnitude,AA->axis,3);
            return;
        }

        // general case
        float axis[3];
        axis[0] = R[2][1] - R[1][2];
        axis[1] = R[0][2] - R[2][0];
        axis[2] = R[1][0] - R[0][1];
        //printf("no normalized axis:\n");
        //V_print(axis,3);
        V_dir(axis,AA->axis,3);
    }
    void T_from_RP(const float R[3][3], const float P[3], Transformation* des)
    {
        R_assign(R,des->rotation);
        V_assign(P,des->translation,3);
    }
    void V_print(const float* V,const unsigned int dimension)
    {
        printf("[ ");
        for(unsigned int i=0;i<dimension;++i)
        {
            printf("%lf ",V[i]);
        }
        printf("]T\n");
    }
    void Pose_to_T(const SPose *pose, Transformation *T)
    {
        float km=pose->alpha*M_PI/180;
        float bt=pose->beta*M_PI/180;
        float af=pose->gamma*M_PI/180;

        float nx=cos(af)*cos(bt);float ox=(cos(af)*sin(bt)*sin(km))-(sin(af)*cos(km));float ax=(cos(af)*sin(bt)*cos(km))+(sin(af)*sin(km));
        float ny=sin(af)*cos(bt);float oy=(sin(af)*sin(bt)*sin(km))+(cos(af)*cos(km));float ay=(sin(af)*sin(bt)*cos(km))-(cos(af)*sin(km));
        float nz=-sin(bt);float oz=cos(bt)*sin(km);float az=cos(bt)*cos(km);

//        qDebug()<<"******************************************";
//        qDebug()<<nx<<" "<<ox<<" "<<ax;
//        qDebug()<<ny<<" "<<oy<<" "<<ay;
//        qDebug()<<nz<<" "<<oz<<" "<<az;
//        qDebug()<<"******************************************";

        T->rotation[0][0] = nx; T->rotation[0][1] = ox; T->rotation[0][2] = ax;
        T->rotation[1][0] = ny; T->rotation[1][1] = oy; T->rotation[1][2] = ay;
        T->rotation[2][0] = nz; T->rotation[2][1] = oz; T->rotation[2][2] = az;
        T->translation[0] = pose->x;
        T->translation[1] = pose->y;
        T->translation[2] = pose->z;
    }


public slots:
    void fkcall(QVector<float> jointvalue, QVector<float> &mrpy);
    void fkcall2(const QVector<float> jointvalue,QVector<float> &rpy);
    bool ikcals(const QVector<float> fpos,const QVector<float> pos,QVector<float> &jointvalue);
    void LBR_fk(const float Q[6], Transformation *T);
    //void Ttopos();
    void LBR_ik(const Transformation* T, const QVector<float> currentq, unsigned int*solutionnum, float inversesolutions[8][6]);
    bool closestq(const float inversesolutions[8][6], const unsigned int solutionnum, const float currentq[6], float closestsolution[6]);
    bool closestq(const float inversesolutions[8][6], const unsigned int solutionnum, const QVector<float> currentq, QVector<float> &solution);
    void getjointparam(QVector<float> paramet);
    void getjointparam(QVector<QVector<float> > paramet);
    void getrobotpath(QVector<Robot_Path> path);


    void movelq(QVector<float> end);
    void stoprun();
    QVector<float> getcurrentposition(){
        return parame;
    }
    int getflags()
    {
        int p=0;
        if(isatpath)
            p+=10;
        if(bescan)
            p+=1;
        return  p;
    }
    void setspeedratevalue(int value);
private:
    void getrpy(const QMatrix4x4 mt,QVector<float> &rpy);
    void getmatrix(const QVector<float> pos,QMatrix4x4 &mt);
    QMatrix4x4 getTjoint(double angle, double ai, double aph, double di);
    bool ikjoint1(const QMatrix4x4 mts,float &joint1);
    bool ikjoint2(const QVector3D p04, const float joint1, float &joint2, float &joint21);
    bool ikjoint3(const float joint1, const QMatrix4x4 mts, float &joint3, float &joint31);
    bool ikjoint4(const float joint1, const float joint2, const float joint3, const QMatrix4x4 mts, float &joint4);
    bool ikjoint5(const float joint1,const float joint2,const float joint3,const QVector3D N60,float &joint5,float &joint51);
    bool ikjoint6(const float joint1, const float joint2, const float joint3, const QMatrix4x4 mts, float &joint6);
    double iszero(double value);

    bool islinefeasible(const float Q0[6], const float Q1[6], const float v, const float a);
    void linepath(const float p0[3], const float p1[3], LineGeometry *line);
    void strajectory(const float l,const float v,const float a,STrajectoryParameter *trajp);
    void orientationpath(const float R0[3][3], const float R1[3][3],OrientationGeometry *orientation);
    void fiveordertrajectory(const float p0, const float v0, const float a0,const float p1, const float v1, const float a1,const float T,float coefficient[6]);

    void calculat_line_Geometry(const float Q0[], const float Q1[], LineGeometry *line, OrientationGeometry *orientation);
    void calculat_line_Trajectory(const LineGeometry *line, const OrientationGeometry *orientation, const float v, const float a, STrajectoryParameter *linetrajp, float orientationtrajp[6]);

    bool isatruange(const float solution[6]);

    bool runonepath(QVector<QVector<float> > &wp);
    void speedcontrol(const QVector<float> &pa);
    float distansabout2point(const QVector<float> &p1,const QVector<float> &p2);

private slots:
    void sendVirtualJointPosition();//轨迹规划
    void atendpoint();//判断是否在轨迹末端
    float cal_position(const FiveOrderPolyCoefficients *coefficients, const float t);//计算五次多项式下一位置
    void getspeedratevalue(int value);//速度控制百分比
    void startrun();//开始



    void getcommonmodeposition();
    void cyclerun();//五次多项式参数计算；
    void cal_timepoints(float *timepoints, const Waypoint *waypoints, const unsigned int waypointsnum, const float *velocitylimit);//时间点计算
    float cal_timeduraion_on_one_interval(const Waypoint *from, const Waypoint *to, const float *velocitylimit);
    void cal_coefficients_for_all_joints_all_intervals(FiveOrderPolyCoefficients **allcoefficients,
                                                       const Waypoint *waypoints,
                                                       const unsigned int waypointsnum,
                                                       const float *timepoints,
                                                       const BoundaryCondition* start,
                                                       const BoundaryCondition* end);//关节参数计算
    void cal_coefficients_for_all_joints_one_interval(FiveOrderPolyCoefficients *jointscoefficients,
                                                      const float timeduration,
                                                      const Waypoint *p0,const Waypoint *p1,
                                                      const Velocity *v0,const Velocity *v1,
                                                      const Acceleration *a0, const Acceleration *a1);
    bool cal_coefficients_for_one_joint_one_interval(FiveOrderPolyCoefficients * jointcoefficients, const float t,
                                                     const float p0, const float p1,
                                                     const float v0, const float v1,
                                                     const float a0, const float a1);
    void cal_velocity_and_acceleration(const Waypoint *waypoints,
                                       const unsigned int waypointsnum,
                                       const float *timepoints,
                                       const BoundaryCondition *start,
                                       const BoundaryCondition *end,
                                       const unsigned int index,
                                       float *timeduration01, float *timeduration12, float *timeduration23,
                                       Velocity *v0, Velocity *v1, Velocity *v2, Velocity *v01, Velocity *v12, Velocity *v23,
                                       Acceleration *a0, Acceleration *a1, Acceleration *a01, Acceleration *a12);//计算速度和加速度
    float cal_velocity_at_mid_point_one_joint(const float v01, const float v12);
    float cal_acceleration_at_mid_point_one_joint(const float a01, const float a12);


    void calculat_next_lq_point(const LineGeometry *line,const OrientationGeometry *orientation,const STrajectoryParameter *linetrajp,const float orientationtrajp[6],\
    const float time, const float currentq[6],float result[6]);
    void lineinterpolation(const LineGeometry* line,
                           const STrajectoryParameter* trajp,
                           const float t,
                           float p[3]);
    float strajectoryscale(const STrajectoryParameter* trajp,
                           const float t);

    void caculate_next_arc_point(const CircleArcGeometry *circlearc, const OrientationGeometry *orientation, const STrajectoryParameter *circlearctrajp, const float orientationtrajp[], const float time, const float currentq[], float result[]);
    void orientationinterpolation(const OrientationGeometry *orientation, const float coefficient[6],const float t,float r[3][3]);
    float fiveordertrajectoryscale(const float coefficient[6], const float t);
    void circlearcinterpolation(const CircleArcGeometry *circlearc,const STrajectoryParameter *trajp,const float t,float p[3]);


protected:
    void DH2T(const float theta, const float d,const float a,const float alpha, QMatrix4x4 &matrix);
    unsigned char phase_shift_eq(float sin_cos_pairs[2][2], const float a, const float b, const float c);

    float caldistance(const float Q1[6], const float Q2[6]);
    void detjacobianinfo(const float Q[6],bool* isclosetozero,int* sign);
signals:
    void currentcommandsignal();
    void dataforsimulate(QVector<float> para,int funcord);
    void singularpointinline();


private:
    QVector<float> DH_parame;
    float toollength;
    float a1,a2,a3,d1,d4,d6;



    QTimer *simulationTimer;
    QVector<float> parame;
    float currenttimepoint;
    float speedrate;
    float time_step;
    float tmax;
    bool isatendpoint;
    unsigned int waypointsnum=0;
    float* timepoints;
    FiveOrderPolyCoefficients** allcoefficients;
    Waypoint* waypoints;

    std::vector<std::vector<float> > waypoint;
    int movement_mode;

    LineGeometry *line;
    OrientationGeometry *orientation;
    STrajectoryParameter *linetrajp;
    float orientationtrajp[6];
    float startq[6];
    QVector<GeometryMetaData> geometrymatadataset;

    CircleArcGeometry *circlearc;
    OrientationGeometry* arc_orientation;
    STrajectoryParameter* circlearctrajp;

    QVector<float> maxjointangle={170,45,156,185,120,350};
    QVector<float> minjointangle={-170,-190,-120,-185,-120,-350};


    unsigned int pathflag=0;
    QVector<Robot_Path> currentpath;
    float movedir=1000;
    bool isatpath=false;
    bool bescan=false;
    unsigned int movelqindex=0;





};

#endif // KINEMATICS_H
