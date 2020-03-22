#ifndef	__SPI_H
#define __SPI_H
#include "stm32f10x.h"                  // Device header




void SPI_Configuration(void);
uint8_t SPI1_send_recv_byte(uint8_t byte);


#endif
