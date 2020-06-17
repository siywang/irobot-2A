
#include "sys.h"

char pid_printf=0;


#define MAX_N_ANGLE		100

float 		near_angle[MAX_N_ANGLE];
uint16_t	c_near_a=0,max_near_a=0;

int round_pid_calce(pid_t *pid,float set_speed,float now_speed)
{
	float incrementSpeed;
	float	kp,kd,ki;
//	float max,min;
	pid->SetSpeed	= set_speed;
	pid->ActualSpeed = now_speed;
#if R_PID_DEBUG
	log_printf("%3.1f,",now_speed);
#endif
	pid->err			= pid->SetSpeed-pid->ActualSpeed;

	pid->integral +=pid->err;
	kp = pid->Kp * pid->err;
	kd = pid->Kd * (pid->err-pid->err_next);
	ki = pid->Ki * pid->integral;

	incrementSpeed	=  kp+ ki + kd;
#if R_PID_DEBUG
	log_printf("%3.1f,%3.1f,%3.1f,%3.1f\r\n ",kp,ki,kd,incrementSpeed);
#endif

	if(incrementSpeed < -300)
		incrementSpeed = -300;
	if(incrementSpeed > 300)
		incrementSpeed = 300;
	pid->out			+= incrementSpeed;
	sys->lpwm = sys->pwm  + incrementSpeed;
	sys->rpwm = sys->pwm  - incrementSpeed;
	if(sys->lpwm >=1000)
		sys->lpwm = 999;
	if(sys->lpwm <=50)
		sys->lpwm =50;

	if(sys->rpwm >=1000)
		sys->rpwm = 999;
	if(sys->rpwm <=50)
		sys->rpwm =50;
#if R_PID_DEBUG
//	log_printf(",%5.1f\r\n",pid->out);
#endif
	if(pid->out >=1000)
		pid->out=999;
	if(pid->out < 30)
		pid->out=30;
	pid->err_last	= pid->err_next;
	pid->err_next	= pid->err;
//	log_printf("%3.1f,%3.1f\r\n",now_speed,incrementSpeed);
	return (int)pid->out;
}



#define AJUST_AGLE   	0.2
#define AJUST_AGLE_2	0.2
#define AJUST_AGLE_BIG   	0.5

#define AJUST_AGLE_LOST	2.0f

#define OUT_IDR			950
#define MAX_IN_1		3500
#define MAX_IN_2		2100


#define MAX_NEAR_COUNT	3



float angle_buf[]={0.0f,90.0f,180.0f,270.0f};
uint16_t bf_angle[]={0,90,180,270};


NEAR_PID  r_near_pid,l_near_pid;
/*
level �ӱߵȼ����ȼ�Խ�ͣ�Խ�ϣ�Խ�ߣ�Խ��
level =0 �������ӱ�
level =1 adj�ӱ�
*/
	/*

#define MAX_N_ANGLE		140
#define MAX_N_DIST		1800
#define ANGLE_ADJ_PST	20

*/


#define MAX_N_DIST		1200			///�ӱ߽�������  KWS:2500    ������1200
#define ANGLE_ADJ_PST	25
#define MAX_N_SMALL_DIST		200		///�ӱ��ڼ���С�Ľ�������


float 		near_angle[MAX_N_ANGLE];
//uint16_t	c_near_a=0;
//uint16_t 	c_near_a;

#define ADJ_ANGLE_P		0.3f
void init_near_pid(char level)
{
	//�ӱ߽Ƕȳ�ʼ��
	uint16_t i;
	for(i=0;i<MAX_N_ANGLE;i++)
		near_angle[i] = 1000;
	//c_near_a = 0;
	//log_printf("K1\r\n");
//�ұ��ӱ�PID
	if(level == 0)
	{
		r_near_pid.min_adc 	= IRD_RIGHT_PID_MIN;
		r_near_pid.max_adc 	= IRD_RIGHT_PID_MAX;
		r_near_pid.max_adc2 = IRD_RIGHT_PID_MAX2;
		r_near_pid.lock_adc = IRD_RIGHT_PID_LOCK;

		//����ӱ�PID
		l_near_pid.min_adc 	= IRD_LEFT_PID_MIN;
		l_near_pid.max_adc 	= IRD_LEFT_PID_MAX;
		l_near_pid.max_adc2 = IRD_LEFT_PID_MAX2;
		l_near_pid.lock_adc = IRD_LEFT_PID_LOCK;

	}else if(level == 1)
	{
		r_near_pid.min_adc 	= 90;
		r_near_pid.max_adc 	= IRD_RIGHT_PID_MAX;
		r_near_pid.max_adc2 = IRD_RIGHT_PID_MAX2;
		r_near_pid.lock_adc = IRD_RIGHT_PID_LOCK;

		//����ӱ�PID
		l_near_pid.min_adc 	= 90;
		l_near_pid.max_adc 	= IRD_LEFT_PID_MAX;
		l_near_pid.max_adc2 = IRD_LEFT_PID_MAX2;
		l_near_pid.lock_adc = IRD_LEFT_PID_LOCK;
	}

	r_near_pid.adc  = &sys->g_sta[6];		//������
	r_near_pid.adc2 = &sys->g_sta[5];		//����ڶ�������

	r_near_pid.a_go_outside = 0.3f;	//�����ߵĽǶ�
	r_near_pid.a_go_inside  = -0.3f;	//�����ߵĽǶ�

	r_near_pid.l_angle = 0;	//��ʧʱ��ĽǶ�
	r_near_pid.count  = 0;
//	r_near_pid.c_near =0;		//�رߵĴ���
	r_near_pid.c_lost =0;		//����ʧȥǽ��ʱ��
	r_near_pid.c_dis_a = 0;
	r_near_pid.last_adc= 0;
	r_near_pid.level = level;
	r_near_pid.obst		=	0;
	//r_near_pid.max_lost = 0;
	//r_near_pid.c_angle = 0;
	r_near_pid.real_lock = r_near_pid.real_max = 0;
	r_near_pid.c_big_change = 0;
	r_near_pid.dist = 0;
	r_near_pid.dist_ok = 0;

//	r_near_pid.small_dist_ok = 0;		////
	r_near_pid.small_dist = 0;
	r_near_pid.c_lost_flag =0;	////add 2018-11-03

	//����ӱ�PID

	l_near_pid.adc  = &sys->g_sta[0];		//������
	l_near_pid.adc2 = &sys->g_sta[1];		//����ڶ�������

	l_near_pid.a_go_outside = -0.3f;	//�����ߵĽǶ�
	l_near_pid.a_go_inside  = 0.3f;	//�����ߵĽǶ�

	l_near_pid.l_angle = 0;	//��ʧʱ��ĽǶ�
	l_near_pid.count  = 0;
//	l_near_pid.c_near =0;		//�رߵĴ���
//	l_near_pid.c_near2 =0;		//�رߵĴ���
	l_near_pid.c_lost =0;		//����ʧȥǽ��ʱ��

	l_near_pid.c_dis_a = 0;
	l_near_pid.last_adc= 0;
	l_near_pid.level = level;

	l_near_pid.obst		=	0;
//	l_near_pid.max_lost = 0;
//	l_near_pid.c_angle = 0;
	l_near_pid.c_big_change = 0;
	l_near_pid.dist = 0;
	l_near_pid.dist_ok = 0;

//	l_near_pid.small_dist_ok = 0;			//////
	l_near_pid.small_dist = 0;
	l_near_pid.c_lost_flag =0;			////add 2018-11-03

	l_near_pid.real_lock = l_near_pid.real_max = 0;

	//
	sys->c_near = 0;
	sys->i_c_near = 0;
	sys->dist_ok = 0;
	sys->small_dist_ok = 0;

	l_near_pid.debug 				= r_near_pid.debug = 0;		//������Ϣ
	l_near_pid.yaw 					= r_near_pid.yaw = &sys->angle;
	l_near_pid.angle				= r_near_pid.angle= &navigat->angle;
	l_near_pid.c_pid_near			= r_near_pid.c_pid_near= &cfg->c_pid_near;
	l_near_pid.a_pid_near			= r_near_pid.a_pid_near= &cfg->a_pid_near;
	l_near_pid.nearwall_lost		= r_near_pid.nearwall_lost = &cfg->nearwall_lost;
	l_near_pid.nearwall_signal_lost = r_near_pid.nearwall_signal_lost = &sys->nearwall_signal_lost;
	l_near_pid.distance 			= r_near_pid.distance = &motor.c_left_hw;
	l_near_pid.c_near_a				= r_near_pid.c_near_a = &c_near_a;
	l_near_pid.max_near_a			= r_near_pid.max_near_a = &max_near_a;
	l_near_pid.yaw_buff				= r_near_pid.yaw_buff = near_angle;

	sys->setminpwm = 100;//cfg->setminpwm;			//����Ϊ��ǿ���ӱ���СPWMֵ
	sys->setmaxpwm = 900;//cfg->setmaxpwm;			//����Ϊ��ǿ���ӱ�����PWMֵ
	log_printf("setminpwm=%d,setminpwm:%d\r\n",sys->setminpwm ,sys->setmaxpwm );
}


