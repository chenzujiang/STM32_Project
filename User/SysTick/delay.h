#ifndef __DELAY_H
#define __DELAY_H//这两句话防止在其他文件中有重复定义宏定义
#include "my_include.h"

void SYS_Tick_Configuration(void);

void delay_ms(uint16_t ms);
void delay_us(uint32_t us);

#endif
