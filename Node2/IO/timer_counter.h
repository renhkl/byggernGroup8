#ifndef TIMER_COUNTER
#define TIMER_COUNTER

#include <sam.h>
#include <stdint.h>

uint8_t setFlag(uint8_t setget);
void regulatorTimertInit(void);
void TC0_Handler(void);

#endif