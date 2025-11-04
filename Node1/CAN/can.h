//this driver will implement CAN communication on mcp2515 but should work independent of hardware

#ifndef CAN_H
#define CAN_H

#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

#define BYTE_SIZE 8

typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t data[8];
} frame;

typedef struct {
    uint8_t phaseSeg2;  // Phase 2 segment
    uint8_t propagTimeSeg;  // Propagation time segment
    uint8_t phaseSeg1;  // Phase 1 segment
    uint8_t syncJumpWidth;     // Synchronization jump width
    uint8_t baudRatePrescaler;     // Baud rate prescaler
    uint8_t samplingMode;     // Sampling mode
} canTimeConfig;

void canSetLoopbackMode();
void canTransmit(frame message);
frame canReceive();

#endif //CAN