#include "LBRIII_kinematics.h"
#include <math.h>
#include <stdio.h>
//#include "status.h"
//#include <QVector>

void LBR::LBR_fk(const float Q[6], Transformation* T)
{
    float q1,q2,q3,q4,q5,q6;
    q1 = Q[0]*M_PI/180; q2 = Q[1]*M_PI/180; q3 = Q[2]*M_PI/180;
    q4 = Q[3]*M_PI/180; q5 = Q[4]*M_PI/180; q6 = Q[5]*M_PI/180;
    QMatrix4x4 T01,T12,T23,T34,T45,T56,T06;
    DH2T(q1,d0,d1,-M_PI/2,T01);
    DH2T(q2,0,d2,0,T12);
    DH2T(q3-M_PI/2,0,d3,M_PI/2,T23);
    DH2T(q4,-d4,0,-M_PI/2,T34);
    DH2T(q5,0,0,M_PI/2,T45);
    DH2T(q6,-d5,0,M_PI,T56);
    T06=T01*T12*T23*T34*T45*T56;

    T->rotation[0][0] = T06(0,0);
    T->rotation[1][0] = T06(1,0);
    T->rotation[2][0] = T06(2,0);
    T->rotation[0][1] = T06(0,1);
    T->rotation[1][1] = T06(1,1);
    T->rotation[2][1] = T06(2,1);
    T->rotation[0][2] = T06(0,2);
    T->rotation[1][2] = T06(1,2);
    T->rotation[2][2] = T06(2,2);
    T->translation[0] = T06(0,3);
    T->translation[1] = T06(1,3);
    T->translation[2] = T06(2,3);
}

