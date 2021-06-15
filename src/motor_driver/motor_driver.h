#pragma once

typedef enum _Doug_MD_Direction
{
	DOUG_MD_REVERSE,
	DOUG_MD_FORWARD,
	DOUG_MD_TOGGLE,
} Doug_MD_Direction;

typedef uint16_t Doug_MD_Speed;


void Doug_MD_Set(Doug_MD_Direction direction, Doug_MD_Speed speed);