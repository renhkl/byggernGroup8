#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "latch_test.h"
#include "uart.h"
#include "delay.h"

#define F_CPU 4952000UL
//uart
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

int main() {

    MCUCR |= (1 << SRE);
    latch_test();

    uartInit(MYUBRR);

    printf("LIZZARD");

    while(1){
        char c = getchar();
        printf("You typed: %c\n", c);
    }
    
    return 0;
}

