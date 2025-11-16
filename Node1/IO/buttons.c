#include "buttons.h"
#include "../MCU/spi.h"

Buttons ioButton(void){
    uint8_t data[3];
    SPIMasterReceive(BUTTONS, data, 3);
    return (Buttons) {data[0], data[1], data[2]};
}

uint8_t getSolenoidButton(Buttons buttons) {
    return buttons.R6;
}

Buttons buttonTest(Buttons prevButtons){

    Buttons buttons = ioButton();

    if (prevButtons.left != buttons.left && buttons.left != 0) {printf("buttons.left = 0x%02X\n", buttons.left);}
    //button SR3 toggles constantly anyway, so it has a defect and we choose to not use it
    if (prevButtons.right != buttons.right && buttons.right != 0 && buttons.right != 0x04) {printf("buttons.right = 0x%02X\n", buttons.right);}
    if (prevButtons.nav != buttons.nav && buttons.nav != 0) {printf("buttons.nav = 0x%02X\n", buttons.nav);}
    

    if(buttons.left == 1){
        ledToggle();
        printf("halla");
    }

    return buttons;

}