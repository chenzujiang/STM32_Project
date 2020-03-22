#include "my_include.h"


#define ADC_DMA_LEN  10  //DMA宽度
#define ADC_SELECT    60

//#if ADC_DMA_LEN < ADC_SELECT//宏编译检错

//#error error：ADC_DMA_LEN 必须大于 ADC_SELECT
//#endif

uint16_t ADC_buf[ADC_DMA_LEN + 1];

static void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void ADC_NVIC_Configuration(void)  //、、、、、、、、、、、、、未完成
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
uint16_t ADC_buf[11];//10

static void ADC_DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_Init_Structure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//时钟
  

	DMA_DeInit(DMA1_Channel1);     //DMA。h
  
	DMA_Init_Structure.DMA_BufferSize = 10;
	DMA_Init_Structure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为源
	DMA_Init_Structure.DMA_M2M = DMA_M2M_Disable;//DMA_memory_to_memory 设置为DMA_M2M_Disable
  DMA_Init_Structure.DMA_MemoryBaseAddr = (uint32_t)ADC_buf;//指针都为32位但默认为unsigned int 而系统去比较的是此指针是不是uint32_t
  DMA_Init_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//半字16位，uint16_t ADC_buf[10];上面定义传输16位
  DMA_Init_Structure.DMA_MemoryInc =  DMA_MemoryInc_Enable;//有自加
	DMA_Init_Structure.DMA_Mode = DMA_Mode_Circular; //循环模式（不适用与M to M）
  DMA_Init_Structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//外设基地址
  DMA_Init_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设宽度 半字
  DMA_Init_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址
  DMA_Init_Structure.DMA_Priority = DMA_Priority_High;//就一个此处优先级选高（随便选）

  DMA_Init(DMA1_Channel1,&DMA_Init_Structure);//将上述配置进寄存器
	DMA_Cmd(DMA1_Channel1,ENABLE);//DMA 命令使能
  DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1,ENABLE);//中断（通道，传输完成，使能）
}

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_Init_Structure;
	
	ADC_GPIO_Configuration();
	
	ADC_DeInit(ADC1);
	
	ADC_Init_Structure.ADC_ContinuousConvMode = ENABLE;
	ADC_Init_Structure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init_Structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init_Structure.ADC_Mode = ADC_Mode_Independent;
	ADC_Init_Structure.ADC_NbrOfChannel = ADC_Channel_10;
	ADC_Init_Structure.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1,&ADC_Init_Structure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//复位自我校验 
	while(SET == ADC_GetResetCalibrationStatus(ADC1));// 等待自我校验完成
	
	ADC_StartCalibration(ADC1);
	while(RESET == ADC_GetCalibrationStatus(ADC1));
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_41Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_DMA_Configuration();
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/************滤波函数**************/
void ADC_Result_Handle(void)
{
  uint16_t i,j,val;
	for(i=0; i<9; i++)//排序
	{
 	  for(j=i+1;j<10;j++)
		{
		  if(ADC_buf[i] > ADC_buf[j])
			{
			  val = ADC_buf[i];
				ADC_buf[i] = ADC_buf[j];
				ADC_buf[j] = val;
			}
		}
	}
	 ADC_buf[ADC_DMA_LEN] = ADC_buf[ADC_DMA_LEN - ADC_SELECT];//去掉2个最大，2个最小//slect ==8
	for(i = 0;i<ADC_SELECT - 1;i++)//-2
	{
	  ADC_buf[ADC_DMA_LEN] = (ADC_buf[ADC_DMA_LEN] + ADC_buf[i+1+(ADC_DMA_LEN - ADC_SELECT)]) / 2;
	  //加每次的值然后取均值，再用均值加后一个数，再均值。。
	}
}
/*******************************/

uint16_t Get_ADC_Result(void)
{
//	while(RESET == ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
//	return ADC_GetConversionValue(ADC1);
 return ADC_buf[ADC_DMA_LEN];//中断时刻更新着buf里面的值，应为中断时刻在调用滤波函数
}

