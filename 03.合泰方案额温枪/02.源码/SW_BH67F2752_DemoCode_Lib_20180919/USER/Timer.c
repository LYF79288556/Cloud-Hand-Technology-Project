
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	timer.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/


#include "common.h"

TIME_define	SDK_TM;

/********************************************************************
Function:	TIMER0_INIT
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/
void	fun_TimeInit()
{
	
	SET_CTM_F_SYSc4();			//��Ƶϵͳʱ��4��ƵԴ
	SET_CTM_P_384();			//����384��CTMnʱ������
	SET_CTM_MODE_PWM();			//����CTMn�Ĺ���ģʽΪPWM�������
	SET_PWM_P_PERIOD_A_DUTY();	//����CCRP �C ���ڣ�CCRA �C ռ�ձ�
	SET_TIEM_P_MATCH();			//����CTMn �Ƚ��� P ƥ��
	
	
//	����CCRA�ļĴ�������	
	_ctm1al = 128;		
	_ctm1ah = 0;
//	�� fSYS =8MHz��CTMn ʱ��Դѡ�� f SYS /4��CCRP=0��CCRA=129	
//	CTMn PWM���Ƶ��= (f SYS /4)/1024=1953Hz��duty=128/(2��128)=50%��

//	SET_CTM_ON();
//	_mf0e = 1;
//	_ctmae = 1;	
//	_emi = 1;
	
	SET_TBC_F_SUB();			//Ԥ��Ƶʱ��Դѡ��fsub��Ƶʱ��Դ
	SET_TB0_ON();				//ʹ�ܶ�ʱ��0;
	
//  ѡ��Ԥ��Ƶʱ��ԴΪ�ڲ�����ʱ�ӣ�32K��
//	��ƵĬ��Ϊ000��Ϊ2��8�η�
//	32K/2��8�η�=125Hz
//  1/125=0.008s���ж�

}	


/********************************************************************
Function:	func_mult_0_2timer_isr
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/

//void __attribute((interrupt(0x10)))  func_mult0_isr()
//{	
//	//t0 a match
//	if(_ctm0af == 1)
//	{
//		gbv_10ms = 1;
//	}
//	_ctm0af = 0;
//	_ctm0pf = 0;
//}


/********************************************************************
Function:	fun_SysTime_Scan
INPUT	:
OUTPUT	:	
NOTE	:   ϵ�y�r�Ӌ����8msӋ�rһ�Σ�
********************************************************************/

void	fun_SysTimeScan()
{			
	// 500msӋ��
	SDK_TM.Count1 ++;
	if(SDK_TM.Count1 >=62)
	{
		SDK_TM.Count1 		= 0;
		gbv_500ms 			= 1;
		gbv_500ms_reversal 	= ~gbv_500ms_reversal;		//��ת��־λ�����ø���Щ��Ҫ��˸�Ķ�����
		SDK_TM.HaltTime ++;
	}
	// 1SӋ��
	SDK_TM.Count2++;
	if(SDK_TM.Count2 >= 125)
	{
		SDK_TM.Count2 = 0;
		gbv_1s = 1;
	
	}
}




