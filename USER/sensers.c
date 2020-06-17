#include "sys.h"

uint32_t i_main_cur_c=0;

struct sta_bump_t g_sta_bump = {0};

//�س������յĶ�ʱ��
void irda_timer_cfg(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 65535;//100ms 80 MS 0xFFFF; //����??��??������????��??��//��?�䨮2a��?65000us =65ms
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1);//(180-1);   // 100K







	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //?????����???��y
	TIM_TimeBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	//TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//

	TIM_Cmd(TIM6, ENABLE);

	//sys->gIO_ird=(GPIOC->IDR) & 0x0f;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 65535;//100ms 80 MS 0xFFFF; //����??��??������????��??��//��?�䨮2a��?65000us =65ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72*100 - 1;//(180-1);   // 100K







	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //?????����???��y
	TIM_TimeBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//

	TIM_Cmd(TIM7, ENABLE);


}




//���̶�ʱ��
void hw_timer_cfg(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	//sys->gIO_ird=(GPIOC->IDR) & 0x0f;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 65535;


	TIM_TimeBaseStructure.TIM_Prescaler = (360-1);   // 400K

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //?????����???��y
	TIM_TimeBaseStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//

	TIM_Cmd(TIM4, ENABLE);


}


/*

PE8	��IR��Դ����	���

PC4	�м������ײ������	����
PB0	���к�����ײ������	����
PC5	��ߺ�����ײ������	����
PA6	�ұߺ�����ײ������	����
PA7	���к�����ײ������	����

PD11	��߷�ײ����
PE15	�ұ߷�ײ����

*/

void   senser_gpio_config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;// ѡ������ģʽΪ�������� GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���Ƶ�����50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP; //GPIO_Mode_IPU;// ѡ������ģʽΪ�������� GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���Ƶ�����50MHz
	GPIO_Init(GPIOE,&GPIO_InitStructure);


	//������ײ
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	//GPIO_Init(GPIOD,&GPIO_InitStructure);

	//��������
	GPIO_InitStructure.GPIO_Pin = PIN_LEFT_MOTOR_LEAVE; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;// ѡ������ģʽΪ�������� GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���Ƶ�����50MHz
	GPIO_Init(PORT_LEFT_MOTOR_LEAVE,&GPIO_InitStructure);

	//�Һ������
	GPIO_InitStructure.GPIO_Pin = PIN_RIGHT_MOTOR_LEAVE; //ѡ������2 3 5
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;// ѡ������ģʽΪ�������� GPIO_Mode_IPU;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //���Ƶ�����50MHz
	GPIO_Init(PORT_RIGHT_MOTOR_LEAVE,&GPIO_InitStructure);

//----------------------------------------------------------------------add by wonton2004 20170115

	GPIO_InitStructure.GPIO_Pin = PIN_VWALL_DET; //ѡ������2 3 5
	GPIO_Init(PORT_VWALL_DET,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DUSTBOX_DET; //ѡ������2 3 5
	GPIO_Init(PORT_DUSTBOX_DET,&GPIO_InitStructure);

	////GPIO_InitStructure.GPIO_Pin = PIN_DUST_DET; //ѡ������2 3 5
	////GPIO_Init(PORT_DUST_DET,&GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = PIN_MOP_DET; //ѡ������2 3 5
	//GPIO_Init(PORT_MOP_DET,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_PSW; //��Դ�����Ƿ��
	GPIO_Init(PORT_PSW,&GPIO_InitStructure);

	////GPIO_InitStructure.GPIO_Pin = PIN_AIRTAKE_DET; //ѡ������2 3 5
	////GPIO_Init(PORT_AIRTAKE_DET,&GPIO_InitStructure);

//--------------------------------------------------------------------------------------------end

//----------------------------------------------------------------------add by wonton2004 20170117
#if DP_PANEL_EN <= 1//�ϰ汾��PCB ��ɨ����������Ϊ�ߵ͵�ƽ��ʽ,���¸��µ�PCBΪAD������ʽ,PCB�汾��Ϊ1.7
	//��ɨ����������,�ߵ�ƽΪ����
	GPIO_InitStructure.GPIO_Pin = PIN_MAIN_MOTOR;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_MAIN_MOTOR, &GPIO_InitStructure);
#endif

#if DP_PANEL_EN >= 4
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
#endif
	//��ɨ�������
	////GPIO_InitStructure.GPIO_Pin = PIN_SIDE_MOTOR_L; //ѡ������2 3 5
	////GPIO_Init(PORT_SIDE_MOTOR_L,&GPIO_InitStructure);

	////GPIO_InitStructure.GPIO_Pin = PIN_SIDE_MOTOR_R; //ѡ������2 3 5
	////GPIO_Init(PORT_SIDE_MOTOR_R,&GPIO_InitStructure);
//---------------------------------------------------------------------------------------------end
	#if FRONT_IR_VERSION_1
	GPIO_InitStructure.GPIO_Pin = PIN_FIRPWR_MID; //ǰײ�м������ĵ�Դ�����,�͵�ƽΪ�ϵ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_FIRPWR_MID, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_FIRPWR_L1R1; //ǰײ�м������ĵ�Դ�����,�͵�ƽΪ�ϵ�
	GPIO_Init(PORT_FIRPWR_L1R1, &GPIO_InitStructure);

	FIRPWR_MID_ON();
	FIRPWR_L1R1_ON();
	#else //�ɵķ���ǰײ�м̰���
	GPIO_InitStructure.GPIO_Pin = PIN_FIRPWR_MID; //ǰײ�м������ĵ�Դ�����,�͵�ƽΪ�ϵ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(PORT_FIRPWR_MID, &GPIO_InitStructure);
	FIRPWR_MID_ON();
	#endif

	//PC9 ǰײ����ƿ�����
	GPIO_InitStructure.GPIO_Pin =   PIN_IR_CTRL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_IR_CTRL, &GPIO_InitStructure);
	//PC9 ǰײ����ƿ�����
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//==============�Եغ���===========

/*
PE12	���ż�ѡ��H/L��ƽ���䣩
PE6 ���ż�ѡ��H/L��ƽ���䣩

*/
#if (0)////VER_BORD_2
		GPIO_InitStructure.GPIO_Pin = PIN_IRD_MK_H;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��¥���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_IRD_MK_H, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = PIN_IRD_MK_L;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_IRD_MK_L, &GPIO_InitStructure);
//#else
		GPIO_InitStructure.GPIO_Pin =	PIN_NEAR_LAN_IR;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_NEAR_LAN_IR, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =	PIN_FAR_LAN_IR;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PORT_FAR_LAN_IR, &GPIO_InitStructure);
#endif

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

	//Ĭ�Ϲر�LED��
	AUTO_LAMP_ON();
}


#if 0
#define CODE_MOTOR_LEFT_ERR			1	//����ֲ���
#define CODE_MOTOR_RIGHT_ERR		2
#define CODE_MOTOR_LEFT_LEAVE		3	//��������
#define CODE_MOTOR_RIGHT_LEAVE		4
#define CODE_MOTOR_DUST_ERR			5	//����������ع���
#define CODE_MOTOR_MID_ERR			6	//��ɨ�����ת
#define CODE_ROBOT_LEAVE			8	//���������
#else// add by wonton2004 20170118
#define CODE_MOTOR_LEFT_ERR			1	//����ֲ���
#define CODE_MOTOR_RIGHT_ERR		2
#define CODE_MOTOR_LEFT_LEAVE		3	//��������
#define CODE_MOTOR_RIGHT_LEAVE		4
#define CODE_MOTOR_DUST_ERR			5	//����������ع���
#define CODE_MOTOR_MID_ERR			6	//��ɨ�����ת
#define CODE_ROBOT_LEAVE			8	//���������
#endif

//#if BORD_VERSION == 2
//#define MOTOER_SENSER_CHECK		0

//#else

//#endif

//static uint8_t debug_ir_original_bkbuff[3];
uint8_t debug_ir_original_bk[4];

void statistics(int *data, int cnt, float *ave, float *stdevp)
{
	int data_t = 0;

	for (int i = 0; i<cnt; i++){
		data_t += data[i];
	}
	*ave = data_t / cnt;
	float data_std = 0;
	for (int i = 0; i<cnt; i++){
		float delta_d= *ave - data[i];
		data_std += delta_d*delta_d;
	}
	*stdevp = sqrt(data_std / (cnt-1));
}


float  find_threshold( float value_in,float table[][2],uint16_t cnt){

	if(value_in<table[0][0]){
		return table[0][1];
	}else if(value_in>=table[cnt-1][0]){
		return table[cnt-1][1];
	}else{
		for (int i = 0; i < cnt-1; i++)
		{
			if (value_in>=table[i][0] && value_in < table[i + 1][0]){
				float dy = table[i + 1][1] - table[i][1];
				float dx = table[i + 1][0] - table[i][0];
				if (dx != 0){
					float value_out = dy / dx*(value_in - table[i][0]) + table[i][1];
					return (value_out);
				}
			}
		}
	}
	return -1;
}

#if CURR_ALA_TABLE
#define WHEEL_THRED_CNT 13
#define IDC_THRED_CNT 14


static float idc_table[IDC_THRED_CNT][2]={//��ˢ��ת��PWMֵ����ѹֵ
	{	1.25,	95	},
	{	2.5,	95	},
	{	3.75,	95	},
	{	5,	110.75	},
	{	5.5,	114 },
	{	6,	118.75	},
	{	6.5,	126 },
	{	7,	130.75	},
	{	7.5,	137 },
	{	8,	142.75	},
	{	8.5,	146.75	},
	{	9,	152.25	},
	{	9.5,	158 },
	{	10, 163.75	}

};

static float l_wheel_table[WHEEL_THRED_CNT][2]={//��ˢ��ת��PWMֵ����ѹֵ
	{	12.4865 ,22.5	},
	{	13.73515,	23.5	},
	{	14.9838, 25	},
	{	16.23245,	30	},
	{	17.4811 ,31.5	},
	{	18.72975,	34.5	},
	{	19.6304 ,37	},
	{	20.8573 ,37	},
	{	22.0842 ,41	},
	{	22.57496,	44.5	},
	{	23.25748,	45	},
	{	23.75232,	46.5	},
	{	25.2,	50	}

};

