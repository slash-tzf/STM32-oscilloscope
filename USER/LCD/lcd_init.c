#include "lcd_init.h"
#include "delay.h"

void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PORTA~E,PORTGʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOD,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10);
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
  LCD_CS_Set();	
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u32 dat)
{
	LCD_Writ_Bus(dat>>16);
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LCD_WR_REG(0x2a);//�е�ַ����
	LCD_WR_DATA8(x1>>8);
	LCD_WR_DATA8(x1);
	LCD_WR_DATA8(x2>>8);
	LCD_WR_DATA8(x2);
	LCD_WR_REG(0x2b);//�е�ַ����
	LCD_WR_DATA8(y1>>8);
	LCD_WR_DATA8(y1);
	LCD_WR_DATA8(y2>>8);
	LCD_WR_DATA8(y2);
	LCD_WR_REG(0x2c);//������д
}

 void LCD_Init(void)
{
	LCD_GPIO_Init();//��ʼ��GPIO
	
	LCD_RES_Clr();//��λ
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	
		LCD_BLK_Set();//�򿪱���
  delay_ms(100);
	LCD_WR_REG(0xE0); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x0D); 
	LCD_WR_DATA8(0x1B); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x3c); 
	LCD_WR_DATA8(0x78); 
	LCD_WR_DATA8(0x4A); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x0E); 
	LCD_WR_DATA8(0x09); 
	LCD_WR_DATA8(0x1B); 
	LCD_WR_DATA8(0x1e); 
	LCD_WR_DATA8(0x0f);  
	
	LCD_WR_REG(0xE1); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x24); 
	LCD_WR_DATA8(0x06); 
	LCD_WR_DATA8(0x12); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x36); 
	LCD_WR_DATA8(0x47); 
	LCD_WR_DATA8(0x47); 
	LCD_WR_DATA8(0x06); 
	LCD_WR_DATA8(0x0a); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x0f); 
	
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0x10); 
	LCD_WR_DATA8(0x10); 
	
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0x41); 
	
	LCD_WR_REG(0xC5); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x80); 
	
	LCD_WR_REG(0x36);    // Memory Access Control 
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x48);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0x88);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x28);
	else LCD_WR_DATA8(0xE8);

	
	LCD_WR_REG(0x3A); //Interface Mode Control���˴�ILI9486Ϊ0X55
	LCD_WR_DATA8(0x66);
		
	LCD_WR_REG(0XB0);  //Interface Mode Control  
	LCD_WR_DATA8(0x00); 
	LCD_WR_REG(0xB1);   //Frame rate 70HZ  
	LCD_WR_DATA8(0xB0); 
	LCD_WR_DATA8(0x11); 
	LCD_WR_REG(0xB4); 
	LCD_WR_DATA8(0x02);   
	LCD_WR_REG(0xB6); //RGB/MCU Interface Control
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x02); 
	
	LCD_WR_REG(0xB7); 
	LCD_WR_DATA8(0xC6); 
	LCD_WR_REG(0xE9); 
	LCD_WR_DATA8(0x00);
	
	LCD_WR_REG(0XF7);    
	LCD_WR_DATA8(0xA9); 
	LCD_WR_DATA8(0x51); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x82);
	
	LCD_WR_REG(0x11); 
	delay_ms(120); 
	LCD_WR_REG(0x29); 
/**********************����ΪILI9488��ʼ������*********************************************************************************/
//	//************* Start Initial Sequence **********//
//	����ΪILI9486������ʼ������
//	
//	//************* Start Initial Sequence **********// 
//	LCD_WR_REG(0XF2);
//	LCD_WR_DATA8(0x18);
//	LCD_WR_DATA8(0xA3);
//	LCD_WR_DATA8(0x12);
//	LCD_WR_DATA8(0x02);
//	LCD_WR_DATA8(0XB2);
//	LCD_WR_DATA8(0x12);
//	LCD_WR_DATA8(0xFF);
//	LCD_WR_DATA8(0x10);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_REG(0XF8);
//	LCD_WR_DATA8(0x21);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_REG(0X13);

//	LCD_WR_REG(0x36);    // Memory Access Control 
//	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
//	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
//	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
//	else LCD_WR_DATA8(0xA8);

//	LCD_WR_REG(0xB4);
//	LCD_WR_DATA8(0x02);
//	LCD_WR_REG(0xB6);
//	LCD_WR_DATA8(0x02);
//	LCD_WR_DATA8(0x22);
//	LCD_WR_REG(0xC1);
//	LCD_WR_DATA8(0x41);
//	LCD_WR_REG(0xC5);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_DATA8(0x18);



//	LCD_WR_REG(0x3a);
//	LCD_WR_DATA8(0x66);
//	delay_ms(50);



//	LCD_WR_REG(0xE0);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x1F);
//	LCD_WR_DATA8(0x1C);
//	LCD_WR_DATA8(0x0C);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x08);
//	LCD_WR_DATA8(0x48);
//	LCD_WR_DATA8(0x98);
//	LCD_WR_DATA8(0x37);
//	LCD_WR_DATA8(0x0A);
//	LCD_WR_DATA8(0x13);
//	LCD_WR_DATA8(0x04);
//	LCD_WR_DATA8(0x11);
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_REG(0xE1);
//	LCD_WR_DATA8(0x0F);
//	LCD_WR_DATA8(0x32);
//	LCD_WR_DATA8(0x2E);
//	LCD_WR_DATA8(0x0B);
//	LCD_WR_DATA8(0x0D);
//	LCD_WR_DATA8(0x05);
//	LCD_WR_DATA8(0x47);
//	LCD_WR_DATA8(0x75);
//	LCD_WR_DATA8(0x37);
//	LCD_WR_DATA8(0x06);
//	LCD_WR_DATA8(0x10);
//	LCD_WR_DATA8(0x03);
//	LCD_WR_DATA8(0x24);
//	LCD_WR_DATA8(0x20);
//	LCD_WR_DATA8(0x00);
//	LCD_WR_REG(0x11);
//	delay_ms(120);
//	LCD_WR_REG(0x29);
//	LCD_WR_REG(0x2C);
} 
