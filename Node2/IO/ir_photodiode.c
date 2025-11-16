#include "ir_photodiode.h"
#include "sam.h"
#include <stdint.h>
#include <time.h>

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
    int value = ADC->ADC_CDR[0];
    return value;

}

uint8_t countGoals(uint8_t *goals, int sensor, unsigned long goalDelay) {
    static unsigned long lastGoal = 0; 
    unsigned long now = time_now();

    if(sensor && ((now - lastGoal) > goalDelay)){
        (*goals) += 1;
        lastGoal = now;
        return 1;
    }
    return 0;
}

int readPhotodiode() {
    return 0;
}

//Uses ring buffer to avarage last samples (noise reduction) for goal detection.  
uint8_t isGoal(ringBufferGoals goalArray)
{   
    double avarage = 0;
    for(uint8_t i = 0; i < goalArray.size; i++){
        avarage += ringBufferGoalsGet(&goalArray, i);
    }
    avarage = avarage/goalArray.size;

    if (avarage > 800) return 0;
    else return 1;
}

void ringBufferGoalsInit(ringBufferGoals *rb, uint8_t size){
    rb->head = 0;
    rb->count = 0;
    rb->size = size;
    rb->data = (double *)malloc(size * sizeof(double));
    for(uint8_t i = 0; i < size; i++){
        rb->data[i] = 0;
    }
}

void ringBufferGoalsAdd(ringBufferGoals *rb, double value){
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % rb->size;
    if(rb->count < 4){
        rb->count += 1;
    }
}

double ringBufferGoalsGet(ringBufferGoals *rb, int index){
    int start = (rb-> head - rb->count + rb->size) % rb->size;
    int idx = (start + index) % rb->size;
    return rb->data[idx];
}