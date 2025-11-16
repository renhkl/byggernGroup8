#ifndef IR_PHOTODIODE_H
#define IR_PHOTODIODE_H

#include <stdint.h>

typedef struct{
    int size;
    double *data;
    int head;
    int count;
} ringBufferGoals;

uint8_t countGoals(uint8_t *goals, int sensor, unsigned long goalDelay);
int readPhotodiode();
void adcInit();
void adcINIT();
int adcRead();
uint8_t isGoal(ringBufferGoals goalArray);

double ringBufferGoalsGet(ringBufferGoals *rb, int index);
void ringBufferGoalsAdd(ringBufferGoals *rb, double value);
void ringBufferGoalsInit(ringBufferGoals *rb, uint8_t size);

#endif