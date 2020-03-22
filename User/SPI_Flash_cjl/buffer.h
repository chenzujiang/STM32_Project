#ifndef	__BUFFER_H
#define	__BUFFER_H
#include "stm32f10x.h"

typedef	struct buffer
{
	uint8_t *buf;
	uint16_t len;
	uint16_t head;
	uint16_t end;
}Buffer;

void buffer_init(Buffer *buff,uint8_t *buffer,uint16_t len);
void put_data(Buffer *buff, uint8_t dat);
uint8_t get_data(Buffer *buff);
uint8_t is_empty(Buffer *buff);
void clear_buf(Buffer *buff);

#endif

