
/***********************************************************************************************
14.8V 4��﮵�س�����

ͬƽ̨��ֲ˵��
1��PID��������Ϊ10ms
2����Ҫ�޸ĵĺ������
	a �������������ͨ��
		����ͨ��:LIBAT_CHARGECURRENT_CHNO/LIBAT_CHARGEVOLT_CHNO/LIBAT_LIBATTEMPER_CHNO
		��������:LiBat_GetBatTemper()/LiBat_GetBatVolt()/LiBat_GetChargeCurrent()
	b ����ĵ�Դ����/��༰�Եغ���ĵ�Դ����
		LIBAT_MOTOR_PWR_PORT/LIBAT_MOTOR_PWR_PIN/LIBAT_IRDIST_PWR_PORT/LIBAT_IRDIST_PWR_PIN/
		�漰���� LiBat_TurnOnOtherPwr()
	c ���PWM��TIM�������ͨ��
		���IO��PWM LIBAT_CHPWM_PORT/LIBAT_CHPWM_PIN/LIBAT_CHPWM_TIM/LIBAT_CHPWM_CH
3������
	��ʼ��LiBat_HalInit()
	���ģʽ�˳�LiBat_ExitChargeMode()

	��繦�ܺ��������Ե��� LiBat_CurrentPid()
				һ����÷�ʽ:LiBat_CurrentPid(500,10); //Ԥ��500mA PID������10����������ƽ��
***********************************************************************************************/


/***********************************************************************************************
�ײ������ӿ� INCLUDE
***********************************************************************************************/
#include "sys.h"
/*
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "system_conf.h"
#include "portmacro.h"
#include "detect.h"
#include "stm32f10x.h"

#include "detect.h"
#include "libatcharge.h"
*/
//#include "ADC.h"
//#include "pwms.h"
#include "TempTrans.h"
#include "test_mode.h"

/***********************************************************************************************
��������ر���
***********************************************************************************************/
static int16_t PwmOutOld = 0;//��һ�ε�PWM���ֵ(�Ȿ����LiBat_CurrentPid�ľֲ�����,���ڵ�����
							//�����,�˱�����Ҫ��0,����ᷢ������,���Ը���ȫ�ֱ���,��initial����0)
/***********************************************************************************************
end ��������ر���
***********************************************************************************************/
/***********************************************************************************************
end �ײ������ӿ� INCLUDE
***********************************************************************************************/

/***********************************************************************************************
PID��ر���
***********************************************************************************************/
typedef struct _libat_pid{
	int16_t SetPoint;// �趨Ŀ��Desired value
	int16_t Proportion;// ��������Proportional Const
	int16_t Integral;// ���ֳ���Integral Const
	int16_t Derivative;// ΢�ֳ���Derivative Const
	int16_t LastError;// Error[t - 1]
	int16_t PrevError;// Error[t - 2]
	int16_t SumError;  // Sums of Errors
} LB_PID;


static LB_PID Pid_Current,Pid_Volt;
static int16_t ErrorBufferCurrent[16];
static int16_t ErrorBufferVolt[16];
static uint8_t ErrorBufferCurrentCt;//,ErrorBufferVoltCt;//��ֵ���������²�ֵ���ڵ�λ��
//﮵��������
/*
int libat_cap[28]={ 	16600,16560,16480,16400,16320,16200,16120,
						15880,15720,15600,15480,15360,15240,15160,
						15080,15040,14960,14920,14880,14840,14760,
						14640,14600,14560,14520,14440,14360,14320};
*/

/*
	15200		75%����  ����
	14200		50%����	 ����
	13700		25%��	 ��
	*/
/*
	15200		66%����  ����
	14200		31%����	 ����
	14200		10%����	 һ��
	13500		10%��	 ��
	*/
#if CAPACITY_4400_VER
int libat_cap[54]={ 	16050,16000,15900,15850,15800,15750,15700,		//100-88
						15650,15600,15500,15400,15450,15400,15350,		//87-75
						15300,15100,15050,15000,14950,14890,14860,		//73-62
						14830,14750,14690,14630,14570,14510,14450,		//61-50
						14400,14370,14340,14310,14280,					//49-41
						14250,14150,14135,14115,14100,14050,13950,		//39-20
						13930,13910,13890,13850,13830,13800,13740,		//19-10
						13700,13600,13500,13400,13300,13200,13100};		///9-0

int libat_percent[54] ={100,98,96,94,92,90,88,
							87,85,83,81,79,77,75,
							73,71,69,67,65,63,62,
							61,60,58,56,54,52,50,
							49,47,45,43,41,
							39,33,32,31,30,25,20,
							19,18,17,15,14,12,10,
							 9, 8, 6, 5, 3, 1, 0 };

#else
int libat_cap[21]={			16600,	16410,	16220,	16030,	15840,	
								15650,	15460,	15270,	15080,	14890,	
								14700,	14510,	14320,	14130,	13940,	
								13750,	13560,	13370,	13180,	12990,	
								12800						};

int libat_percent[21] ={	100, 	95,		90,		85,		80,	
								75,		70,		65,		60,		55,	
								50,		45,		40,		35,		30,	
								25,		20,		15,		10,		5,	
								0							};	

#endif

#if CAPACITY_4400_VER
#define	LIBAT_CAP_NUB	54
#else
#define	LIBAT_CAP_NUB	49
#endif
#define BTY_FULL_VOLT		16600
#define BTY_EMPT_VOLT		12800

