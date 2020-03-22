#include "my_include.h"

void SPI_Flash_Init(void)
{
  SPI_Configuration();

}

uint8_t Get_Device_ID(void)
{
	uint8_t id;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Device_ID);
	
	SPI1_send_recv_byte(DUMMY);
	SPI1_send_recv_byte(DUMMY);
	SPI1_send_recv_byte(DUMMY); //三字节无用数据
	Set_CS_Pin(1);
	
	return id;
}

uint8_t Get_Manufacturer_ID(void)
{
	uint8_t id;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Device_ID);
	
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(DUMMY); //三字节无用数据
	Set_CS_Pin(1);
	
	return id;
}

uint8_t Get_JEDEC_ID(void)
{
	uint32_t id;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(JEDEC_ID);
	
	SPI1_send_recv_byte(DUMMY); //三字节无用数据
	Set_CS_Pin(1);
	
	return id;
}

void Flash_Write_Enable(void)
{
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Write_Enable);	
	Set_CS_Pin(1);
}

void Flash_Write_Disable(void)
{
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Write_Disable);	
	Set_CS_Pin(1);
}

uint8_t Flash_Read_Status_Register_1(void)
{
	uint8_t dat;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Read_Status_Register_1);	
	dat = SPI1_send_recv_byte(DUMMY);
	Set_CS_Pin(1);
	return dat;
}

//uint8_t Flash_Read_Status_Register_1(void)
//{
//	uint8_t dat;
//  Set_CS_Pin(0);
//	SPI1_send_recv_byte(Read_Status_Register_1);	
//	dat = SPI1_send_recv_byte(DUMMY);
//	Set_CS_Pin(1);
//	return dat;
//}

uint8_t Flash_Write_Status_Reg(uint8_t reg1,uint8_t reg2)
{
	uint8_t dat;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Write_Status_Register);	
	SPI1_send_recv_byte(reg1);
	SPI1_send_recv_byte(reg2);
	Set_CS_Pin(1);
	return dat;
}

uint8_t Flash_Read_byte(uint8_t reg1,uint8_t reg2)
{
	uint8_t dat;
  Set_CS_Pin(0);
	SPI1_send_recv_byte(Write_Status_Register);	
	SPI1_send_recv_byte(reg1);
	SPI1_send_recv_byte(reg2);
	Set_CS_Pin(1);
	return dat;
}