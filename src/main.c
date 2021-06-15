#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "retarget/retarget.h"
#include <stdio.h>

#include "motor_driver/motor_driver.h"

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

    printf("DOUG FTW\n");

    while (1)
    {

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
