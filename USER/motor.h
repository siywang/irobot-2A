#ifndef _H_MOTOR_
#define _H_MOTOR_

#define MOTOR_ZHENG		1		  //2020-03-15�ֳ�ʵ��Ϊ1

#define  ENABLE_MOTOER()		GPIO_WriteBit(GPIOE, GPIO_Pin_2,   Bit_SET);

//����
#define GO_STOP			0
#define GO_FORWARD		1
#define GO_BACK			2
#define GO_LEFT			3
#define GO_RIGTH		4
#define GO_ROUND		5
#define GO_F_SLOW		6	//����ǰ��
#define GO_WAITING		7
#define GO_SL_ROUND	8
#define GO_LEFT_RD		9	//�����Բ
#define GO_RIGHT_RD		10


#define LEFT_FRONT_IR		2
#define RIGHT_FRONT_IR		4



#define NORM_SIDE_PWM_R		200//200//940//990		����Ǻ�������Ĳ���
#define NORM_SIDE_PWM_L		200//200//940//990  ����Ǻ�������Ĳ���


#define NORM_DUST_PWM		700//650 ����Ǻ�������Ĳ���
#define NORM_MID_PWM		300//750//430//300 750->1000

#define FORCE_DUST_PWM		980//ǿ��ģʽ�µķ��PWMֵ
#define MID_DUST_PWM		500
#define NORMAL_DUST_PWM		100

#define FOCUS_DUST_PWM		920

#if 0
#define DOCK_SIDE_PWM		200		//������ɨ�ı�ˢPWM
#else// modified by wonton2004 20170118
#define DOCK_SIDE_PWM_L		200//964		//������ɨ�ı�ˢPWM
#define DOCK_SIDE_PWM_R		200//950		//������ɨ�ı�ˢPWM

#endif
#define DOCK_DUST_PWM		0
#define DOCK_MID_PWM		300


#define SYS_PWM				700


#define L_FORWRK_PWM		*left_pwm//TIM4->CCR3
//#define L_BACK_PWM			TIM4->CCR4
#define R_FORWRK_PWM		*right_pwm //TIM4->CCR1
//#define R_BACK_PWM			TIM4->CCR2

#define AGITATOR_PWM				(TIM1->CCR4)


#define GO_DOCK_R		0x50

#define LEFT_WHEEL	0x1
#define RIGHT_WHEEL	0x2
#define DOUBLE_WHEEL	(LEFT_WHEEL|RIGHT_WHEEL)
#define NO_DELAY_SET	0x80



#define NO_HW_CALC			0
#define MOTOR_L_HW_CALC	1
#define MOTOR_R_HW_CALC	2


#define GO_FORWARD_SPEED		700


#define MAX_PWM			1000
#define MAX_CAPC_CCR	65500

#define START_PWM			32
//#define  NOMORL_PWM		660		//������PWM
#define  ROUND_PWM			660		//תȦȦ��PWM
#define  TURN_PWM			750		//ת��PWM   660
//#define  SLOW_PWM			750		//�����ϰ����ʱ�򣬵����ߵ�PWM

//#define DOCK_PWM		700

#define DIS_CH1()			(TIM4->CCER &=0xFFFC)		//PA8 PWM��ֹ)
#define DIS_CH2()			(TIM4->CCER &=0xFFCF)
#define DIS_CH3()			(TIM4->CCER &=0xFCFF)
#define DIS_CH4()			(TIM4->CCER &=0xCFFF)

#define EN_CH1()			(TIM4->CCER|=3<<0)//OC1 ���ʹ��)
#define EN_CH2()			(TIM4->CCER|=3<<4)
#define EN_CH3()			(TIM4->CCER|=3<<8)
#define EN_CH4()			(TIM4->CCER|=3<<12)



