/********************************************************************
* ��Ȩ �� -------------micony--------------
* �ļ� :motor.c
* �汾 ��
* ���� ��
* ���� : Τ����
* ʱ�� ��2015-2-13
* ˵�� ��
*/
#include "sys.h"


motor_t motor;

char motorStr[7][6]={"STOP","FORW","BACK","LEFT","RIGT","RUND","SLOW"};


__IO uint16_t *left_pwm,*right_pwm;

/*
��ɨ���
*/

//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
/*
���ˢ���	53	PE14	���ˢ���PWM����   TIM1_CH4
	54	PE13 =>�ĳ�������	���ˢ������⣨H/L��ƽ��⣩
�ұ�ˢ���	55	PE9=>PE11	�ұ�ˢ���PWM����
	56	PE11 =>PE9	�ұ�ˢ������⣨H/L��ƽ��⣩
ʱ�ӳ�ʼ������ʱ����ʼ������ʱ��1.����
*/
void timer1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_DeInit(TIM1);//��ʼ��TIM1�Ĵ���
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOE  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //Timer1��ȫ��ӳ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

	/*
	GPIO_InitStructure.GPIO_Pin =  LIBAT_CHPWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(LIBAT_CHPWM_PORT, &GPIO_InitStructure);
	LIBAT_CHPWM_PORT->ODR &= ~LIBAT_CHPWM_PIN;
	*/
	//GPIO_ResetBits(LIBAT_CHPWM_PORT, LIBAT_CHPWM_PIN);

    //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =(4-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse =0;
	/*
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	//��ʼ��TIM1 Channel1 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse =1000;*/
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1

/*
	//���PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC3Init(LIBAT_CHPWM_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(LIBAT_CHPWM_TIM, TIM_OCPreload_Enable);

	*/

	//TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 	TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//����PMW�����
	//TIM1->CCR2=700;
	//TIM1->CCR4=700;
	//while(1);

	//EN_TM1_CH1();
	//EN_TM1_CH4();
	//TIM1->CCR1=500;
	//TIM1->CCR4=500;

}

void timer2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_DeInit(TIM2);//��ʼ��TIM1�Ĵ���
 	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); //Timer2��ȫ��ӳ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	// 72M /72 / 500 = 2k
	TIM_TimeBaseStructure.TIM_Period = 500; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse =0;

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 	TIM_ARRPreloadConfig(TIM2, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM1
	//TIM_CtrlPWMOutputs(TIM2, ENABLE);//����PMW�����
}

//int ccr3_1=0;

//lock_Init(9); //ϵͳʱ������ delay_init(72); //��ʱ��ʼ��
/*
�����ȵ���ģʽ���Բ���һ����TIMx_ARR�Ĵ���ȷ��Ƶ�ʡ���TIMx_CCRx�Ĵ���ȷ��ռ�ձȵ��źš�
��TIMx_CCMRx�Ĵ����е�OCxMλд�롯110��(PWM ģʽ1) ��111��(PWM ģʽ2) ���ܹ�����������ÿ��OCx���ͨ������һ·PWM��
����ͨ������TIMx_CCMRx�Ĵ�����OCxPEλʹ����Ӧ��Ԥװ�ؼĴ�����
���Ҫ����TIMx_CR1 �Ĵ�����ARPE λ��( �����ϼ��������ĶԳ�ģʽ��)ʹ���Զ���װ�ص�Ԥװ�ؼĴ�����
��������һ�������¼���ʱ��Ԥװ�ؼĴ������ܱ����͵�Ӱ�ӼĴ�����
����ڼ�������ʼ����֮ǰ������ͨ������TIMx_EGR�Ĵ����е�UGλ����ʼ�����еļĴ�����
OCx�ļ��Կ���ͨ�������TIMx_CCER�Ĵ����е�CCxPλ���ã�����������Ϊ�ߵ�ƽ��Ч��͵�ƽ��Ч��
OCx�����ʹ��ͨ��(TIMx_CCER��TIMx_BDTR�Ĵ�����)CCxE��CCxNE��MOE��OSSI��OSSR λ����Ͽ��ơ�
���TIMx_CCER�Ĵ�������������PWMģʽ(ģʽ1��ģʽ2) �£�TIMx_CNT��TIMx_CCRxʼ���ڽ��бȽϣ�(
 ���ݼ������ļ�������)��ȷ���Ƿ����TIMx_CCRx��TIMx_CNT����TIMx_CNT��TIMx_CCRx��
 ����TIMx_CR1 �Ĵ�����CMSλ��״̬����ʱ���ܹ��������ض����PWM�źŻ���������PWM�źš�

����������Ϣ����ʾ�����ǿ��Թ��ɳ��Ĵ��������Ĳ��裺

��1�� ʹ����Ӧ�Ķ�ʱ��ʱ���ߡ�

��2�� ����tim1��·pwm�������Ϊ���ù��������

��3�� ����tim1�ķ�Ƶ�Ⱥͼ���װ��ֵ��

��4�� ������Ӧͨ����pwm���ģʽ��ʹ��Ԥװ�ء�

��5�� ���Զ���װ��Ԥװ������λ��

��6�� ������Ҫѡ�����ϻ��������¼���ģʽ��

��7�� ����Ӧͨ�������ʹ�ܡ�

��8�� ���ø�ͨ����ռ�ձȡ�

��9�� ��������OC��OCN��

��10�� ���ʹ�ܶ�ʱ��1.

���ڷ�Ƶ�Ⱥͼ���װ��ֵ��ѡ��

Tim1�����ʱ��Ƶ��Ϊ72M������������õ�һ��Ƶ��Ϊ4KHZ��ռ�ձ�0~100%����Ϊ1��pwm��
���ݼ�������ʱ��Ƶ��(CK_CNT) ����f CK_PSC/( PSC[15:0]+1)��
��Ϊռ�ձ�0~100%����Ϊ1���������ǰѼ���ֵ��Ϊ100.
�Ƚ�ֵ������Ϊ0~100֮�䣬ʵ��ռ�ձȵĵ��ڡ�
��������ʱ��Ƶ��(CK_CNT)ӦΪ4KHZ X 100=400KHZ������PSC = 179��ARR=100���в����׵��뿴�ֲᡣ

*/

void motor_timer_init(void)
{

 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 //  TIM_ICInitTypeDef  	   TIM_ICInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


     /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOD clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  TIM_ARRPreloadConfig(TIM4, ENABLE);





	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;

  TIM_TimeBaseStructure.TIM_Prescaler = (4-1);


  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Down;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 800;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);


  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);

	l_motor_stop();
	r_motor_stop();

}

