#include <avr/io.h>
#include <util/delay.h>

int main(void){

    DDRB = 0xFF;

    while(1){
        PORTB ^= 0xFF; //toggle pin 6 on port D
        _delay_ms(200); //wait 1 second
    }
    return 0;
}

