#include "../BOARD/ws2812/ws2812.h"
#include "usart.h"
#include "delay.h"
#include "Timer.h"

uint8_t pixelBuffer[PIXEL_NUM][24] ;
 

void ws281x_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTAʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //SPI1ʱ��ʹ�� 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����


  /* PA7  SPI1_MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA7����������� SPI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵ�2�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  
  DMA_DeInit(DMA1_Channel3);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI1 -> DR); //cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pixelBuffer; //cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = PIXEL_NUM * 24; //cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ��� 
  

}
/****************�ر�ȫ��LED**********************/
void ws281x_closeAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      pixelBuffer[i][j] = WS_LOW;
    }
  }
  ws281x_show(); 
}

/***********************д����ɫ***************************/
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

/***********************���ö�ӦLED�Ƶ���ɫ(65536��)***************************/
void ws281x_setPixelColor(uint16_t n ,uint32_t GRBcolor)
{
  uint8_t i;
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer[n][i] = (((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}


/***********************���ö�ӦLED�Ƶ�RGB***************************/
void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}
/***********************��������***************************/
void ws281x_show(void)
{
    DMA_Cmd(DMA1_Channel3, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ�� 
    DMA_ClearFlag(DMA1_FLAG_TC3);    
 	  DMA_SetCurrDataCounter(DMA1_Channel3,24 * PIXEL_NUM );//DMAͨ����DMA����Ĵ�С
 	  DMA_Cmd(DMA1_Channel3, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}



//����һ��0��255��ֵ���õ�һ����ɫֵ��
//��ɫ��r - g - b -��r�Ĺ��ɡ�
uint32_t ws281x_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281x_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281x_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281x_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// ��һ����ɫ��һ����Բ������
void ws281x_colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    delay_ms(wait);
  }
}

void ws281x_rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM; i++) {
      ws281x_setPixelColor(i, ws281x_wheel((i+j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

// ���в�ͬ����ʹ�òʺ���ȷֲ�
void ws281x_rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM; i++) {
      ws281x_setPixelColor(i,ws281x_wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

//TTheatre-style���еơ�
void ws281x_theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, c);    //turn every third pixel on
      }
      ws281x_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//��Ժ�������е���ʺ�Ч��
void ws281x_theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, ws281x_wheel( (i+j) % 255));    //turn every third pixel on
      }
      ws281x_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
void ws281x_singleColor(uint32_t c, uint8_t wait) {
  
  for(uint16_t i=0; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
		}
    ws281x_show();
//    delay_ms(wait);
}

uint16_t TT_i = 0;
uint8_t TT_f = 0;
// ������ɫ��(��ʱ���汾)
void ws281x_singleColor_T(uint32_t c, uint8_t wait) {
  
	if(TT_i <= PIXEL_NUM)
	{
		if(!Time_RGBWait_cnt)
		{
			ws281x_setPixelColor(TT_i, c);
			TT_i++;
		}
	}
	else if(TT_i > PIXEL_NUM)
	{
		if(!Time_RGBWait_cnt)
		{
			if(!TT_f)
			{
			TT_f = 1;
			ws281x_show();
			Time_RGBWait_cnt = wait;
			}
			else
			{
			TT_f = 0;
			TT_i = 0;
			}
		}
	}
}


uint16_t T_i = 0;

// ��һ����ɫ��һ����Բ������(��ʱ���汾)
void ws281x_colorWipe_T(uint32_t c, uint8_t wait) {
  
	if(T_i <= PIXEL_NUM)
	{
		if(!Time_RGBWait_cnt)
		{
			ws281x_setPixelColor(T_i, c);
			ws281x_show();
			T_i++;
			Time_RGBWait_cnt = wait;
		}
	}
	else if(T_i > PIXEL_NUM)
	{

		if(!Time_RGBWait_cnt)
		{
			ws281x_closeAll();			//�ر�ȫ���ƹ�
			T_i = 0;
			Time_RGBWait_cnt = wait;
		}
	}
}
uint16_t T1_i = 0, T1_j = 0;

void ws281x_rainbow_T(uint8_t wait) {

	if(T1_j <= 256)
	{

		if(T1_i <= PIXEL_NUM)
		{
				ws281x_setPixelColor(T1_i, ws281x_wheel((T1_i+T1_j) & 255));
				T1_i++;
		}
		else if(T1_i > PIXEL_NUM)
		{
			if(!Time_RGBWait_cnt)
			{
					T1_i = 0;
					T1_j++;
					ws281x_show();
					Time_RGBWait_cnt = wait;
			}
		}
	}
	else if(T1_j > 256)
	{
		if(!Time_RGBWait_cnt)
		{
			ws281x_closeAll();			//�ر�ȫ���ƹ�
			T1_i = 0;
			T1_j = 0;
			Time_RGBWait_cnt = wait;
		}
	}
}

uint16_t T2_i = 0, T2_j = 0;
// ���в�ͬ����ʹ�òʺ���ȷֲ�
void ws281x_rainbowCycle_T(uint8_t wait) {

	if(T2_j <= 256*5)
	{

		if(T2_i <= PIXEL_NUM)
		{
				ws281x_setPixelColor(T2_i,ws281x_wheel(((T2_i * 256 / PIXEL_NUM) + T2_j) & 255));
				T2_i++;
		}
		else if(T2_i > PIXEL_NUM)
		{
			if(!Time_RGBWait_cnt)
			{
					T2_i = 0;
					T2_j++;
					ws281x_show();
					Time_RGBWait_cnt = wait;
			}
		}
	}
	else if(T2_j > 256*5)
	{
		if(!Time_RGBWait_cnt)
		{
			ws281x_closeAll();			//�ر�ȫ���ƹ�
			T2_i = 0;
			T2_j = 0;
			Time_RGBWait_cnt = wait;
		}
	}
}

uint16_t T3_i = 0, T3_j = 0,T3_q = 0,T3_i2 = 0;
uint8_t T3_f = 0;
//TTheatre-style���еơ�
void ws281x_theaterChase_T(uint32_t c, uint8_t wait) {
	
	if(T3_j <= 10)
	{
		if(T3_q <= 3)
		{
			if(T3_i <= PIXEL_NUM)
			{
					ws281x_setPixelColor(T3_i+T3_q, c);    //turn every third pixel on
					T3_i+=3;
			}
			else if(T3_i > PIXEL_NUM)
			{

					if(!Time_RGBWait_cnt)
					{
						if(!T3_f)
						{
							T3_f = 1;
							ws281x_show();
							Time_RGBWait_cnt = wait;
						}
						else
						{
								if(T3_i2 <= PIXEL_NUM)
								{
										  ws281x_setPixelColor(T3_i2+T3_q, 0);        //turn every third pixel off
										T3_i2+=3;
								
								}
								else	if(T3_i2 > PIXEL_NUM)
								{
									T3_i2 = 0;
									T3_i = 0;
									T3_q ++;
									T3_f = 0;
								
								}
						}
					}
			}
			
		}
		else if(T3_q > 3)
		{
								T3_q = 0;
								T3_j++;
		}
		
	}
	else if(T3_j > 10)
	{
//			ws281x_closeAll();			//�ر�ȫ���ƹ�
			T3_i2 = 0;
			T3_i = 0;
			T3_q = 0;
			T3_f = 0;
			T3_j = 0;
	}
}


uint16_t T4_i = 0, T4_j = 0,T4_q = 0,T4_i2 = 0;
uint8_t T4_f = 0;
//��Ժ�������е���ʺ�Ч��
void ws281x_theaterChaseRainbow_T(uint8_t wait) {
	
		if(T4_j <= 256)
	{
		if(T4_q <= 3)
		{
			if(T4_i <= PIXEL_NUM)
			{
					ws281x_setPixelColor(T4_i+T4_q, ws281x_wheel( (T4_i+T4_j) % 255));    //turn every third pixel on
					T4_i+=3;
			}
			else if(T4_i > PIXEL_NUM)
			{

					if(!Time_RGBWait_cnt)
					{
						if(!T4_f)
						{
							T4_f = 1;
							ws281x_show();
							Time_RGBWait_cnt = wait;
						}
						else
						{
								if(T4_i2 <= PIXEL_NUM)
								{
										  ws281x_setPixelColor(T4_i2+T4_q, 0);        //turn every third pixel off
										T4_i2+=3;
								
								}
								else	if(T4_i2 > PIXEL_NUM)
								{
									T4_i2 = 0;
									T4_i = 0;
									T4_q ++;
									T4_f = 0;
								
								}
						}
					}
			}
			
		}
		else if(T4_q > 3)
		{
								T4_q = 0;
								T4_j++;
		}
		
	}
	else if(T3_j > 256)
	{
//			ws281x_closeAll();			//�ر�ȫ���ƹ�
			T4_i2 = 0;
			T4_i = 0;
			T4_q = 0;
			T4_f = 0;
			T4_j = 0;
	}
}

void ws281x_Init_T(void)
{
			T_i = 0;
	
			T1_i = 0;
			T1_j = 0;
			
			T2_i = 0;
			T2_j = 0;

			T3_i2 = 0;
			T3_i = 0;
			T3_q = 0;
			T3_f = 0;
			T3_j = 0;
	
			T4_i2 = 0;
			T4_i = 0;
			T4_q = 0;
			T4_f = 0;
			T4_j = 0;
	
			TT_i = 0;
			TT_f = 0;
}