int get_libat_percent(int volt)
{
#if 1
		static uint8_t last=0,last_i[5]={0},change_i_c=0,first_in=1;
		uint8_t i;
		#if 0 //���  0,5,10,15
		for(i=0;i<21;i++)
		{
			if(volt >=libat_cap[i])
				break;
		}
		#else//���Լ���	(16600-12800)/100 = 38		
		if(volt>BTY_FULL_VOLT)
			volt = BTY_FULL_VOLT;
		if(volt<BTY_EMPT_VOLT)
			volt = BTY_EMPT_VOLT;
		i = (volt-BTY_EMPT_VOLT)/38;		
		#endif
		//log_printf("get_libat_percent:i:%d\r\n",i);
		if(first_in)
		{
			first_in = 0;
			change_i_c=0;
			last = i;
		}
		else
		{
			if(i != last)	//��ֹ����
			{		
				last_i[change_i_c] = i;
				if(change_i_c++>=3)
				{
					i = (last_i[0]+last_i[1]+last_i[2])/3;					
					change_i_c = 0;
					last=i;
					//log_printf("1i:%dlast(%d,%d,%d)(%d,%d,%d)per%d\r\n",i,last_i[0],last_i[1],last_i[2],libat_cap[last_i[0]],libat_cap[last_i[1]],libat_cap[last_i[2]],libat_percent[i]);
				}
				else
				{
					i = last;		//û��3�λ�ȡ��һ�ε�
					//log_printf("2i:%dlast(last%d)per%d\r\n",i,last,libat_percent[i]);
				}	
			}
			else
			{				
				change_i_c = 0;
				//log_printf("3i:%d,per%d\r\n",i,libat_percent[i]);
			}	
		}
		//log_printf("volt=%d,%d\r\n",volt,libat_percent[i]);
#if 0		
		i = i > 20 ? 20:i;		
		return libat_percent[i];	
#else
		return i;
#endif
#else
		int i;
		for(i=0;i<21;i++)
		{
			if(volt >=libat_cap[i])
				break;
		}
		//log_printf("volt=%d,%d\r\n",volt,libat_percent[i]);
		i = i > 20 ? 20:i;
		return libat_percent[i];
#endif	

}
/***********************************************************************************************
end PID��ر���
***********************************************************************************************/


/***********************************************************************************************
�ײ������ӿ�
***********************************************************************************************/

//�Ƿ��ڳ������1Ϊ��⵽�����,0Ϊδ��⵽
static uint8_t LiBat_isOnDock(void)
{
	//if(NO_CHARGE_DETECTED!=dc_socket_detect())
	return 1;
	//	return 0;
}

//��ʼ�����PWM���
//GPIO PD12 TIM4.1
//PWM���ֵ4095,����Ƶ
//���PWM��ʼ��,����TIMER��������,���������TIMER���³�ʼ��,�ᷢ�������ҹ��ܾòŻ�������������������
//�ָ�������,����������ֱ��ʹ�õ�ԭ�����TIMER����,Ƶ��Ϊ18K,�ȳ��PWMƵ��Ҫ��,��Ŀǰ��Ӱ��ʹ��
void LiBat_InitPwrChargePwm(void)
{

//	TIM_TimeBaseStructure.TIM_Period = 0xff;
//	TIM_TimeBaseStructure.TIM_Prescaler = 9;//36-1;
	GPIO_InitTypeDef GPIO_InitStructure;
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //Timer1��ȫ��ӳ��  TIM1_CH1->E9
#if 0
	TIM_Cmd(TIM1, DISABLE);  //ʹ��TIM1
	TIM_DeInit(TIM1);//��ʼ��TIM1�Ĵ���
#endif
 	LIBAT_CHPWM_PORT->ODR &= ~LIBAT_CHPWM_PIN;
       //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��	GPIOE.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

 	LIBAT_CHPWM_PORT->ODR &= ~LIBAT_CHPWM_PIN;
	// GPIO_SetBits(GPIOE,GPIO_Pin_9);
	// GPIO_SetBits(GPIOE,GPIO_Pin_14);
	// while(1);
 #if 1
 #if 0
       //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = 0xff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =9; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
#endif

	//��ʼ��TIM1 Channel1 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse =1000;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���

#if 0
 	TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	//TIM_ForcedOC3Config(TIM1, TIM_ForcedAction_InActive);
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//����PWM�����
#endif
 	TIM1->CCR3 = 1000;
 	//TIM_ForcedOC3Config(TIM1, TIM_ForcedAction_Active);
	#endif
#if 0

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_DeInit(TIM1);//��ʼ��TIM1�Ĵ���
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //Timer1��ȫ��ӳ��  TIM1_CH1->E9

	 GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1);

      //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��	GPIOE.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

	// GPIO_SetBits(GPIOE,GPIO_Pin_9);
	// GPIO_SetBits(GPIOE,GPIO_Pin_14);
	// while(1);

       //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = 0xff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =9; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ccr1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//���ͬ�࣬TIM_OCNPolarity_Highʱ�������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	TIM_OC1Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
/*
	TIM_OCInitStructure.TIM_Pulse = ccr3;
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = ccr4;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
*/
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
#endif


}

//���ó��PWM�����ֵ,0-4095
void LiBat_SetPwrChargePwm(uint16_t pwm)
{
	TIM1->CCR3 =  /*255 -*/ 1000 - (pwm << 2);
//	PRINTF("%d:%d\r\n",pwm,TIM1->CCR3);
/*
#if LIBAT_CHPWM_CH == 1
	TIM_SetCompare1(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 2
	TIM_SetCompare2(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 3
	TIM_SetCompare3(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 4
	TIM_SetCompare4(LIBAT_CHPWM_TIM, (pwm));
#endif
*/
}
//���ó��PWM�����ֵ,0-4095
uint16_t LiBat_GetPwrChargePwm(void)
{
	return (1000 - TIM1->CCR3) >> 2;
//	PRINTF("%d:%d\r\n",pwm,TIM1->CCR3);
/*
#if LIBAT_CHPWM_CH == 1
	TIM_SetCompare1(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 2
	TIM_SetCompare2(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 3
	TIM_SetCompare3(LIBAT_CHPWM_TIM, (pwm));
#elif LIBAT_CHPWM_CH == 4
	TIM_SetCompare4(LIBAT_CHPWM_TIM, (pwm));
#endif
*/
}



