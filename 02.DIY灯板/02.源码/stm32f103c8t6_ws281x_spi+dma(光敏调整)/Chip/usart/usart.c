/**
******************************************************************************
* @file    usart.c
* @author  Tornado�Ƽ�
* @version V1.0
* @date    2018-4-28
* @brief   ���ܣ����ڵײ�����
******************************************************************************
* @attention
*
* �Ա����ӣ�https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-15484196154.37.1868cf7okMzJm&id=559452266590
*
* Tornado�Ŷ���ϵ��ʽ��
* QQ��  1343263021
* QQȺ��100300621
* ΢�ţ�17625548901
*
******************************************************************************
*/ 

#include "usart.h"	
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


/** 
  * @name   Init_USART1
  
  * @brief  ��ʼ��USART1
  
  * @param  bound��USART1������
  
  * @retval ��
  */
void Init_USART1(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//����GPIO�ṹ��
	USART_InitTypeDef USART_InitStructure;																						//����USART�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;																							//����NVIC�ṹ��

	USART_DeInit(USART1);  																														//��λ����1
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);				//ʹ��USART1��GPIOAʱ��
	
  //USART1_TX:PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 																				//PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O�˿��ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);																						//��ʼ��GPIOA_Pin9
   
  //USART1_RX:PA10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																				//PA9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  																					//��ʼ��GPIOA_Pin10

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;																	//ָ��IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;														//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																										//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;																				//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//�����ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;																//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); 																				//��ʼ������
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																		//�����ж�
  USART_Cmd(USART1, ENABLE);                    																		//ʹ�ܴ��� 
}

/** 
  * @name   Init_USART2
  
  * @brief  ��ʼ��USART2
  
  * @param  bound��USART2������
  
  * @retval ��
  */
void Init_USART2(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//����GPIO�ṹ��
	USART_InitTypeDef USART_InitStructure;																						//����USART�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;																							//����NVIC�ṹ��

	USART_DeInit(USART2);  																														//��λ����2
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);														//ʹ��USART2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);															//ʹ��GPIOAʱ��
	
  //USART1_TX:PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 																				//PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O�˿��ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);																						//��ʼ��GPIOA_Pin2
   
  //USART1_RX:PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																					//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  																					//��ʼ��GPIOA_Pin3

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;																	//ָ��IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;														//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																										//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;																				//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//�����ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;																//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); 																				//��ʼ������
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);																		//�����ж�
  USART_Cmd(USART2, ENABLE);                    																		//ʹ�ܴ��� 
}

/** 
  * @name   Init_USART3
  
  * @brief  ��ʼ��USART3
  
  * @param  bound��USART3������
  
  * @retval ��
  */
void Init_USART3(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//����GPIO�ṹ��
	USART_InitTypeDef USART_InitStructure;																						//����USART�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;																							//����NVIC�ṹ��

	USART_DeInit(USART3);  																														//��λ����3
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);														//ʹ��USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);															//ʹ��GPIOBʱ��
	
  //USART1_TX:PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 																				//PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O�˿��ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);																						//��ʼ��GPIOB_Pin10
   
  //USART1_RX:PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;																				//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  																					//��ʼ��GPIOB_Pin11

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;																	//ָ��IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;														//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																										//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;																				//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//�����ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;																//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); 																				//��ʼ������
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);																		//�����ж�
  USART_Cmd(USART3, ENABLE);                    																		//ʹ�ܴ��� 
}

/** 
  * @name   Print
  
  * @brief  USARTx��ӡ����
  
  * @param  USARTx��USARTx

  * @param  formt����ӡ��ʽ

  * @param  ...����������
  
  * @retval ��
  */
void Print(USART_TypeDef* USARTx,const char *formt,...)
{
	va_list ap;
	unsigned char buf[250];
	unsigned char *pStr = buf;
	va_start(ap, formt);
	vsnprintf((char *)buf, sizeof(buf), formt, ap);				
	va_end(ap);
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}
}

/** 
  * @name   USART_Send_Data
  
  * @brief  USARTx���һ���ֽ�����
  
  * @param  USARTx��USARTx

  * @param  data��Ҫ�����һ���ֽ�
  
  * @retval ��
  */
void USART_Send_Data(USART_TypeDef* USARTx,unsigned char data)
{
	USART_SendData(USARTx, data);													//�򴮿ڷ�������
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);				//�ȴ����ͽ���
}


/** 
  * @name   USART_Send_Str
  
  * @brief  USARTx����ַ���
  
  * @param  USARTx��USARTx

  * @param  str��Ҫ������ַ���
  
  * @retval ��
  */
void USART_Send_Str(USART_TypeDef* USARTx, unsigned char *str)
{
  unsigned int length=0;
	length=strlen((const char *)str);
	for(;length>0;length--)
	{
		USART_SendData(USARTx, *str++);												//�򴮿�1��������
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);					//�ȴ����ͽ���
	}
}

/** 
  * @name   USART_Send_Datas
  
  * @brief  USARTx���һ������
  
  * @param  USARTx��USARTx

  * @param  str��Ҫ���������

  * @param  len��Ҫ��������ݵĳ���
  
  * @retval ��
  */
void USART_Send_Datas(USART_TypeDef* USARTx, unsigned char *str, unsigned int len)
{
	for(;len>0;len--)
	{
		USART_SendData(USARTx, *str++);														//�򴮿�1��������
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	//�ȴ����ͽ���
	}
}

/** 
  * @name   USART1_IRQHandler
  
  * @brief  USART1�ж�

  * @param  ��
  
  * @retval ��
  */	
void USART1_IRQHandler(void)                									//����1�жϷ������
{
	u8 Res=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 				//�жϲ���
	{  
		Res =USART_ReceiveData(USART1);														//(USART1->DR)����ȡ���յ�������
	}
	
	//��������־λ
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART1,USART_FLAG_ORE); 					//����жϱ�־
	}
	//���������־λ
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART1,USART_FLAG_NE); 						//����жϱ�־
	} 
	//֡�����־λ
	if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART1,USART_FLAG_FE); 						//����жϱ�־
	} 
} 

/** 
  * @name   USART2_IRQHandler
  
  * @brief  USART2�ж�

  * @param  ��
  
  * @retval ��
  */
void USART2_IRQHandler(void)                									//����2�жϷ������
{
	u8 Res=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) 				//�жϲ���
	{  
		Res =USART_ReceiveData(USART2);														//(USART2->DR)����ȡ���յ�������
	}
	
	//��������־λ
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART2,USART_FLAG_ORE); 						//����жϱ�־
	}
	//���������־λ
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART2,USART_FLAG_NE); 						//����жϱ�־
	} 
	//֡�����־λ
	if(USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART2,USART_FLAG_FE); 						//����жϱ�־
	} 
} 

/** 
  * @name   USART3_IRQHandler
  
  * @brief  USART3�ж�

  * @param  ��
  
  * @retval ��
  */
void USART3_IRQHandler(void)                								//����3�жϷ������
{
	u8 Res=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) 			//�жϲ���
	{  
		Res =USART_ReceiveData(USART3);													//(USART3->DR)����ȡ���յ�������
	}
	
	//��������־λ
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART3,USART_FLAG_ORE); 				//����жϱ�־
	}
	//���������־λ
	if(USART_GetFlagStatus(USART3, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART3,USART_FLAG_NE); 					//����жϱ�־
	} 
	//֡�����־λ
	if(USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)����ȡ���յ�������
		USART_ClearITPendingBit(USART3,USART_FLAG_FE); 					//����жϱ�־
	}
} 

