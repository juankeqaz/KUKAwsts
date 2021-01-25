#ifndef INTERPOLATIONFUNCTION_H
#define INTERPOLATIONFUNCTION_H
#include <stdbool.h>

//#include "utility.h"

// define struct five order polynomial coefficients
typedef struct _FiveOrderPolyCoefficients
{
    float data[6];
} FiveOrderPolyCoefficients;

// define struct waypoint
typedef struct _Waypoint
{
    float data[6];
} Waypoint;

// define struct velocity
typedef struct _Velocity
{
    float data[6];
} Velocity;

// define struct acceleration
typedef struct _Acceleration
{
    float data[6];
} Acceleration;

// define struct boundary condition
typedef struct _BoundaryCondition
{
    Velocity v;
    Acceleration a;
} BoundaryCondition;

// compute five order polynomial coefficients
bool cal_coefficients_for_one_joint_one_interval(FiveOrderPolyCoefficients * jointcoefficients, const float t,
                                                 const float p0, const float p1,
                                                 const float v0, const float v1,
                                                 const float a0, const float a1);
// compute position at given time point
float cal_position(const FiveOrderPolyCoefficients* coefficients, const float t);
// compute velocity at given time point
float cal_velocity(const FiveOrderPolyCoefficients* coefficients, const float t);
// compute acceleration at given time point
float cal_acceleration(const FiveOrderPolyCoefficients* coefficients, const float t);
// compute jerk at given time point
float cal_jerk(const FiveOrderPolyCoefficients* coefficients, const float t);

// compute velocity at middle via points
float cal_velocity_at_mid_point_one_joint(const float v01, const float v12);

// compute acceleration at middle via point
float cal_acceleration_at_mid_point_one_joint(const float a01, const float a12);

// compute coefficients all joints one interval
void cal_coefficients_for_all_joints_one_interval(FiveOrderPolyCoefficients* jointscoefficients, // six coefficients
                                                  const float timeduration,
                                                  const Waypoint* p0,const Waypoint* p1,
                                                  const Velocity* v0,const Velocity* v1,
                                                  const Acceleration* a0,const Acceleration* a1
                                                  );
// compute all joints interpolated positions velocities accelerations and jerks
void cal_coefficients_for_all_joints_all_intervals(FiveOrderPolyCoefficients** allcoefficients,
                                                   const Waypoint* waypoints,
                                                   const unsigned int waypointsnum,
                                                   const float* timepoints,
                                                   const BoundaryCondition *start,
                                                   const BoundaryCondition *end
                                                   );

// compute velocity and acceleration at mid via points
void cal_velocity_and_acceleration(const Waypoint* waypoints,
                                   const unsigned int waypointsnum,
                                   const float* timepoints,
                                   const BoundaryCondition *start,
                                   const BoundaryCondition *end,
                                   const unsigned int index,
                                   float* timeduration01,float* timeduration12,float* timeduration23,
                                   Velocity* v0,Velocity* v1,Velocity* v2, Velocity* v01, Velocity* v12, Velocity* v23,
                                   Acceleration* a0, Acceleration* a1, Acceleration* a01, Acceleration* a12);

// compute interpolated position for all joints at given time
//void cal_interpolated_position(float * positions,
//                               const FiveOrderPolyCoefficients** allcoefficients,
//                               const float* timepoints,
//                               const unsigned int index,
//                               const float t);

// write all interpolated position velocities and accelerations for all joints at all discrete time
void save_all_interpolated_data_to_file(const char* pfilepath, const char* vfilepath,
                                        const char* afilepath, const char* jfilepath,
                                        const FiveOrderPolyCoefficients** allcoefficients,
                                        const float* timepoints,
                                        const unsigned int waypointsnum,
                                        const float timestep
                                        );
// compute time points corresponding to waypoints
void cal_timepoints(float* timepoints,
                    const Waypoint* waypoints,
                    const unsigned int waypointsnum,
                    const float *velocitylimit);
// compute timeduration on one interval
float cal_timeduraion_on_one_interval(const Waypoint* from,
                                       const Waypoint* to,
                                       const float* velocitylimit);

#endif // INTERPOLATIONFUNCTION_H
