#ifndef PHASE_H
#define PHASE_H

#pragma once

#include "main.h"

class Phase
{
public:
    Phase(TIM_HandleTypeDef* timer, uint16_t timer_ch, volatile uint32_t* ccrx, GPIO_TypeDef* low_mos_port, uint16_t low_pin);
    ~Phase();

    void set_pwm(uint32_t duty_cycle);
    void off();
    void high_on();
    void low_on();
    uint32_t get_duty_cycle();


private:
    // TODO: Change pointer names to pXXXXX

    TIM_HandleTypeDef   *timer;
    const uint16_t      channel;
    volatile uint32_t*  ccrx;
    
    GPIO_TypeDef*       low_port;
    const uint16_t      low_pin;
};

#endif