#ifndef SOLENOID_H
#define SOLENOID_H

#include <stdint.h>

void pulseSolenoid(uint8_t solenoidState);
void toggleSolenoid();
void activateSolenoid();
void deactivateSolenoid();

#endif