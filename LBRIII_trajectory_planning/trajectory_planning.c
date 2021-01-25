#include "trajectory_planning.h"
#include "stdio.h"
#include "math.h"
#include "malloc.h"

void strajectory(const VTYPE l,
                 const VTYPE v,
                 const VTYPE a,
                 STrajectoryParameter *trajp)
{
    VTYPE a_inv = 1/a;

    VTYPE omega,t1,s1,t2,s2,T;
    // -- is the given velocity can be reached? ---
    VTYPE maxv = sqrt(M_2_PI*l*a);

    if(v>maxv)
    {
        //printf("v is greater than maxv\n");
        // only sinusoidal acceleration and deceleration
        trajp->v = maxv;
        omega = 2*a/maxv;
        t1 = M_PI_2*maxv*a_inv;
        s1 = 0.5*l;
        t2 = t1;
        s2 = 0;
        T = 2*t1;
    }
    else
    {
        //printf("v is less than maxv\n");
        trajp->v = v;
        VTYPE v_inv = 1/v;
        omega = 2*a*v_inv;
        t1 = M_PI_2*v*a_inv;
        s1 = 0.5*v*t1;
        t2 = l*v_inv;
        s2 = l - v*t1;
        T = t1 + t2;
    }
    trajp->a = a;
    trajp->omega = omega;
    trajp->t1 = t1;
    trajp->t2 = t2;
    trajp->T = T;
    trajp->s1 = s1;
    trajp->s2 = s2;
}

VTYPE strajectoryscale(const STrajectoryParameter* trajp,
                       const VTYPE t)
{
    if(t<trajp->t1)      // sinusoidal accelaration
    {
        VTYPE omega_inv = 1/trajp->omega;
        return trajp->a*omega_inv*(t-sin(trajp->omega*t)*omega_inv);
    }
    else if(t<trajp->t2) // const velocity
    {
        return trajp->s1 + trajp->v*(t-trajp->t1);
    }
    else                    // sinusoidal decelaration
    {
        VTYPE omega_inv = 1/trajp->omega, tp = t - trajp->t2;
        return trajp->s1 + trajp->s2 + trajp->a*omega_inv*(tp+sin(trajp->omega*tp)*omega_inv);
    }
}

void fiveordertrajectory(const VTYPE p0, const VTYPE v0, const VTYPE a0,
                         const VTYPE p1, const VTYPE v1, const VTYPE a1,
                         const VTYPE T,
                         VTYPE coefficient[6])
{
    VTYPE h = p1 - p0;
    coefficient[0] = 1/(2*pow(T,5))*(12*h-6*(v0+v1)*T+(a1-a0)*T*T);
    coefficient[1] = 1/(2*pow(T,4))*(-30*h+(14*v1+16*v0)*T+(3*a0-2*a1)*T*T);
    coefficient[2] = 1/(2*pow(T,3))*(20*h-(8*v1+12*v0)*T-(3*a0-a1)*T*T);
    coefficient[3] = 0.5*a0;
    coefficient[4] = v0;
    coefficient[5] = p0;
}

VTYPE fiveordertrajectoryscale(const VTYPE coefficient[6], const VTYPE t)
{
    VTYPE result = coefficient[0];
    for(unsigned int i=1;i<6;++i)
    {
        result = result*t + coefficient[i];
    }
    return result;
}

void linepath(const VTYPE p0[3], const VTYPE p1[3], LineGeometry *line)
{
    VTYPE delta[3];
    V_sub_V(p1,p0,delta,3);           // delta = p1 - p0;
    V_assign(p0,line->startpoint,3);  // p0 is the start point
    V_dir(delta,line->direction,3);   // delta/norm(delta) is the direction unit
    line->length = V_norm(delta,3);   // norm(delta) is length
}

