#include "spi.h"

#define SS1 PB3 //display
#define SS2 PB4 //io

//MASTER
void SPIMasterInit(void){
    //      MOSI       SCK        
    DDRB |= (1 << PB5 | 1 << PB7 | 1 << PB3 | 1 << PB4);

    PORTB |= (1 << SS1 | 1 << SS2);

    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (0 << CPOL) | (0 << CPHA);
}

void SPIMasterTransmit(char cData, int slave){

    PORTB |= (1 << SS1 | 1 << SS2);

    _delay_ms(0.408);

    if (slave == 1){
        PORTB &= ~(1 << SS1); //display
    }
    else if (slave == 2){
        PORTB &= ~(1 << SS2); //io
    }

    _delay_ms(0.408);

    SPDR = cData;

    while(!(SPSR & (1 << SPIF))){
        ;
    }

    PORTB |= (1 << SS1 | 1 << SS2);
}


void SPIMasterTransmitDisplay(char cData){
    SPIMasterTransmit(cData, 1); 
}

