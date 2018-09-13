#include <stdio.h>
#include <math.h>
#include "Resistance.h"
#include "Voltage.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "STM32F4xx.h"
#include "Breadboard.h"
#include "Constants.h"
#include <string.h>

int Switch_Resistance(double voltage, int sel, double* current_R, int auto_range){
	if(auto_range == 1){
		switch(sel){
			case 0: //00  1M range
				if (voltage < 0.776){
					sel++;
					GPIOB->BSRR= 1UL<<(8+16) | (1UL<<7); //case 1 pin out
					*current_R = 86*(pow(10,-6));
				}
				else {
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
					*current_R = 7.56*(pow(10,-6));
				}
			break;
				
			case 1: //01  100K range
				if (voltage < 0.868){
					sel++;
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current_R = 0.99*(pow(10,-3));
				}
				else if(voltage > 10){
						sel--;
						GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
						*current_R = 7.56*(pow(10,-6));
				}
				else {
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current_R = 86*(pow(10,-6));
				}
			break;
				
			case 2://10  10K Range
				if (voltage < 0.102){
					sel++;
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out
					*current_R = 4*(pow(10,-3));
				}
				else if(voltage > 10){
					sel--;
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current_R = 86*(pow(10,-6));
				}
				else {
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current_R = 0.99*(pow(10,-3));
				}
			break;
			
			case 3://11  100 range
				if(voltage > 2.108){
					sel--;
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current_R = 0.99*(pow(10,-3));
				}
				else {
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out
					*current_R = 4*(pow(10,-3));
				}
			break;
				
				
		}
	}else{
		switch(sel){
			case 0:
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
					*current_R = 7.56*(pow(10,-6));
					break;
			case 1:
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current_R = 86*(pow(10,-6));
          break;			
			case 2:
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current_R = 0.99*(pow(10,-3));		
					break;
			case 3:
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out
					*current_R = 4*(pow(10,-3));
					break;
		}
			
	}
	return sel;
}

int Measure_Resistance_Continuity(int sel_V, int menu, int* sel_R, int* sel_R_OLD){ //Mode 0 =  Resistance, Mode 1 = Continuity
	double ADC_In = 0;
	int polarity = 0;
	double voltage_value = 0;
	double resistance_value = 0;
	double r_current;		
	double average_voltage = 0;
	char str2[2];
	//start of averaging 'for' loop
	for(int i = 0; i<AVERAGE_LOOP; i++){
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		if(ADC_In < NORMALISE_VOLTAGE){
			polarity = 1;
		}
		else{
			polarity = 0;
		}
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		
		//if(*sel_R<*sel_R_OLD){
			sel_V = Voltage_Range(ADC_In, sel_V, 1);
		//}else{
		//	sel_V = Voltage_Range(ADC_In, 0, 0);
		//}

		voltage_value = Voltage_Display_Value(ADC_In, sel_V, polarity);
		average_voltage = average_voltage + voltage_value;
		//*sel_R_OLD = *sel_R;
		Delay(MEASUREMENT_DELAY/AVERAGE_LOOP);
	}
	average_voltage /= AVERAGE_LOOP;
	average_voltage += VOLTAGE_CALIBRATION;
	
	if (menu == '6' || menu == 'l' ||menu == 'x'){
		*sel_R = Switch_Resistance(average_voltage, *sel_R, &r_current, 1);
	}
	else if (menu == '7'){
		*sel_R = Switch_Resistance(average_voltage, 0, &r_current, 0);
	}
	else if (menu == '8'){
		*sel_R = Switch_Resistance(average_voltage, 1, &r_current, 0);
	}
	else if (menu == '9'){
		*sel_R = Switch_Resistance(average_voltage, 2, &r_current, 0);
	}
	else if (menu == 'a'){
		*sel_R = Switch_Resistance(average_voltage, 3, &r_current, 0);
	}	
	
	resistance_value = voltage_value/r_current;
	if (menu == 'l'){
		if(resistance_value < 50 & resistance_value > 0){
			lcd_write_string("Continuity    ",0,0);
			GPIOE->BSRR = (1UL << 3);
			char str[15];
			//sprintf(str, "%.2f                  ", resistance_value);
//			lcd_write_string(str, 0, 1);
//			sprintf(str2,"%c",222);
//			lcd_write_string(str2, 15, 1);
		}
		else if(resistance_value < 0){
			lcd_write_string("Continuity    ",0,0);
			GPIOE->BSRR = (1UL << 3);
//			lcd_write_string("0.00            ", 0, 1);
//			sprintf(str2,"%c",222);
//			lcd_write_string(str2, 15, 1);
		}
		else {
			lcd_write_string("No Continuity",0,0);
			GPIOE->BSRR = (1UL << (3+16));
			char str[15];
			//sprintf(str, "%.2f                 ", resistance_value);
//			sprintf(str2,"%c",222);
//			lcd_write_string(str, 0, 1);
//			lcd_write_string(str2, 15, 1);
		}
	} else{		
		//writing resistance and voltage to LCD
		char str[15];
		char str2[2];
		if(menu == 'x'){
			sprintf(str, "%f", resistance_value);
			strcat(str,"\n");
			printf("%s",str);
			
			//lcd_write_string(str,0,0);
			lcd_write_string("Resistance: Auto", 0, 1);
		}else if(resistance_value < 1100000 && resistance_value > 0 ){
			if(resistance_value>pow(10,6)){
				resistance_value *= pow(10,-6);
				sprintf(str2,"M%c",222);
			} else if(resistance_value > pow(10,3)){
				resistance_value *= pow(10,-3);
				sprintf(str2,"k%c",222);
			} else{
				sprintf(str2," %c",222);
			}
			
			sprintf(str, "%.2f       ", resistance_value);
			//sprintf(str2, "%f", voltage_value);
			lcd_write_string(str, 0, 0);
			lcd_write_string(str2,14,0);
			//strcat(str,"\n");
			//printf("%s",str);
		} else if (resistance_value < 0){
			lcd_write_string("0.00           ",0,0);
			sprintf(str2,"%c",222);
			lcd_write_string(str2,14,0);
			//strcat(str,"\n");
			//printf("%s",str);
		} else {
			//GPIOE->BSRR = (1UL << 3);
			lcd_write_string("   Overload    ",0,0);
			//strcat(str,"\n");
			//printf("%s",str);
		}
		
		if(menu == '6'){lcd_write_string("Resistance: Auto", 0, 1);}
		else if(menu == '7'){lcd_write_string("Resistance: 1M  ",0,1);}
		else if(menu == '8'){lcd_write_string("Resistance: 100K",0,1);}
		else if(menu == '9'){lcd_write_string("Resistance: 10K ",0,1);}
		else if(menu == 'a'){lcd_write_string("Resistance: 100R",0,1);}
		else if(menu == 'x'){lcd_write_string("Resistance: DLOG",0,1);}
	}
	return sel_V;
}
