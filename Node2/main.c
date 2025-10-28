#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
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

    canTimeConfig tConfig;
    tConfig.phaseSeg2 = 5;
    tConfig.propagTimeSeg = 3;
    

     can_init(tConfig, CAN0_Handler); //can also have 0 as rx interrupt
    can_init_def_tx_rx_mb(baud);

    CanMsg message;
   

    

    //PIO_PB13;

    while (1)
    {
        /* code */
        printf("CHARIZARD\n\r");
        //uint8_t message = can_rx();
        //can_tx(message);
    }
    
}