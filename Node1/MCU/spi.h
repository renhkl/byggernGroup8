#ifndef SPI_H
#define SPI_H
#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define SR1 0x01
#define SR2 0x02
#define SR3 0x04
#define SR4 0x08
#define SR5 0x10
#define SR6 0x20

#define SL1 0x01
#define SL2 0x02
#define SL3 0x04
#define SL4 0x08
#define SL5 0x10
#define SL6 0x20
#define SL7 0x40

#define NAV_BUTTON 0x01
#define NAV_RIGHT 0x02
#define NAV_DOWN 0x04
#define NAV_LEFT 0x08
#define NAV_UP 0x10



typedef enum {
    TOUCH_PAD = 0x01,
    TOUCH_SLIDER = 0x02,
    JOYSTICK = 0x03,
    BUTTONS = 0x04,
    LED_ON_OFF = 0x05,
    LED_PWM = 0x06,
    INFO = 0x07

} IOcommands;

typedef struct __attribute__((packed)) {

     union {
        uint8_t nav;
        struct {
        uint8_t NB:1;
        uint8_t NR:1;
        uint8_t ND:1;
        uint8_t NL:1;
        uint8_t NU:1;
        };
    };
      
    union {
        uint8_t right;
        struct {
        uint8_t R1:1;
        uint8_t R2:1;
        uint8_t R3:1;
        uint8_t R4:1;
        uint8_t R5:1;
        uint8_t R6:1;
        };
    };

      union {
        uint8_t left;
        struct {
        uint8_t L1:1;
        uint8_t L2:1;
        uint8_t L3:1;
        uint8_t L4:1;
        uint8_t L5:1;
        uint8_t L6:1;
        uint8_t L7:1;
        };
    };

   
} Buttons;

void SPIMasterInit(void);
void SPIMasterTransmit(char cData, int slave);
void SPIMasterReceive(IOcommands command, char *data, uint8_t lenData);
uint8_t SPIMasterReceiveCAN(uint8_t command);
void SPIMasterTransmitDisplay(char cData);
int readByte();
void writeByte(int byte);
void writeNBytes(int *bytes, int n);
int readNBytes(int n);

Buttons ioButton(void);

//For testing io receive
void ledInit(void);
void ledToggle(void);
Buttons buttonTest(Buttons prevButtons);

#endif