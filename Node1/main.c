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
#include "CAN/can.h"

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
    //printf("LIZARD\n");
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

    uint8_t read = mcp2515Read(MCP_CANSTAT);
    //printf("MCP_CANSTAT VALUE: %d\n", read);
    if (read == 255) {
      //printf("INVALID MCP READING, CHECK YOUR CONNECTIONS\n");
    }



    canTimeConfig tConfig = {
        .phaseSeg2 = 4,
        .propagTimeSeg = 1,
        .phaseSeg1 = 5,
        .syncJumpWidth = 3,
        .baudRatePrescaler = 3,
        .samplingMode = 0
    };

    canInit(tConfig);

    frame m = {
      .id = 3,
      .length = 2,
      .data = {4, 7}
    };



    while (1)
    {
      /*
      //CAN receive
      frame m = canReceive();
      printf("\nCAN READ VALUE ID %d\t LENGTH %d\t DATA ", m.id, m.length);
      for (uint8_t i = 0; i < m.length; i++) {printf("%d ",m.data[i]);}
      */

      
      uint8_t *values = adcRead();
      int xPercentage = joystickPercentage(values[3]);
      int yPercentage = joystickPercentage(values[2]);

      /*
      enum Direction direction = getJoystickDirection();
      printDirection(direction);
      printf(" %d %d",row,column);
      removeSymbol(row, 1);
      moveArrowWithJoystick(&row, &column, direction);
      oledPrintArrow(row, 1);
      */

      printf(" %d %d",xPercentage,yPercentage);
      uint8_t angle = JoystickPercentageToServoAngle(xPercentage, yPercentage);
      printf(" %d",angle);

      uint8_t state = getJoystickState(xPercentage, yPercentage);
      printf(" %d\t",state);

      uint8_t servo = joystickToServoDegrees(values[2]);
      printf(" %d\t",servo);

      uint8_t motor = joystickToMotorDegrees(values[3]);
      printf(" %d\t",motor);

      m.data[0] = servo;
      m.data[1] = motor;
      

      //CAN transmit
      canTransmit(m);
      printf("\n");
      

    

      /*
      m.data[0] = row; 
      m.data[1] = column;
      m.data[2] = xPercentage;
      m.data[3] = yPercentage;
      m.data[4] = direction;  
      
      buttons = buttonTest(buttons);
        uint8_t *values = adcRead();
        int xPercentage = joystickPercentage(values[3]);
        int yPercentage = joystickPercentage(values[2]);
        //printf("TP_X: %d  TP_Y: %d  JS_Y: %d  JS_X: %d\n",
        //       values[0], values[1], yPercentage, xPercentage);
        enum Direction direction = JoystickPercentageToDirection(xPercentage, yPercentage);
        moveArrowWithJoystick(&row, &column, direction);
        //printf(" DIRECTION: ");
        //printDirection(direction);
        //printf(" Angle: %d\n", voltageToAngle(values));

        //submenu

        
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
