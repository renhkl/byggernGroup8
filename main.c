#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "latch_test.h"
#include "sram_test.h"

#include "uart.h"
#include "delay.h"

#define F_CPU 4952000UL

//uart baudrate
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

int main() {

    MCUCR |= (1 << SRE);

    uartInit(MYUBRR);

    printf("LIZZARD");

    SRAM_test();

    while(1){
        char c = getchar();
        printf("You typed: %c\n", c);
    }
    
    return 0;
}