void l_motor_stop(void)
{
	DIS_CH3();
	DIS_CH4();
}

void l_motor_set_pwm(uint8_t HL,int pwm)
{
	EN_CH3();
	EN_CH4();
	if(HL==GO_FORWARD)
	{
#if(1 == MOTOR_ZHENG)
		TIM4->CCR4=0;			//�ߵ�ƽ
		TIM4->CCR3=MAX_PWM -pwm;
#else
		TIM4->CCR3=0;			//�ߵ�ƽ
		TIM4->CCR4=MAX_PWM -pwm;
#endif
	}else
	{
#if(1 == MOTOR_ZHENG)
		TIM4->CCR3=0;			//�ߵ�ƽ
		TIM4->CCR4=MAX_PWM -pwm;
#else
		TIM4->CCR4=0;			//�ߵ�ƽ
		TIM4->CCR3=MAX_PWM -pwm;
#endif
	}

	sys->pwm1 = pwm;
}

// motor2 input1��PA6 input2��PA7  PWM:PA9
void r_motor_stop(void)
{
	DIS_CH1();
	DIS_CH2();
}

void motor_wheel_backward(u8 wheel,u16 speed)
{
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_Pulse = speed;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High ;
#if(1 == MOTOR_ZHENG)
	if( READ_BIT(wheel,RIGHT_WHEEL))
	{
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);
		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
		right_pwm= &(TIM4->CCR4);
	}
	if(READ_BIT(wheel,LEFT_WHEEL))
	{
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Enable);
		left_pwm  = &(TIM4->CCR2);
	}
#else
	if( READ_BIT(wheel,RIGHT_WHEEL))
	{
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);
		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
		right_pwm= &(TIM4->CCR3);
	}
	if(READ_BIT(wheel,LEFT_WHEEL))
	{
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);
		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
		left_pwm  = &(TIM4->CCR1);
	}
#endif
}


void motor_wheel_forward(u8 wheel,u16 speed)
{
	TIM_OCInitTypeDef  TIM_OCInitStruct;



	TIM_OCInitStruct.TIM_Pulse = speed;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High ;

#if(1 == MOTOR_ZHENG)
	if( READ_BIT(wheel,RIGHT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
		 right_pwm= &(TIM4->CCR3);
	}

	if(READ_BIT(wheel,LEFT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Enable);
		left_pwm = &(TIM4->CCR1);
	}
#else
	if( READ_BIT(wheel,RIGHT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
		 right_pwm= &(TIM4->CCR4);
	}

	if(READ_BIT(wheel,LEFT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
		left_pwm = &(TIM4->CCR2);
	}
#endif
}


void motor_wheel_stop(u8 wheel)
{
	TIM_OCInitTypeDef  TIM_OCInitStruct;



	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High ;



	if( READ_BIT(wheel,RIGHT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_3,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
		//*right_pwm =1000;

	}

	if(READ_BIT(wheel,LEFT_WHEEL)){

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC1Init(TIM4,&TIM_OCInitStruct);

		TIM_OCInitStruct.TIM_OCMode = TIM_ForcedAction_Active;
		TIM_OC2Init(TIM4,&TIM_OCInitStruct);
		TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
		TIM_CCxCmd(TIM4,TIM_Channel_2,TIM_CCx_Enable);
		//*left_pwm=1000;
	}
}

void r_motor_set_pwm(uint8_t HL,int pwm)
{

	EN_CH1();
	EN_CH2();
	if(HL==GO_FORWARD)
	{
#if(1 == MOTOR_ZHENG)
		TIM4->CCR2=0;			//�ߵ�ƽ
		TIM4->CCR1=MAX_PWM - pwm;
#else
		TIM4->CCR1=0;			//�ߵ�ƽ
		TIM4->CCR2=MAX_PWM - pwm;
#endif
	}else
	{
#if(1 == MOTOR_ZHENG)
		TIM4->CCR1=0;			//�ߵ�ƽ
		TIM4->CCR2=MAX_PWM - pwm;
#else
		TIM4->CCR2=0;			//�ߵ�ƽ
		TIM4->CCR1=MAX_PWM - pwm;
#endif
	}

//	sys->pwm2 = MAX_PWM - pwm;

	//TIM1->CCR2=pwm;

}
/*
    ���벶��ģʽ�����������������Ȼ��߲���Ƶ�ʡ�STM32�Ķ�ʱ��������TIM6��TIM7��������ʱ���������벶���ܡ�STM32�����벶�񣬼򵥵�˵����ͨ�����TIMx_CHx�ϵı����źţ��ڱ����źŷ������䣨����������/�½��أ���ʱ�򣬽���ǰ��ʱ����ֵ��TIMx_CNT����ŵ���Ӧ��ͨ���Ĳ���/�ȽϼĴ棨TIMx_CCRx�����棬���һ�β���ͬʱ���������ò���ʱ�Ƿ񴥷��ж�/DMA ��.

     ���磺�����õ�TIM5_CH1������ߵ�ƽ����Ҳ����Ҫ���������벶��Ϊ�����ؼ�⣬��¼���������ص�ʱ��TIM5_CNT��ֵ��Ȼ�����ò����ź�Ϊ�½��ز��񣬵��½��ص���ʱ���������񣬲���¼��ʱ��TIM5_CNTֵ��������ǰ������TIM5_CNT֮����Ǹߵ�ƽ������ͬʱTIM5�ļ���Ƶ��������֪���ģ��Ӷ����Լ�����ߵ�ƽ�����׼ȷʱ�䡣

     ����TIMx_ARR��TIMx_PSC���������Ĵ����������Զ���װ��ֵ��TIMx��ʱ�ӷ�Ƶ��

     ������������/�Ƚ�ģʽ�Ĵ���1��TIMx_CCMR1������Ĵ��������벶���ʱ�򣬷ǳ����ã�TIMx_CCMR1���������2��ͨ�������ã��Ͱ�λ[7��0]���ڲ���/�Ƚ�ͨ��1�Ŀ��ƣ����߰�λ[15��8]�����ڲ���/�Ƚ�ͨ��2�Ŀ��ƣ���ΪTIMx����CCMR2����Ĵ��������Կ���֪��CCMR2����������ͨ��3��ͨ��4�������STM32�ο��ֲᡷ290ҳ��14.4.8�ڣ���

    �����õ�TIM5�Ĳ���/�Ƚ�ͨ��1�������ص����TIMx_CMMR1��[7:0]λ����ʵ��8λ�������ƣ���

    ������������/�Ƚ�ʹ�ܼĴ�����TIMx_CCER��

    �����������ٿ���DMA/�ж�ʹ�ܼĴ�����TIMx_DIER��������Ҫ�õ��ж������������ݣ����Ա��뿪��ͨ��1�Ĳ���Ƚ��жϣ���CC1IE����Ϊ1��

    ���ƼĴ�����TIMx_CR1������ֻ�õ����������λ��Ҳ��������ʹ�ܶ�ʱ���ģ�

    ���������������/�ȽϼĴ���1��TIMx_CCR1���üĴ��������洢������ʱ��TIMx_CNT��ֵ�����Ǵ�TIMx_CCR1�Ϳ��Զ���ͨ��1������ʱ�̵�TIMx_CNTֵ��ͨ�����β���һ�������ز���һ���½��ز��񣩵Ĳ�ֵ���Ϳ��Լ�����ߵ�ƽ����Ŀ�ȡ�

    ʹ�ܲ���͸����жϣ�����TIM5��DIER�Ĵ�����
   ��Ϊ����Ҫ������Ǹߵ�ƽ�źŵ��������ԣ���һ�β����������أ��ڶ��β���ʱ�½��أ������ڲ���������֮�����ò������Ϊ�½��أ�ͬʱ���������Ƚϳ�����ô��ʱ���ͻ����������������������������Ͳ�׼�ˡ��������£����Ƕ����ж������������Ա��뿪�������жϺ͸����жϡ�
*/
#if 0
#define  TIMER TIM2
void init_tim2_camp(u16 psc, u16 arr, u8 way, u8 dir)
{
	RCC->APB1ENR |= 1 << 0;   //ʹ�ܶ�ʱ��2ʱ��
	RCC->APB2ENR |= 1 << 2; //ʹ��PortA
/*
	switch (way)
	{
		case 1:
			GPIOA->CRL &= 0xfffffff0;	  	//PA0
			GPIOA->CRL |= 0x00000008;
			break;
		case 2:
	*/
			GPIOA->CRL &= 0xffffff00;   //PA0 PA1
			GPIOA->CRL |= 0x00000088;
	/*
			break;
		case 3:
			GPIOA->CRL &= 0xfffff000;
			GPIOA->CRL |= 0x00000888;
			break;
		case 4:
			GPIOA->CRL &= 0xffff0000;
			GPIOA->CRL |= 0x00008888;
			break;
	}
	*/

	TIMER->PSC = psc;
	TIMER->ARR = arr;

	switch (way)
	{
		case 4:
			TIMER->CCMR2 |= 1 << 8;
			if (dir == 0)
				TIMER->CCER |= 1 << 13; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 13); //�����ز���
			TIMER->CCER |= 1 << 12;
			TIMER->DIER |= 1 << 4;
		case 3: //CCR3 PA2
			TIMER->CCMR2 |= 1 << 0;
			if (dir == 0)
				TIMER->CCER |= 1 << 9; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 9); //�����ز���
			TIMER->CCER |= 1 << 8;
			TIMER->DIER |= 1 << 3;
		case 2: //CCR2 PA1
			TIMER->CCMR1 |= 1 << 8; //CCR2����ͨ������:����
			TIMER->CCMR1 |= 5 << 12;		//�˲�����Ϊ5
			if (dir == 0)
				TIMER->CCER |= 1 << 5; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 5); //�����ز���
			TIMER->CCER |= 1 << 4; //CCR2ͨ������ʹ��
			TIMER->DIER |= 1 << 2; //CCR2ͨ���������ж�
		case 1: //>CCR1 PA0
			TIMER->CCMR1 |= 1 << 0; //CCR1����ͨ����������
			TIMER->CCMR1 |= 5 << 4;		//�˲�����Ϊ5
			if (dir == 0)
				TIMER->CCER |= 1 << 1; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 1); //�����ز���
			TIMER->CCER |= 1 << 0;     //CCR1����ʹ��
			TIMER->DIER |= 1 << 1;  //CCR1ͨ���������ж�
			break;
	}
	TIMER->DIER |= 1 << 0;     //��������ж�
//	MY_NVIC_Init(1, 2, TIM2_IRQChannel, 2);     //�ж�
	TIMER->CR1 = 0x01;    //ʹ�ܶ�ʱ��
	TIMER->SR &= ~(1 << 0);
}