void lineinterpolation(const LineGeometry* line,
                       const STrajectoryParameter* trajp,
                       const VTYPE t,
                       VTYPE p[3])
{
    // -- calculate scale corresponding to t --             s(t)
    VTYPE s = strajectoryscale(trajp,t);
    // -- calculate point onlth line corresponding to s --  p = p0 + s*(p1 - p0)/norm(p1 - p0)
    VTYPE temp[3];
    V_mul_S(line->direction,s,temp,3);
    V_add_V(line->startpoint,temp,p,3);
}

void circlearcpath(const VTYPE p0[3], const VTYPE p1[3], const VTYPE p2[3], const bool flag, CircleArcGeometry *circlearc)
{
    // --- calculate centre of circle  & radius ---
    // three points cannot be colinear
    VTYPE p10[3], p20[3], pn[3], pc[3], pc0[3], pc2[3],temp[3];
    V_sub_V(p0,p1,p10,3); // p10 = p0 - p1
    V_sub_V(p0,p2,p20,3); // p20 = p0 - p2
    //    V_sub_V(p1,p2,p21); // p21 = p1 - p2
    V_cross_pro(p10,p20,pn); // pn = p10 X p20
    if(V_norm(pn,3)<EPS)
    {
        printf("three points are colinear\n");
        return;
    }
    threepointcircle(p0,p1,p2,pc);
    V_sub_V(p0,pc,pc0,3);
    V_sub_V(p2,pc,pc2,3);
    V_cross_pro(pn,pc0,temp);

    V_assign(pc,circlearc->center,3);  // center of circle
    V_dir(pc0,circlearc->u,3); // u = pc0 / norm(pc0)
    V_dir(temp,circlearc->v,3); // v = (pn X pc0) / norm(pn X pc0)
    circlearc->radius = V_norm(pc0,3); // radius
    if(flag)
    {
        circlearc->maxtheta = 2*M_PI;
    }
    else
    {
        circlearc->maxtheta = atan2(V_dot_pro(pc2,circlearc->v,3),V_dot_pro(pc2,circlearc->u,3)); // --- center angle range ---
        if(circlearc->maxtheta<0)
        {
            circlearc->maxtheta += 2*M_PI;
        }
    }
    // --- print circle arc geometry parameters ---
    //    V_print(circlearc->center,3);    //  print centre of circle
    //    V_print(circlearc->u,3);         //  print u vector
    //    V_print(circlearc->v,3);         //  print v vector
    //    printf("radius: %lf m\n",circlearc->radius);    //  print radius
    //    printf("max theta: %lf rad\n",circlearc->maxtheta);  //  print max theta
}

void circlearcinterpolation(const CircleArcGeometry *circlearc,
                            const STrajectoryParameter *trajp,
                            const VTYPE t,
                            VTYPE p[3])
{
    // -- calculate scale corresponding to t --              s(t)
    VTYPE s = strajectoryscale(trajp,t);
    // -- calculate point onlth line corresponding to s --   p = center + radius*(u*cos(s)+v*sin(s))
    VTYPE uc = circlearc->radius*cos(s),
            vc = circlearc->radius*sin(s),
            temp1[3],temp2[3],temp3[3];
    V_mul_S(circlearc->u,uc,temp1,3);
    V_mul_S(circlearc->v,vc,temp2,3);
    V_add_V(temp1,temp2,temp3,3);
    V_add_V(circlearc->center,temp3,p,3);
}

void orientationpath(const VTYPE R0[3][3], const VTYPE R1[3][3],
OrientationGeometry *orientation)
{
    VTYPE temp1[3][3],temp2[3][3];
    R_inv(R0,temp1);
    R_mul_R(temp1,R1,temp2);
    //printf("temp2:\n");
    //R_print(temp2);
    R_assign(R0,orientation->R0);
    R_to_AA(temp2,&orientation->AA);
    //    printf("oritentation info:\n");
    //    printf("R0:\n");
    //    R_print(orientation->R0);
    //    printf("AA:\n");
    //    AA_print(&orientation->AA);
}

