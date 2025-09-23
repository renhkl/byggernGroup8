#ifndef SPI_H
#define SPI_H

int readByte();
void writeData(int byte);
void writeNBytes(int *bytes, int n);
int readBytes(int n);

#endif