#include "pwm_servo.h"
#include "sam.h"

#define PMC_WPMR_WPEN (0x1u << 0)

void pwmInit() {

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
    PIOB->PIO_PDR |= PIO_PDR_P12;

    //peripheral B function is PWMH0
    PIOB->PIO_ABSR |= PIO_ABSR_P12; 


    //PWM

    //write protections
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(0); 

    
    //choose clk without prescaler, else default
    PWM->PWM_CLK = PWM_CLK_PREB(0) | PWM_CLK_PREA(0);
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_MCK; 
    PWM->PWM_CH_NUM[0].PWM_CPRD = 17000; //period
    PWM->PWM_CH_NUM[0].PWM_CDTY = 50; //duty cycle
    PWM->PWM_ENA |= PWM_ENA_CHID0; //enable channel 0

    printf("%d \n", PWM->PWM_SR & PWM_SR_CHID0);

    //make write protection for pwm active again
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(1); 
   

}

void changeDutyCycle() {

}