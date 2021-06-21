#pragma once

typedef struct _Doug_PID_param
{
    double Kp;
    double Ki;
    double Kd;

    int lastError;
    int errSum;

}Doug_PID_param;

double Doug_PID(int error, volatile Doug_PID_param *param);