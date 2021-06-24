#include "asservissement.h"

#include "motor_driver/hall_encoder.h"
#include "ir_sensors/ir_sensors.h"
#include "motor_driver/motor_driver.h"

#include "main.h"
#include "tim.h"


volatile int errorD;
volatile int errorG;

volatile int error_positionD;
volatile int error_positionG;

extern volatile int consigne_G;
extern volatile int consigne_D;

volatile int PID_D;
volatile int PID_G;

int Doug_Hall_Set_Target(int mesure)
{
    int return_value = 2 * (mesure - consigne_position) + 50;

    if(return_value > 100) return_value = 100;
    if(return_value < 0) return_value = -return_value;

    return return_value;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if
    (
        !HAL_GPIO_ReadPin(COLLISION1_GPIO_Port, COLLISION1_Pin)
        || !HAL_GPIO_ReadPin(COLLISION2_GPIO_Port, COLLISION2_Pin)
    )
    {
        Doug_MD_Set_Motor(DOUG_MD_STOP);
        return;
    }
    #if 1
    if(htim->Instance == htim6.Instance)
    {
        // Mémoire en cas de sortie de la cible du champ
        if(ir_distances[0] <= 100 && ir_distances[1] >= 100 )//&& ir_distances[2] >= 100)
        {
            side_memory = MEMORY_LEFT;
        }
        else if(ir_distances[0] >= 100 && ir_distances[1] <= 100)// && ir_distances[2] >= 100)
        {
            side_memory = MEMORY_RIGHT;
        }

        if(ir_distances[0] >= 100 && ir_distances[1] >= 100 && ir_distances[2] >= 100)
        {
            DougMD_Set_SpeedG(SCAN_SPEED);
            DougMD_Set_SpeedD(SCAN_SPEED);

            if(side_memory == MEMORY_LEFT)
            {
                DougMD_Set_Direction_G(DOUG_MD_REVERSE);
                DougMD_Set_Direction_D(DOUG_MD_FORWARD);
            }
            else if(side_memory == MEMORY_RIGHT)
            {
                DougMD_Set_Direction_G(DOUG_MD_FORWARD);
                DougMD_Set_Direction_D(DOUG_MD_REVERSE);
            }

            Doug_MD_Set_Motor_G(DOUG_MD_START);
            Doug_MD_Set_Motor_D(DOUG_MD_START);

            return;
        }
#if 1
        consigne_G = Doug_Hall_Set_Target(ir_distances[0]);
        consigne_D = Doug_Hall_Set_Target(ir_distances[1]);

        /* droite */
        if(ir_distances[1] <= consigne_position - 10)
        {
            DougMD_Set_Direction_D(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor_D(DOUG_MD_START);
        }
        else if(ir_distances[1] <= consigne_position)
        {
            Doug_MD_Set_Motor_D(DOUG_MD_STOP);
        }
        else
        {
            DougMD_Set_Direction_D(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor_D(DOUG_MD_START);
        }

        errorD = consigne_D - speed_D;
        PID_D = Doug_PID(errorD, &PID_motor);
        DougMD_Set_SpeedD(PID_D);


        /* gauche */
        if(ir_distances[0] <= consigne_position - 10)
        {
            DougMD_Set_Direction_G(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor_G(DOUG_MD_START);
        }
        else if(ir_distances[0] <= consigne_position)
        {
            Doug_MD_Set_Motor_G(DOUG_MD_STOP);
        }
        else
        {
            DougMD_Set_Direction_G(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor_G(DOUG_MD_START);
        }

        errorG = consigne_G - speed_G;

        PID_G = Doug_PID(errorG, &PID_motor);
        DougMD_Set_SpeedG(PID_G);
#endif

#if 1
        // Ultrason
        if(ir_distances[0] > consigne_position || ir_distances[1] > consigne_position)
        {
        if(ir_distances[2] <= consigne_position - 10)
        {
            consigne_G = Doug_Hall_Set_Target(ir_distances[2]);
            consigne_D = Doug_Hall_Set_Target(ir_distances[2]);

            errorG = consigne_G - speed_G;
            PID_G = Doug_PID(errorG, &PID_motor);
            DougMD_Set_SpeedG(PID_G);
            errorD = consigne_D - speed_D;
            PID_D = Doug_PID(errorD, &PID_motor);
            DougMD_Set_SpeedD(PID_D);

            DougMD_Set_Direction_G(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor_G(DOUG_MD_START);
            DougMD_Set_Direction_D(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor_D(DOUG_MD_START);
        }
        else if (ir_distances[2] <= consigne_position)
        {
            Doug_MD_Set_Motor_G(DOUG_MD_STOP);
            Doug_MD_Set_Motor_D(DOUG_MD_STOP);
        }
        }
#if 0
        else
        {
            consigne_G = Doug_Hall_Set_Target(ir_distances[2]);
            consigne_D = Doug_Hall_Set_Target(ir_distances[2]);

            errorG = consigne_G - speed_G;
            PID_G = Doug_PID(errorG, &PID_motor);
            DougMD_Set_SpeedG(PID_G);
            errorD = consigne_D - speed_D;
            PID_D = Doug_PID(errorD, &PID_motor);
            DougMD_Set_SpeedD(PID_D);

            DougMD_Set_Direction_G(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor_G(DOUG_MD_START);
            DougMD_Set_Direction_D(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor_D(DOUG_MD_START);
        }
#endif
#endif
    }
    #endif
}