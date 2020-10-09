
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Lcd.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include	"common.h"


const u16 lu8c_8_table[]=
{
	a + b + c + d + e + f,						// Displays "0"  // 0
	b + c,										// Displays "1"  // 1
	a + b + d + e + g,							// Displays "2"  // 2
	a + b + c + d + g,							// Displays "3"  // 3
	b + c + f + g,								// Displays "4"  // 4
	a + c + d + f +g,							// Displays "5"  // 5
	a + c + d + e + f + g,						// Displays "6"  // 6
	a + b + c,									// Displays "7"  // 7
	a + b + c + d + e + f + g,					// Displays "8"  // 8
	a + b + c + d + f + g,						// Displays "9"  // 9
	a + b + c + e + f + g,						// Displays "A"  // 10
	c + d + e + f + g,							// Displays "B"  // 11
	a + d + e + f,								// Displays "C"  // 12
	b + c + d + e + g,							// Displays "D"  // 13
	a + d + e + f + g,							// Displays "E"  // 14
	a + e + f + g,								// Displays "F"  // 15
	a + c + d + e + f,							// Displays "G"  // 16
	b + c + e + f + g,							// Displays "H"  // 17
	e + f,										// Displays "I"  // 18
	b + c + d + e,								// Displays "J"  // 19
	b + d + e + f + g,							// Displays "K"  // 20
	d + e + f,									// Displays "L"  // 21
	a + c + e + g,								// Displays "M"  // 22
	a + b + c + e + f,							// Displays "N"  // 23
	c + e + g,									// Displays "n"  // 24
	c + d + e + g,								// Displays "o"  // 25
	a + b + c + d + e + f,						// Displays "O"  // 26
	a + b + e + f + g,							// Displays "P"  // 27
	a + b + c + f + g,							// Displays "Q"  // 28
	e + g,										// Displays "r"  // 29
	a + c + d + f +g,							// Displays "S"  // 30
	d + e + f + g,								// Displays "t"  // 31
	a + e + f , 								// Displays "T"  // 32
	b + c + d + e + f,							// Displays "U"  // 33
	c + d + e,									// Displays "v"  // 34
	b + d + f + g,								// Displays "W"  // 35
	b + c + d + f + g,							// Displays "Y"  // 36
	a + b + d + e + g,							// Displays "Z"  // 37
	g,											// Displays "-"  // 38
	b,											// Displays "'"  // 39
	0,											// Displays " "  // 40										// none
	d + e  + g,									// Displays "c"  // 41
}; 


/********************************************************************
Function:	fun_LCD_Init
INPUT	:		
OUTPUT	:	
NOTE	:
********************************************************************/
void fun_LCD_Init()
{
	
//	_lcdcp 	= 0b00001001;	// Iniernal Pump 3.0V	
	_lcdcp 	= 0b00001001;	// Iniernal Pump 3.0V COM4 COM5	
//	_coms00 = 1;
//	_coms01 = 1;
	_lcdc0  = 0b00000011;	
	fun_LCD_FULL(0);	
	GCC_NOP();
}

/********************************************************************
Function:	fun_LCD_FULL
INPUT	:	lu8v_lcd_data
OUTPUT	:	null
NOTE	:	���Ը������f�M��ą����yһ�Č�LCD�ą���
********************************************************************/
void fun_LCD_FULL(u8 lu8v_lcd_data)
{
	u8	lu8v_lcdcount;
     for(lu8v_lcdcount = 0 ;lu8v_lcdcount < 32;lu8v_lcdcount++)    	
     {
     	 gu8v_LCD_buffer[lu8v_lcdcount].U8 = lu8v_lcd_data;
     }  
}


/********************************************************************
Function:	LCD_NUM
INPUT	:	lu8v_lcd_mode 	lu8v_addr lu8v_data
OUTPUT	:	
NOTE	:	lu8v_lcd_mode���@ʾģʽ�������硢�W�q��lu8v_addr���@ʾ�ĵ�ַlu8v_data�@ʾ����
********************************************************************/
//LCD
void fun_LCD_NUM(u8 lu8v_lcd_mode,u8 lu8v_addr,u8 lu8v_data)
{
		volatile	u16 lu8v_table_buff;
		volatile	u8 lu8v_LCD_Addr1,lu8v_LCD_Addr2,lu8v_LCD_Addr3;
		lu8v_table_buff = lu8c_8_table[lu8v_data];
		
		
		if(lu8v_addr == 4)
		{
			lu8v_LCD_Addr1 = 4;
			lu8v_LCD_Addr2 = 12;
			lu8v_LCD_Addr3 = 13;
		}
		else
		{
			lu8v_LCD_Addr1 = lu8v_addr;
			lu8v_LCD_Addr2 = lu8v_addr + 1;	
			lu8v_LCD_Addr3 = lu8v_addr + 2;		
		}
		
		
		gu8v_LCD_buffer[lu8v_LCD_Addr1].U8 	&= 	0b00000100;
		gu8v_LCD_buffer[lu8v_LCD_Addr2].U8 	&= 	0b00000000;		
		gu8v_LCD_buffer[lu8v_LCD_Addr3].U8 	&= 	0b00000100;

		if(lu8v_lcd_mode==0)
		{	

		} 		
		else if(lu8v_lcd_mode == 1)
		{
			 gu8v_LCD_buffer[lu8v_LCD_Addr1].U8 |= lu8v_table_buff	   & 0b00000011;
			 gu8v_LCD_buffer[lu8v_LCD_Addr2].U8 |= lu8v_table_buff >>4 & 0b00000111;	
			 gu8v_LCD_buffer[lu8v_LCD_Addr3].U8 |= lu8v_table_buff >>8 & 0b00000011;			 	 			
		}
		else
		{
			
		}	
}

