#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void pwmMotorInit();
void changeMotorPosition(uint8_t degrees);
void setMotorDirection(uint8_t direction);
void timerCounterInit();
int getDecoderValue();
void decoderInit();

#endif