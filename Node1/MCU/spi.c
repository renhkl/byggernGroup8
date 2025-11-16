#include "spi.h"

#define DISPLAY_SS PB3 //display
#define IO_SS PB4 //io
#define CAN_SS PB0
#define IO_DELAY 40

//MASTER
void SPIMasterInit(void){
    //      MOSI       SCK        
    DDRB |= (1 << PB5 | 1 << PB7 | 1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);

    DDRB &= ~(1 << PB6);

    PORTB |= (1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);

    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (0 << CPOL) | (0 << CPHA);
    printf("spi initialized\n");
}

void SPIMasterTransmit(char cData, int slave){

    PORTB |= (1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);

    _delay_ms(0.408);

    if (slave == 1){
        PORTB &= ~(1 << DISPLAY_SS); //display
    }
    else if (slave == 2){
        PORTB &= ~(1 << IO_SS); //io
    }
    else if (slave == 3){
        PORTB &= ~(1 << CAN_SS); //CAN
    }

    _delay_ms(0.408);

    SPDR = cData;

    while(!(SPSR & (1 << SPIF))){
        ;
    }

    PORTB |= (1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);
}

void SPIMasterReceive(IOcommands command, char *data, uint8_t lenData){

    PORTB |= (1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);

    //_delay_ms(0.408);

    PORTB &= ~(1 << IO_SS);

    SPDR = command;
    _delay_us(IO_DELAY);

    for(int i = 0; i < lenData; i++){
        SPDR = 0xFF;
        while(!(SPSR & (1 << SPIF))){
        ;
        }
        data[i] = SPDR;
        _delay_us(2);
    }

    PORTB |= (1 << DISPLAY_SS | 1 << IO_SS | 1 << CAN_SS);
}

void ledInit(void){
    DDRD |= (1 << PD2);
    PORTD &= ~(1 << PD2); 
}

void ledToggle(void){
    PORTD ^= (1 << PD2);
}

void SPIMasterTransmitDisplay(char cData){
    SPIMasterTransmit(cData, 1); 
}

