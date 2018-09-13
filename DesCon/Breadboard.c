#include "STM32F4xx.h"
#include "Breadboard.h"


//Initialises J5 pins
void J5_Init(void){
	RCC->AHB1ENR  |= ((1UL <<  1) ); /*Turns on clock for GPIO B*/
	
	GPIOB->MODER  &= ~((3UL << 2* 4)|
										 (3UL << 2* 5)|
										 (3UL << 2* 7)|
										 (3UL << 2* 8)|
										 (3UL << 2* 15));
	
	GPIOB->MODER  |=  ((1UL << 2* 4)|
										 (1UL << 2* 5)|
										 (1UL << 2* 7)|
										 (1UL << 2* 8)|
										 (1UL << 2* 15));
										
	GPIOB->OTYPER &= ~((1UL <<    4)|
										 (1UL <<    5)|
										 (1UL <<    7)|
										 (1UL <<    8)|
										 (1UL <<    15));
	
	GPIOB->OSPEEDR  &= ~((3UL << 2* 4)|
											 (3UL << 2* 5)|
										   (3UL << 2* 7)|
										   (3UL << 2* 8)|
										   (3UL << 2* 15));
												
	GPIOB->OSPEEDR  |=  ((2UL << 2* 4)|
											 (2UL << 2* 5)|
										   (2UL << 2* 7)|
										   (2UL << 2* 8)|
										   (2UL << 2* 15));
											 
	GPIOB->PUPDR    &= ~((3UL << 2* 4)|
											 (3UL << 2* 5)|
										   (3UL << 2* 7)|
										   (3UL << 2* 8)|
										   (3UL << 2* 15));
	
	GPIOB->PUPDR    |=  ((1UL << 2* 4)|
											 (1UL << 2* 5)|
										   (1UL << 2* 7)|
										   (1UL << 2* 8)|
										   (1UL << 2* 15));
}

//Initialise J7 Pins
void J7_Init(void){
	RCC->AHB1ENR  |= ((1UL <<  4) ); /*Turns on clock for GPIO B*/
	
	GPIOE->MODER  &= ~((3UL << 2* 3)|
										 (3UL << 2* 4)|
										 (3UL << 2* 5)|
										 (3UL << 2* 6)|
										 (3UL << 2* 7));
	
	GPIOE->MODER  |=  ((1UL << 2* 3)|
										 (1UL << 2* 4)|
										 (1UL << 2* 5)|
										 (1UL << 2* 6)|
										 (1UL << 2* 7));
										
	GPIOE->OTYPER &= ~((1UL <<    3)|
										 (1UL <<    4)|
										 (1UL <<    5)|
										 (1UL <<    6)|
										 (1UL <<    7));
	
	GPIOE->OSPEEDR  &= ~((3UL << 2* 3)|
											 (3UL << 2* 4)|
										   (3UL << 2* 5)|
										   (3UL << 2* 6)|
										   (3UL << 2* 7));
												
	GPIOE->OSPEEDR  |=  ((2UL << 2* 3)|
											 (2UL << 2* 4)|
										   (2UL << 2* 5)|
										   (2UL << 2* 6)|
										   (2UL << 2* 7));
											 
	GPIOE->PUPDR    &= ~((3UL << 2* 3)|
											 (3UL << 2* 4)|
										   (3UL << 2* 5)|
										   (3UL << 2* 5)|
										   (3UL << 2* 7));
	
	GPIOE->PUPDR    |=  ((1UL << 2* 3)|
											 (1UL << 2* 4)|
										   (1UL << 2* 5)|
										   (1UL << 2* 5)|
										   (1UL << 2* 7));
}


//J6 Initialisation
//Sets up PC6 and PC13
void J6_Init(void){
		RCC->AHB1ENR  |= ((1UL <<  2)); /*Turns on clock for GPIO C*/
		GPIOC->MODER  &= ~((3UL << 2* 6)|(3UL << 2*13));
		GPIOC->MODER  |=  ((1UL << 2* 6)|(1UL << 2*13));
		GPIOC->OTYPER &= ~((1UL <<    6)|(1UL << 13));
		GPIOC->OSPEEDR  &= ~((3UL << 2* 6)|(3UL << 2*13));
		GPIOC->OSPEEDR  |=  ((2UL << 2* 6)|(2UL << 2*13));
		GPIOC->PUPDR    &= ~((3UL << 2* 6)|(3UL << 2*13));
		GPIOC->PUPDR    |=  ((1UL << 2* 6)|(1UL << 2*13));
}

