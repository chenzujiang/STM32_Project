#include "my_include.h"



void SPI_GPIO_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |  RCC_APB2Periph_AFIO, ENABLE);

  /* Configure PB0 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void SPI_Configuration()
{
  SPI_InitTypeDef SPI_InitStructure;
	
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 0x55; //自定义校验值
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //两线全双工
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //主设备
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  //片选，用软件片选

  SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);
}

uint8_t SPI1_send_recv_byte(uint8_t byte)  //写一字节，读一字节基本函数
{
  SPI_I2S_SendData(SPI1,byte); //要发送的值写到SPI寄存器中
	while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE))//选用接收非空为标志位
     ;
	return SPI_I2S_ReceiveData(SPI1);
}

