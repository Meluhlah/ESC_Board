/*
 * Addressable_leds.c
 *
 *  Created on: Jun 2, 2023
 *      Author: Lidor
 */
#include "ws2812b.h"

// uint8_t ws2812b_pwmFlag = 0;
uint8_t ws2812b_pwmData[DATA_LENGTH] = {0};

void ws2812b_init(led_t* led){
	ws2812b_clearAll(led);
}


void ws2812b_clearAll(led_t* led){
	for(int i=0; i<NUM_OF_LEDS; ++i)
	{
			ws2812b_setColor(led, 0, 0, 0, i);
	}
		ws2812b_sendData(led);
}

void ws2812b_setRed			(led_t* led, uint8_t redValue){
	led->red = redValue;
	ws2812b_sendData(led);
}

void ws2812b_setGreen		(led_t* led, uint8_t greenValue){
	led->green = greenValue;
	ws2812b_sendData(led);
}


void ws2812b_setBlue		(led_t* led, uint8_t blueValue){
	led->blue = blueValue;
	ws2812b_sendData(led);
}


void ws2812b_setColor(led_t* led, uint8_t red, uint8_t green, uint8_t blue, uint8_t id){
	led[id].red = red;
	led[id].green = green;
	led[id].blue = blue;
	ws2812b_sendData(led);
}


void ws2812b_sendData(led_t* led){
	uint16_t index = 0;
		for(int i = 0; i < NUM_OF_LEDS; ++i)
		{
			uint32_t color = (led[i].green << 16) | (led[i].red << 8) | led[i].blue;
			
			for(int j=LED_DATA_LENGTH-1; j >= 0; --j)
			{
				if (color & (1 << j)){
					ws2812b_pwmData[index] = WS2812B_T1H_T0L;
				}

				else{
					ws2812b_pwmData[index] = WS2812B_T1L_T0H;
				}
				
				++index;

			}
		}
		
		for(int i = index; i < DATA_LENGTH; ++i)
		{
			ws2812b_pwmData[i] = 0;
		}

	HAL_TIM_PWM_Start_DMA(&TIM_HANDLE, TIM_CHANNEL, (uint32_t*)ws2812b_pwmData, sizeof(ws2812b_pwmData));

}

#if (NUM_OF_LEDS > 1)
void ws2812b_setStripColor(led_t* led, uint8_t red, uint8_t green, uint8_t blue){
	for(int i=0; i<NUM_OF_LEDS; ++i)
	{
		ws2812b_setColor(led, red, green, blue, i);
	}
	ws2812b_sendData(led);
}


void ws2812b_blink(led_t* led, uint8_t r, uint8_t g, uint8_t b){
	for(int j=0; j<3; j++){
		ws2812b_setStripColor(led, r, g, b);
		HAL_Delay(100);
		ws2812b_clearAll(led);
		HAL_Delay(100);
	}
}

#endif