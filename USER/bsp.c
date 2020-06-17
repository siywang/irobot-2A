/**
  *************************************************************************
  *@FileName
  *@Author
  *@Version
  *@Date
  *@History
  *@Dependence
  *@Description
  *************************************************************************
  *@CopyRight
  *************************************************************************
  *
	*/

//#include "includes.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"

#include "stm32f10x_pwr.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"


#include "sys.h"
//#include "BLDC.h"

extern uint32_t bldcSpeed;

#if ROOMA_GYROONLY

void RTC_Init(void);
#endif

void  RCC_Config  (void);
void  GPIO_Config (void);
void  NVIC_Config(void);
void  EXTI_Config (void);
void EXTI_cfg(void);
/**
  *@Function
  *@Description
  *@Calls
  *@Call By
  *@Param	xxx
  *		@arg
  *		@arg
  *@Param	xxx
  *		@arg
  *		@arg
  *@Reture
  *@Others
  **/
void Periph_Init(void)
{
	//GPIO_InitTypeDef GPIO_InitStructure;
  /*********ʱ��ʹ��*************/
  RCC_Config();
	/*

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

	 GPIO_SetBits(GPIOE,GPIO_Pin_9);
	while(1);
*/

  /********�ж����ȼ�����********/
  NVIC_Config();

  EXTI_cfg();  //�ⲿ�жϣ����Թ�������!


  /********�ⲿ�ж�����**********/
  /********�ⲿGPIO����**********/


   GPIO_Config();
   senser_gpio_config();
#if ROOMA_GYROONLY
	//#if DP_PANEL_EN == 0 || DP_PANEL_EN == 3
   key_io_init();
   //#endif
	////RTC_Init();
#endif
	I2C1_GPIO_Init();
	//i2c_io_init();

  // USART2_Config();
 //  ADC_Config();
//   MCPWM1_Config();
}
void EXTI_cfg()
{
       EXTI_InitTypeDef EXTI_InitStructure;
       //����жϱ�־
 	 /*
		PD1	��������̳���A��
		PE4	�Һ��������A��
		PD13 ǰ������B��
		PE11 ��ˢ����
 	 */
       EXTI_ClearITPendingBit(EXTI_Line1);////EXTI_ClearITPendingBit(EXTI_Line2);
       EXTI_ClearITPendingBit(EXTI_Line4);
       //EXTI_ClearITPendingBit(EXTI_Line13);
	EXTI_ClearITPendingBit(EXTI_Line11);	//wy190615 PE11��ˢ����

       GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource1);////GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
       //GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource11);//wy190615 PE11��ˢ����

  	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;

       EXTI_InitStructure.EXTI_Line = EXTI_Line1; //ѡ���ж���·2 3 5
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//s_Falling; //�����жϴ�����ʽΪ���½��ش���
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //�ⲿ�ж�ʹ��
       EXTI_Init(&EXTI_InitStructure);

       EXTI_InitStructure.EXTI_Line = EXTI_Line4; //ѡ���ж���·2 3 5
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//s_Falling; //�����жϴ�����ʽΪ���½��ش���
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //�ⲿ�ж�ʹ��
       EXTI_Init(&EXTI_InitStructure);
/*
        EXTI_InitStructure.EXTI_Line = EXTI_Line13; //ѡ���ж���·2 3 5
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//s_Falling; //�����жϴ�����ʽΪ���½��ش���
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //�ⲿ�ж�ʹ��
       EXTI_Init(&EXTI_InitStructure);
*/
	//wy190615 PE11��ˢ����
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


#if 0
/*
	�س�������
	PC7		�м�IR���س����������źţ�		�ж�����/��������	INT7/TIM3_CH2
	PB12		���IR���س����������źŵģ�	�ж�����/��������	INT12	�ܷ�ĳ�PC6?����������TIM3�Ĳ�������CH1
	PC8		�ұ�IR���س����������ź�)		�ж�����/��������	INT8/TIM3_CH3
	PC9		360��IR����					�ж�����/��������	INT9/TIM3_CH4
*/
	   EXTI_ClearITPendingBit(EXTI_Line8);
       EXTI_ClearITPendingBit(EXTI_Line7);
       EXTI_ClearITPendingBit(EXTI_Line9);
       EXTI_ClearITPendingBit(EXTI_Line12);

       GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource7);
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource9);
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource12);

	   EXTI_InitStructure.EXTI_Line = EXTI_Line7 | EXTI_Line8 | EXTI_Line9| EXTI_Line12; //ѡ���ж���·2 3 5
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//s_Falling; //�����жϴ�����ʽΪ���½��ش���
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //�ⲿ�ж�ʹ��
       EXTI_Init(&EXTI_InitStructure);
