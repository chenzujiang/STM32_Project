#include "my_include.h"

#define SYSCLK 72000000L //频率

static uint64_t Sys_ms = 0;

void SYS_Tick_Configuration(void)
{
   SysTick_Config(SYSCLK/1000);// 1ms/(1/SYSCLK = SYSCLK/1000)

}

void delay_ms(uint16_t ms)
{
  uint64_t Out_ms = Sys_ms + ms;//现在的毫秒值+延时的毫秒值
	while(Sys_ms < Out_ms)
	  ;
}

uint16_t Get_Sys_us(void)
{
	return (Sys_ms * 1000 + (Sys_ms/1000 - 1 - SysTick->VAL)/72 );
}

void delay_us(uint32_t us)
{
  uint32_t Out_us = Get_Sys_us() + us;
	while(Get_Sys_us() < Out_us)
		;
}


void Sys_ms_add(void) //用此函数取代原来的 extern uint64_t Sys_ms = 0;转以去用函数去调用
{
  Sys_ms++;
}


