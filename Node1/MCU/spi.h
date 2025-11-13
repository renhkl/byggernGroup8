#ifndef SPI_H
#define SPI_H
#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

typedef enum {
    TOUCH_PAD = 0x01,
    TOUCH_SLIDER = 0x02,
    JOYSTICK = 0x03,
    BUTTONS = 0x04,
    LED_ON_OFF = 0x05,
    LED_PWM = 0x06,
    INFO = 0x07

} IOcommands;

void SPIMasterInit(void);
void SPIMasterTransmit(char cData, int slave);
void SPIMasterReceive(IOcommands command, char *data, uint8_t lenData);
uint8_t SPIMasterReceiveCAN(uint8_t command);
void SPIMasterTransmitDisplay(char cData);
int readByte();
void writeByte(int byte);
void writeNBytes(int *bytes, int n);
int readNBytes(int n);

//For testing io receive
void ledInit(void);
void ledToggle(void);

#endif