#include "sram_test.h"
#include <stdlib.h>

void ADC_test(void)
{
    volatile char *ext_ram = (char *)0x1000; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    printf("Starting ADC test...\r\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)

    // Retrieval phase: Check that no values were changed during or after the write phase
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++)
    {
        uint8_t retreived_value = ext_ram[0x082];
        printf("Retrieval: ext_ram[%4d] = %02X \r\n", i, retreived_value);
    }
    printf("ADC test completed");
}