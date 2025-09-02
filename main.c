//#include </usr/lib/avr/include/avr/io.h>
//#include <avr/io.h>

#include "uart.h"

#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>
//#include <asm/types.h>


#define F_CPU 4952000UL
//#include <util/delay.h>
//#include </usr/lib/avr/include/util/delay.h>
#include "delay.h"


//uart
#define FOSC 1843200 //clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void testPin();
void sleep();

int main() {
    //SFIOR = (1 << PUD);
    DDRB |= (1 << DDB0); //ouput
    //uartInit(54354);
    uartInit(MYUBRR);
   

while (1){

    uartTransmit(0xFF);
         //testPin();
           //testPin();
    //PORTB ^= (1 << PB0); //toggle pin
    sleep();

    uartTransmit(0x00);
    sleep();
    //uartTransmit();
//printf("im here\n");

    

    PORTB |= (1 << PINB0);

    sleep();

    PORTB &= ~(1 << PB0);

     sleep();
         

}
    return 0;
}


void sleep() {
    for (long i = 0; i < 1000000; i++) {
        __asm__("nop");
    }
}

void testPin() {
    
    
    //DDRB &= ~(1 << DDB0); //input
    //PORTB |= (1 << PB0); //internal pullup
    

    
    //PORTB &= ~(1 << PB0);
    PORTB ^= (1 << PB2);
    //PORTB ^= (1 << PB0); //toggle pin
    //PINB |= (1 << PINB0);

    sleep();
    //_delay_ms(1000);

    //PORTB &= ~(1 << PB0);
//sleep();
}
