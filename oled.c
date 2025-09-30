#include "oled.h"

void oledPrintArrow(uint8_t row, uint8_t col) {
    oledSetPosition(row, col);
    oledWriteData(0b00011000);
    oledWriteData(0b00011000);
    oledWriteData(0b01111110);
    oledWriteData(0b00111100);
    oledWriteData(0b00011000);

}