#include <stdbool.h>
//States struct to store velocity and position. 
typedef struct {
    double velocity;
    double position;
} states;

//Controller struct to be passed into "computeGain".
typedef struct {
    double Kp; //Proportional gain.
    double Ki; //Integral gain.
    double Kd; //Derivative gain.
    double T; //Sampling period (measurements).

    double integral; //Summed up error for integral action.
    double errorPrev; //Difference between previous target and previous measurement for derivative action.

} PIDCon;

double integrateVelocity(states *currentState, double measurement, double T);
double compute_gain(PIDCon *controller, double target, double measurement, bool antiWindup, double antiWindupThreshold); 
