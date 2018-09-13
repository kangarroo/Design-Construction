#include <stdio.h>
#include <math.h>
#include "voltage.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Constants.h"
#include <string.h>
#include "Logic.h"

int test_logic (int menu, int sel_V){
	double ADC_In = 0;
	
	ADC_In = read_ADC1();
	ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
	ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
	sel_V = Voltage_Range(ADC_In,sel_V,1);
	ADC_In = Voltage_Display_Value(ADC_In, sel_V,0);
	
	//If R-> TTL, S->CMOS 5, T->CMOS 3.3
	switch(menu){
		case 'r':
			if(ADC_In>2){
				lcd_write_string("High     ",0,0);
			}else if (ADC_In>=0 && ADC_In<= 0.8){
				lcd_write_string("Low      ",0,0);
			}else{
				lcd_write_string("Undefined",0,0);
			}
			lcd_write_string("Logic: TTL",0,1);
			Delay(200);
		break;
			
		case 's':
			if(ADC_In>3.5){
				lcd_write_string("High     ",0,0);
			}else if (ADC_In>=0 && ADC_In<= 1.5){
				lcd_write_string("Low      ",0,0);
			}else{
				lcd_write_string("Undefined",0,0);
			}
			lcd_write_string("Logic: CMOS 5",0,1);
			Delay(200);
		break;
			
		case 't':
			if(ADC_In>2){
				lcd_write_string("High     ",0,0);
			}else if (ADC_In>=0 && ADC_In<= 0.8){
				lcd_write_string("Low      ",0,0);
			}else{
				lcd_write_string("Undefined",0,0);
			}
			lcd_write_string("Logic: CMOS 3.3",0,1);
			Delay(200);
		break;
	}
	
	return sel_V;
}

