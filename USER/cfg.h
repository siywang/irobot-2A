

#ifndef _CFG_H
#define _CFG_H

#define CFG_ADDER	 0x0807F000
// #define CFG_ADDER	0x08032000
#define MAX_CFG_LEN		512//256



#define MIN_CLIFF_SENSOR_VALUE		cfg->cliff_ala_max//2200

#define T_CLIFF_SENSOR_ERR			cfg->cliff_ala_tmr//600000			// 10���ӳ���


#define IC_DOCK_TURN_SPEED			0		//�س�תȦ�ٶ�
#define IC_DOCK_M_WALK_SPEED		1		//�س������ٶ�
#define IC_DOCK_WALK_SPEED			2		//�س��ٶ�
#define IC_DOCK_SET_ANGLE			3
#define IC_AGI_PID					4		//��ˢ�ٶȻ�
#define IC_BUBER_ERR_T				5		//��ײ���Ĺ���ʱ��
#define IC_DUSTBOX_MAX				6		//����������ֵ
#define IC_DUSTBOX_T				7		//������ʱ��
#define IC_DUST_MID				8		//�ҳ�Ũ����
#define IC_DUST_HIGH			9		//�ҳ�Ũ�ȸ�
#define CARPET_L_PRA			10		//���ֵ�̺����
#define CARPET_R_PRA			11		//���ֵ�̺����
#define IC_AGI_AMP_FACTOR		12			//��ɨϵ��
#define IC_LMOT_AMP_FACTOR			13			//����ϵ��
#define IC_RMOT_AMP_FACTOR			14			//����ϵ��

#define IC_FAN_CUR_ERR				15			//�����������澯����
#define IC_FAN_S_LRCUR_ERR			16			//����ģʽ�����ת����
#define IC_FAN_N_LRCUR_ERR			17			//����ģʽ�����ת����
#define IC_FAN_T_LRCUR_ERR			18			//ǿ��ģʽ�����ת����
#define IC_FAN_LRCUR_TMR			19			//�����תʱ���ж���ֵ
#define IC_LED_EN					20			//�Ƿ񿪹ص�

//-------------------------------------------add by wonton2004 @2020.6.2
#define IC_FLOORSENSOR_EN					21			//��̺����Ƿ��
#define IC_DUSTSENSOR_EN					22			//�ҳ�Ũ�ȼ���Ƿ��
#define IC_EDGE_STATUS						23			//�ر߹����Ƿ��,������������һ��,ʵ�ʹ����Ȳ���
//-------------------------------------------add end@2020.6.2





/*
��������
�ٶȲ���
�������
����������
*/
#define DEBUG_NO		0x00
#define DEBUG_PWM		0x01
#define DEBUG_IRD		0x02
#define DEBUG_SES		0x03
#define DEBUG_MOTER		0x04

#define MAGIC 0x55AC
struct cfg_t
{
	uint16_t	magic;

	//�س������Ϣ
	//uint8_t	 dock_stop_step[8];		//ֹͣ��
	uint8_t	 msg;
	uint8_t  crc;
	uint16_t kp1;
	uint16_t ki1;
	uint16_t kd1;
	uint16_t kp2;
	uint16_t ki2;
	uint16_t kd2;

	// 9
	uint16_t ajust_agle;		// 	0
	uint16_t gyro_ajust;		//	 �����ǵĵ���
	uint16_t midle_adc;			//	1 �м����ĺ��ⷶΧ
	uint16_t stop_timr;			//	2 ǰ�����ֺ���󣬼���ǰ�е�ʱ�䡣

	//13
	uint16_t min_left_adc;		//
	uint16_t lock_left_adc;
	uint16_t max_left_adc;
	uint16_t side_left_adc;
	uint16_t lost_left_adc;

	uint16_t min_right_adc;		//8
	uint16_t lock_right_adc;
	uint16_t max_right_adc;
	uint16_t side_right_adc;
	uint16_t lost_right_adc;	// 12

	//23
	uint16_t m_left_adc;		//	13�������
	uint16_t m_right_adc;		//���Һ���

	uint16_t slow_left;			//ת�����Ĳ���
	uint16_t slow_right	;
	int16_t  gyro_ofs1;
	int16_t  gyro_ofs2;

	uint16_t c_pid_near;			//PID�������Ʊ仯�Ĵ���
	uint16_t a_pid_near;			//PID�������Ʊ仯�Ĵ�С


	// 35
	uint16_t l_c_turn;			//��һ��תȦ��ת�������ߵĸ���
	uint16_t dock_l_delay;
	uint16_t dock_l_count;			//ת���������ߵĸ���

