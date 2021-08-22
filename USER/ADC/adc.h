#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#define TableSize 512




void Adc1_Init(void);
void ADC_IRQHandler(void);
void TIM3_Int_Init(u16 arr,u16 psc);


#endif 





