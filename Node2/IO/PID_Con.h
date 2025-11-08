typedef struct {
    double Kp; //Proportional gain.
    double Ki; //Integral gain.
    double Kd; //Derivative gain.
    double T; //Sampling period (measurements).

    double integral; //Summed up error for integral action.
    double errorPrev; //Difference between previous target and previous measurement for derivative action.

} PIDCon;
