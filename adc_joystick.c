#include "adc_joystick.h"
#include <avr/io.h>

// joystick init with calibrate?

void counterInit(void) // timer/counter 3
{

    // wgm
    // 1114
    DDRD |= (1 << DDD4);
    PORTD ^= (1 << PIN4);

    // 01 toggle, 10 clear, 11 set
    // TCCR3A |= (1 << COM3A1);
    TCCR3A |= (1 << COM3A0); //

    TCCR3B |= (1 << CS31);  // prescaler = clk/8
    TCCR3A |= (1 << WGM32); // CTC with OCRnA

    // accessing 16 bit reg on page 109
    // compare value
    OCR3AH = 10;
    OCR3AL = 255;
}

/*
void adcInit(void)
{
}

// uint8_t adc_read(uint8_t channel) { //volatile
//
// }

// function to return analog position
struct getJoystickPosition(void)
{

    return 1;

}

// return joystick direction
enum getJoystickDirection(void) {
    enum direction{
        LEFT,
        RIGHT,
        UP,
        DOWN,
        NEUTRAL};

    return direction;
}

void pinClock()
{ // using pin D4
    DDRD |= (1 << DDD4);
    PORTD ^= (1 << PIN4);
    _delay_ms(100); // unknown delay
}
    */