#ifndef SOLENOID_H
#define SOLENOID_H

#include <stdint.h>
#include <time.h>

void pulseSolenoid(uint8_t solenoidState);
void activateSolenoid();

#endif