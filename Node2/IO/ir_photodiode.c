#include "ir_photodiode.h"
#include "sam.h"
#include <stdint.h>
#include <time.h>

void adcInit() {

    //PIO


    //write protections in control register
    ADC->ADC_WPMR |= ADC_WPMR_WPKEY(0x414443);
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;

    //ADC_MR //mode
    ADC->ADC_MR |= ADC_MR_SLEEP_NORMAL;

    //channel 0
    ADC->ADC_CHER |= ADC_CHER_CH0; //enable

}

int adcRead() {

    ADC->ADC_CR |=  ADC_CR_START; //start conversion
    //last coverted value??

    int value = ADC->ADC_CDR[0];
    return value;

}


void countGoals(uint8_t *goals, int sensor, uint8_t goalDelay) {
    static unsigned long lastGoal = 0; 
    unsigned long now = millis();

    if(sensor && (now - lastGoal) > goalDelay){
        (*goals) += 1;
        lastGoal = now;
    }
}

int readPhotodiode() {
    return 0;
}