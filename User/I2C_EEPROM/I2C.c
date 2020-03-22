#include "my_include.h"

static void IIC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void IIC_Configuration(I2C_TypeDef* I2Cx,uint32_t speed)
{
	I2C_InitTypeDef I2C_InitStructurre;
	
	IIC_GPIO_Configuration();
	I2C_DeInit(I2Cx);
	
	I2C_InitStructurre.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructurre.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructurre.I2C_ClockSpeed = speed;
	I2C_InitStructurre.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructurre.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructurre.I2C_OwnAddress1 = 0x50;
	
	I2C_Init(I2Cx,&I2C_InitStructurre);
	
	I2C_Cmd(I2Cx,ENABLE);
}


int8_t IIC_Send_bytes(I2C_TypeDef* I2Cx,uint8_t Slave_addr,uint8_t Reg_addr,uint8_t *buf,uint16_t len)
{
	int32_t wait_tm = 0x1ffff;
	while(SET == I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY) && wait_tm--);
	if(-1 == wait_tm)
	{
		return -1;			//总线忙
	}
	
	I2C_GenerateSTART(I2Cx,ENABLE);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT) && wait_tm--);
	if(-1 == wait_tm)
	{
		return -2;			//发送起始位失败
	}
	I2C_Send7bitAddress(I2Cx,Slave_addr,I2C_Direction_Transmitter);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && wait_tm--);
	if(-1 == wait_tm)
	{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		return -3;			//发送地址失败
	}
	I2C_SendData(I2Cx,Reg_addr);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) && wait_tm--);
	if(-1 == wait_tm)
	{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		return -4;			//发送数据失败
	}
	while(len--)
	{
		I2C_SendData(I2Cx,*(buf++));
		wait_tm = 0xff;
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) && wait_tm--);
		if(-1 == wait_tm)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return -4;			//发送数据失败
		}
	}
	I2C_GenerateSTOP(I2Cx,ENABLE);
	return 0;
}


int8_t IIC_Recv_bytes(I2C_TypeDef* I2Cx,uint8_t Slave_addr,uint8_t Reg_addr,uint8_t *buf,uint16_t len)
{
	int32_t wait_tm = 0x1ffff;
	while(SET == I2C_GetFlagStatus(I2Cx,I2C_FLAG_BUSY) && wait_tm--);
	if(-1 == wait_tm)
	{
		return -1;			//总线忙
	}
	
	I2C_GenerateSTART(I2Cx,ENABLE);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT) && wait_tm--);
	if(-1 == wait_tm)
	{
		return -2;			//发送起始位失败
	}
	I2C_Send7bitAddress(I2Cx,Slave_addr,I2C_Direction_Transmitter);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && wait_tm--);
	if(-1 == wait_tm)
	{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		return -3;			//发送地址失败
	}
	I2C_SendData(I2Cx,Reg_addr);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED) && wait_tm--);
	if(-1 == wait_tm)
	{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		return -4;			//发送数据失败
	}
	I2C_GenerateSTART(I2Cx,ENABLE);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_MODE_SELECT) && wait_tm--);
	if(-1 == wait_tm)
	{
		return -2;			//发送起始位失败
	}
	I2C_Send7bitAddress(I2Cx,Slave_addr,I2C_Direction_Receiver);
	wait_tm = 0xff;
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && wait_tm--);
	if(-1 == wait_tm)
	{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		return -3;			//发送地址失败
	}
	while(len--)
	{
		if(0 == len)
		{
			I2C_AcknowledgeConfig(I2Cx,DISABLE);
			I2C_GenerateSTOP(I2Cx,ENABLE);
		}
		wait_tm = 0xff;
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED) && wait_tm--);
		if(-1 == wait_tm)
		{
			I2C_GenerateSTOP(I2Cx,ENABLE);
			return -5;			//接收数据错误
		}
		*(buf++) = I2C_ReceiveData(I2Cx);
	}
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	return 0;
}










