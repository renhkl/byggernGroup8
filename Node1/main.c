#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "TEST/latch_test.h"
#include "TEST/sram_test.h"

#include "MCU/uart.h"
#include "MCU/spi.h"
#include "MCU/xmem.h"

#include "IO/adc_joystick.h"
#include "IO/oled.h"
#include "IO/buttons.h"

#include "CAN/mcp2515.h"
#include "CAN/can.h"

#include <avr/interrupt.h>
#include "delay.h"

// #define F_CPU 4952000UL
#define F_CPU 4915200UL

// uart baudrate
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

int main()
{

    uartInit(MYUBRR);
    xmemInit();
    counterInit();
    SPIMasterInit();
    mcp2515Init();
    
    oledInit();
    oledSetBrightness(255);
    joystickInit();

    // Test if MCP2515 connected
    uint8_t read = mcp2515Read(MCP_CANSTAT);
    if (read == 255) {
      printf("INVALID MCP READING, CHECK YOUR CONNECTIONS\n");
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
      .length = 3,
      .data = {4, 7, 5}
    };

    frame recieveMessage = {
      .id = 3,
      .length = 3,
      .data = {4, 7, 5}
    };

    uint8_t goals = 255;
    uint8_t screenInitialized = 0;
    uint8_t gameRunning = 0;
    uint8_t highscore = 255;

    while (1)
    {
      
      //CAN receive
      frame recieveMessage = canReceive();
      if (recieveMessage.id == 5) {
        /*
        printf("CAN READ VALUE ID %d\t LENGTH %d\t DATA ", recieveMessage.id, recieveMessage.length);
        for (uint8_t i = 0; i < recieveMessage.length; i++) {printf("%d ",recieveMessage.data[i]);}
        printf("\n");
        */
       goals = recieveMessage.data[0];
      }
      
      if (!gameRunning) {
        if (!screenInitialized) {
          screenInitialized = 1;

          // Initialize home screen
          oledClearScreen();
          oledMakeHomeMenu();

          // Update highscore
          if (goals < highscore) highscore = goals;
          if(highscore != 255) {oledUpdateHighscore(highscore);}
          
          // Clear spi buffer
          ioButton();
          ioButton();
          ioButton();
        }
        Buttons buttons = ioButton();
        moveArrowHomeMenu(buttons, &gameRunning);
      }
      else {
        if (screenInitialized) {
          screenInitialized = 0;
          goals = 0;

          // Initialize play screen
          oledClearScreen();
          oledMakePlayMenu();
          
          // Clear spi buffer
          ioButton();
          ioButton();
          ioButton();
        }
        oledUpdateScore(goals);
        
        Buttons buttons = ioButton();
        moveArrowPlayMenu(buttons, &gameRunning);
      }
      


      uint8_t *values = adcRead();
      int xPercentage = joystickPercentage(values[3]);
      int yPercentage = joystickPercentage(values[2]);

      // Test if ICE disconnected
      if (xPercentage != -200 && yPercentage != -200)
      {

        uint8_t servo = joystickToServoDegrees(values[2]);
        //printf(" %d\t",servo);

        uint8_t motor = joystickToMotorDegrees(values[3]);
        //printf(" %d\t",motor);

        Buttons buttons = ioButton();
        uint8_t solenoid = getSolenoidButton(buttons);
        //printf(" %d\t",solenoid);

        m.data[0] = servo;
        m.data[1] = motor;
        m.data[2] = solenoid | (gameRunning << 1);
        

        canTransmit(m);
      }
    }
    
    return 0;

}
