#pragma once

#include "dma.h"

#define DOUG_IR_CHANNELS 2

typedef uint32_t IrValue;
typedef double	 IrVoltage;
typedef double	 IrDistance;

#ifdef MAIN
volatile IrValue    ir_values[DOUG_IR_CHANNELS] = {0};
volatile IrVoltage  ir_voltages[DOUG_IR_CHANNELS] = {0.0};
volatile IrDistance  ir_distances[DOUG_IR_CHANNELS] = {0.0};
#else
extern volatile IrValue    ir_values[DOUG_IR_CHANNELS];
extern volatile IrVoltage  ir_voltages[DOUG_IR_CHANNELS];
extern volatile IrDistance ir_distances[DOUG_IR_CHANNELS];
#endif

IrVoltage Doug_IR_value_to_voltage(IrValue value);
IrDistance Doug_IR_voltage_to_distance(IrVoltage voltage);
IrDistance Doug_IR_value_to_distance(IrValue value);