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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);						//使能 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 						//使能 ADC1 时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;										//选择ADC1通道1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;									//设置成模拟输入模式
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;								//不使用上拉和下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

	
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;								//ADC独立工作模式
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;							//ADC1挂在APB2=84M,因此ADC时钟频率21MHZ
	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;			//ADC1通道不采用直接访问DMA模式
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;		//两个采样时间之间的延迟，双重或者多重才有效
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;							//12位分辨率
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;									//采用单通道，因此不需要开启扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;								//不开启连续采集，否则除开第一次后面的数据采集就不是定时器采集了
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_Rising;	//上升沿触发有效
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;			//定时器TIM3触发
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;							//数据右对齐
  ADC_InitStruct.ADC_NbrOfConversion = 1;										//转换通道1个,这个通道是指每个ADC的通道数
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=ADC_IRQn;                   				//选择TIM3的定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;      			//抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;							//子优先级为2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					
	NVIC_Init(&NVIC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);														//开启AD转换器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles );	//设置指定 ADC 的规则组通道1（PA1)，第一个采集，采样时间144个周期，一个周期为1/21us
}



void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//使能TIM3上的APB1时钟
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;						//TIM3的定时器分频系数
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//TIM3为向下计数方式
	TIM_TimeBaseInitStruct.TIM_Period=arr;							//TIM3的自动重装载值
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;			//时钟分频因子
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);			//定时器2更新触发DAC
	TIM_Cmd(TIM3,ENABLE);											//使能定时器3
	
}

void ADC_IRQHandler(void)
{	
	static u32 n=0;
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)!=RESET)						          //等待转换结束		
	{		
		Sintable[n++]=ADC_GetConversionValue(ADC1)*3.3/4096;			         //返回最近一次 ADC1 规则组的转换结果并将将读取到的数字量存入数组中
	}
	if(n==TableSize)
	{
		n=0;
		ADC_EndFlag=1;
		ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);							//采集TableSize个数据后关闭ADC中断
	}
	ADC_ClearITPendingBit(ADC1,ADC_FLAG_EOC);							//清除中断标志位
}

