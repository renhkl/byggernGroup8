#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include "adc_joystick.h"
#include "buttons.h"
#include <avr/io.h>

#define DISPLAY_OFF 0xAE
#define SEGMENT_REMAP 0xA1
#define COM_PINS_HARDWARE_CONFIGURATION 0xDA
#define COM_OUTPUT_SCAN_DIRECTION 0xC8  
#define MULTIPLEXER_RATIO 0xA8
#define DISPLAY_CLOCK_DIVIDER_RATIO 0xD5 
#define CONTRAST_CONTROL_FOR_BANK0 0x81
#define PRE_CHARGE_PERIOD 0xD9
#define MEMORY_ADDRESSING_MODE 0x20
#define VCOMH_DESELECT_LEVEL 0xDB
#define ENTIRE_DISPLAY_ON 0xA4 //out follows RAM contents
#define NORMAL_DISPLAY 0xA6
#define DISPLAY_ON 0xAF

#define DISPLAY_SS 1
#define ASCII_OFFSET 32


struct joystickCoordinates {
    int row;
    int column;
};


void oledInit();
void goToLine(uint8_t line);
void goToColumn(uint8_t column);
void printScreen();
void oledReset();
void oledHome();
void oledClearPage(uint8_t page);
void oledClearScreen();
void oledSetPosition(uint8_t line, uint8_t column);
void oledWriteData(uint8_t data);
void oledWriteCommand(uint8_t command);
void oledSetBrightness(uint8_t lvl);
void oledPrintChar(char c);
void oledPrintString(char *str);
void removeSymbol(uint8_t row, uint8_t col);

//IO functions
void moveArrowWithJoystick(uint8_t *row, uint8_t *column, enum Direction direction);
int getJoystickButtonPosition(enum Direction direction, int row, int column);
void test();


//graphics

void oledPrintArrow(uint8_t row, uint8_t col);
void oledMakeTextMenu();

void oledPrintCircle(uint8_t row, uint8_t col);

uint8_t moveArrowWithNav(Buttons buttons);
#endif