void LBR::LBR_ik(const Transformation *T,const QVector<float> currentq, unsigned int*solutionnum,float inversesolutions[8][6])
{
    unsigned int currentsolutionnum = 0;
    float nx,ny,nz,ox,oy,oz,ax,ay,az,px,py,pz;
    nx = T->rotation[0][0]; ny = T->rotation[1][0]; nz = T->rotation[2][0];
    ox = T->rotation[0][1]; oy = T->rotation[1][1]; oz = T->rotation[2][1];
    ax = T->rotation[0][2]; ay = T->rotation[1][2]; az = T->rotation[2][2];
    px = T->translation[0]; py = T->translation[1]; pz = T->translation[2];

    QVector3D a=QVector3D(ax,ay,az);
    QVector3D p=QVector3D(px,py,pz);

    float solution[6] = {0,0,0,0,0,0};// pre_solution[6] = {10000,0,0,0,0,0};//d_Q = 10000;
    float sinq1,sinq2,sinq3,sinq5,sinq4,sinq6;
    float cosq1,cosq2,cosq3,cosq5,cosq4,cosq6;

    // ---------- solve q1 ----------
    float q1_sin_cos_pair[2][2];
    unsigned char q1_num = phase_shift_eq(q1_sin_cos_pair,-px+d5*ax,py-d5*ay,0);
    QVector3D p04=p-a*d5;

    if(!q1_num)
    {
        printf("no q1 solution\n");
        *solutionnum = 0;
        return; // no inverse solution
    }

    for(unsigned char i=0;i<q1_num;++i)
    {
        sinq1 = q1_sin_cos_pair[i][0];
        cosq1 = q1_sin_cos_pair[i][1];
        //----------solve q3------------
        QVector3D p01=QVector3D(d1*cosq1,d1*sinq1,d0);
        QVector3D p14=p04-p01;
        float q3_sin_cos_pair[2][2];
        unsigned char q3_num = phase_shift_eq(q3_sin_cos_pair,2*d2*d3,2*d2*d4,p14.lengthSquared()-d2*d2-d3*d3-d4*d4);
        if(!q3_num)
        {
            continue;
        }

        for(unsigned char j=0;j<q3_num;++j)
        {
            sinq3=q3_sin_cos_pair[j][0];
            cosq3=q3_sin_cos_pair[j][1];

            QVector3D p14_1=QVector3D((px-ax*d5)*cosq1+(py-ay*d5)*sinq1-d1,az*d5+d0-pz,(ax*d5-px)*sinq1+(py-ay*d5)*cosq1);
            float M=d4*cosq3+d3*sinq3+d2;
            float N=d3*cosq3-d4*sinq3;
            float b1=p14_1.x();
            float b2=p14_1.y();
            sinq2=(N*b1+M*b2)/(M*M+N*N);
            cosq2=(M*b1+N*b2)/(M*M+N*N);

            cosq5=-(((-cosq1*ax-sinq1*ay)*cosq3+sinq3*az)*cosq2+(az*cosq3+sinq3*(cosq1*ax+sinq1*ay))*sinq2);
            sinq5=sqrt(1-cosq5*cosq5);
            float q5_sin_cos_pair[2][2];
            q5_sin_cos_pair[0][0]=sinq5;
            q5_sin_cos_pair[1][0]=-sinq5;
            q5_sin_cos_pair[0][1]=cosq5;
            q5_sin_cos_pair[1][1]=cosq5;
            for(unsigned char k=0;k<2;++k)
            {
                sinq5=q5_sin_cos_pair[k][0];
                cosq5=q5_sin_cos_pair[k][1];
                if(abs(sinq5)<1e-4)
                {
                    sinq4=sin(currentq.at(3)*M_PI/180);
                    cosq4=cos(currentq.at(3)*M_PI/180);
                    float b1=-sinq1*nx+cosq1*ny;
                    float b2=-sinq1*ox+cosq1*oy;
                    sinq6=b1*cosq4+b2*sinq4*cosq5;
                    cosq6=b1*sinq4*cosq5-b2*cosq4;
                    if(cosq6<-1||cosq6>1||sinq6>1||sinq6<-1)
                    {
                        continue;
                    }

                }else
                {
                    float r13=(az*cosq3+sinq3*(cosq1*ax+sinq1*ay)*cosq2+sinq2*((cosq1*ax+sinq1*ay)*cosq3)-sinq3*az);
                    float r23=cosq1*ay-sinq1*ax;
                    float r31=((-cosq1*nx-sinq1*ny)*cosq3+sinq3*nz)*cosq2+(nz*cosq3+sinq3*(cosq1*nx+sinq1*ny))*sinq2;
                    float r32=((-cosq1*ox-sinq1*oy)*cosq3+sinq3*oz)*cosq2+(oz*cosq3+sinq3*(cosq1*ox+sinq1*oy))*sinq2;
                    sinq4=-r23/sinq5;
                    cosq4=-r13/sinq5;
                    sinq6=-r32/sinq5;
                    cosq6=-r31/sinq5;
                }
                solution[0] = atan2(sinq1,cosq1)*180/M_PI;
                solution[1] = atan2(sinq2,cosq2)*180/M_PI;
                solution[2] = atan2(sinq3,cosq3)*180/M_PI;
                solution[3] = atan2(sinq4,cosq4)*180/M_PI;
                solution[4] = atan2(sinq5,cosq5)*180/M_PI;
                solution[5] = atan2(sinq6,cosq6)*180/M_PI;
                for(unsigned int m=0;m<DOF;++m)
                {
                    inversesolutions[currentsolutionnum][m] = solution[m];
                }
                currentsolutionnum++;
            }

        }
    }
    *solutionnum = currentsolutionnum;
}

void LBR::detjacobianinfo(const float Q[6], bool *isclosetozero, int *sign)
{
    *isclosetozero = false;
    float q2 = Q[1]*M_PI/180,q3 = Q[2]*M_PI/180, q4 = Q[3]*M_PI/180, q5 = Q[4]*M_PI/180;
    float factor1 = sin(q3),factor2 = sin(q5),
            factor3 = d2*cos(q2)+d3*cos(q2+q3)-d5*sin(q2+q3-q4);
    if(fabs(factor1)<1e-5 || fabs(factor2)<1e-5 || fabs(factor3)<1e-5)
    {
        *isclosetozero = true;
    }
    int tempsign = -1;
    if(factor1 < 0)
    {
        tempsign *= -1;
    }
    if(factor2 < 0)
    {
        tempsign *= -1;
    }
    if(factor3 < 0)
    {
        tempsign *= -1;
    }
    *sign = tempsign;
}

