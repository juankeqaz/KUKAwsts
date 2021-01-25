#include "mathtype.h"
#include "math.h"
#include "stdio.h"

void V_assign(const VTYPE* src, VTYPE* des,const unsigned int dimension)
{
    for(unsigned int i=0;i<dimension;++i)
    {
        des[i] = src[i];
    }
}

void V_add_V(const VTYPE* V1, const VTYPE* V2, VTYPE* des, const unsigned int dimension)
{
    for(unsigned int i=0;i<dimension;++i)
    {
        des[i] = V1[i] + V2[i];
    }
}

void V_sub_V(const VTYPE* V1, const VTYPE* V2, VTYPE* des,const unsigned int dimension)
{
    for(unsigned int i=0;i<dimension;++i)
    {
        des[i] = V1[i] - V2[i];
    }
}

void V_add_S(const VTYPE* V, const VTYPE s, VTYPE* des, const unsigned int dimension)
{
    for(unsigned int i=0;i<dimension;++i)
    {
        des[i] = V[i] + s;
    }
}

void V_mul_S(const VTYPE* V, const VTYPE s, VTYPE* des,const unsigned int dimension)
{
    for(unsigned int i=0;i< dimension;++i)
    {
        des[i] = V[i] * s;
    }
}

VTYPE V_norm(const VTYPE* V,const unsigned int dimension)
{
    VTYPE sum_square = 0;
    for(unsigned int i=0;i<dimension;++i)
    {
        sum_square += V[i] * V[i];
    }
    return sqrt(sum_square);
}

void V_dir(const VTYPE* V, VTYPE* des, const unsigned int dimension)
{
    V_mul_S(V,1/V_norm(V,dimension),des, dimension);
}

VTYPE V_dot_pro(const VTYPE* V1, const VTYPE* V2,const unsigned int dimension)
{
    VTYPE result = 0;
    for(unsigned int i=0;i<dimension;++i)
    {
        result += V1[i] * V2[i];
    }
    return result;
}

void V_cross_pro(const  VTYPE V1[3], const VTYPE V2[3], VTYPE des[3])
{
    des[0] = V1[1]*V2[2] - V1[2]*V2[1];
    des[1] = V1[2]*V2[0] - V1[0]*V2[2];
    des[2] = V1[0]*V2[1] - V1[1]*V2[0];
}

VTYPE V_abs(const VTYPE* V,const unsigned int dimension)
{
    VTYPE result = 0;
    for(unsigned int i=0;i<dimension;++i)
    {
        result += fabs(V[i]);
    }
    return result;
}

void V_print(const VTYPE* V,const unsigned int dimension)
{
    printf("[ ");
    for(unsigned int i=0;i<dimension;++i)
    {
        printf("%lf ",V[i]);
    }
    printf("]T\n");
}

