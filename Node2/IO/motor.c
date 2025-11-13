#include "motor.h"
#include "sam.h"
#include "pwm_servo.h"

#define PMC_WPMR_WPEN (0x1u << 0)

void pwmMotorInit() {

    //PIO

    //write protections in control register
    PMC->PMC_WPMR |= PMC_WPMR_WPKEY(0x504D43);
    PMC->PMC_WPMR &= ~PMC_WPMR_WPEN;


    //peripheral clock for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;


    //turn off PIO write protection    
    PIOB->PIO_WPMR |= PIO_WPMR_WPKEY(0x50494F); 
    PIOB->PIO_WPMR &= ~PIO_WPMR_WPEN;

    //enable peripheral control of pin, disable PIO
    PIOB->PIO_PDR |= PIO_PDR_P12;

    //peripheral B function is PWMH0
    PIOB->PIO_ABSR |= PIO_ABSR_P12; 


    //PWM

    //write protections
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(0); 

    
    //choose clk 32 prescaler
    PWM->PWM_CLK = PWM_CLK_PREB(0b101) | PWM_CLK_PREA(0b101);
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_32; 
    PWM->PWM_CH_NUM[0].PWM_CMR |= PWM_CMR_CPOL; //change channel polarity 
    
    PWM->PWM_CH_NUM[0].PWM_CPRD = 52500; //period
    PWM->PWM_CH_NUM[0].PWM_CDTY = 25000; //duty cycle
    PWM->PWM_ENA |= PWM_ENA_CHID0; //enable channel 0

    printf("%d \n", PWM->PWM_SR & PWM_SR_CHID0);

    //make write protection for pwm active again
    PWM->PWM_WPCR |=  PWM_WPCR_WPKEY(0x50574D); 
    PWM->PWM_WPCR |= PWM_WPCR_WPRG0 | PWM_WPCR_WPRG2 | PWM_WPCR_WPRG3; //groups it applies to
    PWM->PWM_WPCR |=  PWM_WPCR_WPCMD(1); 
   

}

void motorStartPosition() {
    setMotorDirection(0);
    setMotorVelocity(52500);
}

void setMotorVelocity(int DutyCycle) {
    if ((int)DutyCycle < 200) {DutyCycle = 0;}
    if ((int)DutyCycle > 52500) {DutyCycle = 52500;}
    changeDutyCycle((int)DutyCycle, 0); // from 0 to 52500
}

void setMotorDirection(uint8_t direction) {
    PIOC->PIO_OER = PIO_OER_P23;
    if (direction == 1) {PIOC->PIO_SODR = PIO_SODR_P23;
    printf(" MOVING LEFT");} //set high
    if (direction == 0) {PIOC->PIO_CODR = PIO_CODR_P23; //pioper10 PIOD
    printf(" MOVING RIGHT");} //set low
}

void moveMotor(double gain) {
    printf(" GAINS %lf", gain);
    if (gain > 0){
        setMotorDirection(0);
    };
    if (gain < 0) {
        setMotorDirection(1);
        gain *= -1;
    }
    setMotorVelocity((int)gain);
}

void timerCounterInit()
{

    //write protections in control register
    PMC->PMC_WPMR |= PMC_WPMR_WPKEY(0x504D43);
    PMC->PMC_WPMR &= ~PMC_WPMR_WPEN;

    //peripheral clock for TC2
    PMC->PMC_PCER0 |= PMC_PCER0_PID29;

    //PIO
    PIOC->PIO_WPMR |= PIO_WPMR_WPKEY(0x50494F); 
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;

    //enable peripheral control of pin, disable PIO
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;

    //choose peripheral B
    PIOC->PIO_ABSR |= PIO_ABSR_P25; 
    PIOC->PIO_ABSR |= PIO_ABSR_P26; 

    //TIMER/COUNTER 2
    //write protection
    TC2->TC_WPMR |= TC_WPMR_WPKEY(0x54494D);
    TC2->TC_WPMR &= ~TC_WPMR_WPEN;   //enable peripheral control of pin, disable PIO

    TC2-> TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0;
    //TC->TC_BMR |= TC_BMR_TC0XC0S_TCLK0; //?

    //set block mode

    TC2->TC_BMR |= TC_BMR_POSEN; //position on channel 0 and 1
    TC2->TC_BMR |= TC_BMR_QDEN;
    //TC->TC_BMR |= TC_BMR_SPEEDEN; //speed on channel 0


    TC2-> TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;    

//enable
TC2-> TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
}

