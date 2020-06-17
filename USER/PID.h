#ifndef _PID_H
#define _PID_H


#define NEAR_PID_NEW		1

#define TURN_SPEED					cfg->turn_speed//20    //20
#define CHARGE_TURN_SPEED			(cfg->r[IC_DOCK_TURN_SPEED] / 100)		//30		//�س�ת�٣���λ  7695 = 1E0F ,��λ 1E = 30 ��λ 0F = 15
#define CHARGE_TURN_SPEED_DELAY		(cfg->r[IC_DOCK_TURN_SPEED] %100)//15	//�س�ת�٣���λ



#define SINGLE_NAER_PID		cfg->nearwall_cnt//20


//#define EN_PID		1

//#define PID_DEBUG	0


/*
#define BLDC_MAX_SPEED   4096	 //800 3500
#define BLDC_MIN_SPEED   1
#define PWM_Max			1400
#define PWM_Min     	10
*/
//���ԣ����ڵ��ٲ��ԣ����˼����˰���
#define BLDC_MAX_SPEED   80	 //800 3500
#define BLDC_MIN_SPEED   1
#define PWM_Max			200
#define PWM_Min     	10




typedef struct near_pid_t
{
	//unsigned char 	sta;		//״̬ 0 - ���ӱ� 1-��
	unsigned char		level;		//�ӱߵȼ�
	unsigned char 	obst;		//�����Ƿ����ϰ���������ϰ���Ļ����ӱ߶�ʧ��������PID
	unsigned char 	dist_ok;	//�����������Ҫ��

	uint8_t 		debug;

	unsigned short	min_adc;
	unsigned short	max_adc;
	unsigned short	max_adc2;
	unsigned short	lock_adc;

	unsigned short 	real_lock;
	unsigned short    real_max;

	unsigned short	last_adc;

	unsigned short	count;
	//unsigned short 	c_near;		//�رߵĴ���
	//unsigned short 	c_near2;
	unsigned short 	c_lost;		//����ʧȥǽ��ʱ��
	unsigned short	c_dis_a;	//�Ƕȼ��������ʱ��
	//unsigned short	max_lost;
//	unsigned short	c_angle;	//�Ƕȵ��б�
	unsigned short	c_big_change;
	unsigned short 	dist;

	unsigned short	c_lost_flag; 	//ʧȥǽ�ı�־    2018-11-03 add

	unsigned short	small_dist;		//�����ڼ��С����

	unsigned short 	*adc;		//������
	unsigned short	*adc2;		//����ڶ�������
	float		l_angle;	//��ʧʱ��ĽǶ�
	float		a_go_outside;	//�����ߵĽǶ�
	float		a_go_inside;	//�����ߵĽǶ�
	float 		*yaw;			//ϵͳ�Ƕ�
	float		*angle;			//Ŀ��Ƕ�
	uint16_t    *c_pid_near;
	uint16_t 	*a_pid_near;
	uint16_t	*nearwall_lost;
	uint8_t 	*nearwall_signal_lost;
	int			*distance;
	uint16_t 	*c_near_a;
	uint16_t	*max_near_a;
	float 	*yaw_buff;
	

}NEAR_PID;



extern 	NEAR_PID  r_near_pid,l_near_pid;

typedef struct _pid_t{
	uint8_t debug;
	uint8_t r1;
	float SetSpeed;			
	float ActualSpeed;		//����ʵ��ֵ
	float err; 				//����ƫ��ֵ
	float err_next;	//������һ��ƫ��ֵ
	float integral;
	float err_last;//��������ǰ��ƫ��ֵ
	float Kp,Ki,Kd;	//������������֡�΢��ϵ��
	float out;
	int16_t  *max_pwm;
	int16_t  *min_pwm;
	int		*rpwm;
	int		*lpwm;
	int		*pwm;
	int 	*setminpwm;
	int		*setmaxpwm;
	
	//float angle;
//	float max;
	//int	  c_pid;
}pid_t;


extern  pid_t rpid,*pid;

//extern pid_t lpid,rpid;

void pd_gyro_int(int out);
void proc_hw_pid(void);


int navigat_near_wall_pid(float *agle,int c_lost);

int near_wall_pid(unsigned char nsta,NEAR_PID *npid,float *agle);
void init_near_pid(char level);
void turn_round_pid(int sta);

//#if JUDGE_PID_CTRL_WHEEL_STOP
//void m_speed_pid_ctrl(unsigned short speed,unsigned char type,int hw,int c_left_hw_min,unsigned char ird_state,int hw_min,int walk_dist_min);
//#else
void m_speed_pid_ctrl(unsigned short speed,unsigned char type);
//#endif
void charge_turn_pid(int sta);
int  m_nearwall_pid(void);
void init_nearpid_differ(int pwm,unsigned short speed,unsigned char msta);
signed short dock_speed_pid_ctrl(unsigned short min,unsigned short max,unsigned char type);
void single_whele_pid(int rspeed,int chw,volatile uint16_t *whele_pwm);
char proc_nearwall_pid(float set_angle,uint8_t nside);

void m_speed_pid_indep(uint16_t lspeed,uint16_t rspeed,uint8_t type);
#endif
