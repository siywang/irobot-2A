#ifndef _ICLEAN_H
#define _ICLEAN_H



#define PRINTF		log_printf


// #define CFG_ADDER	 0x08019000

//ϵͳ����״̬��sys->sSta
#define SYS_IDLE		0x00		//����
#define SYS_NAVIGAT		0x01		//����
#define SYS_FOCUS		0x02		//�ص�
#define SYS_RANDOM		0x03		//���
#define SYS_DOCK		0x04		//�س�
#define SYS_NEAR		0x05//�ر�
#define SYS_CHARGE		0x06		//���
#define SYS_TEST_BD		0x07		//?�¡�?2a��??�꨺?
#define SYS_POWEROFF	0xfd	//1??��,��?���䨬?????��???��?o����a(��??��?��1.2��?��2?t��?��????T����1?��?)/����???����?,????��?2?1?��?
#define SYS_TEST_SEFT	0x08		//?�¡�?2a��??�꨺?
#define SYS_WALK		0x0A       //ֱ��
#define SYS_DOCK_NEAR	0x0B		//�س��ӱ�





//�˶�״̬
//#define ROBOT_MOVE_STA() ((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS) || (sys->sState==SYS_DOCK)||(sys->sState==SYS_NEAR))
//������������ģʽ,��̨�ܲ��Ե�ʱ��Ҫ�õ�
#define ROBOT_MOVE_STA() ((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS) || (sys->sState==SYS_DOCK)||(sys->sState==SYS_NEAR)||(sys->sState==SYS_RANDOM)) // modified by wonton2004 20170603

#define CLEAN_RANDOM	0x00
#define CLEAN_NEAR		0x01	//�ر�ģʽ
#define CLEAN_ROUND		0x02	//����ģʽ

#define MAX_IRDA 		8

#define DOCK_NO			0x00
#define DOCK_CHECK		0x01


#define RET_CHECKING	0x00
#define RET_CHECK_OK	0x01
#define RET_CHECK_FAL	0x03



#define FIND_MID_FLAG		0x1
#define FIND_MID_R_FLAG		0x2
#define FIND_MID_L_FLAG		0x4
#define FIND_LL_FLAG		0x8
#define FIND_RR_FLAG		0x10
#define FIND_NEAR_FLAG		0x20
#define FIND_DOC_BASE		0x40
#define FIND_L_M_FLAG		0x100
#define FIND_R_M_FLAG		0x200
#define FIND_ALL_FLAG		(FIND_MID_FLAG|FIND_MID_R_FLAG|FIND_MID_L_FLAG|FIND_LL_FLAG|FIND_RR_FLAG)



//�����������
#if 0
#define MWERR_NONE			0
#define MWERR_CLRDUSTBOX	1//����༯������������
#define MWERR_MIDMOTOR		2//������ɨ�Ƿ����
#define MWERR_DUSTBOX		3//���鼯�����Ƿ���ȷ��װ
#define MWERR_WHEELSUSP		4//�����������Ƿ����
#define MWERR_WHEELBIND		5//�����������Ƿ����
#define MWERR_FLRSENSOR		6//��������Ӧ��
#define MWERR_SIDEMOTOR		7//�����ɨ�Ƿ����
#define MWERR_FRTMOTOR		8//����ǰ���Ƿ����
#define MWERR_POWEROFF		9//�����Դ�����Ƿ��
#define MWERR_DISPSTA		10///����ͷ����
#define MWERR_ESCAPE		11		//����ʧ��
#else
//�����������
#define ERR_NONE									0
#define ERR_MIDMOTOR								1//������ɨ�Ƿ����				-ok
#define ERR_LEFT_WHEELBIND							2//���������Ƿ����				-ok
#define ERR_RIGH_WHEELBIND							3//���������Ƿ����				-ok
#define ERR_DUSTBOX									4//���鼯�����Ƿ���ȷ��װ		-ok
#define ERR_FAN_FILTER_BLOCKAGE						5//���������ס.					--need add3
#define ERR_BUMPER_FUNCTION_ERR						6//��ײ����						-1
#define ERR_FLRSENSOR								7//��������Ӧ��				-ok
#define ERR_WHEELSUSP								8//�����������Ƿ����			-ok
#define ERR_CANT_ESCAPE_WITHIN_TIMES				9//��������						-ok
#define ERR_SUCTION_MOTOR_MALFUNCTIONG				10//�����ת 3���ֱ�����ܴ�		--need add3
#define ERR_CLRDUSTBOX								11//������� ʱ���ۻ�				--1
#define ERR_AGITATOR_INSPECTIONG					12//����ˢ						--need add3
#define ERR_AGITATOR_REPLACEMENT					13//������ˢ ʱ���ۻ�				--1