/*----------------------------------------------------------------------------------------------
-- ��ʼ��ADC,����������ط��ѶԴ������ģ����г�ʼ������˺�������
-- ����������PB0 ADC8
-- ����ѹ����PA7 ADC7
-- ����¶Ȳ���PC5 ADC15
----------------------------------------------------------------------------------------------*/
void LiBat_InitAdc(void)
{
#if 0
	ADC_InitTypeDef ADC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;

	//ADC Channel 7
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ADC Channel 8
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//ADC Channel 15
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC1 configuration ------------------------------------------------------*/
	/* Clock On */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// ADC1 regular channel0 configuration
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 DMA
	ADC_DMACmd(ADC1, ENABLE); */

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
#endif
}


/*----------------------------------------------------------------------------------------------
-- ��ĳһͨ���������ж��ת��
-- Channel:ͨ�����,��0ʼ
-- Channel: ת��ͨ����,ADC_Channel_xx �ɲο� LIBAT_CHARGECURRENT_CH
											 LIBAT_CHARGEVOLT_CH
											 LIBAT_LIBATTEMPER_CH
-- Times: ת������,ת����Ҫȥ��һ�����ֵ��һ����Сֵ����ʵ��ת��������Times��2
----------------------------------------------------------------------------------------------*/
uint16_t LiBat_ADCGetChannel(uint8_t Channel,uint8_t Times)
{
#if 1
	return 0;//getAdcValue(Channel + 1);
#else
	uint8_t i;
	uint16_t TempADC;
	uint16_t ADC_Channel_MAX;//ͨ��ת���еõ������ֵ
	uint16_t ADC_Channel_MIN;//ͨ��ת���еõ�����Сֵ
	uint32_t ADC_Channel_SUM;//ͨ��ת������ĺ�

	if(!Times)
		return 0;
	ADC_Channel_SUM = 0;
	Times += 2;
	ADC_Channel_MAX = 0;
	ADC_Channel_MIN = 0x0fff;
	ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_7Cycles5);
/*	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_TranseCt);*/
	for(i = 0;i < Times;i ++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		TempADC = ADC_GetConversionValue(ADC1);
		if(TempADC > ADC_Channel_MAX)
			ADC_Channel_MAX = TempADC;
		if(TempADC < ADC_Channel_MIN)
			ADC_Channel_MIN = TempADC;
		ADC_Channel_SUM += TempADC;
	}
	ADC_Channel_SUM -= (ADC_Channel_MAX + ADC_Channel_MIN);
	ADC_Channel_SUM /= (Times - 2);

	return (uint16_t)ADC_Channel_SUM;
#endif
}


/***********************************************************************************************
end �ײ������ӿ�
***********************************************************************************************/

/***********************************************************************************************
�м�㺯��
***********************************************************************************************/

/*----------------------------------------------------------------------------------------------
-- ����ѹֵ����Ϊ��ѹֵ�ĺ���
-- adc: adcת����ֵ
-- adc_max: adc���������ֵ
-- voltref: �ο���ѹֵ,��ֵ�Ŵ�1000����ȡ����
-- ����: ת���ĵ�ѹֵ,��λmv
----------------------------------------------------------------------------------------------*/
/*
static uint32_t LiBat_Adc2Volt(uint16_t adc,uint16_t voltref)
{
	uint32_t volt;

	volt = (uint32_t)adc * voltref;
	volt = volt/LIBAT_ADCDIGI_MAX;
	return volt;
}
*/

/*----------------------------------------------------------------------------------------------
-- ����ѹֵ����Ϊ����ֵ�ĺ���,�����������贮�����������λ���·��Ĳ�����·
-- adc: adcת����ֵ
-- adc_max: adc���������ֵ
-- resup: �Ͻӵ������ֵ����λΪŷķ
-- ����: ������ֵ,��λ:ŷķ
----------------------------------------------------------------------------------------------*/
/*
static uint32_t LiBat_Adc2Res(uint16_t adc,uint32_t resup)
{
	uint32_t resistor;

	resistor = (uint32_t)adc * resup;
	resistor = resistor/(LIBAT_ADCDIGI_MAX - adc);
	return resistor;
}
*/

/*----------------------------------------------------------------------------------------------
-- ��ȡ������ֵ,��λ mA
-- ����: ��������ֵ,��λ:mA
----------------------------------------------------------------------------------------------*/
uint16_t LiBat_GetChargeCurrent(void)
{
	int32_t res;
#if 1
	res = I_CHARG_ADC()*2;
	res = (res*25 + 15)/31 - LIBAT_CHARGECURRENT_OFFSET;
	res = res<0 ? 0:res;
#else
	res = LiBat_ADCGetChannel(LIBAT_CHARGECURRENT_CH,8);
	res = LiBat_Adc2Volt((uint16_t)res,LIBAT_ADCVOLREF);//��λΪmv
	res = res*1000*LIBAT_ADCCURRENT_GAIN_DIV/LIBAT_ADCCURRENT_GAIN_MULTI/LIBAT_ADCCURRENT_RES;//�������ֵ,��λΪmA
#endif
	return (uint16_t)res;
}

