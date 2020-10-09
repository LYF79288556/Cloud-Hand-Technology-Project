#include "Upper_Config.h"
#include "Flash_Dis.h"			//Flash�ļ�

uint8_t	String_DTUMode[]					= "AT+DTUMode=";
uint8_t	String_FindDTUMode[]			= "AT+DTUMode?";

uint8_t	Reply_DTUMode[]						= "+DTUMode:";
uint8_t	Reply_FindDTUMode[]				= "+DTUModeState:";

uint8_t	String_DTUDebug[]					= "AT+DTUDebug=";
uint8_t	String_FindDTUDebug[]			= "AT+DTUDebug?";

uint8_t	Reply_DTUDebug[]					= "+DTUDebug:";
uint8_t	Reply_FindDTUDebug[]			= "+DTUDebug?:";

/************************************************************************************
ESP32��ESP8266ָ���ǹ��õ�
************************************************************************************/
/*************************��λ��->��Ƭ����ǰ׺��*************************************/
uint8_t	String_DTUWFMode[] 				= "AT+DTUWFMode=";
uint8_t	String_FindDTUWFMode[] 		= "AT+DTUWFMode?";
uint8_t	String_DTUWFJAP[] 				= "AT+DTUWFJAP=";
uint8_t	String_FindDTUWFJAP[] 		= "AT+DTUWFJAP?";
uint8_t	String_DTUWFSAP[] 				= "AT+DTUWFSAP=";
uint8_t	String_FindDTUWFSAP[] 		= "AT+DTUWFSAP?"; 
uint8_t	String_DTUWFSAPSet[] 			= "AT+DTUCIPSTART=";
uint8_t	String_FindDTUWFSAPSet[] 	= "AT+DTUCIPSTART?";
/*************************��Ƭ��->��λ����ǰ׺��*************************************/
uint8_t	Reply_DTUWFMode[] 				= "+DTUWFMode:";
uint8_t	Reply_FindDTUWFMode[] 		= "+DTUWFMode?:";
uint8_t	Reply_DTUWFJAP[] 					= "+DTUWFJAP:";
uint8_t	Reply_FindDTUWFJAP[] 			= "+DTUWFJAP?:";
uint8_t	Reply_DTUWFSAP[] 					= "+DTUWFSAP:";
uint8_t	Reply_FindDTUWFSAP[] 			= "+DTUWFSAP?:";
uint8_t	Reply_DTUWFSAPSet[] 			= "+DTUWFSAPSet:";
uint8_t	Reply_FindDTUWFSAPSet[] 	= "+DTUWFSAPSet?:";
/***********************************************************************************/

uint8_t  Upper_SetOK[] ="1";
uint8_t  Upper_SetErr[] ="0";



char ESP32_FlashReadTest[50] = "";

uint8_t  Upper_i;
uint8_t  Upper_State;
char  Upper_Rx_Buffer[Uart1_BUFFER_SIZE];

   

extern	uint8_t Uart1_Rx_Len;
extern  uint8_t Uart1_Recv_End_Flag;
extern  uint8_t Uart1_Rx_Buffer[Uart1_BUFFER_SIZE];
extern  uint8_t Debug_f;

Upper_STATE_bit Upper_S_bit;

struct 	Upper_Variable 		 Upper_Val;

struct  Upper_WFVariable 	 Upper_ESP32Val;	

