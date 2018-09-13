#include <stdio.h>
#include "STM32F4xx.h"
#include "LED.h"
#include "SWT.h"
#include "serial.h"
#include "Breadboard.h"
#include "Voltage.h"
#include "lcd_buffer.h"
#include "lcd_driver.h"
#include "Buttons.h"
#include "Resistance.h"
#include "Light.h"
#include <math.h>
#include "Current.h"
#include "Transistor.h"
#include "Capacitance.h"
#include "Frequency.h"
#include "Logic.h"

#define BUFFER_SIZE 128
#define CLEAR_COUNTER(VAL) GPIOC->BSRR = (1UL << (VAL));
#define FREQ_EN(VAL) GPIOE->BSRR = (1UL << (VAL));

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {                                              
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/

int main (void) {

 uint32_t btns = 0;
 int LED_switch[8] = {0};
 int menu = 0;
 int sel_V = 0;
 int sel_T1 = 0;
 int sel_T2 = 0;
 int sel_R = 0;
 int sel_R_OLD = 0;
 int sel_I = 0;
 int freq_cycles = 0;
	
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
  
  LED_Init();
  BTN_Init();   
  SWT_Init();
	J6_Init();
	J5_Init();
	J7_Init();
	ADC1_init(0);
	ADC2_init(1);
	serial_init();

	lcd_init(LCD_LINES_TWO, LCD_CURSOR_OFF, LCD_CBLINK_OFF, BUFFER_SIZE);
	
	//GPIOD->ODR = 0;																/* turn LEDs off */
	//GPIOB->ODR = ((1UL<<4)|(1UL<<5)|(1UL<<7)|(1UL<<8)|(1UL<<15));
	//GPIOE->ODR = ((1UL<<3)|(1UL<<4)|(1UL<<5)|(1UL<<6)|(1UL<<7));
	lcd_clear_display();
	lcd_write_string("SCREEN TEST",0,0);
	Delay(10);
	lcd_clear_display();
	lcd_write_string("Select Function",0,0);
	GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5+16)) | (1UL << (6+16)); //Init_MUX
  while(1) {                                    
    btns = SWT_Get() & 0xFF00;                           
 
if(USART2->SR & USART_SR_RXNE){
		if(USART2->DR != 0){
			GPIOE->BSRR = (1UL << (3+16));
			menu = USART2->DR;
		}
		switch(menu){
			
			//// Voltage & AC Voltage Meter ////
			case '0' : //Voltage Auto-ranging
			case '1' : //Voltage 10V
			case '2' : //Voltage 5V
			case '3' : //Voltage 2V
			case '4' : //Voltage 1V
			case '5' : //AC Voltage Auto-ranging
			case 'u' : //Voltage Data Log
			case 'v' : //AC Voltage Auto-ranging
					lcd_clear_display();
					//General Mux control
					if(menu == '5' || menu == 'v'){
						GPIOE->BSRR = (1UL << (4)) | (1UL << (5)) | (1UL << (6+16));
					}else{
						GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5+16)) | (1UL << (6+16));
					}
					while(menu >= 48 || menu <= 53 || menu == 'u' || menu == 'v'){
					//while((menu == '0')||(menu == '1')||(menu == '2')||(menu == '3')||(menu == '4')||(menu == '5')){ 
						sel_V = Measure_Voltage(menu, sel_V);
						if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
						}
					}
			break;
			
			//// Resistance Meter ////
			case '6' : //Resistance Auto-ranging
			case '7' ://1M
			case '8' ://100K
			case '9' ://10K
			case 'a' ://100R
			case 'x' ://Resistance DataLog
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5)) | (1UL << (6+16));
				while( menu == 'a'||menu == 'x'|| menu == '6'|| menu == '7'|| menu == '8'|| menu == '9'){			//menu >= 54 || menu <= 57 ||		
					sel_V = Measure_Resistance_Continuity(sel_V, menu, &sel_R, &sel_R_OLD);
					if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
						}
				}
			break;
				
			//// Current Meter ////
			case 'b' : //Current Auto-Ranging
			case 'c' : //Current 1A
			case 'd' : //Current 250mA
			case 'e' : //Current 150mA
			case 'f' : //Current 45mA
			case 'w' : //Current DataLog
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4)) | (1UL << (5+16)) | (1UL << (6+16));
				while(menu >= 98 || menu <= 102 || menu == 'w'){
					sel_V = Measure_Current(menu, sel_V, &sel_I);
					if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
						}
				}
				
			//// Light Meter ////
			case 'g' : //White
			case 'h' : //Red
			case 'i' : //Green
		  case 'j' : //Blue
			case 'k' : //cycle
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5+16)) | (1UL << (6));
				//while(menu >= 103 || menu <= 107){
				while(menu == 'g' || menu == 'h' || menu == 'i' || menu == 'j' || menu == 'k'){
					sel_V = Measure_Light(menu, sel_V, &sel_R);
					if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
						}
				}
			
			//// Continuity Test ////
			case 'l' :
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5)) | (1UL << (6+16));
				while(menu == 'l'){
					sel_V = Measure_Resistance_Continuity(sel_V, menu, &sel_R, &sel_R_OLD);
					if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
					}
				}
			
			//// Transistor Tester ////
			case 'm' :
			case 'n' :
			case 'o' :
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4)) | (1UL << (5+16)) | (1UL << (6));
				sel_T1 = 0;
				sel_T2 = 0;
				while(menu == 'm' || menu == 'n' || menu == 'o'){
					Test_Transistor(menu, &sel_T1, &sel_T2);
					if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
					}
				}
			break;
			
			//// Capacitance Measurement ////
			case 'p':
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5)) | (1UL << (6));
				while(menu == 'p'){
					measure_capacitance();
				  if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
					}
				}
			break;
				
			//// Frequency Mesaurement ////
			case 'q':
				freq_cycles = 0;
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4)) | (1UL << (5)) | (1UL << (6));
				while(menu == 'q'){
					freq_cycles = measure_frequency(freq_cycles);			
				  if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
					}
				}
			break;
			
			//// Logic Analyzer ////
			case 'r':
			case 's':
			case 't':
				lcd_clear_display();
				GPIOE->BSRR = (1UL << (4+16)) | (1UL << (5+16)) | (1UL << (6+16));
				while(menu == 'r' || menu == 's' || menu == 't'){
					sel_V = test_logic(menu,sel_V);		
				  if(USART2->SR & USART_SR_RXNE){
						 GPIOE->BSRR = (1UL << (3+16));
						 break;
					}
				}
			break;
		}
  }
		//GPIOD->ODR = (LED_switch[0]|LED_switch[1]|LED_switch[2]|LED_switch[3]|LED_switch[4]|LED_switch[5]|LED_switch[6]|LED_switch[7]);
		Delay(200);
	}
}
