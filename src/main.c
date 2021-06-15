#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
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

    Doug_MD_Set_Params(DOUG_MD_FORWARD, 30);
    Doug_MD_Set_Motor(DOUG_MD_START);

    Doug_Hall_Encoder_Init();

    printf("\e[2J\e[1;1H");
    printf("DOUG FTW\n\r");

    while (1)
    {
        Doug_Hall_Encoder_print_info();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        Doug_MD_Set_Params(DOUG_MD_TOGGLE, 25);
    }
}