/******************************************************************************************************************************************************************************
** ������:void  Upper_RX_Dis(void)
** ��������:��STM32�Ĵ���2���յ������ݽ��д���
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void  Upper_RX_Dis(void)
{
		if(Uart1_Recv_End_Flag == 1)
    {
				DEBUG("rx_len = %d\r\n",Uart1_Rx_Len);				//��ӡ���ճ���
				if((Uart1_Rx_Buffer[Uart1_Rx_Len-2] == '\r')&&(Uart1_Rx_Buffer[Uart1_Rx_Len-1] == '\n'))   	//�ж��ǻ��лس���β
				{
					for(Upper_i=0;Upper_i<(Uart1_Rx_Len-2);Upper_i++)
					{
					Upper_Rx_Buffer[Upper_i] = Uart1_Rx_Buffer[Upper_i];
					}
						DEBUG("���յ����ݣ�%s\r\n",Upper_Rx_Buffer);	//��ӡ��������
					if(memcmp(String_DTUMode, Upper_Rx_Buffer, sizeof(String_DTUMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUMode_f = 1;
							strncpy(Upper_Val.Upper_SetMode,Upper_Rx_Buffer+sizeof(String_DTUMode)-1,sizeof(Upper_Val.Upper_SetMode));	
							strcpy(Upper_Val.Upper_SetMode, strtok(Upper_Val.Upper_SetMode,","));
							DEBUG("����DTUģʽ:%s\r\n",Upper_Val.Upper_SetMode);
					}
					else if(memcmp(String_FindDTUMode, Upper_Rx_Buffer, sizeof(String_FindDTUMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUMode_f = 1;
							DEBUG("��ѯDTU��ǰģʽ\r\n");
					}
					else if(memcmp(String_DTUDebug, Upper_Rx_Buffer, sizeof(String_DTUDebug)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUDebug_f = 1;
							DEBUG("����Debug״̬\r\n");
					}
					else if(memcmp(String_FindDTUDebug, Upper_Rx_Buffer, sizeof(String_FindDTUDebug)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUDebug_f = 1;
							DEBUG("��ѯDebug��ǰ״̬\r\n");
					}
					else if(memcmp(String_DTUWFMode, Upper_Rx_Buffer, sizeof(String_DTUWFMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFMode_f = 1;
							DEBUG("����WIFI��ǰģʽ\r\n");		
					}
					else if(memcmp(String_FindDTUWFMode, Upper_Rx_Buffer, sizeof(String_FindDTUWFMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFMode_f = 1;
							DEBUG("��ѯWIFI��ǰģʽ\r\n");
					}
					else if(memcmp(String_DTUWFJAP, Upper_Rx_Buffer, sizeof(String_DTUWFJAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFJAP_f = 1;
							DEBUG("��������APָ��\r\n");
					}
					else if(memcmp(String_FindDTUWFJAP, Upper_Rx_Buffer, sizeof(String_FindDTUWFJAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFJAP_f = 1;
							DEBUG("��ѯAP״ָ̬��\r\n");
					}
					else if(memcmp(String_DTUWFSAP, Upper_Rx_Buffer, sizeof(String_DTUWFSAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFSAP_f = 1;
							DEBUG("һ������SoftAP����\r\n");
					}
					else if(memcmp(String_FindDTUWFSAP, Upper_Rx_Buffer, sizeof(String_FindDTUWFSAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFSAP_f = 1;
							DEBUG("��ѯSoftAP����\r\n");
					}
					else if(memcmp(String_DTUWFSAPSet, Upper_Rx_Buffer, sizeof(String_DTUWFSAPSet)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFSAPSet_f = 1;
							DEBUG("�������ӷ�ʽ��IP�Ͷ˿ں�\r\n");
					}
					else if(memcmp(String_FindDTUWFSAPSet, Upper_Rx_Buffer, sizeof(String_FindDTUWFSAPSet)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFSAPSet_f = 1;
							DEBUG("��ѯ���ӷ�ʽ��IP�Ͷ˿ں�\r\n");
					}
				}
				Uart1_Rx_Len=0;					//�������
        Uart1_Recv_End_Flag=0;	//������ս�����־λ
			
			  memset(Uart1_Rx_Buffer,0,sizeof(Uart1_Rx_Buffer));	
		 }
}


/******************************************************************************************************************************************************************************
** ������:void   Upper_State_Dis(void)
** ��������:��λ������״̬��
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Upper_State_Dis(void)
{
	Upper_RX_Dis();
	Upper_Find_Dis(&Upper_Val);
	Upper_Set_Dis(&Upper_Val);
	
	switch(Upper_Val.Upper_State)
	{
		case	Upper_FreeMode:
						if(strcmp(Upper_Val.Upper_SetMode,"2") == 0)
						{
							Upper_Val.Upper_State = Upper_ESP32Mode;
							DEBUG("����ESP32ģʽ\r\n");
						}
		break;
		case		Upper_ESP32Mode:
						Upper_ESP32_ValDis(&Upper_ESP32Val);
						Upper_WFFind_ValDis(&Upper_ESP32Val);
		break;
	}
		Upper_S_bit.Upper_Byte = 0;
		memset(Upper_Rx_Buffer,0,sizeof(Upper_Rx_Buffer));
}

/******************************************************************************************************************************************************************************
** ������:void   Upper_Find_Dis(void)
** ��������:��λ�����û�û����ģʽ֮ǰ�Ĳ�ѯ��������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Upper_Find_Dis(struct Upper_Variable *Upper_Val)
{
	if(Upper_S_bit.Upper_f.Upper_FindDTUMode_f)
	{
		if(strlen(Upper_Val->Upper_SetMode) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
		{
		strcpy(Upper_Val->Upper_SetMode, "0"); 
		}
		if(strlen(Upper_Val->Upper_ModuleBrand) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
		{
		strcpy(Upper_Val->Upper_ModuleBrand, "0"); 
		}
		Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUMode,Upper_Val->Upper_SetMode,Upper_Val->Upper_ModuleBrand);	
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUDebug_f)
	{
		if(strlen(Upper_Val->Upper_Debug) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
		{
		strcpy(Upper_Val->Upper_Debug, "0"); 
		}
		Print(&huart1,"%s%s\r\n",Reply_FindDTUDebug,Upper_Val->Upper_Debug);	
	}
}
/******************************************************************************************************************************************************************************
** ������:void   Upper_Find_Dis(void)
** ��������:��λ�����û�û����ģʽ֮ǰ�Ĳ�ѯ��������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Upper_Set_Dis(struct Upper_Variable *Upper_Val)
{
		if(Upper_S_bit.Upper_f.Upper_DTUMode_f)
		{
				if(strcmp(Upper_Val->Upper_SetMode,"2") == 0)
				{
					Print(&huart1,"+DTUMode:%s\r\n",Upper_SetOK);
//					DEBUG("����ESP32ģʽ\r\n");
				}
				else
				{
					Print(&huart1,"+DTUMode:%s\r\n",Upper_SetErr);
					DEBUG("��ģ��ֻ֧�ֽ���ESP32ģʽ\r\n");
				}
		}
		else if(Upper_S_bit.Upper_f.Upper_DTUDebug_f)
		{
		strncpy(Upper_Val->Upper_Debug,Upper_Rx_Buffer+sizeof(String_DTUDebug)-1,sizeof(Upper_Val->Upper_Debug));
//		DEBUG("����Debug״̬:%s\r\n",Upper_Val->Upper_Debug);

/*********************************************************************************************************
��ֹд��������洢����һ�µ��·���������д������֮ǰ�ȶ����ݽ����ж�
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_Val->Upper_Debug) != 0)
		{
				//д���Ӧ�ĵ�ַ������
				Flash_WriteData(SetDebug_Add,Upper_Val->Upper_Debug,sizeof(Upper_Val->Upper_Debug));
		}
		else
		{
				DEBUG("д��������ͬ���������Flash\r\n");
		}
/********************************************************************************************************/