/*----------------------------------------------------------------------------------------------
-- ��ȡ����صĵ�ѹ,��λ mv
-- ����: ������ѹֵ,��λ:mv
----------------------------------------------------------------------------------------------*/
uint16_t LiBat_GetBatVolt(void)
{
	uint32_t res;
#if 1
	res = (V_CHARG_ADC()*807 + 45)/91;// 3.300V�ο���ѹ
	//res = (getAdcValue(8)*7205 + 410)/819;//3.275V �ο���ѹ
#else
	res = LiBat_ADCGetChannel(LIBAT_CHARGEVOLT_CH,8);
	res = LiBat_Adc2Volt((uint16_t)res,LIBAT_ADCVOLREF);//��λΪmv
	res = res*LIBAT_ADCVOLT_GAIN_DIV/LIBAT_ADCVOLT_GAIN_MULTI;//�������ֵ,��λΪmA
#endif
	return (uint16_t)res;
}


/*----------------------------------------------------------------------------------------------
-- �¶Ȳ����,�����¶�ֵ-10~120,*10
-- �¶ȱ�:131*2byte,in flash
-- �¶ȱ���:const uint16 temper[120]
-- T95 T40 ΪУ�²���,ȱ��Ϊ950��400
----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------
-- ��ȡ����ص��¶�,��λ ���϶�*10 ,��Χ-10~110,*10
-- ����: ��ص��¶�ֵ,��λ:���϶�*10 ,��Χ-10~110,*10
-- �¶ȱ���:const u16 r20klist[]
--		 �����¶ȱ����ֵ����TEMPER_MIN - 1
--	     �����¶ȱ����ֵ����TEMPER_MAX + 1
--	     NTCδ�ӷ���1500
----------------------------------------------------------------------------------------------*/
int16_t LiBat_GetBatTemper(void)
{
#if 1
	return 300;//GetTemp(T_BAT_ADC());
	//return 250;//GetTemp(getAdcValue(LIBAT_LIBATTEMPER_CHNO));
#else
	uint32_t res;

	res = LiBat_ADCGetChannel(LIBAT_LIBATTEMPER_CH,8);
	if((res > 60000) || (res < 350))
		return 1500;
	res = LiBat_Adc2Res((uint16_t)res,LIBAT_NTCRESABOVE);//��λΪŷķ
	return LiBat_Res2Temper((uint16_t)res,950,400);
#endif
}





/*----------------------------------------------------------------------------------------------
-- PID����
-- p_pid		 : ��Ӧ��PID�ṹ������,������ص�ϵ����΢�������
-- TgtPoint 	 : Ҫ���õ�Ŀ��ֵ,��Ϊ��ѹ���������¶ȵ�
-- nowpoint 	 : ��ǰ�Ĳ���ֵ,��Ϊ��ѹ���������¶ȵ�
-- p_ErrorBuffer : ��ֵ��ŵ�����,Ŀǰ����Ϊ16,��¼16�����ڵĲ�ֵ����
-- ErrorBufferCt : ��ֵ���ݵ����²�ֵ�±�
-- ����			 : ���������
----------------------------------------------------------------------------------------------*/
int32_t LiBat_GEPIDCalc(LB_PID *p_pid,int16_t TgtPoint, int16_t nowpoint, int16_t *p_ErrorBuffer,uint8_t *ErrorBufferCt)
{
	int16_t dError,Error;
//	int32_t pid_p,pid_i,pid_d;
	int32_t pid_out = 0;

	p_pid->SetPoint = TgtPoint;
	p_pid->PrevError = p_pid->LastError;
	p_pid->LastError = Error;
	dError = p_pid->LastError - p_pid->PrevError;//��ǰ΢��

	Error = (p_pid->SetPoint - nowpoint);
	*ErrorBufferCt ++;//�ƶ���ֵ�����±�����һ��λ��,�򻺴�Ϊѭ��ʽ,����ƶ������λ��Ϊ�����������һ����ֵ
	*ErrorBufferCt &= 0x0f;
	p_pid->SumError -= p_ErrorBuffer[*ErrorBufferCt];
	p_pid->SumError += Error; // ����
	p_ErrorBuffer[*ErrorBufferCt] = Error;

	pid_out += p_pid->Proportion * (int32_t)Error;// ������+ feedback
	pid_out += p_pid->Integral * p_pid->SumError ; // ������
	pid_out += p_pid->Derivative * dError ; // ΢����

	return pid_out;
}

/*----------------------------------------------------------------------------------------------
-- PID��ر�����ʼ��
-- p_pid		 : ��Ӧ��PID�ṹ������,������ص�ϵ����΢�������
-- TgtPoint 	 : Ҫ���õ�Ŀ��ֵ,��Ϊ��ѹ���������¶ȵ�
-- nowpoint 	 : ��ǰ�Ĳ���ֵ,��Ϊ��ѹ���������¶ȵ�
-- p_ErrorBuffer : ��ֵ��ŵ�����,Ŀǰ����Ϊ16,��¼16�����ڵĲ�ֵ����
-- ErrorBufferCt : ��ֵ���ݵ����²�ֵ�±�
-- ����			 : ���������
----------------------------------------------------------------------------------------------*/
static uint8_t n_chs_status;//LiBat_CurrentPid�ĳ��״̬
void LiBat_InitPID(void)
{
	memset((uint8_t*)ErrorBufferCurrent,0,sizeof(ErrorBufferCurrent));
	memset((uint8_t*)ErrorBufferVolt,0,sizeof(ErrorBufferVolt));
	memset((uint8_t*)&Pid_Current,0,sizeof(&Pid_Current));
	memset((uint8_t*)&Pid_Volt,0,sizeof(&Pid_Volt));
	Pid_Volt.Proportion = LIBAT_VOLT_KP_DEF;
	Pid_Volt.Integral = LIBAT_VOLT_KI_DEF;
	Pid_Volt.Derivative= LIBAT_VOLT_KD_DEF;
	ErrorBufferCurrentCt = 0;//ErrorBufferVoltCt = 0;

	Pid_Current.Proportion = LIBAT_CURRENT_KP_DEF;
	Pid_Current.Integral = LIBAT_CURRENT_KI_DEF;
	Pid_Current.Derivative= LIBAT_CURRENT_KD_DEF;

	PwmOutOld = 0;

	n_chs_status = LB_CHS_NORMAL;

}
/***********************************************************************************************
end �м�㺯��
***********************************************************************************************/