#endif

}

/**
  *@Function
  *@Description
  *@Calls
  *@Call By
  *@Param	xxx
  *		@arg
  *		@arg
  *@Param	xxx
  *		@arg
  *		@arg
  *@Reture
  *@Others
  **/
void RCC_Config(void)
{
	SystemInit();

	/* ʹ��GPIO(A~C)|ADC1|USART1 ʱ�� */
	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA
							| RCC_APB2Periph_GPIOB
			 	 	 	 	| RCC_APB2Periph_GPIOC
			 	 	 	 	| RCC_APB2Periph_GPIOD
			 	 	 	 	| RCC_APB2Periph_GPIOE
							| RCC_APB2Periph_AFIO
							| RCC_APB2Periph_TIM1
							| RCC_APB2Periph_ADC1
							| RCC_APB2Periph_TIM8
							| RCC_APB2Periph_USART1
 							, ENABLE );
//	 RCC_APB1PeriphClockCmd(  RCC_APB1Periph_USART2
//	 						, ENABLE );
/******************CANʱ��ʹ��**************************/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
 	RCC_APB1PeriphClockCmd(   RCC_APB1Periph_TIM3
							| RCC_APB1Periph_USART2
							| RCC_APB1Periph_TIM4
							| RCC_APB1Periph_TIM5
							| RCC_APB1Periph_TIM2
							| RCC_APB1Periph_UART4
							| RCC_APB1Periph_PWR
							| RCC_APB1Periph_BKP
							, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);



 TIM_ARRPreloadConfig(TIM3, ENABLE);
#if DUST_BOX_FULL_CHECK

	 /* Enable PWR(��Դ���ƣ� and BKP clock */
		 //RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		 PWR_DeInit();
	 /* Enable write access to Backup domain */
		 PWR_BackupAccessCmd(ENABLE);
	 /* Clear Tamper pin Event(TE) pending flag */
		 BKP_ClearFlag();
#endif




}





/***************************************************
* Descirbe  :
* Input     :
* Output    :
* Attention :
* author 	:
***************************************************/
void  GPIO_Config(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

	//4·�ⲿ�ж�
	/*
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14|GPIO_Pin_15; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;// ѡ������ģʽΪ�������� GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //���Ƶ�����50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	*/
	/*
		PD14	��������̳���A��
		PD13	�Һ��������A��
 	 */
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  |GPIO_Pin_14;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOD,&GPIO_InitStructure);

       //PD1 PD2	��������̳���A��	�ж�����	INT1
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;   //  GPIO_Pin_2 | GPIO_Pin_1;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOD,&GPIO_InitStructure);

       //10	PE4 PE5 �������A��	�ж�����	INT4
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOE,&GPIO_InitStructure);

		//wy190615 ��ˢ����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE,&GPIO_InitStructure);

		//PB15 PE7 PC8 �����һس�IR���
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		//PE6 PE12 ��ײ������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE,&GPIO_InitStructure);


		//PD7 ����ǽ
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOD,&GPIO_InitStructure);

		//PE9 ǰ�յ�
		/*GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		*/
		//PC9 ǰײ�����
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		//PD11 �������λ���
		GPIO_InitStructure.GPIO_Pin = PIN_CHARGE;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_CHARGE,&GPIO_InitStructure);

		//PD0 AC���
		GPIO_InitStructure.GPIO_Pin = PIN_AC;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_AC,&GPIO_InitStructure);

		//PE11 ��Դ����
		/*GPIO_InitStructure.GPIO_Pin =   PIN_VOID_POWER;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_VOID_POWER, &GPIO_InitStructure);*/
