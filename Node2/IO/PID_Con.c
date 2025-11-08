#include "PID_Con.h"

//Init a controller (se header file) and feed the desired position into "target" and the current measurement into "measurement". 
double compute_gain(PIDCon *controller, double target, double measurement) {

    double error = target - measurement;

    controller->integral += error * controller->T;

    //Gain compute. Note that the last term is derivative action. This can be turned off by setting "Kd" to 0 in the controller struct.
    double gain = controller->Kp * error + controller->Ki * controller->integral + (controller->Kd)/(controller->T) * (error - controller->errorPrev);

    controller->errorPrev = error;

    return gain;
}

