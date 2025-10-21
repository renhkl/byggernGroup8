#ifndef UART_H
#define UART_H
#include <stdio.h>



void uartInit(unsigned int ubrr);

void uartTransmit(unsigned char data);

unsigned char uartReceive();

int uartPutChar(char c, FILE *stream);

int uartGetChar(FILE *stream);


#endif //UART_H