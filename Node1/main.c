#include <avr/io.h>
#include </usr/lib/avr/include/avr/iom162.h>
#include <stdio.h>

#include "TEST/latch_test.h"
#include "TEST/sram_test.h"
#include "MCU/xmem.h"
#include "IO/adc_joystick.h"
#include "IO/oled.h"
#include "IO/buttons.h"
#include <avr/interrupt.h>

#include "MCU/uart.h"
#include "delay.h"
#include "CAN/mcp2515.h"
#include "CAN/can.h"

#include "MCU/spi.h"

#define F_CPU 4915200UL

// uart baudrate
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

int main(){

    uartInit(MYUBRR);
    xmem_init();
    counterInit();
    SPIMasterInit();
    mcp2515Init();
    _delay_ms(1000);
    oledInit();

    DDRD &= ~(1 << DDD5); //set pin D5 as input (joystick pin)
 
    oledSetBrightness(255);
 
    uint8_t row = 0;
    uint8_t column = 0;

    uint8_t read = mcp2515Read(MCP_CANSTAT);
    
    if (read == 255) {
      // for debugging
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

    uint8_t ice = 1;
    uint8_t goals = 255;
    uint8_t screenInitialized = 0;
    uint8_t gameRunning = 0;
    uint8_t highscore = 255;

    while (1){
      frame recieveMessage = canReceive();
      if (recieveMessage.id == 5) {
        goals = recieveMessage.data[0];
      }
    
      if (!gameRunning) {
        if (!screenInitialized) {
          screenInitialized = 1;
          oledClearScreen();
          oledMakeHomeMenu();
          printf("GOL %d HI %d",goals, highscore);
          if (goals < highscore) highscore = goals;
          if(highscore != 255) {oledUpdateHighscore(highscore);}
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
          oledClearScreen();
          oledMakePlayMenu();
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

      if (xPercentage != -200 && yPercentage != -200)
      {
        ice = 1;

        uint8_t angle = JoystickPercentageToServoAngle(xPercentage, yPercentage);

        uint8_t state = getJoystickState(xPercentage, yPercentage);

        uint8_t servo = joystickToServoDegrees(values[2]);

        uint8_t motor = joystickToMotorDegrees(values[3]);

        Buttons buttons = ioButton();
        uint8_t solenoid = getSolenoidButton(buttons);

        m.data[0] = servo;
        m.data[1] = motor;
        m.data[2] = solenoid | (gameRunning << 1);

        canTransmit(m);
        printf("\n");
      }
      else if (ice){
        ice = 0;
      }
    }

    return 0;
}
