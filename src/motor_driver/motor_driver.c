#include "motor_driver.h"

void Doug_MD_Set_Params(Doug_MD_Direction direction, Doug_MD_Speed speed)
{
    if (direction == DOUG_MD_FORWARD)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    }
    else if (direction == DOUG_MD_REVERSE)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
    }
    else if (direction == DOUG_MD_TOGGLE)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);
    }

    if (speed > 0)
    {
        Doug_MD_Speed correct_speed = speed;
        if (correct_speed >= 100)
            correct_speed = 100;

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, TIM1->ARR * correct_speed / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, TIM1->ARR * correct_speed / 100);
    }
}

void Doug_MD_Set_Motor(Doug_MD_Motor_State state)
{
    if (state == DOUG_MD_START)
    {
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    }
    else if (state == DOUG_MD_STOP)
    {
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    }
}