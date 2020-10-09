#include "stm32f10x.h"

#include "usart.h"
#include "delay.h"
#include "../BOARD/ws2812/ws2812.h"
#include "public.h"
#include "usart.h"	 
#include "key.h"
#include "key_detect.h"
#include "Timer.h"
#include "RGB_Dispose.h"
#include "Lsens.h"
#include "chip.h"

#define WKUP_KD PAin(1)			//PA0 ����Ƿ��ⲿWK_UP��������

const char s[5];
int8_t i;


void Init_SW(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//ʹ��PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 								//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIO��
}


int main(void)
{
	Init_Delay();																		//��ʼ����ʱ��������ϵͳ�δ�ʱ�����������е���ʱ��ʹ��ϵͳ�δ�ʱ��������ʱ�ģ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  Init_USART1(115200);
  delay_init();
	TIM2_Int_Init(1000,71);			//��ʱ����ʼ��
	Lsens_Init();
	
  ws281x_init();
	Init_KEY();	
	Init_SW();
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	//��ʼ�������ṹ��
	Init_Key_Struct(Update_Key_CallBack,Print_Debug_mess);
	
//�͵�ƽ����
	Reg_Key(&KEY3, 3, L_Trig, S_Click, key3_CallBack);//����
//	Reg_Key(&KEY0, 50, L_Trig, D_Click, key0_CallBack);//˫��
	Reg_Key(&KEY0, 3, L_Trig, S_Click, key0_S_CallBack);//����
//	Reg_Key(&KEY0, 100, L_Trig, L_Press, key0_L_CallBack);//˫��
	Reg_Key(&KEY1, 3, L_Trig, S_Click, key1_CallBack);//����
	Reg_Key(&KEY2, 3, L_Trig, S_Click, key2_CallBack);//����
	
	ws281x_closeAll();  //�ر�ȫ���ĵ�
	delay_ms(5); //�ر�ȫ���ĵ���Ҫһ����ʱ��  
	Print(USART1,"ϵͳ��ʼ�����\r\n");
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
  while(1)
  {

		if(!Time_KEY_cnt)
		{
			Time_KEY_cnt = 5;
			Key_Detect();
		}
		
		RGB_StateDispose();
  }
}
