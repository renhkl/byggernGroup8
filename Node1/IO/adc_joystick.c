#include "adc_joystick.h"
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>

// joystick init with calibrate?

void counterInit(void) // timer/counter 3
{
    DDRD |= (1 << DDD4); //pin D4 to output

    TCCR3A |= (1 << COM3A0); //toggle on compare match
  
    // Waveform generation mode (CTC)
  
    TCCR3B |= (1 << WGM32);

    // Prescaler (satt til 1)
    TCCR3B |= (1 << CS30);
    
    //compare value
    OCR3AH = 0;
    OCR3AL = 100;

}

enum Direction getJoystickDirection() {
    uint8_t *values = adcRead();
    int Xpercentage = joystickPercentage(values[3]);
    int Ypercentage = joystickPercentage(values[2]);
    return JoystickPercentageToDirection(Xpercentage, Ypercentage);
}

uint8_t *adcRead(void)
{
    static uint8_t values[4]; 

    volatile uint8_t *adr = (volatile uint8_t *)0x1000;
    adr[0] = 0;
    while (!(PINE & (1 << PE2))) //check busy signal
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
    //printf("VAL %d ",value);
    int percentage = (value - 160) * 200 / 160;
    //printf("PERC %d ",percentage);
    return percentage;
}

uint8_t joystickToServoDegrees(int value)
{
    //printf("VAL %d ",value);
    double percentage = ((double)value * 5 - 316) * 45 / 236;
    if (percentage < 0) return 0;
    if (percentage > 180) return 180;
    if (percentage > 80 && percentage < 100) return 90;
    return (uint8_t)percentage;
}

uint8_t joystickToMotorDegrees(int value)
{
    //printf("VAL %d ",value);
    double percentage = ((((double)value * 5 - 200) * 45 / 236)-19) * 180/165;
    if (percentage < 0) return 0;
    if (percentage > 180) return 180;
    if (percentage > 80 && percentage < 100) return 90;
    return (uint8_t)percentage;
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
enum Direction JoystickPercentageToDirection(int Xpercentage, int Ypercentage)
{
    enum Direction direction;
    if (Xpercentage < -40)
    {
        direction = LEFT;
    }
    else if (Xpercentage > 40)
    {
        direction = RIGHT;
    }
    else if (Ypercentage > 40)
    {
        direction = UP;
    }
    else if (Ypercentage < -40)
    {
        direction = DOWN;
    }
    else
    {
        direction = NEUTRAL;
    }
    return direction;
}

uint8_t JoystickPercentageToServoAngle(int deltaX, int deltaY)
{
    double rad = atan2((double)deltaY, (double)deltaX);

    uint8_t deg = (uint8_t)((rad * 180 / M_PI));

    return deg;
}

uint8_t getJoystickState(int deltaX, int deltaY)
{
     if (JoystickPercentageToDirection(deltaX, deltaY) == NEUTRAL) {return 0;} //IDLE STATE
     if (deltaY >= 0) {return 1;} // Upper hemisphere, SERVO STATE
     if (deltaY < 0) {return 2;} // Lower hemisphere, MOTOR STATE
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
    */