static float r_wheel_table[WHEEL_THRED_CNT][2]={//���ֶ�ת��PWMֵ����ѹֵ

		{	12.42,	20	},
		{	13.662	,20.5	},
		{	14.904,	25	},
		{	16.146,	27.5	},
		{	17.388,	30	},
		{	18.63	,32.5	},
		{	19.872,	35.5	},
		{	21.114,	37	},
		{	22.356,	43.5	},
		{	22.8528, 39.5	},
		{	23.3496, 43.5	},
		{	23.8464 ,45	},
		{	25.2, 50	}


};
#endif
#if  CAPART_CHECK
#define MAX_CAPART_CNT  25
int data_l[MAX_CAPART_CNT]={0};
int data_r[MAX_CAPART_CNT]={0};
#endif
//-----------------------------------------------------------------modified by wonton2004 20170116
//void check_sensens(uint16_t *g_sta)
void check_sensens(void)
//---------------------------------------------------------------------------------------------end
{


#if MOP_EN
	static uint16_t  c_mopmode = 0,c_no_nopmode=0;
#endif
#if DUST_BOX_CHECK
	static uint16_t c_dustbox = 0;//,dustbox_flag = 0;//,c_airtake = 0;c_dustbox = 0,
	static uint32_t dustbox_leave_tmr = 0;//,dustbox_flag = 0;//,c_airtake = 0;c_dustbox = 0,
#endif
	static uint16_t /*c_motor_left_leave = 0,*/c_motor_left_leave_ok=0;
	static uint8_t motor_left_leave_flag = 0;//,motor_right_leave_flag = 0;

	uint32_t t= msTmr;		//ȡ��ʱ���


	proc_key_task(sys->sState);

	uint8_t ac_flag=0,dc_flag=0;
	//================�����=======================================
	if(sys->sState != SYS_CHARGE)
	{
		if(EXTERAL_AC_DETECT())
			ac_flag = 1;
		else
			ac_flag =0;
		if(DOCK_DETECT())
			dc_flag = 1;
		else
			dc_flag = 0;
		if((ENABLE == sys->dccheck_enable) && (ac_flag/*EXTERAL_AC_DETECT()*/ || (dc_flag/*DOCK_DETECT()*/&& (sys->sState == SYS_DOCK || sys->sState == SYS_IDLE))))
		{

			log_printf("foud charge..,stop work=%d,%d,\r\n",ac_flag, dc_flag);

			//delay_ms(10);
			{
				int i;
				for(i=0;i<10;i++)
				{
					if(!(EXTERAL_AC_DETECT() || DOCK_DETECT()))
						break;
					delay_ms(1);
				}
				log_printf("check dock ok,i=%d\r\n",i);
				if(i>=5)
				{
					motor_run(GO_STOP,0,0,0);
					STOP_ALL_MOTOR();

					/////����Ϊ�����̬
					init_sys_sta(SYS_CHARGE);
					sys->charge_sta = NO_CHARGE;
					if(EXTERAL_AC_DETECT())
						sys->charge_sta |= AC_CHARGE;
					else
						sys->charge_sta |= DOCK_CHARGE;
					sys->power_on = 1;
					all_key_led_off();
					log_printf("charge_sta:%d\r\n",sys->charge_sta);
				}
			}
			///////////-------------�����ѹ�澯
			sys->timeout_navi_work = 0;
		}
	}

	if(sys->power_on == 0)	//�ر�״̬����⴫����
		return ;
//	static uint32_t c_photoresistor_dark=0,c_photoresistor_light=0;
	static uint32_t t_v_phtr=0,c_v_phts=0;
	static uint32_t  t_v_phtr_on=0,t_v_phtr_off=0;

	if(sys->sState == SYS_CHARGE || sys->sState == SYS_IDLE || sys->sState == SYS_DOCK || sys->sState == SYS_DOCK_NEAR)
	{
		sys->auto_lamp_sta =0;
		t_v_phtr = c_v_phts = 0;
		t_v_phtr_off = t_v_phtr_on = 0;
		AUTO_LAMP_OFF();
	}else
	{
		if(cfg->r[IC_LED_EN] == 1)
		{
			t_v_phtr +=V_PHOTORESISITOR();
			c_v_phts++;
			if(c_v_phts >=20)
			{
				uint16_t v_phts = t_v_phtr / c_v_phts;
				t_v_phtr = c_v_phts = 0;
				if(v_phts > cfg->v_phts_led)		//�й�
				{
					t_v_phtr_off = 0;
					if(t_v_phtr_on == 0)
						t_v_phtr_on = t;
					if((t-t_v_phtr_on) > cfg->t_phts_led)
					{
						AUTO_LAMP_ON();
						t_v_phtr_on = t;
					}
				}else
				{

				t_v_phtr_on = 0;
				if(t_v_phtr_off ==0)
					t_v_phtr_off = t;
				if((t - t_v_phtr_off) > cfg->t_phts_led)
				{
					AUTO_LAMP_OFF();
					t_v_phtr_off = t;
				}
			}

			}
		}

	}

	//micony20200112 ���ֳ�����Ͻ�
	if(sys->sState == SYS_CHARGE)
	{
		return ;
	}
//static uint16_t cck=0;
static uint16_t c_mid_ok=0;
	static uint32_t t_mid_err=0;

#if MID_ADC_DEBUG
static uint32_t printf_cnt =0;
//static uint32_t time_period =10;
//static uint32_t time_period1 =10;
#endif
static uint32_t mid_adc_t =0,mid_adc_cnt=0;;
//static uint32_t	dust_last=0,dust_zong_cnt=0;

///////////////////////�����ɨ����//////////////////////////
//if(cck++>=10)

//	cck=0;


 	//	dust_last =DUST_VALUE();

#if MID_ADC_DEBUG

		printf_cnt++;

		if(sys->sState ==0){
			printf_cnt=0;
		}
			static int16_t v_dust_adc_l=0;
			if(disXY((int)v_dust_adc_l,(int)V_DUST_ADC())>20)
			{
			}
			v_dust_adc_l=V_DUST_ADC();
#endif

#if  CAPART_CHECK

	static int cap_cnt=0,get_cnt=0;

	if(sys->sState == SYS_NAVIGAT)
	{
		get_cnt++;
		if(get_cnt==10){

			data_l[cap_cnt] = sys->g_buton[0][0];
			data_r[cap_cnt++] = sys->g_buton[0][2];
			//log_printf("g_buton %d %d cnt %d \r\n",sys->g_buton[0][0],sys->g_buton[0][2],cap_cnt);
			get_cnt=0;
		}

		if(cap_cnt>=MAX_CAPART_CNT){
			cap_cnt=0;
			float ave_cap_l=0,stdev_l=0;
			float ave_cap_r=0,stdev_r=0;

			statistics(data_l,MAX_CAPART_CNT, &ave_cap_l, &stdev_l);
			statistics(data_r,MAX_CAPART_CNT, &ave_cap_r, &stdev_r);

			log_printf("capl %.2f %.2f %.2f %.2f\r\n",ave_cap_l,stdev_l,ave_cap_r,stdev_r);

			if( stdev_l>cfg->r[CARPET_L_PRA]&&stdev_r>cfg->r[CARPET_R_PRA])
			{
				if(sys->Blankets_ground!=SET)
					log_printf("robot--carpet_ground\r\n");
				sys->Blankets_ground = SET;
			}
			else
			{
				if(sys->Blankets_ground!=RESET)
					log_printf("robot--floor_ground\r\n");
				sys->Blankets_ground = RESET;
			}
		}
	}
#endif
#define MID_ADC_CNT1 100
		//��ɨ����ĵ���
		if(mid_adc_cnt<MID_ADC_CNT1)		// 100����һ��ƽ��ֵ
		{
			mid_adc_t+=I_MID_ADC() ;
			mid_adc_cnt++;
			//log_printf("AGITATOR_PWM0  %d ,%d,%d\r\n",mid_adc_t,I_MID_ADC,mid_adc_cnt);
		}
		else
		{
			mid_adc_t/=MID_ADC_CNT1;
			//log_printf("AGITATOR_PWM1  %d ,%d,%d\r\n",mid_adc_t,I_MID_ADC,cfg->i_agi_max);
#if CURR_ALA_TABLE
			float v_agitator = (float)AGITATOR_PWM/1000.0f*sys->volt/1000.0f;		//pwm �ӵ�ѹ����������ʩ���ڵ�����˵ĵ�ѹ
			float i_thred  = (find_threshold(v_agitator,idc_table,IDC_THRED_CNT)*AGI_AMP_FACTOR);	//���ȡ����ֵ
			sys->vagi = i_thred;
#endif
			sys->agi_curr = mid_adc_t;

#if CURR_ALA_TABLE
			if(mid_adc_t >=i_thred &&sys->sState!= SYS_IDLE)
#else
			if(mid_adc_t >=cfg->i_agi_max &&sys->sState!= SYS_IDLE)
#endif
			{
	#if CURR_ALA_TABLE
				log_printf("AGITATOR_PWM2 %d %.2f %d %d ithread %.2f\r\n",AGITATOR_PWM,v_agitator,mid_adc_t,I_MID_ADC(),i_thred);
	#else
				log_printf("AGITATOR_PWM3 %d ,i=%d,%d, ithread %.2f\r\n",AGITATOR_PWM,mid_adc_t,I_MID_ADC(),cfg->i_agi_max);
	#endif
				c_mid_ok = 0;
				if(t_mid_err ==0)			//��һ�Σ���¼ʱ��
					t_mid_err=t;			//ˢ��ʱ��
				else
				{
					if((t - t_mid_err) > cfg->t_agi_max)  //ʱ�䳬������澯
					{
						log_printf("I_MID ERR,pwm=%d,v=%3.2f,t=%3.2f\r\n",AGITATOR_PWM,0,cfg->i_agi_max);
						t_mid_err=t;
						STOP_ALL_MOTOR();
						ny3p_play(VOICE_CHECK_M_MTR);
							motor_run(GO_STOP,0,0,0);
							sys->sState = SYS_IDLE;

							sys->t_beep_on = 10000;		//��10����
						sys->work_errcode |= MWERR_MIDMOTOR;//add by wonton2004 20170118
						sys->wifistatechange = 1;
					}
				}
			}
			else		//��λ����
				{
					if(t_mid_err > 0)		//���֮ǰ�����߳���Χ
					{
						if(c_mid_ok++ > 2)		//����500�ε����������ޣ�����λʱ��
						{
							t_mid_err = 0;			//��λʱ��
						}
					}
				}
				mid_adc_t=0;
				mid_adc_cnt=0;
			}


	/*////////////���λ������ǰ�Ĵ��룬���ڰ���λ����////////////////

	if(I_MID_ADC() >= cfg->i_agi_max)  //��������ֵ��������Χ
	{
		//SET_MID_MOTER
		c_mid_ok = 0;
		}
	}
*/
	//================�������====================
		static uint32_t t_dust_full=0;
		static uint32_t c_dust_full=0;
		if(sys->sState == SYS_IDLE)
		{
			c_dust_full = t_dust_full=0;
		}else
		//�������
		//if(DUST_VALUE() > cfg->r[IC_DUSTBOX_MAX])		//�ҳ�Ũ�ȳ���
		if(0)
		{
			c_dust_full = 0;
			if(t_dust_full==0)
				t_dust_full = t;
			else if((t- t_dust_full) / 1000 > cfg->r[IC_DUSTBOX_T])	//����һ��ʱ��
			{
				log_printf("t_dust_full ERR\r\n");
				t_dust_full=t;
				STOP_ALL_MOTOR();
				ny3p_play(VOICE_CHECK_LINE);
				motor_run(GO_STOP,0,0,0);
				sys->sState = SYS_IDLE;
				sys->t_beep_on = 10000; 	//��10����
				sys->work_errcode |= MWERR_CLRDUSTBOX;//add by wonton2004 20170118
				sys->wifistatechange = 1;

			}
		}else if(t_dust_full !=0)
		{
			if(DUST_VALUE()  < cfg->v_dust_max)		//ҪС�ڶ���ֵ����������
			{
				if(c_dust_full++ >= 10)	//
				{
					t_dust_full=0;
					log_printf("c_dust_full zero\r\n");
				}

			}
		}

		//================���ϼ��=====================================
#if DUST_BOX_CHECK
	//���м��,�͵�ƽΪ���н���
	////log_printf("DUSTBOX:%d\r\n", READ_DUSTBOX_DET())
	if(READ_DUSTBOX_DET() && (sys->sState != 0xff))
	{
		if(c_dustbox < 8000)
			c_dustbox ++;
		if(c_dustbox  >= 8000)//����δ����澯
		{
			if(sys->sState != SYS_IDLE && sys->sState != SYS_CHARGE)
			{

				ny3p_play(VOICE_BOX_OK);
				log_printf("\r\ndustbox!\r\n");
				motor_run(GO_STOP,0,0,0);
				STOP_ALL_MOTOR();
//				dustbox_flag = 1;
				sys->sState = SYS_IDLE;
				sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
				//if(sys->work_errcode < MWERR_AGITATOR_REPLACEMENT)
				{
					sys->t_beep_on = 10000;		//��10����
					sys->work_errcode |= MWERR_DUSTBOX;//add by wonton2004 20170118
					sys->wifistatechange = 1;
					dustbox_leave_tmr = msTmr;
#if DUST_BOX_FULL_CHECK		//�����м��
					dust_box_take_tmr = msTmr;
#endif
				}

			}
			else if(sys->sState == SYS_IDLE)
			{
//				dustbox_flag = 1;
				sys->sState = SYS_IDLE;
				sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
			//	if(sys->work_errcode < MWERR_DISPSTA)
				{
					if((sys->work_errcode & MWERR_DUSTBOX) != MWERR_DUSTBOX)
					{
						sys->work_errcode |= MWERR_DUSTBOX;//add by wonton2004 20170118
						sys->wifistatechange = 1;
						dustbox_leave_tmr = msTmr;
#if DUST_BOX_FULL_CHECK		//�����м��
						dust_box_take_tmr = msTmr;
#endif
					}
				}
			}
#if DUST_BOX_FULL_CHECK		//�����м��
			else if(sys->sState == SYS_CHARGE)
			{
				if( dustbox_flag ==0)
				{
						dust_box_take_tmr = msTmr;
						dustbox_flag = 1;
				}
			}
#endif
		}
	}
	else
	{
		if(sys->work_errcode & MWERR_DUSTBOX)
		{
			sys->work_errcode &= ~MWERR_DUSTBOX;
			sys->t_beep_on = 0;
			log_printf("dust box ala reset...\r\n");
		if(	sys->work_errcode & MWERR_CLRDUSTBOX)
		{
			//�����뿪������ʱ�䣬�����೤ʱ�������㵹���������澯�ָ���
			if((msTmr -dustbox_leave_tmr)>12000)
			{
				dustbox_leave_tmr = msTmr;
				sys->work_errcode &= ~MWERR_CLRDUSTBOX;
				log_printf("MWERR_CLRDUSTBOX\r\n");
				}
			}
		}
		c_dustbox = 0;
	}
#endif

	//=======================���ּ��===============================
#if MOTOER_SENSER_CHECK
			static uint16_t c_left_ok=0,c_right_ok=0;
		static uint32_t t_left_err=0,t_right_err=0;
	if(sys->mState == GO_STOP)
	{
		sys->i_left_whele = sys->i_right_whele =sys->i_whele_cnt = 0;
		sys->left_curr =sys->right_curr=0;
		t_left_err=t_right_err=0;
	}
	else
	{
		sys->i_left_whele+=I_MOTER_LEFT();
		sys->i_right_whele+=I_MOTER_RIGHT();
		sys->i_whele_cnt++;

		if(sys->i_whele_cnt >=500)				//ȡ������ƽ����
		{
			sys->left_curr = sys->i_left_whele / sys->i_whele_cnt;
			sys->right_curr = sys->i_right_whele / sys->i_whele_cnt;
			sys->i_left_whele = sys->i_right_whele =sys->i_whele_cnt = 0;
#if CURR_ALA_TABLE

			float lv_forwrk = (float)(1000-L_FORWRK_PWM)/1000.0f*sys->volt/1000.0f;
			sys->vleft = lv_forwrk;
			float li_thred = (find_threshold(lv_forwrk,l_wheel_table,WHEEL_THRED_CNT)*LMOT_AMP_FACTOR);
#endif
		//	if(printf_cnt%time_period1==0 &&sys->sState !=SYS_IDLE){
		//		log_printf("lcurr %d %d lthred %.2f %d %.2f\r\n",sys->left_curr,L_FORWRK_PWM,li_thred,sys->volt,lv_forwrk);
		//	}

#if CURR_ALA_TABLE
			if(sys->left_curr >= li_thred)		/////E5һֱ��ʾ���а�����������ʧ
#else
			if(sys->left_curr >= cfg->i_left_whele_max)
#endif
			{

				c_left_ok = 0;
				log_printf("left:,%d,t=%d,%d\r\n",I_MOTER_LEFT(),t_left_err,t);
				if(t_left_err ==0)
				{
					t_left_err = t;
					log_printf("big left cur=%d,t=%d\r\n",sys->left_curr,t);
				}
				else if((t-t_left_err) > MAX_COUNT)
				{

					log_printf("left:,%d,t=%d,%d\r\n",sys->left_curr,t_left_err,t);
					log_printf("motor_left_wheel_bind\r\n");
					t_left_err = t;
		//			c_motor_left = 0;
					ny3p_play(VOICE_CHECK_LINE);
					motor_run(GO_STOP,0,0,0);
					STOP_ALL_MOTOR();
					sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
					sys->work_errcode |= MWERR_LEFT_WHEELBIND;//add by wonton2004 20170118
//					dp_tx_data(DP_SYS_STA, WORK_STA_ERR, MWERR_LEFT_WHEELBIND, 0,0);		//������ modified by wonton2004 20170118
					sys->t_beep_on = 10000;		//��10����
					sys->sState = SYS_IDLE;
					navigat->suspend =FALSE;
				}
			}
			else
			{
				c_left_ok++;
				//log_printf("c_left_ok %d-%3.2f\r\n",sys->left_curr,li_thred);

				if(c_left_ok>=MIX_I_COUNT)
				{
					t_left_err = 0;
				}
			}
#if CURR_ALA_TABLE

			float rv_forwrk = (float)(1000-R_FORWRK_PWM)/1000.0f*sys->volt/1000.0f;
			float ri_thred = (find_threshold(rv_forwrk,r_wheel_table,WHEEL_THRED_CNT)*LMOT_AMP_FACTOR);
			sys->vright = rv_forwrk;
#endif

		//	if(printf_cnt%time_period1==0 &&sys->sState !=SYS_IDLE){
		//		log_printf("rcurr %d %d rthred %.2f %d %.2f\r\n",sys->right_curr,R_FORWRK_PWM,ri_thred,sys->volt,rv_forwrk);
		//	}
#if CURR_ALA_TABLE
			if(sys->right_curr >= ri_thred)			/////E5һֱ��ʾ���а�����������ʧ
#else
			if(sys->right_curr >= cfg->i_whele_max)
#endif
			{
				c_right_ok = 0;
				log_printf("right:,%d,t=%d,%d\r\n",I_MOTER_RIGHT(),t_right_err,t);
				if(t_right_err ==0)
				{
					t_right_err = t;
					log_printf("big right cur=%d,t=%d\r\n",sys->right_curr,t);
				}
				else if((t-t_right_err) > MAX_COUNT)
				{
					log_printf("right:%d,t=%d,%d\r\n",sys->right_curr,t_right_err,t);
					log_printf("motor_right_wheel_bind\r\n");
					t_right_err = t;

					ny3p_play(VOICE_CHECK_LINE);
					motor_run(GO_STOP,0,0,0);
					STOP_ALL_MOTOR();
					sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
					sys->work_errcode |= MWERR_RIGH_WHEELBIND;//add by wonton2004 20170118
//					dp_tx_data(DP_SYS_STA, WORK_STA_ERR, MWERR_RIGH_WHEELBIND, 0,0);		//������ modified by wonton2004 20170118
					sys->t_beep_on = 10000;		//��10����
					sys->sState = SYS_IDLE;
				}
			}
			else
			{
				c_right_ok++;
				//log_printf("c_right_ok %d-%3.2f\r\n",sys->right_curr,ri_thred);
				if(c_right_ok>=MIX_I_COUNT)
				{
					t_right_err = 0;
				}
			}

		}
	}


	//====================================������ռ�� ================================
	//if(sys->work_errcode != MWERR_FLRSENSOR && sys->work_errcode != MWERR_DUSTBOX)	///��������ձ���,���и澯,���Ӹ澯�����
	static uint32_t t_left_leave = 0;
	if((LEFT_MOTOR_LEAVE() == 0 || RIGHT_MOTOR_LEAVE()==0 ) /*&& sys->sState != SYS_IDLE && sys->sState != SYS_CHARGE*/)		////0���		E4һֱ��ʾ���ָ���E4��ʧ
	{	//���кͳ�粻���
		//˫����أ���ʱ��϶�
		uint32_t t_leave_k=0;
	   	if(LEFT_MOTOR_LEAVE() == 0 && RIGHT_MOTOR_LEAVE()==0 )
			t_leave_k = (cfg->t_drop_max / 100)*1000;  //��λ��˫�����ʱ��
	   	else
	   		t_leave_k = (cfg->t_drop_max % 100)*1000;	//��λ���������ʱ��

		c_motor_left_leave_ok = 0;
		if(t_left_leave ==0  )
			t_left_leave=t;
		else if((t - t_left_leave) > t_leave_k)  //���������ʱ��
		{
			log_printf("whele leave,err,left,sta=%d,t=%d\r\n",sys->sState,msTmr - t_left_leave);
			t_left_leave = t;
			if(sys->sState != SYS_IDLE)
			{
				log_printf("motor whele leave stop...\r\n");
				ny3p_play(VOICE_CHECK_LEAVE);
				motor_run(GO_STOP,0,0,0);
				//log_printf("work_errcode:%04x_%04x_%04x\r\n",sys->work_errcode,(sys->work_errcode & MWERR_WHEELSUSP),MWERR_WHEELSUSP);
			}
			sys->sState = SYS_IDLE;

			if((sys->work_errcode & MWERR_WHEELSUSP) != MWERR_WHEELSUSP)
			{
				log_printf("MWERR_WHEELSUSP\r\n");
				sys->t_beep_on = 10000;		//��10����
				motor_left_leave_flag = 1;
				sys->work_errcode |= MWERR_WHEELSUSP;//add by wonton2004 20170118
			}
			sys->wifistatechange = 1;

		}
	}
	else
	{
		if(sys->sState == SYS_IDLE)  //�ָ�������ģʽ�£��������ŵ��ˣ���ָ���
		{
			if(sys->work_errcode & MWERR_WHEELSUSP)
			{
				if(motor_left_leave_flag == 1)
				{
					//c_motor_left_leave = 0;
					motor_left_leave_flag = 0;
					log_printf("MWERR_WHEELSUSP clear\r\n");
					//sys->work_errcode = MWERR_NONE;
					sys->work_errcode &= ~MWERR_WHEELSUSP;
					sys->dis_statechange = 1;
					sys->wifistatechange = 1;
					sys->t_beep_on = 0;
					//add 2020-02-26 jzz
					//ͬʱ�������ˢ,������ˢʱ��
					if(cfg->t_agitator_inspection >=300)
					{
						log_printf("t_agitator_inspection err:%d clear to 0\r\n",cfg->t_agitator_inspection);
						cfg->t_agitator_inspection = 0;
						if((sys->work_errcode & MWERR_AGITATOR_INSPECTIONG))
							sys->work_errcode &= ~(MWERR_AGITATOR_INSPECTIONG);
						save_cfg();
					}

					if(cfg->t_agitator_replacemen >=3000)	// 50��Сʱ
					{
						log_printf("t_agitator_replacemen err:%d clear to 0\r\n",cfg->t_agitator_replacemen);
						cfg->t_agitator_replacemen = 0;
						if((sys->work_errcode & MWERR_AGITATOR_REPLACEMENT))
							sys->work_errcode &= ~(MWERR_AGITATOR_REPLACEMENT);
						save_cfg();
					}
				}
				//sys->work_errcode = sys->work_errcode == MWERR_WHEELSUSP ? MWERR_NONE:sys->work_errcode;//add by wonton2004 20170118
			}

		}
		if(c_motor_left_leave_ok++ > MIX_LEAVE_COUNT)
			t_left_leave = 0;
	}


////////////////���������ת�����澯
	static uint16_t c_dust_ok=0;
	static uint32_t t_dust_err=0;
	static uint32_t pri_c1=0;
	if((sys->sState != SYS_IDLE || sys->f_work_mode == MWO_MANUAL) && (sys->sState != SYS_DOCK) && (sys->sState != SYS_DOCK_NEAR) &&
	(V_DUST_ADC() >= cfg->r[IC_FAN_CUR_ERR]))  //��������ֵ��������Χ
	{
		c_dust_ok = 0;
		if(t_dust_err ==0)			//��һ�Σ���¼ʱ��
			t_dust_err=t;			//ˢ��ʱ��
		else
		{
			if((pri_c1++>10) && (pri_c1%10 == 0))
				log_printf("dust err,t=(%d,%d,%d,)i=(%d,%d,)\r\n",(cfg->r[IC_FAN_CUR_ERR]%100),t_dust_err,t,cfg->r[IC_FAN_CUR_ERR],V_DUST_ADC());
			if(((t - t_dust_err)/ 1000) > (cfg->r[IC_FAN_CUR_ERR]%100))  //ʱ�䳬������澯
			{
				log_printf("dust ERR\r\n");
				t_dust_err=t;
				STOP_ALL_MOTOR();
				ny3p_play(VOICE_CHECK_LINE);
				motor_run(GO_STOP,0,0,0);
				sys->sState = SYS_IDLE;
				DUST_MOTOR_OFF();
				sys->t_beep_on = 10000;		//��10����
				sys->work_errcode |= MWERR_SUCTION_MOTOR_MALFUNCTIONG;//add by wonton2004 20170118
				sys->wifistatechange = 1;
			}
		}
	}
	else		//��λ����
	{
		if(t_dust_err > 0)		//���֮ǰ�����߳���Χ
		{
			if(c_dust_ok++ > 10)		//����100�ε����������ޣ�����λʱ��
			{
				if(t_dust_err)
					log_printf("dust err reset\r\n");
				t_dust_err = 0;			//��λʱ��
				pri_c1 = 0;
			}
		}
	}
////////////////������������澯

////////////////�����ڶ�ס�澯,������С
	static uint16_t c_dust_lrc_ok=0;
	static uint32_t t_dust_lrc_err=0;
	static uint32_t pri_c2=0;
	if((sys->sState != SYS_IDLE || sys->f_work_mode == MWO_MANUAL) && (sys->sState != SYS_DOCK) && (sys->sState != SYS_DOCK_NEAR) &&
	((sys->dust_pwm_lev == DUST_MODE_SILENT) && (V_DUST_ADC() > (cfg->r[IC_FAN_S_LRCUR_ERR]-80)) &&(V_DUST_ADC() < cfg->r[IC_FAN_S_LRCUR_ERR]) ||
	((sys->dust_pwm_lev == DUST_MODE_NORMAL) && (V_DUST_ADC() > (cfg->r[IC_FAN_N_LRCUR_ERR]-300)) && (V_DUST_ADC() < cfg->r[IC_FAN_N_LRCUR_ERR])) ||
	((sys->dust_pwm_lev == DUST_MODE_TURBO) && (V_DUST_ADC() > (cfg->r[IC_FAN_T_LRCUR_ERR]-600)) && (V_DUST_ADC() < cfg->r[IC_FAN_T_LRCUR_ERR]))))	//��������ֵ��������Χ
	{
		c_dust_lrc_ok = 0;
		if(t_dust_lrc_err ==0)			//��һ�Σ���¼ʱ��
			t_dust_lrc_err=t;			//ˢ��ʱ��
		else
		{
			if((pri_c2++>10) && (pri_c2%10 == 0))
			{
				if(sys->dust_pwm_lev == DUST_MODE_SILENT)
					log_printf("dust_lrc err,t=(%d,%d,%d,)i=(%d,%d,)\r\n",cfg->r[IC_FAN_LRCUR_TMR],t_dust_lrc_err,t,cfg->r[IC_FAN_S_LRCUR_ERR],V_DUST_ADC());
				else if(sys->dust_pwm_lev == DUST_MODE_NORMAL)
					log_printf("dust_lrc err,t=(%d,%d,%d,)i=(%d,%d,)\r\n",cfg->r[IC_FAN_LRCUR_TMR],t_dust_lrc_err,t,cfg->r[IC_FAN_N_LRCUR_ERR],V_DUST_ADC());
				else if(sys->dust_pwm_lev == DUST_MODE_TURBO)
					log_printf("dust_lrc err,t=(%d,%d,%d,)i=(%d,%d,)\r\n",cfg->r[IC_FAN_LRCUR_TMR],t_dust_lrc_err,t,cfg->r[IC_FAN_T_LRCUR_ERR],V_DUST_ADC());
			}
			if(((t - t_dust_lrc_err)/ 1000) > (cfg->r[IC_FAN_LRCUR_TMR]))  //ʱ�䳬������澯
			{
				log_printf("dust_lrc ERR\r\n");
				t_dust_lrc_err=t;
				STOP_ALL_MOTOR();
				DUST_MOTOR_OFF();
				ny3p_play(VOICE_CHECK_LINE);
				motor_run(GO_STOP,0,0,0);
				sys->sState = SYS_IDLE;

				sys->t_beep_on = 10000; 	//��10����
				sys->work_errcode |= MWERR_FAN_FILTER_BLOCKAGE;//
				sys->wifistatechange = 1;
			}
		}
	}
	else		//��λ����
	{
		if(t_dust_lrc_err > 0)		//���֮ǰ�����߳���Χ
		{
			if(c_dust_lrc_ok++ > 30)		//����100�ε����������ޣ�����λʱ��
			{
				if(t_dust_lrc_err)
					log_printf("dust_lrc err reset\r\n");
				t_dust_lrc_err = 0; 		//��λʱ��
				pri_c2 = 0;
			}
		}
	}
////////////////���������ת�澯
#if DUST_BOX_CHECK
	//���м��,�͵�ƽΪ���н���
	if(READ_DUSTBOX_DET() && (sys->sState != 0xff))
	{
		if(c_dustbox < 8000)
			c_dustbox ++;
		if(c_dustbox  >= 8000)//����δ����澯
		{
			//if(c_dustbox > 100)c_dustbox = 101;
			//if(ROBOT_MOVE_STA())
			if(sys->sState != SYS_IDLE && sys->sState != SYS_CHARGE)
			{
				//if(c_dustbox == 11)
				{
					ny3p_play(VOICE_BOX_OK);
					log_printf("\r\ndustbox!\r\n");
				}
				motor_run(GO_STOP,0,0,0);
				STOP_ALL_MOTOR();
//				dustbox_flag = 1;
				//m_hal->suspend =FALSE;
				sys->sState = SYS_IDLE;
				sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
			//	if(sys->work_errcode < MWERR_DISPSTA)
				{
					//if(sys->work_errcode != MWERR_DUSTBOX)
					{
						sys->t_beep_on = 10000;		//��10����
						sys->work_errcode |= MWERR_DUSTBOX;//add by wonton2004 20170118
						sys->wifistatechange = 1;
#if DUST_BOX_FULL_CHECK		//�����м��
						//dust_box_take_flag = 1;		//����������
						dust_box_take_tmr = msTmr;
#endif
					}
				}

			}
			else if(sys->sState == SYS_IDLE)
			{
					//log_printf("\r\ndustbox!\r\n");

				//motor_run(GO_STOP,0,0,0);
				//STOP_ALL_MOTOR();
//				dustbox_flag = 1;
				//m_hal->suspend =FALSE;
				sys->sState = SYS_IDLE;
				sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
			//	if(sys->work_errcode < MWERR_DISPSTA)
				{
					if((sys->work_errcode & MWERR_DUSTBOX) != MWERR_DUSTBOX)
					{
						sys->work_errcode |= MWERR_DUSTBOX;//add by wonton2004 20170118
						sys->wifistatechange = 1;
#if DUST_BOX_FULL_CHECK		//�����м��
						//dust_box_take_flag = 1;		//����������
						dust_box_take_tmr = msTmr;
#endif
					}
				}
			}
#if DUST_BOX_FULL_CHECK		//�����м��
			else if(sys->sState == SYS_CHARGE)
			{
				if( dustbox_flag ==0)
				{
						dust_box_take_tmr = msTmr;
						dustbox_flag = 1;
				}
			}
#endif
		}
	}
	else //if(ROBOT_MOVE_STA())
	{
		//if(c_dustbox > 0)
		//	c_dustbox--;
#if DUST_BOX_FULL_CHECK		//�����м��
		if(dustbox_flag ==1)
		{
			dustbox_flag = 0;
			if(msTmr-dust_box_take_tmr>=20000)		//����������ʱ��
			{
				log_printf("clear dust_full_check_pro,dust_box_take_tmr timeout:%d\r\n",msTmr-dust_box_take_tmr);
				dust_full_check_pro(1); 			//�����������,�����ݳ�ʼ��
				sys->dis_statechange = 1;
				sys->wifistatechange = 1;
			}
			else
			{
				log_printf("dust_box_take_tmr to shoot:%d \r\n",msTmr-dust_box_take_tmr);
			}
		}
#endif

		if(sys->work_errcode & MWERR_DUSTBOX )
		{
			//sys->work_errcode = MWERR_NONE;
			sys->work_errcode &= ~MWERR_DUSTBOX;

//			dustbox_flag = 0;
			sys->dis_statechange = 1;
			sys->wifistatechange = 1;
		}
		c_dustbox = 0;
	}
#endif
/*
	if(!sys->work_errcode && (sys->work_status_info & MSINFO_ERROR))
	{
		sys->work_status_info &= ~MSINFO_ERROR;
		dp_tx_data(DP_SYS_STA, sys->work_status_info,0,0,0);
	}

//	if(sys->work_errcode >= MWERR_DISPSTA)//�Կ�����ͷ����,�˴���ֻ������������
	{
		sys->work_status_info |= WORK_STA_ERR;//add by wonton2004 20170118
		sys->work_status_info |= (sys->silent << WORK_STA_SILENT);
	}

*/


//--------------------------------------------------------------------------------------------end
#endif



}
//��ӡ�����״̬
void printf_power_sta(void)
{
	static volatile uint8_t t_flag=0;
	sys->t_navi_work= (msTmr - sys->t_work) / 1000;
#if PRINTF_PWR_STA
		log_printf("&<%d,%d>&\r\n",sys->t_navi_work,sys->volt);
#endif


#if 0
	if(mcu_get_wifi_work_state() == WIFI_CONNECTED ||mcu_get_wifi_work_state() == WIFI_CONNECTEDING)
	{
		if((sys->sState == SYS_NAVIGAT || sys->sState == SYS_FOCUS || sys->sState == SYS_RANDOM))
		{
			if(t_flag++ >=5)
			{
				t_flag = 0;
				if(sys->t_navi_work>=60)
				{
					mcu_dp_value_update(DPID_CLEARTIME,sys->t_navi_work/60);	//VALUE�������ϱ�;	ʵʱ��ɨʱ��
					log_printf("wifi_updata_tmr_data:%d\r\n",sys->t_navi_work/60);
				}
			}
		}
	}
#endif
}
uint16_t port_arry[16];
#define C_IRD_ALA	30
#define SENSE_TIMEOUT		30

