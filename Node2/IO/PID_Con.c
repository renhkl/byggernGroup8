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

//Calculates PID gain, with the option of using integral anti-windup according to the Pedersen scheme.
//Init a controller (se header file) and feed the desired position into "target" and the current measurement into "measurement". 
double compute_gain(PIDCon *controller, double target, double measurement, bool antiWindup, double antiWindupThreshold) {

    double error = target - measurement;

    if(antiWindup){
        if(error <= antiWindupThreshold){
            controller->integral += error * controller->T;
        }
        else{
            controller->integral = 0;
        }
    }
    else {
        controller->integral += error * controller->T;
    }

    //Gain compute. Note that the last term is derivative action. This can be turned off by setting "Kd" to 0 in the controller struct.
    double gain = controller->Kp * error + controller->Ki * controller->integral + (controller->Kd)/(controller->T) * (error - controller->errorPrev);

    controller->errorPrev = error;

    return gain;
}