void orientationinterpolation(const OrientationGeometry *orientation, const VTYPE coefficient[6],
const VTYPE t,
VTYPE r[3][3])
{
    VTYPE s = fiveordertrajectoryscale(coefficient,t); // theta
    VTYPE temp[3][3];
    R_from_axis_angle(orientation->AA.axis,s,temp);
    R_mul_R(orientation->R0,temp,r);
}

void movecq(const VTYPE Q0[6], const VTYPE Q1[6], const VTYPE Q2[6], const VTYPE v, const VTYPE a, const bool flag)
{
    Transformation T0,T1,T2;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    LBR_fk(Q2,&T2);
    VTYPE p0[3],p1[3],p2[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T2,R1);
    T_to_P(&T2,p2);
    // position trajectory
    CircleArcGeometry* circlearc = (CircleArcGeometry*)malloc(sizeof(CircleArcGeometry));
    circlearcpath(p0,p1,p2,flag,circlearc);
    STrajectoryParameter* circlearctrajp = (STrajectoryParameter*)malloc(sizeof(STrajectoryParameter));
    strajectory(circlearc->maxtheta,v,a,circlearctrajp);
    // orientation trajectory
    OrientationGeometry* orientation = (OrientationGeometry*)malloc(sizeof(OrientationGeometry));
    orientationpath(R0,R1,orientation);
    VTYPE orientationtrajp[6];
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,circlearctrajp->T,orientationtrajp);

    VTYPE time = 0, timestep = 0.01;
    unsigned int timenum = ceil(circlearctrajp->T/timestep)+1;
    FILE* file = fopen("D:/configurationdata.txt","a");

    VTYPE targetp[3],targetR[3][3],currentq[6];
    V_assign(Q0,currentq,6);
    Transformation* targetT = (Transformation*)malloc(sizeof(Transformation));
    VTYPE inversesolutions[8][6];
    unsigned int solutionnum = 0;
    //    VTYPE distance,currentdistance;
    //    unsigned int minindex;
    printf("time num is: %d\n",timenum);
    clock_t start = clock();
    for(unsigned int i=0;i<timenum;++i)
    {

        circlearcinterpolation(circlearc,circlearctrajp,time,targetp);
        orientationinterpolation(orientation,orientationtrajp,time,targetR);
        // inverse kinematics
        T_from_RP(targetR,targetp,targetT);
        LBR_ik(targetT,&solutionnum,inversesolutions);
        if(!solutionnum)
        {
            printf("not reachable point\n");
            return;
        }
        if(!closestq(inversesolutions,solutionnum,currentq,currentq))
        {
           return;
        }
        time+=timestep;
    }
    clock_t end = clock();
    float duration = ((float)end-start)/CLOCKS_PER_SEC;
    printf("time consumption: %lf s\n",duration/timenum);

    free(circlearc);
    free(circlearctrajp);
    free(orientation);
    free(targetT);
    fclose(file);
}

