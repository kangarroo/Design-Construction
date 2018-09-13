#ifndef _RESISTANCE_H_
#define _RESISTANCE_H_


int Switch_Resistance(double voltage, int sel, double* current_R, int auto_range);
int Measure_Resistance_Continuity(int sel_V, int menu, int* sel_R, int* sel_R_OLD);

#endif /*_RESISTANCE_H_*/
