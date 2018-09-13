#ifndef _CURRENT_H_
#define _CURRENT_H_

int Current_Range(double voltage, int sel, int auto_range, double* current);
void write_current_value(double current_value, int menu);
int Measure_Current(int menu, int sel_V, int* sel_I);
#endif /*_CURRENT_H_*/
