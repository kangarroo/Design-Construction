#include <stdio.h>
#include <math.h>
#include "Light.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Voltage.h"
#include "Constants.h"

int Select_Colour (int menu){
	int sel = 0;
	switch(menu){
			case 'g': //00  White
					GPIOB->BSRR = 1UL<<(8+16) | 1UL<<(7+16); //case 0 pin out
			break;
				
			case 'h': //01  Red
					GPIOB->BSRR= 1UL<<(8+16) | 1UL<<(7); //case 1 pin out
			    sel = 1;
			break;
				
			case 'i'://10  Green
					GPIOB->BSRR = 1UL<<(8) | 1UL<<(7+16); //case 2 pin out
			    sel = 2;
			break;
			
			case 'j'://11  Blue
					GPIOB->BSRR = 1UL<<8 | 1UL<<7; //case 3 pin out	
				  sel = 3;
			break;						
		}
	return sel;
}

double Light_Value (int sel, double voltage){
	switch(sel){
		case 0:
			voltage = voltage * (1000/8)*5;
			break;
		case 1:
			voltage = 100 * pow(10 , ((log10(voltage/0.1891))/log10(10.95)));
			break;
	  case 2:
			voltage = 100 * pow(10 , ((log10(voltage/0.2036))/log10(11.86)));
			break;
		case 3:
			voltage = 100 * pow(10 , ((log10(voltage/0.1067))/log10(11.91)));
			break;
	}
	return voltage;
}

void Write_Lux_Value(double light_value, int red_value, int green_value, int blue_value, int menu){
	char str[15];
	if(menu != 'k'){
		sprintf(str,"%.0f     ",light_value);
		lcd_write_string(str,0,0);
	}else{
		char red[15];
		char green[15];
		char blue[15];
		sprintf(red,"R%.0d   ",red_value);
		sprintf(green,"G%.0d  ",green_value);
		sprintf(blue,"B%.0d   ",blue_value);
		lcd_write_string(red,0,0);
		//Delay(500);
		lcd_write_string(green,5,0);
		//Delay(500);
		lcd_write_string(blue,10,0);
		
	}

	if(menu == 'g'){lcd_write_string("Light: White      ",0,1);}
	else if(menu == 'h'){lcd_write_string("Light: Red      ",0,1);}
	else if(menu == 'i'){lcd_write_string("Light: Green    ",0,1);}
	else if(menu == 'j'){lcd_write_string("Light: Blue     ",0,1);}	
	else if(menu == 'k'){lcd_write_string("Light: RGB Cycle",0,1);}
}

int Measure_Light(int menu, int sel_V, int* sel_R){
	double ADC_In = 0;
	double voltage_value = 0;
	double light_value = 0;
	double red_value = 0;
	double green_value = 0;
	double blue_value = 0;
	
	if(menu != 'k'){
		//selecting colour
		*sel_R = Select_Colour(menu);
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		//switching auto-ranging on for voltage
		sel_V = Voltage_Range(ADC_In, sel_V, 1);
		voltage_value = Voltage_Display_Value(ADC_In, sel_V, 0);
		//conversion between voltage and lux
		light_value = Light_Value(*sel_R, voltage_value);
		if(menu == 'g'){
			if(light_value<40){
				light_value *= 15;
			}else if(light_value<1000){
				light_value *= 5.57;
			}else{
				light_value *= 6.45;
			}
		}
		//writing lux and voltage to LCD
		Write_Lux_Value(light_value,(int)red_value,(int)green_value,(int)blue_value, menu);
		Delay(500);
	}else{
			for(int i =0;i<3;i++){
				*sel_R = Select_Colour('h');
				ADC_In = read_ADC1();
				ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
				ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
				sel_V = Voltage_Range(ADC_In, sel_V, 1);
				voltage_value = Voltage_Display_Value(ADC_In, sel_V, 0);
				red_value = Light_Value(*sel_R,voltage_value);
			}
			for(int i =0;i<3;i++){
				*sel_R = Select_Colour('i');
				ADC_In = read_ADC1();
				ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
				ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
				sel_V = Voltage_Range(ADC_In, sel_V, 1);
				voltage_value = Voltage_Display_Value(ADC_In, sel_V, 0);
				green_value = Light_Value(*sel_R,voltage_value);
			}
			for(int i =0;i<3;i++){
				*sel_R = Select_Colour('j');
				ADC_In = read_ADC1();
				ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
				ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
				sel_V = Voltage_Range(ADC_In, sel_V, 1);
				voltage_value = Voltage_Display_Value(ADC_In, sel_V, 0);
				blue_value = Light_Value(*sel_R,voltage_value);
			}
		Delay(500);
		Write_Lux_Value(light_value,(int)red_value,(int)green_value,(int)blue_value, menu);
	}
	return sel_V;
	
}

