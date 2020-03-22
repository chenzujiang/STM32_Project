#ifndef	__ADC_H
#define __ADC_H

#include "stm32f10x.h"





void ADC_Configuration(void);
uint16_t Get_ADC_Result(void);
void ADC_Result_Handle(void);
#endif
