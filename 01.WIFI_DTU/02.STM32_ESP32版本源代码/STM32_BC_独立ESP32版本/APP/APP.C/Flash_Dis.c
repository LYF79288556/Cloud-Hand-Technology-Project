#include "Flash_Dis.h"


uint32_t WriteFlashData = 0x12345678;
uint32_t addr =  0x08011000;
uint32_t addr1 = 0x08012000;
uint32_t addr2 = 0x08013000;

char string_Test[] = "Hell world";
char string_Test1[] = "chenzhiwang";
char string_Test2[] = "chenzhiwang123";
char string_ReadTest[] = "";

/******************************************************************************************************************************************************************************
** ������:void writeFlashTest(void)
** ��������:FLASHд�����
** �������:	��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void writeFlashTest(void)
{
	/* 1/4����FLASH*/
	HAL_FLASH_Unlock();

	/* 2/4����FLASH*/
	/*��ʼ��FLASH_EraseInitTypeDef*/
	/*������ʽҳ����FLASH_TYPEERASE_PAGES�������FLASH_TYPEERASE_MASSERASE*/
	/*������ַ*/
	/*����ҳ��*/
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.PageAddress = addr;
	FlashSet.NbPages = 1;
	
	/*����PageError�����ò�������*/
	uint32_t PageError = 0;					//������ִ�����������ᱻ����Ϊ�����FLASH��ַ
	HAL_FLASHEx_Erase(&FlashSet, &PageError);

	/* 3/4��FLASH��д*/
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, WriteFlashData);

	/* 4/4��סFLASH*/
	HAL_FLASH_Lock();
}
/******************************************************************************************************************************************************************************
** ������:void printFlashTest(void)
** ��������:FLASH��ȡ��ӡ����
** �������:	��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void printFlashTest(void)
{
	uint32_t temp = *(__IO uint32_t*)(addr);

	DEBUG("addr is:0x%x, data is:0x%x\r\n", addr, temp);
}
/******************************************************************************************************************************************************************************
** ������:void   Flash_Test(void)
** ��������:Flash�Ĳ��Գ���
** �������:	��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void   Flash_Test(void)
{
	Flash_WriteData(addr,string_Test,sizeof(string_Test));
	Flash_ReadData(addr,string_ReadTest);
	DEBUG("���ݽ������\r\n");
	DEBUG("����Ϊ��%s\r\n",string_ReadTest);
	HAL_Delay(1000);
	Flash_WriteData(addr1,string_Test1,sizeof(string_Test1));
	Flash_ReadData(addr1,string_ReadTest);
	DEBUG("���ݽ������\r\n");
	DEBUG("����Ϊ��%s\r\n",string_ReadTest);
	HAL_Delay(1000);
	Flash_WriteData(addr2,string_Test2,sizeof(string_Test2));
	Flash_ReadData(addr2,string_ReadTest);
	DEBUG("���ݽ������\r\n");
	DEBUG("����Ϊ��%s\r\n",string_ReadTest);
}

uint32_t FLASH_ReadHalfWord(uint32_t faddr)
{
	return *(__IO uint32_t*)faddr; 
}

/******************************************************************************************************************************************************************************
** ������:void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
** ��������:��ָ����ַ��ʼ����ָ�����ȵ�����
** �������:	ReadAddr  		��ʼ��ַ
						*pBuffer			����ָ��
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void Flash_ReadData(uint32_t ReadAddr,char *pBuffer)   	
{
	uint16_t i = 0;
	
	while((uint8_t)FLASH_ReadHalfWord(ReadAddr) != 0xFF)
	{
		pBuffer[i]=(char)FLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
		i++;
	}
	
}
/******************************************************************************************************************************************************************************
** ������:void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
** ��������:д��һ����ַ,���ֱ�д��һ���ַ������ݺʹ洢�ĳ���
** �������:	Writeaddr1  ��ַ1
						*data1			����ָ��
						Size1				����ָ��ĳ���
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************************
//ע�������ڳ���ʹ������д��Ͳ�����������ܷɡ������ע�Լ�д�ĳ���ռ�λ�ã�
******************************************************************************************************************************************************************************/
void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
{
	
	if(Writeaddr<FLASH_BASE||((Writeaddr+Size)>=(FLASH_BASE+1024*FLASH_SIZE)))

{
	DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr);
	return;//�Ƿ���ַ

}
	DEBUG("������ַ:0x%x\r\n", Writeaddr);
	DEBUG("��ŵ�����:%s�����ȣ�%d\r\n", data,Size);
	HAL_FLASH_Unlock(); //����д����
    //1������FLASH
    HAL_FLASH_Unlock();

    //2������FLASH
    //��ʼ��FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr;
    f.NbPages = 1;
    //����PageError
    uint32_t PageError = 0;
    //���ò�������
    HAL_FLASHEx_Erase(&f, &PageError);
    //3����FLASH��д
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size ;i++)
    {
        TempBuf = (uint8_t)*(data+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr + i * 2, TempBuf);
    }
    //4����סFLASH
    HAL_FLASH_Lock();
}
/******************************************************************************************************************************************************************************
** ������:void Flash_WriteData_2(uint32_t Writeaddr1,uint32_t Writeaddr2,char *data1,uint16_t Size1,char *data2,uint16_t Size2)
** ��������:д��������ַ,��������ַ��Ҫ��Flash��ͬһҳ�����ֱ�д�������ַ������ݺʹ洢�ĳ���
** �������:	Writeaddr1  ��ʼ��ַ1
					  Writeaddr2	��ʼ��ַ2
						*data1			�ַ���1����ָ��
						*data2			�ַ���2����ָ��
						Size1				�ַ���1����ָ��ĳ���
						Size2				�ַ���2����ָ��ĳ���
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void Flash_WriteData_2(uint32_t Writeaddr1,uint32_t Writeaddr2,char *data1,uint16_t Size1,char *data2,uint16_t Size2)
{
	
	if(Writeaddr1<FLASH_BASE||((Writeaddr1+Size1)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr1);
		return;//�Ƿ���ַ

	}
	if(Writeaddr2<FLASH_BASE||((Writeaddr2+Size2)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr2);
		return;//�Ƿ���ַ

	}
	DEBUG("�ַ���1������ַ:0x%x\r\n", Writeaddr1);
	DEBUG("�ַ���2������ַ:0x%x\r\n", Writeaddr2);
	DEBUG("�ַ���1��ŵ�����:%s�����ȣ�%d\r\n", data1,Size1);
	DEBUG("�ַ���2��ŵ�����:%s�����ȣ�%d\r\n", data2,Size2);
	HAL_FLASH_Unlock(); //����д����
    //1������FLASH
    HAL_FLASH_Unlock();

    //2������FLASH
    //��ʼ��FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr1;
    f.NbPages = 1;
    //����PageError
    uint32_t PageError = 0;
    //���ò�������
    HAL_FLASHEx_Erase(&f, &PageError);
    //3����FLASH��д
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size1 ;i++)
    {
        TempBuf = (uint8_t)*(data1+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr1 + i * 2, TempBuf);
    }
		
		TempBuf = 0;
    for(uint16_t i = 0;i< Size2 ;i++)
    {
        TempBuf = (uint8_t)*(data2+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr2 + i * 2, TempBuf);
    }
    //4����סFLASH
    HAL_FLASH_Lock();
		DEBUG("��ųɹ�\r\n");
}
/******************************************************************************************************************************************************************************
** ������:void Flash_WriteData_3(uint32_t Writeaddr1,uint32_t Writeaddr2,uint32_t Writeaddr3,char *data1,uint16_t Size1,char *data2,uint16_t Size2,char *data3,uint16_t Size3)
** ��������:д��������ַ,��������ַ��Ҫ��Flash��ͬһҳ�����ֱ�д�������ַ������ݺʹ洢�ĳ���
** �������:	Writeaddr1  ��ʼ��ַ1
					  Writeaddr2	��ʼ��ַ2
						Writeaddr3	��ʼ��ַ3
						*data1			�ַ���1����ָ��
						*data2			�ַ���2����ָ��
						*data3			�ַ���3����ָ��
						Size1				�ַ���1����ָ��ĳ���
						Size2				�ַ���2����ָ��ĳ���
						Size3				�ַ���3����ָ��ĳ���
** �������:��

** ���أ���
*******************************************************************************************************************************************************************************/
void Flash_WriteData_3(uint32_t Writeaddr1,uint32_t Writeaddr2,uint32_t Writeaddr3,char *data1,uint16_t Size1,char *data2,uint16_t Size2,char *data3,uint16_t Size3)
{
	
	if(Writeaddr1<FLASH_BASE||((Writeaddr1+Size1)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr1);
		return;//�Ƿ���ַ
	}
	if(Writeaddr2<FLASH_BASE||((Writeaddr2+Size2)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr2);
		return;//�Ƿ���ַ
	}
	if(Writeaddr3<FLASH_BASE||((Writeaddr3+Size3)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("�Ƿ���ַ:0x%x\r\n", Writeaddr3);
		return;//�Ƿ���ַ
	}
	
	DEBUG("�ַ���1������ַ:0x%x\r\n", Writeaddr1);
	DEBUG("�ַ���2������ַ:0x%x\r\n", Writeaddr1);
	DEBUG("�ַ���3������ַ:0x%x\r\n", Writeaddr1);
	DEBUG("�ַ���1��ŵ�����:%s�����ȣ�%d\r\n", data1,Size1);
	DEBUG("�ַ���2��ŵ�����:%s�����ȣ�%d\r\n", data2,Size2);
	DEBUG("�ַ���3��ŵ�����:%s�����ȣ�%d\r\n", data3,Size3);
	HAL_FLASH_Unlock(); //����д����
    //1������FLASH
    HAL_FLASH_Unlock();

    //2������FLASH
    //��ʼ��FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr1;
    f.NbPages = 1;
    //����PageError
    uint32_t PageError = 0;
    //���ò�������
    HAL_FLASHEx_Erase(&f, &PageError);
    //3����FLASH��д
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size1 ;i++)
    {
        TempBuf = (uint8_t)*(data1+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr1 + i * 2, TempBuf);
    }
		TempBuf = 0;
    for(uint16_t i = 0;i< Size2 ;i++)
    {
        TempBuf = (uint8_t)*(data2+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr2 + i * 2, TempBuf);
    }
		TempBuf = 0;
    for(uint16_t i = 0;i< Size3 ;i++)
    {
        TempBuf = (uint8_t)*(data3+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr3 + i * 2, TempBuf);
    }
    //4����סFLASH
    HAL_FLASH_Lock();
}


