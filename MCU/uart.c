#include "uart.h"
#include "avr/io.h"
#include <stdio.h>
#include </usr/lib/avr/include/avr/iom162.h>

//we are using UART0

void uartInit(unsigned int ubrr) {

    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); //enable RX and TX

    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00); //two stop bits

    fdevopen(uartPutChar, uartGetChar);
}


void uartTransmit(unsigned char data) { 
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;

    return 0;
}


unsigned char uartReceive() {
    while (!(UCSR0A & (1 << RXC0))); //data ready to be read
    
    return UDR0;
}

int uartPutChar(char c, FILE *stream){
    if(c == '\n'){
        uartPutChar('\r', stream);
    }
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = c;

    return 0;
}

int uartGetChar(FILE *stream){
    while(!(UCSR0A & (1 << RXC0)));

    return UDR0;
}