/*----------------------------------------------------------------------------------------------
-- ��������,ÿ��10ms����,��,PID����Ϊ100ms
-- Current 	: Ҫ���õĵ���ֵ,��λ:mA
-- Cycles	: ����ȡƽ��ֵ�ĸ��� ���ڲ��������в���,��ȡȡֵN������ȡƽ��ֵ��Ϊ��PID���ڵĵ���ֵʹ��
--			  ��ֵĿǰȡ10
-- ����		: ����enum __LiBat_PID_Errors
--			  0 ִ�гɹ�
-- 			  1 Ϊ������,һ��Ϊ�����¶ȷ�Χ����
--			  2 Ϊ���ѳ���,���ڴ���״̬
----------------------------------------------------------------------------------------------*/
#define LIBAT_PWMVALUE_WAVE_MAX	2//PID�����PWMֵÿ���ڵı仯����

uint8_t LiBat_chgerr_retry4test = 0;//����������ʧ��,��Ҫ��ͣ������,��Ϊ����ģʽ�µļ�����,����3������
									//����Ϊ�������ʧ��

uint8_t LiBat_PwmCurr;
uint16_t LiBat_CurrNow;
uint8_t LiBat_CurrentPid(uint16_t Current,uint8_t Cycles)
{
#if 1

	//PWM���ĳ�������һ���仯��ֵ,��Ҫ���ж�������Բ�����ȡƽ��ֵ
	static uint8_t ct = 0;//��β����ļ�����
	static uint32_t SumCur = 0, SumVolt = 0; //��ε��������ĺ�
	static uint16_t CuteCurrentCt = 0xffff;//С�������ļ�����,������ѹ�ﵽ��ѹ��С�ڽ�������ʱ,
											//��ֵ����0����ʼ����,������LIBAT_CHARGESTOP_TIMESʱֹͣ���
	static uint16_t Standby2ChkVoltCt = 0xffff;//����ʱ����ѹ�ļ�����
	//static int16_t PwmOutOld = 0;//��һ�ε�PWM���ֵ
	int32_t PwmOut;//��ǰ�������õ���PWM���ֵ
	int16_t Temper;//����¶Ȳ���ֵ
#if LIBAT_LOG_OUT
	static uint8_t logoutct = 0;
#endif
	uint16_t n_rd_pwm;//��ȡ��ǰ��PWM��ֵ,������

	static uint16_t n_cherror_timer = 0;

	uint8_t res = LB_ERROR_NONE;

	SumVolt += LiBat_GetBatVolt();//��ȡ��ص�ѹ
	SumCur += LiBat_GetChargeCurrent();
	ct ++;
	if(ct < Cycles)
	{
		return LB_ERROR_NONE;
	}
	ct = 0;
	SumCur /= Cycles;
	SumVolt /= Cycles;

	Temper =LiBat_GetBatTemper();

	sys->sys_charge_cur = SumCur;//2018-10-09 jzz
	if(sys->sState == SYS_TEST_BD)//����ģʽʱ,��¼������
	{
		BDTST_SET_CURR(SumCur);
	}

	if( Temper > 500 && Temper < 1500)//�¶��쳣��ֹͣ���                    Temper < 0 ||  ȥ�������¹��±���  2018 12 10 am
	{
		LiBat_SetPwrChargePwm(0);
		if(Temper == 1500)//NTC���ϻ�δ������Ϊ��ز�����
		{
#if LIBAT_LOG_OUT
			PRINTF("Invalid Battery\r\n");
#endif
			res = LB_ERR_TEMPOVER;
		}
		else
		{
#if LIBAT_LOG_OUT
			PRINTF("Temperature over limit %d\r\n",Temper);
#endif
			res = LB_ERR_NOBAT;
		}
		PwmOutOld = 0;
		n_chs_status = LB_CHS_NOCHARGE;
	}
	else
	{
		if(n_chs_status != LB_CHS_ERROR && SumCur < LIBAT_CHARGESTOP_CURRENT && CuteCurrentCt < 0xfff0)
		{

			CuteCurrentCt ++;

			if(CuteCurrentCt > LIBAT_CHARGESTOP_TIMES)
			{
				CuteCurrentCt = 0xfff1;
				n_chs_status = LB_CHS_STANDBY;
				PwmOutOld = 0;
				LiBat_SetPwrChargePwm(0);
#if LIBAT_LOG_OUT
				PRINTF("LiBat Charge over!\r\n");
#endif
			}
		}
		else if(n_chs_status != LB_CHS_ERROR && SumCur > (LIBAT_CHARGECURRENT_SET * 1.5))		///����   �ټ��Ϲ�ѹ 16.8V
		{
			n_chs_status = LB_CHS_ERROR;
			PwmOut=PwmOutOld = 0;
			//TIM_ForcedOC3Config(TIM1, TIM_ForcedAction_InActive);
			LiBat_SetPwrChargePwm(0);
			if(sys->sState == SYS_CHARGE)
				log_printf("^^^^^LB_CHS_ERROR--%d^^^^^^\r\n",SumCur);
			#if 1
			#if 1
			GPIO_InitTypeDef GPIO_InitStructure;
			LiBat_ExitChargeMode();
		       //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��	GPIOE.9
			GPIO_InitStructure.GPIO_Pin = LIBAT_CHPWM_PIN ; //TIM1_CH1
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(LIBAT_CHPWM_PORT, &GPIO_InitStructure);//��ʼ��GPIO
			#endif
			LIBAT_CHPWM_PORT->ODR &= ~LIBAT_CHPWM_PIN;
			//TIM_Cmd(TIM1, DISABLE);  //ʹ��TIM1

			//delay_ms(200);

			//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			//GPIO_Init(LIBAT_CHPWM_PORT, &GPIO_InitStructure);//��ʼ��GPIO
			#endif
			n_cherror_timer = 0;
		}
	}
	if(LiBat_isOnDock() == 0)//����PWMΪ�����ԭ��,�������������δ����ʱ,��Ҫ��PwmOutOld��0,�Է��ٽ���ʱ��������
		PwmOutOld = 0;
	switch(n_chs_status)
	{
		case LB_CHS_NORMAL:
			if(SumVolt > LIBAT_CHARGEVOLT_MAX)
			{
				if(sys->sState == SYS_CHARGE)
					log_printf("volt overload\r\n");
				PwmOutOld = PwmOutOld > 1 ? (PwmOutOld - 1):0;
				PwmOut = PwmOutOld;
				res = LB_ERR_VOLTOVER;
				if(CuteCurrentCt == 0xffff)
				{
				//	CuteCurrentCt = 0;
#if LIBAT_LOG_OUT
				PRINTF("LiBat CuteCurrent!\r\n");
#endif
				}
				//LiBat_SetPwrChargePwm(0);
			}
			else
			{
				PwmOut = LiBat_GEPIDCalc(&Pid_Current,Current,SumCur,ErrorBufferCurrent,&ErrorBufferCurrentCt)/200;
				LiBat_CurrNow = SumCur;
				//PRINTF("LB_PID:%d,%d\r\n",PwmOut,PwmOutOld);
				PwmOut += 127;
				if(PwmOut > LIBAT_PWM_MAX)PwmOut = LIBAT_PWM_MAX;
				if(PwmOut < 0)PwmOut = 0;
				if(PwmOut - PwmOutOld > LIBAT_PWMVALUE_WAVE_MAX)
				{
					PwmOut = PwmOutOld + LIBAT_PWMVALUE_WAVE_MAX;
					if(PwmOut > LIBAT_PWM_MAX)PwmOut = LIBAT_PWM_MAX;
					//log_printf("s1\r\n");
				}
				else if(PwmOut - PwmOutOld < -LIBAT_PWMVALUE_WAVE_MAX)
				{
					PwmOut = PwmOutOld - LIBAT_PWMVALUE_WAVE_MAX;
					if(PwmOut < 0)PwmOut = 0;
					//log_printf("s2\r\n");
				}
				PwmOutOld = PwmOut;
			}
			LiBat_PwmCurr = PwmOut & 0xff;
			//if(PwmOut > 255)
			//	PwmOut =254;
		//	PRINTF("LB_PID2:%d,%d\r\n",PwmOut,PwmOutOld);
			LiBat_SetPwrChargePwm((uint16_t)PwmOut);
			break;
		case LB_CHS_STANDBY:
			if(Standby2ChkVoltCt == 0xffff)
				Standby2ChkVoltCt = 0;
			else
			{
				Standby2ChkVoltCt ++;
				if(Standby2ChkVoltCt > LIBAT_CHARGERESTART_DELAY)
				{
					Standby2ChkVoltCt = 0;
					if(SumVolt < LIBAT_CHARGERESTART_VOLT)
					{
						Standby2ChkVoltCt = 0xffff;
						CuteCurrentCt = 0xffff;
						n_chs_status = LB_CHS_NORMAL;
						if(sys->sState == SYS_CHARGE)
							log_printf("LiBat Charge restart!\r\n");
					}
				}
			}
			PwmOut=PwmOutOld = 0;
			LiBat_SetPwrChargePwm(0);
			break;
		case LB_CHS_ERROR:
			PwmOut=PwmOutOld = 0;
			if(n_cherror_timer ++ > 50)
			{
				n_cherror_timer = 0;
				n_chs_status = LB_CHS_NORMAL;
				LiBat_InitPwrChargePwm();

				//TIM_ForcedOC3Config(TIM1, TIM_ForcedAction_Active);
				if(sys->sState == SYS_CHARGE)
					log_printf("return to charge...\r\n");
				else if(sys->sState == SYS_TEST_BD)
				{
					if(LiBat_chgerr_retry4test ++ > 2)
					{
						res = LB_ERR_RETRY;
					}
				}
			}
			if(LiBat_GetPwrChargePwm() != 0)
				LiBat_SetPwrChargePwm(0);
			break;
		case LB_CHS_NOCHARGE:
			if((Temper > 5) || (Temper < 360))
				n_chs_status = LB_CHS_NORMAL;
			PwmOut = 0;
			LiBat_SetPwrChargePwm(PwmOut);
		default:
			break;
	}
#if LIBAT_LOG_OUT
	if(logoutct > 5)
	{
		logoutct = 0;
		//PwmOut = 0;
		if(sys->sState == SYS_CHARGE)
		{
			www_idleintel_com();
			n_rd_pwm = LiBat_GetPwrChargePwm();
			log_printf("volt=%d,cur=%d,tmp=%d,pwm=%d-%d(%d,%d,%d,%d)%f\r\n",SumVolt,SumCur,Temper,PwmOut,n_rd_pwm,res,CuteCurrentCt,(GPIOE->IDR & GPIO_Pin_13) != 0,n_chs_status,sys->angle);
		}
	}
	else
		logoutct ++;
#endif
	//��͵ı�����0
	SumVolt = 0;
	SumCur = 0;
	return res;
#else
	uint16_t NowCurrent;
	int32_t PwmOut;
	int16_t Temper;
	Temper = LiBat_GetBatTemper();
	if((Temper < 0) || (Temper > 400))
	{
		LiBat_SetPwrChargePwm(0);
		return -1;
	}
	NowCurrent = LiBat_GetChargeCurrent();
	PwmOut = LiBat_GEPIDCalc(&Pid_Current,Current,NowCurrent,ErrorBufferCurrent,&ErrorBufferCurrentCt)/8;
	if(PwmOut > LIBAT_PWM_MAX)PwmOut = LIBAT_PWM_MAX;
	if(PwmOut < 0)PwmOut = 0;
	LiBat_PwmCurr = PwmOut & 0xff;
	LiBat_SetPwrChargePwm((uint16_t)PwmOut);
	return 0;
#endif
}

