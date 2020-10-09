/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	work.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/



/*************************************************************************************/
//**								����˵��						                 **
//**					����һ��ҪУ׼��У׼�󼴿ɲ�����ȷ�¶�
//**
/*************************************************************************************/
#include "common.h"

SYS_define	SDK_SYS;
CAL_define	SDK_CAL;

/********************************************************************
Function:	fun_WorkModeScan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

void fun_WorkModeScan(void)
{
	switch(SDK_SYS.WorkMode)		//��ȡϵͳ״̬ģʽ
	{
		case	WORKMODE_POWERON:	//ϵͳ����
		break;
		case	WORKMODE_WAKEUP:	//ϵͳ����ģʽ
		/*	if(gbv_KeyStatus_1)		//����1���ѿ���*/
			{
				GOTO_SCAN_MODE();	//��ת������ģʽ
			}
			
		break;
		case	WORKMODE_SCAN:		//ɨ��ģʽ
		{
		
			if(fun_get_NTC())		//��ȡNTC��ֵ������1���ǻ�ȡ�ɹ�������0��ȡʧ��
			{
				SDK_CAL.RES_Buff.U16 = fun_Rntc_Calculate(SDK_CAL.ADC_Res1.S16,SDK_CAL.ADC_Res2.S16);
				gbv_Flag_GetNTC = 1;
			}
				
			if(gbv_Flag_GetNTC)
			{	
				gbv_Flag_GetNTC = 0;
			
				// �M�����y����ģʽ
				//-----------------------------------------------------------------------//	
			/*	gbv_KeyStatus_1 = 1;*/
				if(gbv_KeyStatus_1)
				{
					gbv_KeyStatus_1 = 0;
					
					if(SDK_SYS.DisMode == Mode_Body)
					{
						GOTO_MEAS_MODE();
					}
					else if(SDK_SYS.DisMode == Mode_Room)
					{
						GOTO_AmbTemp_MODE();
					}
					else if(SDK_SYS.DisMode == Mode_SurTmp)
					{
						GOTO_ObjTemp_MODE();
					}
					
					gbv_Flag_GetNTC = 	0;
				}
				
				if(gbv_KeyStatus_2)			//�̰�����2�л�ģʽ��ʾ
				{		
					gbv_KeyStatus_2 = 0;
									
					SDK_SYS.DisMode ++;
					
					if(SDK_SYS.DisMode == 4)
						SDK_SYS.DisMode = 1;
						
					fun_LCD_Display_Mode(SDK_SYS.DisMode);
					
					
				}
				
				if(gbv_KeyStatus_L_2)			//��������2����У��ģʽ
				{		
					gbv_KeyStatus_L_2 = 0;			
					GOTO_CORR_MODE();		//��ת��У׼ģʽ
					fun_LCD_Display_Cor(0);
				}
				
				if(gbv_KeyStatus_3)			//�̰�����3�л����϶Ⱥͻ��϶���ʾ
				{			
					gbv_KeyStatus_3 = 0;
										
					SDK_SYS.Dis_CF_Flag++;
					
					if(SDK_SYS.Dis_CF_Flag == 3)
						SDK_SYS.Dis_CF_Flag = 1;
					
					fun_LCD_Display_CF(SDK_SYS.Dis_CF_Flag);
					
					if(SDK_SYS.Dis_CF_Flag == 1)				//C
					{
						if(SDK_SYS.DisMode == Mode_Body)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//����LCD��ʾ�¶ȣ���С����
						}
						else if(SDK_SYS.DisMode == Mode_Room)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.AmbTemp.U16);		//����LCD��ʾ�¶ȣ���С����
						}
						else if(SDK_SYS.DisMode == Mode_SurTmp)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//����LCD��ʾ�¶ȣ���С����
						}
					
					}
					else if(SDK_SYS.Dis_CF_Flag == 2)			//F
					{
						if(SDK_SYS.DisMode == Mode_Body)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
						}
						else if(SDK_SYS.DisMode == Mode_Room)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.AmbTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
						}
						else if(SDK_SYS.DisMode == Mode_SurTmp)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
						}
					}
						
					
				}
			
			}
			
