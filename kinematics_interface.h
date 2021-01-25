#ifndef KINEMATICS_INTERFACE_H
#define KINEMATICS_INTERFACE_H

#include <QObject>
#include <QString>
#include <QVector>
//#include "ClosedFormLBRSolver.h"
//#include "GerneralLBRSolver.h"
//#include "kinematics.h"

#define RAD2DEG (180.0/M_PI)
#define DEG2RAD (M_PI/180.0)

//const static QVector<float> paramete={0.119,0.49969,0.39602,-0.14257,0.11243,0.058};//三代机械臂
//const static QVector<float> paramete={0.119,0.38,0.33,-0.143,0.111,0.058};//四代机械臂

class KInematics_Interface : public QObject
{
    Q_OBJECT
public:
    explicit KInematics_Interface();
    ~KInematics_Interface();
    //void updataparamete(QStringList paramelist);

private:
//    ClosedFormLBRSolver* kinematics_ptr;
//    GerneralLBRSolver* GerneralLbrSolver_Ptr;
//    rw::models::Device::Ptr _deviceRobot;
//    rw::kinematics::State _state;
//    float x, y, z, af, bt, km;
//    float round_d(float n, int d);
    QString q2str(const std::vector<float>& Q);
    //std::vector<float> Q;
    //std::vector<float> paramet;
    //QVector<float> parameters;

Q_SIGNALS:
    /**
     * @brief FKResults
     * @param ToolPose[0]:px,ToolPose[1]:py,ToolPose[2]:pz
     * ToolPose[3]:Rotate Z,,ToolPose[4]:Rotate y,ToolPose[5]:Rotate x
     */
    void FKResults(const QVector<float>& ToolPose);
    /**
     * @brief IKResults
     * @param isSolutionNotFound: true if soultion is not found
     * @param JointsPosition: input of Inverse kinematics
     */
    void IKResults(const bool& isSolutionNotFound,const QVector<float>& JointsPosition);

public Q_SLOTS:
    // slots to listen to command from request of calculating of kinematics
    /**
     * @brief Forward kinematcis calculation
     * @param CurrentJointsPosition
     */
    void fKCal(const QVector<float> &CurrentJointsPosition, float *ToolPose);
    /**
     * @brief Inverse kinematcis calculation
     * @param CurrentJointsPosition
     * @param TargetPose: TargetPose[0]:px,TargetPose[1]:py,TargetPose[2]:pz
     * TargetPose[3]:Rz,TargetPose[4]:Ry,TargetPose[5]:Rx
     */
    void IKCal(const QVector<float>& CurrentJointsPosition, const QVector<float>& TargetPose);

};
//Q_DECLARE_METATYPE(QVector<float>)
#endif // KINEMATICS_INTERFACE_H
