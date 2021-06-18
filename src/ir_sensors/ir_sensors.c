#include "ir_sensors.h"

IrVoltage Doug_IR_value_to_voltage(IrValue value)
{
	return (double) (value * 5.0 / 1024.0);
}