/*
	ѭ�����100�Σ�
	����������ʱ536us ,0.536ms
	��ʱ����600us����ž���500us��600us����
*/
#define MAC_IRD_COUNT		10
/**************************************************************
 * �������� : get_sensers
 * ��    �� : g_sta - ��������״̬����ײ + ǰײ����
 *			  sys->gSta ,��ײ��״̬����������MASK_BUM_LEFT ��MASK_BUM_RIGHT ���������
 *			  sys->g_sta[] ��·ǰײ�����ADC��ֵ�������ж���ײ��������0 ��� - > 6 �ұ�
 * ˵    �� :
 *				1.�����ײ����ǰײ����������ֻ��ֵsys->gSta��sys->g_sta
 *				2.���������������ڹ���״̬������ͣ������Զ��������ص�sys->sState = SYS_IDLE ����״̬
 *				3.����磬�ⲿ�����磬��������ģʽ
 *				4.���Եغ��⣬�Ƿ񸡿գ�������ֹͣ���������sys->sState = SYS_IDLE ����״̬��
 *				  �����ǰ�����գ���û���գ��������£���ֹͣ������Կ���״̬�˳���������ᵹ�ˡ�
 *				5.500us���ڣ�����0,,500���ⷵ��1
 *
 **************************************************************/
#if PIXYMON_MODULE
int16_t px_deg;
#endif
char get_sensers(uint16_t *g_sta)
{
//	uint16_t gIDR=0;
	uint32_t tt=msTmr;
	int i,k1,k2,j;
	static int t_volt=21000;		//ȡ��ѹ��ʱ��
//	static int t_area = 0;
	static int idx_btn=0;
	static uint8_t first_input = 1;

#if BOTTOM_IR_EN
	static volatile int c_drop=0,c_drop_ok=0,c_drop_down = 0,c_door=0;//,c_front;

#endif
	static uint16_t errcode;

//	static uint16_t bat_twinkle_c = 0;		//ʹ�Կذ�����˸���ۼƼĴ���
	static volatile uint8_t bat_no_Q = 0;			//�޵������

//	static uint16_t charge_power_c = 0,low_power_c = 0;

	static volatile uint32_t wifistate_check_tmr=0;
	sys->bum_sta = 0;
	sys->fall_sta = 0;		//�����״̬
	errcode = sys->work_errcode;
	//=======�����ֵ������ɨ�������ȵ���Ĺ����澯,�澯��ֹͣ�������ʾ�������ص�����״̬======================
	//if(sys->navi_run ==1)

	check_sensens();
	if(sys->power_on == 0)	//�ر�״̬����⴫����
		return 1;
	if(errcode != sys->work_errcode)
	{
		errcode = sys->work_errcode;
		log_printf("errcode1:%04x\r\n",sys->work_errcode);
	}
	if(navigat->whele_stop == 1)		//��
	{
		log_printf("wst\r\n");
		*g_sta |= MASK_BUM_MIDL;
		navigat->whele_stop = 0;
		return 1;
	}


	*g_sta =0;
#if (1 == SIMPLIFY)
	float test_t = sys->t_navi_work;
	test_t *= test_t;
	if(((SYS_NAVIGAT == sys->sState) || (SYS_FOCUS == sys->sState) || (SYS_RANDOM == sys->sState)|| (SYS_NEAR == sys->sState))
		&& (SP_T_MIN < (test_t / SP_TIMEOUT)))
	{
		sys->sState = SYS_IDLE;
		init_sys_sta(SYS_IDLE);
		STOP_ALL_MOTOR();
		return 1;
	}
#endif

	//======================================��ײ���,ֻ��ֵsys->gSta==================================================
		/**/
	#define MAX_C_BUM		20

		if(( (GPIOD->IDR) & GPIO_Pin_0)==0 )
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOD->IDR) & GPIO_Pin_0)
					break;
			}
			if(i>=MAX_C_BUM)
			{
				*g_sta |=MASK_BUM_LEFT2;

			}
		}

		if( ((GPIOE->IDR) & MASK_BUM_RIGHT) ==0 )
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOE->IDR) & MASK_BUM_RIGHT)
					break;
			}
			if(i>=MAX_C_BUM)
			{
				*g_sta |=MASK_BUM_RIGHT;
			}
		}

		if(((GPIOE->IDR) & MASK_BUM_LEFT2) ==0)


		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOE->IDR) & MASK_BUM_LEFT2)
					break;
			}
			if(i>=MAX_C_BUM)
			{
				*g_sta |=MASK_BUM_LEFT;
			}
		}

		if(((GPIOE->IDR) & MASK_BUM_RIGHT2) == 0)
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOE->IDR) & MASK_BUM_RIGHT2)
					break;
			}
			if(i>=MAX_C_BUM)
			{
				*g_sta |=MASK_BUM_RIGHT2;
			}
		}
		static uint32_t t_bumber_err=0;
		static uint16_t c_bumber=0;
			if(*g_sta > 0)
			{
			c_bumber = 0;
			if(t_bumber_err ==0)
				t_bumber_err = tt;
			else if((tt - t_bumber_err) > cfg->r[IC_BUBER_ERR_T])
			{
				if(sys->sState != SYS_IDLE && sys->sState != SYS_CHARGE)
				{
					log_printf("IC_BUBER_ERR_T ERRr\n");
					motor_run(GO_STOP,0,0,0);
					sys->sState = SYS_IDLE;
					STOP_ALL_MOTOR();
					sys->t_beep_on = 10000; 	//��10����
					sys->work_errcode |= MWERR_BUMPER_FUNCTION_ERR;//add by wonton2004 20170118
				}
			}
		}else
		{
			if(t_bumber_err > 0)
			{
				if(c_bumber++ >=10)	//��ײ��ʧ��������
				{
					t_bumber_err = 0;
				}
			}
		}

		if(sys->sState == SYS_CHARGE || sys->sState == SYS_IDLE)
			t_bumber_err = 0;


	if((sys->slipping_state & MASK_BUM_MIDL) && *g_sta ==0)
	{
		*g_sta =sys->slipping_state;
	}
	sys->slipping_state = 0;


//----------------------------------------------------------------------add by wonton2004 20170115
//����ǽ,�͵�ƽΪ����ǽ,����ǽ�ж�Ϊһ����ײ
#if 1
	if(READ_VWALL_DET())
	{
		for(i=0;i<20;i++)
		{
			if(!READ_VWALL_DET())
				break;
		}
		if(i>=10)
		{
			//if(!(*g_sta & MASK_BUM_MIDL))
			//	log_printf("vwall!!!\r\n");
			*g_sta |=MASK_BUM_MIDL;
			sys->bum_sta |= BUM_V_WALL;
		}
	}
#endif
	//else
		//log_printf("vwall__1\r\n");

//--------------------------------------------------------------------------------------------end

	// 500ms ִ��һ��
	if(TIM6->CNT < 500)
		return 0;


	TIM6->CNT = 0;
