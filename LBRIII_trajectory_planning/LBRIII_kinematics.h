#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "KinematicConstants.h"
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <matrix/Matrix.hpp>

class LBR
{
public:
    explicit LBR()
    {

    };


void LBR_fk(const float Q[6], Transformation *T);
/**
 * @brief LBR_ik
 * @param T
 * @param solutionnum
 * @param inversesolutions
 */
void LBR_ik(const Transformation* T, const QVector<float> currentq, unsigned int*solutionnum, float inversesolutions[8][6]);
/**
 * @brief detjacobianinfo
 * @param Q
 * @param isclosetozero
 * @param sign
 */
void detjacobianinfo(const float Q[6],bool* isclosetozero,int* sign);
/**
 * @brief solve equation a*sin(theta)+b*cos(theta)=c
 * @param sin_cos_pairs
 * @param a
 * @param b
 * @param c
 * @return solution number
 */
unsigned char phase_shift_eq(float sin_cos_pairs[2][2], const float a, const float b, const float c);
/**
 * @brief ||Q1-Q2||
 * @param Q1
 * @param Q2
 * @return
 */
float caldistance(const float Q1[6], const float Q2[6]);
/**
 * @brief handle numeric error when sin or cos value is close to limits
 * @param value
 */
void sincosround(float* value);
void setparame(const float P[6]);
void DH2T(const float theta, const float d,const float a,const float alpha, QMatrix4x4 &matrix);
QMatrix3x3 getmatrix33(const float a1,const float a2,const float a3,const float a4,const float a5,const float a6,const float a7,const float a8,const float a9);
};

#endif // KINEMATICS_H
