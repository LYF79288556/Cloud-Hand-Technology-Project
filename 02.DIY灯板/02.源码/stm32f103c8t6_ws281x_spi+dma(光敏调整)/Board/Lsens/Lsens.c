#include "Lsens.h"
#include "delay.h"



void ADC1_Init(void)
{ 
		ADC_InitTypeDef ADC_InitStructure;


		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //ʹ��ADC3ͨ��ʱ��
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC��λ
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//��λ����

		ADC_DeInit(ADC1);//��λADC3,������ ADC3��ȫ���Ĵ�������Ϊȱʡֵ

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ: ����ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE; //ģ��ת�������ڵ�ͨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת�������ڵ���ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת��������������ⲿ��������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת����ADCͨ������Ŀ
		ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


		ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1

		ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼ 

		while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����

		ADC_StartCalibration(ADC1); //����ADУ׼

		while(ADC_GetCalibrationStatus(ADC1)); //�ȴ�У׼����

}

//���ADC3ĳ��ͨ����ֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc1(u8 ch)
{
//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 ); //ADC1,ADCͨ��,����ʱ��Ϊ239.5����
ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ָ����ADC3�����ת����������
while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
return ADC_GetConversionValue(ADC1); //�������һ��ADC3�������ת�����
}


void Lsens_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 anolog����
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
GPIO_Init(GPIOA, &GPIO_InitStructure);
ADC1_Init();//�������ǿ���ֱ�ӵ���ADC�ĳ�ʼ�����Ӷ�������������װ�ɹ����������ĳ�ʼ��������һ�����������ֱ�ӵ���

}

//�Թ�����������ʼ��֮����Ҫ������ADC1�ɼ�����ֵ����һЩת�����Ӷ�ֱ��ͨ��LCD����ʾ������ֵ�Ĵ�С�жϹ�ǿ���䷶Χ����Ϊ0~100����װ�������£�

//��ȡLight Sens��ֵ
//0~1000:0,����; 1000,�
u16 Lsens_Get_Val(void)
{
u32 temp_val=0;
u8 t;
for(t=0;t<Lsens_Sum;t++)
{
temp_val+=Get_Adc1(Lsens_Ch); //��ȡADCֵ
delay_ms(5);
}

temp_val/=Lsens_Sum;//�õ�ƽ��ֵ

//3.3V��Ӧ4096������������ֵõĵ�ѹֵ�����ܴ���3.3V��Ҳ����˵ADC�ɼ�����ֵ�����ܴ���4096�����������4000Ϊ��

if(temp_val>4000)
	temp_val=4000;

//��������ֵõĵ�ѹֵԽ����temp_val/40)��ֵԽ�󣬼���������ֵõĵ�ѹֵԽ��Ҳ������˵������ǿ��Խ�������������ڷֳ�0~100����100-100-(temp_val/40)��ֵҲ��ԽС

return (u16)(Lsens_Max-(temp_val));
//return (u16)(Lsens_Max-(temp_val));
}
