#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "CAN/can_controller.h"
#include "CAN/can.h"
#include "IO/PID_Con.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{

  SystemInit();
  WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer


  //uart_init(/*cpufreq*/, /*baud*/);
  uart_init(84000000, 9600);
  
  //signal pin test
  PIOB->PIO_OER = PIO_OER_P13; //make output
  PIOB->PIO_SODR = PIO_SODR_P13; //set high 
  //PIOB->PIO_CODR = PIO_CODR_P13; //clear/low


  CanInit tConfig = {
      .phase2 = 4,
      .propag = 1,
      .phase1 = 5,
      .sjw    = 3, //synchronization jump width
      .brp    = 20, //baud rate prescaler
      .smp    = 0 //sampling mode
  };

  static const CanInit CFG_250KBPS = {
      .phase2 = 5,
      .propag = 1,
      .phase1 = 6,
      .sjw    = 2,
      .brp    = 20,
      .smp    = 0
  };
  
  // brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 1, .propag = 6

 //using can.h and not can_controller.h
 pwmServoInit();
 pwmMotorInit();
 
  can_init(CFG_250KBPS, 0);
 
    
    /*
    CanMsg write = {
    .id = 3,
    .length = 1,
       .byte[8] = 3,
        .dword[2] = 6,
        .byte8 = 0
  };
  */

  CanMsg read = {
    .id = 3,
    .length = 2
  };
   
  //PIO_PB13;
  printf("CHARIZARD\n\r");
  printf("before\n\r");
  deactivateSolenoid();
  changeServoPosition(0);
  adcInit();
  decoderInit();
  printf("beforel");
  uint8_t goals = 0;
  unsigned long goalDelay = 1000000;
  printf("beforeg");
  moveMotor(0);
  printf("beforeh");
  int currentPosition = 0;
  printf("before\n\r");
  regulatorTimertInit();

  PIDCon controller = {
        .Kp = 10, 
        .Ki = 20,
        .Kd = 0,

        .T = (0.125),
        .integral = 0
      };
  ringBufferInit(&controller.errorPrev);
  
  uint8_t gameInitialized = 0;
  int upperBound = 0;
  while (1)
  {
    
    
   
    //printf("\n\rCURRENT TIME1 %d\n\r", time_now());
      
      
      if(can_rx(&read))
      {
        can_printdata(read);
        uint8_t running = read.byte[2] & 0b10;

      int motorPosition = getDecoderValue();
      //printf(" DECODER VALUE %d\t", motorPosition);
      
      //printf("upperBound %d ",upperBound);

      if (running) {
          if (!gameInitialized) {
              gameInitialized = calibrateField(motorPosition, &upperBound);
              if (gameInitialized) {printf("\n\n\rINITIALIZE\n\n\r");}
          }
          else {
              TC0_Handler();

              uint8_t servoAngle = read.byte[0];
              //printf(" SERVO ANGLE %d", servoAngle);
              changeServoPosition(servoAngle);

              int adc = adcRead();
              //printf("ADC %d\t", adc);

              uint8_t goal = isGoal(adc);
              //printf(" GOAL? %d ", goal);

              //countGoals(&goals, goal, goalDelay);
              //printf(" TOTAL %d\t", goals);

              uint8_t joystickAngleX = read.byte[1];
              //printf(" JOYSTICK %d", joystickAngleX);
              int relativePosition = angleToRelativePosition(joystickAngleX);
              //printf(" RELATIVEPOS %d", relativePosition);
              currentPosition = actuallyGetTargetPosition(relativePosition, upperBound);
              //printf(" CURRENT POS %d", currentPosition);

              uint8_t solenoidState = read.byte[2] & 0b1;
              pulseSolenoid(solenoidState);
              //printf(" SOLENOID %d", solenoidState);

              if(setFlag(0)){
                double gain = compute_gain(&controller, currentPosition, motorPosition, 1, 250, 0, 1);
                moveMotor(gain);
              }

              if(countGoals(&goals, goal, goalDelay)){
                CanMsg m = (CanMsg){
                  .id = 5,
                  .length = sizeof(YourStruct),
                  .byte8 = union_cast(Byte8, ((YourStruct){
                    .a = goals,
                  })),
                };
                
                printf("SENDING: ");
                can_printmsg(m);
                can_tx(m);
                
              }

              //printf(" GOALS %d\t", goals);
          } 
      }
      else {
        if (gameInitialized) {
          gameInitialized = 0;
        }
      }
      printf("\n\r");
      }
      
  }
    
}