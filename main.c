#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "TEST/latch_test.h"
#include "TEST/sram_test.h"
#include "MCU/xmem.h"
#include "IO/adc_joystick.h"
#include "IO/oled.h"
#include <avr/interrupt.h>

#include "MCU/uart.h"
#include "delay.h"
#include "CAN/mcp2515.h"

#include "MCU/spi.h"

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
    mcp2515Init();
    //ledInit();
    // printf("%d\n", adcRead());
    printf("LIZZARD\n");
    _delay_ms(1000);
    oledInit();

    DDRD &= ~(1 << DDD5); //set pin D5 as input (joystick pin)
 
    // SRAM_cr_test();
    // ADC_test();

    oledSetBrightness(255);
    oledClearScreen();
    oledMakeTextMenu();

 
      uint8_t row = 0;
      uint8_t column = 0;

    Buttons buttons = ioButton();

    

    canSetLoopbackMode();
    uint8_t read = mcp2515Read(MCP_CANSTAT);
    //printf("MCP_CANSTAT VALUE: %d\n", read);
    if (read == 255) {
      //printf("INVALID MCP READING, CHECK YOUR CONNECTIONS\n");
    }

    while (1)
    {
      uint64_t x = 25;
      canTransmit(1, x);
      printf("CAN READ VALUE: %d\n", canReceive());
      
      /*
        buttons = buttonTest(buttons);
        uint8_t *values = adcRead();
        int xPercentage = joystickPercentage(values[3]);
        int yPercentage = joystickPercentage(values[2]);
        //printf("TP_X: %d  TP_Y: %d  JS_Y: %d  JS_X: %d\n",
        //       values[0], values[1], yPercentage, xPercentage);
        enum Direction direction = getJoystickDirection(xPercentage, yPercentage);
        moveArrowWithJoystick(&row, &column, direction);
        //printf(" DIRECTION: ");
        //printDirection(direction);
        //printf(" Angle: %d\n", voltageToAngle(values));

        //submenu

        //getJoystickButtonPosition(direction, row, column);
        //printf("%d \n", getJoystickButtonPosition());


        // SPIMasterTransmit(0b00110011, 1);
        */
       

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
