#include "timer_counter.h"

uint8_t setFlag(uint8_t setget)
{
    static uint8_t Flag = 0;
    //printf("FLAG: %d", Flag);
    if (setget == 1)
    {
        Flag = 1;
    }
    else if (setget == 0 && Flag == 1)
    {
        Flag = 0;
        return 1;
    }

    return 0;
}

void regulatorTimertInit(void)
{
    PMC->PMC_PCER0 |= (1 << ID_TC0);
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
    TC0->TC_CHANNEL[0].TC_RC = 0.125 * 42000000;
    TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
    NVIC_EnableIRQ(TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void TC0_Handler(void)
{
    uint32_t interruptFlag = TC0->TC_CHANNEL[0].TC_SR;
    if (interruptFlag & TC_SR_CPCS)
    {
        setFlag(1);
    }
}