#define MWERR_NONE									ERR_NONE
#define MWERR_MIDMOTOR								(BIT_STA(ERR_MIDMOTOR-1))		//������ɨ�Ƿ����					-ok
#define MWERR_LEFT_WHEELBIND						(BIT_STA(ERR_LEFT_WHEELBIND-1))//2//���������Ƿ����				-ok
#define MWERR_RIGH_WHEELBIND						(BIT_STA(ERR_RIGH_WHEELBIND-1))//3//���������Ƿ����				-ok
#define MWERR_DUSTBOX								(BIT_STA(ERR_DUSTBOX-1))//4//���鼯�����Ƿ���ȷ��װ					-ok
#define MWERR_FAN_FILTER_BLOCKAGE					(BIT_STA(ERR_FAN_FILTER_BLOCKAGE-1))//5//���������ס.						--need add3
#define MWERR_BUMPER_FUNCTION_ERR					(BIT_STA(ERR_BUMPER_FUNCTION_ERR-1))//6//��ײ����					-ok
#define MWERR_FLRSENSOR								(BIT_STA(ERR_FLRSENSOR-1))//7//��������Ӧ��								-no
#define MWERR_WHEELSUSP								(BIT_STA(ERR_WHEELSUSP-1))//8//�����������Ƿ����					-ok
#define MWERR_CANT_ESCAPE_WITHIN_TIMES				(BIT_STA(ERR_CANT_ESCAPE_WITHIN_TIMES-1))//9//��������				-ok
#define MWERR_SUCTION_MOTOR_MALFUNCTIONG			(BIT_STA(ERR_SUCTION_MOTOR_MALFUNCTIONG-1))//10//�����ת 3���ֱ�����ܴ�		-no
#define MWERR_CLRDUSTBOX							(BIT_STA(ERR_CLRDUSTBOX-1))//11//������� ʱ���ۻ�							--1
#define MWERR_AGITATOR_INSPECTIONG					(BIT_STA(ERR_AGITATOR_INSPECTIONG-1))//12//����ˢ					-ok
#define MWERR_AGITATOR_REPLACEMENT					(BIT_STA(ERR_AGITATOR_REPLACEMENT-1))//13//������ˢ ʱ���ۻ�					--1


#endif
/*
struct irda_t{

	uint8_t freq;		//Ƶ��
	uint8_t indx;
	uint8_t data[8];
};


struct r_ird_t
{
 	uint32_t t_last;  //����ж�ʱ�䡣��
	uint8_t rx_idx1;
	uint8_t rx_rbe1;
	uint8_t rx_byte1;
	uint8_t l_360;
	uint8_t left;
	uint8_t right;
	uint8_t r_360;

};
*/
#define CONFIG_SIZE		14
struct config_t
{
	//�س������Ϣ
	uint8_t	 stop_step[8];		//ֹͣ��
	uint16_t hw_back;			//��ͷ�Ĳ���
	uint16_t hw_forwak;			//ǰ��Ĳ���
	uint16_t hw_turn;			//ת�����Ĳ���

};
#define MAX_NEAR_ANGLE		100

extern struct r_ird_t iLeft,iRight,iL360;
#define MAX_ADC_C		9