/**************************��ˢ���*********************************************/
#define MOTOR_SIDE_NORMAL_SPEED			150
#define MOTOR_SIDE_SLOW_SPEED			650
#define DIS_TM1_CH1()			(TIM1>CCER &=0xFFFC)		//PA8 PWM��ֹ)
#define DIS_TM1_CH2()			(TIM1->CCER &=0xFFCF)
#define DIS_TM1_CH3()			(TIM1->CCER &=0xFCFF)
#define DIS_TM1_CH4()			(TIM1->CCER &=0xCFFF)

#define EN_TM1_CH1()			(TIM1->CCER|=3<<0)//OC1 ���ʹ��)
#define EN_TM1_CH2()			(TIM1->CCER|=3<<4)
#define EN_TM1_CH3()			(TIM1->CCER|=3<<8)
#define EN_TM1_CH4()			(TIM1->CCER|=3<<12)

//------------------------------------------modified by wonton2004 2016.12.27
#define DIS_SIDE_MOTER_L()		 //DIS_TM1_CH4()
#define DIS_SIDE_MOTER_R()		 //DIS_TM1_CH2()
#define EN_SIDE_MOTER_L()		 //EN_TM1_CH4()
#define EN_SIDE_MOTER_R()		 //EN_TM1_CH2()
#define SET_LSIDE_MOTER(PWM)		//(TIM1->CCR4 = PWM)		//��ߵ��
#define SET_RSIDE_MOTER(PWM)		//(TIM1->CCR2 = PWM)		//�ұ�ˢ���
#define SET_MID_MOTER(PWM)			(TIM1->CCR4 = PWM)//(TIM2->CCR4 = PWM)
#define SET_DST_MOTER(PWM)			//TIM2->CCR3 = PWM		//�������
//------------------------------------------------------------------------end

extern __IO uint16_t *left_pwm,*right_pwm;



#define STIME	TIM6

#define NOMORL_SPEED		1000


/*
PC4	�м������ײ������	����

PB0	���к�����ײ������	����

PC5	��ߺ�����ײ������	����

PA6	�ұߺ�����ײ������	����
PA7	���к�����ײ������	����

PD11	��߷�ײ����
PE15	�ұ߷�ײ����
*/
#define INDEX_BUM_RIGHT2	6
#define INDEX_BUM_LEFT2		12
#define INDEX_BUM_RIGHT	5////9
#define INDEX_BUM_LEFT	8
#define INDEX_LEFT	 	5
#define INDEX_RIGHT		6
#define INDEX_FW_LEFT	0
#define INDEX_FW_RIGHT	7
#define INDEX_FW_MIDL	4
#define INDEX_DOOR_BUM	1		//�ż�
#define INDEX_DROP_BUM	2		//ǰ������



//��������״̬
#define MASK_LEFT			(1 << INDEX_LEFT)
#define MASK_RIGHT			(1 << INDEX_RIGHT)
#define MASK_FW_LEFT		(1 << INDEX_FW_LEFT)//
#define MASK_FW_RIGHT		(1 << INDEX_FW_RIGHT)
#define MASK_FW_MIDL		(1 << INDEX_FW_MIDL)
#define MASK_BUM_LEFT		(1 << INDEX_BUM_LEFT)
#define MASK_BUM_LEFT2		(1 << INDEX_BUM_LEFT2)
#define MASK_BUM_RIGHT		(1 << INDEX_BUM_RIGHT)
#define MASK_BUM_RIGHT2		(1 << INDEX_BUM_RIGHT2)
#define MASK_BUM_DOOR		(1 << INDEX_DOOR_BUM)		//�ż�
#define MASK_BUM_DROP		(1 << INDEX_DROP_BUM)		//ǰ������

#define MASK_BUM_MIDL		(MASK_BUM_LEFT | MASK_BUM_RIGHT)

#define MASK_CYCLE_RIGHT	(1 << 7)		//�ұ���ת���µ���Ϊ��ײ
#define MASK_CYCLE_LEFT		(1 << 8)

#define MASK_IRD			(MASK_LEFT | MASK_RIGHT| MASK_FW_LEFT| MASK_FW_RIGHT| MASK_FW_MIDL )
#define MASK_BUM_ALL		(MASK_BUM_LEFT | MASK_BUM_RIGHT|MASK_BUM_LEFT2 | MASK_BUM_RIGHT2)