uint8_t LiBat_GetPwmCurr(void)
{
	return LiBat_PwmCurr;
}

uint16_t LiBat_GetCurrNow(void)
{
	return LiBat_CurrNow;
}

/*----------------------------------------------------------------------------------------------
-- ��ѹPID����
-- Volt 	: Ҫ���õĵ�ѹֵ,��λ:mv
-- NowVolt  : ��ǰ�ĵ�ѹֵ,���δ֪��д��0xffff,��λ:mv
-- ����		: ִ�гɹ�����0
-- 			  ʧ�ܷ���-1,һ��Ϊ�����¶ȷ�Χ����
----------------------------------------------------------------------------------------------*/
//volatile uint16_t LiBat_VoltValue;
/*
uint8_t LiBat_VoltPid(uint16_t Volt, uint16_t NowVolt)
{
	int32_t PwmOut;
	int16_t Temper;
	Temper = LiBat_GetBatTemper();
	if((Temper < 0) || (Temper > 400))
	{
		LiBat_SetPwrChargePwm(0);
		return -1;
	}
	NowVolt = (NowVolt == 0xffff)?LiBat_GetBatVolt():NowVolt;
	//LiBat_VoltValue = NowVolt;
	//PwmOut = LiBat_GEPIDCalc(&Pid_Volt,Volt,NowVolt,ErrorBufferVolt,&ErrorBufferVoltCt)/500;
	PwmOut = LiBat_GEPIDCalc(&Pid_Volt,Volt,NowVolt,ErrorBufferVolt,&ErrorBufferVoltCt)/4000;
	if(PwmOut > LIBAT_PWM_MAX)PwmOut = LIBAT_PWM_MAX;
	if(PwmOut < 0)PwmOut = 0;
	LiBat_SetPwrChargePwm((uint16_t)PwmOut);
	return 0;
}
*/