//ADC 2 Initialisation
//SEL = 0 -> ADC
//SEL = 1 -> Output Pin
void ADC1_init(int sel) {
	if(sel == 0){
		RCC->APB2ENR  |= ((1UL <<  8) );         /* Enable ADC1 clock                */
		RCC->AHB1ENR  |= ((1UL <<  2) );         /* Enable GPIOC clock                */
		GPIOC->MODER = 0xffffffff;
		GPIOC->PUPDR = 0;
		ADC1->CR1 = 0x00;
		ADC1->CR1 |= (1UL << 11);
		ADC1->CR2 = 0x00;
		ADC1->CR2 |= (1UL << 10) ;					/* right alignement of 12 bits */
		ADC->CCR = 0x00;
		ADC1->SQR1 = 0x01;								/* 1 conversion at a time */
		ADC1->SMPR1 = 0x00;
		ADC1->SMPR1 = 0x0300;
		ADC1->SQR1 = 0x01;
		ADC1->SQR3 = 0x0e;								/* ADC_IN14 = 0x0e: ADC_IN15 = 0x0f */
		ADC1->CR2 |= (1UL << 0);
	}
	else{
		RCC->AHB1ENR  |= ((1UL <<  2) ); /*Turns on clock for GPIO B*/
		GPIOC->MODER  &= ~(3UL << 2* 4);
		GPIOC->MODER  |=  (1UL << 2* 4);
		GPIOC->OTYPER &= ~(1UL <<    4);
		GPIOC->OSPEEDR  &= ~(3UL << 2* 4);
		GPIOC->OSPEEDR  |=  (2UL << 2* 4);
		GPIOC->PUPDR    &= ~(3UL << 2* 4);
		GPIOC->PUPDR    |=  (1UL << 2* 4);
	}
}

//ADC 2 Initialisation
//SEL = 0 -> ADC
//SEL = 1 -> Output Pin
void ADC2_init(int sel) {
	if(sel == 0){
		RCC->APB2ENR  |= ((1UL <<  9) );         /* Enable ADC1 clock                */
		RCC->AHB1ENR  |= ((1UL <<  2) );         /* Enable GPIOC clock                */
		GPIOC->MODER = 0xffffffff;
		GPIOC->PUPDR = 0;
		ADC2->CR1 = 0x00;
		ADC2->CR1 |= (1UL << 11);
		ADC2->CR2 = 0x00;
		ADC2->CR2 |= (1UL << 10) ;					/* right alignement of 12 bits */
		ADC->CCR = 0x00;
		ADC2->SQR1 = 0x01;								/* 1 conversion at a time */
		ADC2->SMPR1 = 0x00;
		ADC2->SMPR1 = 0x0300;
		ADC2->SQR1 = 0x01;
		ADC2->SQR3 = 0x0f;								/* ADC_IN14 = 0x0e: ADC_IN15 = 0x0f */
		ADC2->CR2 |= (1UL << 0);
	}
	else{
		RCC->AHB1ENR  |= ((1UL <<  2) ); /*Turns on clock for GPIO B*/
		GPIOC->MODER  &= ~(3UL << 2* 5);
		GPIOC->MODER  |=  (1UL << 2* 5);
		GPIOC->OTYPER &= ~(1UL <<    5);
		GPIOC->OSPEEDR  &= ~(3UL << 2* 5);
		GPIOC->OSPEEDR  |=  (2UL << 2* 5);
		GPIOC->PUPDR    &= ~(3UL << 2* 5);
		GPIOC->PUPDR    |=  (1UL << 2* 5);
	}
	
}


/* function to read ADC and retun value */
unsigned int read_ADC1 (void) {
	
	ADC1->CR2 |= (1UL << 30)	;		/* set SWSTART to 1 to start conversion */
	while(!(ADC1->SR & ADC_SR_EOC));
	return (ADC1->DR);
}

/* function to read ADC and retun value */
unsigned int read_ADC2 (void) {
	
	ADC2->CR2 |= (1UL << 30)	;		/* set SWSTART to 1 to start conversion */
	while(!(ADC2->SR & ADC_SR_EOC));
	return (ADC2->DR);
}
