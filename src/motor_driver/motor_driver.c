#include "motor_driver.h"

#include <stdio.h>

#include "hall_encoder.h"

void Doug_MD_Init(void)
{
    MX_TIM1_Init();
    Doug_Hall_Encoder_Init();
}

void DougMD_Set_Direction(Doug_MD_Direction direction)
{
    if (direction == DOUG_MD_FORWARD)
    {
        HAL_GPIO_WritePin(DIR1_Gauche_GPIO_Port, DIR1_Gauche_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(PWM1_Droite_GPIO_Port, DIR2_Droite_Pin, GPIO_PIN_RESET);
    }
    else if (direction == DOUG_MD_REVERSE)
    {
        HAL_GPIO_WritePin(DIR1_Gauche_GPIO_Port, DIR1_Gauche_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PWM1_Droite_GPIO_Port, DIR2_Droite_Pin, GPIO_PIN_SET);
    }
    else if (direction == DOUG_MD_TOGGLE)
    {
        HAL_GPIO_TogglePin(DIR1_Gauche_GPIO_Port, DIR1_Gauche_Pin);
        HAL_GPIO_TogglePin(PWM1_Droite_GPIO_Port, DIR2_Droite_Pin);
    }
}

void DougMD_Set_Speed(Doug_MD_Speed speed)
{
    if (speed > 0)
    {
        Doug_MD_Speed correct_speed = speed;
        if (correct_speed >= 100)
            correct_speed = 100;

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, TIM1->ARR * correct_speed / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, TIM1->ARR * correct_speed / 100);
    }
}

void DougMD_Set_SpeedG(Doug_MD_Speed speed)
{
    Doug_MD_Speed correct_speed = speed;
    if (speed > 0)
    {
        if (correct_speed >= 100)
            correct_speed = 100;

        uint32_t rc = (TIM1->ARR * correct_speed / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rc);
    }
    else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    }
}

void DougMD_Set_SpeedD(Doug_MD_Speed speed)
{
    Doug_MD_Speed correct_speed = speed;
    if (speed > 0)
    {
        if (correct_speed >= 100)
            correct_speed = 100;

        uint32_t rc = (TIM1->ARR * correct_speed / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, rc);
    }
    else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
    }
}

double Doug_MD_PID(int error)
{
    static double kP=1.1f;
    static double kI=0.006f;
    // static int kD=50;

    static int lastError=0;
    static int errSum=0;

    double P,I;

    errSum += error;                 //Somme les erreurs depuis le début

    lastError = error;

    P = error * kP;                  //Proportionnelle
    I = errSum * kI;                 //Intégrale
    return P + I;
}

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

        uint32_t rc_d = (TIM1->ARR * correct_speed / 100);
        uint32_t rc_g = (TIM1->ARR * correct_speed / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,rc_d);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,rc_g);
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