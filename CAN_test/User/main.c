 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   CAN通讯实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./can/bsp_can.h"
#include "./key/bsp_key.h"  

CanRxMsg  CAN_Rece_Data;
CanTxMsg  CAN_Tran_Data;

uint8_t flag = 0;
 
 
void Delay(__IO uint32_t nCount); 

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	LED_GPIO_Config();
	LED_BLUE;
	
	/* 配置串口为：115200 8-N-1 */
	USART_Config();
	printf("\r\n 这是一个CAN通讯实验 \r\n");
	
	CAN_Config()	;   
	
	Key_GPIO_Config();
	printf("\r\n 按KEY1按键发送数据\r\n");
	
	while(1)
	{
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			uint8_t box;
			
			CAN_Tran_Data.StdId = 0;//扩展帧发送
			CAN_Tran_Data.ExtId = PASS_ID;//发送的ID号《和原来ID一样，才能接受到
			CAN_Tran_Data.RTR = CAN_RTR_Data;//发送数据帧，
			CAN_Tran_Data.IDE = CAN_Id_Extended ;//使用扩展帧
			CAN_Tran_Data.DLC = 1;//发送一个字节
			CAN_Tran_Data.Data[0] = 10;
			
			box = CAN_Transmit(CAN1,&CAN_Tran_Data);//发送，box返回发送的邮箱号
			
			while(CAN_TransmitStatus(CAN1,box) == CAN_TxStatus_Failed);//等待发送完成
						
			printf("\r\n 数据包发送完成\r\n");			
			
		} 		
		
		
		if(flag == 1)
		{
				printf("\r\n接收到的数据：%d\r\n",CAN_Rece_Data.Data[0]);
			  flag = 0;
		}
		else
		{
		
		}
	}
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
