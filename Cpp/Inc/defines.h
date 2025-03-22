#ifndef _DEFINES_H
#define _DEFINES_H

#include "ws2812b.h"

// Motor Specs Defines *********************** //
#define MOTOR_PARAM_POLE_PAIRS          (uint8_t)7
#define MOTOR_PARAM_KV                  (uint16_t)1000  //D2830

// Motor General Defines ********************* //
#define MOTOR_MAX_PWM                   (uint16_t)100


// Motor Align Defines *********************** //
#define ALIGN_PWM_INIT                  (uint8_t)5      
#define ALIGN_PWM_TARGET                (uint8_t)20
#define ALIGN_STEPS_NUM                 (uint8_t)40
#define ALIGN_STEP_DURATION_MS          (uint8_t)30
#define ALIGN_PWM_INCREMENT_PER_STEP    (uint8_t)1
// #define ALIGN_PWM_INCREMENT_PER_STEP    (uint8_t)((uint16_t)((ALIGN_PWM_TARGET - ALIGN_PWM_INIT) / (ALIGN_STEPS_NUM)))

// Motor Ramp Defines *********************** //
#define RAMP_PWM_INIT                   (uint8_t)(ALIGN_PWM_TARGET)
#define RAMP_STEPS_NUM                  (uint16_t)50
#define RAMP_STEP_INIT_DURATION_US      (uint16_t)2000
#define RAMP_STEP_TARGET_DURATION_US    (uint16_t)100
#define RAMP_STEP_DECREMENT_PER_STEP_US (uint16_t)((RAMP_STEP_INIT_DURATION_US - RAMP_STEP_TARGET_DURATION_US) / RAMP_STEPS_NUM)
#define RAMP_ZC_DET_DISABLE_THRESHOLD   (uint8_t)10
#define RAMP_ZC_DET_ENABLE_STEP         (uint8_t)((MOTOR_PARAM_POLE_PAIRS * 6) - RAMP_ZC_DET_DISABLE_THRESHOLD)
#define RAMP_ZC_DETECTION_THRESHOLD     (uint8_t)5

// AUTO COMMUTATION Defines ***************** //

#define TEST_ZC_MEASUREMENT_DELAY       (uint8_t)(40)   //TODO: Add Motorspeed to calculation, delay decrease with speed increasing.



// Delay Timer        *********************** //
#define DELAY_TIMER                     htim14


/* DEBUGGING **********************************/
// #define DEBUGGING


typedef enum {

    MOTOR_STATE_IDLE,
    MOTOR_STATE_ALIGN,
    MOTOR_STATE_RAMP,
    MOTOR_STATE_AUTO_COMMUTATION, 

} MotorState_e;

typedef enum{

    NO_ERROR,
    ERROR_INVALID_COMMUTATION_STEP,
    ERROR_OVERCURRENT,
    ERROR_MOTOR_LOCKED,
    ERROR_OVER_TEMPERATURE,
    ERROR_LOW_BATTERY

} Error_e;

typedef enum{
    DIRECTION_ABC,
    DIRECTION_CBA
} Direction_e;

typedef enum{

    CMD_IDLE,
    CMD_MOTOR_START,
    CMD_MOTOR_STOP,
    CMD_MOTOR_ALIGN,
    CMD_MOTOR_SET_SPEED,
    CMD_FLASH_WRITE,
    CMD_FLASH_READ,
    CMD_LED_SET_RED,
    CMD_LED_SET_GREEN,
    CMD_LED_SET_BLUE,
    CMD_LED_SET_RGB

} UartCommands_e;

typedef struct __attribute__((packed)){

    uint8_t         slaveId;
    uint8_t         dutyCycle;
    uint8_t         temp;   //Temperature
    uint16_t        vin;
    Direction_e     direction;
    MotorState_e    state;
    Error_e         error;
    uint32_t        flashParameterVal;
    led_t           led_rgb;
    uint8_t         hallSensors[3]; //Optional
    uint32_t        checksum;

}   UartPacketTx_t;

typedef struct __attribute__((packed)){

    uint8_t         slaveId;
    UartCommands_e  command;
    uint8_t         flash_mem_adrs;
    uint16_t        data[3];
    uint32_t        checksum;

}   UartPacketRx_t;


#endif // _DEFINES_H