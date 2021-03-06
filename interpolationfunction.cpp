#include "interpolationfunction.h"
#include <math.h>
#include <stdio.h>

bool cal_coefficients_for_one_joint_one_interval(FiveOrderPolyCoefficients* jointcoefficients, const float t,
                                                 const float p0, const float p1,
                                                 const float v0, const float v1,
                                                 const float a0, const float a1)
{
    // coefficients order is
    // a5 a4 a3 a2 a1 a0
    float h = p1 - p0;
    jointcoefficients->data[0] = 1/(2*pow(t,5))*
            (12*h-6*(v0+v1)*t+(a1-a0)*t*t);
    jointcoefficients->data[1] = 1/(2*pow(t,4))*
            (-30*h+(14*v1+16*v0)*t+(3*a0-2*a1)*t*t);
    jointcoefficients->data[2] = 1/(2*pow(t,3))*
            (20*h-(8*v1+12*v0)*t-(3*a0-a1)*t*t);
    jointcoefficients->data[3] = 0.5*a0;
    jointcoefficients->data[4] = v0;
    jointcoefficients->data[5] = p0;
    return true;
}

float cal_position(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    return polynomialvalue(5,coefficients->data,t);
}

float cal_velocity(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    float velocitycoefficients[5];
    for(unsigned int i=0;i<5;++i)
    {
        velocitycoefficients[i] = coefficients->data[i]*(5-i);
    }
    return polynomialvalue(4,velocitycoefficients,t);
}

float cal_acceleration(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    float accelerationcoefficients[4];
    for(unsigned int i=0;i<4;++i)
    {
        accelerationcoefficients[i] = coefficients->data[i]*(5-i)*(4-i);
    }
    return polynomialvalue(3,accelerationcoefficients,t);
}

float cal_jerk(const FiveOrderPolyCoefficients *coefficients, const float t)
{
    float jerkcoefficients[3];
    for(unsigned int i=0;i<3;++i)
    {
        jerkcoefficients[i] = coefficients->data[i]*(5-i)*(4-i)*(3-i);
    }
    return polynomialvalue(2,jerkcoefficients,t);
}

float cal_velocity_at_mid_point_one_joint(const float v01, const float v12)
{
    // if pre and post average velocities have the same sign
    // the mid point velocity is the average of the two average velocities
    if(isgreaterthanzero(v01) == isgreaterthanzero(v12))
    {
        return 0.5*(v01+v12);
    }
    //
    else
    {
        return 0;
    }

}

float cal_acceleration_at_mid_point_one_joint(const float a01, const float a12)
{
    // if pre and post average accelerations have the same sign
    // the mid point acceleration is the average of the two average accelerations
    if(isgreaterthanzero(a01) == isgreaterthanzero(a12))
    {
        return 0.5*(a01+a12);
    }
    //
    else
    {
        return 0;
    }
}

//void cal_coefficients_for_all_joints_one_interval(FiveOrderPolyCoefficients *jointscoefficients, const float t, const float *p0, const float *p1, const float *v0, const float *v1, const float *a0, const float *a1)
//{
//    for(unsigned int i=0;i<6;++i)
//    {
//        cal_coefficients_for_one_joint_one_interval(jointscoefficients+i,t,p0[i],p1[i],v0[i],v1[i],a0[i],a1[i]);
//    }
//}

void cal_coefficients_for_all_joints_one_interval(FiveOrderPolyCoefficients *jointscoefficients,
                                                  const float timeduration,
                                                  const Waypoint *p0,const Waypoint *p1,
                                                  const Velocity *v0,const Velocity *v1,
                                                  const Acceleration *a0, const Acceleration *a1)
{
    for(unsigned int i=0;i<6;++i)
    {
        //printf("claculating %d joint coefficient\n",i);
        //printf("p0 %lf,v0 %lf,a0 %lf\n",p0->data[i],v0->data[i],a0->data[i]);
        //printf("p1 %lf,v1 %lf,a1 %lf\n",p1->data[i],v1->data[i],a1->data[i]);
        cal_coefficients_for_one_joint_one_interval(jointscoefficients+i,timeduration,
                                                    p0->data[i],p1->data[i],v0->data[i],v1->data[i],a0->data[i],a1->data[i]);
    }
}

