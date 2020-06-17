#include "sys.h"

#include "stm32f10x.h"
/**************************************************��1���������źŲɼ����������õ�ADC***************************************/

#if 1
#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 adc_converted_value[ADC_CHANNEL_NUM+1];
//__IO u16 ADC_ConvertedValueLocal;


/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC0��pc1
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
/*
�ڶ�Ӳ���汾
14	PA3	��ߺ�����ײ������	ADC
15	PA4	���к�����ײ������	ADC
16	PA5	���к�����ײ������	ADC
17	PA6	�м������ײ������	ADC
18	PA7	���к�����ײ������	ADC
19	PB0	���к�����ײ������	ADC
20	PB1	�ұߺ�����ײ������	ADC
����Ӳ���汾
PB1	��ߺ�����ײ������	ADC9
PB0	���к�����ײ������	ADC8
PB0	���к�����ײ������	ADC8
PA7	���к�����ײ������	ADC7
PA3	�м������ײ������	ADC3
PA4	���к�����ײ������	ADC4
PA5	���к�����ײ������	ADC5
PA6	�ұߺ�����ײ������	ADC6

PA0	��߷�����	ADC 		ADC0
PA1	�м������	ADC			ADC1
PA2	�ұ߷�����	ADC			ADC2

�ò��汾
/--------------------------
PB1 ��ߺ����ഫ����	ADC9
PA5	��ߺ�����ײ������	ADC5
PA3	�м�����ഫ����	ADC3
PA4	�ұߺ�����ײ������	ADC4
PA6	�ұߺ����ഫ����	ADC6

PA0	��߷�����			ADC0
PA2	�ұ߷�����			ADC2
----------------------------/

	PC4	��ɨ����Ĺ�����⣨H/L��ƽ��⣩	ADC
	PC5	����������Ĺ�����⣨H/L��ƽ��⣩


PC3		AD13 	����������
PC2		AD12 	����ѹ

PC0		AD10 	�����ҵ������
PC1     AD11	�����ֵ������

*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PC1,����ʱ������������


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PC4	��ɨ����Ĺ������  PC5	����������Ĺ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)adc_converted_value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    /*����ڴ��ַ��һ*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUM;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);

	PC3		AD13 ����������
	PC2		AD12 ����ѹ
	PC0		�����ҵ������
	PC1     �����ֵ������
 */

	//��ɨ��������������AD��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 16, ADC_SampleTime_55Cycles5);	//������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 15, ADC_SampleTime_55Cycles5);	//����ѹ

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 14, ADC_SampleTime_55Cycles5);	//�����ҵ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 13, ADC_SampleTime_55Cycles5);	//������������

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 12, ADC_SampleTime_55Cycles5);	//�Ҽ�
	////ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_55Cycles5);//��ɨ����������

	/*
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 7, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);
	*/
	//��·ǰ�ú���
#if VER_BORD_2
/*
14	PA3	��ߺ�����ײ������	ADC
15	PA4	���к�����ײ������	ADC
16	PA5	���к�����ײ������	ADC
17	PA6	�м������ײ������	ADC
18	PA7	���к�����ײ������	ADC
19	PB0	���к�����ײ������	ADC
20	PB1	�ұߺ�����ײ������	ADC

*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);	//���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_55Cycles5);	//�м�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7, ADC_SampleTime_55Cycles5);	//�ұ�
#else

#if (1 == HOOVER)
/*
PA5	��ߺ����ഫ����	ADC9	1
PB1	���к�����ײ������	ADC8	2
PA3	�м������ײ������	ADC3	4
PA6	���к�����ײ������	ADC4	5
PA4	�ұߺ����ഫ����	ADC6	7
*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5); //���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);	//��ײ��
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); //LAOBANBEN�м�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5); //�м�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);	//��ײ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5); //�ұ�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_55Cycles5);//ǰ��������
#else
/*
PB1	��ߺ�����ײ������	ADC9	1
PB0	���к�����ײ������	ADC8	2
PA7	���к�����ײ������	ADC7	3
PA3	�м������ײ������	ADC3	4
PA4	���к�����ײ������	ADC4	5
PA5	���к�����ײ������	ADC5	6
PA6	�ұߺ�����ײ������	ADC6	7

*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5); //���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); //�м�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5); //�ұ�
#endif
#endif

#if (1 == HOOVER)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 10, ADC_SampleTime_55Cycles5);

#else
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 9, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 10, ADC_SampleTime_55Cycles5);
#endif
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void adc1_init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


#else
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t adc_converted_value[ADC_CHANNEL_NUM+1];//adֵ�ݴ�����


void adc_init(void )
{

/* Private function prototypes -----------------------------------------------*/
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ʹ�� MDA ��Ҫ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);// ʹ�� ADC1 ��Ҫ������


  //ADC_InitTypeDef  ADC_InitStructure;
  //PC1/2/3 ��Ϊģ��ͨ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //ģ����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //ģ����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	/*DMA1 channel1 configuration ----------------------------------------------*/
  	DMA_DeInit(DMA1_Channel1);			   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	  //����DMA�������ַΪADC1
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_converted_value; //����DMA�ڴ����ַ
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	  //������Ϊ���ݴ������Դ
  	DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;	  //����ָ��DMAͨ����DMA����Ĵ�С
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;		//�ڴ��ַ�Ĵ�������
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���ݿ��Ϊ16λ
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//���ݿ��Ϊ16λ
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	  //������ѭ������ģʽ
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMAͨ��1ӵ�и����ȼ�
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//DMAͨ��1û������Ϊ�ڴ浽�ڴ洫��
  	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  	/* Enable DMA1 channel1 */
  	DMA_Cmd(DMA1_Channel1, ENABLE);

  	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //��������ģʽ
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //ɨ��ģʽ
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   //����ת��ģʽ
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������ת��
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //�Ҷ���
  	ADC_InitStructure.ADC_NbrOfChannel =ADC_CHANNEL_NUM;
  	ADC_Init(ADC1, &ADC_InitStructure);

  	/* ADC1 regular channel_8,configuration */
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5); //ת��ʱ��Ϊ17.1US
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);

  	/* Enable ADC1 DMA */
  	ADC_DMACmd(ADC1, ENABLE);

  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);

  	/* Enable ADC1 reset calibaration register */
  	ADC_ResetCalibration(ADC1);

  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibaration */
  	ADC_StartCalibration(ADC1);

  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));

  	/* Start ADC1 Software Conversion */
  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
#endif



