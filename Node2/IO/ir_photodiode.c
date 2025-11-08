#include "ir_photodiode.h"
#include <stdint.h>

void countGoals(uint8_t *goals, int sensor) {
    if(sensor){
        *goals += 1;
    }
}

int readPhotodiode() {
    return 1;
}