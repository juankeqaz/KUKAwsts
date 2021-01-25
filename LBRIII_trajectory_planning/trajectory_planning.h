#ifndef TRAJECTORY_PLANNING_H
#define TRAJECTORY_PLANNING_H

#include "LBRIII_kinematics.h"
#include "mathtype.h"
//namespace LINE_ARC {

typedef enum _PositionPathShape{
    STRAIGHTLINE,
    CIRCLEARC
}PositionPathShape;

typedef struct _STrajectoryParameter // sinusoidal acceleration, const velocity and sinusoidal deceleration
{
    VTYPE v,a,omega;
    VTYPE t1,t2,T;
    VTYPE s1,s2;
}STrajectoryParameter;

typedef struct _LineGeometry
{
    VTYPE startpoint[3]; // start point
    VTYPE direction[3];  // direction unit
    VTYPE length;        // length
}LineGeometry;

typedef struct _CircleArcGeometry
{
    //VTYPE startpoint[3]; // start point
    VTYPE center[3];     // center of circle arc
    VTYPE u[3], v[3];  // u, v vectors
    VTYPE radius;      // radius
    VTYPE maxtheta;    // the center angle corresponding to the endpoint
}CircleArcGeometry;

typedef struct _OrientationGeometry
{
    VTYPE R0[3][3]; // start rotation
    AxisAngle AA; // R0^T*R1
}OrientationGeometry;

typedef struct _TransformationGeometry
{
    PositionPathShape ppshape; // position path shape
    void* ppgeo;           // position path parameter
    void* rpgeo;           // orientation path parameter
}TransformationGeometry;
// S shape curve
void strajectory(const VTYPE l, // path length
                 const VTYPE v, // const velocity -- no more than vlimit
                 const VTYPE a, // no more than alimit
                 STrajectoryParameter* trajp);                              // calculate parameters of s(t)

VTYPE strajectoryscale(const STrajectoryParameter* trajp,                   // s(t)
                       const VTYPE t);

void fiveordertrajectory(const VTYPE p0, const VTYPE v0, const VTYPE a0,
                         const VTYPE p1, const VTYPE v1, const VTYPE a1,
                         const VTYPE T,
                         VTYPE coefficient[6]);

VTYPE fiveordertrajectoryscale(const VTYPE coefficient[6],           // a5 a4 a3 a2 a1 a0
                               const VTYPE t);
// -- line --
void linepath(const VTYPE p0[3], const VTYPE p1[3],
              LineGeometry* line);                                   // f(s)

void lineinterpolation(const LineGeometry* line,                     // f(s(t))
                       const STrajectoryParameter* trajp,
                       const VTYPE t,
                       VTYPE p[3]);
// -- circle --
void circlearcpath(const VTYPE p0[3], const VTYPE p1[3],const VTYPE p2[3],const bool flag,    // f(s)
                   CircleArcGeometry* circlearc);

void circlearcinterpolation(const CircleArcGeometry* circlearc,
                            const STrajectoryParameter* trajp,
                            const VTYPE t,
                            VTYPE p[3]);

// -- orientation path --
void orientationpath(const VTYPE R0[3][3],
                     const VTYPE R1[3][3],
                     OrientationGeometry* orientation);

void orientationinterpolation(const OrientationGeometry* orientation,
                              const VTYPE coefficient[6],
                              const VTYPE t,
                              VTYPE r[3][3]);
void calculat_line_Geometry(const VTYPE Q0[6], const VTYPE Q1[6],\
                              LineGeometry *line,OrientationGeometry *orientation);
void calculat_line_Trajectory(const LineGeometry *line,const OrientationGeometry *orientation,const VTYPE v, const VTYPE a,\
                              STrajectoryParameter *linetrajp,VTYPE orientationtrajp[6]);
void calculat_next_lq_point(const LineGeometry *line,const OrientationGeometry *orientation,const STrajectoryParameter *linetrajp,const VTYPE orientationtrajp[6],\
                            const float time, const VTYPE currentq[6],float result[]);
// -- movecq --
void calculate_arc_Geometry(const VTYPE Q0[6], const VTYPE Q1[6], const VTYPE Q2[6],const bool flag,\
                            CircleArcGeometry* circlearc,OrientationGeometry* orientation);
void caculate_arc_Trajectory(const CircleArcGeometry* circlearc,const OrientationGeometry* orientation,const VTYPE v, const VTYPE a,\
                             STrajectoryParameter* circlearctrajp,VTYPE orientationtrajp[6]);
void caculate_next_arc_point(const CircleArcGeometry* circlearc,const OrientationGeometry* orientation,const STrajectoryParameter* circlearctrajp,\
                             const VTYPE orientationtrajp[6],const float time,const VTYPE currentq[6],float result[]);

void movecq(const VTYPE Q0[6], const VTYPE Q1[6],const VTYPE Q2[6], const VTYPE v, const VTYPE a, const bool flag);

// --- check line feasibility ---
bool islinefeasible(const VTYPE Q0[6], const VTYPE Q1[6], const VTYPE v, const VTYPE a);
// --- check circlearc feasibility ---
bool iscirclearcfeasible(const VTYPE Q0[6], const VTYPE Q1[6],const VTYPE Q2[6], const VTYPE v, const VTYPE a, const bool flag);
int changeparame(VTYPE Q[6]);
// -- some utility functions --
void threepointcircle(const VTYPE p0[3], const VTYPE p1[3],const VTYPE p2[3],VTYPE circlecenter[3]);

// -- closestq --
bool closestq(const VTYPE inversesolutions[8][6], const unsigned int solutionnum, const VTYPE currentq[6], float closestsolution[]);
//}
#endif // TRAJECTORY_PLANNING_H
