#include "pwm_servo.h"
#include "sam.h"
#include <stdint.h>

#define PMC_WPMR_WPEN (0x1u << 0)

void pwmServoInit() {

    //PIO

    //write protections in control register
    PMC->PMC_WPMR |= PMC_WPMR_WPKEY(0x504D43);
    PMC->PMC_WPMR &= ~PMC_WPMR_WPEN;


    //peripheral clock for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;


    //turn off PIO write protection    
    PIOB->PIO_WPMR |= PIO_WPMR_WPKEY(0x50494F); 
    PIOB->PIO_WPMR &= ~PIO_WPMR_WPEN;

    //enable peripheral control of pin, disable PIO
    PIOB->PIO_PDR |= PIO_PDR_P13;

    //peripheral B function is PWMH0
    PIOB->PIO_ABSR |= PIO_ABSR_P13; 


    //PWM

    //write protections
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(0); 

    
    //choose clk 32 prescaler
    PWM->PWM_CLK = PWM_CLK_PREB(0b101) | PWM_CLK_PREA(0b101);
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_32; 
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL; //change channel polarity 
    
    PWM->PWM_CH_NUM[1].PWM_CPRD = 52500; //period
    PWM->PWM_CH_NUM[1].PWM_CDTY = 4000; //duty cycle
    PWM->PWM_ENA |= PWM_ENA_CHID1; //enable channel 1

    printf("%d \n", PWM->PWM_SR & PWM_SR_CHID1);

    //make write protection for pwm active again
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(1); 
   

}

void changeDutyCycle(int degrees, int channel) {

    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(0); 

    if (channel == 0) {
        PWM->PWM_CH_NUM[0].PWM_CDTY = degrees; //duty cycle
        printf(" MOTOR DUTY %d", degrees);
    }

    else if (channel == 1) {
        PWM->PWM_CH_NUM[1].PWM_CDTY = degrees; //duty cycle
    }
        //make write protection for pwm active again
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(1); 

    //5000 = 2 ms
    //duty cycle between a and b seconds
    //with servo

}

int degreesToDutyCycle(int degrees) {
    if (degrees < 0) {degrees = 0;}
    if (degrees > 180) {degrees = 180;}
    double DutyCycle = (5000/1.9) * ((double)degrees/180 + 1);
    return (int)DutyCycle;
}

void changeServoPosition(uint8_t degrees) {
    int DutyCycle = degreesToDutyCycle((int)degrees);
    changeDutyCycle(DutyCycle, 1);
}

void interruptInit(){ 
    NVIC_EnableIRQ(PWM_IRQn); 
    PWM->PWM_ENA |= PWM_ENA_CHID0; 
    PWM->PWM_IER2 = PWM_IER2_CMPM0; 
    PWM->PWM_CMP[0].PWM_CMPM = PWM_CMPM_CEN;
    PWM->PWM_CMP[0].PWM_CMPV = 500; 
} 

void interruptHandler(){ 
    uint32_t status = PWM->PWM_ISR2; 
    if (status & PWM_ISR2_CMPM0){ 
        //logic to be executed 
    } 
}

uint8_t lowpassFilter(float input, float *prevOutput, float alpha){

    float output = alpha * (*prevOutput) + (1 - alpha) * input;
    *prevOutput = output;

    return output;
}