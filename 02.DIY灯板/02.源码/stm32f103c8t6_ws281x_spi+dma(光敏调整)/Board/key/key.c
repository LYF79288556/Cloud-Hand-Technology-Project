
	
#include "key.h"


unsigned char KEY0=0;	//��ȡ����0״̬
unsigned char KEY1=0;	//��ȡ����1״̬
unsigned char KEY2=0;	//��ȡ����2״̬
unsigned char KEY3=0;	//��ȡ����3״̬
	
	
/** 
  * @name   Init_KEY
  
  * @brief  ��ʼ��������I/O��
  
  * @param  ��
  
  * @retval ��
  */
void Init_KEY(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//ʹ��PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIO��
}
