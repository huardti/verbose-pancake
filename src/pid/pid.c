#include "pid.h"

double Doug_PID(int error, volatile Doug_PID_param *p)
{
    double P,I,D;
    int errDif;

    p->errSum += error;                 //Somme les erreurs depuis le début
    errDif = error - p->lastError;      //Calcule la variation de l'erreur

    p->lastError = error;

    P = error * p->Kp;                   //Proportionnelle
    I = p->errSum * p->Ki;                //Intégrale
    D = errDif * p->Kd;                  //Dérivée
    return P + I + D;
}