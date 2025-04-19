#ifndef __PID_H_
#define __PID_H_

typedef struct{

    float kp;
    float ki;
    float kd;

    float tau;

    float limMax;
    float limMin;

    float Time;

    float integrator;
    float prevError;
    float differentiator;
    float prevMeasurement;

    float out;


} PidController_t;

void pidControllerInit(PidController_t* pid, float kp, float ki, float kd);

float pidControllerUpdate(PidController_t* pid, float setPoint, float measurement);

#endif  // __PID_H_