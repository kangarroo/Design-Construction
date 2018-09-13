#include <stdio.h>
#include <math.h>
#include "Frequency.h"
#include "STM32F4xx.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Breadboard.h"
#include "Voltage.h"
#include "Timer.h"
#include "Constants.h"

#define FREQ_EN(VAL) GPIOE->BSRR = (1UL << (VAL));
#define CLEAR_COUNTER(VAL) GPIOC->BSRR = (1UL << (VAL));
#define READ_BITS(VAL1,VAL2,VAL3) GPIOB->BSRR = (1UL<<(VAL1)|1UL<<(VAL2)|1UL<<(VAL3));
#define SETBITS(msb, lsb)  GPIOB->BSRR = 1UL<<(msb) | 1UL<<(lsb);
#define CLOCK_SPEED 84000000 //84MHz
#define BASE_DURATION 8400 //100 microseconds
#define TIME_INCREMENT 20
#define ONE_MICRO_SECOND 84

int measure_frequency(int num_tests){
	int start_time = 0;
	double ADC_In[8] = {0};
	int count_val = 0;
	double frequency = 0;
	int duration = 0;


	init_Timer();

	SETBITS((5+16), (4));	//Set the voltage to +-5V range
	//TODO is this right, or are they reversed?
	FREQ_EN(7+16);	//Counter should not count yet hence turn off enable

	lcd_write_string("Frequency",0,1);

	//Clear the counter before starting
	CLEAR_COUNTER(5);
	Delay(5);				
	CLEAR_COUNTER(5+16);
	//TODO Possible fix could be to check that all bits are actually zero
	
	
	if(num_tests != 0){
		duration = frequency_test(BASE_DURATION*TIME_INCREMENT*num_tests);
		duration /= CLOCK_SPEED; //DURATION IN SECONDS
	}else{
		duration = frequency_test(BASE_DURATION);
		duration /= CLOCK_SPEED; //DURATION IN SECONDS
	}
	
	for(int i = 0; i <= 7; i++){ //TODO check if sequence is right, and if the right bits are targeted
		switch(i){
			case 7:
				READ_BITS(7+16,8+16,15+16);
				//lcd_write_string("0 0 0",0,0);//TODO Remove after testing
				break;
			case 6:
				READ_BITS(7,8+16,15+16);
				//lcd_write_string("1 0 0",0,0);//TODO Remove after testing
				break;
			case 5:
				READ_BITS(7+16,8,15+16);
				//lcd_write_string("0 1 0",0,0);//TODO Remove after testing
				break;
			case 4:
				READ_BITS(7,8,15+16);
				//lcd_write_string("1 1 0",0,0);//TODO Remove after testing
				break;
			case 3:
				READ_BITS(7+16,8+16,15);
				//lcd_write_string("0 0 1",0,0);//TODO Remove after testing
				break;
			case 2:
				READ_BITS(7,8+16,15);
				//lcd_write_string("1 0 1",0,0);//TODO Remove after testing
				break;
			case 1:
				READ_BITS(7+16,8,15);
				//lcd_write_string("0 1 1",0,0);//TODO Remove after testing
				break;
			case 0:
				READ_BITS(7,8,15);
				//lcd_write_string("1 1 1",0,0);//TODO Remove after testing
				break;			
		}
		Delay(5); //Short delay to make sure the bits have changed.

		ADC_In[i] = read_ADC1();
		ADC_In[i] = (ADC_In[i] * (BOARD_VOLTAGE/ADC_MAX));
		ADC_In[i] = fabs(ADC_In[i] - NORMALISE_VOLTAGE);
		ADC_In[i] = Voltage_Display_Value(ADC_In[i],1,0);

		if(ADC_In[i]>2.5){
			ADC_In[i] = 1;
		}else{
			ADC_In[i] = 0;
		}

		//Delay(5000);		//TODO Remove after testing
	}
	
	//Calculate the decimal value of the counter
	for(int i = 0;i<=7;i++){
		count_val += ADC_In[i]*pow(2,i);
	}
	
	//If counter has overflown, the frequency is too low to measure.
	if (ADC_In[7] ==1){
		lcd_write_string("Unmeasureable",0,0);
		num_tests= 0;
	}else if(count_val < 6){ //If the counter has counted to less than 6 peaks, increase the time_constant. 
		num_tests++;
	}else{
		if(num_tests != 0){
			frequency = count_val / duration;
		}else{
			frequency = count_val / duration;
		}

		char str[15];
		sprintf(str,"%.2fHz",frequency);
		lcd_write_string(str,0,0);
		
		char str2[15];
		sprintf(str2,"Count: %d",count_val);
		lcd_write_string(str,0,1);

		num_tests= 0;
	}
	return num_tests;
}

int frequency_test(int test_time){
	int test = 0;
	

	TIM2->CNT = (0x00000000);
	TIM2->CR1 |= TIM_CR1_CEN; //Enable timer
	
	FREQ_EN(7);				// Enable counter to count, then immediately start timing

	while(test<test_time){
		test = TIM2->CNT;
	}

	FREQ_EN(7+16);			//Disable counter from counting

	TIM2->CR1 &= TIM_CR1_CEN;	//Disable timer
	
	return test;
}
