#include "ir_photodiode.h"
#include "sam.h"
#include <stdint.h>

#define PMC_WPMR_WPEN (0x1u << 0)

void adcInit() {

    //PIO
    
    //write protections in control register
    PMC->PMC_WPMR |= PMC_WPMR_WPKEY(0x504D43);
    PMC->PMC_WPMR &= ~PMC_WPMR_WPEN;

    //peripheral clock for ADC
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;

    //turn off PIO write protection    
    PIOA->PIO_WPMR |= PIO_WPMR_WPKEY(0x50494F); 
    PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN;

    //enable peripheral control of pin, disable PIO
    PIOA->PIO_PDR |= PIO_PDR_P2;
    
    //ADC

      //PIOA->PIO_ = PIO_OER_P13; //make output

    //write protections in control register
    ADC->ADC_WPMR |= ADC_WPMR_WPKEY(0x414443);
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;

    //free running means no need for manual ADC trigger
    ADC->ADC_MR |= ADC_MR_FREERUN; 

    //channel 0
    ADC->ADC_CHER |= ADC_CHER_CH0; //enable
    ADC->ADC_CR |=  ADC_CR_START; //start conversion

}

void adcINIT() {

    //free running means no need for manual ADC trigger
    ADC->ADC_MR |= ADC_MR_FREERUN; 

    //channel 0
    ADC->ADC_CHER |= ADC_CHER_CH0; //enable

    //peripheral clock for ADC
    PMC->PMC_PCR |= (1 << 28) | (37 << 0);
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;

    ADC->ADC_CR |=  ADC_CR_START; //start conversion
}

int adcRead() {

    
    //last coverted value??
    //printf("ADC status %d", ADC->ADC_CHSR);
    int value = ADC->ADC_CDR[0];
    return value;

}


void countGoals(uint8_t *goals, int sensor) {
    if(sensor){
        *goals += 1;
    }
}

int readPhotodiode() {
return 0;
}