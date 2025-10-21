//this driver will implement CAN communication on mcp2515 but should work independent of hardware

#ifndef CAN_H
#define CAN_H

#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

struct frame {
    uint8_t id;
    uint8_t data;
    uint8_t length;
};

#endif //CAN