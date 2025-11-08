#ifndef PWM_SERVO_H
#define PWM_SERVO_H

#include <stdint.h>


void pwmInit12();
void pwmInit13();
void interruptInit();
void interruptHandler();
void changeDutyCycle(int dutyCycle, int channel);
void changeServoPosition(uint8_t degrees);
int degreesToDutyCycle(int degrees);
uint8_t lowpassFilter(float input, float *prevOutput, float alpha);


#endif 