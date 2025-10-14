#include "spi.h"

#define SS1 PB3 //display
#define SS2 PB4 //io
#define IO_DELAY 40

//MASTER
void SPIMasterInit(void){
    //      MOSI       SCK        
    DDRB |= (1 << PB5 | 1 << PB7 | 1 << PB3 | 1 << PB4);

    DDRB &= ~(1 << PB6);

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

void SPIMasterReceive(IOcommands command, char *data, uint8_t lenData){

    PORTB &= ~(1 << SS2);

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

    PORTB |= (1 << SS2);
}

Buttons ioButton(void){
    uint8_t data[3];
    SPIMasterReceive(BUTTONS, data, 3);
    return (Buttons) {data[0], data[1], data[2]};
}

void ledInit(void){
    DDRD |= (1 << PD2);
    PORTD &= ~(1 << PD2); 
}

void ledToggle(void){
    PORTD ^= (1 << PD2);
}

Buttons buttonTest(Buttons prevButtons){

    Buttons buttons = ioButton();

    // printf("buttons: %x\n", buttons);
    if (prevButtons.left != buttons.left && buttons.left != 0) {printf("buttons.left = 0x%02X\n", buttons.left);}
    //button SR3 toggles constantly anyway, so it has a defect and we choose to not use it
    if (prevButtons.right != buttons.right && buttons.right != 0 && buttons.right != 0x04) {printf("buttons.right = 0x%02X\n", buttons.right);}
    if (prevButtons.nav != buttons.nav && buttons.nav != 0) {printf("buttons.nav = 0x%02X\n", buttons.nav);}
    

    if(buttons.left == 1){
        ledToggle();
        printf("halla");
    }

    return buttons;

}

void SPIMasterTransmitDisplay(char cData){
    SPIMasterTransmit(cData, 1); 
}

