#include "my_include.h"

#define SYSCLK	72000000L

static uint64_t Sys_ms=0;

void SYS_Tick_Configuration(void)
{
	SysTick_Config(SYSCLK / 1000);		// 1ms / (1/SYSCLK) = SYSCLK / 1000
}



void delay_ms(uint16_t ms)
{
	uint64_t Out_ms = Sys_ms + ms;
	while(Sys_ms < Out_ms)
		;
}

uint64_t Get_Sys_us(void)
{
	return (Sys_ms * 1000 + (SYSCLK / 1000 - 1 - SysTick->VAL) / 72 );
}

void delay_us(uint32_t us)
{
	uint64_t Out_us = Get_Sys_us() + us;
	while(Get_Sys_us() < Out_us)
		;
}

void Sys_ms_add(void)
{
	Sys_ms++;
}