void init_tim4_camp(u16 psc, u16 arr, u8 way, u8 dir)
{
	GPIO_InitTypeDef GPIO_InitStructure;
// TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
// TIM_ICInitTypeDef  	   TIM_ICInitStructure;

	//PB8 TIM4 CH3   pb8 TIM4 ch4 ��ʱ���ں���Ľ���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;// | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	TIM4->PSC = psc;
	TIM4->ARR = arr;
  /*
	switch (way)
	{
		case 4:


			//CH4
			TIMER->CCMR2 |= 1 << 8;
			TIMER->CCMR2 |= 5 << 12;		//�˲�����Ϊ5
			if (dir == 0)
				TIMER->CCER |= 1 << 13; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 13); //�����ز���
			TIMER->CCER |= 1 << 12;
			TIMER->DIER |= 1 << 4;

			*/
		//	CH3
			TIM4->CCMR2 |= 1 << 0;
			TIM4->CCMR2 |= 5 << 4;		//�˲�����Ϊ5
			if (dir == 0)
				TIM4->CCER |= 1 << 9; //�½��ز���
			else
				TIM4->CCER &= ~(1 << 9); //�����ز���
			TIM4->CCER |= 1 << 8;
			TIM4->DIER |= 1 << 3;
		/*
		case 2: //CCR2 PA1
			TIMER->CCMR1 |= 1 << 8; //CCR2����ͨ������:����
			TIMER->CCMR1 |= 5 << 12;		//�˲�����Ϊ5
			if (dir == 0)
				TIMER->CCER |= 1 << 5; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 5); //�����ز���
			TIMER->CCER |= 1 << 4; //CCR2ͨ������ʹ��
			TIMER->DIER |= 1 << 2; //CCR2ͨ���������ж�
		case 1: //>CCR1 PA0
			TIMER->CCMR1 |= 1 << 0; //CCR1����ͨ����������
			TIMER->CCMR1 |= 5 << 4;		//�˲�����Ϊ5
			if (dir == 0)
				TIMER->CCER |= 1 << 1; //�½��ز���
			else
				TIMER->CCER &= ~(1 << 1); //�����ز���
			TIMER->CCER |= 1 << 0;     //CCR1����ʹ��
			TIMER->DIER |= 1 << 1;  //CCR1ͨ���������ж�
			break;
	}
	 */
	TIM4->DIER |= 1 << 0;     //��������ж�
//	MY_NVIC_Init(1, 2, TIM2_IRQChannel, 2);     //�ж�
	TIM4->CR1 = 0x01;    //ʹ�ܶ�ʱ��
	TIM4->SR &= ~(1 << 0);

}

