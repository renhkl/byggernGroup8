#ifndef XMEM_H
#define XMEM_H

#include <stdio.h>

void xmemInit(void);
void xmemWrite(uint8_t data, uint16_t addr);
uint8_t xmemRead(uint16_t addr);

#endif //XMEM_H