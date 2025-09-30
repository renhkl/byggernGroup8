#ifndef OLED_H
#define OLED_H

#include <stdio.h>

void oledInit();
void goToLine();
void goToColumn();
void printScreen();
void oledReset();
void oledHome();
void oledClearLine();
void oledSetPosition();
void oledWriteData(uint8_t data);

//graphics

void oledPrintArrow(uint8_t row, uint8_t col);

#endif