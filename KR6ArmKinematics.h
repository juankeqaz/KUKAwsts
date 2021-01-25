#ifndef KR6_ARM_KINEMATICS
#define KR6_ARM_KINEMATICS

#include "KinematicConstants.h"
#include "DataTypes.h"
#include <vector>
#include <QMatrix4x4>

namespace kr6_arm_kinematics
{

class Kinematics {

    public:

        Kinematics(const std::vector<double> & minAngles, const std::vector<double> & maxAngles);
        ~Kinematics();

        // Forward kinematics
        Vector3d FK(const JointValues & jointValues);

        // Inverse kinematics for one configuration
        bool IK(const Pose & pose, const std::vector<double> & configuration, JointValues & solution);

        // Incerse kinematics for all possible configurations
        int getAllIKSolutions(const Pose & pose, std::vector<JointValues> & solutions);

        void FK(const JointValues & jointValues,Pose &pose);



    private:
        matrix::Dcm<double> calcRotMatix(const double phi, const double theta, const double psi);

        bool checkAngles(const JointValues & jointValues);
        QMatrix4x4 getTjoint(double angle, double ai, double aph, double di);
        void getrpy(const QMatrix4x4 mt,Vector3d &rpy);
        double iszero(double value);

        JointValues minAng, maxAng;
};

}

#endif