	uint16_t r_c_turn;
	uint16_t dock_r_delay;
	uint16_t dock_r_count;			//ת���������ߵĸ���


	uint16_t slow_mid;		//3100
	uint16_t slow_pwm;
	uint16_t turn_pwm;
	//78
	uint16_t walk_pwm;
	uint16_t v6;

	//46
	uint16_t	h_run_speed;		//����
	uint16_t 	m_run_speed;		//����ֱ����ʱ
	uint16_t	l_run_speed;		//����ת�����ߵļ������

	//��ˢ�ٶ�
	uint16_t h_agi_speed;
	uint16_t l_agi_speed;

	uint16_t turn_speed;
	uint16_t nearwall_pwm;
	uint16_t nearwall_hw;

	uint16_t nearwall_lost;
	uint16_t nearwall_cnt;

	uint16_t nearwall_dit;		//�ر߶�����
	uint16_t t_agi_max;			//��ˢ�澯ʱ��
	uint16_t i_agi_max;			//��ˢ�澯����
	uint16_t i_left_whele_max;	//���ָ澯����

	uint16_t t_dust_box_max;		//���ϼ��ʱ��

	uint16_t i_whele_max;		//���ָ澯����
	uint16_t t_whele_max;		//����澯ʱ��

	uint16_t  cliff_max;		//���´���������
	uint16_t  cliff_ala_max;	//�澯����
	uint16_t  cliff_ala_tmr;	//�澯ʱ��,����Ϊ��λ

	uint16_t  t_drop_max;		//������������
	uint16_t  v_dust_max;		//�ҳ��ȷ���ֵ
	uint16_t  v_dust_cnt;		//�ҳ��ȼ���

	uint16_t  v_phts_led;		//�������������
	uint16_t  t_phts_led;		//���������ʱ��

	uint16_t  spot_speed_maxc;		//������ٶ�
	uint16_t  spot_step_disa;		//������Ȧ��
	//69
	uint16_t r[37];//

	uint16_t t_agitator_inspection;	//��ˢ���ʱ��
	uint16_t t_agitator_replacemen;//��ˢ����ʱ��     69*2

	uint16_t steam_clean_id;		//�����ݴ����ID    49 *2
	//84
	uint32_t dust_full_time_alarm;		///3
	uint32_t strainer_use_time_alarm;	///4
	uint32_t side_rush_use_time_alarm;	///5
	uint32_t main_rush_use_time_alarm;	///5 //bytes 102    57 * 2
	uint8_t sn[21];//�������к�,����Ϊ21�ֽ�,��21���ַ�,���һ���ֽ�Ϊ����Ĳ���״̬,0Ϊͨ��,0xffΪδ֪(��δ����),����200���µ�ֵΪ���Բ�ͨ���Ĵ���
	uint8_t uc[1];// 232+24=256

	uint8_t wifi_config_able;			//wifiģ���״�����(���ù���)  	257
	uint8_t dust_pwm_lev;				//����ȼ�					258
	uint8_t speaker_able;				//����ʹ��					259
};

struct robot_msg_t
{
	//uint16_t	magic;
	uint8_t	 	msg;
	uint8_t 	crc;
	uint16_t	data[62];

};

#define SYS_DEBUG_LEN	30
struct sys_debug_t
{
	uint8_t		msg;
	uint8_t		crc;
	uint16_t 	imu;
	uint16_t 	gyro;
	uint16_t	pid1;
	uint16_t	r[12];

	uint16_t 	c_gyro;
	uint16_t 	c_imu;
	uint16_t	c_pid1;
	uint16_t	c_r[10];
	uint8_t		debug;
	uint8_t 	pwm;

};

struct h_cfg_t
{

	uint16_t magic;
	uint8_t	msg;
	uint8_t crc;
	uint16_t dock_hw_back;			//��ͷ�Ĳ���
	uint16_t dock_hw_forwak;			//ǰ��Ĳ���
	uint16_t dock_hw_turn;			//ת�����Ĳ���
	uint16_t dock_pwm;
	uint16_t dock_m_pwm;
	uint16_t dock_speed;
	uint16_t pwm_normal;
	uint16_t pwm_slow;
	uint16_t speed_normal;
};
extern struct cfg_t *cfg;
extern uint16_t NOMORL_PWM,SLOW_PWM;
extern char cfg_buf[];

extern struct sys_debug_t sys_debug;

void read_Flashbuf(u32 addr,u8 *p,u16 n);


void save_cfg(void);
void init_cfg(void);
void reset_cfg(void);
void proc_rx_uart(uint8_t *buff,int len);

#endif
