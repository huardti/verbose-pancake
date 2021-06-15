#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"

#include "retarget/retarget.h"
#include <stdio.h>

#include "motor_driver/motor_driver.h"
#include "motor_driver/hall_encoder.h"

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
    MX_ADC1_Init();

//    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_IT(&hadc1);

    Doug_MD_Init();
    DougMD_Set_Direction(DOUG_MD_FORWARD);
    Doug_MD_Set_Motor(DOUG_MD_START);

    printf("\e[2J\e[1;1H");
    printf("DOUG FTW\n\r");

    consigne = 70;

    while (1)
    {
        printf("\r");
        HAL_Delay(100);
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    int16_t adc_value = HAL_ADC_GetValue(&hadc1);
    float   voltage = (float)adc_value * 3.3 / 1024;
 //   if() // 558-713

    printf("\r ADC : %d", adc_value);

//        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


}
