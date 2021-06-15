#include "hall_encoder.h"
#include "retarget/retarget.h"
#include "stm32_hal_legacy.h"

#include <stdio.h>

#include "main.h"
#include "tim.h"

volatile uint16_t time_encoder_D;
volatile uint16_t time_encoder_G;

void Doug_Hall_Encoder_Init(void)
{
    MX_TIM3_Init();
    MX_TIM4_Init();
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_Base_Start(&htim4);

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
        time_encoder_G = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
        __HAL_TIM_SetCounter(&htim3, 0);
        HAL_TIM_Base_Start(&htim3);
    }

    if(htim->Instance == htim4.Instance)
    {
        time_encoder_D = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);
        __HAL_TIM_SetCounter(&htim4, 0);
        HAL_TIM_Base_Start(&htim4);
    }
}
