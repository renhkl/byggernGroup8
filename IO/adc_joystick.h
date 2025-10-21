#ifndef ADC_JOYSTICK_H
#define ADC_JOYSTICK_H

#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include <avr/io.h>

// struct getJoystickPosition(void);
// enum getJoystickDirection(void);
void counterInit(void);
uint8_t *adcRead(void);
enum Direction getJoystickDirection(int Xpercentage, int Ypercentage);
int joystickPercentage(int value);
uint8_t voltageToAngle(uint8_t *values);
void printDirection(enum Direction dir);

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
};

#endif // ADC_JOYSTICK_H