//�ӱ�PID�㷨
/*
typedef struct near_wall_type
{
	uint8_t c_lost_wall_right;
	uint8_t	c_near_wall_right;
	uint8_t c_lost_wall_left;
	uint8_t	c_near_wall_left;
}near_wall_t;
*/
#define WORK_STA_ERR		(2)
#define WORK_STA_SILENT		2
#define WORK_STA_WIFI		3

#define WORK_MOD_DOOR		(1)
//#define WORK_MOD_ORDER		(1<<1)
#define WORK_MOD_MOP		(1<<2)
#define WORK_MOD_ORDER		(1<<7)  //ԤԼ

#define NO_CHARGE			0
#define AC_CHARGE			1
#define DOCK_CHARGE			2
#define CHARG_DONE			4


#define STACK_LEN		4096
#if NEAR_DRAW_MAP
	#define STACK_LEN2		256
#else
	#define STACK_LEN2		4//4096
#endif



extern uint8_t m_stack[STACK_LEN];
extern uint8_t m_stack2[STACK_LEN2];			//�ֲ�ʹ��
//----------------------------------------------------------------------add by wonton2004 20170115
//work_mod
#define MWO_SILENT		0x01	//�׵�
#define MWO_NORMAL		0x02	//�Զ���
#define MWO_TURBO		0x04	 //�ߵ�
#define MWO_MANUAL		0x08	//�ֶ���
#define MWO_FULL2GO		0x10	//FULL AND GO
#define MWO_SPOT		0x20	//������ɨ
#define MWO_SCH			0x80	//ԤԼ��ɨ���
#define DUST_LOW 			0x00	//������
#define DUST_MEDIUM 		0x01	//������
#define DUST_HIGH 			0x02	//������

#define MSINFO_DUSTFULL	0x01
#define MSINFO_ERROR	0x02
#define MSINFO_MUTE		0x04	//����
#define MSINFO_DCJACK	0x10//DCͷ����/�ֶ����
#define MSINFO_CON_WIFI		0x80	//WIFI����״̬
#define MSINFO_CON_WIFI2	0x40	//WIFI����״̬2
#define MSINFO_WIFI			0x08	//WIFI����״̬

//--------------------------------------------------------------------------------------------end

#define ERR_STA_PICK_UP		(1<<0)		//


#define BUM_V_WALL			(1<<0)		//����ǽ
#define BUM_DOOR			(1<<1)		//�ż�

#define LEFT_SIDE_FALL		1
#define RIGHT_SIDE_FALL		2

#define MAX_YAW_FIFO		1000
#define MAX_FRONT_HW		20

#define ERP_LEN			12

#define WIFI_STA_STANDBY		0x00
#define WIFI_STA_AUTO			0x01
#define WIFI_STA_SPOT			0x02
#define WIFI_STA_PAUSE			0x04
#define WIFI_STA_FULL2GO		0x08
#define WIFI_STA_FINISH			0x10
#define WIFI_STA_CHARGE			0x20


struct sys_t
{
	//�洢������
	uint32_t 	t_area;		//�����,��λ:ƽ����
	uint32_t	t_clean;	//����ɨʱ��,��λ:����

	uint16_t 	m_area;		//�������,��λ��ƽ����
	uint16_t	m_worktime;//���ε���ɨʱ��,��λ�Ƿ���
#if (1 == SIMPLIFY)
	unsigned char test_line_go_ctrl;
#endif
	FunctionalState dccheck_enable;
	uint8_t	sState;		//ϵͳ״̫
 	uint8_t 	mState;		//���״̬
 	uint8_t 	auto_sta;	//auto ģʽ��״̬ mpu6500
 	uint8_t voice;			//�����Ƿ�
 	uint8_t	sState_auxi;		//����sState״̬�ں����ڲ����ж�ʱ��仯����������һ������״̬������ʱ��仯�����sState_auxi�����λΪ1,����λΪϵͳ״̬

	uint8_t	wifi_sta;		//WIFI״̬
	
