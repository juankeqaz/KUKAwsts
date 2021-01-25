#ifndef KINEMATIC_CONSTANTS
#define KINEMATIC_CONSTANTS

#include <vector>
#define M_PI_2 1.57079632679489661923
#define M_2_PI 0.63661977236758134308


const int DOF = 6;

const double d0 = 0.4;
const double a1 = 0.025;
const double a2 = 0.455;
const double a3 = 0.025;
const double d4 = 0.445;
const double d6 = 0.09;

const std::vector<double> maxJointAngles = {2.96705972839, 0.78539816339, 2.72271363311, 3.22885911619, 2.09439510239, 6.10865238198};
const std::vector<double> minJointAngles = {-2.96705972839, -3.31612557879, -2.09439510239, -3.22885911619, -2.09439510239, -6.10865238198};

typedef struct _FiveOrderPolyCoefficients
{
    float data[6];
} FiveOrderPolyCoefficients;

typedef struct _Waypoint
{
    float data[6];
} Waypoint;

typedef struct _Velocity
{
    float data[6];
} Velocity;

// define struct acceleration
typedef struct _Acceleration
{
    float data[6];
} Acceleration;


typedef struct _BoundaryCondition
{
    Velocity v;
    Acceleration a;
} BoundaryCondition;

typedef struct _STrajectoryParameter // sinusoidal acceleration, const velocity and sinusoidal deceleration
{
    float v,a,omega;
    float t1,t2,T;
    float s1,s2;
}STrajectoryParameter;

typedef struct _LineGeometry
{
    float startpoint[3]; // start point
    float direction[3];  // direction unit
    float length;        // length
}LineGeometry;

typedef struct _CircleArcGeometry
{
    //VTYPE startpoint[3]; // start point
    float center[3];     // center of circle arc
    float u[3], v[3];  // u, v vectors
    float radius;      // radius
    float maxtheta;    // the center angle corresponding to the endpoint
}CircleArcGeometry;
typedef struct _AxisAngle
{
    float axis[3];
    float angle;
}AxisAngle;

typedef struct _OrientationGeometry
{
    float R0[3][3]; // start rotation
    AxisAngle AA; // R0^T*R1
}OrientationGeometry;

typedef struct _Transformation
{
    float rotation[3][3];
    float translation[3];
}Transformation;

typedef struct _Pose
{
    float x,y,z,alpha,beta,gamma;
} SPose;

typedef enum GeometryShape{
    LINE,
    CIRCLE
}GeometryShape;

typedef struct GeometryMetaData
{
    void* shapegeometry;
    GeometryShape shape;

}GeometryMetaData;

#endif
