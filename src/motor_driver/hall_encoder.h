#pragma once

#include <inttypes.h>

#include "pid/pid.h"

#define time_max_speed 3
#define time_min_speed 40

extern volatile int consigne_d;
extern volatile int consigne_g;

extern volatile Doug_PID_param PID_motor;


/*
extern volatile int errorD;
extern volatile int errorG;
extern volatile int PID_D;
extern volatile int PID_G;
*/

#ifdef MAIN
volatile uint16_t time_encoder_D;
volatile uint16_t time_encoder_G;

volatile uint16_t speed_G;
volatile uint16_t speed_D;
#else
extern volatile uint16_t time_encoder_D;
extern volatile uint16_t time_encoder_G;

extern volatile uint16_t speed_G;
extern volatile uint16_t speed_D;
#endif

/* debug PID */
// printf("speed %4d,  %4d ; error %4d %4d ; PID : %d, %d", speed_G,speed_D, errorD, errorG, PID_G, PID_D);

void Doug_Hall_Encoder_Init(void);
void Doug_Hall_Encoder_print_info(void);