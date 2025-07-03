#ifndef PID_H
#define PID_H

class pid_base_t
{
    protected:
        float target;
        pid_base_t * next;
    public:
        pid_base_t();
        virtual void setTarget(float target) = 0;
        virtual float update(float value) = 0;
        pid_base_t& operator<<(pid_base_t& other);
};

class p_base_t
{
    protected:
        float kp;
};

class i_base_t
{
    protected:
        float ki;
        float integral;
        float integralLimit;
};

class d_base_t
{
    protected:
        float kd;
        float lastError;
};

class pid_t : public pid_base_t, public p_base_t, public i_base_t, public d_base_t
{
    public:
        pid_t(float kp, float ki, float kd, float target ,float integralLimit);
        float update(float current);
        void setTarget(float target);
};

class p_t : public pid_base_t, public p_base_t
{
    public:
        p_t(float kp, float target);
        float update(float current);
};

class pi_t : public pid_base_t, public p_base_t, public i_base_t
{
    public:
        float update(float current);
};

class pd_t : public pid_base_t, public p_base_t, public d_base_t
{
    public:
        float update(float current);
};


#endif
