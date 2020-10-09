
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	main.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/
#include "common.h"

#define	 Backlight   _pb6					//��������
#define	 Backlight_HIGH()		  {Backlight = 0;}
#define	 Backlight_LOW()		  {Backlight = 1;}

void	fun_BacklightScan(void);
/********************************************************************
Function:	main
INPUT	:
OUTPUT	:	
NOTE	:   �������
********************************************************************/

void main()
{
	u32  ADC_Dep;
	double   ADC_Dep2;
	//�Д��Ƿ�����늏�λ������������r�µķ�������λ
	//��������t������늳�ʼ����������֮�t����WDT�����ʼ��
	if(_to ==0 || _pdf ==0)									// WDT	Reset
	{	//��늏�λ������������r�µķ�������λ
		fun_Ram_CLear();									// CLR  RAM 		
		fun_Sys_Init();
		fun_Reset_Init();		
	}
	else							 						
	{	//WDT �����λ
		fun_Reset_Init();
	}
	fun_Uart_Init();
	fun_Test_Function();
	fun_LCD_Display_Power();	//�ϵ��ʼ����ʾ
/*	fun_BAT_INIT();*/
	
	while(1)
	{
	// 	������T��	 		
		GCC_CLRWDT();	
	//	����ѭ��		
		SDK_TM.HaltTime = 0;	//Test ��Ҫ��������	
			
		if(SDK_TM.HaltTime < HALT_TIME)		
		{
			if(_tb0f)	//	8ms�жϱ�־λ�ж�
			{
				_tb0f = 0;	//8ms��־λ����
				
				
				
				fun_SysTimeScan();
				fun_WorkModeScan();			//����ģʽ����
				fun_Key_Scan();				//����ɨ�账��
				fun_BeepScan();				//����������
				fun_BacklightScan();		//���⴦��
				
					
				
				//������Iλ ���ڹ������̿��ƿ�ʹ��gbv_500ms���ɴ˵ط��yһ���
				if (gbv_500ms)
				{
					gbv_500ms = 0;
					
					/*fun_LCD_Display_Mode();*/
//					if(gbv_adc_Success)
//					{	
//						//	��ص�ѹ����
//						SDK_CAL.BAT_Buff.S16 = SDK_ADC.AdcData.S32;	
//						
//						fun_Uart_SendASCII(SDK_CAL.BAT_Buff.S16);
//						LCD_DISPLAY_NUM(1,SDK_CAL.BAT_Buff.S16);
///*						fun_Uart_SendASCII(ADC_Dep);*/
//							
//					/*	fun_ADC_Disable();*/
//						
//					}
				}
			}
	//	ADC ݆ԃ����
			fun_ADC_Polling();
		}
	//	����ģʽ
		else	
		{		
			fun_SysSleep();
		}		
	}			
	
}




/********************************************************************
Function:	fun_Test_Function
INPUT	:
OUTPUT	:	
NOTE	:   ���ܜyԇ
********************************************************************/
void	fun_Test_Function()
{	
	#if(TEST_MODE == TEST_LCD)
	fun_LCD_Test();
	#endif
	#if(TEST_MODE == TEST_BEEP)
	fun_Beep_Test();
	#endif
	#if(TEST_MODE == TEST_PERI)
	fun_Peripherals_Test();
	#endif
	#if(TEST_MODE == TEST_UART)
	fun_Uart_Test();
	#endif
	#if(TEST_MODE == TEST_NTC)
	fun_ADC_NTC_Test();
	#endif
	#if(TEST_MODE == TEST_OPAZERO)
	fun_ADC_OPA_ZeroTest();
	#endif
	#if(TEST_MODE == TEST_OPAVTP)
	fun_ADC_OPA_VTPTest();
	#endif
}


/********************************************************************
Function:	fun_BacklightScan()
INPUT	:
OUTPUT	:	
NOTE	:   ���⴦��
********************************************************************/
void	fun_BacklightScan()
{	
		if(gbv_KeyStatus_L_4)
		{
			gbv_KeyStatus_L_4 = 0;
			fun_Uart_SendByte(0x11);
			Backlight = ~Backlight;
		}
}


