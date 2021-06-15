#pragma once

#include <inttypes.h>

extern volatile uint16_t time_encoder_D;
extern volatile uint16_t time_encoder_G;

void Doug_Hall_Encoder_Init(void);
void Doug_Hall_Encoder_print_info(void);