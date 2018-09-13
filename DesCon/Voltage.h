#ifndef _VOLTAGE_H_
#define _VOLTAGE_H_

int Voltage_Range(double voltage, int sel, int auto_range);
double Voltage_Display_Value(double voltage, int sel, int pol);
void Write_Voltage (char str[15],double voltage_value, double max_V, double min_V, int pol, int menu);
int Measure_Voltage(int menu, int sel_V);
#endif /*_VOLTAGE_H_*/