void cal_coefficients_for_all_joints_all_intervals(FiveOrderPolyCoefficients **allcoefficients,
                                                   const Waypoint *waypoints,
                                                   const unsigned int waypointsnum,
                                                   const float *timepoints,
                                                   const BoundaryCondition* start,
                                                   const BoundaryCondition* end)
{
    // only one interval
    if(waypointsnum == 2)
    {
        cal_coefficients_for_all_joints_one_interval(allcoefficients[0],
                timepoints[1]-timepoints[0],
                &waypoints[0],&waypoints[1],
                &(start->v),&(end->v),
                &(start->a),&(end->a));
        //printf("waypoints num =2 computed\n");
        return;
    }

    // other cases
    unsigned int index = 0;
    float timeduration01 = 0;
    float timeduration12 = 0;
    float timeduration23 = 0;
    Velocity v0,v1,v2,v01,v12,v23;
    Acceleration a0,a1,a01,a12;
    while(index<=waypointsnum-2)
    {
        // calculate velocity and acceleration
        cal_velocity_and_acceleration(waypoints,waypointsnum,timepoints,
                                      start,end,
                                      index,
                                      &timeduration01,&timeduration12,&timeduration23,
                                      &v0,&v1,&v2,&v01,&v12,&v23,
                                      &a0,&a1,&a01,&a12);
        cal_coefficients_for_all_joints_one_interval(allcoefficients[index],timeduration01,
                                                     &waypoints[index],&waypoints[index+1],&v0,&v1,&a0,&a1);
        ++index;
    }

}


