#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"


void IIC_Configuration(I2C_TypeDef* I2Cx,uint32_t speed);
int8_t IIC_Send_bytes(I2C_TypeDef* I2Cx,uint8_t Slave_addr,uint8_t Reg_addr,uint8_t *buf,uint16_t len);
int8_t IIC_Recv_bytes(I2C_TypeDef* I2Cx,uint8_t Slave_addr,uint8_t Reg_addr,uint8_t *buf,uint16_t len);



#endif

