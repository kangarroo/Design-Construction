#ifndef _LIGHT_H_
#define _LIGHT_H_

int Select_Colour (int menu);
double Light_Value (int sel, double voltage);
void Write_Lux_Value(double light_value, int red_value, int green_value, int blue_value, int menu);
int Measure_Light(int menu, int sel_V, int* sel_R);
#endif /*_LIGHT_H_*/