#if 0 //�������жϵ�̺�߼�
	static unsigned char blankets_cnt = 0;
	static unsigned char no_blankets_cnt = 0;
	if((SYS_NAVIGAT ==  sys->sState) || (SYS_FOCUS ==  sys->sState) || (SYS_DOCK ==  sys->sState))
	{
		if(0 != GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
		{
			no_blankets_cnt = 0;
			if(10 >= blankets_cnt)
	{
				if(10 == blankets_cnt)
				{
					sys->Blankets_ground = SET;
					//sys->dust_pwm_value = FORCE_DUST_PWM;
				}
				blankets_cnt++;
			}
		}
		else
		{
			blankets_cnt = 0;
			if(10 >= no_blankets_cnt)
			{
				if(10 == no_blankets_cnt)
				{
					sys->Blankets_ground = RESET;
					//sys->dust_pwm_value = NORM_DUST_PWM;
				}
				no_blankets_cnt++;
			}
		}
	}
	else
	{
		blankets_cnt = 0;
		no_blankets_cnt = 0;
	}

#endif

	if(sys->wifi_old_state != mcu_get_wifi_work_state())
	{
		log_printf("W_O_state:%d,W_N_state:%d\r\n",sys->wifi_old_state,mcu_get_wifi_work_state());
		sys->wifi_old_state = mcu_get_wifi_work_state();
		wifi_sync_state_pro(sys->wifi_old_state);
	}

#if FRONT_IR_VERSION_1
if(sys->sState == SYS_DOCK)
	{
		//GPIOC->ODR &= ~(GPIO_Pin_10 | GPIO_Pin_11);
		FIRPWR_MID_OFF();
		FIRPWR_L1R1_OFF();
	}
	else
	{
		//GPIOC->ODR |= (GPIO_Pin_10 | GPIO_Pin_11);
		FIRPWR_MID_ON();
		FIRPWR_L1R1_ON();
	}
#else
if(sys->sState == SYS_DOCK)
	{
		//GPIOC->ODR |= GPIO_Pin_11;
		FIRPWR_MID_OFF();
	}
	else
	{
		//GPIOC->ODR &= ~GPIO_Pin_11;
		FIRPWR_MID_ON();
	}
#endif

	if(errcode != sys->work_errcode)
	{
		errcode = sys->work_errcode;
		log_printf("errcode2:%04x\r\n",sys->work_errcode);
	}

#if WIFIAPP_EN
	if(!sys->sync_wifi)
	{
		if(mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)//wifi���Ӻ�,����һ��ʱ��ͬ��
		{
			mstimeout(&sys->sync_loop,0);
			//synctime_1min = 0;
			log_printf("chktim by first con\r\n");
			if(sys->sync_wifi != 2)
			{
				mcu_get_system_time();
				sys->sync_wifi = 2;//����Ѿ���WIFI��ʱ��ͬ����������λ��
			}
		}
	}
	else if(sys->sync_wifi == 1)
	{
		if(mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)//wifi���Ӻ�,����һ��ʱ��ͬ��
		{
			sys->sync_wifi = 1;
			if(mstimeout(&sys->sync_loop,86400000)==1)
			{
				//mstimeout(&sys->sync_loop,0);
				//log_printf("[synctime_1min] %d\t%d\t%d\r\n",synctime_1min,sys->sync_loop,msTmr);
				//if(synctime_1min ++ > 1440)
				{
					//log_printf("chktim by timeout %d\r\n",synctime_1min);
					//synctime_1min = 0;
					log_printf("chktim by timeout\r\n");
					mcu_get_system_time();
				}
			}
		}
		//else
		//	sys->sync_flg = 0;
	}
#endif
    // static int get_c=0;
     //static int get_d=0;
	//ȡ��ѹ 10����һ��,����������ٷֱȲ��ϱ�
	if(t_volt++ > 10000 || sys->dis_statechange == 1)	//ȥһ�ε�ѹ
	{
		t_volt = 0;
		sys->dis_statechange = 0;
		sys->volt = LiBat_GetBatVolt();
//		log_printf("[bat volt adc]:%d\r\n",V_CHARG_ADC());

		i = get_libat_percent(sys->volt);
		//log_printf("1:pri:%d,volt:%d\r\n",i,sys->volt);
		log_printf("[bat volt adc]:%d,v=%d,pst=%d%\r\n",V_CHARG_ADC(),sys->volt,i);
		//mcu_dp_value_update(DPID_BATTERY, (unsigned long)(TIM6->CNT%100));

/*
		 //if(sys->sState==SYS_CHARGE)   //20200529 Ӧ�ȴ��ʦҪ�����ֻ�ڳ��ģʽ�·���
		{
		     get_c++;
		     if(get_d==0)
		     {
		     mcu_dp_value_update(DPID_BATTERY, (unsigned long)i);
			 log_printf("DPID_BATTERY0=%d\r\n",get_d);
               get_d=1;
		     }
			 if(get_c ++>12)
			{
				get_c=0;
			}
			if(get_c == 1)
			{
				mcu_dp_value_update(DPID_BATTERY, (unsigned long)i);
				log_printf("DPID_BATTERY1=%d\r\n",get_c);
			}
		}*/
		//ÿ10����һ��WIFI��״̬�������δ֪(0xff)������һ��ͬ������
		if(mcu_get_wifi_work_state() == 0xff)
		{
			mcu_sync_wifi_net_state();
		}

		if(first_input==1)
		{
			first_input = 0;
			sys->prs = i;
		}
		if(sys->sState!=SYS_CHARGE)
		{//�ǳ��״̬
			if(i>sys->prs)//ȡ���ĵ�����֮ǰֵ��,ʹ��֮ǰֵ
			{
				//log_printf("get big Q:%d,%d\r\n",i,sys->prs);
				i = sys->prs;
			}
		}
		else//���״̬
		{
			if(i<sys->prs)//ȡ���ĵ�����֮ǰֵС,ʹ��֮ǰֵ
			{
				//log_printf("charge get small Q:%d,%d\r\n",i,sys->prs);
				i = sys->prs;
			}
		}
		sys->prs = i;
		log_printf("volot=%d,prs=%d,udp_prs=%d,cur:%d,sta=%d,cnt=%d\r\n",sys->volt,sys->prs,i,sys->sys_charge_cur,sys->sState,TIM5->CNT );

		//if(sys->volt < CHARGE_POWER &&
		if(sys->prs < LOW_POW_PRS_TWS &&
			(/**/sys->sState == SYS_NAVIGAT || sys->sState == SYS_FOCUS || sys->sState == SYS_RANDOM))
		{
			//if(charge_power_c++>4)
			{
				if(sys->sState == SYS_RANDOM)		//���ģʽ����ֱ��ͣ��������
				{
					log_printf("no power stop work\r\n");
					ny3p_play(VOICE_M_FINISHED);
					sys->sState = SYS_IDLE;
					motor_run(GO_STOP,0,0,0);
					STOP_ALL_MOTOR();
				}
				else if(sys->dock_near != NEAR_DOCK_STA )
				{
					log_printf("low power charge back...\r\n");
					sys->sState = SYS_DOCK;		//�س䡣
					all_key_led_off();
					LED4_ON();
				}
			}
		}
		//else
			//charge_power_c = 0;

		//if(sys->volt < LOW_POWER/* || bat_no_Q == 1*/)
		if(sys->prs<3)
		{
			//if(sys->c_low_power ++ >= 20)
			//if(low_power_c++>5)
			{
				if(sys->sState != SYS_CHARGE)
				{
					log_printf("no power stop work\r\n");
					ny3p_play(VOICE_CHARGE_M);
					sys->sState = SYS_IDLE;
					motor_run(GO_STOP,0,0,0);
					all_key_led_off();
					STOP_ALL_MOTOR();
					delay_ms(500);
				}
			}
		}
		//else
			//low_power_c= 0;

	}

	if(errcode != sys->work_errcode)
	{
		errcode = sys->work_errcode;
		log_printf("errcode3:%04x\r\n",sys->work_errcode);
	}

	i=0,j=0;
#define DROP_IR_DEBUG		0
	/****************************************************************************
	ǰײ����,��ֵsys->g_sta[]��·�����ADCֵ�������Ե�Զ�����⣬��ֵsys->g_buton ���ڴ洢�Եؽ���Զ����
	*******************************************************************************/
	//���ͣ��Ͳ��������
#if DROP_IR_DEBUG
	log_printf("%d",sys->c_ir_adc);
#endif
	for(i=0;i<MAC_IRD_COUNT;i++)
	{
#if DP_PANEL_EN >= 4//V1.8 ��2����2��IO��ʽ���,ʵ�ⲻͬ�����޷��ı�IO�о���ƽ,����������·������
		if(i == 1)//��2
		{
			if(GPIOE->IDR & GPIO_Pin_9)
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
			else
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
		}
		else if(i == 5)//��2
		{
			if(GPIOE->IDR & GPIO_Pin_15)
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
			else
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
		}
		else
#endif
		sys->m_sta[i][sys->c_ir_adc] = adc_converted_value[i];
#if DROP_IR_DEBUG
		if(i == 7)
			log_printf("(%d",sys->m_sta[i][sys->c_ir_adc] );
		else
			log_printf(",%d",sys->m_sta[i][sys->c_ir_adc] );
#endif
	}
#if DROP_IR_DEBUG
	log_printf(")\r\n");
#endif
	sys->c_ir_adc++;
	if(sys->c_ir_adc >=8)
	{
		/*log_printf("\r\n");
		for(i= 0;i < 9;i ++)
		{
			log_printf("%d\r\n",sys->m_sta[0][i]);
		}*/
		for(i=0;i<MAC_IRD_COUNT;i++)
		{
			k1 = k2 = 0;
			for(j=0;j<4;j++)
				k1 +=sys->m_sta[i][j];
			for(j=4;j<8;j++)
				k2 +=sys->m_sta[i][j];

			if(i >=7)				//�Եغ���
			{/*
				k1 = k2 = 0;
				k1 = (sys->m_sta[i][0]+sys->m_sta[i][1]) >> 1;		//ȡ������
				k2 = (sys->m_sta[i][2] +sys->m_sta[i][3]+sys->m_sta[i][4]) / 3;
				log_printf("k1=%d,k2=%d.",k1,k2);
				if(k1 > k2 && ((k1 - k2) /4) < 5000)
					sys->g_buton[BOTOM_IDX][i-7]  =  k1 - k2;
				else
					sys->g_buton[BOTOM_IDX][i-7]  = 3000;		//������
				if(k1 < 1800 && dis_xy(k1,k2) < 300)
				{
					sys->g_buton[BOTOM_IDX][i-7] = 2000;
				}
				k2 = (sys->m_sta[i][5] +sys->m_sta[i][6]+sys->m_sta[i][7]) / 3;
				log_printf("k3=%d\r\n",k2);
				if(k1 > k2 && ((k1 - k2) /4) < 5000)
					sys->g_buton[DOOR_IDX][i-7]  =  k1 - k2;
				else
					sys->g_buton[DOOR_IDX][i-7]  = 3000;		//������
				*/
				if(8 == i)
				{
					sys->g_buton[BOTOM_IDX][i-7] = 0;
					sys->g_buton[DOOR_IDX][i-7] = 0;
				}
				else
				{
					sys->g_buton[BOTOM_IDX][i-7] = k2 >>2; //sys->g_sta[i];
					sys->g_buton[DOOR_IDX][i-7] = 0;
				}
			}else							//��ײ����
			{

				//if(i== 0 /*|| i == 3 */|| i==6)
				//{
				//	sys->g_sta[i] = k1 / 4;
				//	if(sys->g_sta[i] > 4000)
				//		sys->g_sta[i] = 0;
				//}else

				{

					if(k1 > k2 && ((k1 - k2) /4) < 5000)
						sys->g_sta[i] = (k1 - k2) / 4;
					else
						sys->g_sta[i] = 0;
				}

			}

		//log_printf("%d,%d,%d,%d\r\n",i,k1 / 4,k2 /4,(k1-k2 ) / 4);
		}


		idx_btn = (++idx_btn) & 1;
		sys->c_ir_adc = 0;
	}


	//testmicony20190912-�����⣬��ʱȥ��
	//sys->g_sta[2] = sys->g_sta[4] = 0;
#if (0)
	sys->g_sta[0] = 10;
	sys->g_sta[2] = 10;
	sys->g_sta[3] = 10;
	sys->g_sta[4] = 10;
	sys->g_sta[6] = 10;
#endif
	comm_ultrasonic(&sys->ground_ul);
	if(sys->sState == SYS_DOCK)
		sys->g_sta[3] = 0;

	//�س䲻������
	//�س��͵�ѹ��ʱ��
	if(sys->sState == SYS_DOCK )

	{
		//GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);		//�͵�ƽ�ص�
		//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); 	//�͵�ƽ�ص�

		//sys->g_sta[0] = 0;
		//sys->g_sta[3] = 0;
		//sys->g_sta[6] = 0;
		GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
		/*
		if(sys->c_ir_adc <=3 )
		{
			GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
			//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); //�͵�ƽ���ص�
			//�Ե�
			NEAR_LAN_OFF();
			FAR_LAN_ON();
		}
		else
		{
			//GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
			//GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
			//�ż�
			NEAR_LAN_ON();
			FAR_LAN_OFF();
		}
		*/
	}
	else
	{
		if (sys->sState == SYS_IDLE &&  ( /*sys->volt < LOW_POWER ||*/ sys->t_idle >= T_IDLE_IR) ) 		//�����ǶԵģ�
	//	if(sys->sState == SYS_IDLE)
		{
				GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);		//�͵�ƽ�ص�
				GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
				//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);		//�͵�ƽ�ص�
				FAR_LAN_OFF();
				NEAR_LAN_OFF();
		}
		else
		{






			if(/*sys->sState != SYS_POWEROFF && */sys->sState != SYS_CHARGE/* || sys->sState != SYS_DOCK*/)//�ػ������״̬�¹رպ���
			{
				if(sys->c_ir_adc <=3 )
				{
					GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
					//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);	//�͵�ƽ���ص�
					//�Ե�
					NEAR_LAN_OFF();
					FAR_LAN_ON();
				}
				else
				{

					GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
					GPIO_SetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
					//GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
					//�ż�
					NEAR_LAN_ON();
					FAR_LAN_OFF();
				}
			}
			/*
			//�Եصģ���������
			if(sys->c_ir_adc <=1 )		//�ص�
			{
				FAR_LAN_OFF();
				NEAR_LAN_OFF();
			}else if(sys->c_ir_adc <=4 )
			{
				NEAR_LAN_ON();
				FAR_LAN_OFF();
			}else
			{
				NEAR_LAN_OFF();
				FAR_LAN_ON();
			}



			if(sys->c_ir_adc <=3 )
			{
				GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);
				GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
				//�ż�
				NEAR_LAN_ON();
				FAR_LAN_OFF();
			}
			else
			{

				GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
				GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);

				NEAR_LAN_OFF();
				FAR_LAN_ON();
			}
			*/
		}
	}
//	return 1;

	if(errcode != sys->work_errcode)
	{
		errcode = sys->work_errcode;
		log_printf("errcode4:%04x\r\n",sys->work_errcode);
	}

	//micony20200112 ���ֳ�����Ͻ�
	if(sys->sState == SYS_CHARGE)
	{
		return 1;
	}


#if !BOTTOM_IR_EN
	return 1;
#else


		//if(sys->work_errcode != MWERR_DUSTBOX)	///����г��и澯,���ո澯����� 2018-05-22 jzz


	{
		//�ж��豸��� sys->g_buton[0] �Ե�Զ�����ֵ
		//if(sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX] > 2000 && sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] > 2000 && sys->g_buton[BOTOM_IDX][BOTOM_FRONT_IDX] > 2000)
	//if(LEFT_MOTOR_LEAVE()	==0 && RIGHT_MOTOR_LEAVE()== 0)
#if 0
			if(sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX] < LEFT_IR_DROP && sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] < RIGHT_IR_DROP /*&& sys->g_buton[BOTOM_IDX][BOTOM_FRONT_IDX] > FRONT_IR_DROP*/ 	//WY190527 ȥ��ǰ�˵ĶԵغ���
			/*&& ( sys->t_idle < T_IDLE_IR )*/ )//modified by wonton2004 20170118
		{		/////E6һֱ��ʾ���ָ�����ʧ
		//
			//log_printf("drop:%d\r\n",c_drop);
			//if(c_drop++ >=240 /*&& c_drop_ok == 0*/)
			if(c_drop++ >=5000)	//wy190527 �ӳ��ж�
			{
				//if( (c_drop & 0x7F) == 0)
				//	log_printf("robot drop\r\n");
				log_printf("drop stop  333!!! \r\n");
				if(sys->sState != SYS_IDLE)
				{
					//log_printf("drop test 1...\r\n");
					STOP_ALL_MOTOR();
					ny3p_play(VOICE_CHECKLEAVE2);
					motor_run(GO_STOP,0,0,0);

					sys->sState = SYS_IDLE;
					 LED2_OFF();
				     LED3_OFF();
				     LED1_OFF();
					sys->err_sta |=ERR_STA_PICK_UP;
					sys->sensens_err = 1;		///2018-05-18 jzz �����������־����ɨ������״̬�������س����
					c_drop = 0;
					c_drop_ok = 1;

					//work_sta = WORK_STA_ERR | (sys->wifi << WORK_STA_WIFI) | (sys->silent << WORK_STA_SILENT);
					if(sys->work_errcode < MWERR_FLRSENSOR)
					{
						log_printf("get MWERR_FLRSENSOR \r\n");
						if(sys->work_errcode != MWERR_FLRSENSOR)
						{
							sys->work_errcode = MWERR_FLRSENSOR;
							sys->wifistatechange = 1;
						}
					}
				}
				else
				{
					//log_printf("drop test 2...\r\n");
					sys->err_sta |=ERR_STA_PICK_UP;
					c_drop = 0;
					c_drop_ok = 1;

					//work_sta = WORK_STA_ERR | (sys->wifi << WORK_STA_WIFI) | (sys->silent << WORK_STA_SILENT);
					//dp_tx_data(DP_SYS_STA, work_sta, CODE_ROBOT_LEAVE, 0,0);		//������

					if(sys->work_errcode < MWERR_FLRSENSOR)
					{
						if(sys->work_errcode != MWERR_FLRSENSOR)
						{
							log_printf("get MWERR_FLRSENSOR \r\n");
							sys->work_errcode = MWERR_FLRSENSOR;
							sys->wifistatechange = 1;
						}
					}
				}

				navigat->suspend =FALSE;
			}
		}
		else
		{
			if(c_drop_down ++ > 3 )
			{
				//log_printf("drop test 3...\r\n");
				c_drop = 0;
				c_drop_ok = 0;
				c_drop_down = 0;
				//if(sys->work_errcode < MWERR_FLRSENSOR)
				{
					if(sys->work_errcode == MWERR_FLRSENSOR )
					{
						log_printf("MWERR_FLRSENSOR clear\r\n");
						//work_sta = WORK_STA_ERR | (sys->wifi << WORK_STA_WIFI) | (sys->silent << WORK_STA_SILENT);
						sys->work_errcode = MWERR_NONE;//add by wonton2004 20170118
						//dp_tx_data(DP_SYS_STA, work_sta, MWERR_NONE, 0,0);
						sys->dis_statechange = 1;
						sys->wifistatechange = 1;
					}
				}

			}
		}
#endif
			//���䴫�����澯����,������С����û������
			if(sys->sState !=  SYS_CHARGE)
		 {
			if((sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX] < MIN_CLIFF_SENSOR_VALUE ||
			  sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] < MIN_CLIFF_SENSOR_VALUE) &&
			  LEFT_MOTOR_LEAVE() && RIGHT_MOTOR_LEAVE())
			{
				uint32_t t=msTmr;
				if(sys->t_cliff_sensor_err == 0)
				{
					sys->t_cliff_sensor_err =t;
					log_printf("found cliff err,cliff=%d,%d,leave=%d,%d,t=%d\r\n",sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX],
							sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] ,LEFT_MOTOR_LEAVE() ,RIGHT_MOTOR_LEAVE(),sys->t_cliff_sensor_err);

				}else if(t > sys->t_cliff_sensor_err && ((t-sys->t_cliff_sensor_err) / 1000) > T_CLIFF_SENSOR_ERR)
				{
					log_printf("cliff sensor err,t=%d,%d\r\n",t,sys->t_cliff_sensor_err);
					{
						sys->t_cliff_sensor_err = 0;
						sys->t_beep_on = 10000;		//��10����
						sys->work_errcode |= MWERR_FLRSENSOR;//add by wonton2004 20170118
					}

				}
			}else
				sys->t_cliff_sensor_err = 0;
		}

		if(sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX] < LEFT_IR_DROP)	//wy190527
		{
			sys->c_left_drop++;
			//log_printf("l(%d,%d,%d)\r\n",sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX],sys->g_buton[DOOR_IDX][BOTOM_LEFT_IEX] ,sys->c_left_drop);
			//
			if(sys->c_left_drop>=2)
			{

					if(sys->sState != SYS_IDLE)
					{
						if( sys->mState ==GO_FORWARD)
						{
							motor_run(GO_STOP,0,0,0);
							log_printf("drop but no idle2 ,stop motor\r\n");
						}
						*g_sta |=MASK_BUM_LEFT;
						if(sys->c_left_drop < 4)
							log_printf("ldrop1(%d,%d,%d)\r\n",sys->g_buton[BOTOM_IDX][BOTOM_LEFT_IEX],sys->g_buton[DOOR_IDX][BOTOM_LEFT_IEX] ,sys->c_left_drop);
					}
					sys->fall_sta |= LEFT_SIDE_FALL;

				}
		}
		else
			sys->c_left_drop = 0;
		//�ұߵ���
		//if(sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] > RIGHT_IR_DROP && sys->g_buton[DOOR_IDX][BOTOM_RIGHT_IDX] > RIGHT_IR_DROP)
		if(sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX] < RIGHT_IR_DROP)
		{
				sys->c_right_drop++;
				if(sys->c_right_drop >=2 )
				{
					if(sys->sState != SYS_IDLE)
					{
						*g_sta |=MASK_BUM_RIGHT;
						if(sys->sState != SYS_IDLE && sys->mState ==GO_FORWARD)
						{
							motor_run(GO_STOP,0,0,0);
							log_printf("drop but no idle1 ,stop motor\r\n");
						}
						if(sys->c_right_drop < 4)
					   		log_printf("rdrop1(%d,%d,%d)\r\n",sys->g_buton[BOTOM_IDX][BOTOM_RIGHT_IDX],sys->g_buton[DOOR_IDX][BOTOM_RIGHT_IDX],sys->c_right_drop );

					}
					sys->fall_sta |= MASK_BUM_RIGHT;

			}
		}
		else
			sys->c_right_drop = 0;



	}