/*			fun_LCD_Display_C();		//��˸����*/

			break;
		}
		case	WORKMODE_MEAS:			//��ȡ�����¶�
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//��NTC���¶Ⱥ�VTP��ADCֵ����		
				SDK_CAL.ObjTemp.U16 	= 	fun_Tobj_Calculate(SDK_CAL.AmbTemp.U16,SDK_CAL.ADC_Vtp.S16);	//����һ��У׼�����	
				SDK_CAL.ObjTemp.U16 	= 	fun_ObjTemp_DataHandle(SDK_CAL.ObjTemp.U16);					//��ȡ����¶�
				SDK_CAL.BodyTemp.U16	=	fun_Temp_SurfaceToBody(SDK_CAL.ObjTemp.U16);					//��ȡ�����¶�
				
				fun_BeepSet(BEEP_1);						//���÷�����ģʽ
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//����LCD��ʾ�¶ȣ���С����
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
				}
				GOTO_SCAN_MODE();							//���صȴ���������ģʽ
				
			}
			else
			{	
				fun_LCD_Display____();			//��ʾ�ȴ���ȡ
			}
		}
		break;	
		case	WORKMODE_AmbTemp:			//��ȡ�����¶�
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//��NTC���¶Ⱥ�VTP��ADCֵ����		

				fun_BeepSet(BEEP_1);						//���÷�����ģʽ
				
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.AmbTemp.U16);		//����LCD��ʾ�¶ȣ���С����
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.AmbTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
				}
				GOTO_SCAN_MODE();							//���صȴ���������ģʽ
				
			}
			else
			{	
				fun_LCD_Display____();			//��ʾ�ȴ���ȡ
			}	
		}
		case	WORKMODE_ObjTemp:			//��ȡ�����¶�
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//��NTC���¶Ⱥ�VTP��ADCֵ����		
				SDK_CAL.ObjTemp.U16 	= 	fun_Tobj_Calculate(SDK_CAL.AmbTemp.U16,SDK_CAL.ADC_Vtp.S16);	//����һ��У׼�����	

				fun_BeepSet(BEEP_1);						//���÷�����ģʽ
				
				
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//����LCD��ʾ�¶ȣ���С����
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//����LCD��ʾ�¶ȣ���С����
				}
				GOTO_SCAN_MODE();							//���صȴ���������ģʽ
				
			}
			else
			{	
				fun_LCD_Display____();			//��ʾ�ȴ���ȡ
			}
		}	
		case	WORKMODE_CORR:			//У׼ģʽ
		{
			fun_CORR_MODE();
			break;		
		}
	
		default:	GOTO_SCAN_MODE();break;		
	}
}

volatile bit 	gbv_Corr_Lock;
volatile u8 	gu8v_CorrMode;
volatile u8 	gu8v_CorrCount;
void	fun_CORR_MODE()			//У��ģʽ
{
	switch(gu8v_CorrMode)
	{
		case	0://�M��ضȜy��
		
			if(gbv_Corr_Lock == 0)
			{
				if(fun_get_NTC())//NTC	//��ȡ�����¶�
				{	
					gbv_Corr_Lock = 1;
					SDK_CAL.RES_Buff.U16 = fun_Rntc_Calculate(SDK_CAL.ADC_Res1.S16,SDK_CAL.ADC_Res2.S16);	
					SDK_CAL.AmbTemp.U16	 = fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);
				}
			}
			else
			{
				if(gbv_500ms_reversal)
				{
					LCD_DISPLAY_NUM(1,SDK_CAL.AmbTemp.U16);			//��ʾ����¶�
				}
				else
				{
					fun_LCD_FULL(0);			//ʱ�仹û������ʾ0;
				}
				if(gbv_KeyStatus_1)				//�̰�һ�°��������ȡ�¶�
				{
					gbv_KeyStatus_1 = 0;				
					gbv_Corr_Lock = 0;
					gu8v_CorrCount = 0;
					fun_LCD_Display_Cor(1);
					gu8v_CorrMode++;
				}
			}
			break;
		case	1:
			gu8v_CorrCount++;
			if(gu8v_CorrCount>100)
			{
				gu8v_CorrCount = 0;
				gu8v_CorrMode++;
			}
		break;
		case	2:
		
			if(fun_get_VTP_CORR())//ӛ䛵�һ���ض��c�ą���	
			{	
				LCD_DISPLAY_NUM(1,SDK_CAL.ADC_Vtp.S16);
			}
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;	
				gu8v_CorrMode++;		
				SDK_CAL.VTP_Buff1.S16 = SDK_CAL.ADC_Vtp.S16;
				fun_LCD_Display_Cor(2);
			}
		break;
		case	3:
			gu8v_CorrCount++;
			if(gu8v_CorrCount>100)
			{
				gu8v_CorrCount = 0;
				gu8v_CorrMode++;
			}
		break;
		case	4:
			if(fun_get_VTP_CORR())//ӛ䛵ڶ����ض��c�ą���	
			{	
				LCD_DISPLAY_NUM(1,SDK_CAL.ADC_Vtp.S16);	
			}	
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;	
				gu8v_CorrMode++;		
				SDK_CAL.VTP_Buff2.S16 = SDK_CAL.ADC_Vtp.S16;
			}
		break;
		case	5:
		{

			fun_Corr_Calculate(SDK_CAL.VTP_Buff1.S16,SDK_CAL.VTP_Buff2.S16,SDK_CAL.RES_Buff.U16);					

			//���������� eeprom ��	
			if(gu16v_Correction_K2.U16>9600&&gu16v_Correction_K2.U16<10400)	
			{
				fun_EEprom_Write(0,0x55);			
				fun_EEprom_Write(1,gu16v_Correction_K1.byte.byte0);
				fun_EEprom_Write(2,gu16v_Correction_K1.byte.byte1);
				fun_EEprom_Write(3,gu16v_Correction_K2.byte.byte0);
				fun_EEprom_Write(4,gu16v_Correction_K2.byte.byte1);
				fun_LCD_Display_Success();	
			}
			else
			{
				fun_LCD_Display_Fail();
			}
				gu8v_CorrMode++;
			break;	
		}
		case 	6:
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;
				GOTO_SCAN_MODE();
				fun_LCD_FULL(0);
				gbv_1s	=	1;
				gbv_Flag_GetNTC	= 0;
				gu8v_CorrMode = 0;
			}
			
		break;

		default: break;
	}
	
}