bool islinefeasible(const VTYPE Q0[6], const VTYPE Q1[6], const VTYPE v, const VTYPE a)
{
    int initsign; // start point det jacobian sign
    bool isclosetozero;
    int sign;
    detjacobianinfo(Q0,&isclosetozero,&initsign);
    if(isclosetozero)
    {
        printf("line start point is singular\n");
        return false;
    }

    detjacobianinfo(Q1,&isclosetozero,&sign);
    if(isclosetozero)
    {
        printf("line end point is singular\n");
        return false;
    }
    if(sign!=initsign)
    {
        printf("singular point on the line\n");
        return false;
    }

    Transformation T0,T1;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    VTYPE p0[3],p1[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T1,R1);
    // position trajectory
    LineGeometry* line = (LineGeometry*)malloc(sizeof(LineGeometry));
    linepath(p0,p1,line);
    STrajectoryParameter* linetrajp = (STrajectoryParameter*)malloc(sizeof(STrajectoryParameter));
    strajectory(line->length,v,a,linetrajp);
    // orientation trajectory
    OrientationGeometry* orientation = (OrientationGeometry*)malloc(sizeof(OrientationGeometry));
    orientationpath(R0,R1,orientation);
    VTYPE orientationtrajp[6];
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,linetrajp->T,orientationtrajp);

    VTYPE timestep = 0.05,time = timestep;

    VTYPE targetp[3],targetR[3][3],currentq[6];
    V_assign(Q0,currentq,6);
    Transformation* targetT = (Transformation*)malloc(sizeof(Transformation));
    VTYPE inversesolutions[8][6];
    unsigned int solutionnum = 0;
    VTYPE distance,currentdistance;
    unsigned int minindex;
    while(time<linetrajp->T)
    {
        // cartesian interpolation
        lineinterpolation(line,linetrajp,time,targetp);
        orientationinterpolation(orientation,orientationtrajp,time,targetR);
        // inverse kinematics
        T_from_RP(targetR,targetp,targetT);
        LBR_ik(targetT,&solutionnum,inversesolutions);
        if(!solutionnum)
        {
            printf("not reachable point on the line\n");
            return false;
        }
        // find closet configuration
        minindex = 0;
        currentdistance = caldistance(inversesolutions[0],currentq);
        distance = currentdistance;
        for(unsigned int j=1;j<solutionnum;++j)
        {
            currentdistance = caldistance(inversesolutions[j],currentq);
            if(currentdistance < distance)
            {
                distance = currentdistance;
                minindex = j;
            }
        }
        V_assign(inversesolutions[minindex], currentq, JOINT_NUM);
        // check
        detjacobianinfo(currentq,&isclosetozero,&sign);
        if(isclosetozero)
        {
            printf("singular point on the line\n");
            V_print(currentq,6);
            return false;
        }
        if(sign!=initsign)
        {
            printf("middle point on the line is singular\n");
            return false;
        }

        time += timestep;
    }
    free(line);
    free(linetrajp);
    free(orientation);
    free(targetT);
    return true;
}

bool iscirclearcfeasible(const VTYPE Q0[6], const VTYPE Q1[6], const VTYPE Q2[6], const VTYPE v, const VTYPE a, const bool flag)
{
    //printf("checking circle feasibility\n");
    //V_print(Q0,6);V_print(Q1,6);V_print(Q2,6);
    int initsign; // start point det jacobian sign
    bool isclosetozero;
    int sign;
    detjacobianinfo(Q0,&isclosetozero,&initsign);
    if(isclosetozero)
    {
        printf("circlearc start point is singular\n");
        return false;
    }

    detjacobianinfo(Q2,&isclosetozero,&sign);
    if(isclosetozero)
    {
        printf("circlearc end point is singular\n");
        return false;
    }
    if(sign!=initsign)
    {
        printf("singular point on the circlearc\n");
        return false;
    }

    Transformation T0,T1,T2;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    LBR_fk(Q2,&T2);
    VTYPE p0[3],p1[3],p2[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T2,R1);
    T_to_P(&T2,p2);
    // position trajectory
    CircleArcGeometry* circlearc = (CircleArcGeometry*)malloc(sizeof(CircleArcGeometry));
    circlearcpath(p0,p1,p2,flag,circlearc);
    STrajectoryParameter* circlearctrajp = (STrajectoryParameter*)malloc(sizeof(STrajectoryParameter));
    strajectory(circlearc->maxtheta,v,a,circlearctrajp);
    // orientation trajectory
    OrientationGeometry* orientation = (OrientationGeometry*)malloc(sizeof(OrientationGeometry));
    orientationpath(R0,R1,orientation);
    VTYPE orientationtrajp[6];
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,circlearctrajp->T,orientationtrajp);
    VTYPE timestep = 0.01, time = timestep;

    VTYPE targetp[3],targetR[3][3],currentq[6];
    V_assign(Q0,currentq,6);
    Transformation* targetT = (Transformation*)malloc(sizeof(Transformation));
    VTYPE inversesolutions[8][6];
    unsigned int solutionnum = 0;
    //    VTYPE distance,currentdistance;
    //    unsigned int minindex;
    while(time < circlearctrajp->T)
    {
        circlearcinterpolation(circlearc,circlearctrajp,time,targetp);
        orientationinterpolation(orientation,orientationtrajp,time,targetR);
        // inverse kinematics
        T_from_RP(targetR,targetp,targetT);
        //T_print(targetT);
        LBR_ik(targetT,&solutionnum,inversesolutions);
        if(!solutionnum)
        {
            printf("not reachable point on the circlearc\n");
            return false;
        }
        // find closet configuration
        if(!closestq(inversesolutions,solutionnum,currentq,currentq))
        {
            return false;
        }
        //        minindex = 0;
        //        currentdistance = caldistance(inversesolutions[0],currentq);
        //        distance = currentdistance;

        //        for(unsigned int j=1;j<solutionnum;++j)
        //        {

        //            currentdistance = caldistance(inversesolutions[j],currentq);
        //            if(currentdistance < distance)
        //            {
        //                distance = currentdistance;
        //                minindex = j;
        //            }
        //        }
        //        V_assign(inversesolutions[minindex], currentq, JOINT_NUM);

        // check
        detjacobianinfo(currentq,&isclosetozero,&sign);
        if(isclosetozero)
        {
            printf("singular point on the circlearc\n");
            V_print(currentq,6);
            return false;
        }
        if(sign!=initsign)
        {
            printf("middle point on the circlearc is singular\n");
            V_print(currentq,6);
            return false;
        }

        time += timestep;
    }
    free(circlearc);
    free(circlearctrajp);
    free(orientation);
    free(targetT);
    return true;
}

