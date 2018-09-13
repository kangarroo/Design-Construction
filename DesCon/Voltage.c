#include <stdio.h>
#include <math.h>
#include "voltage.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Constants.h"
#include <string.h>

/* Function to intiialise ADC1    */
int Voltage_Range(double voltage, int sel, int auto_range){

if(auto_range == 1){		
		switch(sel){
			case 0: //00  pm10V
				if (voltage < 0.71){
					sel++;
					GPIOB->BSRR= 1UL<<(5+16) | 1UL<<(4); //case 1 pin out
				}
				else {
					GPIOB->BSRR = 1UL<<(5+16) | 1UL<<(4+16); //case 0 pin out
				}
			break;
				
			case 1: //01  pm5V
				if (voltage < 0.57){
					sel++;
					GPIOB->BSRR = 1UL<<(5) | 1UL<<(4+16); //case 2 pin out
				}
				else if(voltage > 1.49){//1.49
					sel--;
					GPIOB->BSRR = 1UL<<(5+16) | 1UL<<(4+16); //case 0 pin out
				}
				else {
					GPIOB->BSRR= 1UL<<(5+16) | 1UL<<(4); //case 1 pin out
				}
			break;
				
			case 2://10  pm2V
				if (voltage < 0.71){
					sel++;
					GPIOB->BSRR = 1UL<<5 | 1UL<<4; //case 3 pin out
				}
				else if(voltage > 1.49){//1.49
					sel--;
					GPIOB->BSRR= 1UL<<(5+16) | 1UL<<(4); //case 1 pin out
				}
				else {
					GPIOB->BSRR = 1UL<<(5) | 1UL<<(4+16); //case 2 pin out
				}
			break;
			
			case 3://11  pm1V
				if(voltage > 1.49){
					sel--;
					GPIOB->BSRR = 1UL<<(5) | 1UL<<(4+16); //case 2 pin out
				}
				else {
					GPIOB->BSRR = 1UL<<5 | 1UL<<4; //case 3 pin out
				}
			break;
				
				
		}
	} else{
			switch(sel){
			case 0 : //00  pm10V
					GPIOB->BSRR = 1UL<<(5+16) | 1UL<<(4+16); //case 0 pin out
					sel = 0;
			break;
				
			case 1 : //01  pm5V
					GPIOB->BSRR= 1UL<<(5+16) | 1UL<<(4); //case 1 pin out
					sel = 1;
			break;
				
			case 2 ://10  pm2V
					GPIOB->BSRR = 1UL<<(5) | 1UL<<(4+16); //case 2 pin out
					sel = 2;
			break;
			
			case 3 ://11  pm1V
					GPIOB->BSRR = 1UL<<5 | 1UL<<4; //case 3 pin out		
					sel = 3;
			break;						
		}

	
}
	return sel;
}

double Voltage_Display_Value(double voltage, int sel, int pol){
		
	switch(sel){
		case 0: //00  pm10V
			if(pol == 1){
				voltage = -((voltage*10)/NORMALISE_VOLTAGE);
			}
			else{
				voltage = ((voltage*10)/NORMALISE_VOLTAGE);
			}
		break;
			
		case 1: //01  pm5V
			if(pol == 1){
				voltage = -((voltage*5)/NORMALISE_VOLTAGE);
			}
			else{
				voltage = ((voltage*5)/NORMALISE_VOLTAGE);
			}
		break;
			
		case 2://10  pm2V
			if(pol == 1){
				voltage = -((voltage*2)/NORMALISE_VOLTAGE);
			}
			else{
				voltage = ((voltage*2)/NORMALISE_VOLTAGE);
			}
		break;
		
		case 3://11  pm1V
			if(pol == 1){
				voltage = -((voltage)/NORMALISE_VOLTAGE);
			}
			else{
				voltage = ((voltage)/NORMALISE_VOLTAGE);
			}
		break;
			
	}
	return voltage;
}

