#ifndef __Breadboard_H
#define __Breadboard_H

extern void Delay (uint32_t dlyTicks);
extern void J5_Init(void);
extern void J7_Init(void);
extern void J6_Init(void);
extern void ADC1_init(int sel);
extern void ADC2_init(int sel);
extern unsigned int read_ADC1(void);
extern unsigned int read_ADC2(void);


#endif
