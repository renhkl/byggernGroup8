#ifndef ADC_JOYSTICK_H
#define ADC_JOYSTICK_H

#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include <avr/io.h>

// struct getJoystickPosition(void);
// enum JoystickPercentageToDirection(void);
void counterInit(void);
uint8_t *adcRead(void);
enum Direction JoystickPercentageToDirection(int Xpercentage, int Ypercentage);
int joystickPercentage(int value);
uint8_t JoystickPercentageToServoAngle(int deltaX, int deltaY);
uint8_t getJoystickState(int deltaX, int deltaY);
void printDirection(enum Direction dir);
enum Direction getJoystickDirection();
uint8_t joystickToServoDegrees(int value);
uint8_t joystickToMotorDegrees(int value);

enum Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
};

#endif // ADC_JOYSTICK_H