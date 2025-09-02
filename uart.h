#ifndef UART_H
#define UART_H



void uartInit(unsigned int ubrr);

void uartTransmit();

unsigned char uartReceive();




#endif //UART_H