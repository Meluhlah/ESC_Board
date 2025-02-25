#include "maincpp.h"

volatile uint8_t commutation_step;

void setup()
{
    commutation_step = 0;

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
            EXTI->RTSR1 |= BEMF_2_Pin;  // Enabling Interrupt
        break;

        case 1:
            phaseA.off();    
            phaseB.high_on();
            phaseC.low_on();
            EXTI->FTSR1 |= BEMF_1_Pin;
        break;
        
        case 2:
            phaseC.off();    
            phaseB.high_on();
            phaseA.low_on();
            EXTI->RTSR1 |= BEMF_3_Pin;
        break;
        
        case 3:
            phaseB.off();    
            phaseC.high_on();
            phaseA.low_on();
            EXTI->FTSR1 |= BEMF_2_Pin;
        break;
        
        case 4:
            phaseA.off();    
            phaseC.high_on();
            phaseB.low_on();
            EXTI->RTSR1 |= BEMF_1_Pin;
        break;
        
        case 5:
            phaseC.off();    
            phaseA.high_on();
            phaseB.low_on();
            EXTI->FTSR1 |= BEMF_3_Pin;
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


void maincpp(){


    htim1.Instance->CCR1 = 10;
    phaseA.set_pwm(20);
    while(1){

        HAL_Delay(500);
        DEBUGGER << "Phase A CRR1:\t" << phaseA.get_duty_cycle() << "\n";
        DEBUGGER << "CCR1:\t" << htim1.Instance->CCR1 << "\n";
        
        DEBUGGER << "-----------------\n";

        
    }
}