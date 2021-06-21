#pragma once

#include "gpio.h"
#include "tim.h"

#include "pid/pid.h"
typedef enum _Doug_MD_Direction
{
	DOUG_MD_REVERSE,
	DOUG_MD_FORWARD,
	DOUG_MD_TOGGLE,
} Doug_MD_Direction;

typedef uint8_t Doug_MD_Speed;

volatile Doug_MD_Speed Doug_speed;
typedef enum _Doug_MD_Motor_State
{
	DOUG_MD_START,
	DOUG_MD_STOP,
} Doug_MD_Motor_State;

void Doug_MD_Init(void);
void DougMD_Set_Direction(Doug_MD_Direction direction);

void Doug_MD_Set_Params(Doug_MD_Direction direction, Doug_MD_Speed speed);
void Doug_MD_Set_Motor(Doug_MD_Motor_State state);

void DougMD_Set_Speed(Doug_MD_Speed speed);
void DougMD_Set_SpeedG(Doug_MD_Speed speed);
void DougMD_Set_SpeedD(Doug_MD_Speed speed);