#include "my_include.h"


#define ADC_DMA_LEN  10  //DMA���
#define ADC_SELECT    60

//#if ADC_DMA_LEN < ADC_SELECT//�������

//#error error��ADC_DMA_LEN ������� ADC_SELECT
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

static void ADC_NVIC_Configuration(void)  //��������������������������δ���
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//ʱ��
  

	DMA_DeInit(DMA1_Channel1);     //DMA��h
  
	DMA_Init_Structure.DMA_BufferSize = 10;
	DMA_Init_Structure.DMA_DIR = DMA_DIR_PeripheralSRC;//������ΪԴ
	DMA_Init_Structure.DMA_M2M = DMA_M2M_Disable;//DMA_memory_to_memory ����ΪDMA_M2M_Disable
  DMA_Init_Structure.DMA_MemoryBaseAddr = (uint32_t)ADC_buf;//ָ�붼Ϊ32λ��Ĭ��Ϊunsigned int ��ϵͳȥ�Ƚϵ��Ǵ�ָ���ǲ���uint32_t
  DMA_Init_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//����16λ��uint16_t ADC_buf[10];���涨�崫��16λ
  DMA_Init_Structure.DMA_MemoryInc =  DMA_MemoryInc_Enable;//���Լ�
	DMA_Init_Structure.DMA_Mode = DMA_Mode_Circular; //ѭ��ģʽ����������M to M��
  DMA_Init_Structure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�������ַ
  DMA_Init_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//������ ����
  DMA_Init_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ
  DMA_Init_Structure.DMA_Priority = DMA_Priority_High;//��һ���˴����ȼ�ѡ�ߣ����ѡ��

  DMA_Init(DMA1_Channel1,&DMA_Init_Structure);//���������ý��Ĵ���
	DMA_Cmd(DMA1_Channel1,ENABLE);//DMA ����ʹ��
  DMA_ITConfig(DMA1_Channel1,DMA1_IT_TC1,ENABLE);//�жϣ�ͨ����������ɣ�ʹ�ܣ�
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
	
	ADC_ResetCalibration(ADC1);//��λ����У�� 
	while(SET == ADC_GetResetCalibrationStatus(ADC1));// �ȴ�����У�����
	
	ADC_StartCalibration(ADC1);
	while(RESET == ADC_GetCalibrationStatus(ADC1));
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_41Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_DMA_Configuration();
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/************�˲�����**************/
void ADC_Result_Handle(void)
{
  uint16_t i,j,val;
	for(i=0; i<9; i++)//����
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
	 ADC_buf[ADC_DMA_LEN] = ADC_buf[ADC_DMA_LEN - ADC_SELECT];//ȥ��2�����2����С//slect ==8
	for(i = 0;i<ADC_SELECT - 1;i++)//-2
	{
	  ADC_buf[ADC_DMA_LEN] = (ADC_buf[ADC_DMA_LEN] + ADC_buf[i+1+(ADC_DMA_LEN - ADC_SELECT)]) / 2;
	  //��ÿ�ε�ֵȻ��ȡ��ֵ�����þ�ֵ�Ӻ�һ�������پ�ֵ����
	}
}
/*******************************/

uint16_t Get_ADC_Result(void)
{
//	while(RESET == ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
//	return ADC_GetConversionValue(ADC1);
 return ADC_buf[ADC_DMA_LEN];//�ж�ʱ�̸�����buf�����ֵ��ӦΪ�ж�ʱ���ڵ����˲�����
}