void threepointcircle(const VTYPE p0[3], const VTYPE p1[3], const VTYPE p2[3], VTYPE circlecenter[3])
{
    VTYPE a1, b1, c1, d1;
    VTYPE a2, b2, c2, d2;
    VTYPE a3, b3, c3, d3, d4;

    VTYPE x1 = p0[0], y1 = p0[1], z1 = p0[2];
    VTYPE x2 = p1[0], y2 = p1[1], z2 = p1[2];
    VTYPE x3 = p2[0], y3 = p2[1], z3 = p2[2];

    a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
    b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
    c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

    a2 = 2 * (x2 - x1);
    b2 = 2 * (y2 - y1);
    c2 = 2 * (z2 - z1);
    d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

    a3 = 2 * (x3 - x1);
    b3 = 2 * (y3 - y1);
    c3 = 2 * (z3 - z1);
    d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

    d4 = 1/(a1*(b2*c3 - b3*c2) - a2*(b1*c3 - b3*c1) + a3*(b1*c2 - b2*c1));

    circlecenter[0] = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)*d4;
    circlecenter[1] =  (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)*d4;
    circlecenter[2] = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)*d4;
}

bool closestq(const VTYPE inversesolutions[8][6], const unsigned int solutionnum, const VTYPE currentq[6], VTYPE closestsolution[6])
{
    // find closet configuration
    unsigned int minindex = 0;
    VTYPE currentdistance = caldistance(inversesolutions[0],currentq);
    VTYPE distance = currentdistance;
    for(unsigned int i=1;i<solutionnum;++i)
    {
        currentdistance = caldistance(inversesolutions[i],currentq);
        if(currentdistance < distance)
        {
            distance = currentdistance;
            minindex = i;
        }

    }
//    for(int t=0;t<JOINT_NUM;++t)
//    {
//       printf("closetq=%f\n",inversesolutions[minindex][t]);
//    }
//    printf("distance=%f\n",distance);

    if(distance>5)
    {
        //printf("distance is bigger than 0.4\n");
        return false;
    }
    //    if(sizeof(closestsolution) == JOINT_NUM)
    //    {
    for(int i=0;i<JOINT_NUM;++i)
    {
        closestsolution[i]=inversesolutions[minindex][i];
    }
    return true;
    //    }
    //V_assign(inversesolutions[minindex],closestsolution,JOINT_NUM);
}

