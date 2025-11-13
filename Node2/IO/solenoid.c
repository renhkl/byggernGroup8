#include "solenoid.h"
#include "sam.h"

void pulseSolenoid(uint8_t solenoidState) {
    unsigned long delay = 1000000;
    static unsigned long lastTime = 0; 
    unsigned long now = time_now();

    if(solenoidState && ((now - lastTime) > delay))
    {
        lastTime = now;

        time_spinFor(mescs(200));
    }
}

void activateSolenoid() {
    //PIOC->PIO_OER |= PIO_OER24;
}