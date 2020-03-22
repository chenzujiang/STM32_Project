#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define Set_CS_Pin(status) GPIO_WriteBit(GPIOA,GPIO_Pin_4,status)

#define Write_Enable             0x06
#define Write_Disable            0x04
#define Read_Status_Register_1   0x05
#define Read_Status_Register_2   0x35
#define Write_Status_Register    0x01
#define Page_Program             0x02
#define Read                     0x03
#define Fast_Read                0x0b
#define Quad_Page_Program        0x32
#define Block_Erase_64KB         0xd8
#define Block_Erase_32KB         0x52
#define Sector_Erase_4KB         0x20
#define Chip_Erase               0xc7
#define Erase_Suspend            0x75
#define Erase_Resume             0x7a
#define Power_down               0xb9
#define High_Performace_Mode     0xa3
#define Mode_Bit_Reaset          0xff
#define Release_Power_down       0xab
#define Device_ID                0x90
#define Read_Unique_ID           0x4b
#define JEDEC_ID                 0x9f

#define DUMMY                    0xff

void SPI_Configuration();

uint8_t Get_Device_ID(void);
uint8_t Get_Manufacturer_ID(void);

#endif