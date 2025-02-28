/*
 * Addressable_leds.c
 *
 *  Created on: Jun 2, 2023
 *      Author: Lidor
 */
#include "ws2812b.h"

uint8_t ws2812b_pwmFlag = 0;
uint8_t ws2812b_pwmData[DATA_LENGTH];

void ws2812b_init(LED* led){
	ws2812b_initPattern(led);
	ws2812b_clearAll(led);
}

void ws2812b_initPattern(LED* led){
	for(int j=0 ; j<3; ++j){
		for(int i=0 ; i<=255; i++){
			ws2812b_setStripColor(led, 0, i, 0);
		}
		for(int i=255; i>=0; --i){
			ws2812b_setStripColor(led, 0, i, 0);
		}
	}
}

void ws2812b_clearAll(LED* led){
	for(int i=0; i<NUM_OF_LEDS; ++i)
			ws2812b_setColor(led, 0, 0, 0, i);
		ws2812b_sendData(led);
}


void ws2812b_setColor(LED* led, uint8_t red, uint8_t green, uint8_t blue, uint8_t id){
	led[id].red = red;
	led[id].green = green;
	led[id].blue = blue;
}


void ws2812b_sendData(LED* led){
	uint16_t index = 0;
		for(int i = 0; i < NUM_OF_LEDS; ++i)
		{
			uint32_t color = (led[i].green << 16) | (led[i].red << 8) | led[i].blue;
			
			for(int j=23; j >= 0; --j)
			{
				if (color & (1 << j))
					ws2812b_pwmData[index] = T1H_T0L;

				else
					ws2812b_pwmData[index] = T1L_T0H;
				++index;

			}
		}
		
		for(int i = index; i < DATA_LENGTH; ++i)
			ws2812b_pwmData[i] = 0;

	HAL_TIM_PWM_Start_DMA(&TIM_HANDLE, TIM_CHANNEL, (uint32_t*)ws2812b_pwmData, sizeof(ws2812b_pwmData));
	while(ws2812b_pwmFlag == 0);	// TODO: Check if can be removed.
	ws2812b_pwmFlag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(htim == &TIM_HANDLE){
		HAL_TIM_PWM_Stop_DMA(&TIM_HANDLE, TIM_CHANNEL);
	}
	ws2812b_pwmFlag = 1;
}

void ws2812b_setStripColor(LED* led, uint8_t red, uint8_t green, uint8_t blue){
	for(int i=0; i<NUM_OF_LEDS; ++i)
	{
		ws2812b_setColor(led, red, green, blue, i);
	}
	ws2812b_sendData(led);
}


void ws2812b_blink(LED* led, uint8_t r, uint8_t g, uint8_t b){
	for(int j=0; j<3; j++){
		ws2812b_setStripColor(led, r, g, b);
		HAL_Delay(100);
		ws2812b_clearAll(led);
		HAL_Delay(100);
	}
}

