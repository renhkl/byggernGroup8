#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "latch_test.h"
#include "sram_test.h"
#include "xmem.h"   

#include "uart.h"
#include "delay.h"

//#define F_CPU 4952000UL
#define F_CPU 4915200UL

//uart baudrate
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

int main() {

    uartInit(MYUBRR);

    printf("LIZZARD");

    xmem_init();
    SRAM_cr_test();


    while(1){

        //chip select test
        //xmem_write(0xF0, 0x1D00); //sram address, active high
        //xmem_write(0x0F, 0x1500);//adc address, active low

        //char c = getchar();
        //printf("You typed: %c\n", c);
    }
    
    return 0;
}