void Write_Voltage (char str[15], double voltage_value, double max_V, double min_V, int pol, int menu){
	if(menu == ('0')|| menu == ('1')|| menu == ('2')|| menu == ('3') || menu == ('4')|| menu == ('u')){
			if((voltage_value < max_V)&& pol == 0){
				if(menu == 'u'){
					sprintf(str, "%f            ", voltage_value);
					strcat(str,"\n");
					printf("%s",str);
				}else if(voltage_value<pow(10,0)){
					voltage_value *= pow(10,3);
					sprintf(str, "%.1f       ", voltage_value);
					lcd_write_string(str,0,0);
					lcd_write_string("mV   ", 9, 0);
				}else{
					sprintf(str, "%.2f     ", voltage_value);
					lcd_write_string(str,0,0);
					lcd_write_string("V   ", 9, 0);
				}
				GPIOE->BSRR = (1UL << (3+16));		
			}
			else if((voltage_value > min_V+VOLTAGE_CALIBRATION) && pol == 1){
				if(menu == 'u'){
					sprintf(str, "%f           ", voltage_value);
					strcat(str,"\n");
					printf("%s",str);
				}else if(fabs(voltage_value)<pow(10,0)){
					voltage_value *= pow(10,3);
					sprintf(str, "%.1f         ", voltage_value);
					lcd_write_string(str,0,0);
					lcd_write_string("mV   ", 9, 0);
				}else{
					sprintf(str, "%.2f       ", voltage_value);
					lcd_write_string(str,0,0);
					lcd_write_string("V   ", 9, 0);
				}
				//sprintf(str, "%.3f   ", voltage_value);
				GPIOE->BSRR = (1UL << (3+16));
				//lcd_write_string(str,0,0);
				//lcd_write_string("V   ", 9, 0);
				//strcat(str,"\n");
				//printf("%s",str);
			}
			else{
				// white space after overload to clear screen
				GPIOE->BSRR = (1UL << 3);
				lcd_write_string("   Overload    ",0,0);
				sprintf(str, "%f", voltage_value);
				//strcat(str,"\n");
				//printf("%s",str);
			}
			char range[16];
			//lcd_scrolling(LCD_SCROLLING_ON,300);
			//if(menu >= 48 || menu <= 52){
			if(menu == '0'||menu == '1'||menu == '2'||menu == '3'||menu == '4'||menu == 'u'){
				sprintf(range,"Voltage: %c",272);
				lcd_write_string(range,0,1);
				if(menu == '0'){lcd_write_string("Auto",9,1);}
				if(menu == '1'){lcd_write_string("10V",10,1);}
				if(menu == '2'){lcd_write_string("5V",10,1);}
				if(menu == '3'){lcd_write_string("2V",10,1);}
				if(menu == '4'){lcd_write_string("1V",10,1);}
				if(menu == 'u'){lcd_write_string("DLOG",9,1);}
			}
	}
	//if(menu >= 53 || menu <= 57){
	else{
		char range[16];
		if (menu == 'v'){
			sprintf(str, "%f", voltage_value);
			strcat(str,"\n");
			printf("%s",str);
		}else if(voltage_value<pow(10,0)){
			voltage_value *= pow(10,3);
			sprintf(str, "%.1f", voltage_value);
			lcd_write_string(str,0,0);
			lcd_write_string("mV   ", 9, 0);
		}else{
			sprintf(str, "%.2f   ", voltage_value);
			lcd_write_string(str,0,0);
			lcd_write_string("V   ", 9, 0);
		}
		lcd_write_string("AC Voltage: Auto",0,1);
		//sprintf(str, "%.3f   ", voltage_value);
		//sprintf(range,);
		//lcd_write_string(str,0,0);
		
		//if(menu == '5'||menu == 'v'){lcd_write_string("Auto",12,1);}
//		if(menu == '6'){lcd_write_string("10V",13,1);}
//		if(menu == '7'){lcd_write_string("5V",13,1);}
//		if(menu == '8'){lcd_write_string("2V",13,1);}
//		if(menu == '9'){lcd_write_string("1V",13,1);}		
	}

}

int Measure_Voltage(int menu, int sel_V){
	double average_voltage = 0;
	double ADC_In = 0;
	double voltage_value = 0;
	int polarity = 0;
	
	//start of averaging 'for' loop
	for(int i = 0; i<AVERAGE_LOOP; i++){
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		if(ADC_In < NORMALISE_VOLTAGE){
			polarity = 1;
		}
		else {
			polarity = 0;
		}
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		

		//Controls the voltage ranging function based on which mode has been selected
		//DC & AC Voltage Auto-range
		if((menu == '0')||(menu == '5')||menu == 'u'||menu == 'v'){
			sel_V = Voltage_Range(ADC_In, sel_V, 1);
		}
		//10V Voltage range
		else if(menu == '1'){
			sel_V = Voltage_Range(ADC_In, 0, 0);
		}
		//5V Voltage range
		else if(menu == '2'){
			sel_V = Voltage_Range(ADC_In, 1, 0);
		}
		//2V Voltage range
		else if(menu == '3'){
			sel_V = Voltage_Range(ADC_In, 2, 0);
		}
		//1V Voltage range
		else if(menu == '4'){
			sel_V = Voltage_Range(ADC_In, 3, 0);
		}	
		
		voltage_value = Voltage_Display_Value(ADC_In, sel_V, polarity);
		average_voltage = average_voltage + voltage_value;
		Delay(MEASUREMENT_DELAY/AVERAGE_LOOP);
	}
	average_voltage /= AVERAGE_LOOP;
	average_voltage += VOLTAGE_CALIBRATION;
	//AC Voltage scalling
	if(menu == '5'||menu == 'v'){
		average_voltage /= 10;
		if(average_voltage >= 0.061){
			//average_voltage = log(average_voltage)*0.0503 + 0.8057;
			average_voltage /= ((pow(4,-9)*pow(average_voltage,3))-(pow(4,-6)*pow(average_voltage,2))+(0.00165*average_voltage)+0.9215);
		} else {
			average_voltage /= ((log(average_voltage)*0.1503) + 1.4378);
		}
	}
	//AC Voltage END
	
	char str[15];

	
	//Writing the voltage to the LCD with Overload function
	//Voltage & AC Voltage Auto-range/10V Range
	if (menu == '0'||menu == '1'||menu == '5'||menu == 'u'||menu == 'v'){
		Write_Voltage(str, average_voltage, 11, -10, polarity, menu);
	}
	//Voltage 5V Range
	else if (menu == '2'){
		Write_Voltage(str, average_voltage, 6.1, -5, polarity, menu);
	}
	//Voltage 2V Range
	else if (menu == '3'){
		Write_Voltage(str, average_voltage, 2.44, -2, polarity, menu);
	}
	//Voltage 1V Range
	else if (menu == '4'){
		Write_Voltage(str, average_voltage, 1.22, -1, polarity, menu);
	}
	
	return sel_V;
}
