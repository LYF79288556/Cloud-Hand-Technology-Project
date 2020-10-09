/**
  ******************************************************************************
  * @file    public.c
  * @author  Tornado-IOT�Ƽ�
  * @version V1.0
  * @date    2017-10-15
  * @brief   ����ϵͳ�δ�ʱ��������ʱ
  ******************************************************************************
  * @attention
  *
  * QQȺ��100300621
  *
  * ΢��Ⱥ��Tornado-IOT����������Ⱥ
  *
  * �Ա����ӣ�https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-15484196154.37.1868cf7okMzJm&id=559452266590
  *
  * Tornado�Ŷ���ϵ��ʽ��
  *
  * QQ��1343263021
  *
  * ΢�ţ�17625548901
  *
  ******************************************************************************
  */

#include "public.h"


static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������

/** 
  * @name   Set_One_Pin_State
  
  * @brief  ����ĳһ��IO��
  
  * @param  GPIOx������
	
	* @param  GPIO_Pin���˿�
	
	* @param  state���˿�״̬
  
  * @retval ��
  */
void Set_One_Pin_State(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,unsigned char state)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, state >0 ? Bit_SET : Bit_RESET);
}

/** 
  * @name   Init_Delay
  
  * @brief  ��ʼ���ӳٺ�����SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8��SYSCLK:ϵͳʱ�ӡ���
  
  * @param  ��
  
  * @retval ��
  */
void Init_Delay(void)	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    			//�رռ�����
	SysTick->VAL =0X00;       								//��ռ�����	
	
	fac_us=SystemCoreClock/8000000;							//Ϊϵͳʱ�ӵ�1/8
	fac_ms=(u16)fac_us*1000;								//����ÿ��ms��Ҫ��systickʱ����   
}								    

/** 
  * @name   Delay_us
  
  * @brief  ��ʱnus��nus�ķ�Χ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nus<=0xffffff*8/SYSCLK��SYSCLK��λΪHz,nus��λΪus����72M������,nus<=1864135	 ��
  
  * @param  nus����ʱʱ�䣨��λ΢�
  
  * @retval ��
  */  								   
void Delay_us(u32 nus)
{	
	u32 temp;	
	if(nus<=0) return;
	SysTick->VAL=0x00;        					//��ռ�����	
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;    //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽�� 
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    //�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	 
}

/** 
  * @name   Delay_ms
  
  * @brief  ��ʱnms��nms�ķ�Χ��SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:nms<=0xffffff*8*1000/SYSCLK��SYSCLK��λΪHz,nms��λΪms����72M������,nms<=1864 ����
  
  * @param  GPIOx������
	
	* @param  GPIO_Pin���˿�
	
	* @param  state���˿�״̬
  
  * @retval ��
  */
void Delay_ms(u16 nms)
{	 
	u32 temp;
	if(nms<=0) return;
	SysTick->VAL =0x00;           			//��ռ�����
	SysTick->LOAD=(u32)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//�رռ�����
	SysTick->VAL =0X00;       				//��ռ�����	  	    
} 


