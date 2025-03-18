#include "maincpp.h"


// ******************** Global Variables ******************** //

// Commutation Params *************************************** //

Phase phaseA(&htim1, TIM_CHANNEL_1, &TIM1->CCR1, PHASE1_LOW_GPIO_Port, PHASE1_LOW_Pin);
Phase phaseB(&htim1, TIM_CHANNEL_2, &TIM1->CCR2, PHASE2_LOW_GPIO_Port, PHASE2_LOW_Pin);
Phase phaseC(&htim1, TIM_CHANNEL_3, &TIM1->CCR3, PHASE3_LOW_GPIO_Port, PHASE3_LOW_Pin);

volatile uint8_t commutation_step;
volatile uint8_t zc_counter = 0;
volatile uint32_t imrDefaultFlags = 0;
uint8_t zcDetEnable = 0;

// Status Params ******************************************** //
volatile MOTOR_STATE_e motorState;
volatile ERROR_e error;

// Delay Params ********************************************* //
volatile uint8_t tim14DelayFlag = 0;

// WS2812B LED Params *************************************** //
led_t ws2812b_led[NUM_OF_LEDS];

// Debugger ************************************************* //
Debug DEBUGGER(&huart1);

// ********************************************************** //

void phasesOff(){

    phaseA.off();
    phaseB.off();
    phaseC.off();

}


void setPhasesPwm(uint32_t dutyCycle){

    phaseA.set_pwm(dutyCycle);
    phaseB.set_pwm(dutyCycle);
    phaseC.set_pwm(dutyCycle);

}


void delayMicro(uint16_t delay){
	__HAL_TIM_SET_COUNTER(&DELAY_TIMER, 0);
	while(__HAL_TIM_GET_COUNTER(&DELAY_TIMER) < delay);
}


void setup()
{

    ws2812b_init(ws2812b_led);
    
    #ifdef DEBUGGING
    HAL_Delay(2);
    ws2812b_setColor(ws2812b_led, 0, 0, 255, 0);
    DEBUGGER << "Setup Function Called.\n";
    #endif

    EXTI->IMR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);
    imrDefaultFlags = EXTI->IMR1;
    phasesOff();
    setPhasesPwm(0);
    commutation_step = 0;
    error = NO_ERROR;
    motorState = MOTOR_STATE_IDLE;
    HAL_TIM_Base_Start(&htim14);

}


void commutate()
{

    EXTI->IMR1 = imrDefaultFlags;
    EXTI->IMR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);

    // Disabling Interrupts on BEMF inputs
    EXTI->RTSR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);
    EXTI->FTSR1 &= ~(BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);

    // Clearing Pending Interrupt Requests
    EXTI->RPR1 |= (BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);
    EXTI->FPR1 |= (BEMF_1_Pin | BEMF_2_Pin | BEMF_3_Pin);

    commutation_step++;
    commutation_step %= 6;

    delayMicro(2);


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
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_2_Pin);
                EXTI->RTSR1 |= BEMF_2_Pin;  // Enabling Rising Interrupt on B
            }
            phaseB.off();    
            phaseA.high_on();
            phaseC.low_on();
        break;

        case 1:
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_1_Pin);
                EXTI->FTSR1 |= BEMF_1_Pin; // Enabling Falling Interrupt on A
            }
            phaseA.off();    
            phaseB.high_on();
            phaseC.low_on();
        break;
        
        case 2:
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_3_Pin);
                EXTI->RTSR1 |= BEMF_3_Pin; // Enabling Rising Interrupt on C
            }
            phaseC.off();    
            phaseB.high_on();
            phaseA.low_on();
        break;
        
        case 3:
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_2_Pin);
                EXTI->FTSR1 |= BEMF_2_Pin; // Enabling Falling Interrupt on B
            }
            phaseB.off();    
            phaseC.high_on();
            phaseA.low_on();
        break;
        
        case 4:
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_1_Pin);
                EXTI->RTSR1 |= BEMF_1_Pin; // Enabling Rising Interrupt on A
            }
            phaseA.off();    
            phaseC.high_on();
            phaseB.low_on();
        break;
        
        case 5:
            if(1 == zcDetEnable){
                EXTI->IMR1  = (imrDefaultFlags | BEMF_3_Pin);
                EXTI->FTSR1 |= BEMF_3_Pin; // Enabling Falling Interrupt on C
            }
            phaseC.off();    
            phaseA.high_on();
            phaseB.low_on();
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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

    if(huart->Instance == USART1){

        DEBUGGER.setTxFlag();

    }

}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin == BEMF_1_Pin || GPIO_Pin == BEMF_2_Pin || GPIO_Pin == BEMF_3_Pin){

        #ifdef DEBUGGING
        HAL_GPIO_TogglePin(SPARE3_GPIO_Port, SPARE3_Pin);
        #endif

        if(zcDetEnable){
            zc_counter++;
        }
        
        if(motorState == MOTOR_STATE_AUTO_COMMUTATION){  
            
            delayMicro(TEST_ZC_MEASUREMENT_DELAY);
            if(getBemfStateRising() == 0){
                return;
            }         
            commutate();     

        }
    }
    
}