/*
�����ϰ�����PID��
1.�������ϰ����ʧ��ʱ�򣬾Ͳ���������ʧ��PID�ˡ�
*/
int pid_obst_around(navigat_t *navi,float angle)
{
/**/
	NEAR_WALL *near;
	NEAR_PID  *npid;

	int x,y;
	static int lx=0,ly=0;

	near=&navi->near;
	npid=near->pid;

	//��һ�㣬�ܱ����ϰ���Ͳ���ȡ���ӱ�
	if(lx == X_NOW && ly == Y_NOW)
		return 0;
	lx = X_NOW;
	ly = Y_NOW;
	for(x=X_NOW-2;x<=X_NOW+2;x++)
	{
		for(y = Y_NOW -2;y<=Y_NOW+2;y++)
		{
				if(OBST_XY(x,y))
				{
					//�̾����ڣ����ϰ�����ӱ�

						log_printf("nobst(%d,%d,)\r\n",x,y);
						npid->obst = 1;		//ȡ���ӱ�
						return 0;
				}

			}

	}
	return 0;

}


char *strl="l";
char *strr="r";


#define NEAR_PID_DEBUG		1
#define NEAR_PID_OFSET		200


int navigat_near_wall_pid(float *agle,int c_lost)
{
	static int c_pid=0;
	NEAR_WALL *near;

	near = &navigat->near;			//��navigat->near��ȡ����
	if(c_pid ++ >=2)
	{
		c_pid = 0;
		if(near->n_sta !=NO_SIDE_NEAR && near->pid != NULL)		//ǿ���ӱ�
		{
			return (near_wall_pid(1,near->pid,agle));
		}else													//����ǿ���ӱ�
		{
			if(near_wall_pid(0,&l_near_pid,agle) ==0)
			{
				return (near_wall_pid(0,&r_near_pid,agle));
			}
			return 1;
		}
	}
	return 1;
}


