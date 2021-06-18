#include "hall_encoder.h"
#include "retarget/retarget.h"
#include "stm32_hal_legacy.h"

#include <stdio.h>

#include "motor_driver.h"

#include "main.h"
#include "tim.h"

volatile int consigne = 30;
void Doug_Hall_Encoder_Init(void)
{
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM6_Init();

    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_Base_Start(&htim4);

    HAL_TIM_Base_Start_IT(&htim6);

    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);

    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
}

void Doug_Hall_Encoder_print_info(void)
{
    printf("droite : %6d, gauche : %6d\r", time_encoder_D, time_encoder_G);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim3.Instance)
    {
        time_encoder_G += __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
        time_encoder_G = time_encoder_G/2;
        speed_G = (((time_encoder_G - time_min_speed)*100) / (time_max_speed - time_min_speed));
        if(speed_G > 100)
        {
            speed_G = 1;
        }
        __HAL_TIM_SetCounter(&htim3, 0);
        HAL_TIM_Base_Start(&htim3);
    }

    if(htim->Instance == htim4.Instance)
    {
        time_encoder_D += __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);
        time_encoder_D = time_encoder_D/2;
        speed_D = (((time_encoder_D - time_min_speed)*100) / (time_max_speed - time_min_speed));
        if (speed_D > 100)
        {
            speed_D = 1;
        }
        __HAL_TIM_SetCounter(&htim4, 0);
        HAL_TIM_Base_Start(&htim4);
    }
}

volatile int errorD;
volatile int errorG;

volatile int PID_D;
volatile int PID_G;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == htim6.Instance)
    {
        errorD = consigne - speed_D;
        errorG = consigne - speed_G;

        PID_D = Doug_MD_PID(errorD);
        PID_G = Doug_MD_PID(errorG);

       DougMD_Set_SpeedD(PID_D);
       DougMD_Set_SpeedG(PID_G);
    }
}