void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin == BEMF_1_Pin || GPIO_Pin == BEMF_2_Pin || GPIO_Pin == BEMF_3_Pin){

       #ifdef DEBUGGING
       HAL_GPIO_TogglePin(SPARE3_GPIO_Port, SPARE3_Pin);
       #endif


        if(zcDetEnable){
            zc_counter++;
        }
        
        if(motorState == MOTOR_STATE_AUTO_COMMUTATION){       
        
            delayMicro(TEST_ZC_MEASUREMENT_DELAY);
            if(getBemfStateFalling() == 1){
                return;
            }         
        
            commutate();          
        }
    }
}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){

    // WS2812B LED
    if(htim == &TIM_HANDLE){
		HAL_TIM_PWM_Stop_DMA(&TIM_HANDLE, TIM_CHANNEL);
	}

    
}


void motorAlign(){

    //TODO: Make Align function Smoother, Less Amp

    #ifdef DEBUGGING
    DEBUGGER << "Motor Align Function Called.\n";
    #endif
    
    motorState = MOTOR_STATE_ALIGN;
    phaseB.low_on();
    phaseC.low_on();
    phaseA.high_on();
    
    for(int i = 0; i < ALIGN_STEPS_NUM; i++){
        uint32_t newDutyCycle = phaseA.get_duty_cycle() + ALIGN_PWM_INCREMENT_PER_STEP;
        if(newDutyCycle <= ALIGN_PWM_TARGET){
            phaseA.set_pwm(newDutyCycle);
        }        
        HAL_Delay(ALIGN_STEP_DURATION_MS);
    }

    #ifdef DEBUGGING
    ws2812b_setColor(ws2812b_led, 0, 255, 0, 0);
    #endif

    commutation_step = 0;
    // phaseA.off();
    // phaseB.off();
    // phaseC.off();

    #ifdef DEBUGGING
    DEBUGGER << "Motor Align Function Done.\n";
    #endif
}


void motorRamp(){

    #ifdef DEBUGGING
    DEBUGGER << "Motor Ramp Function Called.\n";
    #endif

    motorState = MOTOR_STATE_RAMP;
    setPhasesPwm(ALIGN_PWM_TARGET);
    zcDetEnable = 0;
    uint16_t stepDelayUs = RAMP_STEP_INIT_DURATION_US;

    for(int i = 0; i < RAMP_STEPS_NUM; i++){

        if(zc_counter >= RAMP_ZC_DETECTION_THRESHOLD){

            motorState = MOTOR_STATE_AUTO_COMMUTATION;
            break;

        }

        if(i == RAMP_ZC_DET_ENABLE_STEP){

            zcDetEnable = 1;

        }
        commutate();

        if(stepDelayUs - RAMP_STEP_DECREMENT_PER_STEP_US >= RAMP_STEP_TARGET_DURATION_US){
           stepDelayUs -= RAMP_STEP_DECREMENT_PER_STEP_US;
        }
        delayMicro(stepDelayUs);
    }
    
    // Check for motor Lock.
    if(motorState != MOTOR_STATE_AUTO_COMMUTATION){
        error = ERROR_MOTOR_LOCKED;
        phasesOff();

    }

    #ifdef DEBUGGING
    DEBUGGER << "Motor Ramp Function Done.\n";
    ws2812b_setColor(ws2812b_led, 255, 0, 0, 0);
    #endif

}


void maincpp(){

    setup();
    // motorAlign();
    // motorRamp();
    // phaseA.low_on();
    // phaseB.low_on();
    // phaseC.low_on();
    

    while(1){


    }
}