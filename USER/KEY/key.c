#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 



//������ʼ������
void KEY_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	
	/*��������GPIO�ڵ�ʱ��*/
	RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	
  /*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN; 
  
  /*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  
  /*�������Ų�����Ҳ������*/
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;;
	
  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);   
  
  /*ѡ�񰴼�������*/
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN; 
  
  /*ʹ������Ľṹ���ʼ������*/
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure); 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
/*u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(0)key_up=1;  //֧������		  
	if(key_up&&(KEY0==1||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==1)return KEY0_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==0&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
















