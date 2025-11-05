#include "pwm_servo.h"
#include "sam.h"

void pwmInit() {
    //choose peripheral
    //PMC->PMC_PCER0 |= (1 << PID0);
    //PIOc-> PIO_PDR |= 

    //turn off PIO write protection
    PIOB->PIO_WPMR |= PIO_WPEN;

    //enable peripheral control of pin
    PIOB->PIO_PDR |= PIO_PDR_P12;

    //peripheral B function is PWMH0
    PIOB->PIO_ABSR |= PIO_ABSR_P12; 



    //channel mode

    //channel period

    //duty cycle

    //enable register

}