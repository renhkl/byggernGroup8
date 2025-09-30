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
    // printf("%d\n", adcRead());
    printf("LIZZARD");

    // SRAM_cr_test();
    // ADC_test();

    while (1)
    {
        uint8_t *values = adcReap();
        int xPercentage = joystickPercentage(values[3]);
        int yPercentage = joystickPercentage(values[2]);
        printf("TP_X: %d  TP_Y: %d  JS_Y: %d  JS_X: %d\n",
               values[0], values[1], yPercentage, xPercentage);
        enum Direction direction = getJoystickDirection(xPercentage, yPercentage);
        printf(" DIRECTION: ");
        printDirection(direction);
        printf(" Angle: %d\n", voltageToAngle(values));
       

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
