
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	adc.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"

ADC_define	SDK_ADC;

/********************************************************************
*	Function:	fun_VTP_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_VTP_INIT()
{
	SET_PGA_x64();
	SET_AGS_1();
	SET_VTP_CHANNAL();
	SET_ADC_SPEED_20HZ();
	fun_ADC_Power_Enable();
	fun_ADC_Enable();
}

/********************************************************************
*	Function:	fun_TEMP1_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_TEMP1_INIT()
{
	SET_PGA_x1();		//PGA DI+/DI- ���ͨ����������ѡ��
	SET_AGS_1();		//A/D ת���� PGAOP/PGAON ��������ź�����ѡ��
	SET_TEMP1_CHANNAL(); //ѡ�������������ѡ��λ
	SET_ADC_SPEED_80HZ();	//ѡ��A/Dת���Ĳ�����
	fun_ADC_Power_Enable();	//ADC�ĵ�Դʹ��
	fun_ADC_Enable();	
}

/********************************************************************
*	Function:	fun_TEMP2_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_TEMP2_INIT()
{
	SET_PGA_x1();
	SET_AGS_1();
	SET_TEMP2_CHANNAL();	
	SET_ADC_SPEED_40HZ();
	fun_ADC_Power_Enable();
	fun_ADC_Enable();	
}

/********************************************************************
*	Function:	fun_BAT_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_BAT_INIT()
{
	SET_PGA_x1();   //PGA DI+/DI- ���ͨ����������ѡ��
	SET_AGS_1();	//A/D ת���� PGAOP/PGAON ��������ź�����ѡ��
	SET_BAT_CHANNAL();	//ѡ�������������ѡ��λ
	SET_ADC_SPEED_40HZ(); //ѡ��A/Dת���Ĳ�����
	fun_ADC_Power_Enable(); //ADC�ĵ�Դʹ��
	fun_ADC_Enable();		
}


/********************************************************************
*	Function:	fun_ADC_Power_Enable
*	INPUT	:
*	OUTPUT	:
*	NOTE	:  	
********************************************************************/
void fun_ADC_Power_Enable()
{

	SET_AVDD_2_4();  //LDO �����ѹѡ��2.4V����
	//�����������m�����в���׃�������ڴ��M���O��
	SET_VGS_0();	//REFP/REFN ��ֲο���ѹ����ѡ��
//	SET_VREFBUFF_DISABLE();
	SET_ADC_FMCLOCK();	//A/D ת����ʱ��Դ f MCLK ����
	SET_LDO_OPEN();		//��LDO�����ѹ

}

/********************************************************************
*	Function:	fun_ADC_Power_Disable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Power_Disable()
{
//	SET_LDO_CLOSE();
}



/********************************************************************
*	Function:	fun_ADC_Enable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Enable()
{
	SDK_ADC.SampleCount = 0;
	gbv_adc_SampleMode = 0;
	gbv_adc_Success = 0;	
	SET_ADC_OPEN();			//��ADC
	SET_ADC_NORMAL();		//��������ģʽ��A/D ת��������ģʽ����λ
	SET_ADRST_DISABLE();	//A/D ת���������λ����λ���رո�λ
	SET_ADRST_ENABLE();		//A/D ת���������λ����λ���򿪸�λ
	SET_ADRST_DISABLE();	//A/D ת���������λ����λ���رո�λ
	SET_ADCEOC_CLEAR();		//���ADת��������־λ

}

/********************************************************************
*	Function:	fun_ADC_Disable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Disable()
{
	SDK_ADC.SampleCount = 0;
	gbv_adc_SampleMode = 0;
	SET_ADC_SLEEP();
	SET_ADRST_DISABLE();
	SET_ADC_CLOSE();
	fun_ADC_Power_Disable();	
}

/********************************************************************
*	Function:	fun_ADC_Polling
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   �@ȡʮ�P��������ȥ�����Сֵ��ƽ��
********************************************************************/
void fun_filter_adc()
{	
	u8 i;
	s32 ls32v_adc_result = 0;
	s16 ls16v_adc_max;
	s16 ls16v_adc_min;
	
	ls16v_adc_max = SDK_ADC.AdcBuff[0];
	ls16v_adc_min = SDK_ADC.AdcBuff[0];
	
	for(i=0;i<10;i++)
	{
		ls32v_adc_result += SDK_ADC.AdcBuff[i];
		ls16v_adc_max = (ls16v_adc_max > SDK_ADC.AdcBuff[i]) ?  ls16v_adc_max : SDK_ADC.AdcBuff[i] ; 
		ls16v_adc_min = (ls16v_adc_min < SDK_ADC.AdcBuff[i]) ?  ls16v_adc_min : SDK_ADC.AdcBuff[i] ; 
	}
		
	SDK_ADC.AdcData.S32 = (u16)((ls32v_adc_result - ls16v_adc_min - ls16v_adc_max) >> 3);
}

