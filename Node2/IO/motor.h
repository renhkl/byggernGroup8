#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void pwmMotorInit();
void motorStartPosition();
void setMotorVelocity(int DutyCycle);
void setMotorDirection(uint8_t direction);

void timerCounterInit();
int getDecoderValue();
void decoderInit();

int desiredMotorPosition(int currentPosition, uint8_t joystickAngle, int motorPosition);

#endif