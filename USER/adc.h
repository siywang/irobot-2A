#ifndef  _assignment2_H_
#define  _assignment2_H_

///#include "stm32f10x.h"




#if(1)
#define	MAX_ADC_VALUE		4
#define ADC_CHANNEL_NUM 16  //ADC����ͨ����
#define ADC_SAMPLE_NUM  1 //ÿ��ͨ��������

#define V_DUST_ADC()		adc_converted_value[15]
#define I_DUST_ADC()		adc_converted_value[11]		//����
#define I_MID_ADC()			adc_converted_value[10]

#define I_SMTL_ADC()		adc_converted_value[5]
#define I_SMTR_ADC()		adc_converted_value[1]

#define I_CHARG_ADC()		0//adc_converted_value[15]
#define V_CHARG_ADC()		adc_converted_value[14]
#define T_BAT_ADC()			0//adc_converted_value[11]

#define I_MOTER_RIGHT()		adc_converted_value[13]
#define I_MOTER_LEFT()		adc_converted_value[12]
#define V_PHOTORESISITOR()	adc_converted_value[8]	//��������
#define DUST_VALUE()		0//	adc_converted_value[1]		//�ҳ�Ũ��

#else
#define	MAX_ADC_VALUE		4
#define ADC_CHANNEL_NUM 16  //ADC����ͨ����
#define ADC_SAMPLE_NUM  1 //ÿ��ͨ��������

#define I_DUST_ADC()		adc_converted_value[11]
#define I_MID_ADC()			adc_converted_value[10]

#define I_SMTL_ADC()		adc_converted_value[5]
#define I_SMTR_ADC()		adc_converted_value[1]

#define I_CHARG_ADC()		adc_converted_value[15]
#define V_CHARG_ADC()		adc_converted_value[14]
#define T_BAT_ADC()			adc_converted_value[11]

#define I_MOTER_RIGHT()		adc_converted_value[13]
#define I_MOTER_LEFT()		adc_converted_value[12]
#endif
//void adc_config(void);
void adc1_init(void);




extern __IO uint16_t adc_converted_value[ADC_CHANNEL_NUM+1];//adֵ�ݴ�����
//extern __IO uint16_t adc_converted_value[3];//adֵ�ݴ�����void adc_init(void );
#endif