void cal_velocity_and_acceleration(const Waypoint *waypoints,
                                   const unsigned int waypointsnum,
                                   const float *timepoints,
                                   const BoundaryCondition *start,
                                   const BoundaryCondition *end,
                                   const unsigned int index,
                                   float *timeduration01, float *timeduration12, float *timeduration23,
                                   Velocity *v0, Velocity *v1, Velocity *v2, Velocity *v01, Velocity *v12, Velocity *v23,
                                   Acceleration *a0, Acceleration *a1, Acceleration *a01, Acceleration *a12)
{
    if(index == 0 && waypointsnum-3!=index) // the first interval and total points numer is not 3
    {
        //printf("the first interval of over 3 points\n");
        float t01 = timepoints[index+1]-timepoints[index];
        float t12 = timepoints[index+2]-timepoints[index+1];
        float t23 = timepoints[index+3]-timepoints[index+2];
        *timeduration01 = t01;
        *timeduration12 = t12;
        *timeduration23 = t23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = start->v.data[i];
            a0->data[i] = start->a.data[i];
            float v01i = (waypoints[index+1].data[i]-waypoints[index].data[i])/t01;
            float v12i = (waypoints[index+2].data[i]-waypoints[index+1].data[i])/t12;
            float v23i = (waypoints[index+3].data[i]-waypoints[index+2].data[i])/t23;
            float v1i = cal_velocity_at_mid_point_one_joint(v01i,v12i);
            float v2i = cal_velocity_at_mid_point_one_joint(v12i,v23i);
            float a01i = (v1i-v0->data[i])/t01;
            float a12i = (v2i-v1i)/t12;
            float a1i = cal_acceleration_at_mid_point_one_joint(a01i,a12i);
            v01->data[i] = v01i;
            v12->data[i] = v12i;
            v23->data[i] = v23i;
            v1->data[i] = v1i;
            v2->data[i] = v2i;
            a01->data[i] = a01i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(index == 0 && waypointsnum == 3) // the firse interval and total point number is 3
    {
        //printf("the first interval of 3 points\n");
        float t01 = timepoints[index+1]-timepoints[index];
        float t12 = timepoints[index+2]-timepoints[index+1];
        *timeduration01 = t01;
        *timeduration12 = t12;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = start->v.data[i];
            a0->data[i] = start->a.data[i];
            float v01i = (waypoints[index+1].data[i]-waypoints[index].data[i])/t01;
            float v12i = (waypoints[index+2].data[i]-waypoints[index+1].data[i])/t12;
            float v1i = cal_velocity_at_mid_point_one_joint(v01i,v12i);
            float a01i = (v1i-v0->data[i])/t01;
            float a12i = (end->v.data[i]-v1i)/t12;
            float a1i = cal_acceleration_at_mid_point_one_joint(a01i,a12i);
            v01->data[i] = v01i;
            v12->data[i] = v12i;
            v1->data[i] = v1i;
            a01->data[i] = a01i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(index!=0 && waypointsnum-3 == index) // the second last interval
    {
        //printf("the second lase interval of over 3 points\n");
        *timeduration01 = *timeduration12;
        *timeduration12 = *timeduration23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            v1->data[i] = v2->data[i];
            a0->data[i] = a1->data[i];
            a01->data[i] = a12->data[i];
            float a12i = (end->v.data[i]-v1->data[i])/(*timeduration12);
            float a1i = cal_acceleration_at_mid_point_one_joint(a01->data[i],a12i);
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
    else if(waypointsnum-2 == index) // the last interval
    {
        //printf("the last interval\n");
        *timeduration01 = *timeduration12;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            a0->data[i] = a1->data[i];
            v1->data[i] = end->v.data[i];
            a1->data[i] = end->a.data[i];
        }
    }
    else   // interval 1 to n-4
    {
        *timeduration01 = *timeduration12;
        *timeduration12 = *timeduration23;
        float t23 = timepoints[index+3]-timepoints[index+2];
        *timeduration23 = t23;
        for(unsigned int i=0;i<6;++i)
        {
            v0->data[i] = v1->data[i];
            v1->data[i] = v2->data[i];
            v01->data[i] = v12->data[i];
            v12->data[i] = v23->data[i];
            a0->data[i] = a1->data[i];
            a01->data[i] = a12->data[i];
            float v23i = (waypoints[index+3].data[i]-waypoints[index+2].data[i])/t23;
            float v2i =cal_velocity_at_mid_point_one_joint(v12->data[i],v23i);
            float a12i = (v2i-v1->data[i])/(*timeduration12);
            float a1i = cal_acceleration_at_mid_point_one_joint(a01->data[i],a12i);
            v23->data[i] = v23i;
            v2->data[i] = v2i;
            a12->data[i] = a12i;
            a1->data[i] = a1i;
        }
    }
}

//void cal_interpolated_position(float * positions,const FiveOrderPolyCoefficients **allcoefficients,
//                               const float *timepoints, const unsigned int index, const float t)
//{
//    // shift time
//    float shiftedtime = t - timepoints[index];
//    // compute interpolated positions for all joints
//    for(unsigned int i=0;i<6;++i)
//    {
//        positions[i] = cal_position(&(allcoefficients[index][i]),shiftedtime);
//    }
//}

void save_all_interpolated_data_to_file(const char* pfilepath, const char* vfilepath,
                                        const char* afilepath, const char *jfilepath,
                                        const FiveOrderPolyCoefficients **allcoefficients,
                                        const float *timepoints, const unsigned int waypointsnum,
                                        const float timestep)
{
    // open file
    FILE* fp,*fv,*fa,*fj;

    fp = fopen(pfilepath,"w");
    fv = fopen(vfilepath,"w");
    fa = fopen(afilepath,"w");
    fj = fopen(jfilepath,"w");
    if(fp == NULL || fv == NULL || fa == NULL || fj == NULL)
    {
        printf("cannot open file!\n");
        return;
    }

    // determine discrete time point number
    unsigned int timecount = (unsigned int)floor(timepoints[waypointsnum-1]/timestep)+1;
    //    printf("raw time count: %lf\n",timepoints[waypointsnum-1]/timestep);
    //    printf("ceil time count: %lf\n",ceil(timepoints[waypointsnum-1]/timestep));
    //printf("time count: %d\n",timecount);
    // init interval index,currenttime and coefficients
    unsigned int index = 0;
    float currenttime = timepoints[0];
    float shiftedtime = 0;
    const FiveOrderPolyCoefficients* coefficients = allcoefficients[0];
    float position,velocity,acceleration,jerk;
    for(unsigned int i=0;i<timecount;++i)
    {
        if(currenttime>timepoints[index+1])
        {
            index++;
            // end time point limit
            if(index>waypointsnum-2)
            {
                index = waypointsnum-2;
            }
            coefficients = allcoefficients[index];
        }
        shiftedtime = currenttime - timepoints[index];
        currenttime += timestep;
        for(unsigned int j=0;j<5;++j)
        {
            position = cal_position(&(coefficients[j]),shiftedtime);
            velocity = cal_velocity(&(coefficients[j]),shiftedtime);
            acceleration = cal_acceleration(&(coefficients[j]),shiftedtime);
            jerk = cal_jerk(&(coefficients[j]),shiftedtime);
            fprintf(fp,"%lf,",position);
            fprintf(fv,"%lf,",velocity);
            fprintf(fa,"%lf,",acceleration);
            fprintf(fj,"%lf,",jerk);
        }
        position = cal_position(&(coefficients[5]),shiftedtime);
        velocity = cal_velocity(&(coefficients[5]),shiftedtime);
        acceleration = cal_acceleration(&(coefficients[5]),shiftedtime);
        jerk = cal_jerk(&(coefficients[5]),shiftedtime);
        fprintf(fp,"%lf\n",position);
        fprintf(fv,"%lf\n",velocity);
        fprintf(fa,"%lf\n",acceleration);
        fprintf(fj,"%lf\n",jerk);

    }
    fclose(fp);
    fclose(fv);
    fclose(fa);
    fclose(fj);
}

void cal_timepoints(float *timepoints, const Waypoint *waypoints, const unsigned int waypointsnum, const float *velocitylimit)
{
    // the first time point is always zero
    timepoints[0] = 0;
    for(unsigned int i=1;i<waypointsnum;++i)
    {
        timepoints[i] = timepoints[i-1] + cal_timeduraion_on_one_interval(&(waypoints[i-1]),&(waypoints[i]),velocitylimit);
    }
}

float cal_timeduraion_on_one_interval(const Waypoint *from, const Waypoint *to, const float *velocitylimit)
{
    float timeduration = 0;
    float temp = 0;
    for(unsigned int i=0;i<6;++i)
    {
        temp = fabs(from->data[i]-to->data[i])/velocitylimit[i]*2;
        timeduration = (temp>timeduration? temp:timeduration);
    }
    return timeduration;
}
