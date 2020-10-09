#include "DTU_SYS_Dis.h"
#include "LED_Dis.h"
#include "Upper_Config.h"

uint8_t	ESP32_ATReady[]					= "\r\nready";
uint8_t	ESP32_ATOK[]						= "\r\nOK";
uint8_t	ESP32_ATERROR[]					= "\r\nERROR";





uint8_t	ESP32_ATRST[]					= "AT+RST";

uint8_t  ESP32_ATE_f;

uint8_t  ESP32_i;
uint8_t  ESP32_Data_Len;
uint8_t  ESP32_Rx_Buffer[Uart1_BUFFER_SIZE];

extern	uint8_t Uart2_Rx_Len;
extern  uint8_t Uart2_Recv_End_Flag;
extern  uint8_t Uart2_Rx_Buffer[Uart1_BUFFER_SIZE];

ESP32_STATE_bit ESP32_S_bit;

struct	ESP32_STATE_Dep    ESP32_STATE_D;

uint8_t KEY = 0;	//��ȡ����״̬


/******************************************************************************************************************************************************************************
** ������:void Print_Debug_mess(unsigned char *debug_Mess)
** ��������:��ӡ���������Ϣ
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void Print_Debug_mess(unsigned char *debug_Mess)
{
    HAL_UART_Transmit(&huart1, debug_Mess, 1, 100);
}

/******************************************************************************************************************************************************************************
** ������:void key_S_CallBack(void)
** ��������:��������������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void key_S_CallBack(void)	//����
{

    if(ESP32_STATE_D.ESP32_State != DTU_ConfigurationMode)
    {
        ESP32_STATE_D.ESP32_State++;

        if(ESP32_STATE_D.ESP32_State > DTU_DataAnalysisMode)
        {
            ESP32_STATE_D.ESP32_State = DTU_FreeMode;
        }
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_ConfigurationMode)
    {
        ESP32_STATE_D.ESP32_State = DTU_FreeMode;
    }

    DEBUG("��������\r\n");
}
/******************************************************************************************************************************************************************************
** ������:void key_L_CallBack(void)
** ��������:������������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void key_L_CallBack(void)	//����
{
    if(ESP32_STATE_D.ESP32_State != DTU_ConfigurationMode)	//������������ģʽ
    {
        ESP32_STATE_D.ESP32_State = DTU_ConfigurationMode;
    }

    DEBUG("��������\r\n");
}

/******************************************************************************************************************************************************************************
** ������:void Update_Key_CallBack(void)
** ��������:���°���״̬
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void Update_Key_CallBack(void)
{
    KEY = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);	//��ȡ����״̬
}

/******************************************************************************************************************************************************************************
** ������:void  KEY_Ready(void)
** ��������:���������ú���
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void  KEY_Ready(void)
{
    //��ʼ�������ṹ��
    Init_Key_Struct(Update_Key_CallBack, Print_Debug_mess);

    Reg_Key(&KEY, 10,  L_Trig, S_Click, key_S_CallBack);//����
    Reg_Key(&KEY, 100, L_Trig, L_Press, key_L_CallBack);//����
}
/******************************************************************************************************************************************************************************
** ������:void  DTU_State_Dispose(void)
** ��������:DTU��״̬������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void  DTU_State_Dispose(void)
{
    /*********************************************************************
    ������������ȥ���õ��Ƕ�ʱ��ȥ����
    *********************************************************************/
    if(!Time_KEY_Cnt)
    {
        Time_KEY_Cnt = 5;
        Key_Detect();
    }

    /*******************************************************************/

    /*********************************************************************
    ϵͳ״̬��ӦLED��
    *********************************************************************/
    if(ESP32_STATE_D.ESP32_State == DTU_FreeMode)//ϵͳ����ģʽ����ʾ
    {
        LED_SetColor(0, 1, 0);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_PassthroughMode)
    {
        LED_SetColor(1, 0, 0);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_DataAnalysisMode)
    {
        LED_SetColor(0, 1, 1);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_ConfigurationMode)
    {
        LED_SetColor(0, 0, 1);
    }

    /*******************************************************************/

    if(!ESP32_ATE_f)
    {
        ESP32_ATE_f = 1;
        Print(&huart2, "ATE0\r\n");		//�������͹رջ���
    }

    ESP32_STATE_D.ESP32_State = DTU_ConfigurationMode;


    ESP32_DataDis();									//ESP32���ݿڽ������ݲ�����

    switch(ESP32_STATE_D.ESP32_State)
    {
        case DTU_FreeMode:					//ϵͳ����ģʽ
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else
            {
                Print(&huart2, "AT\r\n");
                DEBUG("ģ�鲻����\r\n");
            }

            break;

        case DTU_ConfigurationDetMode:		//ϵͳ�洢���ü��ģʽ

            if(ESP32_PassthroughDet(&Upper_ESP32Val) == ESP32_OK)
            {
                DEBUG("��⵽�洢��������Ϣ��ȫ\r\n");
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else if(ESP32_PassthroughDet(&Upper_ESP32Val) == ESP32_ERR)
            {
                DEBUG("��⵽�洢��������Ϣ�пյ�\r\n");
                DEBUG("���Գ�����������ģʽ\r\n");
                DEBUG("���óɹ���̰��˳�����ģʽ\r\n");
                DEBUG("�˳�����ģʽ���Զ���������͸��ģʽ\r\n");
                ESP32_STATE_D.ESP32_State = DTU_ERRORMode;
            }

            break;

        case DTU_ConfigurationSetMode:		//ϵͳ��������ģʽ

            if(ESP32_SetMode_Dis(&Upper_ESP32Val) == ESP32_OK)
            {
                DEBUG("������ɽ���͸��ģʽ\r\n");
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else if(ESP32_SetMode_Dis(&Upper_ESP32Val) == ESP32_ERR)
            {
                DEBUG("���ù����д�������\r\n");
                DEBUG("���Գ�����������ģʽ\r\n");
                DEBUG("���óɹ���̰��˳�����ģʽ\r\n");
                DEBUG("�˳�����ģʽ���Զ���������͸��ģʽ\r\n");
                ESP32_STATE_D.ESP32_State = DTU_ERRORMode;
            }

            break;

        case DTU_PassthroughMode:					//ϵͳ͸��ģʽ

            break;

        case DTU_DataAnalysisMode:				//ϵͳ͸�����ݽ���ģʽ

            break;

        case DTU_ConfigurationMode: 			//ϵͳ����ģʽ
            Upper_State_Dis();

            break;

        case DTU_ERRORMode:								//ϵͳ����ģʽ

            break;
    }

    ESP32_S_bit.ESP32_Byte = 0;
    memset(ESP32_Rx_Buffer, 0, sizeof(ESP32_Rx_Buffer));

}

/******************************************************************************************************************************************************************************
** ������:void  ESP32_DataDis(void)
** ��������:ESP32�����ݴ�����
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void  ESP32_DataDis(void)
{
    if(Uart2_Recv_End_Flag == 1)
    {
        if((Uart2_Rx_Buffer[Uart2_Rx_Len - 2] == '\r') && (Uart2_Rx_Buffer[Uart2_Rx_Len - 1] == '\n'))   	//�ж��ǻ��лس���β
        {
            for(ESP32_i = 0; ESP32_i < (Uart2_Rx_Len - 2); ESP32_i++)
            {
                ESP32_Rx_Buffer[ESP32_i] = Uart2_Rx_Buffer[ESP32_i];
            }

            DEBUG("���յ�������Ϊ��%s\r\n", ESP32_Rx_Buffer);

            if(memcmp(ESP32_ATRST, ESP32_Rx_Buffer, sizeof(ESP32_ATRST) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_READY_f = 1;
            }
            else if(memcmp(ESP32_ATOK, ESP32_Rx_Buffer, sizeof(ESP32_ATOK) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_OK_f = 1;
            }
            else if(memcmp(ESP32_ATERROR, ESP32_Rx_Buffer, sizeof(ESP32_ATERROR) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_ERROR_f = 1;
            }
        }

        Uart2_Rx_Len = 0;					//�������
        Uart2_Recv_End_Flag = 0;	//������ս�����־λ

        memset(Uart2_Rx_Buffer, 0, sizeof(Uart2_Rx_Buffer));
    }
}
/******************************************************************************************************************************************************************************
** ������:uint8_t  ESP32_PassthroughDet(struct Upper_WFVariable *ESP32_Val)
** ��������:ESP32��͸��ģʽ�����ݼ��
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
uint8_t  ESP32_PassthroughDet(struct Upper_WFVariable *ESP32_Val)
{
    if(strcmp(ESP32_Val->Upper_SetWFMode, "0") == 0)		//���ģʽ����Ϊ��Ϊ��
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("��Wi-Fiģʽ,���ҹر�Wi-Fi RF\r\n");
    }
    else
    {
        if(strcmp(ESP32_Val->Upper_SetWFMode, "1") == 0)
            DEBUG("��ǰģʽ���ã�Station ģʽ\r\n");
        else if(strcmp(ESP32_Val->Upper_SetWFMode, "2") == 0)
            DEBUG("��ǰģʽ���ã�SoftAP ģʽ\r\n");
        else if(strcmp(ESP32_Val->Upper_SetWFMode, "3") == 0)
            DEBUG("��ǰģʽ���ã�SoftAP+Station ģʽ\r\n");
    }

    if(strlen(ESP32_Val->Upper_SetWFJAP_PWD) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("��ǰAPģʽ��PWDΪ��\r\n");
    }
    else
        DEBUG("��ǰAPģʽ��PWDΪ��%s\r\n", ESP32_Val->Upper_SetWFJAP_PWD);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_Type) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("��ǰ��������Ϊ��\r\n");
    }
    else
        DEBUG("��ǰ��������Ϊ��%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_Type);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_RemoteIP) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("��ǰAPģʽ������IPΪ��\r\n");
    }
    else
        DEBUG("��ǰAPģʽ������IPΪ��%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_RemoteIP);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_RemotePort) == NULL)				//�ж��ַ���Upper_SetWFJAP_PWD�ǲ��ǿյ�
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("��ǰ���Ӷ˿ں�Ϊ��\r\n");
    }
    else
        DEBUG("��ǰ���Ӷ˿ں�Ϊ��%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_RemotePort);

    if(ESP32_STATE_D.ESP32_Passthrough_ErrFlag == 1)
    {
        return ESP32_ERR;
    }

    return ESP32_OK;
}

/******************************************************************************************************************************************************************************
** ������:uint8_t  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set)
** ��������:ESP32������ģʽ��ָ������
** �������:��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
uint8_t  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set)
{
    switch(ESP32_STATE_D.ESP32_SetModeState)
    {
        case 0:

            if(ESP32_S_bit.ESP32_f.ESP32_READY_f)
            {
                DEBUG("����ESP32��WIFIģʽ�ɹ�\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 1;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "%s\r\n", ESP32_ATRST);
            }

            break;

        case 1:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("����ESP32��WIFIģʽ�ɹ�\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 2;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
//						Print(&huart2,"AT+CWMODE=%d\r\n",ESP32_Val_Set->Upper_SetWFMode);
                Print(&huart2, "AT+CWMODE=3\r\n");

            }

            break;

        case 2:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("����ESP32��SSID��PWD�ɹ�\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 3;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CWJAP=\"%s\",\"%s\",%s\r\n", ESP32_Val_Set->Upper_SetWFJAP_SSID, ESP32_Val_Set->Upper_SetWFJAP_PWD);
            }

            break;

        case 3:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("����IP�������Ͷ˿�\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 4;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPSTART=\"%s\",\"%s\"\r\n", ESP32_Val_Set->Upper_SetWFCIPSTART_Type, ESP32_Val_Set->Upper_SetWFCIPSTART_RemoteIP, ESP32_Val_Set->Upper_SetWFCIPSTART_RemotePort);
            }

            break;

        case 4:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("׼������͸��ģʽ\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 5;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPMODE=1\r\n");

            }

            break;

        case 5:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("����͸��ģʽ\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 6;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPSEND\r\n");
                DEBUG("ģ�鲻����\r\n");
            }

            break;

        case 6:
            return ESP32_OK;
            break;

        case 7:
            return ESP32_ERR;
            break;

    }

    return ESP32_ERR;

}

