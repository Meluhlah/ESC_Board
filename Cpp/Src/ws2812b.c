/*
 * Addressable_leds.c
 *
 *  Created on: Jun 2, 2023
 *      Author: Lidor
 */
#include "ws2812b.h"

uint8_t pwmflag = 0;
uint8_t pwmData[DATA_LENGTH];
struct LED led[NUM_OF_LEDS];

void led_init(){
	led_initPattern();
	led_clearAll();
}

void led_initPattern(){
	for(int j=0 ; j<3; ++j){
		for(int i=0 ; i<=255; i++){
			led_setStrip_color(0, i, 0);
		}
		for(int i=255; i>=0; --i){
			led_setStrip_color(0, i, 0);
		}
	}
}

void led_clearAll(){
	for(int i=0; i<NUM_OF_LEDS; ++i)
			led_setColor(0, 0, 0, i);
		led_sendData();
}


void led_setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t id){
	led[id].red = red;
	led[id].green = green;
	led[id].blue = blue;
}


void led_sendData(){
	uint16_t index = 0;
		for(int i = 0; i < NUM_OF_LEDS; ++i)
		{
			uint32_t color = (led[i].green << 16) | (led[i].red << 8) | led[i].blue;
			
			for(int j=23; j >= 0; --j)
			{
				if (color & (1 << j))
					pwmData[index] = T1H_T0L;

				else
					pwmData[index] = T1L_T0H;
				++index;

			}
		}
		
		for(int i = index; i < DATA_LENGTH; ++i)
			pwmData[i] = 0;

	HAL_TIM_PWM_Start_DMA(&TIM_HANDLE, TIM_CHANNEL, (uint32_t*)pwmData, sizeof(pwmData));
	while(pwmflag == 0);
	pwmflag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(htim == &TIM_HANDLE){
		HAL_TIM_PWM_Stop_DMA(&TIM_HANDLE, TIM_CHANNEL);
	}
	pwmflag = 1;
}

void led_setStrip_color(uint8_t red, uint8_t green, uint8_t blue){
	for(int i=0; i<NUM_OF_LEDS; ++i)
	{
		led_setColor(red, green, blue, i);
	}
	led_sendData();
}


void led_blink(uint8_t r, uint8_t g, uint8_t b){
	for(int j=0; j<3; j++){
		led_setStrip_color(r, g, b);
		HAL_Delay(100);
		led_clearAll();
		HAL_Delay(100);
	}
}