/*#if DP_PANEL_EN < 4
		GPIO_InitStructure.GPIO_Pin = PIN_SIDE_MOTOR_L;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_SIDE_MOTOR_L, &GPIO_InitStructure);


		GPIO_InitStructure.GPIO_Pin = PIN_SIDE_MOTOR_R;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_SIDE_MOTOR_R, &GPIO_InitStructure);
#endif
*/
/*
		GPIO_InitStructure.GPIO_Pin = PIN_DUST_MOTOR;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_DUST_MOTOR, &GPIO_InitStructure);
*/
		//����ĵ�������
		GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_POWER;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_MOTOR_POWER, &GPIO_InitStructure);

		//�������
		GPIO_InitStructure.GPIO_Pin = PIN_DUST_MOTOR_PWM;
		GPIO_Init(PORT_DUST_MOTOR_PWM, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = PIN_DUST_MOTOR_SPD;
		GPIO_Init(PORT_DUST_MOTOR_SPD, &GPIO_InitStructure);

//IC���ڳ��������ok
	GPIO_InitStructure.GPIO_Pin = CHARGE_ING_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CHARGE_ING_PORT,&GPIO_InitStructure);

//IC�����ɼ������ok
	GPIO_InitStructure.GPIO_Pin = CHARGE_DONE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CHARGE_DONE_PORT,&GPIO_InitStructure);

}
/***************************************************
* Descirbe  :
* Input     :
* Output    :
* Attention :
* author 	:
***************************************************/
void NVIC_Config(void)
{
	   NVIC_InitTypeDef NVIC_InitStructure; 					 //��λNVIC�Ĵ���ΪĬ��ֵ


       NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //�����ж����ȼ���

/*
	   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		//��TIM2�ж�
       NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
       NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
       NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
       NVIC_Init(&NVIC_InitStructure);

	   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;		//��TIM2�ж�
       NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
       NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
       NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
       NVIC_Init(&NVIC_InitStructure);

*/
		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);


		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

/**/


		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);



		//	�ⲿ�ж�

		//����
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                          //ѡ���жϷ���2


		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;     //ѡ���ж�ͨ��2
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ�ж����ȼ�����Ϊ0
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧʽ�ж����ȼ�����Ϊ0
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //ʹ���ж�
        NVIC_Init(&NVIC_InitStructure);


        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;            //ѡ���ж�ͨ��3
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռʽ�ж����ȼ�����Ϊ1
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��Ӧʽ�ж����ȼ�����Ϊ1
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //ʹ���ж�
        NVIC_Init(&NVIC_InitStructure);
 		 /*
		//�������
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
       */
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);


		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}
/***************************************************
* Descirbe  :
* Input     :
* Output    :
* Attention :
* author 	:
***************************************************/

void Delayus(int t)
{
   while(t--)
   {
   }
}
/*
void timer2_cfg(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //�����Զ�װ�ؼĴ���//������65000us =65ms
	//��Ƶ���� 72��Ƶ����ʱ��Ϊ72M����Ƶ�󣬾���1M,��һ��ʱ�ӣ�����1us��Ҳ���Ǽ�������1������һus
	TIM_TimeBaseStructure.TIM_Prescaler = (720000-1);

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //ѡ�����ϼ���
	TIM_TimeBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE); //���ܶ�ʱ��
  //
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

 TIM_DeInit(TIM2);

 TIM_TimeBaseStructure.TIM_Period=0xFFFF;		 //ARR��ֵ
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //������Ƶ
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM2,71999,TIM_PSCReloadMode_Immediate);//ʱ�ӷ�Ƶϵ��72000�����Զ�ʱ��ʱ��Ϊ1K
 TIM_ARRPreloadConfig(TIM2, DISABLE);//��ֹARRԤװ�ػ�����
 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

 TIM_Cmd(TIM2, ENABLE);	//����ʱ��	  //
}

 */

void TargetSysReset(void)
{
 __set_FAULTMASK(1);
 SCB->AIRCR = 0x05FA0000 | (u32)0x04;
 while(1);
}

#if ROOMA_GYROONLY
void soft_delay_ms(uint32_t ms_num)
{
	uint32_t jisuan = 0;

	for(uint32_t ii = 0; ii < ms_num; ii++)
	{
		jisuan = 72000;
		while(jisuan--);
	}
}

