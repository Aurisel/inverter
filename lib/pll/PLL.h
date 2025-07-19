#ifndef __PLL_H
#define __PLL_H

#include "arm_math.h"

#define FSW 20e3

#define ssrf_ts (1.0f/FSW)
#define ssrf_kp 1.007f
#define sser_ki 0.006f
#define ssrf_up_limt 58
#define ssrf_low_limt 42
#define value_2pi 2.0f * 3.1415926f

//[0.003405429629029,0,-0.003405429629029]
//[0.010982881827120,0,-0.010982881827120]
#define SIGO_U_GAIN 1.0f
#define SIGO_U_B0 0.01098288182712f
#define SIGO_U_B1 0.0f
#define SIGO_U_B2 -0.010982881827120f

//[1,-1.993165860418354,0.993189140741942]
//[1,-1.977790221205283,0.978034236345759]
#define SIGO_U_A1 -1.977790221205283
#define SIGO_U_A2 0.978034236345759

// qu coeff [8.229594388365577e-06,1.645918877673115e-05,8.229594388365577e-06]
//[8.625935215831610e-05,1.725187043166322e-04,8.625935215831610e-05]
#define SIGO_QU_GAIN 1.0e-03
#define SIGO_QU_B0 0.08625935215831610f
#define SIGO_QU_B1 0.1725187043166322f
#define SIGO_QU_B2 0.08625935215831610f

// q coef  [1,-1.993165860418354,0.993189140741942]
//[1,-1.977790221205283,0.978034236345759]
#define SIGO_QU_A1 -1.977790221205283f
#define SIGO_QU_A2 0.978034236345759f
//
// Const data define
//
typedef struct DIS_2ORDER_TF_COEF_TAG
{
    float gain;
    float B0;
    float B1;
    float B2;
    float A1;
    float A2;
} DIS_2ORDER_TF_COEF_DEF;

typedef struct DIS_2ORDER_TF_DATA_TAG
{
    float output;
    float w0;
    float w1;
    float w2;
} DIS_2ORDER_TF_DATA_DEF;

//
// SOGI PLL date struct
//
typedef struct SOGI_PLL_DATA_STRUCT_TAG
{
    float ac_u;
    float ac_qu;
    float u_q;
    float u_d;
    float theta;
    float theta_1;
    float cos_theta;
    float sin_theta;
    float grid_freq;
    float pll_freq_out;
    float delta_t;

    DIS_2ORDER_TF_COEF_DEF sogi_u_coeff;
    DIS_2ORDER_TF_DATA_DEF sogi_u_data;

    DIS_2ORDER_TF_COEF_DEF sogi_qu_coeff;
    DIS_2ORDER_TF_DATA_DEF sogi_qu_data;

    float spll_kp;
    float spll_ki;
    float spll_integrator;
    float spll_freq_min_limt;
    float spp_freq_max_limt;
} SOGI_PLL_DATA_DEF;

void sogi_pll_init(SOGI_PLL_DATA_DEF *spll_obj, float grid_freq, float ts);
float discrete_2order_tf(const float input, DIS_2ORDER_TF_COEF_DEF *coeff, DIS_2ORDER_TF_DATA_DEF *data);
void spll_sogi_func(SOGI_PLL_DATA_DEF *spll_obj, float grid_volt_sen);


#endif
