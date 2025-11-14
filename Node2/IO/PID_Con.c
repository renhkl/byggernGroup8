#include "PID_Con.h"

//Simple function for integrating velocity, yielding an estimated position 
//that can be fed as "measurement" to the "compute_gain" function below.
//NB: it might be a good idea to low-pass filter the velocity measurement before integrating it.
//T is sample period.
double integrateVelocity(states *currentState, double measurement, double T){

    currentState->velocity = measurement;
    currentState->position += measurement * T;

    return currentState->position;
}

void ringBufferInit(ringBuffer *rb){
    rb->head = 0;
    rb->count = 0;
    rb->data[0] = 0;
    rb->data[1] = 0;
    rb->data[2] = 0;
    rb->data[3] = 0;
}

void ringBufferAdd(ringBuffer *rb, double value){
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % 4;
    if(rb->count < 4){
        rb->count += 1;
    }
}

double ringBufferGet(ringBuffer *rb, int index){
    int start = (rb-> head - rb->count + 4) % 4;
    int idx = (start + index) % 4;
    return rb->data[idx];
}

//Calculates PID gain, with the option of using integral anti-windup according to the Pedersen scheme.
//Also feature anti-stick logic. 
//Init a controller (se header file) and feed the desired position into "target" and the current measurement into "measurement". 
double compute_gain(PIDCon *controller, double target, double measurement, bool antiWindup, double antiWindupThreshold, bool antiStick, bool targetBooster) {

    double error = target - measurement;
    printf(" ERROR: %lf\t", error);

    if(antiWindup){
        if(abs(error) <= antiWindupThreshold){
            controller->integral += error * controller->T;
        }
        else{
            controller->integral = 0;
        }
    }
    else {
        controller->integral += error * controller->T;
    }

    printf(" INTEGRAL: %lf\t", controller->integral);

    double boost = 1;
    double targetBoost = 1;

    if(antiStick){
        if((int)ringBufferGet(&controller->errorPrev, 0) == (int)error && (int)ringBufferGet(&controller->errorPrev, 1) == (int)error &&
           (int)ringBufferGet(&controller->errorPrev, 2) == (int)error && (int)ringBufferGet(&controller->errorPrev, 3) == (int)error){
            boost = 5;
        }
    }

    /*if(targetBooster){
        double eps = 0.1;
        targetBoost = 1/(error + (error == 0)*eps);
        printf("TARGETBOOST: %lf\t", targetBoost);
    }*/

    if(targetBooster){
        if(abs(error) <= 1000){
            targetBoost = 10;
        }
    }

    //Gain compute. Note that the last term is derivative action. This can be turned off by setting "Kd" to 0 in the controller struct.
    double gain = controller->Kp * boost * targetBoost * error + controller->Ki * controller->integral;

    ringBufferAdd(&controller->errorPrev, error);
    printf("GAIN: %lf\t", gain);
    return gain;
}

