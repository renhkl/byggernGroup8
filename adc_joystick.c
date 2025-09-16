#include "adc_joystick.h"
#include <avr/io.h>

//joystick init with calibrate?


void counterInit(void) {
    
    TCCR3A |= (1 << WGM32); //CTC with OCRnA
    TCCR3B |= (1 << CS11); //prescaler = clk/8


    //accessing 16 bit reg on page 109
    //compare value
    OCR3AH = 800;
    OCR3AL = 50;
}


void adcInit(void) {




}

//uint8_t adc_read(uint8_t channel) { //volatile
//
//}

//function to return analog position 
struct getJoystickPosition(void) {

    return 1;

}

//return joystick direction
enum getJoystickDirection(void) {
    enum direction {
        LEFT,
        RIGHT, 
        UP,
        DOWN, 
        NEUTRAL
    };

    return direction;
}

void pinClock() { //using pin D4
     DDRD |= (1 << DDD4);
     PORTD ^= (1 << PIN4);
     _delay_ms(100); //unknown delay
     
}