/*********************************************************************************************************
�ж��Ƿ�д��ɹ����д��ɹ��򷵻���Ӧ����Ϣ
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
		
	Print(&huart1,"��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
		if(strcmp(ESP32_FlashReadTest,Upper_Val->Upper_Debug) == 0)
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUDebug,Upper_SetOK);
			if(strcmp("0",Upper_Val->Upper_Debug) == 0)
			{
					Debug_f = 0;
			}
			else if(strcmp("1",Upper_Val->Upper_Debug) == 0)
			{
					Debug_f = 1;
			}
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUDebug,Upper_SetErr);
		}
/********************************************************************************************************/
		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	
}
/******************************************************************************************************************************************************************************
** ������:void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
** ��������:��λ�����ý���ESP32ģʽ��,����Ҫ���Ҷ�Ӧ��ATָ����д���
** �������:Upper_WFVariable
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
{
	if(Upper_S_bit.Upper_f.Upper_FindDTUWFMode_f)
	{
		if(strlen(Upper_WFVal->Upper_SetWFMode) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
		{
		strcpy(Upper_WFVal->Upper_SetWFMode, "0"); 
		}

			Print(&huart1,"%s%s\r\n",Reply_FindDTUWFMode,Upper_ESP32Val.Upper_SetWFMode);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFJAP_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFJAP_SSID) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFJAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFJAP_PWD) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFJAP_PWD, "0"); 
			}
			Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUWFJAP,Upper_WFVal->Upper_SetWFJAP_SSID,Upper_WFVal->Upper_SetWFJAP_PWD);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFSAP_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFSAP_SSID) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFSAP_PWD) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
		Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUWFSAP,Upper_WFVal->Upper_SetWFSAP_SSID,Upper_WFVal->Upper_SetWFSAP_PWD);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFSAPSet_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_Type) == NULL)				//�ж��ַ���Upper_SetWFJAP_SSID�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
		Print(&huart1,"%s%s,%s,%s\r\n",Reply_FindDTUWFSAPSet,Upper_WFVal->Upper_SetWFCIPSTART_Type,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort);
	}
}

/******************************************************************************************************************************************************************************
** ������:void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
** ��������:��λ�����ý���ESP32ģʽ�󣬶���Ҫ�洢�����ݽ��д洢�ʹ���
** �������:Upper_WFVariable
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Upper_ESP32_ValDis(struct Upper_WFVariable *Upper_WFVal)
{
	if(Upper_S_bit.Upper_f.Upper_DTUWFMode_f)
	{
		strncpy(Upper_WFVal->Upper_SetWFMode,Upper_Rx_Buffer+sizeof(String_DTUWFMode)-1,sizeof(Upper_WFVal->Upper_SetWFMode));
		DEBUG("����WIFIģʽ:%s\r\n",Upper_WFVal->Upper_SetWFMode);

/*********************************************************************************************************
��ֹд��������洢����һ�µ��·���������д������֮ǰ�ȶ����ݽ����ж�
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFMode_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFMode) != 0)
		{
				//д���Ӧ�ĵ�ַ������
				Flash_WriteData(SetWFMode_Add,Upper_WFVal->Upper_SetWFMode,sizeof(Upper_WFVal->Upper_SetWFMode));
		}
		else
		{
				DEBUG("д��������ͬ���������Flash\r\n");
		}
/********************************************************************************************************/