#endif
/*

תȦ����
*/
/*
void motor_turn(uint8_t RL,int cHw)
{
	if(RL==TURN_R)		//�ҹ�
	{
		l_motor_stop();  	//�ҵ��ֹͣ
		motor2_set_pwm(GO_FORWARD,TURN_PWM);	//����ת
		//c_hw1=0;
		c_hw=cHw;	  //ת�Ļ�����
		sys->mState = MST_RIGHT;
	}else if(RL == TURN_L)
	{
 		r_motor_stop();  	//�ҵ��ֹͣ
		motor1_set_pwm(GO_FORWARD,TURN_PWM);	//����ת
		//c_hw2=0;
		c_hw=cHw;	  //ת�Ļ�����
		sys->mState = MST_LEFT;
	}
}
*/
/*
	dir:����
	pwm:pwm
	cHw:�˶��Ļ��������ỻ���Ȧ����û�н��Ƶ��˶���Ϊ0
	next:��һ�����������������Ķ���
*/

void motor_turn(uint8_t dir,int pwm,float agle)
{
//	int pwm=600;
	//int dg_turn=deg - 2.5;
//	int c;
	float dis_agle,b_agle;
	log_printf("motor_turn,dir=%s,agle=%f\r\n",get_motor_sta(dir),agle);
	motor.c_left_hw = motor.c_right_hw = 0;
	www_idleintel_com();
	b_agle= format_agle(sys->angle, ANGLE_360);
	motor_run(dir,pwm,0,0);
	while(1)
	{
		delay_ms(5);
		www_idleintel_com();

		dis_agle = b_agle -format_agle(sys->angle, ANGLE_360);
		if(dis_agle > 180)
			dis_agle = 360 - dis_agle;
		if(dis_agle <-180)
			dis_agle += 360;
		if(dis_agle >=agle || dis_agle <=-agle)
			break;

		//log_printf("%3.1f,%3.1f\r\n",format_agle(sys->angle, ANGLE_360),dis_agle);
	}
	motor_run(GO_STOP,0,0,0);
	/*
	TIM5->CNT =0;
	c=0;
	while(1)
	{
		if(TIM5->CNT >=2000)
		{
			if(motor.c_left_hw > deg)
			{
				motor_run(GO_STOP,0,0,0);
				break;
			}
		//log_printf("%d,%d,%d\r\n",c++,motor.c_left_hw,motor.c_right_hw);
		}
	}
	*/
	delay_ms(20);
	log_printf("..OK,c=%d,%d\r\n",motor.c_left_hw,motor.c_right_hw);
	motor.c_left_hw = motor.c_right_hw = 0;
	//Ϊ���ҳ����ֱ��PID

}
void motor_turn_circle(uint8_t dir,int pwm,int hw)
{
//l_motor_turn_circle:
//	float bagle = sys->angle;
	char flage = 1;
	int k=0;
	int count=0,count1=0;
	//uint16_t max_adc=0,less=0;
	float max_agle=0;

//	int hw1=0,hw2=0,lhw1=0,lhw2=0;
	//int pid_pwm;
	log_printf("[motor_turn_circle]hw=%d,dir =%d\r\n",hw,dir);
	/*
	if(dir == GO_LEFT)
	{
		r_motor_set_pwm(GO_FORWARD,600);
		l_motor_set_pwm(GO_FORWARD,700);
	}else
	{
		l_motor_set_pwm(GO_FORWARD,600);
		r_motor_set_pwm(GO_FORWARD,700);
	}
	*/
	MOTOR_POWER_ON();
	if(sys->shut_down_motor)
		MOTOR_CTRL1(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value)
	else
		MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value)

//	motor_run(dir,pwm,0,0);
	motor.c_left_hw = motor.c_right_hw = 0;

	motor_turn_dir(dir ,pwm,pwm);
	turn_round_pid(0);
	navigat->distance = 0;
	TIM5->CNT =0;
	navigat->is_walk = 0;

//	msTmr = 0;
	while(1)
	{
		get_sensers(&sys->gSta);
		if(TIM5->CNT >=1000)
		{
			TIM5->CNT=0;

			count++;
			count1++;
			if(count >=5)
			{
				count = 0;
				www_idleintel_com();
				turn_round_pid(TURN_SPEED);
				if(sys->angle > 180 && flage)
				{
					k ++;

					flage = 0;
					log_printf("k=%d,tmr=%d\r\n",k,msTmr / 1000);
				}
				if(sys->angle < 10)
					flage = 1;
				if(k >= hw /* & dis_float(0 , sys->angle) < 5*/)
				{
					if((dir == GO_LEFT && sys->angle > 355) || (dir == GO_RIGTH&& sys->angle < 5))
					{
						motor_run(GO_STOP,0,0,0);
						break;
					}
				}
			}
			/*

			*/
			if(count1>=2)
			{
				count1 = 0;
				/*
				log_printf("%d,%d,%d,%d,%d,%d,%d,%3.1f\r\n",sys->g_sta[0],sys->g_sta[1],sys->g_sta[2],sys->g_sta[3]
													  ,sys->g_sta[4],sys->g_sta[5],sys->g_sta[6],sys->angle);

				if(dir == GO_LEFT)
					log_printf("%d\r\n",sys->g_sta[6]);
				else
					log_printf("%d\r\n",sys->g_sta[0]);
				if(max_adc < sys->g_sta[6])
				{
					max_adc = sys->g_sta[6];
					max_agle = sys->angle;
				}
				if(sys->g_sta[6] < max_adc && sys->g_sta[6] > 500)
				{
					less++;
					if(less >=6)
					{
						motor_run(GO_STOP,0,0,0);
						break;

					}
				}else
					less = 0;
				*/
			}





		}

	}
	log_printf("count=%d,angle=%3.3f,k=%d,max=%3.3f\r\n",count,sys->angle,k,max_agle);
	//goto l_motor_turn_circle;

}

