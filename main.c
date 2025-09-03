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
#define FOSC 4915200 //clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void testPin();
void sleep();


int main() {
    //SFIOR = (1 << PUD);
    DDRB |= (1 << DDB0); //ouput
    //uartInit(54354);
    uartInit(MYUBRR);

    //unsigned char received_data;

    fdevopen(uartPutChar, NULL);

    printf("Ready");
    
   

while (1){

    char c2 = 'V';
    uartTransmit('A');
    putchar(c2);

    /*
    uartTransmit("{");
         //testPin();
           //testPin();
    //PORTB ^= (1 << PB0); //toggle pin
    sleep();

    uartTransmit(" ");
    sleep();
    */

    char c = getchar();
    putchar(c);
    
    /*received_data = uartReceive();

    uartTransmit(received_data);*/

    /*printf("hei\n");

    uartTransmit("{");

    printf(uartReceive());*/

   
    
    //uartTransmit();
//printf("im here\n");

    

    /*
    PORTB |= (1 << PINB0);

    sleep();

    PORTB &= ~(1 << PB0);
    */
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