 	uint8_t	spot_flag;
 	uint8_t	pause;			//��ͣ�ı�־
 	////uint8_t wifiAreaNoCalc_en;
	uint8_t 	dust_level;
	uint8_t 	gyro_ofs;
	uint8_t		wifi;
	FlagStatus wifi_switch;		//Wi-Fiģ�鿪��
	FlagStatus wifi_switch_old;		//Wi-Fiģ�鿪��(��)
	uint8_t 	silent;
	uint8_t	power_on;
	uint8_t power_go;
	uint8_t charge_sta;
	uint8_t	dock_near;
	uint8_t fall_sta;		//����״̬
	uint8_t side_bum;		//������ײ��λ
	uint8_t actionwifi;		//wifi ������Ӧ
	uint8_t  err_sta;
	uint8_t	bum_sta;		//��ײ��״̬
	uint8_t walk_bum;		//��ײ
	uint8_t nearwall_signal_lost;
	uint16_t slipping_state;
	uint8_t shut_down_motor;		//????
	uint8_t	navi_run;		//������ɨ
	//������Ϣ
	uint8_t	near_debug;
#if PID_DEBUG
	uint8_t	walk_pid_debug;
#endif

/*
bit2	ʪ��
bit1	ԤԼ
bit0	�ż�
*/
	uint8_t 	f_work_mode;	//����ģʽ  	���� ���� ǿ�� ������ɨ �������� �ֶ�
	uint8_t 	work_mod;		//����ģʽ
	uint8_t 	up_work_mod;	//��ǰ����ģʽ	����,����,ǿ�� �������� �ֶ�
	uint8_t		work_tmr;		//����ʱ�䣬���ӵ�λ
	uint8_t 	full2go;
	uint8_t 	dust_sensor;	//���촫����
	uint8_t 	tSta;		//���Ե�״̬
	uint8_t 	gey_key_clear_err;		///������������־
	uint8_t 	pair_sta;

//----------------------------------------------------------------------add by wonton2004 20170115
	uint8_t work_options;//����ѡ�� �ż�ģʽ/ԤԼ��ɨ/ǿ��ģʽ/Ĩ��/��ɨ��ɱ�ǵ�,���� MWO_xxx
	uint16_t work_errcode;//����������� ����ɨ���ߵ�,���� MWERR_xxx
	uint8_t main_rush_usetime_alarm;
	uint16_t dust_pwm_value;//�����PWMֵ,������ǿ��ģʽ,�û���Ҫ��ʱ���������ת��

	uint8_t wifi_led_enable;
//--------------------------------------------------------------------------------------------end
	uint16_t dust_full_check_tmr_min;
	uint32_t strainer_check_tmr_min;
	uint32_t side_rush_check_tmr_min;
	uint32_t main_rush_check_tmr_min;
	uint8_t work_alarmcode;//�Ĳĸ澯
	uint8_t dust_pwm_lev;	//����ȼ�
	uint16_t run_speed;
	uint16_t agi_speed;		//��ˢ�ٶ�

	uint16_t c_escape ;		//��������

	uint32_t t_beep_on;		//�澯�ĺ�����
	uint32_t t_beep_off;

	uint32_t t_flash_halt;		//��ͣ����˸
	uint32_t t_halt_on;

//----------------------------------------------------------------------add by wonton2004 20170913
#if ROOMA_GYROONLY
 	uint8_t		manState;	//ң�����ֶ�����ģʽ�µ�״̬
	uint8_t 	man_worktype;//�ֶ�����ģʽ�µļ�ʱ������ʽ,Ϊ0ʱ��man_typetimerΪ��������ʱ,Ϊ1ʱ��ʾ������ʱ

 	uint32_t	man_typetimer;//�ֶ�����ģʽʱ�ĺ��������,Ϊ0xffffffffʱ��ʾ����ʱ,����man_typetimer����200ʱ��������
 							//���Ϊ-1ʱ��ʾ������ʱ
 							//����ң��ʱ,�ó�ʱģʽ,���������,����������Ԥ��Ϊ0
 							//wifiң��ʱ,��������дΪ-1
 	uint32_t	c_cliff_sensor_err;
 	uint32_t	t_cliff_sensor_err;

