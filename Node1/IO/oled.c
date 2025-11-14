#include "oled.h"
#include "../MCU/spi.h"
#include "fonts.h"
#include "adc_joystick.h"
#include <avr/pgmspace.h>
#include "buttons.h"

void oledPrintArrow(uint8_t row, uint8_t col) {
    oledSetPosition(row, col);
    oledWriteData(0b00011000);
    oledWriteData(0b00011000);
    oledWriteData(0b01111110);
    oledWriteData(0b00111100);
    oledWriteData(0b00011000);
}



void oledPrintCircle(uint8_t row, uint8_t col) {
    oledSetPosition(row, col);
    oledWriteData(0b00111100);
    oledWriteData(0b01111110);
    oledWriteData(0b11111111);
    oledWriteData(0b01111110);
    oledWriteData(0b00111100);
}

void removeSymbol(uint8_t row, uint8_t col) {
    oledSetPosition(row, col);
    oledWriteData(0);
    oledWriteData(0);
    oledWriteData(0);
    oledWriteData(0);
    oledWriteData(0);

}

void oledInit() { //datasheet

   DDRB |= (1 << PB2);
   //PORTB |= (1 << PIN2);
    PORTB &= ~(1 << PIN2);

SPIMasterTransmitDisplay(DISPLAY_OFF);
SPIMasterTransmitDisplay(SEGMENT_REMAP);
//SPIMasterTransmitDisplay(COM_PINS_HARDWARE_CONFIGURATION);
//SPIMasterTransmitDisplay(0x12);
SPIMasterTransmitDisplay(COM_OUTPUT_SCAN_DIRECTION);
//SPIMasterTransmitDisplay(MULTIPLEXER_RATIO);
/*SPIMasterTransmitDisplay(0x3F);
SPIMasterTransmitDisplay(DISPLAY_CLOCK_DIVIDER_RATIO);
SPIMasterTransmitDisplay(0x80);
SPIMasterTransmitDisplay(CONTRAST_CONTROL_FOR_BANK0);
SPIMasterTransmitDisplay(0x50);
SPIMasterTransmitDisplay(PRE_CHARGE_PERIOD);
SPIMasterTransmitDisplay(0x21);
SPIMasterTransmitDisplay(MEMORY_ADDRESSING_MODE);
SPIMasterTransmitDisplay(0x02);
SPIMasterTransmitDisplay(VCOMH_DESELECT_LEVEL);
SPIMasterTransmitDisplay(0x30);
SPIMasterTransmitDisplay(0xAD);
SPIMasterTransmitDisplay(0x00);
SPIMasterTransmitDisplay(ENTIRE_DISPLAY_ON);
SPIMasterTransmitDisplay(NORMAL_DISPLAY);
*/
SPIMasterTransmitDisplay(DISPLAY_ON);

}

void oledWriteData(uint8_t data) { //datasheet

    //data = ~data; //background becomes white

    DDRB |= (1 << PB2);
    PORTB |= (1 << PIN2);
    SPIMasterTransmitDisplay(data);
    PORTB &= ~(1 << PIN2);


}

void oledWriteCommand(uint8_t command) { //datasheet

    DDRB |= (1 << PB2);
    PORTB &= ~(1 << PIN2);
    SPIMasterTransmitDisplay(command);


}

void goToLine(uint8_t line) {
    line = line | 0xB0;
    oledWriteCommand(line);
}

void goToColumn(uint8_t column) {

    uint8_t higher_nibble = (column >> 4);

    higher_nibble = higher_nibble | 0x10;
    higher_nibble &= 0x1F;

    uint8_t lower_nibble = column & 0x0F;
    
    oledWriteCommand(lower_nibble);
    oledWriteCommand(higher_nibble);
}

void oledSetPosition(uint8_t line, uint8_t column) {
    goToLine(line);
    goToColumn(column);
}

void oledClearPage(uint8_t line) {
    goToLine(line);
    goToColumn(0);
    for (int i=0; i<128; i++) {
        oledWriteData(0x0); //0 = black, FF = white
    }
}

void oledClearScreen() {
    
    oledClearPage(0);
        oledClearPage(1);
        oledClearPage(2);
        oledClearPage(3);
        oledClearPage(4);
        oledClearPage(5);
        oledClearPage(6);
        oledClearPage(7);

}

void oledSetBrightness(uint8_t lvl) {
    oledWriteCommand(CONTRAST_CONTROL_FOR_BANK0);
    oledWriteCommand(lvl);
}

