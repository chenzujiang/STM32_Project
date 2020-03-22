#ifndef	__SPI_FLASH_H
#define __SPI_FLASH_H
#include "stm32f10x.h"   

#define Set_CS_Pin(status) 	GPIO_WriteBit(GPIOA,GPIO_Pin_4,status)

#define Write_Enable			0x06
#define Write_Disable			0x04
#define Read_Status_Reg1		0x05
#define Read_Status_Reg2		0x35
#define	Write_Status_Reg		0x01
#define Page_Program			0x02
#define Read					0x03
#define Fast_Read				0x0b
#define Quad_Page_Program		0x32
#define	Block_Erase_64KB		0xD8
#define	Block_Erase_32KB		0x52
#define	Sector_Erase_4KB		0x20
#define	Chip_Erase				0xc7
#define	Erase_Suspend			0x75
#define	Erase_Resume			0x7A
#define Power_Down				0xB9
#define	High_Performance_Mode	0xa3
#define	Mode_Bit_Reset			0xff
#define	Release_Power_Down		0xAB
#define Device_ID				0xAB
#define Manufacturer_ID			0x90
#define Read_Unique_ID			0x4B
#define	JEDEC_ID				0x9F

#define DUMMY					0xff


void SPI_Flash_Init(void);
uint8_t Get_Device_ID(void);
uint16_t Get_Manufacturer_ID(void);
void Get_Unique_ID(uint8_t *dat);
uint32_t Get_JEDEC_ID(void);
void Flash_Read_bytes(uint32_t addr,uint8_t *buf,uint16_t len);
void Flash_Fast_Read_bytes(uint32_t addr,uint8_t *buf,uint16_t len);
void Flash_Page_Program(uint32_t addr,uint8_t *buf,uint16_t len);
void Flash_Sector_Erase_4KB(uint32_t addr);
void Flash_Block_Erase_32(uint32_t addr);
void Flash_Block_Erase_64(uint32_t addr);
void Flash_Chip_Erase(void);
void Flash_Power_Down(void);
void Flash_Release_Power_Down(void);
void Flash_Write_bytes(uint32_t addr,uint8_t *buf,uint32_t len);

#endif
