#include "adc.h"
#include "delay.h"
u32 ADC_EndFlag;
float Sintable[TableSize]={0};
void Adc1_Init(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);						//ʹ�� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 						//ʹ�� ADC1 ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;										//ѡ��ADC1ͨ��1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;									//���ó�ģ������ģʽ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;								//��ʹ������������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 

	
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;								//ADC��������ģʽ
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;							//ADC1����APB2=84M,���ADCʱ��Ƶ��21MHZ
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;			//ADC1ͨ��������ֱ�ӷ���DMAģʽ
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;		//��������ʱ��֮����ӳ٣�˫�ػ��߶��ز���Ч
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;							//12λ�ֱ���
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;									//���õ�ͨ������˲���Ҫ����ɨ��ģʽ
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;								//�����������ɼ������������һ�κ�������ݲɼ��Ͳ��Ƕ�ʱ���ɼ���
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_Rising;	//�����ش�����Ч
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;			//��ʱ��TIM3����
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;							//�����Ҷ���
  ADC_InitStruct.ADC_NbrOfConversion = 1;										//ת��ͨ��1��,���ͨ����ָÿ��ADC��ͨ����
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=ADC_IRQn;                   				//ѡ��TIM3�Ķ�ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;      			//��ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;							//�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					
	NVIC_Init(&NVIC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);														//����ADת����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles );	//����ָ�� ADC �Ĺ�����ͨ��1��PA1)����һ���ɼ�������ʱ��144�����ڣ�һ������Ϊ1/21us
}



void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//ʹ��TIM3�ϵ�APB1ʱ��
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;						//TIM3�Ķ�ʱ����Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//TIM3Ϊ���¼�����ʽ
	TIM_TimeBaseInitStruct.TIM_Period=arr;							//TIM3���Զ���װ��ֵ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;			//ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);			//��ʱ��2���´���DAC
	TIM_Cmd(TIM3,ENABLE);											//ʹ�ܶ�ʱ��3
	
}

void ADC_IRQHandler(void)
{	
	static u32 n=0;
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)!=RESET)						          //�ȴ�ת������		
	{		
		Sintable[n++]=ADC_GetConversionValue(ADC1)*3.3/4096;			         //�������һ�� ADC1 �������ת�������������ȡ��������������������
	}
	if(n==TableSize)
	{
		n=0;
		ADC_EndFlag=1;
		ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);							//�ɼ�TableSize�����ݺ�ر�ADC�ж�
	}
	ADC_ClearITPendingBit(ADC1,ADC_FLAG_EOC);							//����жϱ�־λ
}

