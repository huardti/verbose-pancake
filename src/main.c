#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "motor_driver/motor_driver.h"

void SystemClock_Config(void);
volatile int button_toggle = 0;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

    Doug_MD_Set(DOUG_MD_FORWARD, 100);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    /* USER CODE END 2 */

    while (1)
    {

    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        Doug_MD_Set(DOUG_MD_TOGGLE, 100);
    }
}