void motor_turn_360(uint8_t dir)
{
//	int pwm=600;

	int c;
	float cp_deg,cp_deg1;
	//gyro_offset_manage(300);
//	log_printf("motor_turn,begin angle=%f\r\n",sys->angle);
	motor_run(dir,600,0,0);
	if(dir == GO_LEFT)
	{
		cp_deg = 0 - sys->ajust_agle;
		cp_deg1 = 360 - sys->ajust_agle;
	}
	else
		cp_deg = sys->ajust_agle;
	TIM5->CNT =0;
	c=0;

	while(1)
	{
		if(TIM5->CNT >=2000)
		{

			TIM5->CNT=0;
			www_idleintel_com();
			if(sys->angle >=100 && sys->angle < 290)
				c=1;
			if(c)
			{
				if(dir == GO_LEFT )
				{
					if((sys->angle <10 && sys->angle >=cp_deg) || sys->angle >cp_deg1)
					{
						motor_run(GO_STOP,0,0,0);
						log_printf("stop:%f,%f\r\n",sys->angle,cp_deg);
						break;
					}
				}
				if(dir == GO_RIGTH)
				{
					if(sys->angle <=cp_deg)
					{
						motor_run(GO_STOP,0,0,0);
						log_printf("stop:%f,%f\r\n",sys->angle,cp_deg);
						break;
					}
				}
			}
		}
	}

	delay_ms(600);
	www_idleintel_com();
	log_printf("turn ok ,agle=%f,ajust=%f,%f\r\n",sys->angle,sys->ajust_agle,cp_deg);
}

#define TURN_START_SLOW			0

void motor_turn_dir(uint8_t dir ,int left_pwm,int right_pwm)
{
#if TURN_START_SLOW
	int i,start_pwm=MAX_PWM;
#endif
	//uint8_t mSta = sys->mState;

	sys->mState=dir;				//��¼���εķ���
	sys->c_right_drop = sys->c_left_drop = 0;
#if	LOCKED_MOTOR_PARA
	judge_left_right_motor_locked_rotor_process(1);
#endif
	switch(dir)
	{
		case GO_RIGTH:			//���ֹͣ���ұ�ת
			//û��ͣ����
			/*
			if(mSta == GO_FORWARD)
			{
				motor_wheel_stop(RIGHT_WHEEL);
			}else
			*/
			{
				motor_wheel_backward(RIGHT_WHEEL, 900);
				motor_wheel_forward(LEFT_WHEEL, 900);
				delay_ms(50);

				motor_wheel_backward(RIGHT_WHEEL, right_pwm);
				motor_wheel_forward(LEFT_WHEEL, left_pwm);
			}
			sys->left_dir = GO_FORWARD;
			sys->right_pwm = sys->left_pwm = left_pwm;
			break;
		case GO_LEFT:
		/*
			if(mSta == GO_FORWARD)
			{
				motor_wheel_stop(LEFT_WHEEL);
			}else
			*/
			{
				motor_wheel_backward(LEFT_WHEEL, 900);
				motor_wheel_forward(RIGHT_WHEEL, 900);
				delay_ms(50);


				motor_wheel_backward(LEFT_WHEEL, left_pwm);
				motor_wheel_forward(RIGHT_WHEEL, right_pwm);
			}
			sys->left_dir = GO_BACK;
			sys->right_pwm = sys->left_pwm = left_pwm;
			break;

	}
	//sys->mState = dir;
}

void motor_run(uint8_t dir ,int pwm,int cHw,uint8_t next)
{
	//ǰ�����ˣ��������Ӷ�ת
	log_printf("[motor run]%s,pwm=%d\r\n",&motorStr[dir][0],pwm);
	//test
	//if(dir !=GO_STOP)
	//motor.c_right_hw = motor.c_left_hw =0;
//	sys->trg_speed=sys->trg_speed2=0;
	//int start_pwm;
	sys->nearwall_signal_lost = RESET;		//�ر߶�ʧ�ı�ǣ�ÿ���˶���ʱ������
	if(dir != GO_STOP)
	{
		navigat->whele_stop = 0;
		//micony201710
		if(sys->sState == SYS_DOCK && sys->motor_power_off)
		{
		    log_printf("MOTOR_POWER_OFF 3\r\n");
			log_printf("found motoroff,power on...\r\n");
			MOTOR_POWER_ON();
			MOTOR_CTRL_SIDE(DOCK_SIDE_PWM_L,DOCK_SIDE_PWM_R);
		}
#if	LOCKED_MOTOR_PARA
		judge_left_right_motor_locked_rotor_process(1);
#endif
	}
	sys->c_right_drop = sys->c_left_drop = 0;
	sys->c_front_hw = 0;
	for(uint8_t i;i<MAX_FRONT_HW;i++)
		sys->front_hw[i] = 1;
	sys->max_front_stop = 15;

	switch(dir)
	{
		case GO_STOP:
		/*
			{
				int i;
				if( *left_pwm > 300 && *left_pwm < 800 && sys->mState != GO_STOP)
				{
					for(i=0;i<10;i++)
					{
						if(*left_pwm < 900)
							*left_pwm +=100;
						if(*right_pwm < 900)
							*right_pwm +=100;
						log_printf("stop:%d,%d\r\n",*left_pwm,*right_pwm);
						if(*left_pwm >=890 ||  *right_pwm >=890)
						{
							 motor_wheel_stop(DOUBLE_WHEEL);
							 break;
						}
						delay(20);
					}
				}

			}
			*/
			 motor_wheel_stop(DOUBLE_WHEEL);
			 sys->c_left=sys->c_right=0;
			 delay(20);
			 break;
		case GO_FORWARD:


			motor_wheel_forward(DOUBLE_WHEEL, pwm);
			break;
		case GO_BACK:


			motor_wheel_backward(DOUBLE_WHEEL, pwm);
			break;
		case GO_RIGTH:			//���ֹͣ���ұ�ת
		/*
			start_pwm = MAX_PWM;
			while(1)
			{
				start_pwm -= 100;
				if((start_pwm ) <= pwm)
					break;

				motor_wheel_backward(RIGHT_WHEEL, start_pwm);
				motor_wheel_forward(LEFT_WHEEL, start_pwm);
				delay_ms(3);
			}
			*/
			motor_wheel_backward(RIGHT_WHEEL, pwm);
			motor_wheel_forward(LEFT_WHEEL, pwm);
			sys->left_dir = GO_FORWARD;
			sys->right_pwm = sys->left_pwm = pwm;
			break;
		case GO_LEFT:
		/*
			start_pwm = MAX_PWM;
			while(1)
			{
				start_pwm -= 50;
				if((start_pwm ) <= pwm)
					break;

				motor_wheel_backward(LEFT_WHEEL, start_pwm);
				motor_wheel_forward(RIGHT_WHEEL, start_pwm);
				delay_ms(2);
			}
			*/
			motor_wheel_backward(LEFT_WHEEL, pwm);
			motor_wheel_forward(RIGHT_WHEEL, pwm);
			sys->left_dir = GO_BACK;
			sys->right_pwm = sys->left_pwm = pwm;
			break;
		case GO_ROUND:		 // תȦ
			r_motor_set_pwm(GO_FORWARD,560);
			l_motor_set_pwm(GO_FORWARD,800);
			//sys->trg_speed=1100;
		//	sys->trg_speed2=1800;		//����ٶ�Ҫ���Թ�

			break;
		case GO_F_SLOW:		  //����ǰ��
			r_motor_set_pwm(GO_FORWARD,pwm);
			l_motor_set_pwm(GO_FORWARD,pwm);
			break;
		case GO_SL_ROUND:
			r_motor_set_pwm(GO_FORWARD,420);
			l_motor_set_pwm(GO_FORWARD,550);
			//sys->trg_speed=2200;
			//sys->trg_speed2=1700;		//����ٶ�Ҫ���Թ�
			//log_printf("set l speed=%d, r_speed=%d\r\n",sys->trg_speed,sys->trg_speed2);
			sys->c_left=sys->c_right=0;
			break;
		case GO_LEFT_RD:
			r_motor_set_pwm(GO_FORWARD,560);
			l_motor_set_pwm(GO_FORWARD,800);
			break;
		case GO_RIGHT_RD:
			log_printf("GO_RIGHT_RD\r\n");
			//l_motor_set_pwm(GO_FORWARD,560);
			//r_motor_set_pwm(GO_FORWARD,800);
			motor_wheel_forward(RIGHT_WHEEL, 800);
			motor_wheel_forward(LEFT_WHEEL, 550);
			break;
		default :
			r_motor_set_pwm(GO_FORWARD,NOMORL_PWM);
			l_motor_set_pwm(GO_FORWARD,NOMORL_PWM);
			break;



	}
	//sys->pwm = pwm;
	//sys->c_hw=cHw;					//��Ҫת�ĵĻ�����
	sys->mState=dir;				//��¼���εķ���
	sys->m_dist = motor.c_left_hw;
//	sys->nextState=next;			//��������󣬵��ƶ�����
	//sys->c_timeout_2=0;
	//sys->m_cHw1=sys->m_cHw2=0;			//��������Ļ���������

//	sys->chw_next=50;

	sys->m_speed1=0;

	//printf_motor_sta();
 // log_printf("cHW1=%d,cHW2=%d,cHw=%d,,m_state=%d,motor=%s,netx=%s\r\n",sys->m_cHw1,sys->m_cHw2,sys->c_hw,sys->mState,get_motor_sta(sys->mState),get_motor_sta(sys->nextState));
}


