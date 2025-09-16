#include <avr/io.h>
#include <util/delay.h>

#include "latch_test.h"

void latch_test() {
    
    volatile char *ext_mem = (char *) 0x1800;

    while (1) {
        for (uint8_t i = 0; i <= 0xFF; i++) {
            volatile char *addr = (char *)(0x1800 + i);
            *addr = 0x00;  
            _delay_ms(250);  
        }
    }
}
