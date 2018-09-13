#include "STM32F4xx.h"
#include <stdio.h>
#include <math.h>
#include "Timer.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"

void init_Timer(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //emable timer 2 clock
	TIM2->PSC = 0; //Prescaler
	TIM2->ARR = 0xFFFFFFFF; //Auto reload
	//lcd_write_string("TIME INIT",0,0);
	//TIM2->CR1 = TIM_CR1_CEN;
}
