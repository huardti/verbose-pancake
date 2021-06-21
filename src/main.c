#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "dma.h"

#define MAIN

#include "retarget/retarget.h"
#include <stdio.h>

#include "motor_driver/motor_driver.h"
#include "motor_driver/hall_encoder.h"

#include "ir_sensors/ir_sensors.h"

void SystemClock_Config(void);
volatile int button_toggle = 0;
volatile IrDistance consigne_distance = 40;
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    RetargetInit(&huart2);

    MX_TIM1_Init();
    MX_DMA_Init();
    MX_ADC1_Init();

    Doug_MD_Init();
    DougMD_Set_Direction(DOUG_MD_REVERSE);
    Doug_MD_Set_Motor(DOUG_MD_START);

    printf("\e[2J\e[1;1H");
    printf("DOUG FTW\n\r");

    consigne_d = 70;
    consigne_g = 70;

    while (1)
    {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ir_values, DOUG_IR_CHANNELS);

        printf
        (
            "\r ADC : GAUCHE (( %4ld - %4.2f V - %4.2f cm )) DROITE (( %4ld - %4.2f V - %4.2f cm ))",
            ir_values[0], ir_voltages[0], ir_distances[0],
            ir_values[1], ir_voltages[1], ir_distances[1]
        );

        if
        (
            !HAL_GPIO_ReadPin(COLLISION1_GPIO_Port, COLLISION1_Pin)
            || !HAL_GPIO_ReadPin(COLLISION2_GPIO_Port, COLLISION2_Pin)
        )
        {
            Doug_MD_Set_Motor(DOUG_MD_STOP);
            continue;
        }

        if(ir_distances[0] <= 30 || ir_distances[1] <= 30)
        {
            consigne_g = 30;
            consigne_d = consigne_g;
            DougMD_Set_Direction(DOUG_MD_REVERSE);
            Doug_MD_Set_Motor(DOUG_MD_START);
            continue;
        }


        if(ir_distances[0] <= 40 || ir_distances[1] <= 40)
        {
            Doug_MD_Set_Motor(DOUG_MD_STOP);
        }
        else if(ir_distances[0] <= 60)
        {
            consigne_d = 90;
            DougMD_Set_Direction(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }
        else if(ir_distances[1] <= 60)
        {
            consigne_g = 90;
            DougMD_Set_Direction(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }
        else if(ir_distances[0] > 60)
        {
            consigne_d = 70;
            DougMD_Set_Direction(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }
        else if(ir_distances[1] > 60)
        {
            consigne_g = 70;
            DougMD_Set_Direction(DOUG_MD_FORWARD);
            Doug_MD_Set_Motor(DOUG_MD_START);
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BLUE_USER_Pin)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

        DougMD_Set_Direction(DOUG_MD_TOGGLE);
    }
    if(GPIO_Pin == COLLISION1_Pin || GPIO_Pin == COLLISION2_Pin)
    {
        Doug_MD_Set_Motor(DOUG_MD_STOP);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc __attribute__((unused)))
{
    //ir_values[0] = HAL_ADC_GetValue(&hadc1);
}
