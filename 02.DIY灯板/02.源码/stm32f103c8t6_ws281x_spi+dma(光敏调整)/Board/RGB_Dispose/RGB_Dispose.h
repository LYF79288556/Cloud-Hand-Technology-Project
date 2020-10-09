#ifndef __RGB_Dispose_H
#define __RGB_Dispose_H

#include "stm32f10x.h"
#include "key.h"
#include "key_detect.h"
#include "usart.h"
#include "public.h"
#include "Timer.h"
#include "Lsens.h"
#include "../BOARD/ws2812/ws2812.h"
#include "delay.h"



#define  RGB_Lum			250			//LED��������
#define  RGB_Graded		10			//LED�����ݶ�
#define  Set_Wait			50			//LED���ʱ��  ��λms
#define  Auto_ScanTime_Set	20	//�Զ�ģʽ��LEDɨ��ʱ��  ��λms


typedef struct 
{
	uint8_t  R_Cnt;						//�����ֵ
	uint8_t  G_Cnt;						//�̵���ֵ
	uint8_t  B_Cnt;						//������ֵ
	uint8_t  Level_Cnt;					//���Ӽ���
}RGB_Variable;




enum  S_RGB_SingleColor
{
	White = 0,			//��ɫ
	Red,						//��ɫ
	Blue,						//��ɫ
	Green,					//��ɫ
	Yellow,					//��ɫ�����̣�
	cyan,						//��ɫ�����̣�
	Violet,					//��ɫ�����죩
};




enum  S_RGB_STATE
{
		RGB_OFF = 0,
		Automatic_LED_Mode,					//�Զ�����ģʽ
		SingleColor_Mode,						//��ɫ��ģʽ
		ColorWipe_Mode,							//��ˮ��ģʽ
		Rainbow_Mode,
		RainbowCycle_Mode,
		TheaterChase_Mode,
		TheaterChaseRainbow_Mode,
};

void key0_CallBack(void);
void key0_S_CallBack(void);
void key0_L_CallBack(void);
void key1_CallBack(void);
void key2_CallBack(void);
void key3_CallBack(void);
void Update_Key_CallBack(void);
void Print_Debug_mess(unsigned char *debug_Mess);

void  RGB_StateDispose(void);
void  SingleColor_ModeDispose(void);
void  ColorWipe_ModeDispose(void);
void  TheaterChase_ModeDispose(void);
#endif /* __RGB_Dispose_H */