/*----------------------------------------------------------------------------------------------
-- �رճ�ң�ؽ��պ�����������������Դ,���������Դ/��༰��������Դ
-- OnOff 1 On 0 Off
----------------------------------------------------------------------------------------------*/
void LiBat_TurnOnOtherPwr(uint8_t OnOff)
{
/*
	if(!OnOff)
	{
		GPIO_ResetBits(LIBAT_IRDIST_PWR_PORT, LIBAT_IRDIST_PWR_PIN);//�رղ�����ĵ�Դ
		GPIO_ResetBits(LIBAT_MOTOR_PWR_PORT, LIBAT_MOTOR_PWR_PIN);//�ر����е���ĵ�Դ
	}
	else
	{
		GPIO_SetBits(LIBAT_IRDIST_PWR_PORT, LIBAT_IRDIST_PWR_PIN);//�򿪲�����ĵ�Դ
		GPIO_SetBits(LIBAT_MOTOR_PWR_PORT, LIBAT_MOTOR_PWR_PIN);//�����е���ĵ�Դ
	}
	*/
}


/*----------------------------------------------------------------------------------------------
-- ���ģ��������ʼ��
----------------------------------------------------------------------------------------------*/
void LiBat_HalInit(void)
{


	LiBat_TurnOnOtherPwr(0);
	LiBat_InitPwrChargePwm();
	//LiBat_InitAdc();
	LiBat_InitPID();
}


