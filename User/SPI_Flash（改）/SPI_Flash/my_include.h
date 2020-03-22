#ifndef __MY_INCLUDE_H
#define __MY_INCLUDE_H

#include <STDIO.h>
#include <STDLIB.h>
#include <string.h>
#include "stm32f10x.h"
#include "delay.h"
#include "USART.h"
#include "buffer.h"
#include "shell.h"
#include "SPI.h"
#include "SPI_Flash.h"


typedef enum
{
	FALSE = 0,
	TRUE
}My_bool;

#ifdef __DEBUG__

#define Debug(str)	printf(str"File:%s,Lines:%d\r\n",__FILE__,__LINE__)

#else

#define Debug(str)	__NOP();

#endif



#endif
