#include "adc_joystick.h"
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>

// joystick init with calibrate?

void counterInit(void) // timer/counter 3
{

    DDRD |= (1 << DDD4);

    // TCCR3B |= (1 << CS30); // prescaler = clk/1
    // TCCR3B |= (1 << CS31); // prescaler = clk/8
        // 01 toggle, 10 clear, 11 set
    // TCCR3A |= (1 << COM3A1);
    // TCCR3A |= (1 << COM3A0); //toggle


    // TCCR3B |= (1 << CS32);

    ///////////////////////////////////////////////
    /*
    TCCR3A |= (1 << WGM32); // CTC with OCRnA
    TCCR3A |= (1 << FOC3A);

    // accessing 16 bit reg on page 109
    // compare value
    OCR3AH = 0;
    OCR3AL = 100;

    cli();

    TIFR |= (1 << OCF3A);
    TIMSK |= (1 << OCIE3A);

    sei();
    //TOV3
    */

    // TCCR3A |= (1 << COM3A1);
    TCCR3A |= (1 << COM3A0); //toggle on compare match
    // TCCR3A |= (1 << COM3B1);
    // TCCR3A |= (1 << COM3B0);

    // TCCR3A |= (1 << FOC3A);
    // TCCR3A |= (1 << FOC3B);
    

    // TCCR3B |= (1 << ICNC3);
    // TCCR3B |= (1 << ICES3);

    // Waveform generation mode (CTC)
    // TCCR3A |= (1 << WGM31);
    // TCCR3A |= (1 << WGM30);
    // TCCR3B |= (1 << WGM33);
    TCCR3B |= (1 << WGM32);

    // Prescaler (satt til 1)
    // TCCR3B |= (1 << CS32);
    // TCCR3B |= (1 << CS31);
    TCCR3B |= (1 << CS30);
    
    OCR3AH = 0;
    OCR3AL = 100;

}

int adcRead(void) //not used
{

    xmem_read(0x1500); // read from adc so chip select
    // send signal to read from ADC. it happens automatically
    /*DDRD |= (1 << DDD7);
    PORTD &= ~(1 << PIN7);
    _delay_us(0.100);
    PORTD |= (1 << PIN7);
    _delay_us(0.180);
    // PORTD &= ~(1 << PIN7);
    //_delay_us(100);
    */

    DDRA |= 0x00; // set port A as input
    // PORTA |= 0xFF;            // set as pullups
    int joystickValue = PINA; // read value on port A
    return joystickValue;
}

uint8_t *adcReap(void)
{
    static uint8_t values[4];

    volatile uint8_t *adr = (volatile uint8_t *)0x1000;
    adr[0] = 0;
    while (!(PINE & (1 << PE2)))
    {
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        values[i] = adr[i];
    }
    return values;
}

int joystickPercentage(int value)
{
    int percentage = (value - 160) * 200 / 160;
    return percentage;
}

void printDirection(enum Direction dir)
{
    switch (dir)
    {
    case LEFT:
        printf("LEFT");
        break;
    case RIGHT:
        printf("RIGHT");
        break;
    case UP:
        printf("UP");
        break;
    case DOWN:
        printf("DOWN");
        break;
    case NEUTRAL:
        printf("NEUTRAL");
        break;
    default:
        break;
    }
}

// return joystick direction
enum Direction getJoystickDirection(int Xpercentage, int Ypercentage)
{
    enum Direction direction;
    if (Xpercentage < -33)
    {
        direction = LEFT;
    }
    else if (Xpercentage > 33)
    {
        direction = RIGHT;
    }
    else if (Ypercentage > 33)
    {
        direction = UP;
    }
    else if (Ypercentage < -33)
    {
        direction = DOWN;
    }
    else
    {
        direction = NEUTRAL;
    }
    return direction;
}

uint8_t voltageToAngle(uint8_t *values)
{
    double deltaX = joystickPercentage(values[3]);
    double deltaY = joystickPercentage(values[2]);
    double rad = atan2(deltaY, deltaX);

    uint8_t deg = (uint8_t)((rad * 180 / M_PI));

    return deg;
}
/*
// uint8_t adc_read(uint8_t channel) { //volatile
//
// }

// function to return analog position
struct getJoystickPosition(void)
{

    return 1;

}



void pinClock()
{ // using pin D4
    DDRD |= (1 << DDD4);
    PORTD ^= (1 << PIN4);
    _delay_ms(100); // unknown delay
}
    */