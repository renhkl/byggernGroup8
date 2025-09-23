#ifndef ADC_JOYSTICK_H
#define ADC_JOYSTICK_H

#include <stdio.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include <avr/io.h>

// struct getJoystickPosition(void);
// enum getJoystickDirection(void);
void counterInit(void);
int adcRead(void);

#endif // ADC_JOYSTICK_H