/*
ֱ��PID���̣�ע�⣬�����ڷ��������߳��н��С�
*/

char proc_line_pid(float set_angle)
{
//#if GYRO_VERSION
	static uint8_t count=0;		//2018-07-16 jzz

	//www_idleintel_com();
	//�����и�bug������set_angle == navigat->out  û���ýǶȣ���ȡ��ǰ�Ƕ�
	count ++;
	if( count>=2)//2018-07-16 jzz
	{
		count = 0;
//#endif
		if(set_angle == NO_ANGLE_V)
		{
			set_angle = navigat->out = sys->angle;
			log_printf("set = %3.1f\r\n",sys->angle);
		}
		set_angle = format_agle(set_angle, ANGLE_360);
		/*
		if(sys->angle > 330)
			sys->angle -= 360.0f;
		if(set_angle > 330)
			set_angle -=360;
		*/
		p_gyro_calce(set_angle,sys->angle);

		L_FORWRK_PWM =  sys->lpwm;
		R_FORWRK_PWM =  sys->rpwm;

	//	log_printf("%d,%d\r\n",L_FORWRK_PWM,R_FORWRK_PWM);
//#if GYRO_VERSION
	}
//#endif
	return 1;
}
char proc_back_pid(float set_angle)
{


	//www_idleintel_com();
	set_angle = format_agle(set_angle, ANGLE_360);
	/*
	if(sys->angle > 330)
		sys->angle -= 360.0f;
	if(set_angle > 330)
		set_angle -=360;
	*/
	p_gyro_calce(set_angle,sys->angle);

	R_FORWRK_PWM =  sys->lpwm;
	L_FORWRK_PWM =  sys->rpwm;

	return 1;
}

char * get_motor_sta(uint8_t indx)
{
	return (&motorStr[indx][0]);
}
void printf_motor_sta(void)
{
//#if 1//MOTOR_DEBUG
	log_printf("H1=%d,H2=%d,S1=%d,S2=%d,w=%d,%s\r\n",sys->m_cHw1,sys->m_cHw2,sys->m_speed1,sys->m_speed2,sys->c_hw,&motorStr[sys->mState][0]);
//#endif
}
/*
#define FW_SLOW_PWM			780//sys->pwm//620
#define BACK_OFF_PWM		780//650//(sys->pwm+80)//cfg->go_back_pwm
#define GO_FORWARD_PWM		780//sys->pwm//cfg->go_forward_pwm
#define GO_ROUTINT_PWM		780//sys->pwm//cfg->go_route_pwm
#define TURN_DEG_PWM		720//sys->pwm
#define GO_NEAR_PWM			700
#define GO_NEAR_PWM_FAST	630




uint16_t get_pwm(uint8_t type)
{
	log_printf("t=%d,v=%d\r\n",type,sys->volt);
	if(type == T_GO_FW_PWM )
	{
	//	if(sys->voice < 1400)
		//	return 680;
		if(sys->volt < 1460)
			return 700;
		return 780;

	}
	if (type == T_GO_NEAR_PWM)
	{
		if(sys->volt < 1460)
			return 640;
		return 700;

	}
	if (type == T_GO_TURN_DEG)
	{
		if(sys->volt < 1460)
			return 660;
		return 720;

	}

}
*/
#if LOCKED_MOTOR_PARA
#define CYC_MO_LO_TIME 	5000		//5S���һ���Ƿ��ת
	//#define CYC_MO_INIT_TIME	20		//20ms���һ���Ƿ��ʼ����ת
