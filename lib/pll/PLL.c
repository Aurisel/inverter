#include "PLL.h"




//extern float deta_theta;
//extern AC_DC_CTRL_DEF g_acdc_ctrlpar;
//extern PI_CTRL_DATA_DEF vloop_pi;
//extern PI_CTRL_DATA_DEF iloop_pi;
//extern Vofa_HandleTypedef vofa1;
//extern SOGI_PLL_DATA_DEF spll_data;

// Function description :
void sogi_pll_init(SOGI_PLL_DATA_DEF *spll_obj, float grid_freq, float ts)
{
    spll_obj->grid_freq = grid_freq;
    spll_obj->delta_t = ts;

    spll_obj->sogi_u_coeff.gain = SIGO_U_GAIN;
    spll_obj->sogi_u_coeff.B0 = SIGO_U_B0;
    spll_obj->sogi_u_coeff.B1 = SIGO_U_B1;
    spll_obj->sogi_u_coeff.B2 = SIGO_U_B2;

    spll_obj->sogi_u_coeff.A1 = SIGO_U_A1;
    spll_obj->sogi_u_coeff.A2 = SIGO_U_A2;

    spll_obj->sogi_qu_coeff.gain = SIGO_QU_GAIN;
    spll_obj->sogi_qu_coeff.B0 = SIGO_QU_B0;
    spll_obj->sogi_qu_coeff.B1 = SIGO_QU_B1;
    spll_obj->sogi_qu_coeff.B2 = SIGO_QU_B2;

    spll_obj->sogi_qu_coeff.A1 = SIGO_QU_A1;
    spll_obj->sogi_qu_coeff.A2 = SIGO_QU_A2;

    spll_obj->spll_kp = ssrf_kp;
    spll_obj->spll_ki = sser_ki;
    spll_obj->spp_freq_max_limt = ssrf_up_limt;
    spll_obj->spll_freq_min_limt = ssrf_low_limt;
}

float discrete_2order_tf(const float input, DIS_2ORDER_TF_COEF_DEF *coeff, DIS_2ORDER_TF_DATA_DEF *data)
{

    data->w0 = input - coeff->A1 * data->w1 - coeff->A2 * data->w2;

    data->output = coeff->gain * (coeff->B0 * data->w0 + coeff->B1 * data->w1 + coeff->B2 * data->w2);

    data->w2 = data->w1;
    data->w1 = data->w0;

    return (data->output);
}

// sogo pll
void spll_sogi_func(SOGI_PLL_DATA_DEF *spll_obj, float grid_volt_sen)
{
    // signal genertor
    spll_obj->ac_u = discrete_2order_tf(grid_volt_sen, &(spll_obj->sogi_u_coeff), &(spll_obj->sogi_u_data));
    spll_obj->ac_qu = discrete_2order_tf(grid_volt_sen, &(spll_obj->sogi_qu_coeff), &(spll_obj->sogi_qu_data));

    // Park transfrom from alpha beta tp d-q axis
    spll_obj->u_d = spll_obj->ac_u * spll_obj->cos_theta + spll_obj->ac_qu * spll_obj->sin_theta;
    spll_obj->u_q = -spll_obj->ac_u * spll_obj->sin_theta + spll_obj->ac_qu * spll_obj->cos_theta;

    // pi ctrol q to 0
    float spll_diff = 0 - spll_obj->u_q;

    spll_obj->spll_integrator += spll_diff * spll_obj->spll_ki;

    spll_obj->pll_freq_out = spll_diff * spll_obj->spll_kp + spll_obj->spll_integrator;

    spll_obj->theta -= (spll_obj->pll_freq_out + 0.5f) * value_2pi * spll_obj->delta_t;

    if (spll_obj->theta > value_2pi)
    {
        spll_obj->theta -= value_2pi;
    }
    else if (spll_obj->theta < 0)
    {
        spll_obj->theta += value_2pi;
    }

    spll_obj->cos_theta = arm_cos_f32(spll_obj->theta);
    spll_obj->sin_theta = arm_sin_f32(spll_obj->theta);
}


