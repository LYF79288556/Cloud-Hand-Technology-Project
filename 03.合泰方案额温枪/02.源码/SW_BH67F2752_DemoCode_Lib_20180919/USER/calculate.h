
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	calculate.h 	       	                                    **
//**	Description	: 																**
//**	MCU      	: 	xx															**
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _CALCULATE_H_
#define _CALCULATE_H_

//=========================================У���c�O��=======================================//
#define CORRTEMP_LOW    250
#define CORRTEMP_HIGH   370

//=========================================�ӿڱ���=========================================//
extern	volatile	__16_type   gu16v_Correction_K1;        //  У�ʂS��
extern	volatile	__16_type   gu16v_Correction_K2;

//=========================================�ӿں���=========================================//

/********************************************************************
*   Function:	fun_Rntc_Calculate
*   INPUT	:	lu16v_adc_res1		>>	�։����ADCֵ1
*   			lu16v_adc_res2 		>>	�։����ADCֵ2
*   OUTPUT	:	lu16v_res_ntc		>>	NTC ��ֵ
*   NOTE	:   �·�D����:	(ȡADC�Ľ^��ֵ)
*   			---  VCM
*   			 |	
*   			---
*   			| |	 NTC
*   			---
*                |   TEMP1 (adc_res1)
*               ---
*               | |  100K
*               ---
*                |   TEMP2 (adc_res2)
*               --- 
*               | |	 300K
*               ---
*                |
*               ---  GND
********************************************************************/
u16	    fun_Rntc_Calculate(u16 lu16v_adc_res1,u16 lu16v_adc_res2);
/********************************************************************
*	Function:	fun_Tsen_Calculate
*	INPUT	:	lu16v_res_ntc       >>  NTC����ֵ
*	OUTPUT	:	ls16v_Tntc_actual   >>  NTC���¶�
*	NOTE	:   NTC �����Ĝض�
********************************************************************/
s16		fun_Tsen_Calculate(u16 lu16v_res_ntc);
/********************************************************************
*	Function:	fun_Tobj_Calculate
*	INPUT	:	ls16v_temp_ntc		>>  NTC �ض�
*				ls16v_adc_vtp		>>  VTP ADCֵ
*	OUTPUT	:	ls16v_Tobj_Result 	>>  ���ض�
*	NOTE	:   ���ض�Ӌ��
********************************************************************/ 
s16	fun_Tobj_Calculate(s16 ls16v_temp_ntc,s16 ls16v_adc_vtp);
/********************************************************************
*   Function:	fun_Corr_Calculate
*   INPUT	:	ls16v_adc_CorrVtp1	>>	��һ��У���c��ADֵ
*   			ls16v_adc_CorrVtp2	>>	�ڶ���У���c��ADֵ
*   			lu16v_res_ntc		>>	NTC�����ֵ
*   OUTPUT	:	NONE
*   NOTE	:   ͨ�^У�ʃɂ��ض��c�@ȡK1��K2��ֵ
********************************************************************/
void	fun_Corr_Calculate(s16 ls16v_adc_CorrVtp1,s16 ls16v_adc_CorrVtp2,u16 lu16v_res_ntc);
/********************************************************************
*   Function:	fun_Temp_SurfaceToBody
*   INPUT	:	lu16v_ObjTemp		>>	���ض�	
*   OUTPUT	:	lu16v_BodyTemp		>>	���w�ض�
*   NOTE	:   
********************************************************************/
u16		fun_Temp_SurfaceToBody(u16 lu16v_ObjTemp);
/********************************************************************
*   Function:	fun_ObjTemp_DataHandle
*   INPUT	:	ls16v_ObjTemp		>>	���ض�	
*   OUTPUT	:	
*   NOTE	:   xx.xx ��C -- xx.x ��C
********************************************************************/
s16		fun_ObjTemp_DataHandle(s16 ls16v_ObjTemp);




#endif