/*
  45��			204
  90��  		408
 135��			612
 180��			816
 */
#define HW_ANGLE_45		204
#define HW_ANGLE_90		408
#define HW_ANGLE_135	612
#define HW_ANGLE_180	816
#define DUST_LOW_POWER		0x01
#define DUST_NORMAL_POWER	0x02
#define DUST_HIGHT_POWER	0x03

typedef struct motor_type
{
	uint32_t total_right_hw;
	uint32_t total_left_hw;
	int c_right_hw;
	int c_left_hw;
	int16_t	c_front_stop;
	//int	c_left_step;
//	int16_t	speed_right;
	//int16_t speed_left;
	/*
	uint32_t t_begin_left;
	uint32_t t_begin_right;
	uint32_t t_end_left;
	uint32_t t_end_right;
	*/
	uint32_t c_front_hw;
//	int		speed_left;
	//int 	speed_right;
	uint32_t stop_motor_judgment;
}motor_t;


extern motor_t motor;

typedef struct h_motor_hal_t
{
	
	int walk_dis;		//�ߵľ���

	uint8_t side;
	uint8_t c_near;				//��ǽ�Ĵ�����Ҫ�����εġ�

	uint8_t	is_bum;				//ֱ�е�ʱ���Ƿ���ײ
	uint8_t	adj_nsta;			//�رߵı�
	uint8_t	adj_c_lost;
	uint8_t c_navi_err;
	uint8_t	force_obst;		//ǿ�����ϰ���
	uint8_t suspend;		//��ͣ���������ڴ档
	int adj_walk_dis;		//�ر��ߵ�·��
	int walk_speed;			//�ƶ��ٶ�
	



	float	to_angle;
	float	angle;			//�Ƕ�

	float	lst_angle;		//������й����ĽǶ�
	float  	out;

	float	now_angle;

	NEAR_WALL	near;
	
	//uint8_t next_y[MAX_GRID];		//��һ��Y����ϰ���
}motor_hal_t;

#define CYC_HW		240

//extern char motorStr[6][20];

//extern int ccr3_1;
//extern int c_hw;//1,c_hw2;

//extern uint32_t m_cHw1,m_cHw2;
void timer1_init(void);
void motor_timer_init(void);
void timer3_pwm_init(u16 psc, u16 arr);
void l_motor_set_pwm(uint8_t HL,int pwm);
void r_motor_set_pwm(uint8_t HL,int pwm);
void timr3_cfg(void);
void r_motor_stop(void);
void l_motor_stop(void);
void motor_run(uint8_t dir ,int pwm,int cHw,uint8_t next);
char * get_motor_sta(uint8_t indx);
void printf_motor_sta(void);
void init_tim4_camp(u16 psc, u16 arr, u8 way, u8 dir);

void rl_motor_stop_v(uint8_t v);

void init_tim5_camp(u16 psc, u16 arr, u8 way, u8 dir);
void tx_timer_cfg(void);
void motor_turn(uint8_t dir,int pwm,float agle);
char proc_line_pid(float set_angle);
//char proc_near_pid(float ird,float n_ird);

void motor_turn_360(uint8_t dir);
void motor_turn_circle(uint8_t dir,int pwm,int hw);
void motor_wheel_forward(u8 wheel,u16 speed);
void motor_wheel_backward(u8 wheel,u16 speed);
void motor_wheel_stop(u8 wheel);
void timer2_init(void);
void motor_turn_dir(uint8_t dir ,int left_pwm,int right_pwm);
char proc_back_pid(float set_angle);
uint16_t get_pwm(uint8_t type);
void judge_left_right_motor_locked_rotor_process(uint8_t type);
void set_mode_and_power_pro(uint8_t sta,uint8_t f_work_mode,uint8_t dust_sensor,FlagStatus Blankets_ground);
void dis_mode_pro(void);

#endif






