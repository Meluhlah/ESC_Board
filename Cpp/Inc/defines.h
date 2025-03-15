#ifndef _DEFINES_H
#define _DEFINES_H

// Motor Specs Defines *********************** //
#define MOTOR_PARAM_POLE_PAIRS          (uint8_t)7
#define MOTOR_PARAM_KV                  (uint16_t)1000  //D2830

// Motor Align Defines *********************** //
#define ALIGN_PWM_INIT                  (uint8_t)15
#define ALIGN_PWM_TARGET                (uint8_t)30
#define ALIGN_STEPS_NUM                 (uint8_t)35
#define ALIGN_STEP_DURATION_MS          (uint8_t)30
#define ALIGN_PWM_INCREMENT_PER_STEP    (uint8_t)1
// #define ALIGN_PWM_INCREMENT_PER_STEP    (uint8_t)((uint16_t)((ALIGN_PWM_TARGET - ALIGN_PWM_INIT) / (ALIGN_STEPS_NUM)))

// Motor Ramp Defines *********************** //
#define RAMP_PWM_INIT                   (uint8_t)(ALIGN_PWM_TARGET)
#define RAMP_STEPS_NUM                  (uint16_t)40
#define RAMP_STEP_INIT_DURATION_US      (uint16_t)1000
#define RAMP_STEP_TARGET_DURATION_US    (uint16_t)100
#define RAMP_STEP_DECREMENT_PER_STEP_US (uint16_t)((RAMP_STEP_INIT_DURATION_US - RAMP_STEP_TARGET_DURATION_US) / RAMP_STEPS_NUM)
#define RAMP_ZC_DET_DISABLE_THRESHOLD   (uint8_t)10
#define RAMP_ZC_DET_ENABLE_STEP         (uint8_t)((MOTOR_PARAM_POLE_PAIRS * 6) - RAMP_ZC_DET_DISABLE_THRESHOLD)
#define RAMP_ZC_DETECTION_THRESHOLD     (uint8_t)3


// Delay Timer        *********************** //
#define DELAY_TIMER                     htim14

typedef enum {

    MOTOR_STATE_IDLE,
    MOTOR_STATE_ALIGN,
    MOTOR_STATE_RAMP,
    MOTOR_STATE_AUTO_COMMUTATION, 

} MOTOR_STATE_e;

typedef enum{

    NO_ERROR,
    ERROR_INVALID_COMMUTATION_STEP,
    ERROR_OVERCURRENT,
    ERROR_MOTOR_LOCKED,
    ERROR_OVER_TEMPERATURE,
    ERROR_LOW_BATTERY

} ERROR_e;


#endif // _DEFINES_H