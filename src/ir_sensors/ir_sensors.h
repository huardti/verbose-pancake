#pragma once

#include "dma.h"

#define DOUG_IR_CHANNELS 2

typedef uint32_t IrValue;
typedef double	 IrVoltage;

#ifdef MAIN
volatile IrValue    ir_values[DOUG_IR_CHANNELS] = {0};
volatile IrVoltage  ir_voltages[DOUG_IR_CHANNELS] = {0.0};
#else
extern volatile IrValue    *ir_values;
extern volatile IrVoltage  *ir_voltages;
#endif

IrVoltage Doug_IR_value_to_voltage(IrValue value);