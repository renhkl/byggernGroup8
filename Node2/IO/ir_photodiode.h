#ifndef IR_PHOTODIODE_H
#define IR_PHOTODIODE_H

#include <stdint.h>

uint8_t countGoals(uint8_t *goals, int sensor, unsigned long goalDelay);
int readPhotodiode();
void adcInit();
void adcINIT();
int adcRead();
uint8_t isGoal(int sensorValue);

#endif