#include "pid.h"

pid_base_t::pid_base_t()
{
    next = nullptr;
}

pid_t::pid_t(float kp, float ki, float kd, float target ,float integralLimit) : pid_base_t()
{
    this -> kp = kp;
    this -> ki = ki;
    this -> kd = kd;
    this -> target = target;
    this -> integralLimit = integralLimit;
    this -> integral = 0.0f;
}

pid_base_t& pid_base_t::operator<<(pid_base_t& other)
{
    next = &other;
    return other;
}

float pid_t::update(float current)
{
    float error = this -> target - current;
    this -> integral += error;
    float derivative = (error - this -> lastError);
    if(this -> integral > this -> integralLimit)
    {
        this -> integral = this -> integralLimit;
    }
    else if(this -> integral < -this -> integralLimit)
    {
        this -> integral = -this -> integralLimit;
    }
    float uk = (this -> kp * error) + (this -> ki * this -> integral) + (this -> kd * derivative);
    this -> lastError = error;
    if(this -> next != nullptr)
    {
        this -> next -> setTarget(uk);
    }
    return uk;
}

void pid_t::setTarget(float target)
{
    this -> target = target;
    this -> lastError = 0.0f;
    this -> integral = 0.0f;
}

/* float pi_t::update(float value)
{

}

float pd_t::update(float value)
{

}

float p_t::update(float value)
{

} */
