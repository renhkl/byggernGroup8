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

  //Uncomment after including uart above
  //uart_init(/*cpufreq*/, /*baud*/);
  uart_init(84000000, 9600);
  
  PIOB->PIO_OER = PIO_OER_P13; //make output
  PIOB->PIO_SODR = PIO_SODR_P13; //set high 
  //PIOB->PIO_ODR = PIO_CODR_P13; //clear/low
  int baud = 14343;


  /*
  uint32_t phaseSeg2;  // Phase 2 segment
  uint32_t propagTimeSeg;  // Propagation time segment
  uint32_t phaseSeg1;  // Phase 1 segment
  uint32_t syncJumpWidth;     // Synchronization jump width
  uint32_t baudRatePrescaler;     // Baud rate prescaler
  uint32_t samplingMode;     // Sampling mode
  uint32_t reg;
  */

  CanInit tConfig = {
      .phase2 = 4,
      .propag = 1,
      .phase1 = 5,
      .sjw    = 3,
      .brp    = 20,
      .smp    = 0
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

  //can_init_def_tx_rx_mb(tConfig); //init with predfined mailboxes

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
  while (1)
  {
    
      //can_send(&message, 0);
      can_tx(m);
      
      
      /*printf("Reading... ");
      if(can_rx(&read)) can_printmsg(read);
      else printf("empty soul, empty head\n\r");*/
      
      
  }
    
}