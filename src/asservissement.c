#include "asservissement.h"

#include "motor_driver/hall_encoder.h"
#include "ir_sensors/ir_sensors.h"

#include "main.h"
#include "tim.h"

/* vitesse */
volatile int errorD;
volatile int errorG;

volatile int PID_D;
volatile int PID_G;


/* position */
extern volatile IrDistance consigne_distance;


volatile int errorD_position;
volatile int errorG_position;

volatile int PID_D_position;
volatile int PID_G_position;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    for(int i = 0 ; i < DOUG_IR_CHANNELS ; ++i)
    {
        ir_voltages[i] = Doug_IR_value_to_voltage(ir_values[i]);
        ir_distances[i] = Doug_IR_value_to_distance(ir_values[i]);
    }

    if(htim->Instance == htim6.Instance)
    {
        /* vitesse */
        errorD = consigne_d - speed_D;
        errorG = consigne_g - speed_G;

        PID_D = Doug_PID(errorD, &PID_motor);
        PID_G = Doug_PID(errorG, &PID_motor);

       DougMD_Set_SpeedD(PID_D);
       DougMD_Set_SpeedG(PID_G);

        /* position */
        errorG_position  = consigne_distance - ir_distances[0];
        errorD_position  = consigne_distance - ir_distances[1];

        consigne_g = errorG_position;
        consigne_d = errorD_position;
    }
}