void calculat_next_lq_point(const LineGeometry *line,const OrientationGeometry *orientation,const STrajectoryParameter *linetrajp,const VTYPE orientationtrajp[6],\
const float time, const VTYPE currentq[6],float result[6])
{
    // 1. transformation interpolation
    Transformation targetT;
    VTYPE inversesolutions[8][6];
    unsigned int solutionnum = 0;

    lineinterpolation(line,linetrajp,time,targetT.translation);
    orientationinterpolation(orientation,orientationtrajp,time,targetT.rotation);

    // 2. inverse kinematics
    LBR_ik(&targetT,&solutionnum,inversesolutions);
    if(!solutionnum)
    {
        printf("not reachable point\n");
        return;
    }
    // 3. find closet configuration
    closestq(inversesolutions,solutionnum,currentq,result);
}

void calculat_line_Geometry(const float Q0[], const float Q1[], LineGeometry *line, OrientationGeometry *orientation)
{
    //printf("11111111111111");
    Transformation T0,T1;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    VTYPE p0[3],p1[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T1,R1);

    linepath(p0,p1,line);
    orientationpath(R0,R1,orientation);
}

void calculat_line_Trajectory(const LineGeometry *line, const OrientationGeometry *orientation, const float v, const float a, STrajectoryParameter *linetrajp, float orientationtrajp[6])
{
    //printf("222222222222222222");
    strajectory(line->length,v,a,linetrajp);
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,linetrajp->T,orientationtrajp);
}

void calculate_arc_Geometry(const float Q0[], const float Q1[], const float Q2[], const bool flag, CircleArcGeometry *circlearc, OrientationGeometry *orientation)
{
    Transformation T0,T1,T2;
    LBR_fk(Q0,&T0);
    LBR_fk(Q1,&T1);
    LBR_fk(Q2,&T2);
    VTYPE p0[3],p1[3],p2[3],R0[3][3],R1[3][3];
    T_to_P(&T0,p0);T_to_R(&T0,R0);
    T_to_P(&T1,p1);T_to_R(&T2,R1);
    T_to_P(&T2,p2);
    // position trajectory
    circlearcpath(p0,p1,p2,flag,circlearc);
    orientationpath(R0,R1,orientation);
}

void caculate_arc_Trajectory(const CircleArcGeometry *circlearc, const OrientationGeometry *orientation, const float v, const float a, STrajectoryParameter *circlearctrajp, float orientationtrajp[])
{
    strajectory(circlearc->maxtheta,v,a,circlearctrajp);
    fiveordertrajectory(0,0,0,orientation->AA.angle,0,0,circlearctrajp->T,orientationtrajp);
}

void caculate_next_arc_point(const CircleArcGeometry *circlearc, const OrientationGeometry *orientation, const STrajectoryParameter *circlearctrajp, const float orientationtrajp[], const float time, const float currentq[], float result[])
{
    VTYPE targetp[3],targetR[3][3];;
    //V_assign(Q0,currentq,6);
    Transformation targetT;
    VTYPE inversesolutions[8][6];
    unsigned int solutionnum = 0;
    //printf("time num is: %d\n",timenum);

    circlearcinterpolation(circlearc,circlearctrajp,time,targetp);
    orientationinterpolation(orientation,orientationtrajp,time,targetR);
    // inverse kinematics
    T_from_RP(targetR,targetp,&targetT);
    LBR_ik(&targetT,&solutionnum,inversesolutions);
    if(!solutionnum)
    {
        printf("not reachable point\n");
        return;
    }
    // find closet configuration
    closestq(inversesolutions,solutionnum,currentq,result);
}

int changeparame(float Q[6])
{
    setparame(Q);
    //printf("current paramet is %f,%f,%f,%f,%f,%f\n",Q[0],Q[1],Q[2],Q[3],Q[4],Q[5]);
    return 1;
}