void turn_round_pid(int sta)
{
	static int  c =0;
	static int lhw1=0,lhw2=0;
	int	speed = sta;
	static uint8_t count=0;//,c_ok=0;
#if JUDGE_PID_CTRL_WHEEL_STOP
			static int idex=0,hw_l=0,hw_r=0;	//,lhw2=0;
#endif
//	log_printf("c:%d,%d,sta=%d\r\n",c,count,sta);

	//��ʼ��
	if(sta ==0)
	{
		count =c =  lhw1 = lhw2 = 0;
#if JUDGE_PID_CTRL_WHEEL_STOP
		idex=0;
#endif
		return ;
	}

	//ѭ��
	if(c++>8)              //20ms����һ���ٶȣ�ִ���ٶȻ�
	{
		int hw1=0,hw2=0;
		int pid_pwm;
		c=0;

		hw1 = motor.c_left_hw - lhw1;  //�ñ�����������ȥ�ϴε��������������ٶȣ������ٶ�
		hw2 = motor.c_right_hw- lhw2;  //���ֵ��ٶ�

		lhw1 = motor.c_left_hw ;
		lhw2 = motor.c_right_hw;
		if(count >=4)                     ////����80ms�󣬲ſ�ʼִ���ٶȻ�
		{
			//motor.c_left_hw = motor.c_right_hw = 0;
			sys->left_speed = hw1;
			sys->right_speed = hw2;
			pid_pwm = (hw1 - speed)*5;    //speed�����趨���ٶȣ��ò�ֵ������һ��ϵ����5�����Լ����PWM�Ĳ�ֵ��
			sys->left_pwm +=pid_pwm;
			if(sys->left_pwm < 200)
				sys->left_pwm = 200;
			if(sys->left_pwm > 960)
				sys->left_pwm = 960;
			*left_pwm = sys->left_pwm;   //ֱ�Ӹ������
			//log_printf("l speed=%d,set=%d,p=%d,pwm=%d\r\n",hw1,speed,pid_pwm,sys->left_pwm );
			pid_pwm = (hw2 - speed)*5;
			sys->right_pwm +=pid_pwm;
			if(sys->right_pwm < 200)
				sys->right_pwm = 200;
			if(sys->right_pwm > 960)
				sys->right_pwm = 960;
			*right_pwm = sys->right_pwm;
			//log_printf("%d,%d,%d,%d\r\n",hw1, hw2,sys->left_pwm,sys->right_pwm);
			//log_printf("r speed=%d,set=%d,p=%d,pwm=%d\r\n",hw2,speed,pid_pwm,sys->right_pwm );
		}else
			count++;
#if JUDGE_PID_CTRL_WHEEL_STOP
#if GET_OUT_OF_TROUBLE_EN
	if( /*(motor.c_left_hw > 50 || motor.c_right_hw > 50 ) && */ ((disXY(hw_l,motor.c_left_hw)<4 && *left_pwm <701) || ((disXY(hw_r,motor.c_right_hw) <4) && *right_pwm < 701)) &&  sys->motor_primary_locked == 0)
#else
	if(/*(motor.c_left_hw > 10 || motor.c_right_hw > 10 ) &&*/((disXY(hw_l,motor.c_left_hw)<4&& *left_pwm <901) || (disXY(hw_r,motor.c_right_hw) <4 && *right_pwm <901) ))
#endif
	{
		c_ok = 0;
		if(idex++ >580)		//3s*7=21s
		{	log_printf("dubug_turn_idex:L(%d,%d)R(%d,%d)%d\r\n",hw_l,motor.c_left_hw,hw_r,motor.c_right_hw,idex);
#if GET_OUT_OF_TROUBLE_EN
			sys->motor_primary_locked = 1;
#else
			sys->motor_locked_flag = 1;//����
#endif
			idex = 0;
		}
	}
	else
	{	if(c_ok++>3)
			idex = 0;
		//sys->motor_locked_flag = 0;//����
	}
	hw_l = motor.c_left_hw ;
	hw_r = motor.c_right_hw;
#endif
	}
}


void charge_turn_pid(int sta)
{
//	static int  c =0;
	static int lhw1=0,lhw2=0;
	int	speed = sta;
	static uint8_t count=0;//,c_ok=0;

#if JUDGE_PID_CTRL_WHEEL_STOP
		static int idex=0,hw_l=0,hw_r=0;	//,lhw2=0;
#endif

	//��ʼ��
	if(sta ==0)
	{
		motor.c_left_hw =motor.c_right_hw=count =  lhw1 = lhw2 = 0;//=c;

#if JUDGE_PID_CTRL_WHEEL_STOP
		idex=0;
#endif
		return ;
	}

	//ѭ��
	//if(c++>4)
	{
		int hw1=0,hw2=0;
		int pid_pwm;
//		c=0;

		hw1 = motor.c_left_hw - lhw1;
		hw2 = motor.c_right_hw- lhw2;

		lhw1 = motor.c_left_hw ;
		lhw2 = motor.c_right_hw;
		if(count >=2)//4)
		{
			sys->left_speed = hw1;
			sys->right_speed = hw2;
			//motor.c_left_hw = motor.c_right_hw = 0;
			pid_pwm = (hw1 - speed)*5;
			sys->left_pwm +=pid_pwm;
			if(sys->left_pwm < 200)
				sys->left_pwm = 200;
			if(sys->left_pwm > 980)
				sys->left_pwm = 980;
			*left_pwm = sys->left_pwm;

			pid_pwm = (hw2 - speed)*5;
			sys->right_pwm +=pid_pwm;
			if(sys->right_pwm < 200)
				sys->right_pwm = 200;
			if(sys->right_pwm > 980)
				sys->right_pwm = 980;
			*right_pwm = sys->right_pwm;
			log_printf("%d,%d(%d),%d,%d\r\n",hw1, hw2,sta,sys->left_pwm,sys->right_pwm);
			//log_printf("%d,%d,%d,%d\r\n",hw1, hw2,*right_pwm,*left_pwm);
		}else
			count++;

#if JUDGE_PID_CTRL_WHEEL_STOP
#if GET_OUT_OF_TROUBLE_EN          //�����ֲ���
	if(/*(motor.c_left_hw > 50 || motor.c_right_hw > 50) &&*/ ((disXY(hw_l,motor.c_left_hw)<4 && *left_pwm <701) || ((disXY(hw_r,motor.c_right_hw) <4) && *right_pwm < 701)) &&  sys->motor_primary_locked == 0)
#else
	if(/*(motor.c_left_hw > 10 || motor.c_right_hw > 10) &&*/((disXY(hw_l,motor.c_left_hw)<4 && *left_pwm <901) || (disXY(hw_r,motor.c_right_hw) <4 && *left_pwm<901) ))
#endif
	{
		c_ok = 0;
		if(idex++ >2300)		//
		{	log_printf("change_idex:L(%d,%d)R(%d,%d)%d\r\n",hw_l,motor.c_left_hw,hw_r,motor.c_right_hw,idex);
#if GET_OUT_OF_TROUBLE_EN
			sys->motor_primary_locked = 1;
#else
			sys->motor_locked_flag = 1;//����
#endif
			idex = 0;
		}
	}
	else
	{
		if(c_ok++>3)
			idex = 0;
		//sys->motor_locked_flag = 0;//����
	}
	hw_l = motor.c_left_hw ;
	hw_r = motor.c_right_hw;
#endif


	}


}


/*
�ٶȻ�
*/
void speed_pid_ctrl(uint16_t speed)
{

	static int  c =0;
	static int lhw1=0,lhw2=0;
	//ѭ�� ÿ��50����һ��
	if(c++>10)
	{
		int hw1=0,hw2=0;
		int pid_pwm;
		c=0;

		hw1 = motor.c_left_hw - lhw1;
		hw2 = motor.c_right_hw- lhw2;

		lhw1 = motor.c_left_hw ;
		lhw2 = motor.c_right_hw;
		//motor.c_left_hw = motor.c_right_hw = 0;
		pid_pwm = (hw1 - hw2)*5;
		sys->left_pwm +=pid_pwm;
		if(sys->left_pwm < 450)
			sys->left_pwm = 450;
		if(sys->left_pwm > 780)
			sys->left_pwm = 780;
		//l_motor_set_pwm(sys->left_dir,sys->left_pwm);
		*left_pwm = sys->left_pwm;
	//	log_printf("%3.1f,%d,%d,%d,%d\r\n",sys->angle,hw1, hw2,pid_pwm,sys->left_pwm);

	}


}

