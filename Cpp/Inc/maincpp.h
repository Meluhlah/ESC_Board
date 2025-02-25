#ifndef _MAINCPP_H_
#define _MAINCPP_H_

#include "main.h"
#include "Phase.h"
#include "Debug.h"
#include "defines.h"


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

Debug DEBUGGER;

Phase phaseA(&htim1, TIM_CHANNEL_1, &htim1.Instance->CCR1, PHASE1_LOW_GPIO_Port, PHASE1_LOW_Pin);
Phase phaseB(&htim1, TIM_CHANNEL_2, &htim1.Instance->CCR2, PHASE2_LOW_GPIO_Port, PHASE2_LOW_Pin);
Phase phaseC(&htim1, TIM_CHANNEL_3, &htim1.Instance->CCR3, PHASE3_LOW_GPIO_Port, PHASE3_LOW_Pin);


#endif // _MAINCPP_H_