/********************************************************************
*	Function:	fun_ADC_Polling
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Polling()
{

	if(_eoc!=0)		//�ж�ADCת������û�С�1��ת��������0��δ����
	{
		//�i�攵��
		SET_ADCLATCH_ENABLE();   //ʹ��A/D ת���������湦�ܿ���
		//�������D�Q��32bit ���з�̖����
		SDK_ADC.AdcData.U32 = 0;
		SDK_ADC.AdcData.byte.byte0	= _adrl;		//��ȡ��8λ����
		SDK_ADC.AdcData.byte.byte1	= _adrm;		//��ȡ��8λ����
		SDK_ADC.AdcData.byte.byte2	= _adrh;		//��ȡ��8λ����
		if(SDK_ADC.AdcData.bits.b23)             	//�ж����1λ�ǲ���Ϊ1������ǽ�ʣ������8λ��Ϊ1
		{
			SDK_ADC.AdcData.byte.byte3 = 0xff;
		}

		#if(TEST_MODE==mac_Test_On)
		// �yԇģʽ 
		SDK_ADC.AdcData.S32 >>= 8;
		#else
		// ȡ 24bit adc �е� 16bit ����������Ч����
		SDK_ADC.AdcData.S32 >>= 8;
		#endif
		SET_ADCLATCH_DISABLE();		//����A/D ת���������湦�ܿ���
		SET_ADCEOC_CLEAR();			//���A/D ת��������־
		
		//���Ӌ��
		SDK_ADC.SampleCount++;
		if(gbv_adc_SampleMode==0)
		{
			//�G��ǰ���P����
			if(SDK_ADC.SampleCount>3)
			{
				SDK_ADC.SampleCount=0;
				gbv_adc_SampleMode = 1;

			}
		}
		else if(gbv_adc_SampleMode==1)
		{	
			
			
		/*	array_uart_txbuff[0] = 0x55;
			array_uart_txbuff[1] = 0x00;			
			array_uart_txbuff[2] = SDK_ADC.AdcData.byte.byte0;			
			array_uart_txbuff[3] = SDK_ADC.AdcData.byte.byte1;			
			array_uart_txbuff[4] = SDK_ADC.AdcData.byte.byte2;						
			array_uart_txbuff[5] = array_uart_txbuff[0]+array_uart_txbuff[1]+array_uart_txbuff[2]+array_uart_txbuff[3]+array_uart_txbuff[4];						
			gu8v_tx_guide = 0;
			_txr_rxr = array_uart_txbuff[gu8v_tx_guide];*/
		
			
//			SDK_ADC.AdcBuff[SDK_ADC.SampleCount-1] = SDK_ADC.AdcData.S32;
//			if(SDK_ADC.SampleCount>=10)
//			{
//
//				fun_filter_adc();
//				gbv_adc_Success		= 1;
//				SDK_ADC.SampleCount = 0;
//				gbv_adc_SampleMode 	= 0;
//
//			}

			SDK_ADC.SampleCount = 0;
			gbv_adc_SampleMode 	= 0;
			gbv_adc_Success		= 1;	
			
		//	GCC_DELAY(1000);
		}	 
	}		
}

/********************************************************************
*	Function:	fun_ADC_ZeroTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_ZeroTest()
{
//	fun_ADC_Zero();
	fun_Uart_Init();
	while(1)
	{
		GCC_CLRWDT();//������T��	
		fun_ADC_Polling();
		
		
	}

}

/********************************************************************
*	Function:	fun_ADC_NTC_Test
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_NTC_Test()
{
	fun_TEMP2_INIT();
	fun_Uart_Init();
		_flms2 = 0;
		_flms1 = 1;				
		_flms0 = 0;					
		
		
		array_uart_rxbuff[1] = 7;
		_pgac0 &= 0b11111000;
		_pgac0 += array_uart_rxbuff[1];		
	
	while(1)
	{
		GCC_CLRWDT();//������T��	
		fun_ADC_Polling();
		fun_Uart_Handle();
		
	}

}

/********************************************************************
*	Function:	fun_ADC_OPA_ZeroTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_OPA_ZeroTest()
{
	fun_VTP_INIT();
	fun_Uart_Init();
	SET_OPA_ZERO();
	while(1)
	{
		GCC_CLRWDT();//������T��	
		fun_ADC_Polling();
		
	}

}
/********************************************************************
*	Function:	fun_ADC_OPA_VTPTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_OPA_VTPTest()
{
	fun_VTP_INIT();
	fun_Uart_Init();
	SET_OPA_VTP();
	while(1)
	{
		GCC_CLRWDT();//������T��	
		fun_ADC_Polling();
		
	}

}
