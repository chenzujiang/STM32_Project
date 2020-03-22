#include "my_include.h"



void SPI_Flash_Init(void)
{
	SPI_Configuration();
	Set_CS_Pin(1);
}


uint8_t Get_Device_ID(void)
{
	uint8_t id;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Device_ID);
	SPI1_send_recv_byte(DUMMY);
	SPI1_send_recv_byte(DUMMY);
	SPI1_send_recv_byte(DUMMY);
	id = SPI1_send_recv_byte(DUMMY);
	Set_CS_Pin(1);
	return id;
}

uint16_t Get_Manufacturer_ID(void)
{
	uint16_t id;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Manufacturer_ID);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	id = SPI1_send_recv_byte(DUMMY);
	id = (id << 8) | SPI1_send_recv_byte(DUMMY);
	Set_CS_Pin(1);
	return id;
}

void Get_Unique_ID(uint8_t *dat)
{
	uint8_t i;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Read_Unique_ID);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	SPI1_send_recv_byte(0x00);
	for(i=0;i<8;i++)
	{
		*(dat++) = SPI1_send_recv_byte(DUMMY);
	}
	Set_CS_Pin(1);
}

uint32_t Get_JEDEC_ID(void)
{
	uint32_t id;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(JEDEC_ID);
	id = SPI1_send_recv_byte(DUMMY);
	id = (id << 8) | SPI1_send_recv_byte(DUMMY);
	id = (id << 8) | SPI1_send_recv_byte(DUMMY);
	id = (id << 8) | SPI1_send_recv_byte(DUMMY);
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

uint8_t Flash_Read_Status_Reg1(void)
{
	uint8_t dat;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Read_Status_Reg1);
	dat = SPI1_send_recv_byte(DUMMY);
	Set_CS_Pin(1);
	return dat;
}

uint8_t Flash_Read_Status_Reg2(void)
{
	uint8_t dat;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Read_Status_Reg2);
	dat = SPI1_send_recv_byte(DUMMY);
	Set_CS_Pin(1);
	return dat;
}

void Flash_Write_Status_Reg(uint8_t reg1,uint8_t reg2)
{
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Write_Status_Reg);
	SPI1_send_recv_byte(reg1);
	SPI1_send_recv_byte(reg2);
	Set_CS_Pin(1);
}

void Flash_Read_bytes(uint32_t addr,uint8_t *buf,uint16_t len)
{
	uint16_t i;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Read);
	SPI1_send_recv_byte(addr >> 16);
	SPI1_send_recv_byte(addr >> 8);
	SPI1_send_recv_byte(addr);
	for(i=0;i<len;i++)
	{
		*(buf++) = SPI1_send_recv_byte(DUMMY);
	}
	Set_CS_Pin(1);
}

void Flash_Fast_Read_bytes(uint32_t addr,uint8_t *buf,uint16_t len)
{
	uint16_t i;
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Fast_Read);
	SPI1_send_recv_byte(addr >> 16);
	SPI1_send_recv_byte(addr >> 8);
	SPI1_send_recv_byte(addr);
	SPI1_send_recv_byte(DUMMY);
	for(i=0;i<len;i++)
	{
		*(buf++) = SPI1_send_recv_byte(DUMMY);
	}
	Set_CS_Pin(1);
}

void Flash_Wait_Write_End(void)
{
	while(0x01 & Flash_Read_Status_Reg1())
		;
}

void Flash_Page_Program(uint32_t addr,uint8_t *buf,uint16_t len)
{
	uint16_t i;
	Flash_Wait_Write_End();
	Flash_Write_Enable();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Page_Program);
	SPI1_send_recv_byte(addr >> 16);
	SPI1_send_recv_byte(addr >> 8);
	SPI1_send_recv_byte(addr);
	for(i=0;i<len;i++)
	{
		 SPI1_send_recv_byte(*(buf++));
	}
	Set_CS_Pin(1);
	Flash_Wait_Write_End();
	Flash_Write_Disable();
}

void Flash_Sector_Erase_4KB(uint32_t addr)
{
	Flash_Wait_Write_End();
	Flash_Write_Enable();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Sector_Erase_4KB);
	SPI1_send_recv_byte(addr >> 16);
	SPI1_send_recv_byte(addr >> 8);
	SPI1_send_recv_byte(addr);
	Set_CS_Pin(1);
	Flash_Wait_Write_End();
	Flash_Write_Disable();
}

void Flash_Block_Erase_32(uint32_t addr)
{
	Flash_Wait_Write_End();
	Flash_Write_Enable();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Block_Erase_32KB);
	SPI1_send_recv_byte(addr>>16);
	SPI1_send_recv_byte(addr>>8);
	SPI1_send_recv_byte(addr);
	Set_CS_Pin(1);
	Flash_Wait_Write_End();
	Flash_Write_Disable();
}

void Flash_Block_Erase_64(uint32_t addr)
{
	Flash_Wait_Write_End();
	Flash_Write_Enable();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Block_Erase_64KB);
	SPI1_send_recv_byte(addr>>16);
	SPI1_send_recv_byte(addr>>8);
	SPI1_send_recv_byte(addr);
	Set_CS_Pin(1);
	Flash_Wait_Write_End();
	Flash_Write_Disable();
}

void Flash_Chip_Erase(void)
{
	Flash_Wait_Write_End();
	Flash_Write_Enable();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Chip_Erase);
	Set_CS_Pin(1);
	Flash_Wait_Write_End();
	Flash_Write_Disable();
}

void Flash_Power_Down(void)
{
	Set_CS_Pin(0);
	Flash_Wait_Write_End();
	SPI1_send_recv_byte(Power_Down);
	Set_CS_Pin(1);
}

void Flash_Release_Power_Down(void)
{
	Flash_Wait_Write_End();
	Set_CS_Pin(0);
	SPI1_send_recv_byte(Release_Power_Down);
	Set_CS_Pin(1);
}


void Flash_Write_bytes(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint16_t page=0;
	uint32_t page_addr = addr,sended_len=0,sending_len=0;
	do
	{
		sending_len = ((len-sended_len)<(256-page_addr%256))?
		(len-sended_len):(256-page_addr%256);
		Flash_Page_Program(page_addr,buf+sended_len,sending_len);
		page_addr += sending_len;
		sended_len += sending_len;
	}while(sended_len < len);
}













