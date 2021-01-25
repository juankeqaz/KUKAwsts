#ifndef MATHTYPE_H
#define MATHTYPE_H
//const
//#define PI 3.14159265358979323846
//#define PI_1 0.31830988618379067154 // 1/pi
#ifndef RAD2DEG
#define RAD2DEG 57.295779513082323
#define DEG2RAD 0.017453292519943
#endif

#define JOINT_NUM 6
//#define MAX_DELTA_Q 5 // 单关节一个插值周期最大转角
#define EPS 1e-6 // tolerance for numerical error
#define VTYPE float



//function declaration
// -- vec3 --
void V_assign(const VTYPE* src, VTYPE* des, const unsigned int dimension);
void V_add_V(const VTYPE* V1, const VTYPE* V2, VTYPE* des, const unsigned int dimension);
void V_sub_V(const VTYPE* V1, const VTYPE* V2, VTYPE* des, const unsigned int dimension);
void V_add_S(const VTYPE* V, const VTYPE s, VTYPE* des, const unsigned int dimension);
void V_mul_S(const VTYPE* V, const VTYPE s, VTYPE* des, const unsigned int dimension);
VTYPE V_norm(const VTYPE* V,const unsigned int dimension);
void V_dir(const VTYPE* V, VTYPE* des,const unsigned int dimension);
VTYPE V_dot_pro(const VTYPE* V1, const VTYPE* V2,const unsigned int dimension);
void V_cross_pro(const VTYPE V1[3], const VTYPE V2[3], VTYPE des[3]);
VTYPE V_abs(const VTYPE* V,const unsigned int dimension);
void V_print(const VTYPE* V,const unsigned int dimension);


// -- R --
void R_from_AA(const AxisAngle* AA, VTYPE R[3][3]);
void R_from_axis_angle(const VTYPE axis[3], const VTYPE angle, VTYPE R[3][3]);
void R_to_AA(const VTYPE R[3][3], AxisAngle* AA);
void R_assign(const VTYPE src[3][3], VTYPE des[3][3]);
void R_mul_R(const VTYPE R1[3][3], const VTYPE R2[3][3], VTYPE des[3][3]);
void R_mul_V(const VTYPE R[3][3], const VTYPE V[3], VTYPE des[3]);
void R_inv(const VTYPE src[3][3], VTYPE des[3][3]);
VTYPE R_abs(const VTYPE R[3][3]);
void R_print(const VTYPE R[3][3]);
void R_to_EulerAngle(const VTYPE R[3][3],VTYPE* eulerangle);

// -- AxisAngle --
void AA_print(const AxisAngle* AA);
// -- T --
void T_from_RP(const VTYPE R[3][3], const VTYPE P[3], Transformation* des);
void T_to_R(const Transformation* T, VTYPE des[3][3]);
void T_to_P(const Transformation* T, VTYPE des[3]);
void T_assign(const Transformation* src, Transformation* des);
void T_mul_V(const Transformation* T, const VTYPE V[3], VTYPE des[3]);
void T_mul_T(const Transformation* T1,const Transformation* T2, Transformation* des);
void T_inv(const Transformation* src, Transformation* des);
VTYPE T_abs(const Transformation* T);
void T_print(const Transformation* T);
void T_to_Pose(const Transformation* T,Pose* pose);
void Pose_to_T(const Pose* pose,Transformation* T);
//}
#endif // MATHTYPE_H