/*
ֱ�е�ʱ�򣬼��ٶȻ�
*/
#if JUDGE_PID_CTRL_WHEEL_STOP
void m_speed_pid_ctrl(uint16_t speed,uint8_t type,int hw,int c_left_hw_min,uint8_t ird_state,int hw_min,int walk_dist_min)
{

//	static int  c =0;
static int lhw1=0;	//,lhw2=0;
	int hw1=0;////,hw2=0;
	int pid_pwm;

	static int idex=0,hw_l=0,hw_r=0,c_ok=0;	//,lhw2=0;

	static int pid_flag = 0;

	if(type ==0)
	{
		lhw1 = motor.c_left_hw;
		idex=0;
		return ;
	}
	if(ird_state == 1)
	{
		if(motor.c_left_hw > c_left_hw_min && !SLOW_IRD_MIDL() &&
		 !(hw > hw_min && WALK_DIST() > (hw-walk_dist_min)))

			pid_flag = 1;
		else
			pid_flag = 0;

	}
	else if(ird_state == 0)
	{
		if(motor.c_left_hw > c_left_hw_min &&/* !SLOW_IRD_MIDL() && */
					!(hw > hw_min && WALK_DIST() > (hw-walk_dist_min)))
			pid_flag = 1;
		else
			pid_flag = 0;
	}
	else if(ird_state == 2)
	{

	}
	//ѭ�� ÿ��50����һ��
	//if(c++>10)
	if(pid_flag)
	{
#if SYS_VER == VER_KESMAN
	#define RUN_SPEED	100
	if(sys->pwm !=470)
		sys->pwm = 470;
	return ;
#endif
#if SYS_VER == VER_ROMMAT || VER_SMALLBLACKMACH || VER_BIGWHITEMACH	|| VER_WHITEBLUEMACH
	#define RUN_SPEED	300//180//180

		//c=0;

		hw1 = motor.c_left_hw - lhw1;
		lhw1 = motor.c_left_hw ;
		//log_printf("hw=%d,%d,",hw1,sys->pwm);
		//if(hw1 > 100 && hw1 < 300)
		if(hw1 < RUN_SPEED)
		{

			/**/
			pid_pwm = (hw1 - RUN_SPEED) / 2;
			sys->pwm +=pid_pwm;
			if(sys->pwm > 800)
			{
				sys->pwm = 800;
			}
			if(sys->pwm < 300)
			{
				sys->pwm = 300;
			}

			//log_printf("%d(%d)\r\n",sys->pwm,pid_pwm);
		}
		//log_printf(",%d\r\n",sys->pwm);
		//hw2 = motor.c_right_hw- lhw2;

		/*
		lhw1 = motor.c_left_hw ;
		lhw2 = motor.c_right_hw;
		//motor.c_left_hw = motor.c_right_hw = 0;
		pid_pwm = (hw1 - hw2)*5;
		sys->left_pwm +=pid_pwm;
		if(sys->left_pwm < 450)
			sys->left_pwm = 450;
		if(sys->left_pwm > 780)
			sys->left_pwm = 780;
		//l_motor_set_pwm(sys->left_dir,sys->left_pwm);
		*left_pwm = sys->left_pwm;
		*/
	//	log_printf("%3.1f,%d,%d,%d,%d\r\n",sys->angle,hw1, hw2,pid_pwm,sys->left_pwm);
#endif
/*
#if SYS_VER == VER_SMALLBLACKMACH
	#define RUN_SPEED	100
		if(sys->pwm !=470)
			sys->pwm = 470;

#endif
*/
	}
#if GET_OUT_OF_TROUBLE_EN
	if(( /*(motor.c_left_hw > 50 || motor.c_right_hw > 50 ) && */(disXY(hw_l,motor.c_left_hw)<8 && *left_pwm <701) || ((disXY(hw_r,motor.c_right_hw) <8) && *right_pwm < 701)) &&  sys->motor_primary_locked == 0)
#else
	if(/*(motor.c_left_hw > 10 || motor.c_right_hw > 10 ) &&*/ /*()&&*/ ((disXY(hw_l,motor.c_left_hw)<8 && *left_pwm <901) || ((disXY(hw_r,motor.c_right_hw) <8) && *right_pwm < 901)) )
#endif
	//if((disXY(hw_l,motor.c_left_hw)<8 ) || (disXY(hw_r,motor.c_right_hw) <8))
	{
		if(idex++ >68 /*&& */)		//20s
		{	log_printf("dubug_idex:L%d(%d,%d)R%d(%d,%d)%d\r\n",*left_pwm ,hw_l,motor.c_left_hw,*right_pwm ,hw_r,motor.c_right_hw,idex);
#if GET_OUT_OF_TROUBLE_EN
			sys->motor_primary_locked = 1;
#else
			sys->motor_locked_flag = 1;//����
#endif
			idex = 0;
		}
		c_ok = 0;
	}
	else
	{
		if(c_ok++>3)
			idex = 0;
		//sys->motor_locked_flag = 0;//����
	}
	hw_l = motor.c_left_hw ;
	hw_r = motor.c_right_hw;

	return ;
}

#else