	uint32_t	sync_loop;//wifiģʽ�µ�ʱ��ͬ��������,ÿ24Сʱͬ��һ��ʱ��

	uint32_t	smt_l_tmout;//���ˢ������ļ�ʱ��,����ʱ��,��ˢ�ָ�����
	uint32_t	smt_r_tmout;//�ұ�ˢ������ļ�ʱ��,����ʱ��,��ˢ�ָ�����

	uint8_t	sync_flg;//wifiģʽ�µ�ʱ��ͬ���ı�־,����һ������ͬ����ʶ,����ʱ��0,WIFI������,ͬ��һ��ʱ�䲢��ֵ��1
					//֮��ÿ��24Сʱͬ��һ��ʱ��
	uint8_t charge_done_clean_goon;	//������������ɨ

 	//uint16_t	sWorkTimeMinCt;//����ʱ��(��Ϊ��)�ļ���
 	uint16_t	sWorkTimeMin;//����ʱ��(��Ϊ����),�ڷǳ��/����/�ػ��س��״̬�µĹ���ʱ��,���Զ�/����/�����
 	//uint16_t	sSleepTimeMin;//˯��ʱ��
#endif
//--------------------------------------------------------------------------------------------end
		uint16_t 	kdis_yaw;
	uint16_t 	cdis_yaw;
	uint8_t near_wall;	//
	uint8_t slip;
	//uint8_t	from_bum;		//����ײת������
	uint8_t walk_sta;		//�Ƿ��߳ɹ�

	uint8_t left_dir;
	uint8_t	walk_ctrl;		//�˶����ƣ�ң�����������ߣ�0����������

	uint8_t nsta;

	//uint8_t low_power;		//�͵�ѹ

	uint16_t	timeout_navi_work;
	uint32_t 	t_navi_work;	//������ʱ�䣬����Ϊ��λ




	uint32_t	g_t_walk;
	//	uint16_t 	kdis_yaw;
	//uint16_t 	cdis_yaw;

	//uint8_t 	gIO_ird;

	uint16_t	left_pwm;
	int16_t 	right_pwm;

	uint16_t	c_loop;
	uint16_t 	c_bum;		//��ײ�Ĵ���
	uint16_t	lst_distance;
	uint16_t	c_low_power;
	uint16_t	c_charge_power;

	uint16_t 	c_go_bum;
	uint16_t	c_walk_drop;
//	uint16_t	lst_distance;
	uint16_t	tv1;		// ���Բ���1
	uint16_t	tv2;		//���Բ���2

	uint16_t 	gSta;		//��ײ��״̬
	uint16_t	gsta;		//���ݵ�״̬
	uint16_t	g_sta[10];	//ǰײ�����ֵ
	uint16_t	m_sta[10][MAX_ADC_C];	//����
	uint16_t	volt;	//��ѹ
	uint16_t 	prs;	//�����ٷֱ�
	//uint16_t 	current;	//����
	uint16_t 	ir_front_1;	//��1 ԭʼֵ
	uint16_t	ir_front_3;//��2 ԭʼֵ
	uint16_t	g_buton[2][3];	// g_buton[0][0~3];�������  g_buton[1][0~3] �ż�

	uint16_t c_right_drop;
	uint16_t c_left_drop;
	//int16_t 	gyro_ofs;

	unsigned short ground_ul;

	int16_t  max_pwm;
	int16_t  min_pwm;
	int16_t	 max_front_stop;

	int16_t 	i_c_near;
	uint16_t 	c_near;
	uint8_t 	dist_ok;
	uint8_t		small_dist_ok;

	uint8_t		init_prs;

	//uint8_t c_lost_wall_right;
//	uint8_t	c_near_wall_right;
	//uint8_t c_lost_wall_left;
	//uint8_t	c_near_wall_left;
	// uint8_t	c_lost_wall_right	;
	// uint8_t	c_near_wall_right 	;
	//	uint8_t c_lost_wall_left;

