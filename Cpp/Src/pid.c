#include "pid.h"

void pidControllerInit(PidController_t* pid ,float kp, float ki, float kd){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->differentiator = 0.0f;
    pid->integrator = 0.0f;

    pid->limMax = 0.0f;
    pid->limMin = 0.0f;

    pid->prevError = 0.0f;
    pid->prevMeasurement = 0.0f;
    
    pid->tau = 0.0f;
    pid->Time = 0.0f;
    
    pid->out = 0.0f;

}

float pidControllerUpdate(PidController_t* pid, float setPoint, float measurement){

    float error = setPoint - measurement;


    float proportional = pid->kp * error;

    pid->integrator = pid->integrator + (pid->ki * pid->Time / 2.0f) * (error + pid->prevError);

    float limMin;
    float limMax;

    if(pid->limMax > proportional){

        limMax = pid->limMax - proportional;

    }
    else{

        limMax = 0.0f;

    }

    if(pid->limMin < proportional){

        limMin = pid->limMin - proportional;

    }
    else{

        limMin = 0.0f;

    }

    if(pid->integrator > limMax){

        pid->integrator = limMax;

    }

    else if(pid->integrator < limMin){

        pid->integrator = limMin;

    }

    // TODO: Add Derivative function.

    pid->out = proportional + pid->integrator + pid->differentiator;

    if(pid->out > pid->limMax){

        pid->out = pid->limMax;

    }

    else if(pid->out < pid->limMin){

        pid->out = pid->limMin;

    }
    
    pid->prevError = error;
    return pid->out;

}