void decoderInit()
{
    //PMC
    //write protections in control register
    PMC->PMC_WPMR |= PMC_WPMR_WPKEY(0x504D43);
    PMC->PMC_WPMR &= ~PMC_WPMR_WPEN;

    //some clock stuff, idk why it works but it must be done
    PMC->PMC_PCER0 |= (1 << ID_PIOC);
    PMC->PMC_PCER1 |= (1 << (ID_TC6) -32);

    //peripheral clock for TC2
    PMC->PMC_PCER0 |= PMC_PCER0_PID29;

    //PIO
    //write protections
    PIOC->PIO_WPMR |= PIO_WPMR_WPKEY(0x50494F); 
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;

    //enable peripheral control of pin, disable PIO
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;

    //choose peripheral B
    PIOC->PIO_ABSR |= PIO_ABSR_P25; 
    PIOC->PIO_ABSR |= PIO_ABSR_P26; 
    
    //TIMER/COUNTER 2

    //write protection
    TC2->TC_WPMR |= TC_WPMR_WPKEY(0x54494D);
    TC2->TC_WPMR &= ~TC_WPMR_WPEN;   

    TC2->TC_CHANNEL[0].TC_CMR &= ~TC_CMR_WAVE; //set capture mode, just to be sure
    TC2-> TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0;    //XC0 as clock
    TC2-> TC_CHANNEL[0].TC_CMR |= TC_CMR_ETRGEDG_RISING; //Triggering edge
    TC2-> TC_CHANNEL[0].TC_CMR |= TC_CMR_ABETRG;        //TIOA as external trigger

    //set block mode
    TC2->TC_BMR |= TC_BMR_QDEN;
    TC2->TC_BMR |= TC_BMR_POSEN; //position on channel 0 and 1
    //TC->TC_BMR |= TC_BMR_SPEEDEN; //speed on channel 0
    TC2->TC_BMR |= TC_BMR_EDGPHA;
    //TC2->TC_BMR |= TC_BMR_MAXFILT(5);

    TC2-> TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG; //sw trigger
    TC2-> TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN; //enable

}


int getDecoderValue() {
    int value = TC2->TC_CHANNEL[0].TC_CV; 
    //read value
    return REG_TC2_CV0;
    //return value;
}

#define MAXBOUNDS 5632

int desiredMotorPosition(int currentPosition, uint8_t joystickAngle, int motorPosition) {
    int scale = 10;

    int deltaPosition;
    if (joystickAngle < 90) {deltaPosition = (90 - joystickAngle) * -scale;}
    else if (joystickAngle != 90) {deltaPosition = (joystickAngle - 90) * scale;}
    else {deltaPosition = 0;}
    
    static int lowerBound;
    static int upperBound;

    static uint8_t initialized = 0;

    if (!initialized) {
        lowerBound = motorPosition;
        upperBound = motorPosition;
        initialized = 1;
    }

    //printf(" LOWER %d", lowerBound);
    //printf(" UPPER %d", upperBound);
    if (lowerBound > motorPosition + deltaPosition) {lowerBound = motorPosition + deltaPosition;}
    if (upperBound < motorPosition + deltaPosition) {upperBound = motorPosition + deltaPosition;}

    currentPosition = motorPosition + 3*deltaPosition;

    if (!(upperBound - lowerBound < 5632*0.9)) {
        if (currentPosition < lowerBound) {currentPosition = lowerBound;}
        if (currentPosition > upperBound) {currentPosition = upperBound;}
    }
    return currentPosition;
}

int angleToRelativePosition(uint8_t joystickAngle) {
    double scale = 5632/180;

    double position;
    if (joystickAngle < 90) {position = (90 - (double)joystickAngle) * -scale;}
    else if (joystickAngle != 90) {position = ((double)joystickAngle - 90) * scale;}
    else {position = 0;}

    return (int)position;
}

int getTargetPosition(int motorPosition, int relativePosition) {
    static int lowerBound;
    static int upperBound;

    static uint8_t initialized = 0;

    if (!initialized) {
        lowerBound = motorPosition;
        upperBound = motorPosition;
        initialized = 1;
    }

    if (lowerBound-2000 > motorPosition) {lowerBound = motorPosition+2000;}
    if (upperBound+2000 < motorPosition) {upperBound = motorPosition-2000;}

    if (!(upperBound - lowerBound < 5632*0.9)) {
        return relativePosition*2 + (upperBound-lowerBound)/2;
    }

    return relativePosition + (upperBound-lowerBound)/2;
}

int actuallyGetTargetPosition(int relativePosition, int upperBound) {
    return relativePosition + upperBound - 5632/2;
}

uint8_t calibrateField(int position, int *upperBound){
    motorStartPosition();
    static int init = 0;
    static int posPrev = 0;
    if (!init) {
        posPrev = position - 1;
        init = 1;
    }

    uint8_t state = (posPrev == position);

    printf(" POS %d PREV %d",position,posPrev);

    if (position > posPrev){
        *upperBound = position;
    }
    printf(" BOUND %d",*upperBound);

    posPrev = position;
    return state;
}