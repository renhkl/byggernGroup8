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
    // printf("%d\n", adcRead());
    printf("LIZZARD");
      oledInit();
 
  

    // SRAM_cr_test();
    // ADC_test();

    oledSetBrightness(255);
    oledClearScreen();
    oledPrintArrow(2, 0b01010101);
       oledSetPosition(2, 4);
         oledPrintChar('h');
      oledPrintChar('i');
           oledPrintChar('g');
      oledPrintChar('h');
             oledPrintChar('s');
      oledPrintChar('c');
           oledPrintChar('o');
      oledPrintChar('r');
           oledPrintChar('e');

           oledSetPosition(3, 4);

                 oledPrintChar('p');
      oledPrintChar('l');
           oledPrintChar('a');
      oledPrintChar('y');

      oledSetPosition(4, 4);
             oledPrintChar('e');
      oledPrintChar('x');
           oledPrintChar('i');
      oledPrintChar('t');



    


    while (1)
    {
        /*
        uint8_t *values = adcReap();
        int xPercentage = joystickPercentage(values[3]);
        int yPercentage = joystickPercentage(values[2]);
        printf("TP_X: %d  TP_Y: %d  JS_Y: %d  JS_X: %d\n",
               values[0], values[1], yPercentage, xPercentage);
        enum Direction direction = getJoystickDirection(xPercentage, yPercentage);
        printf(" DIRECTION: ");
        printDirection(direction);
        printf(" Angle: %d\n", voltageToAngle(values));
        */



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
