#include <stdio.h>
#include "STM32F4xx.h"
#include "Continuity.h"
#include "Resistance.h"
#include "Breadboard.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"


void Test_Continuity (){
	int ADC_In;
	int samples[9] = {0}; 
	int continuity[8] = {0};
	for(int i = 0;i<9;i++){
		samples[i] = read_ADC1();
		ADC_In = samples[i];
		char str[16];
		sprintf(str, "%d", ADC_In);
		lcd_write_string(str,0,1);
		
	}
	for(int i = 0;i<8;i++){
		int sample_Max = samples[i+1]+100;
		int sample_Min = samples[i+1]-100;
		if(!(samples[i] < sample_Max && samples[i] > sample_Min)){
			continuity[i] = 0;
		}else{
			continuity[i] = 1;
		}
	}
	
	for(int i = 0;i<8;i++){
		if (continuity[i] == 1){
			
			lcd_write_string("Continuity    ",0,0);
		}else{
			lcd_write_string("No Continuity",0,0);
			break;
		}
	}
	

	
	Delay(100);
}
