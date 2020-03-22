/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32f10x_it.h"

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

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
static void delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for(i=ms; i>0; i--)
		for(j=10000; j>0; j--)
			;
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

uint64_t SysTm = 0;
uint8_t Dir_flag = 0;
uint16_t PWM_val = 0;
uint16_t PWM_vals[100] = { 0,1,2,3,4,5,6,7,8,9,
	                         10,12,14,17,19,23,27,32,37,41,
	                         47,54,60,67,74,91,103,111,123,133,
	                         150,175,200,230,265,300,350,420,460,520,
	                         570,640,730,780,800,850,900,950,970,1000
                          };//呼吸灯PWM数组，列非线性数组以便使呼吸灯看起来效果更好

void TIM1_IRQHandler()
{
  if(SET == TIM_GetITStatus(TIM1,TIM_IT_COM))   //timer.h 1145
	{
	   SysTm++;
//		if(!(SysTm%1000))
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)! GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0));
	  
		/*PWM实现呼吸灯*/
		if(!(SysTm%40))
		{
		  if(!Dir_flag)			
			{
			   PWM_val ++;
				if(PWM_val == 49)
				{ 
				   Dir_flag = 1;				 
				}
			}
		 else			
			{
			   PWM_val --;
				if(PWM_val == 0)
				{ 
				   Dir_flag = 0;				 
				}
			}
		  TIM_SetCompare3(TIM1, PWM_vals[PWM_val]);//#
		}
		TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
	}
}

void EXTI0_IRQHandler(void)
{
	if(SET == EXTI_GetITStatus(EXTI_Line0))
	{
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			delay_ms(5);
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
			{
				GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0));
				while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
					;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(SET == EXTI_GetITStatus(EXTI_Line13))
	{
		if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
		{
			delay_ms(5);
			if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_3,(BitAction)!GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13));
				while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
					;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
