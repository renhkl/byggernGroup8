#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "CAN/can_controller.h"
#include "CAN/can.h"

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
  //PIOB->PIO_ODR = PIO_CODR_P13; //clear/low


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
 pwmInit13();
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

  printf("TURNING %d INTO %d\n\r",0,degreesToDutyCycle(0));
  printf("TURNING %d INTO %d\n\r",90,degreesToDutyCycle(90));
  printf("TURNING %d INTO %d\n\r",180,degreesToDutyCycle(180));

  changeServoPosition(0);
  adcInit();

  while (1)
  {

    printf("ADC %d\t", adcRead());

    
    
 
      //can_tx(m);
      
      
      printf("Reading... ");
      if(can_rx(&read)) can_printmsg(read);
      else printf("empty soul, empty head");

      printf(" STATE %d", read.byte[0]);
      if (read.byte[0] == 1) {
        printf(" CHANGE TO ANGLE %d", read.byte[1]);
        changeServoPosition(read.byte[1]);
      }
      
      printf("\n\r");
      
      
  }
    
}