#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void pwmMotorInit();
void changeMotorPosition(uint8_t degrees);
void setMotorDirection(uint8_t direction);

void motorEnable();
void motorDisable();

void timerCounterInit();
int getDecoderValue();
void decoderInit();

int desiredMotorPosition(int currentPosition, uint8_t motorAngle);

#endif