void R_from_AA(const AxisAngle *AA, VTYPE R[3][3])
{
    VTYPE sinq = sin(AA->angle), cosq = cos(AA->angle);
    VTYPE one_cosq = 1-cosq;
    VTYPE kx = AA->axis[0], ky = AA->axis[1], kz = AA->axis[2];
    VTYPE a = kx*one_cosq, b = ky*one_cosq, c = kz*one_cosq,
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

void R_from_axis_angle(const VTYPE axis[3], const VTYPE angle, VTYPE R[3][3])
{
    VTYPE sinq = sin(angle), cosq = cos(angle);
    VTYPE one_cosq = 1-cosq;
    VTYPE kx = axis[0], ky = axis[1], kz = axis[2];
    VTYPE a = kx*one_cosq, b = ky*one_cosq, c = kz*one_cosq,
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

void R_to_AA(const VTYPE R[3][3], AxisAngle* AA)
{
    VTYPE cosq = 0.5*(R[0][0] + R[1][1] + R[2][2] - 1);
    if(cosq > 1)
        cosq = 1;
    if(cosq < -1)
        cosq = -1;
    AA->angle = acos(cosq);
    //printf("r to aa axis:\n");
    //printf("%lf,%lf,%lf\n",AA->axis[0],AA->axis[1],AA->axis[2]);
    if(fabs(AA->angle)<EPS) // angle close to zero
    {
        //printf("angle close to 0\n");
        AA->angle = 0;
        for(unsigned int i=0;i<3;++i)
        {
            AA->axis[i] = 0;
        }
        return;
    }
    else if(fabs(AA->angle-M_PI)<EPS) // angle close to 180 degree
    {
        //printf("angle close to pi\n");
        AA->angle = M_PI;
        VTYPE magnitude[3];
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
            VTYPE v1v2 = R[0][1] + R[1][0];
            VTYPE v1v3 = R[0][2] + R[2][0];
            if(v1v2 < 0.)
                magnitude[1] *= -1;
            if(v1v3 < 0.)
                magnitude[2] *= -1;
        }
        else if(maxind == 1)
        {
            VTYPE v1v2 = R[0][1] + R[1][0];
            VTYPE v2v3 = R[1][2] + R[2][1];
            if(v1v2 < 0.)
                magnitude[0] *= -1;
            if(v2v3 < 0.)
                magnitude[2] *= -1;
        }
        else if(maxind == 2){
            VTYPE v1v3 = R[0][2] + R[2][0];
            VTYPE v2v3 = R[1][2] + R[2][1];
            if(v1v3 < 0.)
                magnitude[0] *= -1;
            if(v2v3 < 0.)
                magnitude[1] *= -1;
        }
        V_assign(magnitude,AA->axis,3);
        return;
    }

    // general case
    VTYPE axis[3];
    axis[0] = R[2][1] - R[1][2];
    axis[1] = R[0][2] - R[2][0];
    axis[2] = R[1][0] - R[0][1];
    //printf("no normalized axis:\n");
    //V_print(axis,3);
    V_dir(axis,AA->axis,3);
}

void R_assign(const VTYPE src[3][3], VTYPE des[3][3])
{
    for(unsigned int i=0;i<3;++i)
    {
        for(unsigned int j=0;j<3;++j)
        {
            des[i][j] = src[i][j];
        }
    }
}

void R_mul_R(const VTYPE R1[3][3], const VTYPE R2[3][3], VTYPE des[3][3])
{

    for(unsigned int i=0;i<3;++i)
    {
        for(unsigned int j=0;j<3;++j)
        {
            VTYPE sum = 0;
            for(unsigned int k=0;k<3;++k)
            {
                sum += R1[i][k] * R2[k][j];
            }
            des[i][j] = sum;
        }
    }
}

void R_mul_V(const VTYPE R[3][3], const VTYPE V[3], VTYPE des[3])
{
    for(unsigned int i=0;i<3;++i)
    {
        VTYPE sum = 0;
        for(unsigned int j=0;j<3;++j)
        {
            sum += R[i][j] * V[j];
        }
        des[i] = sum;
    }
}

void R_inv(const VTYPE src[3][3], VTYPE des[3][3])
{
    for(unsigned int i=0;i<3;++i)
    {
        for(unsigned int j=0;j<3;++j)
        {
            des[i][j] = src[j][i];
        }
    }
}

VTYPE R_abs(const VTYPE R[3][3])
{
    VTYPE result = 0;
    for(unsigned int i=0;i<3;++i)
    {
        for(unsigned int j=0;j<3;++j)
        {
            result += fabs(R[i][j]);
        }
    }
    return result;
}

void R_print(const VTYPE R[3][3])
{
    printf("Rotation:\n");
    printf("[ ");
    for(unsigned int i=0;i<3;++i)
    {
        for(unsigned int j=0;j<3;++j)
        {
            printf("%lf ",R[i][j]);
        }
        if(i == 2)
        {
            printf("]\n");
        }
        else {
            printf("\n  ");
        }
    }
}

void AA_print(const AxisAngle *AA)
{
    printf("Axis Angle:\n");
    printf("Axis:\n[ ");
    for(unsigned int i=0;i<3;++i)
    {
        printf("%lf ",AA->axis[i]);
    }
    printf("]\n");
    printf("Angle:\n%lf deg\n",AA->angle*RAD2DEG);
}

void T_from_RP(const VTYPE R[3][3], const VTYPE P[3], Transformation* des)
{
    R_assign(R,des->rotation);
    V_assign(P,des->translation,3);
}

void T_to_R(const Transformation* T, VTYPE des[3][3])
{
    R_assign(T->rotation,des);
}

void T_to_P(const Transformation* T, VTYPE des[3])
{
    V_assign(T->translation,des,3);
}

void T_assign(const Transformation* src, Transformation* des)
{
    R_assign(src->rotation,des->rotation);
    V_assign(src->translation,des->translation,3);
}

void T_mul_V(const Transformation* T, const VTYPE V[3], VTYPE des[3])
{
    VTYPE temp[3];
    R_mul_V(T->rotation,V,temp);
    V_add_V(temp,T->translation,des,3);
}

void T_mul_T(const Transformation* T1,const Transformation* T2, Transformation* des)
{
    R_mul_R(T1->rotation,T2->rotation,des->rotation);
    T_mul_V(T1,T2->translation,des->translation);
}

void T_inv(const Transformation *src, Transformation *des)
{
    VTYPE RT[3][3],temp[3];
    R_inv(src->rotation,RT);
    R_mul_V(RT,src->translation,temp);
    R_assign(RT,des->rotation);
    V_mul_S(temp,-1,des->translation,3);
}

VTYPE T_abs(const Transformation *T)
{
    return R_abs(T->rotation) + V_abs(T->translation,3);
}

void T_print(const Transformation *T)
{
    printf("Transformation:\n");
    R_print(T->rotation);
    V_print(T->translation,3);
}

void T_to_Pose(const Transformation *T, Pose *pose)
{
    pose->x = T->translation[0]*1000;
    pose->y = T->translation[1]*1000;
    pose->z = T->translation[2]*1000;
    R_to_EulerAngle(T->rotation,pose+3);
}

void R_to_EulerAngle(const VTYPE R[3][3], VTYPE *eulerangle)
{
    VTYPE rx,ry,rz;
    VTYPE sy = sqrt(R[0][0]*R[0][0]+R[1][0]*R[1][0]);
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
    eulerangle[0] = rx;
    eulerangle[1] = ry;
    eulerangle[2] = rz;
}

void Pose_to_T(const Pose *pose, Transformation *T)
{
    VTYPE km=pose->alpha;//*M_PI/180;
    VTYPE bt=pose->beta;//*M_PI/180;
    VTYPE af=pose->gamma;//*M_PI/180;

    VTYPE nx=cos(af)*cos(bt);VTYPE ox=(cos(af)*sin(bt)*sin(km))-(sin(af)*cos(km));VTYPE ax=(cos(af)*sin(bt)*cos(km))+(sin(af)*sin(km));
    VTYPE ny=sin(af)*cos(bt);VTYPE oy=(sin(af)*sin(bt)*sin(km))+(cos(af)*cos(km));VTYPE ay=(sin(af)*sin(bt)*cos(km))-(cos(af)*sin(km));
    VTYPE nz=-sin(bt);VTYPE oz=cos(bt)*sin(km);VTYPE az=cos(bt)*cos(km);
//    std::cout<<"******************************************"<<std::endl;
//    std::cout<<nx<<" "<<ox<<" "<<ax<<std::endl;
//    std::cout<<ny<<" "<<oy<<" "<<ay<<std::endl;
//    std::cout<<nz<<" "<<oz<<" "<<az<<std::endl;
//    std::cout<<"******************************************"<<std::endl;

    T->rotation[0][0] = nx; T->rotation[0][1] = ox; T->rotation[0][2] = ax;
    T->rotation[1][0] = ny; T->rotation[1][1] = oy; T->rotation[1][2] = ay;
    T->rotation[2][0] = nz; T->rotation[2][1] = oz; T->rotation[2][2] = az;
    T->translation[0] = pose->x/1000.0;
    T->translation[1] = pose->y/1000.0;
    T->translation[2] = pose->z/1000.0;
}