u8 fun_get_NTC()		//��ȡNTC�¶ȣ������¶ȣ�
{
	switch(SDK_SYS.PublicMode)
	{
		case 0:
		{	
			fun_TEMP1_INIT();		//��ʼ���¶�1��
			SDK_SYS.PublicMode++;
			break;
		}
		case 1:
		{
			if(gbv_adc_Success)		//�ȴ�ADת���ɹ�û��
			{	
				SDK_CAL.ADC_Res1.S16 = SDK_ADC.AdcData.S32;	
				fun_ADC_Disable();
				
				fun_TEMP2_INIT();		//��ʼ���¶�2��
				SDK_SYS.PublicMode ++;	
			}
			break;
		}
		case 2:
		{
			if(gbv_adc_Success)
			{	
				SDK_CAL.ADC_Res2.S16 = SDK_ADC.AdcData.S32;	

			// �õ�0�c ��·VTP
				fun_VTP_INIT();			//��ʼ��VTP����
				SET_OPA_ZERO();			//����OPA�����
			// �ГQ����һ����B
				SDK_SYS.PublicMode ++;	
			}
			break;
		}	
		case 3: 
		{
			if(gbv_adc_Success)
			{	
				SDK_CAL.ADC_VtpZero.S16 = SDK_ADC.AdcData.S32;			
				
				fun_BAT_INIT();
				SDK_SYS.PublicMode ++;

				return	1;
			}
			break;
		}
		case 4: 
		{
			if(gbv_adc_Success)
			{	
				//	��ص�ѹ����
				
				SDK_CAL.BAT_Buff.S16 = SDK_ADC.AdcData.S32;			
				fun_ADC_Disable();		//����ADC
				
				SDK_SYS.PublicMode = 0;

				return	1;
			}
			break;
		}
		default:break;
	}	
	return 0;
}


u8	fun_get_VTP()  //��ȡVTP�¶�
{
	switch(SDK_SYS.PublicMode)
	{
		case	0:
		{
			fun_VTP_INIT();
			SET_OPA_VTP();		
			SDK_SYS.PublicMode ++;					
			break;
		}
		case	1:
		if(gbv_adc_Success)
		{	
			SDK_SYS.PublicMode 	= 0;
			SDK_CAL.ADC_Vtp.S16 	= SDK_ADC.AdcData.S32 - SDK_CAL.ADC_VtpZero.S16;

			return 1;	
		}
	}
	return	0;
}



u8	fun_get_VTP_CORR()  //��ȡVTPУ׼��Ĳ���
{
	switch(SDK_SYS.PublicMode)
	{
		case	0:
		{
			fun_VTP_INIT();
			SET_OPA_ZERO();		
			SDK_SYS.PublicMode ++;					
			break;
		}
		case	1:
		if(gbv_adc_Success)
		{	

			SDK_CAL.ADC_VtpZero.S16 = SDK_ADC.AdcData.S32;

			SET_OPA_VTP();				
			fun_VTP_INIT();
			SDK_SYS.PublicMode ++;	
			break;	
		}
		case	2:
		if(gbv_adc_Success)
		{
			SDK_SYS.PublicMode = 0;	
			SDK_CAL.ADC_Vtp.S16 = SDK_ADC.AdcData.S32  - SDK_CAL.ADC_VtpZero.S16;
			return 1; 	
		}
		break;	
		default: break;	
	}
	return	0;
}


