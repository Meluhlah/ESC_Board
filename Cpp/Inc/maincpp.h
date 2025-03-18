#ifndef _MAINCPP_H_
#define _MAINCPP_H_

#include "main.h"
#include "Phase.h"
#include "Debug.h"
#include "defines.h"
#include "ws2812b.h"

void setup();
void commutate();
uint8_t getBemfStateFalling();
uint8_t getBemfStateRising();

extern "C"{

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

}

#endif // _MAINCPP_H_