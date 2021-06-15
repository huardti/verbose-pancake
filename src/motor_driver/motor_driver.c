#include "gpio.h"
#include "motor_driver.h"

void Doug_MD_Set(Doug_MD_Direction direction, Doug_MD_Speed speed)
{
	if(direction == DOUG_MD_FORWARD)
	{
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	}
	else if(direction == DOUG_MD_REVERSE)
	{
    		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

	}
	else if(direction == DOUG_MD_TOGGLE)
	{
    		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11);

	}

	Doug_MD_Speed correct_speed = speed;
	if(correct_speed >= 100) correct_speed = 100;


}