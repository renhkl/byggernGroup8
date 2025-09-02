#include "uart.h"
#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>

//we are using UART0

void uartInit(unsigned int ubrr) {

UBRR0H = (unsigned char) (ubrr >> 8);
UBRR0L = (unsigned char) ubrr;
UCSR0B = (1 << RXEN0) | (1< TXEN0); //enable RX and TX

UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00); //two stop bits

}


 uartTransmit(uint8_t data) { //ubrr er input..
/*int ubrr = 0;
*/

while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;

}

unsigned char uartReceive() {
// while (!(UCSR0A & (1 << RXC0))) { //if receive completed
//    return UDR;
// }
}