#endif
	return 0;

}


char get_irda(uint16_t *g_sta)
{
//	uint16_t gIDR=0;
	int i,k1,k2,j;
//	static int t_volt=0;		//ȡ��ѹ��ʱ��
	static int idx_btn=0;



//	*g_sta =0;
	//======================================��ײ���,ֻ��ֵsys->gSta==================================================


#define MAX_C_BUM		20
#if BUM_STA_OLD

			if(( (GPIOD->IDR) & MASK_BUM_LEFT) )



			{
				for(i=0;i<MAX_C_BUM;i++)
				{
					if(((GPIOD->IDR) & MASK_BUM_LEFT)==0)
						break;
				}
				if(i>=MAX_C_BUM)
					*g_sta |=MASK_BUM_LEFT;
			}

			if( ((GPIOD->IDR) & MASK_BUM_RIGHT) )





			{
				for(i=0;i<MAX_C_BUM;i++)
				{
					if(((GPIOD->IDR) & MASK_BUM_RIGHT) ==0)
						break;
				}
				if(i>=MAX_C_BUM)
					*g_sta |=MASK_BUM_RIGHT;
			}
#else
			if(( (GPIOD->IDR) & MASK_BUM_LEFT)==0 )


			{
				for(i=0;i<MAX_C_BUM;i++)
				{
					if(((GPIOD->IDR) & MASK_BUM_LEFT))
						break;
				}
				if(i>=MAX_C_BUM)
				{
					*g_sta |=MASK_BUM_LEFT;
					//if(sys->mState == GO_FORWARD)
					//	motor_run(GO_STOP,0,0,0);
				}
			}

			if( ((GPIOD->IDR) & MASK_BUM_RIGHT) ==0 )
			{
				for(i=0;i<MAX_C_BUM;i++)
				{
					if(((GPIOD->IDR) & MASK_BUM_RIGHT))
						break;
				}
				if(i>=MAX_C_BUM)
				{
					*g_sta |=MASK_BUM_RIGHT;
					//if(sys->mState == GO_FORWARD)
					//	motor_run(GO_STOP,0,0,0);
				}
			}









#endif



	// 500ms ִ��һ��
	if(TIM6->CNT < 500)
		return 0;


	TIM6->CNT = 0;


#if FRONT_IR_VERSION_1
if(sys->sState == SYS_DOCK)
	{
		//GPIOC->ODR &= ~(GPIO_Pin_10 | GPIO_Pin_11);
		FIRPWR_MID_OFF();
		FIRPWR_L1R1_OFF();
	}
	else
	{
		//GPIOC->ODR |= (GPIO_Pin_10 | GPIO_Pin_11);
		FIRPWR_MID_ON();
		FIRPWR_L1R1_ON();
	}
#else
if(sys->sState == SYS_DOCK)
	{
		//GPIOC->ODR |= GPIO_Pin_11;
		FIRPWR_MID_OFF();
	}
	else
	{
		//GPIOC->ODR &= ~GPIO_Pin_11;
		FIRPWR_MID_ON();
	}
#endif

	i=0,j=0;

	/****************************************************************************
	ǰײ����,��ֵsys->g_sta[]��·�����ADCֵ�������Ե�Զ�����⣬��ֵsys->g_buton ���ڴ洢�Եؽ���Զ����
	*******************************************************************************/
	//���ͣ��Ͳ��������
	//log_printf("%d",sys->c_ir_adc);
	for(i=0;i<MAC_IRD_COUNT;i++)
	{
		//sys->m_sta[i][sys->c_ir_adc] = adc_converted_value[i];
		//log_printf(",%d",sys->m_sta[i][sys->c_ir_adc] );
#if DP_PANEL_EN >= 4//V1.8 ��2����2��IO��ʽ���,ʵ�ⲻͬ�����޷��ı�IO�о���ƽ,����������·������
		if(i == 1)//��2
		{
			if(GPIOE->IDR & GPIO_Pin_9)
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
			else
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
		}
		else if(i == 5)//��2
		{
			if(GPIOE->IDR & GPIO_Pin_15)
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
			else
			{
				sys->m_sta[i][sys->c_ir_adc] = 10;
			}
		}
		else
#endif
		sys->m_sta[i][sys->c_ir_adc] = adc_converted_value[i];
	}
	//log_printf("\r\n");
	sys->c_ir_adc++;
	if(sys->c_ir_adc >=8)
	{
		for(i=0;i<MAC_IRD_COUNT;i++)
		{
			k1 = k2 = 0;
			for(j=0;j<4;j++)
				k1 +=sys->m_sta[i][j];
			for(j=4;j<8;j++)
				k2 +=sys->m_sta[i][j];


			if(i >=7)
			{
				if(8 == i)
				{
					sys->g_buton[0][i-7] = 0;
					sys->g_buton[1][i-7] = 0;
				}
				else
				{
					sys->g_buton[0][i-7] = k2 / 4;
					sys->g_buton[1][i-7] = 0;
				}

			}else
			{

				////if(i== 0 /*|| i == 3 */|| i==6)
				////{
				////	sys->g_sta[i] = k1 / 4;
				////	if(sys->g_sta[i] > 4000)
				////		sys->g_sta[i] = 0;
				////}else

				{

					if(k1 > k2 && ((k1 - k2) /4) < 5000)
						sys->g_sta[i] = (k1 - k2) / 4;
					else
						sys->g_sta[i] = 0;
				}


			}
		//log_printf("%d,%d,%d,%d\r\n",i,k1 / 4,k2 /4,(k1-k2 ) / 4);
		}


		idx_btn = (++idx_btn) & 1;
		sys->c_ir_adc = 0;
	}
#if (0)
	sys->g_sta[0] = 10;
	sys->g_sta[2] = 10;
	sys->g_sta[3] = 10;
	sys->g_sta[4] = 10;
	sys->g_sta[6] = 10;
#endif
	comm_ultrasonic(&sys->ground_ul);
	if(sys->sState == SYS_DOCK)
		sys->g_sta[3] = 0;
	//�س䲻������
	//�س��͵�ѹ��ʱ��
	if(sys->sState == SYS_DOCK )
	//if(sys->sState == SYS_DOCK )

	{
		//GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);		//�͵�ƽ�ص�
		//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); 	//�͵�ƽ�ص�

		//sys->g_sta[0] = 0;
		//sys->g_sta[3] = 0;
		//sys->g_sta[6] = 0;

		if(sys->c_ir_adc <=3 )
		{
			GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
			//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); //�͵�ƽ���ص�
			//�Ե�
			NEAR_LAN_OFF();
			FAR_LAN_ON();
		}
		else
		{
			//GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
			//GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
			//�ż�
			NEAR_LAN_ON();
			FAR_LAN_OFF();
		}
	}

	else
	{
		if (sys->sState == SYS_IDLE &&  ( /*sys->volt < LOW_POWER ||*/ sys->t_idle >= T_IDLE_IR) ) 		//�����ǶԵģ�
	//	if(sys->sState == SYS_IDLE)
		{
				GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);		//�͵�ƽ�ص�
				GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
				//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);		//�͵�ƽ�ص�
				FAR_LAN_OFF();
				NEAR_LAN_OFF();
		}
		else
		{
			if(/*sys->sState != SYS_POWEROFF && */sys->sState != SYS_CHARGE/* || sys->sState != SYS_DOCK*/)//�ػ������״̬�¹رպ���
			{
				if(sys->c_ir_adc <=3 )
				{
					GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
					//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);	//�͵�ƽ���ص�
					//�Ե�
					NEAR_LAN_OFF();
					FAR_LAN_ON();
				}
				else
				{

					GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
					GPIO_SetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
					//GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
					//�ż�
					NEAR_LAN_ON();
					FAR_LAN_OFF();
				}
			}
			/*
			//�Եصģ���������
			if(sys->c_ir_adc <=1 )		//�ص�
			{
				FAR_LAN_OFF();
				NEAR_LAN_OFF();
			}else if(sys->c_ir_adc <=4 )
			{
				NEAR_LAN_ON();
				FAR_LAN_OFF();
			}else
			{
				NEAR_LAN_OFF();
				FAR_LAN_ON();
			}



			if(sys->c_ir_adc <=3 )
			{
				GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);
				GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
				//�ż�
				NEAR_LAN_ON();
				FAR_LAN_OFF();
			}
			else
			{

				GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
				GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);

				NEAR_LAN_OFF();
				FAR_LAN_ON();
			}
			*/
		}
	}
	return 1;
}

int get_distanc(uint16_t adc)
{
	return (13524000 / (adc * 33 - 1800) - 40);
}
/*
#define MAX_FW_RD	60
uint16_t fw_rd_fifo[MAX_FW_RD];
int fw_rd_len = 0;

void insert_fw_rd(uint16_t value)
{
	fw_rd_fifo[fw_rd_len++] = value;
	if(fw_rd_len >=MAX_FW_RD)
		fw_rd_len = 0;
}
void reset_fw_fifo(void)
{
	int i;
	fw_rd_len = 0;
	for(i=0;i<MAX_FW_RD;i++)
		fw_rd_fifo[i] = 0x00;
}
uint16_t max_fw_rd(int max_v)
{
	int i;
	uint16_t value=0;
	for(i=0;i<MAX_FW_RD ;i++)
	{
		if(fw_rd_fifo[i]  > max_v)
			value++;
		//value = (fw_rd_fifo[i] > value)?fw_rd_fifo[i] :value;
	}
	return value;
}
*/

//u8 IrCheckFlag = 0;

u8 IrRevData[4]={
	0,0,0,0
};

static u16 IrData[6]={
	0,0,0,0,
};

static u8 IrCnt[4] ={
	0,0,0,0,
};
u16 g_ir_data[4] = {0,0,0,0};


uint32_t irda_tmr = 0;

void ir_recv_ext_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;




	EXTI_StructInit(&EXTI_InitStructure);
	//EXTI_InitTypeDef EXTI_InitStructure;

	//���========================
	GPIO_InitStructure.GPIO_Pin = IR_L_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IR_L_GPIO_PORT, &GPIO_InitStructure);
	GPIO_EXTILineConfig(IR_L_SOURCE, IR_L_PIN);

	NVIC_InitStructure.NVIC_IRQChannel = IR_L_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(IR_L_LINE);


	//�ұ�========================
	GPIO_InitStructure.GPIO_Pin = IR_R_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IR_R_GPIO_PORT, &GPIO_InitStructure);
	GPIO_EXTILineConfig(IR_R_SOURCE, IR_R_PIN);
	NVIC_InitStructure.NVIC_IRQChannel = IR_R_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(IR_R_LINE);


	//�м�
	GPIO_InitStructure.GPIO_Pin = IR_M_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IR_M_GPIO_PORT, &GPIO_InitStructure);
	GPIO_EXTILineConfig(IR_M_SOURCE, IR_M_PIN);

	NVIC_InitStructure.NVIC_IRQChannel = IR_M_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(IR_M_LINE);

	//�м��ұ�
	GPIO_InitStructure.GPIO_Pin = IR_M2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING;//GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IR_M2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_EXTILineConfig(IR_M2_SOURCE, IR_M2_PIN);

	NVIC_InitStructure.NVIC_IRQChannel = IR_M2_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(IR_M2_LINE);




	/* Configure EXTI Line0 to generate an interrupt on falling edge */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;    //�趨�½��ش���ģʽ

	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

	EXTI_InitStructure.EXTI_Line |= IR_L_LINE;
	EXTI_ClearITPendingBit(IR_L_LINE);

	EXTI_InitStructure.EXTI_Line |= IR_R_LINE;
	EXTI_ClearITPendingBit(IR_R_LINE);

	EXTI_InitStructure.EXTI_Line |= IR_M_LINE;
	EXTI_ClearITPendingBit(IR_M_LINE);

	EXTI_InitStructure.EXTI_Line |= IR_M2_LINE;
	EXTI_ClearITPendingBit(IR_M2_LINE);


	EXTI_Init(&EXTI_InitStructure);
}

static u32 Timesave[4]={
	0,0,0,0,
};

static u32 ir_time_base[4]={
	0,0,0,0,
};


uint8_t ir_parse_printen = 0;
#if 1

static uint8_t ir_parse_get_anydata = 0;
uint8_t seget_ir_parse_datain(void)
{
	uint8_t ret;

	ret = ir_parse_get_anydata;
	ir_parse_get_anydata = 0;
	return ret;
}

//�س������ĸ�4λ
#define IR_PARSE_HEAD	0xe0

#define IR_PARSE_TOPLED	(IR_PARSE_HEAD | 0x10)

#define IR_BIT_PERIOD	39 // ���ⷢ��,����λ��ռ��ʱ��(��λ: 100us)

//static uint8_t ir_parse_rxover = 0;//������ɵı�־λ,��0λΪ���,��1λΪ�м�,��2λΪ�ұ�

#if 1

//������յ���ԭʼ����
static uint16_t ir_original_data[6]={
	0,0,0,0,0,0
};

//static uint8_t ir_dock_signal = 0;//������Ķ����ź�

//���س�������Ƶ��ź�ֵ(��/��/��λ�ŷֱ�Ϊbit0/bit1/bit2)
//����: clr Ϊ1ʱ,ir_dock_signal������0
uint8_t ir_dock_insight(uint8_t clr)
{
	//uint8_t ret = ir_dock_signal;

	//if(clr)ir_dock_signal = 0;
	return read_ir_dock_knk();

	//return ret;
}

static uint8_t ir_alter_off = 0;//������������ʹ��,Ĭ��Ϊ�ر�,��������״̬��,��������Ҫ�Գ����������
								//���н�����,����߻س��׼�ľ���,������תѰ���Ĺ�����,��Ҫ��ֹ�����
								//��Ϊ���������ʹ�����Գ�����ķ����������
//0 ��ֵ 1 ��ʼ��
void ir_original_bkbuff_test(uint8_t flag)
{
/*
	if(flag == 1)
	{
		debug_ir_original_bk[IR_L_PIN_NUM] = 0;
		debug_ir_original_bk[IR_M_PIN_NUM] = 0;
		debug_ir_original_bk[IR_R_PIN_NUM] = 0;
		return;
	}
	debug_ir_original_bk[IR_L_PIN_NUM] = ir_original_data[IR_L_PIN_NUM];
	debug_ir_original_bk[IR_M_PIN_NUM] = ir_original_data[IR_M_PIN_NUM];
	debug_ir_original_bk[IR_R_PIN_NUM] = ir_original_data[IR_R_PIN_NUM];
*/

}
//����ԭʼ������յ�����
void ir_parse_original_data(void)
{
	/////debug_ir_buff	2018-07-16

//	ir_original_bkbuff_test(0);		//0 ��ֵ 1 ��ʼ��
	debug_ir_original_bk[IR_L_PIN_NUM] = ir_original_data[IR_L_PIN_NUM];
	debug_ir_original_bk[IR_ML_PIN_NUM] = ir_original_data[IR_ML_PIN_NUM];
#if (0 == SINGLE_LED_DOCK)
	debug_ir_original_bk[IR_MR_PIN_NUM] = ir_original_data[IR_MR_PIN_NUM];
#endif
	debug_ir_original_bk[IR_R_PIN_NUM] = ir_original_data[IR_R_PIN_NUM];
	if(ir_alter_off != 0)//�������������ܹر�ʱ,����ֵ���н���,����ԭ��Ϊ,�����������ߵ�ֵ����Ϊ������ҵ�ֵ
	{
		switch(ir_original_data[IR_ML_PIN_NUM]&0xfe)
		{
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
			//case IR_PARSE_HEAD|0x0c:
				IrRevData[IR_ML_PIN_NUM] = IR_MID;
				break;
			case IR_PARSE_HEAD|0x04:
			case IR_PARSE_HEAD|0x06://0x06
				IrRevData[IR_ML_PIN_NUM] = IR_RIGHT;
				break;
			case IR_PARSE_HEAD|0x08:
			case IR_PARSE_HEAD|0x0a://0x0a
				IrRevData[IR_ML_PIN_NUM] = IR_LEFT;
				break;
			default:
				IrRevData[IR_ML_PIN_NUM] = 0;
				break;
		}
#if (0 == SINGLE_LED_DOCK)
		switch(ir_original_data[IR_MR_PIN_NUM]&0xfe)
		{
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
			//case IR_PARSE_HEAD|0x0c:
				IrRevData[IR_MR_PIN_NUM] = IR_MID;
				break;
			case IR_PARSE_HEAD|0x04:
			case IR_PARSE_HEAD|0x06://0x06
				IrRevData[IR_MR_PIN_NUM] = IR_RIGHT;
				break;
			case IR_PARSE_HEAD|0x08:
			case IR_PARSE_HEAD|0x0a://0x0a
				IrRevData[IR_MR_PIN_NUM] = IR_LEFT;
				break;
			default:
				IrRevData[IR_MR_PIN_NUM] = 0;
				break;
		}
#endif
		switch(ir_original_data[IR_L_PIN_NUM]&0xfe)
		{
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
			//case IR_PARSE_HEAD|0x0c:
				IrRevData[IR_L_PIN_NUM] = IR_MID;
				break;
			case IR_PARSE_HEAD|0x04:
			case IR_PARSE_HEAD|0x06://0x06
				IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
				break;
			case IR_PARSE_HEAD|0x08:
			case IR_PARSE_HEAD|0x0a://0x0a
				IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
			default:
				IrRevData[IR_L_PIN_NUM] = 0;
				break;
		}
		switch(ir_original_data[IR_R_PIN_NUM]&0xfe)
		{
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
			//case IR_PARSE_HEAD|0x0c:
				IrRevData[IR_R_PIN_NUM] = IR_MID;
				break;
			case IR_PARSE_HEAD|0x04:
			case IR_PARSE_HEAD|0x06://0x06
				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case IR_PARSE_HEAD|0x08:
			case IR_PARSE_HEAD|0x0a://0x0a
				IrRevData[IR_R_PIN_NUM] = IR_LEFT;
				break;
			default:
				IrRevData[IR_R_PIN_NUM] = 0;
				break;
		}
		goto __ir_parse_original_data_end;
	}
	switch(ir_original_data[IR_ML_PIN_NUM]&0xfe)
	{
		case IR_PARSE_HEAD|0x02:
		case IR_PARSE_HEAD|0x0e:
		//case IR_PARSE_HEAD|0x0c:
			IrRevData[IR_ML_PIN_NUM] = IR_MID;
			break;
		case IR_PARSE_HEAD|0x04:
			IrRevData[IR_ML_PIN_NUM] = IR_RIGHT;
			break;
		case IR_PARSE_HEAD|0x08:
			IrRevData[IR_ML_PIN_NUM] = IR_LEFT;
			break;
		case IR_PARSE_HEAD|0x0a://0x0a
			//if(!(ir_original_data[IR_R_PIN_NUM]&0xfe))//����ұ�û���յ�,����032,����ұ��յ�,��ô�м佫����0
				IrRevData[IR_ML_PIN_NUM] = IR_MID;
			IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
			goto __ir_parse_original_data_end;
			//break;
		case IR_PARSE_HEAD|0x06://0x06
			//if(!(ir_original_data[IR_L_PIN_NUM]&0xfe))//������û���յ�,����130,�������յ�,��ô�м佫����0
				IrRevData[IR_ML_PIN_NUM] = IR_MID;
			IrRevData[IR_L_PIN_NUM] = IR_LEFT;
			goto __ir_parse_original_data_end;
		//	break;
		default:
			IrRevData[IR_ML_PIN_NUM] = 0;
			break;
	}
#if (0 == SINGLE_LED_DOCK)
	switch(ir_original_data[IR_MR_PIN_NUM]&0xfe)
	{
		case IR_PARSE_HEAD|0x02:
		case IR_PARSE_HEAD|0x0e:
		//case IR_PARSE_HEAD|0x0c:
			IrRevData[IR_MR_PIN_NUM] = IR_MID;
			break;
		case IR_PARSE_HEAD|0x04:
			IrRevData[IR_MR_PIN_NUM] = IR_RIGHT;
			break;
		case IR_PARSE_HEAD|0x08:
			IrRevData[IR_MR_PIN_NUM] = IR_LEFT;
			break;
		case IR_PARSE_HEAD|0x0a://0x0a
			//if(!(ir_original_data[IR_R_PIN_NUM]&0xfe))//����ұ�û���յ�,����032,����ұ��յ�,��ô�м佫����0
				IrRevData[IR_MR_PIN_NUM] = IR_MID;
			IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
			goto __ir_parse_original_data_end;
			//break;
		case IR_PARSE_HEAD|0x06://0x06
			//if(!(ir_original_data[IR_L_PIN_NUM]&0xfe))//������û���յ�,����130,�������յ�,��ô�м佫����0
				IrRevData[IR_MR_PIN_NUM] = IR_MID;
			IrRevData[IR_L_PIN_NUM] = IR_LEFT;
			goto __ir_parse_original_data_end;
		//	break;
		default:
			IrRevData[IR_MR_PIN_NUM] = 0;
			break;
	}
#endif
	if(!IrRevData[IR_L_PIN_NUM])
	{
		switch (ir_original_data[IR_L_PIN_NUM]&0xfe) {
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
				if(IrRevData[IR_ML_PIN_NUM] != IR_LEFT)
					IrRevData[IR_L_PIN_NUM] = IR_MID;
				else
					IrRevData[IR_L_PIN_NUM] = 0;
				break;
			case IR_PARSE_HEAD|0x04:
				if(IrRevData[IR_ML_PIN_NUM] == IR_RIGHT || !IrRevData[IR_ML_PIN_NUM])
					IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
				else
					IrRevData[IR_L_PIN_NUM] = 0;
				break;
			case IR_PARSE_HEAD|0x08:
					IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
#if 0	///old2018-10-21
				//case IR_PARSE_HEAD|0x0a://0x0a
				case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_R_PIN_NUM] && IrRevData[IR_ML_PIN_NUM] == IR_MID)
						IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
					break;
				case IR_PARSE_HEAD|0x0a://0x0a
				//case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_R_PIN_NUM] && !(IrRevData[IR_ML_PIN_NUM] == IR_MID))
					{
						IrRevData[IR_ML_PIN_NUM] = IR_RIGHT;
						IrRevData[IR_L_PIN_NUM] = IR_MID;
					}
					break;
#else	//new 2018-10-22
				//case IR_PARSE_HEAD|0x0a://0x0a
				case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_R_PIN_NUM] && IrRevData[IR_ML_PIN_NUM] == IR_MID)
						IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
					//--------------------------------------------------add 2018-10-22
					else if(!IrRevData[IR_R_PIN_NUM] && !(IrRevData[IR_ML_PIN_NUM]))
					{
						IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
						IrRevData[IR_ML_PIN_NUM] = 0;
						IrRevData[IR_R_PIN_NUM] = 0;
					}
					//--------------------------------------------------add end
					//break;
					goto __ir_parse_original_data_end;
				case IR_PARSE_HEAD|0x0a://0x0a
				//case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_R_PIN_NUM] && !(IrRevData[IR_ML_PIN_NUM]))
					{
						IrRevData[IR_L_PIN_NUM] = IR_LEFT;
						IrRevData[IR_ML_PIN_NUM] = 0;
						IrRevData[IR_R_PIN_NUM] = 0;

					}
					//break;
					goto __ir_parse_original_data_end;
#endif
			default:
				IrRevData[IR_L_PIN_NUM] = 0;
				break;
		}
	}

	if(!IrRevData[IR_R_PIN_NUM])
	{
		switch (ir_original_data[IR_R_PIN_NUM]&0xfe) {
			case IR_PARSE_HEAD|0x02:
			case IR_PARSE_HEAD|0x0e:
				if(IrRevData[IR_ML_PIN_NUM] != IR_RIGHT)
					IrRevData[IR_R_PIN_NUM] = IR_MID;
				else
					IrRevData[IR_R_PIN_NUM] = 0;
				break;
			case IR_PARSE_HEAD|0x04:
				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case IR_PARSE_HEAD|0x08:
				if(IrRevData[IR_ML_PIN_NUM] == IR_LEFT || !IrRevData[IR_ML_PIN_NUM])
					IrRevData[IR_R_PIN_NUM] = IR_LEFT;
				else
					IrRevData[IR_R_PIN_NUM] = 0;
				break;
#if 0	///old2018-10-21
				//case IR_PARSE_HEAD|0x0a://0x0a
				case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_L_PIN_NUM] && IrRevData[IR_ML_PIN_NUM] == IR_MID)
						IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
					break;
				case IR_PARSE_HEAD|0x0a://0x0a
				//case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_L_PIN_NUM] && !IrRevData[IR_ML_PIN_NUM])
					{
						IrRevData[IR_ML_PIN_NUM] = IR_LEFT;
						IrRevData[IR_R_PIN_NUM] = IR_MID;
					}
					break;
#else	//new 2018-10-22
				//case IR_PARSE_HEAD|0x0a://0x0a
				case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_L_PIN_NUM] && IrRevData[IR_ML_PIN_NUM] == IR_MID)
						IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
					//--------------------------------------------------add 2018-10-22
					else if(!IrRevData[IR_L_PIN_NUM] && !IrRevData[IR_ML_PIN_NUM])
					{
						IrRevData[IR_L_PIN_NUM] = 0;
						IrRevData[IR_ML_PIN_NUM] = 0;
						IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
					}
					//--------------------------------------------------add end
					//break;
					goto __ir_parse_original_data_end;
				case IR_PARSE_HEAD|0x0a://0x0a
				//case IR_PARSE_HEAD|0x06://0x06
					if(!IrRevData[IR_L_PIN_NUM] && !IrRevData[IR_ML_PIN_NUM])
					{
						IrRevData[IR_L_PIN_NUM] = 0;
						IrRevData[IR_ML_PIN_NUM] = 0;
						IrRevData[IR_R_PIN_NUM] = IR_LEFT;
					}
					//break;
					goto __ir_parse_original_data_end;
#endif
			default:
				IrRevData[IR_R_PIN_NUM] = 0;
				break;
		}
	}
__ir_parse_original_data_end:
#if 0
	//�ȼ��һ���յ������Ƿ���Ч
	if(ir_original_data[IR_ML_PIN_NUM] & 0xf0 != 0xf0)ir_original_data[IR_ML_PIN_NUM] = 0;
	if(ir_original_data[IR_L_PIN_NUM] & 0xf0 != 0xf0)ir_original_data[IR_L_PIN_NUM] = 0;
	if(ir_original_data[IR_L_PIN_NUM] & 0xf0 != 0xf0)ir_original_data[IR_L_PIN_NUM] = 0;
	ir_dir = 0;
	ir_dir = (((ir_original_data[IR_ML_PIN_NUM]&0x0e)?1:0)<<IR_ML_PIN_NUM) |
					(((ir_original_data[IR_L_PIN_NUM]&0x0e)?1:0)<<IR_L_PIN_NUM) |
					(((ir_original_data[IR_R_PIN_NUM]&0x0e)?1:0)<<IR_R_PIN_NUM);
#endif
	/*

	if(IrRevData[IR_R_PIN_NUM] || IrRevData[IR_ML_PIN_NUM] || IrRevData[IR_L_PIN_NUM])
		log_printf("\r\n[%x%x%x]%d%d%d\r\n",ir_original_data[IR_L_PIN_NUM]&0x0f,ir_original_data[IR_ML_PIN_NUM]&0x0f,ir_original_data[IR_R_PIN_NUM]&0x0f,
											IrRevData[IR_L_PIN_NUM],IrRevData[IR_ML_PIN_NUM],IrRevData[IR_R_PIN_NUM]);
		//log_printf("\r\n[%x%x%x]%d%d%d-",ir_original_data[IR_L_PIN_NUM]&0x0f,ir_original_data[IR_ML_PIN_NUM]&0x0f,ir_original_data[IR_R_PIN_NUM]&0x0f,
		//									IrRevData[IR_L_PIN_NUM],IrRevData[IR_ML_PIN_NUM],IrRevData[IR_R_PIN_NUM]);

*/
#if (0 == SINGLE_LED_DOCK)
	ir_original_data[IR_MR_PIN_NUM] = ir_original_data[IR_ML_PIN_NUM] = ir_original_data[IR_R_PIN_NUM] = ir_original_data[IR_L_PIN_NUM] = 0;
#else
	ir_original_data[IR_ML_PIN_NUM] = ir_original_data[IR_R_PIN_NUM] = ir_original_data[IR_L_PIN_NUM] = 0;
#endif
}

#endif

void ir_parse(uint8_t ir_channel){
//	u32 cur_time;
	uint32_t IrDelay;
	//uint32_t n_timeerr;//ʱ���,�������㵱ǰ���ڵڼ�λ

	//����ʱ���
#if 0//ICLEAN_IRDA_NOTIMER
	irda_tmr = mpu6500_getexttick()/100;
#endif
	//����ʱ���
	irda_tmr += TIM7->CNT;			///100us�ж�
	TIM7->CNT = 0;
	IrDelay = irda_tmr - Timesave[ir_channel];
	//Timesave[ir_channel] = msTmr;
	//irda_tmr = msTmr;

	//IrDelay = irda_tmr - Timesave[ir_channel];
	//if(IrDelay < 10)//���˸���
	//	return;
	//log_printf("%d\r\n",IrDelay);
	//��ʼλ
	if(IrDelay>290){//245			29ms
		Timesave[ir_channel] = irda_tmr;
		if(IR_L_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_L_GPIO_PORT, IR_L_GPIO_PIN)){
			//log_printf("\t%d\t%2x\r\n", ir_channel,IrData[ir_channel]);
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 0xff;
			return;
		}else if(IR_ML_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_M_GPIO_PORT, IR_M_GPIO_PIN)){
			//log_printf("\t%d\t%2x\r\n", ir_channel,IrData[ir_channel]);
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 0xff;
			return;
#if (0 == SINGLE_LED_DOCK)
		}else if(IR_MR_PIN_NUM == ir_channel &&!GPIO_ReadInputDataBit(IR_M2_GPIO_PORT, IR_M2_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 0xff;
			return;
#endif
		}else if(IR_R_PIN_NUM == ir_channel &&!GPIO_ReadInputDataBit(IR_R_GPIO_PORT, IR_R_GPIO_PIN)){
			//log_printf("\t%d\t%2x\r\n", ir_channel,IrData[ir_channel]);
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 0xff;
			return;
		}

	}
	if(!IrCnt[ir_channel] || IrCnt[ir_channel] > 0xf0 || (irda_tmr - ir_time_base[ir_channel] > IR_BIT_PERIOD * 8))
	{
		Timesave[ir_channel] = irda_tmr;
		if(IrDelay>57 && IrDelay<63)
		{
			if(IR_ML_PIN_NUM == ir_channel)
			{
				if((IR_M_GPIO_PORT->IDR & IR_M_GPIO_PIN) != 0)//����Ǹߵ�ƽ,����Ϊ��ʼλ�ĸߵ�ƽ����
				{
					if(IrCnt[ir_channel] == 0xff)
						IrCnt[ir_channel] = 0xfe;
					else if(IrCnt[ir_channel] < 8 && IrCnt[ir_channel])//���֮ǰ�н��չ�����,��ô��λΪֹͣλ
					{
						IrCnt[ir_channel] = 8;
						goto __ir_parse_rxover;
					}
					else
					{
						IrCnt[ir_channel] = 0;
					}
				}
				return;
			}
#if (0 == SINGLE_LED_DOCK)
			else if(IR_MR_PIN_NUM == ir_channel)
			{
				if((IR_M2_GPIO_PORT->IDR & IR_M2_GPIO_PIN) != 0)//����Ǹߵ�ƽ,����Ϊ��ʼλ�ĸߵ�ƽ����
				{
					if(IrCnt[ir_channel] == 0xff)
						IrCnt[ir_channel] = 0xfe;
					else if(IrCnt[ir_channel] < 8 && IrCnt[ir_channel])//���֮ǰ�н��չ�����,��ô��λΪֹͣλ
					{
						IrCnt[ir_channel] = 8;
						goto __ir_parse_rxover;
					}
					else
					{
						//ir_parse_rxover &= ~(1<<ir_channel);
						IrCnt[ir_channel] = 0;
					}
				}
				return;
			}
#endif
			else if(IR_L_PIN_NUM == ir_channel)
			{
				if((IR_L_GPIO_PORT->IDR & IR_L_GPIO_PIN) != 0)//����Ǹߵ�ƽ,����Ϊ��ʼλ�ĸߵ�ƽ����
				{
					if(IrCnt[ir_channel] == 0xff)
						IrCnt[ir_channel] = 0xfe;
					else if(IrCnt[ir_channel] < 8 && IrCnt[ir_channel])//���֮ǰ�н��չ�����,��ô��λΪֹͣλ
					{
						IrCnt[ir_channel] = 8;
						goto __ir_parse_rxover;
					}
					else
					{
						//ir_parse_rxover &= ~(1<<ir_channel);
						IrCnt[ir_channel] = 0;
					}
				}
				return;
			}
			else if(IR_R_PIN_NUM == ir_channel)
			{
				if((IR_R_GPIO_PORT->IDR & IR_R_GPIO_PIN) != 0)//����Ǹߵ�ƽ,����Ϊ��ʼλ�ĸߵ�ƽ����
				{
					if(IrCnt[ir_channel] == 0xff)
						IrCnt[ir_channel] = 0xfe;
					else if(IrCnt[ir_channel] < 8 && IrCnt[ir_channel])//���֮ǰ�н��չ�����,��ô��λΪֹͣλ
					{
						IrCnt[ir_channel] = 8;
						goto __ir_parse_rxover;
					}
					else
					{
						//ir_parse_rxover &= ~(1<<ir_channel);
						IrCnt[ir_channel] = 0;
					}
				}
				return;
			}
		}
		else if(IrDelay>42 && IrDelay<48)
		{
			if(IR_ML_PIN_NUM == ir_channel)
			{
				if((IR_M_GPIO_PORT->IDR & IR_M_GPIO_PIN) == 0)//����ǵ͵�ƽ,����Ϊ��ʼλ�ĵ͵�ƽ����,��ȫ������ʼλ������,�������ݽ���
				{
					if(IrCnt[ir_channel] == 0xfe)
					{
						IrCnt[ir_channel] = 1;
						ir_time_base[ir_channel] = irda_tmr;
						//log_printf(">");
					}
					else
						IrCnt[ir_channel] = 0;
				}
				return;
			}
#if (0 == SINGLE_LED_DOCK)
			else if(IR_MR_PIN_NUM == ir_channel)
			{
				if((IR_M2_GPIO_PORT->IDR & IR_M2_GPIO_PIN) == 0)//����ǵ͵�ƽ,����Ϊ��ʼλ�ĵ͵�ƽ����,��ȫ������ʼλ������,�������ݽ���
				{
					if(IrCnt[ir_channel] == 0xfe)
					{
						IrCnt[ir_channel] = 1;
						ir_time_base[ir_channel] = irda_tmr;
					}
					else
						IrCnt[ir_channel] = 0;
				}
				return;
			}
#endif
			else if(IR_L_PIN_NUM == ir_channel)
			{
				if((IR_L_GPIO_PORT->IDR & IR_L_GPIO_PIN) == 0)//����ǵ͵�ƽ,����Ϊ��ʼλ�ĵ͵�ƽ����,��ȫ������ʼλ������,�������ݽ���
				{
					if(IrCnt[ir_channel] == 0xfe)
					{
						IrCnt[ir_channel] = 1;
						ir_time_base[ir_channel] = irda_tmr;
						//log_printf(">");
					}
					else
						IrCnt[ir_channel] = 0;
				}
				return;
			}
			else if(IR_R_PIN_NUM == ir_channel)
			{
				if((IR_R_GPIO_PORT->IDR & IR_R_GPIO_PIN) == 0)//����ǵ͵�ƽ,����Ϊ��ʼλ�ĵ͵�ƽ����,��ȫ������ʼλ������,�������ݽ���
				{
					if(IrCnt[ir_channel] == 0xfe)
					{
						IrCnt[ir_channel] = 1;
						ir_time_base[ir_channel] = irda_tmr;
						//log_printf(">");
					}
					else
						IrCnt[ir_channel] = 0;
				}
				return;
			}
		}
		if(!IrCnt[ir_channel] || IrCnt[ir_channel] > 0xf0){
			return;
		}
	}

	Timesave[ir_channel] = irda_tmr;
	//log_printf("\t%d",IrDelay);
	if(IrDelay > 23 && IrDelay < 32){
		uint16_t n_now_bit;
		n_now_bit = (uint16_t)((irda_tmr - ir_time_base[ir_channel])/IR_BIT_PERIOD);
		if(n_now_bit > 7)
		{
			IrCnt[ir_channel] = 0;
			//log_printf(">%d",n_now_bit);
			return;
		}
		if(IR_L_PIN_NUM == ir_channel&&((IR_L_GPIO_PORT->IDR & IR_L_GPIO_PIN) != 0)){
			//log_printf("IR_L_PIN_NUM \r\n");
			IrData[ir_channel] |= (1 << (7 - n_now_bit));
		}else if(IR_ML_PIN_NUM == ir_channel&&((IR_M_GPIO_PORT->IDR & IR_M_GPIO_PIN) != 0)){
			IrData[ir_channel] |= (1 << (7 - n_now_bit));
#if (0 == SINGLE_LED_DOCK)
		}else if(IR_MR_PIN_NUM == ir_channel&&((IR_M2_GPIO_PORT->IDR & IR_M2_GPIO_PIN) != 0)){
			IrData[ir_channel] |= (1 << (7 - n_now_bit));
			//log_printf("IR_M_PIN_NUM \r\n");
#endif
		}else if(IR_R_PIN_NUM == ir_channel&&((IR_R_GPIO_PORT->IDR & IR_R_GPIO_PIN) != 0)){
			IrData[ir_channel] |= (1 << (7 - n_now_bit));
			//log_printf("IR_R_PIN_NUM \r\n");
		}
		//log_printf(">%02x",IrData[ir_channel]);
		//PRINTF("NULL \r\n");
		//IrCnt[ir_channel]++;
	}
	/*else if(IrDelay > 4)
	{
		IrData[ir_channel] <<=1;
		IrCnt[ir_channel]++;
	}
	else
	{
		IrCnt[ir_channel] = 0;
		return;
	}*/
