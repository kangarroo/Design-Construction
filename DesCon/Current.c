#include <stdio.h>
#include <math.h>
#include "current.h"
#include "voltage.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Constants.h"
#include <string.h>

int Current_Range(double voltage, int sel, int auto_range, double* current){
	if(auto_range == 1){		
		switch(sel){
			case 0: //00  1A
				if (voltage < 0.862){
					sel++;
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out /1.1 -> 1.4 /0.2->0.144
					*current = (voltage/(80*0.144));//0.2
					*current /= 1.308;
				}
				else {
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
					*current = (voltage/(20*0.144));//0.2
					*current /= 1.4239;
				}
			break;
				
			case 1: //01  250mA
				if (voltage < 2.6){//2.7
					sel++;
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current = (voltage/(20*1.4));
					*current /= 0.8874;
				}
				else if(voltage > 3.8){
					sel--;
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
					*current = (voltage/(20*0.144));//0.2
					*current /= 1.4239;
				}
				else {
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current = (voltage/(80*0.144));//0.2
					*current /= 1.308;
				}
			break;
				
			case 2://10  150mA
				if (voltage < 1.2){
					sel++;
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out
					*current = (voltage/(80*1.4));
					*current /= 0.829;
				}
				else if(voltage > 4.54){
					sel--;
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current = (voltage/(80*0.144));//0.2
					*current /= 1.308;
				}
				else {
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current = (voltage/(20*1.4));
					*current /= 0.8874;
				}
			break;
			
			case 3://11  45mA
				if(voltage > 4.8){
					sel--;
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current = (voltage/(20*1.4));
					*current /= 0.8874;
				}
				else {
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out
					*current = (voltage/(80*1.4));
					*current /= 0.829;
				}
			break;
		}
	} 
	else {
			switch(sel){
			case 0: //00 
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
					*current = (voltage/(20*0.144));
					*current /= 1.4239;
			break;
				
			case 1: //01 
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
					*current = (voltage/(80*0.144));
					*current /= 1.308;
			break;
				
			case 2://10  
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
					*current = (voltage/(20*1.4));
					*current /= 0.8874;
			break;
			
			case 3://11  
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7); //case 3 pin out		
					*current = (voltage/(80*1.4));//100*1.1
					*current /= 0.829;
			break;						
			}
		}
	return sel;
}

void write_current_value(double current_value, int menu){
	char str[15];
	char str2[15];
	
	sprintf(str2, "Current: %c", 272);
	if(menu == 'w'){
		sprintf(str, "%f", current_value);
		strcat(str,"\n");
		printf("%s",str);
	}
	else if(fabs(current_value)<1){
		if(fabs(current_value) < pow(10,-3)){
			current_value *= pow(10,6);
			sprintf(str, "%.1f       ", current_value);
			lcd_write_string(str, 0, 0);
			char str2[2];
			//sprintf(str2,"%cA",);
			lcd_write_string(" uA ",8,0);
		}	else if(fabs(current_value) < pow(10,0)){
			current_value *= pow(10,3);
			sprintf(str, "%.2f      ", current_value);
			lcd_write_string(str, 0, 0);
			lcd_write_string(" mA ",8,0);
		}
		GPIOE->BSRR = (1UL << (3+16));
		
		
	}else{
		GPIOE->BSRR = (1UL << 3);
		lcd_write_string("   Overload    ",0,0);
		//strcat(str,"\n");
		//printf("%s",str);
	}
	
	lcd_write_string(str2, 0, 1);
	if(menu == 'b'){
		lcd_write_string("Auto", 9, 1);
	}
	else if(menu == 'c'){
		lcd_write_string("1A", 10, 1);
	}
	else if(menu == 'd'){
		lcd_write_string("250mA", 10, 1);
	}
	else if(menu == 'e'){
		lcd_write_string("150mA", 10, 1);
	}
	else if(menu == 'f'){
		lcd_write_string("30mA", 10, 1);
	}
	else if(menu == 'w'){
		lcd_write_string("DLOG", 10, 1);
	}

	
}

int Measure_Current(int menu, int sel_V, int* sel_I){
	double ADC_In = 0;
	double voltage_value = 0;
	double current_value = 0;
	double average_voltage = 0;
	int polarity = 0;
	
	//start of averaging 'for' loop
	int  i = 0;
	for(i = 0; i<AVERAGE_LOOP; i++){
		ADC_In = read_ADC1();
		ADC_In = ADC_In * (BOARD_VOLTAGE/ADC_MAX);
		if(ADC_In < NORMALISE_VOLTAGE){
			polarity = 1;
		}
		else {
			polarity = 0;
		}
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		sel_V = Voltage_Range(ADC_In, sel_V, 1);
		voltage_value = Voltage_Display_Value(ADC_In,sel_V, polarity);
		average_voltage = average_voltage + voltage_value;
		Delay(MEASUREMENT_DELAY/AVERAGE_LOOP);
	}
	average_voltage /= AVERAGE_LOOP;
	average_voltage += VOLTAGE_CALIBRATION;
	average_voltage = fabs(average_voltage);
	//Controls the current ranging function based on which mode has been selected
	if(menu == 'b'||menu == 'w'){
		*sel_I = Current_Range(average_voltage, *sel_I, 1, &current_value);
	}
	else if(menu == 'c'){
		*sel_I = Current_Range(average_voltage, 0, 0, &current_value);
	}
	else if(menu == 'd'){
		*sel_I = Current_Range(average_voltage, 1, 0, &current_value);
	}
	else if(menu == 'e'){
		*sel_I = Current_Range(average_voltage, 2, 0, &current_value);
	}
	else if(menu == 'f'){
		*sel_I = Current_Range(average_voltage, 3, 0, &current_value);
	}
	
	if(polarity == 1){
		current_value = -current_value;
	}
	//writing current and voltage to LCD
	write_current_value(current_value, menu);
	return sel_V;
}
