#include <stdio.h>
#include <math.h>
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Transistor.h"
#include "Voltage.h"
#include "Constants.h"

void Test_Transistor(int menu, int* sel_T1, int* sel_T2){
	double v_beta_n = 0;
	double v_be_n = 0;
	double v_beta_p = 0;
	double v_be_p = 0;
	double volt_I = 0;
	double volt_D = 0;
	double NPN_beta = 0;
	double PNP_beta = 0;
	double current_diode = 0;
	int polarity = 0;
	char beta[1];
	
	sprintf(beta,"%c:",224);

	switch(menu){
		case 'm': //NPN Test
			GPIOB->BSRR= 1UL<<(15) | (1UL<<(8+16)) | (1UL<<(7+16)); //100
			Delay(100);
		
			v_beta_n = read_ADC1();
			v_beta_n = v_beta_n * (BOARD_VOLTAGE/ADC_MAX);
		
			if(v_beta_n < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			
			v_beta_n = fabs(v_beta_n - NORMALISE_VOLTAGE);
			*sel_T1 = Sel_Control(*sel_T1, v_beta_n);
			Voltage_Range(v_be_n, *sel_T2, 0);			
			v_beta_n = Voltage_Display_Value(v_beta_n, *sel_T1, polarity);
			
			GPIOB->BSRR= 1UL<<(15) | (1UL<<(8+16)) | (1UL<<(7)); //101
			Delay(100);
			v_be_n = read_ADC1();
			v_be_n = v_be_n * (BOARD_VOLTAGE/ADC_MAX);
			if(v_be_n < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			
			v_be_n = fabs(v_be_n - NORMALISE_VOLTAGE);
			*sel_T2 = Sel_Control(*sel_T2, v_be_n);
			Voltage_Range(v_beta_n, *sel_T1, 0);
			v_be_n = Voltage_Display_Value(v_be_n, *sel_T2, polarity);
			NPN_beta = Test_NPN(v_beta_n, v_be_n);
			
			if(fabs(v_be_n) <= 10){
				char str[15];
				sprintf(str, "%.0f  ", NPN_beta);
				char str1[15];
				sprintf(str1, "%.3f  ",v_be_n);
				lcd_write_string(beta,0,0);
				lcd_write_string(str,2,0);
				lcd_write_string("Vbe: ",6,0);
				lcd_write_string(str1,11,0);
			} else {
				lcd_write_string(" Connect Device ",0,0);
			}
			lcd_write_string("Transistor: NPN",0,1);
			
		break;
			
		case 'n': //PNP Test
			GPIOB->BSRR= 1UL<<(15+16) | (1UL<<(8)) | (1UL<<(7+16)); //010
			Delay(100);
		
			v_beta_p = read_ADC1();
			v_beta_p = v_beta_p* (BOARD_VOLTAGE/ADC_MAX);
		
			if(v_beta_p < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			v_beta_p = fabs(v_beta_p - NORMALISE_VOLTAGE);
			*sel_T1 = Sel_Control(*sel_T1, v_beta_p);
			Voltage_Range(v_be_p, *sel_T2, 0);	
		  v_beta_p = Voltage_Display_Value(v_beta_p,*sel_T1, polarity);
			
			GPIOB->BSRR= 1UL<<(15+16) | (1UL<<(8+16)) | (1UL<<(7)); //001
			Delay(100);
			v_be_p = read_ADC1();
			v_be_p = v_be_p * (BOARD_VOLTAGE/ADC_MAX);
			
			if(v_be_p < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			v_be_p = fabs(v_be_p - NORMALISE_VOLTAGE);
			*sel_T2 = Sel_Control(*sel_T2, v_be_p);
			Voltage_Range(v_beta_p, *sel_T1, 0);
			v_be_p = Voltage_Display_Value(v_be_p,*sel_T2,polarity);
			PNP_beta = Test_PNP(v_beta_p,v_be_p);
			
			if(fabs(v_be_p) < 10){
				char str2[15];
				sprintf(str2, "%.0f  ", PNP_beta);
				char str3[15];
				sprintf(str3, "%.3f  ",v_be_p);
				lcd_write_string(beta,0,0);
				lcd_write_string(str2,2,0);
				lcd_write_string("Vbe: ",6,0);
				lcd_write_string(str3,10,0);
			} else {
				lcd_write_string(" Connect Device ",0,0);
			}
			lcd_write_string("Transistor: PNP",0,1);
		break;
			
		case 'o': //Diode Tester
			GPIOB->BSRR= 1UL<<(15+16) | (1UL<<(8+16)) | (1UL<<(7+16)); //000
			Delay(100);
		
			volt_I = read_ADC1();
			volt_I = volt_I* (BOARD_VOLTAGE/ADC_MAX);
		
			if(volt_I < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			volt_I = fabs(volt_I - NORMALISE_VOLTAGE);
			*sel_T1 = Sel_Control(*sel_T1, volt_I);
			Voltage_Range(volt_D, *sel_T2, 0);
		  volt_I = Voltage_Display_Value(volt_I, *sel_T1, polarity);
			
			GPIOB->BSRR= 1UL<<(15+16) | (1UL<<(8)) | (1UL<<(7)); //011
			Delay(100);
			volt_D = read_ADC1();
			volt_D = volt_D * (BOARD_VOLTAGE/ADC_MAX);
			
			if(volt_D < NORMALISE_VOLTAGE){
				polarity = 1;
			}
			else {
				polarity = 0;
			}
			volt_D = fabs(volt_D - NORMALISE_VOLTAGE);
			*sel_T2 = Sel_Control(*sel_T2, volt_D);
			Voltage_Range(volt_I, *sel_T1, 0);
			volt_D = Voltage_Display_Value(volt_D, *sel_T2, polarity);
			current_diode = Test_Diode(volt_I);
			
			if(fabs(volt_D) <= 11){
				char str4[15];
				current_diode *= pow(10,3);
				sprintf(str4, "%.1fm  ", current_diode);
				char str5[15];
				sprintf(str5, "%.2f  ", volt_D);
				lcd_write_string("I: ",0,0);
				lcd_write_string(str4,2,0);
				lcd_write_string("Vd: ",8,0);
				lcd_write_string(str5,11,0);
			} else {
				lcd_write_string(" Connect Device ",0,0);
			}			
			lcd_write_string("Transistor:Diode",0,1);
			
		break;
	}
}

double Test_NPN (double v_beta_n, double v_be_n){
	double beta_out = 0;
	beta_out = ((12-v_beta_n)/300)/((12-v_be_n)/110000);
	return beta_out;
}

double Test_PNP (double v_beta_p, double v_be_p){
	double beta_out = 0;
	beta_out = ((v_beta_p+12)/300)/((v_be_p+12)/110000);
	return beta_out;
}

double Test_Diode (double volt_I){
	double current_diode = 0;
	current_diode = ((-volt_I + 0.761 + 12)/47000)*206;	
	return current_diode;
}

int Sel_Control(int sel, double voltage){
	switch(sel){
		case 0: //00  pm10V
			if (voltage < 0.71){//0.72
				sel++;
			}
		break;
			
		case 1: //01  pm5V
			if (voltage < 0.57){//0.57
				sel++;
			}
			else if(voltage > 1.49){//1.49
				sel--;
			}
		break;
			
		case 2://10  pm2V
			if (voltage < 0.71){//0.68
				sel++;
			}
			else if(voltage > 1.49){//1.49
				sel--;
			}
		break;
		
		case 3://11  pm1V
			if(voltage > 1.49){//1.49
				sel--;
			}
		break;
	}
	return sel;
}
