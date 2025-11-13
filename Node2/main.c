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
 

  CanMsg m = (CanMsg){
    .id = 5,
    .length = sizeof(YourStruct),
    .byte8 = union_cast(Byte8, ((YourStruct){
      .a = 10,
      .b = 20,
      .c = 25,
      .d = 30,
    })),
  };
    
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
  can_printmsg(m);

  deactivateSolenoid();
  changeServoPosition(0);
  adcInit();
  decoderInit();

  int goals = 0;
  unsigned long goalDelay = 1000000;

  moveMotor(0);

  int currentPosition = 0;

  printf("TEST DOUBLE %lf", (double)0.18347);

  while (1)
  {
    
    
    int adc = adcRead();
    printf("ADC %d\t", adc);
    uint8_t goal = isGoal(adc);
    printf(" GOAL? %d ", goal);
    countGoals(&goals, goal, goalDelay);
    printf(" TOTAL %d\t", goals);
    int motorPosition = getDecoderValue();
    printf(" DECODER VALUE %d\t", motorPosition);
   
    
    
 
      //can_tx(m);
      
      
      printf("Reading... ");
      if(can_rx(&read)) can_printmsg(read);
      else printf("empty soul, empty head");

      
      uint8_t servoAngle = read.byte[0];
      printf(" SERVO ANGLE %d", servoAngle);
      changeServoPosition(servoAngle);
      
      
      uint8_t motorAngle = read.byte[1];
      currentPosition = desiredMotorPosition(currentPosition, motorAngle, motorPosition);
      printf(" CURRENT POS %d", currentPosition);

      uint8_t solenoidState = read.byte[2];
      pulseSolenoid(solenoidState);
      printf(" SOLENOID %d", solenoidState);
      /*


      
      PIDCon controller = {
        .Kp = 10, 
        .Ki = 10,
        .Kd = 0,

        .T = (1/4000000),
        .integral = 0
      };
      ringBufferInit(&controller.errorPrev);
      double gain = compute_gain(&controller, currentPosition, motorPosition, 1, 1, 1);
      
      moveMotor(gain);
      */
      
      

     /*
      motorEnable();
      if (motorAngle < 90) {setMotorDirection(0);}
      else if (motorAngle != 90) {setMotorDirection(1);}
      else {motorDisable();}
      setMotorVelocity(motorAngle);
      */

      

    /*

      if (motorAngle > 180) {


        motorAngle=180;


      }


      if (motorAngle < 90) {

1
        setMotorDirection(1);


        motorAngle *= 2;


        printf(" MOTOR LEFT %d", motorAngle);


      }


      else {


        setMotorDirection(0);


        motorAngle -= 90;


        motorAngle *= 2;


        printf(" MOTOR RIGHT %d", motorAngle);


      }


      setMotorVelocity(motorAngle);
    */
    
    printf("\n\r");
      
  }
    
}