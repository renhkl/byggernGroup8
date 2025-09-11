//#include </usr/lib/avr/include/avr/io.h>
#include <avr/io.h>

#include "uart.h"

//#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>
//#include <asm/types.h>


#define F_CPU 4952000UL
//#include <util/delay.h>
//#include </usr/lib/avr/include/util/delay.h>
#include "delay.h"


//uart
#define F_CPU 4915200 //clock speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

int main() {

    uartInit(MYUBRR);

    printf("HELLO WORLD");

    while(1){
        char c = getchar();
        printf("You typed: %c\n", c);
    }
    
    return 0;
}