	//��� �㷨
	//uint8_t    dock_discovery; 		//


	uint8_t		g_fall_sta;//�����״̬,bit0Ϊǰ�� bit1��� bit2�ұ�
	//uint8_t	near_pid;		//�Ƿ�����ǽ��
	uint8_t	c_ir_adc;			//adc����
	uint8_t	motor_power_off;
    uint8_t t_adj_run;

	


	//uint8_t 	lStopSta;		//���ʱֹͣ��״̬
		uint8_t		auto_lamp_sta;	//����Կص�״̬
//	uint8_t	c_near_wall_right;
	//uint8_t	c_near_wall_left;
	//uint16_t	c_load;
	//uint16_t  c_load2;
	uint16_t  chw_next;
	uint16_t  gyro_stop;
	uint16_t  c_pid_hw;
	uint16_t  c_front_stop;
	uint16_t  c_front_hw;
	uint16_t  front_hw[MAX_FRONT_HW];

	int16_t		gyro_short;		//������ԭʼ����

	//uint8_t	needStop;

	uint8_t	c_hw_diff;	//��������֮��Ĳ�����
	//uint8_t	caleHW;		//�Ƿ�Ҫ��������� 0:�����������1:���1(����)���㣬2�����2����

	//int		trg_speed;		//Ŀ���ٶ�
	//int		trg_speed2;		//Ŀ���ٶ�
	uint32_t 	c_hw;//1=0,c_hw2=0;		//

	int		nextPWM;			//����������һ��������ʱ������õ���
	int		next_c_hw;			//����������һ��������ʱ������õ���

	int		pwm1;
	int 		pwm2;
	int		rpwm,lpwm,pwm;

	int setminpwm,setmaxpwm;
	uint16_t	c_left;
	uint16_t  c_right;
	//uint16_t	c_near_angle;

	uint16_t 	c_turn_navi;//mpu6500
	uint16_t c_ajust_agle;
	uint16_t c_fw_rd;		//�м䴫�������ֵ�ĸ���
	uint32_t t_loop;
	uint16_t	c_yaw;

	uint16_t  left_speed;		//��¼�����ٶ�
	uint16_t  right_speed;
	uint16_t  turn_speed;


	volatile uint32_t 	m_cHw1;
	volatile uint32_t	m_cHw2;
	uint32_t	m_speed1;
	uint32_t  m_speed2;
	//uint32_t	tStop;			//�м���ݵ���ͣʱ��;
	//uint32_t	t_ss;
	uint32_t	irda[9];
	uint32_t 	tird;
	uint32_t	timeout;
	uint32_t 	t_gyro_ofst;		//�����ǳ�ʼ����ʱ�䳬ʱ
	uint32_t	t_idle;				//���е�ʱ�䣬����ʱ��̫������غ���
	int 	m_dist;		//��¼��ʼ����ʱ�ľ���


	uint32_t	adjust_gyro_time;	///����������ʱ��

	//int 	cross_yaw;			//��ʱ����
	uint16_t c_yaw_ok1;//MPU6500 mjw 20191129
	uint8_t sensens_err;	///�����������־
	uint32_t sys_charge_cur;	//ϵͳ������

	

	uint32_t	t_work;		//��ʼ������ʱ��
	int16_t	gyro_offset;
	float   langle;
	float	yaw;
	float	angle;
	float	gyro;
	float lspeed,rspeed;
	float	ajust_agle;		//ÿ�������ĽǶ�
	float  total_ajust_agle;
//	float	yaw_ofset;			//�ǶȽ���
//	float 	yaw_fifo[MAX_YAW_FIFO];

	//struct cfg_t	*m_cfg;
//	navigat_t		*m_navi;
	//near_wall_t	near_wall;
	//float  last_near_angle;

//	float	tyaw;

