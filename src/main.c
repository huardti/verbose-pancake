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
    DougMD_Set_Direction(DOUG_MD_FORWARD);
    Doug_MD_Set_Motor(DOUG_MD_START);

    printf("\e[2J\e[1;1H");
    printf("DOUG FTW\n\r");

    consigne = 70;


    while (1)
    {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ir_values, DOUG_IR_CHANNELS);

        for(int i = 0 ; i < DOUG_IR_CHANNELS ; ++i)
        {
            ir_voltages[i] = Doug_IR_value_to_voltage(ir_values[i]);
        }

        printf
        (
            "\r ADC : GAUCHE (( %4ld - %4.2f V )) DROITE (( %4ld - %4.2f V ))",
            ir_values[0], ir_voltages[0],
            ir_values[1], ir_voltages[1]
        );
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BLUE_USER_Pin)
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

        DougMD_Set_Direction(DOUG_MD_TOGGLE);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc __attribute__((unused)))
{
    //ir_values[0] = HAL_ADC_GetValue(&hadc1);
}