/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : ����������RTC��BKP�����ڼ�⵽�󱸼Ĵ������ݶ�ʧʱʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_RCCCfg(void)
{
//	uint32_t rtc_count = 0;
	//����PWR��BKP��ʱ�ӣ�from APB1��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	//�������
	PWR_BackupAccessCmd(ENABLE);

	//���ݼĴ���ģ�鸴λ
	BKP_DeInit();
#if 1
	//�ⲿ32.768K��Ӵż�Ǹ�
	RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ��ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//i ++;
	//log_printf("[RTC_RCCCfg] 4 %ld\r\n",i);

	//RTCʱ��Դ���ó�LSE���ⲿ32.768K��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//����ʹ��LSI��RTCʱ��Դ����˹����ò���RTC,ֻ�õ���RAM

	//RTC����
	RCC_RTCCLKCmd(ENABLE);
	//��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ�������ܶ�д�Ĵ���
	RTC_WaitForSynchro();
	//��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
	RTC_WaitForLastTask();
	//����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);
#else
	//�ⲿ32.768K��Ӵż�Ǹ�
	//RCC_LSEConfig(RCC_LSE_ON);
	RCC_LSICmd(ENABLE);
	//�ȴ��ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);//i ++;
	//log_printf("[RTC_RCCCfg] 4 %ld\r\n",i);

	//RTCʱ��Դ���ó�LSE���ⲿ32.768K��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//����ʹ��LSI��RTCʱ��Դ����˹����ò���RTC,ֻ�õ���RAM

	//RTC����
	RCC_RTCCLKCmd(ENABLE);

	//��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ�������ܶ�д�Ĵ���
	RTC_WaitForSynchro();

	//��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
	RTC_WaitForLastTask();

	//����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(39999);
#endif

	//�ȴ��Ĵ���д�����
	RTC_WaitForLastTask();

	RCC_RTCCLKCmd(ENABLE);
	//�ȴ�RTCʱ����APB1ʱ��ͬ��
	RTC_WaitForSynchro();

	//ʹ�����ж�
	//RTC_ITConfig(RTC_IT_SEC, ENABLE);
	//ʹ�������ж�
	//RTC_ITConfig(RTC_IT_ALR, ENABLE);// | RTC_IT_SEC  

	//�ȴ�д�����
	RTC_WaitForLastTask();


	return;
}
/*******************************************************************************
* Function Name  : RTC_Config
* Description    : �ϵ�ʱ���ñ��������Զ�����Ƿ���ҪRTC��ʼ����
* 					����Ҫ���³�ʼ��RTC�������RTC_Configuration()�����Ӧ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Config(void)
{
	//������BKP�ĺ󱸼Ĵ���1�У�����һ�������ַ�0xA5A5
	//��һ���ϵ��󱸵�Դ����󣬸üĴ������ݶ�ʧ��
	//����RTC���ݶ�ʧ����Ҫ��������
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		log_printf("rtc power lost\r\n");
		//��������RTC
		RTC_RCCCfg();
		//������ɺ���󱸼Ĵ�����д�����ַ�0xA5A5
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		//���󱸼Ĵ���û�е��磬��������������RTC
		//�������ǿ�������RCC_GetFlagStatus()�����鿴���θ�λ����
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			//�����ϵ縴λ
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			//�����ⲿRST�ܽŸ�λ
		}
		//���RCC�и�λ��־
		RCC_ClearFlag();

		//��ȻRTCģ�鲻��Ҫ�������ã��ҵ���������󱸵����Ȼ����
		//����ÿ���ϵ�󣬻���Ҫʹ��RTCCLK???????
		RCC_RTCCLKCmd(ENABLE);
		//�ȴ�RTCʱ����APB1ʱ��ͬ��
		RTC_WaitForSynchro();

		//ʹ�����ж�
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//ʹ�������ж�
		//RTC_ITConfig(RTC_IT_ALR, ENABLE);// | RTC_IT_SEC
		//�ȴ��������
		RTC_WaitForLastTask();
	}

	return;
}

void RTC_Init(void)
{
	RTC_Config();
}

#endif

