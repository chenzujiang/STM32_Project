#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "buffer.h"


void USART1_Configuration(void);
void Clear_USART1_Busy(void);
void USART1_PutChr(uint8_t chr);
void Recv_data(uint8_t val);
uint8_t IS_USART1_Rev(void);
uint8_t Get_Recv_dat(void);
void my_printf(uint8_t *str);
Buffer * Get_USART1_buf_handle(void);


#endif
