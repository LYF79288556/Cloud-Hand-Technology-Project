
#ifndef __ESP32_Config_H
#define __ESP32_Config_H

#include "main.h"
#include "DTU_SYS_Dis.h"
#include "Upper_Config.h"

 struct	ESP32_STATE_Dep		//����15����־λ
{
/**************************************
ESP32�ĵĽ��ձ�־λ
**************************************/
	uint8_t	ESP32_State;
	uint8_t	ESP32_PassthroughState;
	uint8_t	ESP32_Passthrough_ErrFlag;
	uint8_t	ESP32_SetModeState;
/*************************************/

};

/***********************************************************
DTU״̬��ESP32�б�
************************************************************/
enum   DTU_State_ESP32List
{
	DTU_ESP32FreeMode = 0,				//ϵͳ����ģʽ
	DTU_ESP32ConfigUpperMode,			//ϵͳ��λ������ģʽ
	DTU_ESP32PassthroughMode,			//ϵͳ͸��ģʽ
	DTU_ESP32AnalyticalModel,			//ϵͳ����ģʽ
};

/***********************************************************
ESP32͸��״̬�б�
************************************************************/
enum   ESP32_PassthroughModeList
{
	ESP32_PassthroughFreeMode = 0,				//ϵͳ����ģʽ
	ESP32_PassthroughDetMode,							//ϵͳ�洢���ģʽ
	ESP32_PassthroughSetMode,							//ESP32͸������ģʽ
	ESP32_PassthroughErrMode,							//ESP32����ģʽģʽ
};

//void  ESP32_State_Dis(uint8_t ESP32_StateIng);
//void  ESP32_DataDis(void);
//void  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set);
#endif /* __ESP32_Config_H */
