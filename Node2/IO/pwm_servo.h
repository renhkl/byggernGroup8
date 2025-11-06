#ifndef PWM_SERVO_H
#define PWM_SERVO_H

#include <stdint.h>


void pwmInit();
void changeDutyCycle();
void interuptInit()
void changeDutyCycle(int dutyCycle);
void changeServoPosition(uint8_t degrees);
int degreesToDutyCycle(int degrees);



#endif 