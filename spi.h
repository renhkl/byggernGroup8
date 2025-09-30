#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void SPIMasterInit(void);
void SPIMasterTransmit(char cData, int slave);
int readByte();
void writeByte(int byte);
void writeNBytes(int *bytes, int n);
int readNBytes(int n);

#endif