void LCD_DISPLAY_NUM(u8 lu8v_mode,u16 lu8v_DISPLAUDATA)
{   

	if(lu8v_mode == 0)
	{
		fun_LCD_NUM(1,LCD_ADDR3,lu8v_DISPLAUDATA%10);
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR2,lu8v_DISPLAUDATA%10);
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR1,lu8v_DISPLAUDATA%10);	
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR0,lu8v_DISPLAUDATA%10);
			
		
		SET_LCD_DOT();	
	}
	if(lu8v_mode == 1)
	{
		fun_LCD_NUM(1,LCD_ADDR3,lu8v_DISPLAUDATA%10);
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR2,lu8v_DISPLAUDATA%10);
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR1,lu8v_DISPLAUDATA%10);	
		lu8v_DISPLAUDATA /= 10;
		fun_LCD_NUM(1,LCD_ADDR0,lu8v_DISPLAUDATA%10);
		
		CLR_LCD_DOT();	
	}
}

void fun_LCD_Display_CF(u8 CF_Flag)			//ģʽ��ʾ
{
	if(CF_Flag == 0)
	{
		CLR_LCD_TempC();
		CLR_LCD_TempF();
	}
	else if(CF_Flag == 1)
	{
		SET_LCD_TempC();
	}
	else if(CF_Flag == 2)
	{
		SET_LCD_TempF();
	}
}

void fun_LCD_Display_Mode(u8 Mode_Sum)			//ģʽ��ʾ
{
	if(Mode_Sum == Mode_None)
	{
		CLR_LCD_SurTmp();
		CLR_LCD_Body();
		CLR_LCD_Room();
	}
	else if(Mode_Sum == Mode_SurTmp)
	{
		SET_LCD_SurTmp();
		CLR_LCD_Body();
		CLR_LCD_Room();
	}
	else if(Mode_Sum == Mode_Body)
	{
		CLR_LCD_SurTmp();
		SET_LCD_Body();
		CLR_LCD_Room();
	}
	else if(Mode_Sum == Mode_Room)
	{
		CLR_LCD_SurTmp();
		CLR_LCD_Body();
		SET_LCD_Room();
	}
}

void fun_LCD_Display_C()			//��˸
{
	if(gbv_500ms_reversal)
	{
		fun_LCD_NUM(1,LCD_ADDR3,LCDCODE_c);	 //c 
	}
	else
	{
		fun_LCD_NUM(1,LCD_ADDR3,LCDCODE_NONE);	//none 
	}
}

void fun_LCD_Display_BAT()			//�����ʾ��˸
{
	if(gbv_500ms_reversal)
	{
		SET_LCD_BAT();
	}
	else
	{
		CLR_LCD_BAT();
	}
}

void fun_LCD_Display_Cor(u8 lu8v_corr)
{
	if(lu8v_corr == 0)
	{
		fun_LCD_NUM(1,LCD_ADDR0,LCDCODE_C);	 
		fun_LCD_NUM(1,LCD_ADDR1,LCDCODE_o);	 	
		fun_LCD_NUM(1,LCD_ADDR2,LCDCODE_r);	 
		fun_LCD_NUM(1,LCD_ADDR3,LCDCODE_r);	 
	}
	else
	{
		fun_LCD_NUM(1,LCD_ADDR0,LCDCODE_C);	
		fun_LCD_NUM(1,LCD_ADDR1,LCDCODE__);		
		fun_LCD_NUM(1,LCD_ADDR2,lu8v_corr);	
		fun_LCD_NUM(1,LCD_ADDR3,LCDCODE_NONE);	
	}
}

void fun_LCD_Display_Success()
{
	fun_LCD_NUM(1,LCD_ADDR0,LCDCODE__);	
	fun_LCD_NUM(1,LCD_ADDR1,LCDCODE_O);		
	fun_LCD_NUM(1,LCD_ADDR2,LCDCODE_K);	
	fun_LCD_NUM(1,LCD_ADDR3,LCDCODE__);	
}

void fun_LCD_Display_Fail()
{
	fun_LCD_NUM(1,LCD_ADDR0,LCDCODE_F);	
	fun_LCD_NUM(1,LCD_ADDR1,LCDCODE_A);		
	fun_LCD_NUM(1,LCD_ADDR2,LCDCODE_I);	
	fun_LCD_NUM(1,LCD_ADDR3,LCDCODE_L);	
}

void fun_LCD_Display____()
{
	fun_LCD_NUM(1,LCD_ADDR0,LCDCODE__);	
	fun_LCD_NUM(1,LCD_ADDR1,LCDCODE__);		
	fun_LCD_NUM(1,LCD_ADDR2,LCDCODE__);	
	fun_LCD_NUM(1,LCD_ADDR3,LCDCODE__);	
	CLR_LCD_DOT();  //ȡ��С����
}

void fun_LCD_Display_Power()			//�ϵ���ʾ
{
	SDK_SYS.Dis_CF_Flag = 1;
	SDK_SYS.DisMode = Mode_Body;
	
	SET_LCD_BAT();		//�����ʾ��
	SET_LCD_BEEP();    	//��������ʾ��
	fun_LCD_Display_Mode(SDK_SYS.DisMode);  //�������¶���ʾ
	fun_LCD_Display____();
	fun_LCD_Display_CF(SDK_SYS.Dis_CF_Flag);
}

void fun_LCD_Test()
{
	fun_LCD_Init();
	LCD_DISPLAY_NUM(1,4000);
/*	gu8v_LCD_buffer[3].U8 = 0b00000100;*/
//	LCD_DISPLAY_NUM(0,1234);
	while(1)
	{
		GCC_CLRWDT();
		
	}
}