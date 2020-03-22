/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "my_include.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure PB0 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}



char const *str = "The Page Program instruction allows from one byte to 256 bytes (a page) of data to be programmed at \
previously erased (FFh) memory locations. A WriteEnable instruction mustbe executed before the \
device will accept the Page Program Instruction (Status Register bit WEL=1). The instruction is initiated \
by driving the /CS pin low then shifting the instruction code “02h” followed by a 24-bit address (A23and at \
least one data byte, into the DI pin. The /CS pin must be held low for the entire length of the \
instruction while data is being sent to the device.The Page Program instruction sequence is shown in \
figure 14. \
If an entire 256 byte page is to be programmed, the last address byte (the 8 least significant address \
bits) should be set to 0. If the last address byteis not zero, and the number of clocks exceed the \
remaining page length, the addressing will wrap to the beginning of the page. In some cases, less than \
256 bytes (a partial page) can be programmed without having any effect on other bytes within the same \
page. One condition to perform a partial page program is that the number of clocks can not exceed the \
remaining page length. If more than 256 bytes are sent to the device the addressing will wrap to the \
beginning of the page and overwrite previously sent data. \
As with the write and erase instructions, the /CS pin must be driven high after the eighth bit of the last \
byte has been latched. If this is not done the Page Program instruction will not be executed. After /CS is \
driven high, the self-timed Page Program instruction will commence for a time duration of tpp (See AC \
Characteristics). While the Page Program cycle is in progress, the Read Status Register instruction may \
still be accessed for checking the status of the BUSY bit. The BUSY bit is a 1 during the Page Program \
cycle and becomes a 0 when the cycle is finished and the device is ready to accept other instructions \
again. After the Page Program cycle has finished the Write Enable Latch (WEL) bit in the Status \
Register is cleared to 0. The Page Program instruction will not be executed if the addressed page is \
protected by the Block Protect (BP2, BP1, and BP0) bits. ";

uint8_t show_buf[0x2000],buf2[8*1024];

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    uint32_t test_val;
	GPIO_Configuration();    
//	EXTI_Configuration();
//	NVIC_Configuration();
	SYS_Tick_Configuration();
	USART1_Configuration();
	SPI_Flash_Init();
	Flash_Block_Erase_32(0x00);
//	for(test_val=0; test_val<256; test_val++)
//		show_buf[test_val] = 0x05;
//	for(test_val=0; test_val<256; test_val++)
//		show_buf[256 + test_val] = 0x50;
	Flash_Write_bytes(0x10,(uint8_t *)str,strlen(str));
	Flash_Read_bytes(0x10,buf2,strlen(str));
	for(test_val=0; test_val<strlen(str); test_val++)
		USART1_PutChr(buf2[test_val]);
	while (1)
	{

	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