#define MO_LO_MIN_HW	10			//�жϵ����������С������
void judge_left_right_motor_locked_rotor_process(uint8_t type)
{
	static uint16_t motor_lock_times_count_s = 0;
	static int init_lock_left_hw = 0;
	static int init_lock_right_hw = 0;
	int lock_left_hw;
	int lock_right_hw;

	if(type == 1)
	{
		motor_lock_times_count_s = 0;
		init_lock_left_hw = motor.c_left_hw;
		init_lock_right_hw = motor.c_right_hw;
		return;
	}


	//20ms�������ת�����Ƿ���Ҫ��ʼ
	/*
	if(sys->motor_lock_times_count_ms++ > CYC_MO_INIT_TIME)
	{
		sys->motor_lock_times_count_ms = 0;
		if(sys->mState == GO_STOP )
		{
			init_left_right_motor_locked_parameter();	////��ʼ����ʼHWֵ
		}
	}
	*/
	//�ڵ������״̬��,û���жϳ��������Ž����Ƿ���ס���
	if(sys->sState != SYS_CHARGE)
	{
		if(sys->mState != GO_STOP && sys->motor_locked_flag == 0)	//
		{
			if( motor_lock_times_count_s++ > CYC_MO_LO_TIME )
			{
				motor_lock_times_count_s = 0;
				lock_left_hw = motor.c_left_hw;
				lock_right_hw = motor.c_right_hw;

				if((disXY(init_lock_left_hw,lock_left_hw) <= MO_LO_MIN_HW) || (disXY(init_lock_right_hw,lock_right_hw) <= MO_LO_MIN_HW))
				{
					log_printf("==motor_locked:L[%d,%d]R[%d,%d]\r\n",lock_left_hw,init_lock_left_hw,lock_right_hw,init_lock_right_hw);
					sys->motor_locked_flag = 1;
				}
				else		//���û�ж�ת
				{
					init_lock_left_hw = motor.c_left_hw;
					init_lock_right_hw = motor.c_right_hw;
				}
			}
		}
		else
			motor_lock_times_count_s = 0;
	}
}
#endif
static uint8_t s_sta = 0;
static uint8_t s_work_mode = 0;
static uint8_t s_dust_sensor = 0;
static FlagStatus s_blankets_ground = RESET;
//����: ϵͳ״̬,����ģʽ,�ҳ����,��̺���
//���: ��������ٶ� ��ˢ�ٶ� �����ٶ�
//sta:ϵͳ״̬ ���� ���� dock change
//f_work_mode:6������ģʽ:����,����,ǿ��,full2go,�ֶ�,������ɨ
/* if(sys->f_work_mode==MWO_TURBO||sys->f_work_mode==MWO_SPOT)
	{
      log_printf("MWO_TURBO = %d\r\n",AGITATOR_PWM);
		AGITATOR_PWM = 999;
        return ;
	}*/