__ir_parse_rxover:
	if(8 == IrCnt[ir_channel]){

		//IrData[ir_channel] = IrData[ir_channel] >>1;
#if 0
		if (IrData[ir_channel]&0xffff == 0x26a5)

		else
			IrData[ir_channel] = IrData[ir_channel]&0xfff;
#endif

		IrData[ir_channel] = IrData[ir_channel]&0xffff;

		//PRINTF("%x\r\n", IrData[ir_channel]);
		//if(ir_parse_printen)
		//if(ir_channel == IR_M_PIN_NUM)
			//log_printf("\t%d,%x\r\n", ir_channel,IrData[ir_channel]);
		//if(IrData[ir_channel] == 0xf0)
		//	log_printf("ir(%d,%x)\r\n", ir_channel,IrData[ir_channel]);
		g_ir_data[ir_channel] = IrData[ir_channel];
		ir_original_data[ir_channel] = IrData[ir_channel];
		//ir_parse_get_anydata = (g_ir_data[ir_channel]&0xf0 ==IR_PARSE_HEAD)?1:0;
		//ir_parse_get_anydata = (IrData[ir_channel]>0x10)?1:0;
		ir_parse_get_anydata = 0;
		/*	*/
#if (1 == SINGLE_LED_DOCK)
		if((IrData[ir_channel] & (IR_PARSE_HEAD | 0x01)) == (IR_PARSE_HEAD | 0x01))
			ir_dock_signal |= (1 << ir_channel);
#endif
		//else
		//	ir_dock_signal &= ~(1 << ir_channel);
		//if(ir_parse_get_anydata)
			//log_printf("ir_parse(%d,%x)\r\n", ir_channel,IrData[ir_channel]);
		IrData[ir_channel] = 0;
		IrCnt[ir_channel] = 0;

	}

	return;

}
#else
void ir_parse(uint8_t ir_channel){
//	u32 cur_time;
	u32 IrDelay;

	//����ʱ���
	IrDelay = msTmr - Timesave[ir_channel];
	Timesave[ir_channel] = msTmr;


	//��ʼλ
	if(IrDelay>8){
		if(IR_L_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_L_GPIO_PORT, IR_L_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}else if(IR_M_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_M_GPIO_PORT, IR_M_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}else if(IR_R_PIN_NUM == ir_channel &&!GPIO_ReadInputDataBit(IR_R_GPIO_PORT, IR_R_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}

	}
	if(!IrCnt[ir_channel]){
		return;
	}

	if(IrDelay>=2){
		if(IR_L_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_L_GPIO_PORT, IR_L_GPIO_PIN)){
			//PRINTF("IR_L_PIN_NUM \r\n");
			IrData[ir_channel] |= 0x1;
		}else if(IR_M_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_M_GPIO_PORT, IR_M_GPIO_PIN)){
			IrData[ir_channel] |= 0x1;
			//PRINTF("IR_M_PIN_NUM \r\n");
		}else if(IR_R_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_R_GPIO_PORT, IR_R_GPIO_PIN)){
			IrData[ir_channel] |= 0x1;
			//PRINTF("IR_R_PIN_NUM \r\n");
		}
		//PRINTF("NULL \r\n");
		IrCnt[ir_channel]++;
		IrData[ir_channel] <<=1;
	}
	if(8 == IrCnt[ir_channel]){

		//IrData[ir_channel] = IrData[ir_channel] >>1;
#if 0
		if (IrData[ir_channel]&0xffff == 0x26a5)

		else
			IrData[ir_channel] = IrData[ir_channel]&0xfff;
#endif
		IrData[ir_channel] = IrData[ir_channel]&0xffff;

		//PRINTF("%x\r\n", IrData[ir_channel]);
		log_printf("ir(%d,%x)\r\n", ir_channel,IrData[ir_channel]);
		g_ir_data[ir_channel] = IrData[ir_channel];
		//ir_original_data[ir_channel] = IrData[ir_channel];
#if 1
		switch (ir_channel) {
		case IR_M_PIN_NUM:
			switch(IrData[ir_channel]){
			case 0xf2:
			case 0xfe:
				IrRevData[IR_M_PIN_NUM] = IR_MID;
				break;
			case 0xf4:
				IrRevData[IR_M_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf8:
				IrRevData[IR_M_PIN_NUM] = IR_LEFT;
				break;
			case 0xfa:
				IrRevData[IR_M_PIN_NUM] = IR_MID;

				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf6:
				IrRevData[IR_M_PIN_NUM] = IR_MID;

				IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
			case 0xc4:
				IrRevData[IR_M_PIN_NUM] = IR_WALL;

				break;
			default:
				IrRevData[IR_M_PIN_NUM] = 0;
				break;
			}
			break;
		case IR_R_PIN_NUM:
			switch (IrData[ir_channel]) {
			case 0xf2:
			case 0xfe:
				IrRevData[IR_R_PIN_NUM] = IR_MID;
				break;
			case 0xc4:
				IrRevData[IR_R_PIN_NUM] = IR_WALL;
				break;
#if 1
			case 0xf4:
				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf8:
				IrRevData[IR_R_PIN_NUM] = IR_LEFT;
				break;
#endif
			default:
				break;
			}
			break;
		case IR_L_PIN_NUM:
			switch (IrData[ir_channel]) {
			case 0xf2:
			case 0xfe:
				IrRevData[IR_L_PIN_NUM] = IR_MID;
				break;
			case 0xc4:
				IrRevData[IR_L_PIN_NUM] = IR_WALL;
				break;
#if 1
			case 0xf4:
				IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf8:
				IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
#endif
			default:
				break;
			}
			break;
		default:
			break;
		}
#endif
		IrData[ir_channel] = 0;
		IrCnt[ir_channel] = 0;

	}

	return;

}

#endif
void read_ir_data(u8 *IR,char flage)
{
//	static int ko = 0;
//	taskENTER_CRITICAL();
//	static uint8_t i = 0;
	#if (1 == SINGLE_LED_DOCK)
	ir_parse_original_data();
	//memcpy(IR,IrRevData,sizeof(IrRevData));
	IR[IR_L_PIN_NUM] = IrRevData[IR_L_PIN_NUM];
	IR[IR_ML_PIN_NUM] = IrRevData[IR_ML_PIN_NUM];
	IR[IR_R_PIN_NUM] = IrRevData[IR_R_PIN_NUM];
	memset(IrRevData,0,sizeof(IrRevData));
	#elif (0 == SINGLE_LED_DOCK)
	IR[IR_L_PIN_NUM] = ir_original_data[IR_L_PIN_NUM] & 0x0F;
	IR[IR_ML_PIN_NUM] = ir_original_data[IR_ML_PIN_NUM]& 0x0F;
	IR[IR_MR_PIN_NUM] = ir_original_data[IR_MR_PIN_NUM]& 0x0F;
	IR[IR_R_PIN_NUM] = ir_original_data[IR_R_PIN_NUM]& 0x0F;
	memset(ir_original_data,0,sizeof(ir_original_data));
	#endif
	//log_printf("(%d%d%d)-%d\r\n",IR[IR_L_PIN_NUM],IR[IR_M_PIN_NUM],IR[IR_R_PIN_NUM],i ++);
	/*
	if(flage)
		return ;
	if(flage == 100)
		memset(IrRevData,0,sizeof(IrRevData));
	if(ko++ >=5)
	{
		memset(IrRevData,0,sizeof(IrRevData));
		ko = 0;
	}
	*/

//	taskEXIT_CRITICAL();
}

void read_ir_data_timeout(u8 *IR,char flage)
{

	uint32_t tt=0;
	if(mstimeout(&tt,81))
	{
#if (1 == SINGLE_LED_DOCK)
		ir_parse_original_data();
		//memcpy(IR,IrRevData,sizeof(IrRevData));
		IR[IR_L_PIN_NUM] = IrRevData[IR_L_PIN_NUM];
		IR[IR_ML_PIN_NUM] = IrRevData[IR_ML_PIN_NUM];
		IR[IR_R_PIN_NUM] = IrRevData[IR_R_PIN_NUM];
		memset(IrRevData,0,sizeof(IrRevData));
	//	log_printf("a");
#elif (0 == SINGLE_LED_DOCK)
		IR[IR_L_PIN_NUM] = ir_original_data[IR_L_PIN_NUM] & 0x0F;
		IR[IR_ML_PIN_NUM] = ir_original_data[IR_ML_PIN_NUM]& 0x0F;
		IR[IR_MR_PIN_NUM] = ir_original_data[IR_MR_PIN_NUM]& 0x0F;
		IR[IR_R_PIN_NUM] = ir_original_data[IR_R_PIN_NUM]& 0x0F;
		memset(ir_original_data,0,sizeof(ir_original_data));
	//	return SET;
#endif
	}else
	{
/*
		IR[IR_L_PIN_NUM] = 0;
		IR[IR_ML_PIN_NUM] = 0;
		IR[IR_MR_PIN_NUM] = 0;
		IR[IR_R_PIN_NUM] = 0;
*/
	//	return RESET;
	}

}


//�����������ر�ʹ��,Ϊ1ʱ��������ر�,Ϊ0ʱ���������
void set_ir_alter_dis(uint8_t dis)
{
	ir_alter_off = dis ? 1:0;
	log_printf("[set_ir_alter_dis]%d\r\n",ir_alter_off);
}


uint16_t read_ir_dock_knk(void)
{
	uint8_t n_irdata[4];
	uint16_t res = 0;
	//memcpy(n_irdata,g_ir_data,sizeof(g_ir_data));
	n_irdata[IR_L_PIN_NUM] = g_ir_data[IR_L_PIN_NUM];
	n_irdata[IR_ML_PIN_NUM] = g_ir_data[IR_ML_PIN_NUM];
#if (0 == SINGLE_LED_DOCK)
	n_irdata[IR_MR_PIN_NUM] = g_ir_data[IR_MR_PIN_NUM];
#endif
	n_irdata[IR_R_PIN_NUM] = g_ir_data[IR_R_PIN_NUM];
	#if 1
	//log_printf("\r\n%02x-%02x-%02x\r\n",n_irdata[0],n_irdata[1],n_irdata[2]);
	if((n_irdata[IR_L_PIN_NUM] & IR_PARSE_TOPLED) == IR_PARSE_TOPLED)
	{
		g_ir_data[IR_L_PIN_NUM] = 0;
		res |= MASK_BUM_LEFT;
	}
	if((n_irdata[IR_R_PIN_NUM] & IR_PARSE_TOPLED) == IR_PARSE_TOPLED)
	{
		g_ir_data[IR_R_PIN_NUM] = 0;
		res |= MASK_BUM_RIGHT;
		log_printf("rbum...\r\n");
	}
	if((n_irdata[IR_ML_PIN_NUM] & IR_PARSE_TOPLED) == IR_PARSE_TOPLED)
	{
		g_ir_data[IR_ML_PIN_NUM] = 0;
		res = MASK_BUM_MIDL;
	}
#if (0 == SINGLE_LED_DOCK)
	if((n_irdata[IR_MR_PIN_NUM] & IR_PARSE_TOPLED) == IR_PARSE_TOPLED)
	{
		g_ir_data[IR_MR_PIN_NUM] = 0;
		res = MASK_BUM_MIDL;
	}
#endif
	#else
	if(n_irdata[IR_L_PIN_NUM] == 0xf0)
	{
		g_ir_data[IR_L_PIN_NUM] = 0;
		res |= MASK_BUM_LEFT;
	}
	if(n_irdata[IR_R_PIN_NUM] == 0xf0)
	{
		g_ir_data[IR_R_PIN_NUM] = 0;
		res |= MASK_BUM_RIGHT;
	}
	if(n_irdata[IR_ML_PIN_NUM] == 0xf0)
	{
		g_ir_data[IR_ML_PIN_NUM] = 0;
		res = MASK_BUM_MIDL;
	}
	#endif

	if(res)
		log_printf("\r\n%02x-%02x-%02x-%02x dock:%02x\r\n"
		,n_irdata[IR_L_PIN_NUM],n_irdata[IR_ML_PIN_NUM],n_irdata[IR_MR_PIN_NUM],n_irdata[IR_R_PIN_NUM],res);


	return res;
}


#if 0
void ir_parse(uint8_t ir_channel){
//	u32 cur_time;
	u32 IrDelay;

	//����ʱ���
	IrDelay = msTmr - Timesave[ir_channel];
	Timesave[ir_channel] = msTmr;


	//��ʼλ
	if(IrDelay>8){
		if(IR_L_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_L_GPIO_PORT, IR_L_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}else if(IR_M_PIN_NUM == ir_channel&&!GPIO_ReadInputDataBit(IR_M_GPIO_PORT, IR_M_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}else if(IR_R_PIN_NUM == ir_channel &&!GPIO_ReadInputDataBit(IR_R_GPIO_PORT, IR_R_GPIO_PIN)){
			IrData[ir_channel] = 0;
			IrCnt[ir_channel] = 1;
			return;
		}

	}
	if(!IrCnt[ir_channel]){
		return;
	}

	if(IrDelay>=2){
		if(IR_L_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_L_GPIO_PORT, IR_L_GPIO_PIN)){
			//PRINTF("IR_L_PIN_NUM \r\n");
			IrData[ir_channel] |= 0x1;
		}else if(IR_M_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_M_GPIO_PORT, IR_M_GPIO_PIN)){
			IrData[ir_channel] |= 0x1;
			//PRINTF("IR_M_PIN_NUM \r\n");
		}else if(IR_R_PIN_NUM == ir_channel&&GPIO_ReadInputDataBit(IR_R_GPIO_PORT, IR_R_GPIO_PIN)){
			IrData[ir_channel] |= 0x1;
			//PRINTF("IR_R_PIN_NUM \r\n");
		}
		//PRINTF("NULL \r\n");
		IrCnt[ir_channel]++;
		IrData[ir_channel] <<=1;
	}
	if(8 == IrCnt[ir_channel]){

		//IrData[ir_channel] = IrData[ir_channel] >>1;
#if 0
		if (IrData[ir_channel]&0xffff == 0x26a5)

		else
			IrData[ir_channel] = IrData[ir_channel]&0xfff;
#endif
		IrData[ir_channel] = IrData[ir_channel]&0xffff;

		//PRINTF("%x\r\n", IrData[ir_channel]);
	//	PRINTF("%x\r\n", IrData[ir_channel]);
		g_ir_data[ir_channel] = IrData[ir_channel];
		switch (ir_channel) {
		case IR_M_PIN_NUM:
			switch(IrData[ir_channel]){
			case 0xf2:
			case 0xfe:
				IrRevData[IR_M_PIN_NUM] = IR_MID;
				break;
			case 0xf4:
				IrRevData[IR_M_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf8:
				IrRevData[IR_M_PIN_NUM] = IR_LEFT;
				break;
			case 0xfa:
				IrRevData[IR_M_PIN_NUM] = IR_MID;

				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case 0xf6:
				IrRevData[IR_M_PIN_NUM] = IR_MID;

				IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
			case 0xC4:
				IrRevData[IR_M_PIN_NUM] = IR_WALL;

				break;
			default:
				IrRevData[IR_M_PIN_NUM] = 0;
				break;
			}
			break;
		case IR_R_PIN_NUM:
			switch (IrData[ir_channel]) {
			case 0x52:
			case 0x5e:
				IrRevData[IR_R_PIN_NUM] = IR_MID;
				break;
			case 0xc4:
				IrRevData[IR_R_PIN_NUM] = IR_WALL;
				break;
#if 1
			case 0x54:
				IrRevData[IR_R_PIN_NUM] = IR_RIGHT;
				break;
			case 0x58:
				IrRevData[IR_R_PIN_NUM] = IR_LEFT;
				break;
#endif
			default:
				break;
			}
			break;
		case IR_L_PIN_NUM:
			switch (IrData[ir_channel]) {
			case 0x52:
			case 0x5e:
				IrRevData[IR_L_PIN_NUM] = IR_MID;
				break;
			case 0xc4:
				IrRevData[IR_L_PIN_NUM] = IR_WALL;
				break;
#if 1
			case 0x54:
				IrRevData[IR_L_PIN_NUM] = IR_RIGHT;
				break;
			case 0x58:
				IrRevData[IR_L_PIN_NUM] = IR_LEFT;
				break;
#endif
			default:
				break;
			}
			break;
		default:
			break;
		}
		IrData[ir_channel] = 0;
		IrCnt[ir_channel] = 0;

	}

	return;

}
#endif


//������⽨��
#define MAX_IRD_MAP		11

/*
//�汾1
int right_ird_map[MAX_IRD_MAP]={600,650,810,1060,1210,1370,1680,1900,2320,2900,3300};
int right_dist_map[MAX_IRD_MAP]={15,13,11,9,8,7,6,5,4,3,3};
int left_ird_map[MAX_IRD_MAP]={740,810,970,1260,1450,1760,2100,2450,3200,3740,3800};
int left_dist_map[MAX_IRD_MAP]={15,13,11,9,8,7,6,5,4,3,3};
*/