	uint8_t upmap_en_flag;		//�ϱ���ͼʹ�ܱ�־
	uint8_t uphismap_en_flag;	//�ϱ���ͼʹ�ܱ�־
	uint8_t upmap_en_flag_first;	//�����ϱ�ʵʱ��ʷ��ͼʹ�ܱ�־
	uint8_t clean_map_flag;		//�����ͼ��־

	uint8_t sync_wifi;			//wifiʱ��ͬ��
	uint8_t dis_statechange;	//�Կ�״̬�仯
	uint8_t wifistatechange;	//WIFI״̬�仯
	uint8_t wifi_config_state;	//WIFI����ģʽ
	uint8_t wifi_state;			//WIFI����״̬
	uint8_t wifi_old_state;
	uint8_t wifiAreaNoCalc_en;
	uint32_t wifistatechange_tmr;	//WIFI״̬�ı�ʱ��

	uint8_t dusk_usetime_alarm;			//����ʹ��ʱ�䵽�˸澯
	uint8_t strainer_usetime_clear;		//����ʹ��ʱ�䵽�˸澯
	uint8_t side_rush_usetime_clear;			//��ˣʹ��ʱ�䵽�˸澯
	uint8_t main_rush_usetime_clear;		//��ˢʹ��ʱ�䵽�˸澯

	uint16_t 	agi_curr;
	uint16_t	left_curr;
	uint16_t	right_curr;
	uint16_t	i_whele_cnt;
	uint32_t	i_left_whele;
	uint32_t 	i_right_whele;

	uint32_t 	t_agitator_inspection;
	uint32_t 	t_agitator_replacemen;

	uint8_t gyro_init_ok;
	////uint32_t wifistatechange_tmr;	//WIFI״̬�ı�ʱ��
//	uint8_t stream_state;
//�����ж��������Ƿ��ת��ر���
	uint8_t motor_primary_locked;	//�������ס�˱�־

	uint8_t motor_locked_flag;	//�����ת��־


	FlagStatus upgrade_mode;	//����ģʽ

	FlagStatus Blankets_ground;		//ë̺����

		FlagStatus wifi_time_sync;
	float dis_yaw[5];
	float vleft;
	float vright;
	float vagi;
};

struct sta_bump_t
{
	unsigned int sec_bum_execute;
	FlagStatus sta_bum_keep;
	FlagStatus sta_bump_op;
};
extern struct sta_bump_t g_sta_bump;


extern struct sys_t *sys;
extern gyro_t kgyro;

//extern struct irda_t  rx_irda,rx_irda1,rx_irda8;

void init_sys(void);
char check_io(uint16_t valu);
char check_io_h(uint16_t valu);

//void insert_irda(uint16_t ird);
void printf_irda(void);

int disXY(int x,int y);
void en_pwm(uint8_t chnl,uint8_t en);
void send_irda(uint8_t chnl,uint16_t bt);
void init_tim3_camp(void);
 //uint8_t  dock_check_dir(void);

void  docking_init(void);
void docking(void);
//void insert_irda1(uint16_t ird);
void rx_ird(struct r_ird_t *rx_ird);

void waiting_stop(void);
//void get_sensers(uint16_t *g_sta);

void init_near_wall(void);
void near_wall_mode(uint8_t gSta);

void irda_timer_cfg(void);
void infra_sensers(uint16_t *g_sta);
void set_walk_angle(uint8_t dir,float agle);
void charging_turn_round(void);
float disf_xy(float x,float y);

int find_big_count( int *n ,int nNum );
void init_sys_status(uint8_t sta,uint8_t old_sta);
void robot_run_ctrl(void);
void init_robt_run_ctrl(int deg,int ctrl);
int dis_xy(short x,short y);
uint8_t in_area(int16_t x,int16_t bx,int16_t ex);
void init_gyro_g(void);

#if ROOMA_GYROONLY
uint8_t sys_state_convert2wifi(void);
uint8_t wifi_state_convert2sys(uint8_t wifi_mode);

extern uint16_t sWorkTimeMinCt,sWorkTimeMin,sSleepTimeMin;
#endif

#endif
