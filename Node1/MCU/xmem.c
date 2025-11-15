//source: functions taken from lab lecture 2

#include <stdio.h>
#include "avr/io.h"
#include </usr/lib/avr/include/avr/iom162.h>
//#include <avr/iom162.h>

#define BASE_ADDRESS 0x1000


void xmemInit(void) {
    MCUCR |= (1 << SRE); //enable external memory, activates AD7:0 and A15:8, ALE, invWR and invRD signals
    SFIOR |= (1 << XMM2); //release port pins PC7-PC4 which are used by JTAG
}

//write to external memory, choose both what data and where it should be stored (address)
void xmemWrite(uint8_t data, uint16_t addr) {
    volatile char *ext_mem = (char *) BASE_ADDRESS;
    ext_mem[addr] = data;
}

//read to external memory by giving address for where to read
uint8_t xmemRead(uint16_t addr) {
    volatile char *ext_mem = (char *) BASE_ADDRESS;
    uint8_t ret_val = ext_mem[addr];
    return ret_val;
}
