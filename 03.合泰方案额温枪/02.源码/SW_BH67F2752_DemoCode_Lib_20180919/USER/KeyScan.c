
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	key.c	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/


#include "common.h"

/************************Key_Scan Variable****************/

KEY_define SDK_KEY;


#define	KEYPRESS_SCAN()			{							\
		if(KEY_PORT0 == 0)	SDK_KEY.KeyCurrent.bits.b0 = 1;	\
		if(KEY_PORT1 == 0)	SDK_KEY.KeyCurrent.bits.b1 = 1;	\
		if(KEY_PORT2 == 0)	SDK_KEY.KeyCurrent.bits.b2 = 1;	\
		if(KEY_PORT3 == 0)	SDK_KEY.KeyCurrent.bits.b3 = 1;	\
		if(KEY_PORT4 == 0)	SDK_KEY.KeyCurrent.bits.b4 = 1;	\
		if(KEY_PORT5 == 0)	SDK_KEY.KeyCurrent.bits.b5 = 1;	\
		if(KEY_PORT6 == 0)	SDK_KEY.KeyCurrent.bits.b6 = 1;	\
		if(KEY_PORT7 == 0)	SDK_KEY.KeyCurrent.bits.b7 = 1;	\
								}
								
							
								

/********************************************************************
Function:	fun_Key_dealwith
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Key_init(void)
{

}

/********************************************************************
Function:	Key_Scan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Key_Scan(void)
{

	SDK_KEY.KeyCurrent.U8 = 0;	//������а�����ǰ״̬
	KEYPRESS_SCAN();			//����״̬��ȡ
	if (SDK_KEY.KeyCurrent.U8 != SDK_KEY.KeyOld) 	//�жϰ�����ǰ״̬��������һ��״̬
	{
		SDK_KEY.KeyNoChangedTime = 0;			
		SDK_KEY.KeyOld = SDK_KEY.KeyCurrent.U8; 	//���浱ǰ״̬
		SDK_KEY.KeyDown.U8 = 0;
		SDK_KEY.KeyUp.U8 = 0;
		return; 
	}
	else
	{
		SDK_KEY.KeyNoChangedTime++;					//��ǰ����״̬δ�ı��ʱ�����			  
		if (SDK_KEY.KeyNoChangedTime >= LUCC_KEY_DEBOUNCE) //50  //ȥ������
		{
			SDK_KEY.KeyNoChangedTime = LUCC_KEY_DEBOUNCE;
			SDK_KEY.KeyPress.U8 = SDK_KEY.KeyOld;																//
			SDK_KEY.KeyDown.U8 = SDK_KEY.KeyPress.U8 & (SDK_KEY.KeyPress.U8 ^ SDK_KEY.KeyLast); //���津�����̰�����һ��
			SDK_KEY.KeyUp.U8 |= SDK_KEY.KeyLast & (~SDK_KEY.KeyPress.U8);								//�������ְ���
			SDK_KEY.KeyLast = SDK_KEY.KeyPress.U8;															//���浱ǰ����״̬���洢Ϊ��һ��
		}
	}
//--------------------------------����--------------------------------------------//
	if (SDK_KEY.KeyPressTime == LUCC_LONG_KEY_TIME)  //��������ѹʱ����ڳ���ʱ��
	{
		if (SDK_KEY.KeyPress.bits.b0)
		{
			gbv_KeyStatus_L_1 = 1;
			fun_Uart_SendByte(0x01);
		}
		if (SDK_KEY.KeyPress.bits.b1)
		{
			gbv_KeyStatus_L_2 = 1;
			fun_Uart_SendByte(0x02);
		}
		if (SDK_KEY.KeyPress.bits.b2)
		{
			gbv_KeyStatus_L_3 = 1;
			fun_Uart_SendByte(0x03);
		}
		if (SDK_KEY.KeyPress.bits.b3)
		{
			gbv_KeyStatus_L_4 = 1;
			fun_Uart_SendByte(0x04);
		}

		SDK_KEY.KeyPressTime = LUCC_LONG_KEY_TIME + 1;
	}
//--------------------------------�̰�--------------------------------------------//
	else if (SDK_KEY.KeyPressTime < LUCC_LONG_KEY_TIME)  //��������ѹʱ��С�ڳ���ʱ�������
	{
//--------------------------------����--------------------------------------------//		
		if (SDK_KEY.KeyUp.bits.b0)
		{
			gbv_KeyStatus_1 = 1;
			fun_Uart_SendByte(0x11);
		}
		if (SDK_KEY.KeyUp.bits.b1)
		{
			gbv_KeyStatus_2 = 1;
			fun_Uart_SendByte(0x22);
		}
		if (SDK_KEY.KeyUp.bits.b2)
		{
			gbv_KeyStatus_3 = 1;
			fun_Uart_SendByte(0x33);
		}
		if (SDK_KEY.KeyUp.bits.b3)
		{
			gbv_KeyStatus_4 = 1;
			fun_Uart_SendByte(0x44);
		}

//--------------------------------����--------------------------------------------//
		if (SDK_KEY.KeyDown.bits.b0)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b1)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b2)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b3)
		{
		
		}

	}
	else		//��������ѹʱ����ڳ���ʱ��
	{
		SDK_KEY.KeyPressTime = LUCC_LONG_KEY_TIME + 1; //Key time more than 3 s don't count,b.
															   //because of judge gu8v_KeyPressTime==5
	}
	if (SDK_KEY.KeyDown.U8 != 0)		//�ж�����а�������
	{
		SET_HALT_CLEAR();				//���ȴ��豸�ػ�ʱ������
	}
	if (SDK_KEY.KeyPress.U8)			//�ж��а�������ȥ
	{
		SDK_KEY.KeyPressTime++;			//������ѹ��ʱ�����
	}
	else								//�ж�û�а�������ȥ
	{
		SDK_KEY.KeyPressTime = 0;		//������ѹ��ʱ������
	}
	SDK_KEY.KeyUp.U8 = 0;
}

