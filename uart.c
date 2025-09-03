#include "uart.h"
#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

//we are using UART0

void uartInit(unsigned int ubrr) {

UBRR0H = (unsigned char) (ubrr >> 8);
UBRR0L = (unsigned char) ubrr;
UCSR0B = (1 << RXEN0) | (1< TXEN0); //enable RX and TX

UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00); //two stop bits

UCSR0B |= (1 << RXCIE0); //interrupt rx

}


void uartTransmit(unsigned char data) { //ubrr er input.., byttet fra unit8_t
while (!(UCSR0A & (1 << UDRE0)));
//while ((!UCSR0A & (1 << TXC0))) //when tx is not busy
UDR0 = data;
}

unsigned char uartReceive() {
 while (!(UCSR0A & (1 << RXC0))); //data ready to be read
 putchar(UDR0);
    return UDR0;
 
}

int uartPutChar(char c, FILE *stream) {
    if (c == '\n') {
        uartTransmit('\r');
    }
    uartTransmit(c);
    return 0;
}

int uartGetChar(FILE *stream) {
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}