void set_mode_and_power_pro(uint8_t sta,uint8_t f_work_mode,uint8_t dust_sensor,FlagStatus Blankets_ground)
{
	if(sys->sState == SYS_DOCK || sys->sState == SYS_DOCK_NEAR)
		return ;

	if(s_sta != sta || s_work_mode != f_work_mode || s_dust_sensor != dust_sensor || s_blankets_ground != Blankets_ground )
	{
		log_printf1("[set_mode_and_power_pro]sta:(%d)%d,work_mode:(%d)%d\r\n",s_sta,sta,s_work_mode,f_work_mode);
		log_printf1("[set_mode_and_power_pro]dust_sensor(%d)%d,Blankets_ground(%d)%d\r\n",s_dust_sensor,dust_sensor,s_blankets_ground,Blankets_ground);
		s_sta = sta;
		s_work_mode = f_work_mode;
		s_dust_sensor = dust_sensor;
		s_blankets_ground = Blankets_ground;
		switch(f_work_mode)
		{
			case MWO_SILENT:
				log_printf1("MWO_SILENT\r\n");
				sys->dust_pwm_lev = DUST_MODE_SILENT;
				sys->agi_speed = AGI_L_SPEED;			//
				//SET_MID_MOTER(cfg->l_agi_speed);
				//log_printf1("SET_MID_MOTER=%d\r\n",cfg->l_agi_speed);
				sys->run_speed = LOW_SPEED;				//
			break;
			case MWO_NORMAL:
			case MWO_FULL2GO:
				log_printf1("MWO_NORMAL\r\n");
				if(dust_sensor == DUST_LOW )
				{
					log_printf1("dust_sensor1=%d\r\n",DUST_LOW);

					sys->dust_pwm_lev = DUST_MODE_NORMAL;
					sys->agi_speed = AGI_ML_SPEED;		//
					if(Blankets_ground)
						sys->run_speed = MIDLE_M_SPEED;			//
					else
						sys->run_speed = MIDLE_H_SPEED;			//
				}
				else if(dust_sensor == DUST_MEDIUM)
				{
				   log_printf1("dust_sensor2=%d\r\n",DUST_MEDIUM);
					sys->dust_pwm_lev = DUST_MODE_NORMAL;
					if(Blankets_ground)
					{
						sys->agi_speed = AGI_M_SPEED;		//
						sys->run_speed = MIDLE_L_SPEED;			//
					}
					else
					{
						sys->agi_speed = AGI_ML_SPEED;		//
						sys->run_speed = MIDLE_SPEED; 		//
					}
				}
				else
				{
			        log_printf1("dust_sensor3=%d\r\n",DUST_MEDIUM);
					sys->dust_pwm_lev = DUST_MODE_TURBO;
					if(Blankets_ground)
						sys->agi_speed = AGI_MH_SPEED;		//
					else
						sys->agi_speed = AGI_M_SPEED;		//
					sys->run_speed = LOW_SPEED;			//
				}
			break;
			case MWO_TURBO:
			case MWO_MANUAL:
				log_printf1("MWO_TURBO\r\n");
				sys->dust_pwm_lev = DUST_MODE_TURBO;
				//sys->agi_speed = AGI_H_SPEED;		//
				sys->run_speed = HIGHT_SPEED; 		//
			break;
			case MWO_SPOT:
				log_printf1("MWO_TURBO\r\n");
				sys->dust_pwm_lev = DUST_MODE_TURBO;
				sys->agi_speed = AGI_H_SPEED;		//  ����������
				//sys->run_speed = HIGHT_SPEED;			//�������ٶ�
			break;
			default:

			break;
		}


		if((sta != SYS_IDLE && sta != SYS_CHARGE) || f_work_mode == MWO_MANUAL)
		{
			log_printf1("dust_motor_rank change right now,lev=%d\r\n",sys->dust_pwm_lev );
			if(sys->dust_pwm_lev == DUST_MODE_SILENT)
			{
				DUST_MOTOR_RANK1();
				AGITATOR_PWM = 100;
				log_printf1("MWO_TURBO1=%d\r\n",AGITATOR_PWM);
			}
			else if(sys->dust_pwm_lev == DUST_MODE_NORMAL)
			{
				DUST_MOTOR_RANK2();
				AGITATOR_PWM = 450;
				log_printf1("MWO_TURBO2=%d\r\n",AGITATOR_PWM);
			}
			else
			{
				DUST_MOTOR_RANK3();
				AGITATOR_PWM = 850;
				log_printf1("MWO_TURBO3=%d\r\n",AGITATOR_PWM);
			}
		}
		else
		{
			log_printf("dust_motor_rank change idle,mode=%d\r\n",f_work_mode);
			if(f_work_mode == MWO_MANUAL)
			{
				sys->dust_pwm_lev = DUST_MODE_TURBO;
				// sys->dust_pwm_lev =  DUST_MODE_NORMAL;
				sys->agi_speed = AGI_H_SPEED;		//
				sys->run_speed = HIGHT_SPEED;		//
				log_printf1("MWO_TURBO4\r\n");
			}
			else
			{
				//DUST_MOTOR_OFF();
				//sys->agi_speed = AGI_H_SPEED;		//���Ϊʲô����Ϊ����������,�����ε�
			}
		}
		/*
			if(sta == SYS_NAVIGAT&&f_work_mode == MWO_MANUAL)   //��2a�����޷��ı����ȼ�ǿ�������ڵ���״̬�·���ȼ������е�
		{
		        sys->dust_pwm_lev = DUST_MODE_NORMAL;
				log_printf1("MWO_TURBO5\r\n");
		}
		*/
		log_printf("[set_mode_and_power_pro]dust_pwm_lev:%d,agi_speed:%d,run_speed:%d\r\n",sys->dust_pwm_lev,sys->agi_speed,sys->run_speed);
#if 1	//2a		//���Խ��������õ�
		if(sys->dust_pwm_lev == DUST_MODE_SILENT)
			log_printf1("Vacuum motor power:L_15\r\n");
		else if(sys->dust_pwm_lev == DUST_MODE_NORMAL)
			log_printf1("Vacuum motor power:M_42.5\r\n");
		else if(sys->dust_pwm_lev == DUST_MODE_TURBO)
			log_printf1("Vacuum motor power:H_70\r\n");
		if(sys->agi_speed == AGI_H_SPEED)
			log_printf("Agitator motor power:H_20\r\n");
		else if(sys->agi_speed == AGI_MH_SPEED)
			log_printf("Agitator motor power:M_10\r\n");
		else if(sys->agi_speed == AGI_M_SPEED)
			log_printf("Agitator motor power:M_10\r\n");
		else if(sys->agi_speed == AGI_ML_SPEED)
			log_printf("Agitator motor power:M_10\r\n");
		else if(sys->agi_speed == AGI_L_SPEED)
			log_printf("Agitator motor power:L_5\r\n");
		if(f_work_mode==MWO_SPOT)
			return;
		if(sys->run_speed == HIGHT_SPEED)
			log_printf("Speed:H_0.35\r\n");
		else if(sys->run_speed == MIDLE_H_SPEED)
			log_printf("Speed:M_0.27\r\n");
		else if(sys->run_speed == MIDLE_SPEED)
			log_printf("Speed:M_0.27\r\n");
		else if(sys->run_speed == MIDLE_M_SPEED)
			log_printf("Speed:M_0.27\r\n");
		else if(sys->run_speed == MIDLE_L_SPEED)
			log_printf("Speed:M_0.27\r\n");
		else if(sys->run_speed == LOW_SPEED)
			log_printf("Speed:L_0.2\r\n");
#else	//2b
		if(sys->dust_pwm_lev == DUST_MODE_SILENT)
			log_printf("Vacuum motor power:L_20\r\n");
		else if(sys->dust_pwm_lev == DUST_MODE_NORMAL)
			log_printf("Vacuum motor power:M_40\r\n");
		else if(sys->dust_pwm_lev == DUST_MODE_TURBO)
			log_printf("Vacuum motor power:H_120\r\n");
		if(sys->agi_speed == AGI_H_SPEED)
			log_printf("Agitator motor power:H_40\r\n");
		else if(sys->agi_speed == AGI_MH_SPEED)
			log_printf("Agitator motor power:MH_30\r\n");
		else if(sys->agi_speed == AGI_M_SPEED)
			log_printf("Agitator motor power:M_20\r\n");
		else if(sys->agi_speed == AGI_ML_SPEED)
			log_printf("Agitator motor power:ML_15\r\n");
		else if(sys->agi_speed == AGI_L_SPEED)
			log_printf("Agitator motor power:L_5\r\n");
		if(f_work_mode==MWO_SPOT)
			return;
		if(sys->run_speed == HIGHT_SPEED)
			log_printf("Speed:H_0.35\r\n");
		else if(sys->run_speed == MIDLE_H_SPEED)
			log_printf("Speed:MH_0.30\r\n");
		else if(sys->run_speed == MIDLE_SPEED)
			log_printf("Speed:M_0.27\r\n");
		else if(sys->run_speed == MIDLE_M_SPEED)
			log_printf("Speed:M_0.25\r\n");
		else if(sys->run_speed == MIDLE_L_SPEED)
			log_printf("Speed:M_0.22\r\n");
		else if(sys->run_speed == LOW_SPEED)
			log_printf("Speed:M_0.2\r\n");

#endif
	}
}
void dis_mode_pro(void)
{
	uint8_t up_work_mod;
	///1����ϵͳ״̬,����ģʽ,����,��̺��Ϣ,�õ�ϵͳ״̬���������ת����
	if(sys->sState == SYS_TEST_BD)
		return;
	set_mode_and_power_pro(sys->sState,sys->f_work_mode,sys->dust_sensor,sys->Blankets_ground);
	if(sys->gey_key_clear_err == 1)/// E2һֱ��ʾ���а������������
	{
		log_printf("gey_key_clear_err CLEAR\r\n");
		sys->gey_key_clear_err = 0;
		sys->work_errcode = MWERR_NONE;
		//dp_led_err_contrl(0,sys->sState,sys->work_errcode);
	}
	///2�ϱ�ģʽ ָʾ��
	if(sys->f_work_mode == MWO_SPOT)
		 up_work_mod = MWO_TURBO;
	else
		up_work_mod = sys->f_work_mode;
	dp_led_light_contrl(1,sys->sState,up_work_mod,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
	//if(sys->power_on)
	dp_led_err_contrl(1,sys->sState,sys->work_errcode);
}