/*********************************************************************************************************
�ж��Ƿ�д��ɹ����д��ɹ��򷵻���Ӧ����Ϣ
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFMode_Add,ESP32_FlashReadTest);
		
		DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFMode) == 0)
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFMode,Upper_SetOK);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFMode,Upper_SetErr);
		}
/********************************************************************************************************/
		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFJAP_f)
	{
		strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFJAP)-1,sizeof(Upper_Rx_Buffer));
	
		strcpy(Upper_WFVal->Upper_SetWFJAP_SSID, strtok(Upper_Rx_Buffer,","));	//��ȡSSID
		strcpy(Upper_WFVal->Upper_SetWFJAP_PWD, strtok(NULL,"\r\n"));						//��ȡPWD
	
		DEBUG("AP_SSID:%s\r\n",Upper_WFVal->Upper_SetWFJAP_SSID);
		DEBUG("AP_PWD:%s\r\n",Upper_WFVal->Upper_SetWFJAP_PWD);
/*********************************************************************************************************
��ֹд��������洢����һ�µ��·���������д������֮ǰ�ȶ����ݽ����ж�
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFJAP_SSID_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_SSID) == 0)						//�ж������APģʽ��SSID��洢����ͬ
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFJAP_PWD_Add,ESP32_FlashReadTest);
			
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_PWD) != 0)				//�ж������APģʽ��PWD��洢�Ĳ���ͬ
				{
					//д���Ӧ�ĵ�ַ������
					Flash_WriteData_2(SetWFJAP_SSID_Add,SetWFJAP_PWD_Add,
														Upper_WFVal->Upper_SetWFJAP_SSID,sizeof(Upper_WFVal->Upper_SetWFJAP_SSID),
														Upper_WFVal->Upper_SetWFJAP_PWD,sizeof(Upper_WFVal->Upper_SetWFJAP_PWD));
				}
				else
				{
					DEBUG("д��������ͬ���������Flash\r\n");
				}
		}
		else
		{
					//д���Ӧ�ĵ�ַ������
					Flash_WriteData_2(SetWFJAP_SSID_Add,SetWFJAP_PWD_Add,
														Upper_WFVal->Upper_SetWFJAP_SSID,sizeof(Upper_WFVal->Upper_SetWFJAP_SSID),
														Upper_WFVal->Upper_SetWFJAP_PWD,sizeof(Upper_WFVal->Upper_SetWFJAP_PWD));
		}
/********************************************************************************************************/
/*********************************************************************************************************
�ж��Ƿ�д��ɹ����д��ɹ��򷵻���Ӧ����Ϣ
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFJAP_SSID_Add,ESP32_FlashReadTest);
		
		DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_SSID) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFJAP_PWD_Add,ESP32_FlashReadTest);
			
			DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_PWD) == 0)
			{
					Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetOK);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFSAP_f)
	{
			strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFSAP)-1,sizeof(Upper_Rx_Buffer));
		
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, strtok(Upper_Rx_Buffer,","));			//��ȡSSID
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, strtok(NULL,"\r\n"));								//��ȡPWD
		
			DEBUG("SoftAP_SSID:%s\r\n",Upper_WFVal->Upper_SetWFSAP_SSID);
			DEBUG("SoftAP_PWD:%s\r\n",Upper_WFVal->Upper_SetWFSAP_PWD);
/*********************************************************************************************************
��ֹд��������洢����һ�µ��·���������д������֮ǰ�ȶ����ݽ����ж�
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFSAP_SSID_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_SSID) == 0)						//�ж������APģʽ��SSID��洢����ͬ
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFSAP_PWD_Add,ESP32_FlashReadTest);
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_PWD) != 0)				//�ж������APģʽ��PWD��洢�Ĳ���ͬ
				{
					//д���Ӧ�ĵ�ַ������
						Flash_WriteData_2(SetWFSAP_SSID_Add,SetWFSAP_PWD_Add,
															Upper_WFVal->Upper_SetWFSAP_SSID,sizeof(Upper_WFVal->Upper_SetWFSAP_SSID),
															Upper_WFVal->Upper_SetWFSAP_PWD,sizeof(Upper_WFVal->Upper_SetWFSAP_PWD));
				}
				else
				{
					DEBUG("д��������ͬ���������Flash\r\n");
				}
		}
		else
		{
					//д���Ӧ�ĵ�ַ������
						Flash_WriteData_2(SetWFSAP_SSID_Add,SetWFSAP_PWD_Add,
															Upper_WFVal->Upper_SetWFSAP_SSID,sizeof(Upper_WFVal->Upper_SetWFSAP_SSID),
															Upper_WFVal->Upper_SetWFSAP_PWD,sizeof(Upper_WFVal->Upper_SetWFSAP_PWD));
		}
/********************************************************************************************************/
/*********************************************************************************************************
�ж��Ƿ�д��ɹ����д��ɹ��򷵻���Ӧ����Ϣ
*********************************************************************************************************/	
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFSAP_SSID_Add,ESP32_FlashReadTest);
		
		DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_SSID) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFSAP_PWD_Add,ESP32_FlashReadTest);
			
			DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_PWD) == 0)
			{
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetOK);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFSAPSet_f)
	{
			strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFSAPSet)-1,sizeof(Upper_Rx_Buffer));
		
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_Type, 			strtok(Upper_Rx_Buffer,","));			//��ȡ���ӷ�ʽ
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP, 	strtok(NULL,","));								//��ȡIP
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort, strtok(NULL,"\r\n"));						//��ȡ�˿�
		
			DEBUG("���ӷ�ʽ:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_Type);
			DEBUG("IP:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP);
			DEBUG("�˿�:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_RemotePort);

/*********************************************************************************************************
��ֹд��������洢����һ�µ��·���������д������֮ǰ�ȶ����ݽ����ж�
*********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFCIPSTART_Type_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_Type) == 0)								//�ж����������ģʽ��洢����ͬ
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFCIPSTART_RemoteIP_Add,ESP32_FlashReadTest);
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == 0)				//�ж�����IP��洢����ͬ
				{
					memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
					Flash_ReadData(SetWFCIPSTART_RemotePort_Add,ESP32_FlashReadTest);
					if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) != 0)		//�ж�����˿���洢�Ĳ���ͬ
					{
						Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
															Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
															Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
															Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
					}
					else
					{
						DEBUG("д��������ͬ���������Flash\r\n");
					}
				}
				else
				{
					Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
														Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
														Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
														Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
				}
		}
		else
		{
					//д���Ӧ�ĵ�ַ������
						Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
														Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
														Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
														Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
		}
/********************************************************************************************************/
/*********************************************************************************************************
�ж��Ƿ�д��ɹ����д��ɹ��򷵻���Ӧ����Ϣ
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFCIPSTART_Type_Add,ESP32_FlashReadTest);
		
		DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_Type) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFCIPSTART_RemoteIP_Add,ESP32_FlashReadTest);
			
			DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == 0)
			{
					memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
					Flash_ReadData(SetWFCIPSTART_RemotePort_Add,ESP32_FlashReadTest);
					
					DEBUG("��ӡ�����ݣ�%s\r\n",ESP32_FlashReadTest);
					if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) == 0)
									Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetOK);
					else
									Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		
	}
}
/******************************************************************************************************************************************************************************
** ������:void   DTU_Flash_GetData(uint32_t Writeaddr,char *data,char *Prefix)
** ��������:��ȡFlash�е�ָ����ַ������
** �������:	Writeaddr   ��ʼ��ַ
						data				����ָ��
						Prefix			��ӡ��ǰ׺
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   DTU_Flash_GetData(uint32_t Writeaddr,char *data,char *Prefix)
{
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(Writeaddr,ESP32_FlashReadTest);
		strcpy(data,ESP32_FlashReadTest);
		DEBUG("��ַ��0x%x,%s��%s\r\n",Writeaddr,Prefix,data);
}

/******************************************************************************************************************************************************************************
** ������:void   DTU_ReadFlash_SetData(struct Upper_WFVariable *Upper_WFVal)
** ��������:��ȡ����ǰ͸�������ò���ӡ
** �������:	Upper_WFVal   �洢�ṹ��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   DTU_ReadFlash_SetData(void)
{
	memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
	strcpy(Upper_Val.Upper_Debug,ESP32_FlashReadTest);
	if(strcmp("0",Upper_Val.Upper_Debug) == 0)
	{
			Debug_f = 0;
	}
	else if(strcmp("1",Upper_Val.Upper_Debug) == 0)
	{
			Debug_f = 1;
	}
	
	DTU_Flash_GetData(SetDebug_Add,Upper_Val.Upper_Debug,"Debug״̬:");
	DTU_Flash_GetData(SetWFMode_Add,Upper_ESP32Val.Upper_SetWFMode,"WIFIģʽ:");
	
	DTU_Flash_GetData(SetWFJAP_SSID_Add,Upper_ESP32Val.Upper_SetWFJAP_SSID,"APģʽSSID:");
	DTU_Flash_GetData(SetWFJAP_PWD_Add,Upper_ESP32Val.Upper_SetWFJAP_PWD,"APģʽPWD:");
	
	DTU_Flash_GetData(SetWFSAP_SSID_Add,Upper_ESP32Val.Upper_SetWFSAP_SSID,"SoftAPģʽSSID:");
	DTU_Flash_GetData(SetWFSAP_PWD_Add,Upper_ESP32Val.Upper_SetWFSAP_PWD,"SoftAPAPģʽPWD:");
	
	DTU_Flash_GetData(SetWFCIPSTART_Type_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_Type,"���ӵ�ģʽ:");
	DTU_Flash_GetData(SetWFCIPSTART_RemoteIP_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_RemoteIP,"�������˵�IP:");
	DTU_Flash_GetData(SetWFCIPSTART_RemotePort_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_RemotePort,"�������˵Ķ˿ں�:");
	

}