void m_speed_pid_ctrl(uint16_t speed,uint8_t type)
{

//	static int  c =0;
	static int lhw1=0,rhw1=0;	//��¼��һ�����ֵĻ���ֵ
	int hw1=0;////,hw2=0;
	int pid_pwm;
	static uint16_t count=0;


	if(type ==0)
	{
		lhw1 = motor.c_left_hw;  //��¼��һ�����ֵĻ���ֵ
		rhw1 = motor.c_right_hw;  //��¼��һ�����ֵĻ���ֵ
		count = 0;
		return ;
	}
	if(count < 2)
	{
		count++;
		return ;
	}
/*

ÿ250msִ��һ�Σ�ÿ29����һ����,290�ٶȣ���250ms10cm��= 0.1/0.25 = 0.4m/s
*/
	//�ٶȵ�������
	int MAX_SPEED	= 350;//180//180
	int MIN_SPEED	= 280;
	//�ӱ�
	if(type ==5)
	{
		MAX_SPEED = 450;
		MIN_SPEED  = 400;
	}else
	{
		if(sys->run_speed >=100 && sys->run_speed < 600)
		{
			MAX_SPEED = sys->run_speed+10;
			MIN_SPEED = sys->run_speed-10;
		}

	}


		//��������ֵ��ٶ�
		hw1 = motor.c_left_hw - lhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
		lhw1 = motor.c_left_hw ;		//��¼���εĻ���ֵ
		sys->left_speed = hw1;

	//�ٶ�С��
		if(L_FORWRK_PWM > 200 && L_FORWRK_PWM < 900)
		{
			if(hw1 < MIN_SPEED)
			{
				pid_pwm = (hw1 - MIN_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				if(type ==5)
				{
					sys->pwm+=pid_pwm;
					log_printf("lspeed2(%d)+:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,sys->pwm);
					if(sys->pwm < 200)
						sys->pwm = 200;
					if(sys->pwm > 800)
						sys->pwm = 800;

				}else
				{
					L_FORWRK_PWM +=pid_pwm;
					if(L_FORWRK_PWM < 200)
						L_FORWRK_PWM = 200;
					if(L_FORWRK_PWM > 999)
						L_FORWRK_PWM = 999;

					log_printf("lspeed(%d)+:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,L_FORWRK_PWM);
				}

			}else if(hw1 > MAX_SPEED)
			{
				pid_pwm = (hw1 - MAX_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				   if(type ==5)
				{
					sys->pwm+=pid_pwm;
					log_printf("rspeed2(%d)+:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,sys->pwm);
					if(sys->pwm < 200)
						sys->pwm = 200;
					if(sys->pwm > 800)
						sys->pwm = 800;

				}else
				{
					L_FORWRK_PWM +=pid_pwm;
					if(L_FORWRK_PWM < 200)
						L_FORWRK_PWM = 200;
					if(L_FORWRK_PWM > 999)
						L_FORWRK_PWM = 999;
					log_printf("lspeed(%d)-:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,L_FORWRK_PWM);
				}
			}
		}

		//��������ֵ��ٶ�
		hw1 = motor.c_right_hw- rhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
		rhw1 = motor.c_right_hw ;		//��¼���εĻ���ֵ
		sys->right_speed = hw1;

	//�ٶ�С��
		if(R_FORWRK_PWM > 200 && R_FORWRK_PWM < 900)
		{
			if(hw1 < MIN_SPEED)
			{
				pid_pwm = (hw1 - MIN_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				R_FORWRK_PWM +=pid_pwm;
				if(R_FORWRK_PWM < 200)
					R_FORWRK_PWM = 200;
				if(R_FORWRK_PWM > 999)
					R_FORWRK_PWM = 999;
			//	log_printf("rspeed+:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,L_FORWRK_PWM);
			}else if(hw1 > MAX_SPEED)
			{
				pid_pwm = (hw1 - MAX_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				R_FORWRK_PWM +=pid_pwm;

				if(R_FORWRK_PWM < 200)
					R_FORWRK_PWM = 200;
				if(R_FORWRK_PWM > 999)
					R_FORWRK_PWM = 999;
				//log_printf("rspeed-:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,L_FORWRK_PWM);
			}
		}
		//�������ֶ���pwm������ֵ ���Ǹ��ǶȻ���
		sys->pwm = (R_FORWRK_PWM+L_FORWRK_PWM) >> 1;
		//log_printf("pwm=%d,%d,%d\r\n",L_FORWRK_PWM,R_FORWRK_PWM,sys->pwm);




}

#endif


/*
������˫��PID
*/
void m_speed_pid_indep(uint16_t lspeed,uint16_t rspeed,uint8_t type)
{

//	static int  c =0;
	static int lhw1=0,rhw1=0;	//��¼��һ�����ֵĻ���ֵ
	int hw1=0;////,hw2=0;
	int pid_pwm;
	static uint16_t count=0;


	if(type ==0)
	{
		lhw1 = motor.c_left_hw;  //��¼��һ�����ֵĻ���ֵ
		rhw1 = motor.c_right_hw;  //��¼��һ�����ֵĻ���ֵ
		count = 0;
		return ;
	}
	//micony 2020101 ���붪��ǰ�ߵļ�Ȧ��Ҫ��Ȼǰ�ߵļ�Ȧ��ò��浼���ٶȿ��
	if(count < 2)
	{
		count++;
		return ;
	}
/*

ÿ250msִ��һ�Σ�ÿ29����һ����,290�ٶȣ���250ms10cm��= 0.1/0.25 = 0.4m/s
*/
	//�ٶȵ�������




		int MAX_SPEED = lspeed+6;
		int MIN_SPEED = lspeed-6;


		//��������ֵ��ٶ�
		hw1 = motor.c_left_hw - lhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
		lhw1 = motor.c_left_hw ;		//��¼���εĻ���ֵ

	//�ٶ�С��
		if(L_FORWRK_PWM > 200 && L_FORWRK_PWM < 900)
		{
			if(hw1 < MIN_SPEED)
			{
				pid_pwm = (hw1 - MIN_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;

					L_FORWRK_PWM +=pid_pwm;
					if(L_FORWRK_PWM < 200)
						L_FORWRK_PWM = 200;
					if(L_FORWRK_PWM > 999)
						L_FORWRK_PWM = 999;

					log_printf("lspeed(%d)+:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,L_FORWRK_PWM);

			}else if(hw1 > MAX_SPEED)
			{
				pid_pwm = (hw1 - MAX_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;

					L_FORWRK_PWM +=pid_pwm;
					if(L_FORWRK_PWM < 200)
						L_FORWRK_PWM = 200;
					if(L_FORWRK_PWM > 999)
						L_FORWRK_PWM = 999;
					log_printf("lspeed(%d)-:%d,pid=%d,pwm=%d\r\n",sys->run_speed,hw1,pid_pwm,L_FORWRK_PWM);
			}
		}
		MAX_SPEED = rspeed+6;
		MIN_SPEED = rspeed-6;

		//��������ֵ��ٶ�
		hw1 = motor.c_right_hw- rhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
		rhw1 = motor.c_right_hw ;		//��¼���εĻ���ֵ
		sys->right_speed = hw1;

	//�ٶ�С��
		if(R_FORWRK_PWM > 200 && R_FORWRK_PWM < 999)
		{
			if(hw1 < MIN_SPEED)
			{
				pid_pwm = (hw1 - MIN_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				R_FORWRK_PWM +=pid_pwm;
				if(R_FORWRK_PWM < 200)
					R_FORWRK_PWM = 200;
				if(R_FORWRK_PWM > 999)
					R_FORWRK_PWM = 999;
				log_printf("rspeed+:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,R_FORWRK_PWM);
			}else if(hw1 > MAX_SPEED)
			{
				pid_pwm = (hw1 - MAX_SPEED) / 2;
				if(pid_pwm > 200 )
				   pid_pwm = 200;
				 if(pid_pwm < -200 )
				   pid_pwm = -200;
				R_FORWRK_PWM +=pid_pwm;

				if(R_FORWRK_PWM < 200)
					R_FORWRK_PWM = 200;
				if(R_FORWRK_PWM > 999)
					R_FORWRK_PWM = 999;
				log_printf("rspeed-:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,R_FORWRK_PWM);
			}
		}
		//�������ֶ���pwm������ֵ ���Ǹ��ǶȻ���
		//sys->pwm = (R_FORWRK_PWM+L_FORWRK_PWM) >> 1;
		//log_printf("pwm=%d,%d,%d\r\n",L_FORWRK_PWM,R_FORWRK_PWM,sys->pwm);




}





//int near_lost = 0;



typedef struct _mpid_t{
	int16_t c_near_loop;
	int16_t c_near_lost ;
	int16_t c_near_wall;
	uint8_t m_nsta;
	uint8_t	distok;
	uint16_t	min_adc;
	uint16_t	max_adc;
	uint16_t 	c_near;
	int	dist;

	uint16_t 	*adc;		//������

	float SetSpeed;			//�����趨ֵ
	float ActualSpeed;		//����ʵ��ֵ
	float err; 				//����ƫ��ֵ
	float err_next;	//������һ��ƫ��ֵ
	float integral;
	float err_last;//��������ǰ��ƫ��ֵ
	float Kp,Ki,Kd;	//������������֡�΢��ϵ��
	float out;

//	float max;
	//int	  c_pid;
}mpid_t;

mpid_t mrpid,mlpid;
#define NEAR_DIS_PID_DEBUG	1

void init_nearpid_differ(int pwm,uint16_t speed,uint8_t msta)
{
	mpid_t *pid;

//	near_lost = 0;
	//�ұ�
	pid = &mrpid;
	pid->SetSpeed =(float)speed;
	pid->ActualSpeed=0.0;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->err_next=0.0;
	pid->out = 0;
  	//sys->pwm = pwm;
	pid->c_near_loop = 30;
	pid->c_near_lost = 0;
	pid->m_nsta = msta;

	//pid->Kp = 0.5f;
	//pid->Kd = 0.5f;
	pid->Kp = cfg->kp1;//0.5f;
	pid->Kd = cfg->kd1;
	pid->Kp /=100.0f;
	pid->Kd /=100.0f;

	pid->adc =  &sys->g_sta[6];
	pid->min_adc = cfg->min_right_adc;
	pid->max_adc = cfg->max_right_adc;

	//���
	pid = &mlpid;
	pid->SetSpeed =(float)speed;
	pid->ActualSpeed=0.0;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->err_next=0.0;
	pid->out = 0;
  	//sys->pwm = pwm;
	pid->c_near_loop = 30;
	pid->c_near_lost = 0;
	pid->m_nsta = msta;
	//pid->Kp = 0.5f;
	//pid->Kd = 0.5f;
	pid->Kp = cfg->kp1;//0.5f;
	pid->Kd = cfg->kd1;
	pid->Kp /=100.0f;
	pid->Kd /=100.0f;

	pid->adc =  &sys->g_sta[0];
	pid->min_adc = cfg->min_left_adc;
	pid->max_adc = cfg->max_left_adc;

	sys->nsta = NO_SIDE_NEAR;



	//pid->Kp = (float)cfg->kp2 / PID_PEC;
	//lpid.Ki = (float)cfg->ki2 / PID_PEC;
	//pid->Kd = (float)cfg->kd2 / PID_PEC;
#if NEAR_DIS_PID_DEBUG
	log_printf("init_nearpid(%3.2f,%3.2f)\r\n",pid->Kp,pid->Kd);
#endif


}


void init_nearmpid(mpid_t *pid,int pwm,uint16_t speed)
{

	pid->SetSpeed =(float)speed;
	pid->ActualSpeed=0.0;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->err_next=0.0;

	pid->out = 0;
  	sys->pwm = pwm;
	pid->c_near_loop = 30;
	pid->c_near_lost = 0;
	pid->distok= 0;

	pid->Kp = 0.3f;
	pid->Kd = 0.3f;
	//pid->dist = motor.c_left_hw;
	pid->c_near = 0;

#if NEAR_DIS_PID_DEBUG
	log_printf("rset,speed=%d,adc=%d\r\n",speed,*pid->adc);
#endif


}


/*
���ٿ��Ƶ�PID
*/
#define MAX_N_ANGLE1		10
float		near_angle1[MAX_N_ANGLE1];
uint16_t	c_near_a1=0;



pid_t rpid,*pid;

#define PID_PEC	100.0f


extern void pd_gyro_int(int out)
{
/*
	pid_t *pid;
	pid = &lpid;
	pid->SetSpeed =0.0;
	pid->ActualSpeed=0.0;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->err_next=0.0;
	//pid->Kp=0.008f;
	//pid->Ki=0.035f;
	//pid->Kd=0.02f;
	pid->out = out;
*/
//	g_printf("1\r\n");
	pid = &rpid;
	pid->SetSpeed =0.0;
	pid->ActualSpeed=0.0;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->err_next=0.0;
	pid->integral = 0;

//	g_printf("2\r\n");

	pid->out = out;

//	pid->max = 20;


/*
	lpid.Kp = (float)cfg->kp1 / PID_PEC;
	lpid.Ki = (float)cfg->ki1 / PID_PEC;
	lpid.Kd = (float)cfg->kd1 / PID_PEC;
	*/

	//g_printf("3\r\n");

#if GYRO_TO_LIB
	rpid.Kp = (float)(*(mgyro->kp2)) / PID_PEC;

//	g_printf("4\r\n");
	rpid.Ki = (float)(*(mgyro->ki2))/ PID_PEC;
	rpid.Kd = (float)(*(mgyro->kd2)) / PID_PEC;

//	sys->c_pid_loop = 0;

	*(mgyro->rpwm)=*(mgyro->lpwm)=out;
	*(mgyro->pwm)= out;
#else
rpid.Kp = (float)(cfg->kp2) / PID_PEC;

//	g_printf("4\r\n");
	rpid.Ki = (float)(cfg->ki2)/ PID_PEC;
	rpid.Kd = (float)(cfg->kd2) / PID_PEC;

//	sys->c_pid_loop = 0;

	sys->rpwm=sys->lpwm=out;
	sys->pwm= out;
#endif


	sys->min_pwm = -130;//-160;

	sys->max_pwm = 130;//160;

	sys->setminpwm = 200;//cfg->setminpwm;			//����Ϊ��ǿ���ӱ���СPWMֵ
	sys->setmaxpwm = 900;//cfg->setmaxpwm;			//����Ϊ��ǿ���ӱ�����PWMֵ


	pid->min_pwm = &sys->min_pwm;
	pid->max_pwm = &sys->max_pwm;
	pid->lpwm = &sys->lpwm;
	pid->rpwm = &sys->rpwm;
	pid->pwm = &sys->pwm;

	pid->setminpwm = &sys->setminpwm;
	pid->setmaxpwm = &sys->setmaxpwm;

	pid->debug = 0;


}


int16_t dock_speed_pid_ctrl(uint16_t min,uint16_t max,uint8_t type)
{
		static int	c =0;
		static int lhw1=0,rhw1=0;	//��¼��һ�����ֵĻ���ֵ
		int hw1=0;////,hw2=0;
		int pid_pwm;
		static uint16_t count=0;


		if(type ==0)
		{
			lhw1 = motor.c_left_hw = 0;  //��¼��һ�����ֵĻ���ֵ
			rhw1 = motor.c_right_hw = 0;  //��¼��һ�����ֵĻ���ֵ
			log_printf("ds t=%d,hw=%d,%d\r\n",msTmr,motor.c_left_hw,motor.c_right_hw);
			c=count = 0;
			return 0;
		}
		//micony 2020101 ���붪��ǰ�ߵļ�Ȧ��Ҫ��Ȼǰ�ߵļ�Ȧ��ò��浼���ٶȿ��
		if(c++ < 20)
		{
			return 0;
		}
		c = 0;
		count++;
		if(count < 2)
		{
			lhw1 = motor.c_left_hw;  //��¼��һ�����ֵĻ���ֵ
			rhw1 = motor.c_right_hw;  //��¼��һ�����ֵĻ���ֵ
			log_printf("ds2 t=%d,hw=%d,%d\r\n",msTmr,motor.c_left_hw,motor.c_right_hw);
			return 0;
		}
	/*

	ÿ250msִ��һ�Σ�ÿ29����һ����,290�ٶȣ���250ms10cm��= 0.1/0.25 = 0.4m/s
	*/
		//�ٶȵ�������
		int MAX_SPEED;//	= 350;//180//180
		int MIN_SPEED;//	= 280;
		if(charge_info.disc_signal ==0)
		{
			MAX_SPEED = 300;
			MIN_SPEED = 200;
		}else
		//�ӱ�
		if(charge_info.c_midle >=3)
		{
			MAX_SPEED = cfg->r[IC_DOCK_M_WALK_SPEED]+10;
			MIN_SPEED = cfg->r[IC_DOCK_M_WALK_SPEED]-10;

		}else
		{
			MAX_SPEED = cfg->r[IC_DOCK_WALK_SPEED]+10;
			MIN_SPEED = cfg->r[IC_DOCK_WALK_SPEED]-10;

		}


	//log_printf("ds3 t=%d,hw=%d,%d\r\n",msTmr,motor.c_left_hw,motor.c_right_hw);

			//��������ֵ��ٶ�
			hw1 = motor.c_left_hw - lhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
			lhw1 = motor.c_left_hw ;		//��¼���εĻ���ֵ
			sys->left_speed = hw1;
		//�ٶ�С��
			if(L_FORWRK_PWM > 200 && L_FORWRK_PWM < 900)
			{
				if(hw1 < MIN_SPEED)
				{
					pid_pwm = (hw1 - MIN_SPEED) / 2;
					if(pid_pwm > 200 )
					   pid_pwm = 200;
					 if(pid_pwm < -200 )
					   pid_pwm = -200;

					{
						L_FORWRK_PWM +=pid_pwm;
						if(L_FORWRK_PWM < 200)
							L_FORWRK_PWM = 200;
						if(L_FORWRK_PWM > 999)
							L_FORWRK_PWM = 999;

						//log_printf("lspeed(%d)+:%d,pid=%d,pwm=%d\r\n",cfg->r[IC_DOCK_M_WALK_SPEED],hw1,pid_pwm,L_FORWRK_PWM);
					}

				}else if(hw1 > MAX_SPEED)
				{
					pid_pwm = (hw1 - MAX_SPEED) / 2;
					if(pid_pwm > 200 )
					   pid_pwm = 200;
					 if(pid_pwm < -200 )
					   pid_pwm = -200;

					{
						L_FORWRK_PWM +=pid_pwm;
						if(L_FORWRK_PWM < 200)
							L_FORWRK_PWM = 200;
						if(L_FORWRK_PWM > 999)
							L_FORWRK_PWM = 999;
						//log_printf("lspeed(%d)-:%d,pid=%d,pwm=%d\r\n",cfg->r[IC_DOCK_M_WALK_SPEED],hw1,pid_pwm,L_FORWRK_PWM);
					}
				}
			}

			//��������ֵ��ٶ�
			hw1 = motor.c_right_hw- rhw1;  //�ñ��εĻ���ֵ ��ȥ�ϴεĻ���ֵ �͵õ��ٶ�
			rhw1 = motor.c_right_hw ;		//��¼���εĻ���ֵ
			sys->right_speed = hw1;

		//�ٶ�С��
			if(R_FORWRK_PWM > 200 && R_FORWRK_PWM < 900)
			{
				if(hw1 < MIN_SPEED)
				{
					pid_pwm = (hw1 - MIN_SPEED) / 2;
					if(pid_pwm > 200 )
					   pid_pwm = 200;
					 if(pid_pwm < -200 )
					   pid_pwm = -200;
					R_FORWRK_PWM +=pid_pwm;
					if(R_FORWRK_PWM < 200)
						R_FORWRK_PWM = 200;
					if(R_FORWRK_PWM > 999)
						R_FORWRK_PWM = 999;
					//log_printf("rspeed+:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,L_FORWRK_PWM);
				}else if(hw1 > MAX_SPEED)
				{
					pid_pwm = (hw1 - MAX_SPEED) / 2;
					if(pid_pwm > 200 )
					   pid_pwm = 200;
					 if(pid_pwm < -200 )
					   pid_pwm = -200;
					R_FORWRK_PWM +=pid_pwm;

					if(R_FORWRK_PWM < 200)
						R_FORWRK_PWM = 200;
					if(R_FORWRK_PWM > 999)
						R_FORWRK_PWM = 999;
					//log_printf("rspeed-:%d,pid=%d,pwm=%d\r\n",hw1,pid_pwm,L_FORWRK_PWM);
				}
			}
			//�������ֶ���pwm������ֵ ���Ǹ��ǶȻ���
			sys->pwm = (R_FORWRK_PWM+L_FORWRK_PWM) >> 1;
			log_printf("pwm=%d,%d,%d\r\n",L_FORWRK_PWM,R_FORWRK_PWM,sys->pwm);



/*
	static int  c =0;//,c_zero=0;
	static int lhw1=0;//,lhw2=0;
	int hw1=0;//,hw2=0;
//	static int count=0;
//	int pid_pwm;
	if(type ==0)
	{
		c = lhw1 = motor.c_left_hw=0 ;
		//c_zero = 0;
		log_printf("speed_pid_ctrl reset\r\n");
		return 0;
	}

	if(c++>20)
	{

		c=0;

		if(charge_info.c_midle >=3)		//����???D??
		{
			min = 40;
			max = 50;
		}
		else
		{
			//min = 70;
			//max = 150;
			min = 80;				//2018-10-18 jzz
			max = 90;
		}

		hw1 = motor.c_left_hw - lhw1;
		lhw1 = motor.c_left_hw ;

		log_printf("spd=%d,pwm=%d,left=%d\r\n",hw1,sys->pwm, motor.c_left_hw);

//		log_printf("hw=%d,%d,%d\r\n",hw1,motor.c_left_hw,sys->pwm);
			if(hw1 < min)
		{
			sys->pwm -=50;
			log_printf("-hw=%d,%d,%d,%d\r\n",hw1,sys->pwm,L_FORWRK_PWM,R_FORWRK_PWM);
			if((L_FORWRK_PWM > sys->pwm && R_FORWRK_PWM > sys->pwm ) || (L_FORWRK_PWM < sys->pwm && R_FORWRK_PWM < sys->pwm ))
				L_FORWRK_PWM = R_FORWRK_PWM = sys->pwm;


		}else if(hw1 > max)
		{
			sys->pwm +=50;
			log_printf("+hw=%d,%d,%d,%d\r\n",hw1,sys->pwm,L_FORWRK_PWM,R_FORWRK_PWM);
			if((L_FORWRK_PWM > sys->pwm && R_FORWRK_PWM > sys->pwm ) || (L_FORWRK_PWM < sys->pwm && R_FORWRK_PWM < sys->pwm ))
				L_FORWRK_PWM = R_FORWRK_PWM = sys->pwm;

		}//else
//			log_printf("hw=%d,%d,%d,",hw1,motor.c_left_hw,sys->pwm);
		if(sys->pwm < 200)
				sys->pwm = 200;
		else if(sys->pwm >900)
			sys->pwm = 900;


	}
	return 0;
*/
return 0;
}


//�̣�??PID
void single_whele_pid(int rspeed,int chw,volatile uint16_t *whele_pwm)
{
	static int  c =0;
	static int lhw1=0;//,lhw2=0;
	static uint8_t count=0;
	static float last_yaw=0;
	static uint16_t ac=0;
	//3?��??��
	if(rspeed ==0)
	{
		count =c =  lhw1  = 0;
		lhw1 = chw;
		last_yaw=sys->angle;
		ac=0;
		return ;
	}
	//?-?��
	if(c++>16)
	{
		int hw1=0;
		int pid_pwm;
		int pwm=*whele_pwm;
		c=0;


		hw1 = chw- lhw1;

		lhw1 = chw ;
		if(count >=2)
		{

			pid_pwm = (hw1 - rspeed)*2;

			pwm +=pid_pwm;
			if(pwm < 150)
				pwm  = 150;
			if(pwm  > 800)
				pwm  = 800;
			*whele_pwm = pwm;
		if(disfloat(last_yaw,sys->angle) < 0.35f)
				ac++;
			else
				ac = 0;
			log_printf("hw=%d,speed=%d,%d,pwm=%d,%d,%d,yaw=%3.3f,%d\r\n\r\n",chw,hw1,rspeed,*whele_pwm,pid_pwm,pwm,sys->angle,ac);
			last_yaw = sys->angle;
			if(chw >900 && ac >=3)
			{
				log_printf("slip bum,side =%d\r\n",navigat->near.n_sta);
				if(navigat->near.n_sta == LEFT_SIDE_NEAR)
					sys->slipping_state=MASK_BUM_LEFT;
				if(navigat->near.n_sta == RIGHT_SIDE_NEAR)
					sys->slipping_state=MASK_BUM_RIGHT;
			}
		}else
			count++;
	}
}


/*
??��?PID��?���騰a��?��?D??����?������?��???3��?D??DD?��
*/
#define SINGLE_NAER_PID		cfg->nearwall_cnt//20
char proc_nearwall_pid(float set_angle,uint8_t nside)
{

	static uint8_t count=0; 	//2018-07-16 jzz
//	static uint8_t snd_cnt = 0;
	uint8_t is_line_pid=1;
//	static float last_angle=0;
	www_idleintel_com();
//	uint16_t lspeed,rspeed ;
	if(sys->nearwall_signal_lost==SET )	//??��??a���쨢?��??��?��?��
	{

		if(nside == LEFT_SIDE_NEAR)	//����??��?
		{
			//�������??
			L_FORWRK_PWM = cfg->nearwall_pwm;
			single_whele_pid(SINGLE_NAER_PID,motor.c_right_hw,right_pwm);
			is_line_pid = 0;

		}else if(nside == RIGHT_SIDE_NEAR)
		{
			R_FORWRK_PWM = cfg->nearwall_pwm;
			single_whele_pid(SINGLE_NAER_PID,motor.c_left_hw,left_pwm);
			is_line_pid = 0;
		}


	}
	count ++;
	if( count>=2)//2018-07-16 jzz
	{
		count = 0;
		//log_printf("a=%3.1f,d=%d\r\n",sys->angle,motor.c_left_hw);
		if(is_line_pid) //??��D?a�����?��????��?��,��??a��?��???��?��??�䨢???count??��?
		{
			if(set_angle == NO_ANGLE_V)
			{
				set_angle = navigat->out = sys->angle;
				log_printf("set = %3.1f\r\n",sys->angle);
			}
			set_angle = format_agle(set_angle, ANGLE_360);

			p_gyro_calce(set_angle,sys->angle);		//?��?Y??����???���̡�??��?????������?pwm,sys->lpwm,sys->rpwm

			L_FORWRK_PWM =  sys->lpwm;
			R_FORWRK_PWM =  sys->rpwm;
//			last_angle = sys->angle;
		}else
		{

//			last_angle = sys->angle;
		}
	}


	return 1;
}


