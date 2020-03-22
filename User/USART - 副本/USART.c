#include "my_include.h"

static My_bool USART1_Busy = FALSE;

static uint8_t USART1_Rev_flag = 0,USART1_Rev_data;

void Recv_data(uint8_t val)
{
   USART1_Rev_flag = 1;
 	 USART1_Rev_data = val;
}

uint8_t  IS_USART1_Rev(void)
{
  return USART1_Rev_flag;
}

uint8_t Get_Recv_dat(void)
{
  USART1_Rev_flag = 0;
	return USART1_Rev_data;
}

static void USART1_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);//开启时钟（GPIOA | AFIO复用口 | USART1）

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //因为USART1复用在GPIOA上
}

static void USART1_NVIC_Configuration(void)//NVIC中断管理器初始化
{
	NVIC_InitTypeDef NVIC_InitStructure;

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	
	NVIC_Init(&NVIC_InitStructure);
}

void USART1_Configuration(void)
{
  USART_InitTypeDef USART1_InitStructure;
	
	USART1_GPIO_Configuration();
	
	USART1_InitStructure.USART_BaudRate = 115200;//波特率设置
  USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStructure.USART_Parity = USART_Parity_No; //校验 = 无
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;//停止发送标志校验
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART1_InitStructure);//将以上配置写进寄存器
	
	USART_Cmd(USART1,ENABLE);
	
	USART_ITConfig(USART1,USART_IT_TC,ENABLE);//使能USART_IT_TC串口中断发送完成
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能USART_IT_RXNE串口中断接收完成

  USART1_NVIC_Configuration();
}

void Clear_USART1_Busy(void)//避免externd 函数
{ 
   USART1_Busy = FALSE;
}

void USART1_PutChr(uint8_t chr)
{
  while(USART1_Busy == TRUE);  //发送要等待，未发送完不能再发送
	USART_SendData(USART1,chr);
	USART1_Busy = TRUE;
}
