#ifndef __DELAY_H
#define __DELAY_H
#include "my_include.h"


void SYS_Tick_Configuration(void);
void delay_ms(uint16_t ms);
uint64_t Get_Sys_us(void);
void delay_us(uint32_t us);
void Sys_ms_add(void);
void delay_tm(uint16_t tm);

#endif
