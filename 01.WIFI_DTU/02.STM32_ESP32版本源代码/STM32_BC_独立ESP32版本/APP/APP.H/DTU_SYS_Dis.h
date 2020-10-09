
#ifndef __DTU_SYS_Dis_H
#define __DTU_SYS_Dis_H

#include "main.h"
#include "KEY_Dispose.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "stm32f1xx_it.h"
#include "ESP32_Config.h"

#define  __DEBUG_VERSION  1 
//#define DEBUG(format,...)\
//				if(format)\
////							printf(__VA_ARGS__)
//						printf("[File:%s, Line:%d] "format, __FILE__, __LINE__, ##__VA_ARGS__)
			
				
				
				
//#if	 		__DEBUG_VERSION
//    #define DEBUG(format,...)  printf("[File:%s, Line:%d] "format, __FILE__, __LINE__, ##__VA_ARGS__)
//#else
//    #define DEBUG(format,...)
//#endif


 struct	ESP32_STATE_Store					//ϵͳ״̬�����洢
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

typedef struct	ESP32_STATE_FLAG				//
{
/**************************************
ESP32�ĵĽ��ձ�־λ
**************************************/
	uint8_t	ESP32_READY_f:1;
	uint8_t	ESP32_OK_f:1;
	uint8_t	ESP32_ERROR_f:1;
/*************************************/

}ESP32_STATE_flag;


typedef union   ESP32_STATE_BIT					//�����干���ڴ�
{
    uint32_t ESP32_Byte;
    ESP32_STATE_flag  ESP32_f;

}ESP32_STATE_bit;

/**********************
DTU״̬�б�
**********************/
enum   DTU_State_List
{
	DTU_FreeMode = 0,					//ϵͳ����ģʽ
	DTU_ConfigurationDetMode,	//ϵͳ�洢���ü��ģʽ
	DTU_ConfigurationSetMode,	//ϵͳ��������ģʽ
	DTU_PassthroughMode,			//ϵͳ͸��ģʽ
	DTU_DataAnalysisMode,			//ϵͳ͸�����ݽ���ģʽ
	DTU_ConfigurationMode,		//ϵͳ����ģʽ
	DTU_ERRORMode,						//ϵͳ����ģʽ
};

/**********************
ESP32��������״̬
**********************/
enum   ESP32_Return
{
	ESP32_OK = 0,					//ϵͳ����ģʽ
	ESP32_ERR,
};


void key_S_CallBack(void);	//����
void key_L_CallBack(void);	//����
void Update_Key_CallBack(void);
void KEY_Ready(void);
void DTU_State_Dispose(void);
uint8_t  ESP32_PassthroughDet(struct Upper_WFVariable *ESP32_Val);
void  ESP32_DataDis(void);
uint8_t  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set);

#endif /* __DTU_SYS_Dis_H */