/*----------------------------------------------------------------------------------------------
-- ���ģ���˳�
-- �����ɻ��߻����˹���ģʽ�ı��,���ڲ���MCUƬ�����踴�õĹ�ϵ,��Ҫ���³�ʼ��,�Իָ��ֳ�
-- ������Ҫ�Ƕ�TIM3���³�ʼ��һ��,����֮ǰ����Ҫ�����ر�
----------------------------------------------------------------------------------------------*/
void LiBat_ExitChargeMode(void)
{
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//���PWM����ΪGPIOģʽ,���õ�
	GPIO_InitStructure.GPIO_Pin =  LIBAT_CHPWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(LIBAT_CHPWM_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LIBAT_CHPWM_PORT, LIBAT_CHPWM_PIN);

	TIM_Cmd(LIBAT_CHPWM_TIM, DISABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xff;
	TIM_TimeBaseStructure.TIM_Prescaler = 15;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TIM_CKD_DIV2;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(LIBAT_CHPWM_TIM, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	//TIM_OC1Init(LIBAT_CHPWM_TIM, &TIM_OCInitStructure);
	//TIM_OC1PreloadConfig(LIBAT_CHPWM_TIM, TIM_OCPreload_Enable);

	//TIM_SetCompare1(TIM3, (0x7f));

	//TIM_OC2Init(LIBAT_CHPWM_TIM, &TIM_OCInitStructure);
	//TIM_OC2PreloadConfig(LIBAT_CHPWM_TIM, TIM_OCPreload_Enable);

	//TIM_SetCompare2(TIM3, (0x7f));

	TIM_OC3Init(LIBAT_CHPWM_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(LIBAT_CHPWM_TIM, TIM_OCPreload_Enable);

	/* TIM3 enable counter */
	TIM_Cmd(LIBAT_CHPWM_TIM, ENABLE);
	TIM_CtrlPWMOutputs(LIBAT_CHPWM_TIM, ENABLE);
#endif
	TIM1->CCR3 = 1000;
	LiBat_TurnOnOtherPwr(1);
}


char libat_charge_task(void)
{
		//static int cc=0;
	   	if(sys->sState != SYS_CHARGE)
	  	{
	  		//GPIO_Config();
	   		return 0;
	   	}
		//������ȥ�ӳ��׮����

	  sys->auto_lamp_sta=0;
	   if(TIM5->CNT >=10000)
	   {
	   		TIM5->CNT =0;
	   		
	  	 	////LiBat_CurrentPid(LIBAT_CHARGECURRENT_SET,10);
	  	 	if((0 == CHARGE_DONE_DETECT()) && ((sys->charge_sta & CHARG_DONE) != CHARG_DONE ) )
	  	 	{
				sys->charge_sta |= CHARG_DONE;	//�����ɱ�־,�ṩ���Կ���ʾled	
				log_printf("[libat_charge_task]charge done,full and go=%d\r\n",sys->full2go);
	  	 	}
	  	 	if((sys->charge_sta & CHARG_DONE) == CHARG_DONE)
	  	 	{				
				if(sys->full2go && ((sys->charge_sta & DOCK_CHARGE) == DOCK_CHARGE))
				{
					log_printf("now cleanning....\r\n");
					sys->sState = SYS_NAVIGAT;
					sys->gey_key_clear_err = 1;		
					ny3p_play(VOICE_M_START);
					LED3_ON();
					return 1;
				}
	  	 	}
	   }

	   if(    (!EXTERAL_AC_DETECT()) && (!DOCK_DETECT())   )
	   {
	   		log_printf("no charge... sta=%x,%d,%d\r\n",sys->charge_sta,EXTERAL_AC_DETECT(),DOCK_DETECT());
	   		{
				int i=0;
				for(i=0;i<200;i++)
				{
					delay_ms(5);
					if(EXTERAL_AC_DETECT() || DOCK_DETECT())
						break;
					//log_printf("doc sta=(%d,%d)\r\n",EXTERAL_AC_DETECT() , DOCK_DETECT());
				}
				if( i>=190)
				{
					ny3p_play(VOICE_DIDI);
					/*
					if(sys->charge_sta & DOCK_CHARGE)		//���ӳ�����
					{

					}
					*/
			   		if(sys->charge_sta & AC_CHARGE)
			   		{
			   			sys->sState = SYS_IDLE;
			   			//ny3p_play(VOICE_CHARGE_M);
			   		}
			   		else
			   		{
						log_printf("init to dock sta\r\n");
						//micony201809
						 LED4_ON();
						reset_angle();
						///2018-10-18 jzz
						timer1_init();					//ʱ��1������������ˢ
						timer2_init();					//������ɨ������������
						MOTOR_POWER_ON();
						MOTOR_CTRL_SIDE(DOCK_SIDE_PWM_L,DOCK_SIDE_PWM_R);
						///
						//sys->angle = 0;
						//MOTOR_POWER_ON();
						//MOTOR_CTRL1(0,0,0,0);
						//MOTOR_CTRL_SIDE(DOCK_SIDE_PWM_L,DOCK_SIDE_PWM_R);
						motor_run(GO_STOP,0,0,0);
						//reset_gyro(1);
						usart_init_info();

						//delay(100);
						init_charge(SYS_IDLE);
						//delay(100);
						delay_sensor(100);
						if(sys->sState == SYS_IDLE)///2018-05-24 jzz
							return 0;
						sys->sState = SYS_DOCK;
						sys->wifiAreaNoCalc_en = 1;
#if WIFICONFIG
		init_stream_clean_id();
#endif
					//	navigat_init(5);

						log_printf("go back...\r\n");
						motor_back_off(80);
						if(sys->sState == SYS_IDLE)///2018-05-24 jzz
							return 0;
							//delay_ms(300);
						//micony201809
						//MOTOR_POWER_ON();
						//MOTOR_CTRL1(0,0,0,0);
						//delay(1000);
						//reset_angle();
						get_gyro_ofs();
						get_gyro_org();
						www_idleintel_com();
						log_printf("sys angle=%3.1f\r\n",sys->angle);
						motor_run(GO_STOP,0,0,0);
						//delay(200);
						//ny3p_play(VOICE_DIDI);
#if DUST_BOX_FULL_CHECK
	dust_full_check_pro(2);		//��ɨ�������³����ۻ�ʱ��
#endif
#if STRAINER_ALARM_CHECK//����
	strainer_alarm_check_pro(2);
#endif
#if SIDE_RUSH_ALARM_CHECK
	side_rush_alarm_check_pro(2);
#endif
#if MAIN_RUSH_ALARM_CHECK
	main_rush_alarm_check_pro(2);
#endif

					//	if(sys->angle < 45 || sys->angle > 270)
						turn_to_deg(180);

						//robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,80);
						//robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,80);
						if(sys->sState == SYS_IDLE)///2018-05-24 jzz
							return 0;
						navigat->angle = 180;

						log_printf("turn to 180,go...\r\n");
						//micony201809
						motor_go_forwark(5000,NO_SIDE_NEAR,NULL);  //  20200311    hw=2000
						if(sys->sState == SYS_IDLE)		///2018-05-24	jzz
						 	return 0;
						//timer1_init();					//ʱ��1������������ˢ
						//timer2_init();					//������ɨ������������
						//ny3p_play(VOICE_M_DOCK);
						sys->sState = SYS_DOCK;
						//pwifistream->wifiAreaNoCalc = 1;
						//MOTOR_POWER_ON();

						//MOTOR_CTRL(DOCK_SIDE_PWM,DOCK_SIDE_PWM,DOCK_DUST_PWM,DOCK_MID_PWM);
						//init_charge(SYS_IDLE);
						//init_sys_sta(SYS_DOCK);
			   		}

			sys->wifistatechange = 1;
					return 1;
				}
	   		}
	   }
	return 0;

}

