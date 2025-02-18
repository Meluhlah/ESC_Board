#ifndef PHASE_H
#define PHASE_H

#pragma once

#include "main.h"

class Phase
{
public:
    Phase(TIM_HandleTypeDef* high_mos_tim, uint16_t high_mos_tim_ch, volatile uint32_t* ccrx, GPIO_TypeDef* low_mos_port, uint16_t low_mos_pin);
    ~Phase();

    void set_pwm(uint32_t duty_cycle);
    void phase_off();
    void phase_on();

private:
    // TODO: Change pointer names to pXXXXX
    
    TIM_HandleTypeDef*  high_mos_tim;
    uint16_t            high_mos_tim_ch;
    volatile uint32_t*  ccrx;
    
    GPIO_TypeDef*       low_mos_port;
    uint16_t            low_mos_pin;

    uint8_t             pwm_duty_cycle;
};

#endif