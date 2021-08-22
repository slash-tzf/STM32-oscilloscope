#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "./LCD/lcd.h"
#include "./LCD/lcd_init.h"
#include "./ADC/adc.h"
#include "math.h" 
#include "arm_math.h"
#include "./KEY/key.h"
#define FFT_LENGTH 256

extern float Sintable[TableSize];
float LastSintable[TableSize];

float fft_inputbuf[FFT_LENGTH*2];	//FFT��������
float fft_outputbuf[FFT_LENGTH];	//FFT�������
float fft_lastoutputbuf[FFT_LENGTH];
extern u32 ADC_EndFlag;

void LCD_showwave()																//��LCD��Ļ����ʾ����
{
	u32 i;
	u32 m=1;
	for(i=1;i<TableSize;i++)
	{	
		
		LCD_DrawLine(m-1,150-40*LastSintable[i-1],m,150-40*LastSintable[i],WHITE);
		LCD_DrawLine(m-1,150-40*Sintable[i-1],m,150-40*Sintable[i],RED);//120����ȷ��һ����׼��X�ᣬ20�ǽ�ʵ�ʵĵ�ѹ����ֵ��󣬺���LCD����ʾ
		m++;
	}
}
void LCD_showFFT()																//��LCD��Ļ����ʾFFT������
{
	u32 i;
	u32 m=223;
	for(i=2;i<FFT_LENGTH;i++)
	{	
		
		LCD_DrawLine(m-1,310-fft_lastoutputbuf[i-1]/3,m,310-fft_lastoutputbuf[i]/3,WHITE);
		LCD_DrawLine(m-1,310-fft_outputbuf[i-1]/3,m,310-fft_outputbuf[i]/3,RED);//310����ȷ��һ����׼��X�ᣬ/3�ǽ�ʵ�ʵķ�ֵ��ֵ��С������LCD����ʾ
		m++;
	}
}
float MAX1(void)		//��ѹ���ֵ
{
	float max1=0;
	u32 i=0;
	for(i=0;i<TableSize;i++)
	{
		if(Sintable[i]>max1)
			max1=Sintable[i];
	}
	return max1;
}
int MAX2(int fbl)		//��ѹ���ֵ
{
	float max2=0;
	u32 i=0;
	u32 index=0;
	u32 t=0;
	u32 a=0;
	a=2000/fbl;
	for(i=1;i<FFT_LENGTH/2;i++)
	{
		if(fft_outputbuf[i]>max2){
			index=i;
			max2=fft_outputbuf[i];
		}
	}
	t=fbl*index;
	LCD_ShowIntNum(0,300,index,2,RED,WHITE,16);
	LCD_ShowIntNum(0,240,fft_outputbuf[a],5,RED,WHITE,16); 
	return t;
}

int main(void)
{	
	arm_cfft_radix4_instance_f32 scfft;
	int i;
	int arr=655;
	int pres=5;
	u32 cypl;     //����Ƶ��
	u32 fbl;        //�ֱ���
	float max1=0;
	int max2=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����ϵͳ�ж����ȼ�����2
	delay_init(168);   								    //��ʼ����ʱ����
 	LCD_Init();         								//��ʼ��LCD�ӿ�
	KEY_Init();													//��ʼ������
	Adc1_Init();      								    //��ʼ��ADC
	TIM3_Int_Init(arr,pres);								//5us��ʱ����ʼ����	
	LCD_Fill(0,0,480,320,WHITE);
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);
	while(1)
	{ 
		cypl= 84000000/((arr+1)*(pres+1));   //�������Ƶ��
		fbl = cypl/256;	  //����ֱ���
		LCD_DrawLine(0,150,480,150,BLACK);
		if(ADC_EndFlag==1)
		{	
			for(i=0;i<FFT_LENGTH;i++)//�����ź�����
			{
				 fft_inputbuf[2*i]=Sintable[i];
				 fft_inputbuf[2*i+1]=0;//�鲿ȫ��Ϊ0
			}
			arm_cfft_radix4_f32(&scfft,fft_inputbuf);
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);			
			max1=MAX1();
			max2=MAX2(fbl);
			LCD_ShowString(0,260,"Vpp:",RED,WHITE,16,0);
			LCD_ShowFloatNum1(40,260,max1,3,RED,WHITE,16);
			LCD_ShowString(0,280,"Fre:",RED,WHITE,16,0);
			LCD_ShowIntNum(40,280,max2,5,RED,WHITE,16);
			LCD_ShowIntNum(40,300,cypl,5,RED,WHITE,16);
			LCD_ShowIntNum(80,300,fbl,5,RED,WHITE,16);
			LCD_showwave();					//����
			LCD_showFFT();					//��Ƶ��
			ADC_EndFlag=0;
			for(i=0;i<TableSize;i++){
			LastSintable[i]=Sintable[i];
			}
			for(i=0;i<FFT_LENGTH;i++){
			fft_lastoutputbuf[i]=fft_outputbuf[i];
			}
			ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);	//����ADC�жϣ�ʵ��ʵʱ�ɼ�	
			delay_ms(50);
		}
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
		 TIM_Cmd(TIM3, DISABLE);
			if(pres>1)
			pres-=1;
			TIM_PrescalerConfig(TIM3,pres,TIM_PSCReloadMode_Immediate);
			TIM_Cmd(TIM3, ENABLE);
		}
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
		{
		 TIM_Cmd(TIM3, DISABLE);
			if(pres<8)
			pres+=1;
		 TIM_PrescalerConfig(TIM3,pres,TIM_PSCReloadMode_Immediate);
		 TIM_Cmd(TIM3, ENABLE);
		}
	}
}

