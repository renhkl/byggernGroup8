#include "solenoid.h"
#include "sam.h"

void pulseSolenoid(uint8_t solenoidState) {
    static uint8_t state = 0;
    
    unsigned long delay = 100000000;
    unsigned long pulseLength = 10000000;
    static unsigned long lastPulse = 0; 
    unsigned long now = time_now();
    //printf(" DELAYTIME %d", delay);
    //printf(" CURRENTTIME %d", now);
    //printf(" LASTTIME %d", lastPulse);

    if(solenoidState && !state && ((now - lastPulse) > delay))
    {
        lastPulse = now;
        state = 1;
        activateSolenoid();
    }
    else if(state && ((now - lastPulse) > pulseLength))
    {
        state = 0;
        deactivateSolenoid();
    }
}

void toggleSolenoid() {
    static uint8_t state = 0;

    unsigned long delay = 100000000;
    static unsigned long lastTime = 0; 
    unsigned long now = time_now();

    if(((now - lastTime) > delay))
    {
        lastTime = now;
        return 1;
    }
    return 0;
    
    if (state) {
        state = 0;
        activateSolenoid();
    }
    if (!state) {
        state = 1;
        deactivateSolenoid();
    }
}

void activateSolenoid() {
    PIOC->PIO_OER = PIO_OER_P24;
    PIOC->PIO_CODR = PIO_CODR_P24; //set low
    //printf(" ACTIVATE");
}

void deactivateSolenoid() {
    PIOC->PIO_OER |= PIO_OER_P24;
    PIOC->PIO_SODR = PIO_SODR_P24; //set high
    //printf(" DEACTIVATE");
}