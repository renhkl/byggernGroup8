#include <stdbool.h>
//States struct to store velocity and position. 
typedef struct {
    double velocity;
    double position;
} states;

//Ring buffer for storing last four position errors in PIDcon struct. 
typedef struct{
    double data[4];
    int head;
    int count;
} ringBuffer;

//Controller struct to be passed into "computeGain".
//NB: remember to call "ringBufferInit(&controller->errorPrev)" after making instance of PIDcon.
typedef struct {
    double Kp; //Proportional gain.
    double Ki; //Integral gain.
    double Kd; //Derivative gain.
    double T; //Sampling period (measurements).

    double integral; //Summed up error for integral action.
    ringBuffer errorPrev; //Difference between previous target and previous measurement for derivative action.

} PIDCon;

double integrateVelocity(states *currentState, double measurement, double T);
double compute_gain(PIDCon *controller, double target, double measurement, bool antiWindup, double antiWindupThreshold, bool antiStick); 
void ringBufferInit(ringBuffer *rb);
void ringBufferAdd(ringBuffer *rb, double value);
double ringBufferGet(ringBuffer *rb, int index);

