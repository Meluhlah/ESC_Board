/*
 * Addressable_leds.h
 *
 *  Created on: Jun 2, 2023
 *      Author: Lidor
 */

#ifndef INC_ADDRESSABLE_LEDS_H_
#define INC_ADDRESSABLE_LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "tim.h"

/* ------------------ Timer Configuration: ------------------------
 *
 * Timer Clock = 64Mhz.
 * GPIO Maximum Output Speed = Very High
 * Prescaler = 1 (1-1 = 0)
 * Counter Period (ARR) = 83 (83-1)
 * Frequency = Timer Clock / ARR --> Freq = 770kHz --> T = 1.3us.
 * Duty [%] = (CCRx / ARR) * 100
 * In this case, LOGIC_ONE = 54 ---> 54/83 * 100 ~ 65%
 * 				 LOGIC_ZERO = 7 --> 29/83 * 100 ~ 35%
 * 1.3us * 65% = 0.845us Which is the time needed for T0H/T1L.
 * 1.3us * 35% = 0.455us Which is the time needed for T0H/T1H.
 *
 * DMA Configuration:
 * 	- Direction : Memory to Peripheral
 * 	- Data Width: Memory     = Byte
 * 				  Peripheral = word
 */


/*-----------------------Defines -------------------------------- */
#define WS2812B_TIMER_FREQ		64000000
#define WS2812B_REQUIRED_FREQ	800000
#define WS2812B_PERIOD			(uint32_t)(WS2812B_TIMER_FREQ / WS2812B_REQUIRED_FREQ)	// ARR = 80
#define WS2812B_T1H_T0L			(uint16_t)(WS2812B_PERIOD * 0.8)
#define WS2812B_T1L_T0H			(uint16_t)(WS2812B_PERIOD * 0.4)

#define NUM_OF_LEDS 			1	// Total LEDS (1 LED is an RGB LED).
#define LED_DATA_LENGTH 		24 	// Each LED has 3 bytes of color, Red, Green, Blue - Total of 3*8 = 24 bits.
#define CYCLES_FOR_RESET 		100	// After sending stream of data, it is required to have a low output for more than 300us.
#define DATA_LENGTH 			(24 * NUM_OF_LEDS) + CYCLES_FOR_RESET
#define TIM_NUM					16
#define TIM_CH					1
#define MAX_BRIGHTNESS			255

/* --------------------- TIMER HANDLER -------------------------- */
#if 	TIM_NUM == 1
#define TIM_HANDLE		htim1
#elif	TIM_NUM == 2
#define TIM_HANDLE		htim2
#elif 	TIM_NUM == 3
#define TIM_HANDLE		htim3
#elif	TIM_NUM == 4
#define TIM_HANDLE		htim4
#elif 	TIM_NUM == 5
#define TIM_HANDLE		htim5
#elif	TIM_NUM == 6
#define TIM_HANDLE		htim6
#elif 	TIM_NUM == 7
#define TIM_HANDLE		htim7
#elif	TIM_NUM == 8
#define TIM_HANDLE		htim8
#elif	TIM_NUM == 9
#define TIM_HANDLE		htim9
#elif	TIM_NUM == 10
#define TIM_HANDLE		htim10
#elif	TIM_NUM == 11
#define TIM_HANDLE		htim11
#elif	TIM_NUM == 12
#define TIM_HANDLE		htim12
#elif	TIM_NUM == 13
#define TIM_HANDLE		htim13
#elif	TIM_NUM == 14
#define TIM_HANDLE		htim14
#elif	TIM_NUM == 15
#define TIM_HANDLE		htim15
#elif	TIM_NUM == 16
#define TIM_HANDLE		htim16
#endif

#if 	TIM_CH == 1
#define TIM_CHANNEL		TIM_CHANNEL_1
#elif	TIM_CH == 2
#define TIM_CHANNEL		TIM_CHANNEL_2
#elif 	TIM_CH == 3
#define TIM_CHANNEL		TIM_CHANNEL_3
#elif	TIM_CH == 4
#define TIM_CHANNEL		TIM_CHANNEL_4
#elif 	TIM_CH == 5
#define TIM_CHANNEL		TIM_CHANNEL_5
#elif 	TIM_CH == 6
#define TIM_CHANNEL		TIM_CHANNEL_6
#elif 	TIM_CH == 7
#define TIM_CHANNEL		TIM_CHANNEL_7
#elif 	TIM_CH == 8
#define TIM_CHANNEL		TIM_CHANNEL_8
#endif

typedef struct{
	uint8_t 	red;
	uint8_t 	green;
	uint8_t 	blue;

} led_t;

void ws2812b_init			(led_t* led);
void ws2812b_clearAll		(led_t* led);
void ws2812b_setRed			(led_t* led, uint8_t redValue);
void ws2812b_setGreen		(led_t* led, uint8_t greenValue);
void ws2812b_setBlue		(led_t* led, uint8_t blueValue);
void ws2812b_setColor		(led_t* led, uint8_t red, uint8_t green, uint8_t blue, uint8_t index);	// Setting each led_t color.
void ws2812b_sendData		(led_t* led);
void ws2812b_blink			(led_t* led, uint8_t r, uint8_t g, uint8_t b);
void ws2812b_setStripColor	(led_t* led, uint8_t red, uint8_t green, uint8_t blue);


#ifdef __cplusplus
}
#endif

#endif /* INC_ADDRESSABLE_LEDS_H_ */
