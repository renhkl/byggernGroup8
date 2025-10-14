#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "latch_test.h"
#include "sram_test.h"
#include "xmem.h"
#include "adc_joystick.h"
#include <avr/interrupt.h>

#include "uart.h"
#include "delay.h"

#include "spi.h"

// #define F_CPU 4952000UL
#define F_CPU 4915200UL

// uart baudrate
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1


int main()
{

    uartInit(MYUBRR);
    xmem_init();
    counterInit();
    SPIMasterInit();
    //ledInit();
    // printf("%d\n", adcRead());
    printf("LIZZARD");
      oledInit();

      DDRD &= ~(1 << DDD5); //set pin D5 as input (joystick pin)
 
    // SRAM_cr_test();
    // ADC_test();

    oledSetBrightness(255);
    oledClearScreen();
    oledMakeTextMenu();

 
      int row = 0;
      int column = 0;

    Buttons buttons = ioButton();

    while (1)
    {
        
        buttons = buttonTest(buttons);
        uint8_t *values = adcReap();
        int xPercentage = joystickPercentage(values[3]);
        int yPercentage = joystickPercentage(values[2]);
        //printf("TP_X: %d  TP_Y: %d  JS_Y: %d  JS_X: %d\n",
        //       values[0], values[1], yPercentage, xPercentage);
        enum Direction direction = getJoystickDirection(xPercentage, yPercentage);
        //printf(" DIRECTION: ");
        printDirection(direction);
        //printf(" Angle: %d\n", voltageToAngle(values));

        //submenu

        //getJoystickButtonPosition(direction, row, column);
        //printf("%d \n", getJoystickButtonPosition());

        
        if (direction == UP) {
          if (row > 0){
             removeSymbol(row, 1);
            row--;
            oledPrintArrow(row, 1);
            _delay_ms(500);
          }

        }

        else if (direction == DOWN) {
          if (row < 7) {
          removeSymbol(row, 1);
          row++;
          oledPrintArrow(row, 1);
          _delay_ms(500);
          }

        }

        else if (direction == LEFT) {
          if (column > 0) {
            column--;
               _delay_ms(500);
        }
        }

        else if (direction == RIGHT) {
          if(column < 127) {
            column++;
               _delay_ms(500);
        }
        }

         if (!(PIND & (1 << PIN5))) {
         printf("%d %d\n", row, column);
        }


       

        
        

  
        



        // SPIMasterTransmit(0b00110011, 1);
        
       

        // chip select test
        // xmem_write(0xF0, 0x1D00); //sram address, active high
        // xmem_write(0x0F, 0x1500);//adc address, active low

        // printf("%d\n", adcRead());
        // printf("%d\n", adcRead());
        // printf("%d\n", adcRead());
        //_delay_ms(100);

        // char c = getchar();
        // printf("You typed: %c\n", c);
    }

    return 0;

}
