#include "maincpp.h"


// ******************** Global Variables ******************** //

volatile uint8_t commutation_step;
volatile ERROR_e error;
volatile MOTOR_STATE_e motorState;
led_t ws2812b_led[NUM_OF_LEDS];

// ********************************************************** //

void setup()
{

    ws2812b_init(ws2812b_led);
    HAL_Delay(2);
    ws2812b_setColor(ws2812b_led, 0, 50, 0, 0);
    phaseA.set_pwm(0);
    phaseA.off();
    phaseB.set_pwm(0);
    phaseB.off();
    phaseC.set_pwm(0);
    phaseC.off();
    commutation_step = 0;
    error = NO_ERROR;
    motorState = MOTOR_STATE_IDLE;


#ifdef DEBUGGING
    DEBUG << "Init Function Done.\n";
#endif

}


void commutate()
{
    // Disabling Interrupts on BEMF inputs
    EXTI->RTSR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);
    EXTI->FTSR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);

    // Clearing Pending Interrupt Requests
    EXTI->RPR1 &= (BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);
    EXTI->FPR1 &= (BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);

    commutation_step++;
    commutation_step %= 6;

    /*
        STEP:   0   1   2   3   4   5 | 0   1
                                      |
        HIGH:   A   B   B   C   C   A | A   B
                                      |
        OFF:    B   A   C   B   A   C | B   A
                                      |
        LOW:    C   C   A   A   B   B | C   C
                                      |
        BEMF:   ↑   ↓   ↑   ↓   ↑   ↓ | ↑   ↓
                B   A   C   B   A   C | B   A
    */

    switch(commutation_step)
    {
        case 0:
            phaseB.off();    
            phaseA.high_on();
            phaseC.low_on();
            EXTI->RTSR1 |= BEMF_2_Pin;  // Enabling Rising Interrupt on B
        break;

        case 1:
            phaseA.off();    
            phaseB.high_on();
            phaseC.low_on();
            EXTI->FTSR1 |= BEMF_1_Pin; // Enabling Falling Interrupt on A
        break;
        
        case 2:
            phaseC.off();    
            phaseB.high_on();
            phaseA.low_on();
            EXTI->RTSR1 |= BEMF_3_Pin; // Enabling Rising Interrupt on C
        break;
        
        case 3:
            phaseB.off();    
            phaseC.high_on();
            phaseA.low_on();
            EXTI->FTSR1 |= BEMF_2_Pin; // Enabling Falling Interrupt on B
        break;
        
        case 4:
            phaseA.off();    
            phaseC.high_on();
            phaseB.low_on();
            EXTI->RTSR1 |= BEMF_1_Pin; // Enabling Rising Interrupt on A
        break;
        
        case 5:
            phaseC.off();    
            phaseA.high_on();
            phaseB.low_on();
            EXTI->FTSR1 |= BEMF_3_Pin; // Enabling Falling Interrupt on C
        break;
        
        default:
        break;

        /*
        TODO: Add delay for ringing
        */

    }
}

// Used to filter False readings on interrupt pin
uint8_t getBemfStateRising(){
    
    if(commutation_step == 0){
        return (HAL_GPIO_ReadPin(BEMF_2_GPIO_Port, BEMF_2_Pin));
    }

    else if(commutation_step == 2){
        return (HAL_GPIO_ReadPin(BEMF_3_GPIO_Port, BEMF_3_Pin));
    }

    else if(commutation_step == 4){
        return (HAL_GPIO_ReadPin(BEMF_1_GPIO_Port, BEMF_1_Pin));
    }

    else
        return 0;


}

// Used to filter False readings on interrupt pin
uint8_t getBemfStateFalling(){
    
    if(commutation_step == 1){
        return (HAL_GPIO_ReadPin(BEMF_1_GPIO_Port, BEMF_1_Pin));
    }

    else if(commutation_step == 3){
        return (HAL_GPIO_ReadPin(BEMF_2_GPIO_Port, BEMF_2_Pin));
    }

    else if(commutation_step == 5){
        return (HAL_GPIO_ReadPin(BEMF_3_GPIO_Port, BEMF_3_Pin));
    }
    
    else
        return 0;

}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin == BEMF_1_Pin || GPIO_Pin == BEMF_2_Pin || GPIO_Pin == BEMF_3_Pin){

    /*
        TODO: Check the Pin's state, to filter out noise.
        
        if(getBemfStateRising() == 0){
            return;
        }
    */
        commutate();     
    }
    
}


void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin == BEMF_1_Pin || GPIO_Pin == BEMF_2_Pin || GPIO_Pin == BEMF_3_Pin){

        /*
            TODO: Check the Pin's state, to filter out noise.
            
            if(getBemfStateFalling() == 0){
                return;
            }
        */
            commutate();     
        }
}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(htim == &TIM_HANDLE){
		HAL_TIM_PWM_Stop_DMA(&TIM_HANDLE, TIM_CHANNEL);
	}
}

void maincpp(){

    setup();


    while(1){
       
    }
}