#include "Phase.h"
#include "Debug.h"

Phase::Phase(TIM_HandleTypeDef* timer, uint16_t channel,
    volatile uint32_t* ccrx, GPIO_TypeDef* low_port, uint16_t low_pin)

    :timer(timer),
    channel(channel),
    ccrx(ccrx),
    low_port(low_port),
    low_pin(low_pin)

{
    if (ccrx != nullptr)
    {
        *ccrx = 0;
    }  
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

void Phase::off()
{
    HAL_TIM_PWM_Stop(timer, channel);
    HAL_GPIO_WritePin(low_port, low_pin, GPIO_PIN_RESET);
}

void Phase::high_on()
{
    HAL_TIM_PWM_Start(timer, channel);
    HAL_GPIO_WritePin(low_port, low_pin, GPIO_PIN_RESET);
}

void Phase::low_on()
{
    HAL_TIM_PWM_Stop(timer, channel);
    HAL_GPIO_WritePin(low_port, low_pin, GPIO_PIN_SET);
}

uint32_t Phase::get_duty_cycle()
{
    return *ccrx;
}
