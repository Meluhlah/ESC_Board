#include "Phase.h"


Phase::Phase(TIM_HandleTypeDef* high_mos_tim, uint16_t high_mos_tim_ch,
    volatile uint32_t* ccrx, GPIO_TypeDef* low_mos_port, uint16_t low_mos_pin)
{
    high_mos_tim = high_mos_tim;
    high_mos_tim_ch = high_mos_tim_ch;
    ccrx = ccrx;
    low_mos_port = low_mos_port;
    low_mos_pin = low_mos_pin;
    pwm_duty_cycle = 0;
    
}

Phase::~Phase()
{

}

void Phase::set_pwm(uint32_t duty_cycle)
{
    if(ccrx != nullptr)
    {
        *ccrx = duty_cycle;
    }
}

void Phase::phase_off()
{
    HAL_TIM_PWM_Stop(high_mos_tim, high_mos_tim_ch);
    HAL_GPIO_WritePin(low_mos_port, low_mos_pin, GPIO_PIN_RESET);
}

void Phase::phase_on()
{
}