/*
�汾2
int right_ird_map[MAX_IRD_MAP]={300,350,400,500,600,700,800,900,1000,1200,1900};
int right_dist_map[MAX_IRD_MAP]={15,13,11,9,8,7,6,5,4,3,3};
int left_ird_map[MAX_IRD_MAP]={300,350,400,500,600,700,800,900,1000,1200,1900};
int left_dist_map[MAX_IRD_MAP]={15,13,11,9,8,7,6,5,4,3,3};
*/
int right_ird_map[MAX_IRD_MAP]={300,350,450,500,600,700,800,900,1000,1200,1900};
int right_dist_map[MAX_IRD_MAP]={14,12,11,10,8,7,6,5,4,3,3};
int left_ird_map[MAX_IRD_MAP]={300,350,400,500,600,700,800,900,1000,1200,1900};
int left_dist_map[MAX_IRD_MAP]={15,13,11,9,8,7,6,5,4,3,3};



int ird_to_dist(int side,int ird)
{
	int i;
	int *ird_map;
	int *dist_map;

	if(side == LEFT_SIDE)
	{
		ird_map 	= left_ird_map;
		dist_map 	= left_dist_map;
	}else if (side == RIGHT_SIDE)
	{
		ird_map 	= right_ird_map;
		dist_map 	= right_dist_map;
	}else
		return 0;
	ird +=50;		//��������
	if(ird <ird_map[0])
		return 0;
	if(ird > ird_map[MAX_IRD_MAP-1])
		return CM_PER_HW;

	for(i=0;i<MAX_IRD_MAP;i++)
	{
		if(ird < ird_map[i])
			break;
	}
	if(i > 0 && i<=MAX_IRD_MAP)
	{
		return(dist_map[i-1]*CM_PER_HW);
	}else
		return 0;
}
/*
�����м���ײ
�ص㣬���������������Ƚϴ��������������
*/
#define MIN_MDL_IR_ADC		500
#define MAX_MAL_IR_ADC		1200
int IR_MIDLE_BUM1(void)
{
	//���붼Ҫ����
	if(sys->g_sta[2] > 400 && sys->g_sta[3] > 200 && sys->g_sta[4]>300)
	{
		int c=0;
		if(sys->g_sta[2] >= 1100)
			c++;
		if(sys->g_sta[3] >= 700)
			c++;
		if(sys->g_sta[4] >= 700)
			c++;
		if(c >=2)
			return c;
		else
			return 0;
	}else
		return 0;
}


//?����??-��?��?o����a��y?Y,��?o����y��??����?2a?D,????��?��???��D��?��?
void read_ir_original_data(u8 *IR)
{
//#if (1 == SINGLE_LED_DOCK)
//	static int ko = 0;
//	taskENTER_CRITICAL();

	//memcpy(IR,IrRevData,sizeof(IrRevData));
	IR[IR_L_PIN_NUM] = ir_original_data[IR_L_PIN_NUM];
	IR[IR_ML_PIN_NUM] = ir_original_data[IR_ML_PIN_NUM];
	IR[IR_R_PIN_NUM] = ir_original_data[IR_R_PIN_NUM];
	IR[IR_MR_PIN_NUM] = ir_original_data[IR_MR_PIN_NUM];
	memset(ir_original_data,0,sizeof(ir_original_data));
	//log_printf("(%d%d%d)-%d\r\n",IR[IR_L_PIN_NUM],IR[IR_M_PIN_NUM],IR[IR_R_PIN_NUM],i ++);
	/*
	if(flage)
		return ;
	if(flage == 100)
		memset(IrRevData,0,sizeof(IrRevData));
	if(ko++ >=5)
	{
		memset(IrRevData,0,sizeof(IrRevData));
		ko = 0;
	}
	*/

//	taskEXIT_CRITICAL();
//#endif
}

void init_BackupRegister(void)
{
	//dust
	sys->dust_full_check_tmr_min = sys->t_navi_work/60;
	BKP_WriteBackupRegister(BKP_DUST_DATA,sys->dust_full_check_tmr_min);			//		����ʱ��Ĵ���
	log_printf("first or BKP err:update dust_full_check_tmr_min:%d\r\n",sys->dust_full_check_tmr_min);
	//strainer
	sys->strainer_check_tmr_min = sys->t_navi_work/60;
	BKP_WriteBackupRegister(BKP_STRAINER_ALARM_DATA,sys->strainer_check_tmr_min);			//
	log_printf("first or BKP err:update strainer_check_tmr_min:%d\r\n",sys->strainer_check_tmr_min);
	//side_rush
	sys->side_rush_check_tmr_min = sys->t_navi_work/60;
	BKP_WriteBackupRegister(BKP_SIDE_RUSH_ALARM_DATA,sys->side_rush_check_tmr_min); 		//
	log_printf("first or BKP err:update side_rush_check_tmr_min:%d\r\n",sys->side_rush_check_tmr_min);
	//main_rush
	sys->main_rush_check_tmr_min = sys->t_navi_work/60;
	BKP_WriteBackupRegister(BKP_MAIN_RUSH_ALARM_DATA,sys->main_rush_check_tmr_min); 		//
	log_printf("first or BKP err:update main_rush_check_tmr_min:%d\r\n",sys->main_rush_check_tmr_min);

	BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
}


///0,ֱ�ӻ�ȡ����,����ȷ����,��ʼ��
///1,��ʼ������
///2,��������
///3,���ݼ��
void dust_full_check_pro(uint8_t flag)
{
	if(flag == 1)
	{
		sys->dust_full_check_tmr_min = 0;
		log_printf("init dust_full_check_tmr_min:%d\r\n",sys->dust_full_check_tmr_min);
		BKP_WriteBackupRegister(BKP_DUST_DATA,sys->dust_full_check_tmr_min);		//		����ʱ��Ĵ���
		BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
		return;
	}
	else if(flag == 2)
	{
		if( BKP_ReadBackupRegister(BKP_CONSUMABLES_CHECK_CODE) == BKP_CHECK)
		{
			sys->dust_full_check_tmr_min = BKP_ReadBackupRegister(BKP_DUST_DATA);			//��ȡ����ʱ��Ĵ���ʱ��
			log_printf("last dust_full_check_tmr_min:%d\r\n",sys->dust_full_check_tmr_min);
			sys->dust_full_check_tmr_min += sys->t_navi_work/60;			      					//�ϱ�ʱ�����
			BKP_WriteBackupRegister(BKP_DUST_DATA,sys->dust_full_check_tmr_min);			//		���³���ʱ��Ĵ���
			log_printf("update dust_full_check_tmr_min:%d\r\n",sys->dust_full_check_tmr_min);
		}
		else
		{
			init_BackupRegister();
		}
	}
	else if(flag == 3)
	{
		if(sys->dust_full_check_tmr_min>=CFG_DUST_FULL_TIMER_ALARM)		//�������г���ʱ��
		{
			//if( sys->work_errcode == MWERR_NONE)
			if((sys->work_errcode & MWERR_CLRDUSTBOX) != MWERR_CLRDUSTBOX)
			{
				log_printf("dust_box_full\r\n");
				sys->work_errcode |= MWERR_CLRDUSTBOX;
				ny3p_play(VOICE_CHECK_BOX); 	//�����Ѿ�����
//				sys->wifistatechange = 1;
				sys->dis_statechange = 1;
				sys->wifistatechange = 1;
				//sys->dusk_usetime_alarm = 1;		//������Ҫͨ��DP���ϱ���APP
				//dust_box_full_flag = 1;
			}
		}
	}
	if(sys->dust_full_check_tmr_min>=1200)
	{
		log_printf("dust_box_full too big\r\n");
		log_printf("init dust_full_check_tmr_min:%d to 0\r\n",sys->dust_full_check_tmr_min);
		sys->dust_full_check_tmr_min = 0;
		BKP_WriteBackupRegister(BKP_DUST_DATA,sys->dust_full_check_tmr_min);
		//����ʱ��Ĵ���
		BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);
	}
}



///0,ֱ�ӻ�ȡ����,����ȷ����,��ʼ��
///1,��ʼ������
///2,��������
///3,���ݼ��
void strainer_alarm_check_pro(uint8_t flag)
{
	uint32_t strainer_check_tmr_min=0;
	if(sys->t_navi_work>0 && sys->t_navi_work<18000)
	{

	if(flag == 1)
	{
		sys->strainer_check_tmr_min = 0;
		log_printf("init strainer_check_tmr_min:%d\r\n",sys->strainer_check_tmr_min);
		BKP_WriteBackupRegister(BKP_STRAINER_ALARM_DATA,sys->strainer_check_tmr_min);		//
		//BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
		return;
	}
	else if(flag == 2)
	{
		if( BKP_ReadBackupRegister(BKP_CONSUMABLES_CHECK_CODE) == BKP_CHECK)
		{
			strainer_check_tmr_min = BKP_ReadBackupRegister(BKP_STRAINER_ALARM_DATA);			//
			log_printf("last strainer_check_tmr_min:%d\r\n",strainer_check_tmr_min);
			sys->strainer_check_tmr_min = strainer_check_tmr_min + sys->t_navi_work/60;
			BKP_WriteBackupRegister(BKP_STRAINER_ALARM_DATA,sys->strainer_check_tmr_min);			//		���³���ʱ��Ĵ���
			log_printf("update strainer_check_tmr_min:%d\r\n",sys->strainer_check_tmr_min);
		}
		else
		{
			init_BackupRegister();
		}
	}
	}
	#if 0
	else if(flag == 3)
	{
		if(sys->strainer_check_tmr_min>=CFG_STRAINER_ALARM)
		{
			//
			//if( sys->work_errcode == MWERR_NONE)
			if(sys->strainer_usetime_alarm == 0)
			{
				log_printf("strainer_usetime_alarm\r\n");
			//	sys->work_errcode = MWERR_CLRDUSTBOX;
				//ny3p_play(VOICE_CHECK_BOX); 	//�����Ѿ�����
//				sys->wifistatechange = 1;
				//sys->dis_statechange = 1;
				sys->wifistatechange = 1;
				sys->strainer_usetime_alarm = 1;
				//sys->work_alarmcode |= 0x02;
				sys->work_alarmcode |= STRAINER_ALARM;
				//01 ���� 02 ���� 03 ��ˢ 04 ��ˢ

				//dust_box_full_flag = 1;
			}
		}
	}
	#endif
}



///0,ֱ�ӻ�ȡ����,����ȷ����,��ʼ��
///1,��ʼ������
///2,��������
///3,���ݼ��
void side_rush_alarm_check_pro(uint8_t flag)
{
	uint32_t side_rush_check_tmr_min=0;
	if(flag == 1)
	{
		sys->side_rush_check_tmr_min = 0;
		log_printf("init strainer_check_tmr_min:%d\r\n",sys->side_rush_check_tmr_min);
		BKP_WriteBackupRegister(BKP_SIDE_RUSH_ALARM_DATA,sys->side_rush_check_tmr_min);		//
		//BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
		return;
	}
	else if(flag == 2)
	{
		if( BKP_ReadBackupRegister(BKP_CONSUMABLES_CHECK_CODE) == BKP_CHECK)
		{
			side_rush_check_tmr_min = BKP_ReadBackupRegister(BKP_SIDE_RUSH_ALARM_DATA);			//
			log_printf("last side_rush_check_tmr_min:%d\r\n",side_rush_check_tmr_min);
			sys->side_rush_check_tmr_min = side_rush_check_tmr_min + sys->t_navi_work/60;
			BKP_WriteBackupRegister(BKP_SIDE_RUSH_ALARM_DATA,sys->side_rush_check_tmr_min);			//		���³���ʱ��Ĵ���
			log_printf("update side_rush_check_tmr_min:%d\r\n",sys->side_rush_check_tmr_min);
		}
		else
		{
			init_BackupRegister();
		}
	}
	#if 0
	else if(flag == 3)
	{
		if(sys->side_rush_check_tmr_min>=CFG_SIDE_RUSH_ALARM)
		{
			//if( sys->work_errcode == MWERR_NONE)
			if(sys->side_rush_usetime_alarm == 0)
			{
				log_printf("side_rush_usetime_alarm\r\n");
				sys->wifistatechange = 1;
				sys->side_rush_usetime_alarm = 1;
				sys->work_alarmcode |= SIDE_RUSH_ALARM;
			}
		}
	}
	#endif
}



///0,ֱ�ӻ�ȡ����,����ȷ����,��ʼ��
///1,��ʼ������
///2,��������
///3,���ݼ��
void main_rush_alarm_check_pro(uint8_t flag)
{
	uint32_t main_rush_check_tmr_min=0;

	if(flag == 1)
	{
		sys->main_rush_check_tmr_min = 0;
		log_printf("init main_rush_check_tmr_min:%d\r\n",sys->main_rush_check_tmr_min);
		BKP_WriteBackupRegister(BKP_MAIN_RUSH_ALARM_DATA,sys->main_rush_check_tmr_min);		//
		//BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
		return;
	}
	else if(flag == 2)
	{
		if( BKP_ReadBackupRegister(BKP_CONSUMABLES_CHECK_CODE) == BKP_CHECK)
		{
			main_rush_check_tmr_min = BKP_ReadBackupRegister(BKP_MAIN_RUSH_ALARM_DATA);			//
			log_printf("last main_rush_check_tmr_min:%d\r\n",main_rush_check_tmr_min);
			sys->main_rush_check_tmr_min = main_rush_check_tmr_min + sys->t_navi_work/60;
			BKP_WriteBackupRegister(BKP_MAIN_RUSH_ALARM_DATA,sys->main_rush_check_tmr_min);			//		���³���ʱ��Ĵ���
			log_printf("update main_rush_check_tmr_min:%d\r\n",sys->main_rush_check_tmr_min);
		}
		else
		{
			init_BackupRegister();
		}
	}
#if 0
	else if(flag == 3)
	{
		if(sys->main_rush_check_tmr_min>=CFG_MAIN_RUSH_ALARM)
		{
			//if( sys->work_errcode == MWERR_NONE)
			if(sys->main_rush_usetime_alarm == 0)
			{
				log_printf("main_rush_usetime_alarm\r\n");
				sys->wifistatechange = 1;
				sys->main_rush_usetime_alarm = 1;
				sys->work_errcode = MWERR_AGITATOR_REPLACEMENT;
				//sys->work_alarmcode |= MAIN_RUSH_ALARM;
			}
		}
	}
#endif
}


////���ӱ�ˢ����ˢend


void consumables_check_pro(uint8_t flag)
{
	if(flag == 0)		//��֤���ݼĴ�������Ķ������Ƿ�Ϊ�洢������
	{
		if( BKP_ReadBackupRegister(BKP_CONSUMABLES_CHECK_CODE) == BKP_CHECK)   //���м��
		{		//�Ĵ�����������������
#if DUST_BOX_FULL_CHECK	//���м��
			log_printf("get last dust_full_check_tmr_min:");                    //���м��ʱ��
			sys->dust_full_check_tmr_min = BKP_ReadBackupRegister(BKP_DUST_DATA);
			log_printf("%d\r\n",sys->dust_full_check_tmr_min);
#endif
#if STRAINER_ALARM_CHECK //�������
			log_printf("get last strainer_check_tmr_min:");
			sys->strainer_check_tmr_min = BKP_ReadBackupRegister(BKP_STRAINER_ALARM_DATA);
			log_printf("%d\r\n",sys->strainer_check_tmr_min);
#endif
			////���ӱ�ˢ����ˢ
#if SIDE_RUSH_ALARM_CHECK	//��ˢ
			log_printf("get last side_rush_check_tmr_min:");					//���м��ʱ��
			sys->side_rush_check_tmr_min = BKP_ReadBackupRegister(BKP_SIDE_RUSH_ALARM_DATA);
			log_printf("%d\r\n",sys->side_rush_check_tmr_min);
#endif
#if MAIN_RUSH_ALARM_CHECK //��ˢ
			log_printf("get last main_rush_check_tmr_min:");
			sys->main_rush_check_tmr_min = BKP_ReadBackupRegister(BKP_MAIN_RUSH_ALARM_DATA);
			log_printf("%d\r\n",sys->main_rush_check_tmr_min);
#endif
			////���ӱ�ˢ����ˢend
		}
		else	//�Ĵ�����û������������,��ʼ��
		{
#if DUST_BOX_FULL_CHECK	//���м��

			log_printf("null\r\n");
			sys->dust_full_check_tmr_min = 0;
			log_printf("init strainer_check_tmr_min:%d\r\n",sys->dust_full_check_tmr_min);
			BKP_WriteBackupRegister(BKP_DUST_DATA,sys->dust_full_check_tmr_min);		//
#endif

#if STRAINER_ALARM_CHECK //�������
			log_printf("null\r\n");
			sys->strainer_check_tmr_min = 0;
			log_printf("init dust_full_check_tmr_min:%d\r\n",sys->strainer_check_tmr_min);
			BKP_WriteBackupRegister(BKP_STRAINER_ALARM_DATA,sys->strainer_check_tmr_min);		//		����ʱ��Ĵ���
#endif
////���ӱ�ˢ����ˢ
#if SIDE_RUSH_ALARM_CHECK	//
			log_printf("null\r\n");
			sys->side_rush_check_tmr_min = 0;
			log_printf("init side_rush_check_tmr_min:%d\r\n",sys->side_rush_check_tmr_min);
			BKP_WriteBackupRegister(BKP_SIDE_RUSH_ALARM_DATA,sys->side_rush_check_tmr_min);		//

#endif

#if MAIN_RUSH_ALARM_CHECK //
			log_printf("null\r\n");
			sys->main_rush_check_tmr_min = 0;
			log_printf("init main_rush_check_tmr_min:%d\r\n",sys->main_rush_check_tmr_min);
			BKP_WriteBackupRegister(BKP_MAIN_RUSH_ALARM_DATA,sys->main_rush_check_tmr_min);		//		����ʱ��Ĵ���

#endif
////���ӱ�ˢ����ˢend
			BKP_WriteBackupRegister(BKP_CONSUMABLES_CHECK_CODE,BKP_CHECK);						//		У��
		}
	}	///ok
}

void comm_ultrasonic(unsigned short * data)
{
	union{
		unsigned char c[4];
		unsigned short s[2];
		unsigned int i;
	} distance_temp;

	for(unsigned char cu_num = 0; cu_num < 2; cu_num++)
	{
		distance_temp.c[cu_num] = I2C1_ReadByte(ULTRASONIC_I2CADDR, DISTANCE_I2CADDR);
	}
	*data = distance_temp.s[0];
}

