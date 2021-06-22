#include "asservissement.h"

#include "motor_driver/hall_encoder.h"
#include "ir_sensors/ir_sensors.h"

#include "main.h"
#include "tim.h"


volatile int errorD;
volatile int errorG;

volatile int error_positionD;
volatile int error_positionG;

volatile int consigneG;
volatile int consigneD;

volatile int PID_D;
volatile int PID_G;

int Doug_Hall_Set_Target(int mesure)
{
    int return_value = 2 * (mesure - consigne_position) + 50;

    if(return_value > 100) return_value = 100;
    if(consigne < 0) consigne = -consigne;

    return return_value;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim6.Instance)
    {
        consigneG = Doug_Hall_Set_Target(ir_distances[0]);
        consigneD = Doug_Hall_Set_Target(ir_distances[1]);

        if(ir_distances[1] <= consigne_position - 10)
        {
            DougMD_Set_Direction(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }
        else if(ir_distances[1] <= consigne_position)
        {
            Doug_MD_Set_Motor(DOUG_MD_STOP);
        }
        else
        {
            DougMD_Set_Direction(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }

        errorD = consigneD - speed_D;
        errorG = consigneG - speed_G;

        PID_D = Doug_PID(errorD, &PID_motor);
        PID_G = Doug_PID(errorG, &PID_motor);

        //error_positionD = consigne_position - ir_distances[1];

        //DougMD_Set_SpeedD(error_positionD);
        //DougMD_Set_SpeedG(error_positionG);
        DougMD_Set_SpeedD(PID_D);
        DougMD_Set_SpeedG(PID_G);
    }
}