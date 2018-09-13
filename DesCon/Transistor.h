#ifndef _TRANSISTOR_H_
#define _TRANSISTOR_H_

void Test_Transistor(int menu, int* sel_T1, int* sel_T2);
double Test_NPN (double v_beta_n, double v_be_n);
double Test_PNP (double v_beta_p, double v_be_p);
double Test_Diode (double volt_I);
int Sel_Control(int sel, double voltage);

#endif /*_TRANSISTOR_H_*/
