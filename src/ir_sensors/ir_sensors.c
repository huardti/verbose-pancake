#include "ir_sensors.h"

IrVoltage Doug_IR_value_to_voltage(IrValue value)
{
    return (double) (value * 3.3 / 1024.0);
}

IrDistance Doug_IR_voltage_to_distance(IrVoltage voltage)
{
    /*
    Linear curve :
        60cm <> 1,06 V
        30cm <> 2 V
    Higher voltages : non-linear, but Doug flees anyway
    */
    return (IrDistance)
        1 /
        (voltage * (IrDistance) ((1/30. - 1/60.) / (2. - 1.06)));
}

IrDistance Doug_IR_value_to_distance(IrValue value)
{
    IrDistance return_value = Doug_IR_voltage_to_distance(Doug_IR_value_to_voltage(value));

    if(return_value >= 150)
        return 150;
    else
        return return_value;
}