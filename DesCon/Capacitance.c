#include <stdio.h>
#include <math.h>
#include "Capacitance.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Voltage.h"
#include "Timer.h"
#include "Constants.h"

#define SETBITS(msb, lsb)  GPIOB->BSRR = 1UL<<(msb) | 1UL<<(lsb);
//CLK == 84MHz
void measure_capacitance(void){
	
	double voltage = 0;
	double capacitance = 0;
	double ADC_In = 0;
	double end_Time = 0;
	int start_time = 0 ;
	double time_constant_fraction = 0;
	int mode = 0;
	
	init_Timer();
	SETBITS((5+16), (4));					//Set the voltage to +-5V, nothing less is needed.
	
	//Discharge
	SETBITS((8+16), (7+16));					//Set the mux sel bits to 00
	do { //REad voltage once, then make sure it is less than 0.01 V
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		voltage = ((ADC_In*5)/1.5);
	}
	while(voltage > 0.01);

	//Loop 1
	SETBITS((8), (7));		//Set the mux sel bits to 11
	TIM2->CNT = 0x00000000;		//Reset the timer
	TIM2->CR1 |= TIM_CR1_CEN;	//Enable the timer
	start_time = TIM2->CNT;		//Find the time passed JUST before the while loop

	while(ADC_In < 3026) { //While voltage is less than (bitvalue) 3245 = 7.585V 
			ADC_In = read_ADC1();
	}
	end_Time = TIM2->CNT - start_time; 	//Read the time STRAIGH after the while loop is finished
	TIM2->CR1 &= TIM_CR1_CEN;			//Disable timer
	
	SETBITS((8+16), (7+16));			//Set the mux sel bits to 00	
	end_Time /= 84000000; 				//Find end_time = duration in seconds
	
	if (end_Time >= 0.005) { //If end_time is bigger than 5ms, be complete
		mode =1;
	}


	//Discharge
	SETBITS((8+16), (7+16));					//Set the mux sel bits to 00
	do { //REad voltage once, then make sure it is less than 0.01 V
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		voltage = ((ADC_In*5)/1.5);
	}
	while(voltage > 0.01);

	
	//If not complete, do loop 2
	if (mode == 0) {
		SETBITS((8), (7+16));		//Set the mux sel bits to 10
		TIM2->CNT = 0x00000000;		//Reset the timer
		TIM2->CR1 |= TIM_CR1_CEN;	//Enable the timer
		start_time = TIM2->CNT;		//Find the time passed JUST before the while loop

		while(ADC_In < 3026) { //While voltage is less than (bitvalue) 3245 = 7.585V 
				ADC_In = read_ADC1();
		}
		end_Time = TIM2->CNT - start_time; 	//Read the time STRAIGH after the while loop is finished
		TIM2->CR1 &= TIM_CR1_CEN;			//Disable timer
		
		SETBITS((8+16), (7+16));			//Set the mux sel bits to 00	
		end_Time /= 84000000; 				//Find end_time = duration in seconds
		
		if (end_Time >= 0.005) { //If end_time is bigger than 5ms, be complete
			mode =2;
		}
	}
	

	//Discharge
	SETBITS((8+16), (7+16));					//Set the mux sel bits to 00
	do { //REad voltage once, then make sure it is less than 0.01 V
		ADC_In = read_ADC1();
		ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));
		ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
		voltage = ((ADC_In*5)/1.5);
	}
	while(voltage > 0.01);


	//If not complete, do loop 3
	if (mode == 0) {
		SETBITS((8+16), (7));		//Set the mux sel bits to 01
		TIM2->CNT = 0x00000000;		//Reset the timer
		TIM2->CR1 |= TIM_CR1_CEN;	//Enable the timer
		start_time = TIM2->CNT;		//Find the time passed JUST before the while loop

		while(ADC_In < 3026) { //While voltage is less than (bitvalue) 3245 = 7.585V 
				ADC_In = read_ADC1();
		}
		end_Time = TIM2->CNT - start_time; 	//Read the time STRAIGH after the while loop is finished
		TIM2->CR1 &= TIM_CR1_CEN;			//Disable timer
		
		SETBITS((8+16), (7+16));			//Set the mux sel bits to 00	
		end_Time /= 84000000; 				//Find end_time = duration in seconds
		
		if (end_Time >= 0.002) { //If end_time is bigger than 4.5ms, be complete
			mode =3;
		}
	}

	//Final processing:
	ADC_In = (ADC_In * (BOARD_VOLTAGE/ADC_MAX));//Find the actual voltage resulting from the last ADC reading.
	ADC_In = fabs(ADC_In - NORMALISE_VOLTAGE);
	voltage = ((ADC_In*5)/1.5);

	time_constant_fraction = voltage / 7.585; 	//Find how many time constants (RC) was measured
	end_Time /= time_constant_fraction; 		//Find exactly 1 RC

	//Depending on which mode was completed, do the correct T=RC calculation with correct R
	switch (mode) {
		case 0:
			capacitance = 0; 					//Capacitance could not be measured
			break;
		case 1:
			capacitance = end_Time/510; 		//Calculate the capacitance based on which resistor is used in 11
			capacitance /= 1.58;
			break;
		case 2:
			capacitance = end_Time/51000; 		//Calculate the capacitance based on which resistor is used in 10
			capacitance /= 1.58;
			break;
		case 3:
			capacitance = end_Time/4700000; 	//Calculate the capacitance based on which resistor is used in 01
			capacitance /= 4.73;
			break;
	}
	//capacitance /= ;
	
	//Print Capacitance with correct prefixes
	char str[15];
	char str_test[15];
	if (mode == 0) {
			sprintf(str,"C too small");	
	}else if (capacitance < pow(10,-9)){
			capacitance *= pow(10, 12);
			sprintf(str,"%.2fpF     ", capacitance);			
	}else if (capacitance < pow(10, -6)) {
			capacitance *= pow(10, 9);
			sprintf(str,"%.2fnF     ", capacitance);			
	}else if (capacitance < pow(10, -3)) {
			capacitance *= pow(10, 6);
			sprintf(str,"%.2fuF     ", capacitance);		
	}else if (capacitance < pow(10, 0)) {
			capacitance *= pow(10, 3);
			sprintf(str,"%.2fmF     ", capacitance);
			
	} else {
			sprintf(str,"%.2fF      ", capacitance);
	}
	lcd_write_string("Capacitance",0,1);
 	lcd_write_string(str,0,0);
	//lcd_write_string(str_test,0,1);
	Delay(500);
}