unsigned char LBR::phase_shift_eq(float sin_cos_pairs[2][2], const float a, const float b, const float c)
{
    float denominator = a*a+b*b;
    float delta = denominator-c*c;
    if(fabs(delta)<1e-5)
    {
        float sinx = a*c/denominator;
        float cosx = b*c/denominator;
        sincosround(&sinx);
        sincosround(&cosx);
        sin_cos_pairs[0][0] = sinx;
        sin_cos_pairs[0][1] = cosx;
        //printf("one solution\n");
        return 1;
    }

    if(delta<0)
    {
        if(fabs(a-c)<1e-5)
        {
            sin_cos_pairs[0][0] = 1;
            sin_cos_pairs[0][1] = 0;
            return 1;
        }
        if(fabs(a+c)<1e-5)
        {
            sin_cos_pairs[0][0] = -1;
            sin_cos_pairs[0][1] = 0;
            return 1;
        }
        if(fabs(b-c)<1e-5)
        {
            sin_cos_pairs[0][0] = 0;
            sin_cos_pairs[0][1] = 1;
            return 1;
        }
        if(fabs(b+c)<1e-5)
        {
            sin_cos_pairs[0][0] = 0;
            sin_cos_pairs[0][1] = -1;
            return 1;
        }
        //printf("no solution\n");
        return 0;
    }

    float sqrt_delta = sqrt(delta);
    float sinx1,cosx1,sinx2,cosx2;
    sinx1 = (a*c+b*sqrt_delta)/denominator;
    cosx1 = (b*c-a*sqrt_delta)/denominator;
    sinx2 = (a*c-b*sqrt_delta)/denominator;
    cosx2 = (b*c+a*sqrt_delta)/denominator;
    sincosround(&sinx1); sincosround(&cosx1); sincosround(&sinx2); sincosround(&cosx2);
    sin_cos_pairs[0][0] = sinx1;
    sin_cos_pairs[0][1] = cosx1;
    sin_cos_pairs[1][0] = sinx2;
    sin_cos_pairs[1][1] = cosx2;
    //printf("two solution\n");
    return 2;
}

void LBR::sincosround(float *value)
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

float LBR::caldistance(const float *Q1, const float *Q2)
{
    float delta = fabs(Q1[0]-Q2[0]);
    float d = delta;

    for(unsigned int i=1;i<DOF;++i)
    {
        delta = fabs(Q1[i]-Q2[i]);
        if(delta>d)
        {
            d = delta;
        }
    }
    return d;
}

void LBR::setparame(const float P[6])
{

}

void LBR::DH2T(const float theta, const float d, const float a, const float alpha, QMatrix4x4& matrix)
{
    float ct=cos(theta);
    float ca=cos(alpha);
    float st=sin(theta);
    float sa=sin(alpha);
    matrix.setToIdentity();
    matrix(0,0)=ct;
    matrix(0,1)=-st*ca;
    matrix(0,2)=st*sa;
    matrix(0,3)=a*ct;
    matrix(1,0)=st;
    matrix(1,1)=ct*ca;
    matrix(1,2)=-ct*sa;
    matrix(1,3)=a*st;
    matrix(2,0)=0;
    matrix(2,1)=sa;
    matrix(2,2)=ca;
    matrix(2,3)=d;
}

QMatrix3x3 LBR::getmatrix33(const float a1, const float a2, const float a3, const float a4, const float a5, const float a6, const float a7, const float a8, const float a9)
{
    QMatrix3x3 temp;
    temp(0,0)=a1;
    temp(0,1)=a2;
    temp(0,2)=a3;
    temp(1,0)=a4;
    temp(1,1)=a5;
    temp(1,2)=a6;
    temp(2,0)=a7;
    temp(2,1)=a8;
    temp(2,2)=a9;
    return temp;
}