void oledPrintChar(char c) {
 
    int index = c - ASCII_OFFSET;
    //unsigned char charData[1][8] = pgm_read_byte(*font8[index][8]);
    //printf(pgm_read_word(&font8[index][0]));
    unsigned char charData[9] = //{pgm_read_byte(&font8[index])};
    { pgm_read_byte(&font8[index][0]),
    pgm_read_byte(&font8[index][1]),
    pgm_read_byte(&font8[index][2]),
    pgm_read_byte(&font8[index][3]),
    pgm_read_byte(&font8[index][4]),
    pgm_read_byte(&font8[index][5]),
    pgm_read_byte(&font8[index][6]),
    pgm_read_byte(&font8[index][7])};
    
    
    
  /*
    printf("%d %d %d\n", c, index, pgm_read_byte(&font8[0]));
    printf("%d %d %d %d %d %d %d %d \n", pgm_read_byte(&font8[index][0]),
    pgm_read_byte(&font8[index][1]),
    pgm_read_byte(&font8[index][2]),
    pgm_read_byte(&font8[index][3]),
    pgm_read_byte(&font8[index][4]),
    pgm_read_byte(&font8[index][5]),
    pgm_read_byte(&font8[index][6]),
    pgm_read_byte(&font8[index][7]));
   

oledWriteData(0b00000000);
oledWriteData(0b00000110);
oledWriteData(0b01011111);
oledWriteData(0b01011111);
oledWriteData(0b00000110);
oledWriteData(0b00000000);
oledWriteData(0b00000000);
oledWriteData(0b00000000);
*/


    for (int i = 0; i < 9; i++) {

        oledWriteData(charData[i]);
        //printf("%d\n", charData[i]);

    }
        
        
}

void oledPrintString(char *str) {
    while (*str != '\0') {
        oledPrintChar(*str);
        str++;
    }
}


int getJoystickButtonPosition(enum Direction direction, int row, int column) {
    DDRD &= ~(1 << DDD5); //set pin D5 as input

    /*
    struct joystickCoordinates jsCoord;
    jsCoord.row = row;
    jsCoord.column = column;
    */

    switch (direction) {
        case LEFT:
        if (column > 0 ) {
            column--;
               _delay_ms(500);
        }
     
        break;
        
        case RIGHT:
        if(column < 127) {
            column++;
               _delay_ms(500);
        }
        break;
        
        case UP:
        if (row > 0) {
            removeSymbol(row, 1);
            row--;
             oledPrintArrow(row, 1);
            _delay_ms(500);
          }
        break;

        case DOWN:
         if (row < 7) {
             removeSymbol(row, 1);
          row++;
           oledPrintArrow(row, 1);
          _delay_ms(500);
          }
        break;
    }

    
    if (!(PIND & (1 << PIN5))) {
        return row;
    }
    else {
        return column;
    }

    //oledSetPosition(row, column);


}

void oledMakeHomeMenu() {
     
      oledSetPosition(0, 9);
      oledPrintString("highscore");
           
      oledSetPosition(1, 9);
      oledPrintChar('0');

      oledSetPosition(2, 9);
      oledPrintString("play");

      oledSetPosition(3, 9);
      oledPrintString("exit");

      oledPrintArrow(2, 1);
}

void oledMakePlayMenu() {
     
      oledSetPosition(0, 9);
      oledPrintString("PLAYING");
           
      oledSetPosition(2, 9);
      oledPrintString("Score");

      oledSetPosition(2, 57);
      oledPrintChar('0');

      oledSetPosition(4, 9);
      oledPrintString("exit");

      oledPrintArrow(4, 1);
}

void oledUpdateScore(uint8_t score) {
    char str[4];
    sprintf(str, "%d", score);
    
    oledSetPosition(2, 57);
    oledPrintString(str);
}

void moveArrowWithJoystick(uint8_t *row, uint8_t *column, enum Direction direction) {
        uint8_t page = *row;
        uint8_t col = *column;
    
        if (direction == UP) {
          if (page > 0){
             removeSymbol(page, 1);
            page--;
            oledPrintArrow(page, 1);
            _delay_ms(500);
          }

        }

        else if (direction == DOWN) {
          if (page < 7) {
          removeSymbol(page, 1);
          page++;
          oledPrintArrow(page, 1);
          _delay_ms(500);
          }

        }

        else if (direction == LEFT) {
          if (col > 0) {
            col--;
               _delay_ms(500);
        }
        }

        else if (direction == RIGHT) {
          if(col < 127) {
            col++;
               _delay_ms(500);
        }
        }

         if (!(PIND & (1 << PIN5))) { //get joystick position on button press
         printf("%d %d\n", page, col);
        }

        //update the global variables again
        *row = page;
        *column = col;
}

uint8_t moveArrowHomeMenu(Buttons buttons) {
        static uint8_t page = 2;
        static uint8_t state = 0;

        if (buttons.NU) {
          if (page > 2){
            removeSymbol(page, 1);
            page--;
            oledPrintArrow(page, 1);
          }

        }

        else if (buttons.ND) {
          if (page < 3) {
          removeSymbol(page, 1);
          page++;
          oledPrintArrow(page, 1);
          }
        }

        else if (buttons.NB && page == 2) {state = 1;}
        else if (buttons.NB && page == 3) {state = 0;}
        return state;
}

uint8_t moveArrowPlayMenu(Buttons buttons) {
        static uint8_t page = 4;
        static uint8_t state = 1;

        if (buttons.NU) {
          if (page > 4){
            removeSymbol(page, 1);
            page--;
            oledPrintArrow(page, 1);
          }

        }

        else if (buttons.ND) {
          if (page < 4) {
          removeSymbol(page, 1);
          page++;
          oledPrintArrow(page, 1);
          }
        }

        else if (buttons.NB && page == 4) {state = 0;}
        return state;
}