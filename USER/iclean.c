

#include "sys.h"

uint16_t sWorkTimeMinCt,sWorkTimeMin,sSleepTimeMin;

struct sys_t syst,*sys;

gyro_t kgyro;
uint8_t m_stack[STACK_LEN];		//ϵͳ��ջ���ں����ڲ���ʱʹ��	
uint8_t	m_stack2[STACK_LEN2];			//�ֲ�ʹ��
//

//struct config_t config,*cfg;

//struct irda_t  rx_irda,rx_irda1,rx_irda8;

 #if 0
void read_Flashbuf(u32 addr,u8 *p,u16 n)
{										   
	while(n--)
	{
		*(p++)=*((u8*)addr++);
	}	
}

void configSave(void)
{


	uint16_t i=0;
	uint32_t r1,addr;
	uint8_t *p;								 
		 
	i=CONFIG_SIZE;	//�����ֽ���
	p=(u8 * )cfg;		   		//���뿽����ַ
	addr = CFG_ADDER;	   		//FLASHĿ���ַ
//	OS_ENTER_CRITICAL() ;	   //���ж�

	FLASH_Unlock();//FLASH����
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ
	FLASH_ErasePage(CFG_ADDER);	 //���ҳ
	while(i--)
	{
		r1=*(p++);
		r1|=*(p++)<<8;
		r1|=*(p++)<<16;
		r1|=*(p++)<<24;
		FLASH_ProgramWord(addr, r1);
		addr+=4;
	}
	FLASH_Lock();	
//	OS_EXIT_CRITICAL() ; 

//	LEDon;

//	for(i = 0; i < configDataSize; i++)
	{
		//read data from eeprom,
		//check, if it has changed, only then rewrite;
		//WriteToEEPROM(i, configData[i]);
		//Delay_ms(5);							
	}
	
//	LEDoff;
}

#endif







void init_sys(void)
{
	sys=&syst;
//	cfg=&config;
	sys->mState = GO_FORWARD;		//ǰ��
//	sys->nextState=GO_FORWARD;
//	sys->tBr=0;
	sys->c_hw=sys->m_cHw1=sys->m_cHw2=0;
	//sys->trg_speed=NOMORL_SPEED;
//	sys->trace =0;
//	sys->turn_right=0;
	sys->ajust_agle = 3.0f;
	sys->c_bum = 0;
	sys->slip = 0;
//	sys->cleanMod = CLEAN_RANDOM;
	//sys->c_near_angle = 0;
	msTmr=0;
	sys->power_on = FALSE;
	sys->sState = 0xFF;
//	sys->dust_pwm_value = NORM_DUST_PWM;//����????��?��?��??����a?��
//	sys->ird_en = 0;
	sys->shut_down_motor = 0;
	sys->t_idle = 0;
//	sys->bottom_ir = 0;
	sys->near_debug = 0;
	sys->ajust_agle = (float)cfg->ajust_agle / 10.0f;
	//sys->m_navi = navigat;
	//sys->m_cfg = cfg;
	init_gyro_g();
	 sys->upmap_en_flag = 1;  
	 sys->sync_wifi = 0;
	 sys->wifistatechange = 0;
	 sys->dis_statechange = 0;
	 	 sys->power_go = 0;
	 	 sys->sys_charge_cur = 500;	//2018-10-09 JZZ
	 	 sys->init_prs = 1;
		 sys->prs = 100;

sys->dusk_usetime_alarm= 0;
sys->strainer_usetime_clear= 0;
sys->side_rush_usetime_clear= 0;
sys->main_rush_usetime_clear= 0;
sys->wifiAreaNoCalc_en = 0;

sys->dccheck_enable = DISABLE;

	sys->auto_lamp_sta = 0;
	sys->dust_pwm_lev = 1;

	sys->upgrade_mode = RESET;
	sys->work_mod = MWO_NORMAL;
	sys->f_work_mode = sys->work_mod;
	sys->Blankets_ground = RESET;
	sys->run_speed = MIDLE_SPEED;
	sys->dust_pwm_value = DUST_MODE_NORMAL;	
	sys->pair_sta = REMOTE_PAIR_NU;
	sys->wifi_switch = SET;
	sys->wifi_switch_old = RESET;
}



#if ROOMA_GYROONLY
/*
0	?��D��
1	???��??����
2	1-��?D???����
3	??��???����
4	??3?
5	??��???����
6	��??��??����
7	?y?��3?��?
253	1??��

#define SYS_IDLE		0x00		//???D
#define SYS_NAVIGAT		0x01		//��?o?
#define SYS_FOCUS		0x02		//??��?
#define SYS_RANDOM		0x03		//???��
#define SYS_DOCK		0x04		//??3?
#define SYS_NEAR		0x05		//??��?
#define SYS_CHARGE		0x06		//3?��?
#define SYS_TEST_BD		0x07		//??????2a��o???����o?
#define SYS_POWEROFF	0xfd	//1??��,��?���䨬?????��???��?o����a(��??��?��1.2��?��2?t��?��????T����1?��?)/����???����?,????��?2?1?��?
*/
//?�̨�3���䨬?��a??��?WIFI��?����??
//static uint8_t m_sys_state2wifi_list[8] = {0,6,3,3,4,5,7,7};
static uint8_t m_sys_state2wifi_list[8] = {0,6,3,5,4,5,7,7};
uint8_t sys_state_convert2wifi(void)
{
	if(sys->sState == SYS_POWEROFF)
		return SYS_POWEROFF;
	return m_sys_state2wifi_list[sys->sState];
}
//WIFI��?����??��a??��??�̨�3���䨬?
static uint8_t m_wifi2sysstate_list[8] = {SYS_IDLE,SYS_NAVIGAT,SYS_NAVIGAT,SYS_RANDOM,SYS_FOCUS,SYS_RANDOM,SYS_DOCK,SYS_IDLE};
uint8_t wifi_state_convert2sys(uint8_t wifi_mode)
{
	if(wifi_mode == SYS_POWEROFF)
		return SYS_POWEROFF;
	return m_wifi2sysstate_list[wifi_mode];
}
#endif


void init_gyro_g(void)
{

		kgyro.angle = &(sys->angle);
		kgyro.rpwm = &(sys->rpwm);
		kgyro.lpwm = &(sys->lpwm);
		kgyro.pwm = &(sys->pwm);
		kgyro.gyro_ofs1 = &(cfg->gyro_ofs1);
		kgyro.gyro_ofs2 = &(cfg->gyro_ofs2);
		kgyro.kp2 = &(cfg->kp2);
		kgyro.ki2 = &(cfg->ki2);
		kgyro.kd2 = &(cfg->kd2);
		kgyro.x_org_f = &(navigat->x_org_f);
		kgyro.y_org_f = &(navigat->y_org_f);		
		kgyro.x_org_r = &(navigat->x_org_r);
		kgyro.y_org_r = &(navigat->y_org_r);		
		kgyro.x_org_t = &(navigat->x_org_t);
		kgyro.y_org_t = &(navigat->y_org_t);		
		kgyro.x_org = &(navigat->x_org);
		kgyro.y_org = &(navigat->y_org);	
		kgyro.radius = &(navigat->radius);
		kgyro.hw_grid =&(navigat->hw_grid);
		kgyro.pid = &rpid;
	
}


char check_io(uint16_t valu)
{
 	int i,j;
	for(i=0;i<5;i++)
	{
		if((GPIOB->IDR & valu)!=0)
			return 0;
	    for(j=0;j<10;j++);
	}
	return 1;
}

char check_io_h(uint16_t valu)
{
 	int i,j;
	for(i=0;i<5;i++)
	{
		if((GPIOB->IDR & valu)!=valu)
			return 0;
	    for(j=0;j<10;j++);
	}
	return 1;
}

int disXY(int x,int y)
{
	return(x>y?(x-y):(y-x));
}
int dis_xy(short x,short y)
{
	return(x>y?(x-y):(y-x));
}
float disf_xy(float x,float y)
{
	return(x>y?(x-y):(y-x)); 
}


void init_tim3_camp(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  //  TIM_ICInitTypeDef TIM_ICInitStruct;
  // TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   
   //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
   //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
   /*ʹ��TIM8_CH2(PC7��)��Ϊ������������*/ 


	TIM3->PSC = 71;
	TIM3->ARR = 2000;
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
	
		
		//	CH3
			TIM4->CCMR2 |= 1 << 0;
			TIM4->CCMR2 |= 5 << 4;		//�˲�����Ϊ5
			if (dir == 0)
				TIM4->CCER |= 1 << 9; //�½��ز���
			else 
				TIM4->CCER &= ~(1 << 9); //�����ز���
			TIM4->CCER |= 1 << 8;
			TIM4->DIER |= 1 << 3; 
	
		case 2: //CCR2 PA1 	*/
			TIM3->CCMR1 |= 1 << 8; //CCR2����ͨ������:����
			TIM3->CCMR1 |= 5 << 12;		//�˲�����Ϊ5
		//	if (dir == 0)
		//		TIMER->CCER |= 1 << 5; //�½��ز���
		//	else 
				TIM3->CCER &= ~(1 << 5); //�����ز���
			TIM3->CCER |= 1 << 4; //CCR2ͨ������ʹ��
			TIM3->DIER |= 1 << 2; //CCR2ͨ���������ж�
		/*	case 1: //>CCR1 PA0
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
	TIM3->DIER |= 1 << 0;     //��������ж�
//	MY_NVIC_Init(1, 2, TIM2_IRQChannel, 2);     //�ж�
	TIM3->CR1 = 0x01;    //ʹ�ܶ�ʱ��
	TIM3->SR &= ~(1 << 0);


}

void en_pwm(uint8_t chnl,uint8_t en)
{
//	log_printf("en pwm,chnl=%d,en=%d\r\n",chnl, en);
 	switch(chnl)
 	{
 		case 0:
			TIM3->CCER &=0xCCCC;		//PA8 PWM��ֹ	
 			break;
		case 1:
			if(en)
				TIM3->CCER|=3<<0; //OC1 ���ʹ��
			else
				TIM3->CCER &=0xFFFC;		//PA8 PWM��ֹ
			break;
		case 2:						//PA7,Ŀǰֻ����һ·
			if(en)
				TIM3->CCER|=3<<4; //OC2 ���ʹ��
			else
				TIM3->CCER &=0xFFCF;		//PA9 PWM��ֹ
			break;
		case 3:
			if(en)
				TIM3->CCER|=3<<8; //OC2 ���ʹ��
			else
				TIM3->CCER &=0xFCFF;		//PA9 PWM��ֹ
			break;
		case 4:
			if(en)
				TIM3->CCER|=3<<12; //OC2 ���ʹ��	
			else
				TIM3->CCER &=0xCFFF;		//PA9 PWM��ֹ
			break;
		
	
	
 	}
	
}

void waiting_stop(void)
{
	while(sys->mState !=GO_STOP)
	{
	}
}

void waiting_hw(uint16_t chw)
{
	uint16_t gSta;
	while(chw > sys->m_cHw1)
	{
		get_sensers(&gSta);
		//wy190527 ���������������ײ���ؼ��
		if(gSta & (MASK_BUM_LEFT | MASK_BUM_LEFT2 | MASK_BUM_RIGHT | MASK_BUM_RIGHT2 | MASK_FW_MIDL))
		{
			log_printf("[waiting_hw]found bum!!!!\r\n");
			return;
		}
	}
	log_printf("[waiting_hw]m_cHw1=%d(%d)\r\n",sys->m_cHw1,chw);
}

#define NEAR_LEFT	1
#define NEAR_RIGHT	2

#define NEAR_FORWARD_HW		200
#define NEAR_MAX_TURN		300
#define NEAR_MAX_BACK		40
uint8_t wall_sta=0;
//uint8_t c_bum=0;
uint8_t c_lost_left=0;
void init_near_wall(void)
{
	log_printf("init_near_wall\r\n");
//	sys->cleanMod = CLEAN_NEAR;
	wall_sta=0;
//	c_bum =0;
	motor_run(GO_STOP, 0, 0, 0);
	motor.c_left_hw = motor.c_right_hw = 0;
	motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);
}

#define NER_TURN_PWM	780
void near_wall_mode(uint8_t gsta)
{
//	uint16_t g_sta=0; 
	uint16_t gSta,g_sta1;
	uint8_t n_loop=0,i;
//	uint16_t c_hw;
	get_sensers(&gSta);
	for(i=0;i<5;i++)
	{
		get_sensers(&g_sta1);
		gSta |=g_sta1;
	}

	switch(wall_sta)
	{
		case 0:			//û��������ģʽ
			if(gSta &  ( MASK_FW_LEFT) && sys->mState ==GO_FORWARD)
			{
				log_printf("[nearwall]FW_MIDL,in the left side,turn right...\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				delay(5);
				//motor.c_left_hw = motor.c_right_hw = 0;
				motor_run(GO_RIGTH,NER_TURN_PWM,70,0);		
				n_loop=1;
				wall_sta=NEAR_LEFT;
				//motor_run(GO_F_SLOW, SLOW_PWM, 3000, GO_F_SLOW);
			}
			//if((sys->mState ==GO_FORWARD) || (sys->mState ==GO_F_SLOW))
			{

				if(gSta & (MASK_BUM_LEFT |MASK_BUM_RIGHT))
				{
					//motor.c_left_hw = motor.c_right_hw = 0;
					log_printf("[nearwall]left bum,back,and turn right 20 hw\r\n");
					motor_run(GO_BACK,NOMORL_PWM,100,0);	
					waiting_stop();
					delay(5);
					if((gSta & (MASK_BUM_LEFT |MASK_BUM_RIGHT)) ==MASK_BUM_LEFT)
					{
						log_printf("only left bum,turn 90hw\r\n");
						motor_run(GO_RIGTH,NER_TURN_PWM,50,0);
					}
					else
					{
						log_printf("middle bum ,turn 200chw\r\n");
						motor_run(GO_RIGTH,NER_TURN_PWM,80,0);
					}
					
					log_printf("[nearwall]turn ok,GO_FORWARD and into NEAR_LEFT mode\r\n");
					wall_sta=NEAR_LEFT;
					n_loop=1;
				}	

			}
			break;
		case NEAR_LEFT:

			if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT |  MASK_FW_MIDL ))
			{
				if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT))
				{
					log_printf("[NEAR LEFT]bum!!back ,and ,turn right\r\n");
				}
				else
					log_printf("[NEAR LEFT]left slant,stop and turn right 10 chw\r\n");

				motor_run(GO_STOP, 0, 0, 0);
				//delay(20);
				//motor.c_left_hw = motor.c_right_hw = 0;
				if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT))
				{					
					motor_run(GO_BACK,NOMORL_PWM,80,0);	
					waiting_stop();	
				}
				//if(gSta && MASK_FW_MIDL)
				//	motor_run(GO_RIGTH,NER_TURN_PWM,100,0);
				//else
					motor_run(GO_RIGTH,NER_TURN_PWM,35,0);
				n_loop=1;
				
			}else

			if(gSta & MASK_LEFT)
			{
				motor_run(GO_STOP, 0, 0, 0);
				motor_run(GO_RIGTH,NER_TURN_PWM,10,0);
				n_loop=1;
				c_lost_left=0;
			}else
			{
				c_lost_left ++;
				motor_run(GO_STOP, 0, 0, 0);
				if(c_lost_left >3 )
					motor_run(GO_LEFT,NER_TURN_PWM,30,0);
				else
					motor_run(GO_LEFT,NER_TURN_PWM,10,0);
				n_loop=1;
				log_printf("c lost left=%d\r\n",c_lost_left);
			}
			
			

			break;
		case NEAR_RIGHT:
			log_printf("right side ,no code...\r\n");
			/*
			if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT |MASK_FW_RIGHT| MASK_FW_MIDL))
			{
				if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT))
				{
					c_bum++;
					log_printf("[NEAR RIGHT]bum!!c_bum=%d,back ,and ,turn left\r\n",c_bum);
				}
				else
					log_printf("[NEAR RIGHT]right slant,stop and turn left 10 chw\r\n");

				motor_run(GO_STOP, 0, 0, 0);
				delay(20);
				if(gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT))
				{
					motor_run(GO_BACK,NOMORL_PWM,20,0);	
					waiting_stop();	
					delay(20);
					motor_run(GO_LEFT,NER_TURN_PWM,20,0);
				}else
					motor_run(GO_LEFT,NER_TURN_PWM,10,0);
				n_loop=1;
				
			}else
			if((gSta & MASK_RIGHT)==0)
			{
				log_printf("[NEAR LEFT]lost right wall,turn right\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				delay(20);
				motor_run(GO_RIGTH,NER_TURN_PWM,10,0);
				n_loop=1;
			}
			*/
			break;

	}

	if(n_loop)
	{
		waiting_stop();
	//	delay(20);
		log_printf("[n_loop]turn ok,GO_FORWARD\r\n");
		//motor.c_left_hw = motor.c_right_hw = 0;
		motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);
		//sys->trg_speed2=sys->trg_speed=2200;
		/*
		if(c_bum >=3)
		{
			log_printf("maney bumm.....!!!\r\n");
			waiting_hw(0);
		}
		else
		*/
			waiting_hw(80);
	}
	
#if 0



	if((gSta &  MASK_FW_MIDL ) && sys->mState == GO_FORWARD)
	{
		log_printf("[nearwall]FW_MIDL,slow down...\r\n");
		motor_run(GO_F_SLOW, SLOW_PWM, 3000, GO_F_SLOW);		
	}else if((sys->mState ==GO_FORWARD) || (sys->mState ==GO_F_SLOW))
	{
		if(wall_sta ==NEAR_RIGHT && sys->m_cHw1> NEAR_FORWARD_HW)		//�ұ��ӱ�
		{
			if((gSta & MASK_RIGHT) ==0)		//�ر���ʧ��
			{
				log_printf("[nearwall]right side,lost wall,turn right 10 hw\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				motor_run(GO_RIGTH,NER_TURN_PWM,10,0);
				waiting_stop();
				delay(20);
				log_printf("[nearwall]right side,turn right ok,GO_FORWARD\r\n");
				motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);
			}
		}else if(wall_sta ==NEAR_LEFT&&  sys->m_cHw1> NEAR_FORWARD_HW)
		{
			if((gSta & MASK_LEFT) ==0)		//�ر���ʧ��
			{
				log_printf("[nearwall]left side,lost wall,turn left 10 hw\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				motor_run(GO_LEFT,NER_TURN_PWM,10,0);
				waiting_stop();
				delay(20);
				log_printf("[nearwall]left side,turn left ok,GO_FORWARD\r\n");
				motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);
			}
		}
	}

	if(gSta & MASK_BUM_LEFT)
	{
		log_printf("[nearwall]left bum,back\r\n");
		motor_run(GO_BACK,NOMORL_PWM,NEAR_MAX_BACK,0);	
		waiting_stop();

		if(wall_sta == NEAR_RIGHT)
		{
			log_printf("[nearwall]I am in right,turn left\r\n");
			motor_run(GO_LEFT,NER_TURN_PWM,NEAR_MAX_TURN,0);
		}else
		{
			//���ұ�ת��ֱ��ǰ����̽ͷ��ʧ
			log_printf("[nearwall]turn right and waiting for ird dis\r\n");
			motor_run(GO_RIGTH,NER_TURN_PWM,NEAR_MAX_TURN,0);
			wall_sta=NEAR_LEFT;
		}
		n_loop=1;
	}else if(gSta & MASK_BUM_RIGHT)
	{
		log_printf("[nearwall]right bum,back\r\n");
		motor_run(GO_BACK,NOMORL_PWM,NEAR_MAX_BACK,0);	
		waiting_stop();
		delay(20);
		//���ұ�ת��ֱ��ǰ����̽ͷ��ʧ
		if(wall_sta == NEAR_LEFT)
		{
			log_printf("[nearwall]i am in left,TURN RIGHT\r\n");
			motor_run(GO_RIGTH,NER_TURN_PWM,NEAR_MAX_TURN,0);
		}else
		{
			log_printf("[nearwall]turn left and waiting for ird dis\r\n");
			motor_run(GO_LEFT,NER_TURN_PWM,NEAR_MAX_TURN,0);
			wall_sta=NEAR_RIGHT;
		}
		n_loop=1;
		
	}


	c_hw=0;
	if(n_loop)
		log_printf("begin nloop...\r\n");
	
	while(n_loop)
	{
		get_sensers(&gSta);
		get_sensers(&g_sta1);
		get_sensers(&g_sta2);
		gSta |=g_sta1;
		gSta |=g_sta2;
	//	log_printf("[near_wall_mode]loop,gSta:%02X\r\n",gSta);
		if(sys->mState ==GO_STOP)
		{
			log_printf("[nearwall]motor stop,turn error,GO_FORWARD\r\n");
			motor_run(GO_STOP, 0, 0, 0);
			motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);			
			break;
		}
		//if(((gSta & (MASK_FW_LEFT | MASK_FW_RIGHT | MASK_FW_RIGHT))==0)
		//   && sys->m_cHw1 > 30)




		if(((gSta & (MASK_FW_LEFT | MASK_FW_RIGHT | MASK_FW_RIGHT))==0) && sys->m_cHw1 > 30)
		{
			log_printf("[nearwall]ird Disappear,turn OK,GO_FORWARD\r\n");
			c_hw = sys->m_cHw1;
			delay(20);
			motor_run(GO_STOP, 0, 0, 0);
			/*
			motor_run(GO_STOP, 0, 0, 0);
			while(1)
			{
				proc_msg_printf();
			}*/
			motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);	
			
			break;
		}
		
	}
	/*
	if(c_hw >0)
	{
		while((sys->m_cHw1-c_hw) < 50);

		log_printf("[nearwall]ird Disappear,turn OK,STOP AND GO_FORWARD\r\n");
		motor_run(GO_STOP, 0, 0, 0);
		motor_run(GO_FORWARD,NOMORL_PWM,0,GO_FORWARD);

	}
	*/
#endif	
}
  /*
void send_irda(uint8_t chnl,uint16_t bt)
{
	irda_timer_cfg();
 	sys->tx_byte= bt;
 	sys->chnl = chnl;
 	en_pwm(chnl, 1);						
 	TIM_Cmd(STIME, ENABLE); //���ܶ�ʱ��
 	STIME->ARR = 800;	//�͵�ƽ0.8ms
 	sys->sta=1;			//����������
 	log_printf("[send_irda]chnl=%d,=%02X\r\n",chnl,bt);
 	//���͵�ʱ�򣬰ѽ��յĹص�byte
 	TIM_Cmd(TIM8, DISABLE); 
	
}
 */
#if 0

//���Է�
uint8_t  c_dock_cyc=0;
uint8_t  c_max_cock;
//uint8_t  motor_sta=0;

#define  DOCK_AREA_NO	0x00		//û�ҵ�����
#define  DOCK_AREA_LR	0x01		//����
#define  DOCK_AREA_M	0x02		//  456
#define  DOCK_AREA_L		0x03		//6 7 8
#define  DOCK_OK			0x04

#define DOCK_AREA_S	0xFF

extern char motorStr[7][6];

uint8_t dock_area;		//����

uint16_t c_right;
uint16_t r_count=0,l_count=0;
uint16_t c_lost=0;

#define SITE_NONE	0x00
#define SITE_LEFT  	0x01
#define SITE_RIGHT 	0x02
struct site_t
{
	uint8_t 	site;		//
	uint8_t 	*v_ird;	//�����жϴ�ֱ��IRD
	uint8_t	*m_ird;
	uint8_t	*my_ird;
	uint8_t 	 *m_line_ird;	//�����ߵı�־
	uint8_t    m_dir;	//ת��������ֱ�ķ���	
	uint8_t	back_dir;	//ת��ȥһ��
	uint8_t  	is_middle;
};

struct site_t l_site,r_site,*p_site;

void  docking_init(void)
{
	log_printf("docking_init...\r\n");
	dock_area=DOCK_AREA_NO;
	sys->sState = SYS_DOCK;
	motor_run(GO_RIGTH,DOCK_PWM,3000,GO_STOP);	//ע�⵽������Ҫ�õ� sys->nextState
	
	l_site.v_ird  = &iLeft.l_360;
	l_site.m_dir = GO_LEFT;
	l_site.back_dir = GO_LEFT;
	l_site.m_ird =  &iL360.right;
	l_site.my_ird =  &iL360.left;
	l_site.m_line_ird = &iLeft.left;

	r_site.v_ird = &iRight.r_360;
	r_site.m_dir = GO_RIGTH;
	r_site.back_dir = GO_RIGTH;
	r_site.m_ird = &iL360.left;
	r_site.my_ird =  &iL360.right;
	r_site.m_line_ird = & iLeft.left;
	

}

#define DOCK_RIGHT_CHW		300
#define DOCK_INIT_CHW			800

uint8_t sta_site=0;
uint8_t cleft=0;
//�������



	
char docking_site(void)
{
	switch(sta_site)
	{
		case 0:		//��ߣ������ת����ת��Ѱ�Ҵ�ֱ��
			//û�ҵ��Ǹ��㣬ת��ͷ�ˣ���ʧ�ܣ�������ô����û���
			if(sys->m_cHw1 >=1000)
			{
				log_printf("[docking_site]not found V,m_cHw1 >=1000\r\n");
				sta_site=20;
				break;
			}
			if(sys->mState == GO_STOP)
			{
				log_printf("[docking_site]back OK,now GO_FORWARD for founding middle...\r\n");
				motor_run(GO_FORWARD, cfg->dock_pwm, 4000, GO_STOP);
				sys->trg_speed=sys->trg_speed2=cfg->dock_speed;
				sta_site = 1;		//����ֱ�н׶�
				p_site->is_middle = 0;
				cleft=0;
			}else
			{
				if( *p_site->v_ird ==0)		//��ʧ�ˣ�֤����ֱ�ˡ�
				{
					log_printf("[docking_site]v_ird lost,we found V!!,now go little back %d...\r\n",cfg->dock_hw_back);
					motor_run(GO_STOP, 0, 0, 0);
					//��΢ת��ȥһ��
					motor_run(p_site->back_dir, cfg->dock_pwm, cfg->dock_hw_back, GO_STOP);
					waiting_stop();
				}
			}
			break;
		case 1:	//ֱ�н׶Σ�Ѱ�����ĵ�
		/*
			//����Ҫ�����ж�
			if(sys->m_cHw1 >=2000)
			{
				log_printf("[docking_site]not found M,m_cHw1 >=2000\r\n");
				sta_site=20;
				break;
			}

			*/
			/*
			if(sys->mState == GO_STOP)		//ֹͣ�ˣ�Ҫת��ȥ��
			{
				log_printf("[docking_site]found M,now turn to middle\r\n");
				p_site->is_middle =0;
				motor_run(p_site->m_dir, 870, 2000, GO_STOP);
				sta_site = 2;
				break;
			}else
			*/
			//if(sys->mState == GO_FORWARD)
			{

			//	if()
				//if(*p_site->m_ird && *p_site->my_ird==0 )			//�����м���ˣ���ֱ��һ��
				if(((iL360.left==0 && iL360.right >0) && (p_site->back_dir == GO_LEFT)) ||
					((iL360.right==0 && iL360.left>0) && (p_site->back_dir == GO_RIGTH)) )
				{ 
					if(cleft++ >=2/**/)
					{
						
						log_printf("[docking_site]found Middle,stop and still go %d chw...\r\n",cfg->dock_hw_forwak);
						motor_run(GO_STOP, 0, 0, 0);
						//motor_run(GO_FORWARD, 860, 170, GO_STOP);
						motor_run(GO_FORWARD, cfg->dock_pwm+60, cfg->dock_hw_forwak, GO_STOP);
						sys->trg_speed=sys->trg_speed2=0;
						sta_site = 2;
					}
				}
			}
			break;
		case 2:
			if(sys->mState == GO_STOP)		//ֹͣ�ˣ�Ҫת��ȥ��
			{
				log_printf("[docking_site]go 170 over,now turn to middle\r\n");
				p_site->is_middle =0;
				
				motor_run(p_site->m_dir, cfg->dock_pwm+30, 2000, GO_STOP);
				sta_site = 3;
				cleft=0;
				
				break;
			}	
			break;
		case 3:		//Ѱ��������
			if(sys->m_cHw1 >=1000)
			{
				log_printf("[docking_site]found middle line ERROR\r\n");
				sta_site=20;
				break;
			}
			//if(iLeft.left)
			//	p_site->is_middle|=1;
			//if(iRight.right)
			//	p_site->is_middle |=2;
			//if((p_site->is_middle & 3) ==3)
			//if(*p_site->m_line_ird)
			//if( (iLeft.left &&  (iRight.right || iRight.left) && p_site->m_dir == GO_LEFT)	||
			//	(iRight.right&&  (iLeft.right || iLeft.left) && p_site->m_dir == GO_RIGTH))
			 if(iLeft.left &&  (iRight.right || iRight.left) )
			
			 {
			 	if(cleft++ > cfg->dock_hw_turn)
			 	{
					log_printf("[docking_site]found middle line OK\r\n");
					motor_run(GO_STOP, 0, 0, 0);
					//while(1);
					return 1;
				}
			}
			break;
		default:
			log_printf("[docking_site]error,sta_site=%d\r\n",sta_site);
			//return 2;
			break;
	}
	return 0;
		
}
uint8_t sta_midle=0;
int m_pwm=0;
#define M_PWM	770
char docking_middle(void)
{
	char i,c=0,j;
	switch(sta_midle)
	{
		case 0:		

			if(iLeft.left || iRight.right)	//����ֱ��
			{
				if(sys->mState !=GO_FORWARD)
				{
					log_printf("[dock_middle]moter sta!=FORWARD,now GO_FORWARD\r\n");
					motor_run(GO_STOP, 0, 0, 0);
					motor_run(GO_FORWARD, cfg->dock_pwm+30, 3000, GO_STOP);
				}

				if(iLeft.left && iRight.right)		//���м�λ��
				{
					log_printf("[dock_middle]left && right,we are in the middle of the middle,GO_FORWARD...\r\n");
					motor_run(GO_FORWARD, cfg->dock_m_pwm, 3000, GO_STOP);
					sta_midle=1;
					
				}				
			}else 
			{
				if(sys->mState == GO_FORWARD )
				{
					if(iLeft.right)
					{
						log_printf("[dock_middle]lost the midle,in the right,mast go left\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						motor_run(GO_LEFT, cfg->dock_pwm, 1000, GO_STOP);
					}else if(iRight.left)
					{
						log_printf("[dock_middle]lost the midle,in the left,mast go right\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						motor_run(GO_RIGTH, cfg->dock_pwm, 1000, GO_STOP);
					}else if(iL360.left )
					{
						log_printf("[dock_middle]lost the midle,in the left left,mast go right\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						motor_run(GO_RIGTH, cfg->dock_pwm, 1000, GO_STOP);
					}else
					{
						log_printf("[dock_middle]lost the midle,in the right right,mast go right\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						motor_run(GO_LEFT, cfg->dock_pwm, 1000, GO_STOP);

					}
				}
			}
	
			break;
		case 1:
			//log_printf("MMMMM\r\n");
			if(iLeft.left && iRight.right)	//����ֱ��
			{
				if(sys->mState !=GO_FORWARD)
				{
					log_printf("MM,moter sta != forward,now GO_FORWARD\r\n");
					motor_run(GO_STOP, 0, 0, 0);
					motor_run(GO_FORWARD, cfg->dock_m_pwm, 3000, GO_STOP);
				}
	
			}else if(iLeft.left)
			{
				log_printf("MM,in the LEFT,now to the right\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				motor_run(GO_RIGTH, cfg->dock_m_pwm, 3000, GO_STOP);				
			}else 
			{
				log_printf("MM,in the RIGHT,now to the left\r\n");
				motor_run(GO_STOP, 0, 0, 0);
				motor_run(GO_LEFT, cfg->dock_m_pwm, 3000, GO_STOP);	
			}
			c=0;
			for(i=0;i<50;i++)
			{
				if((GPIOE->IDR & 1) ==0)
					c++;
				for(j=0;j<8;j++);
			}
			if(c >=40)
			{
				log_printf("charge OK,STOP \r\n");
				motor_run(GO_STOP, 0, 0, 0);
				return 1;
			}
			break;
	}
	return 0;
}



	
void docking(void)
{
	char i,c,j;
//����� ��3
/**/
	if(sys_debug.debug ==DEBUG_SES)
	{
		log_printf("(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)\r\n",
		iLeft.l_360,iLeft.left,iLeft.right,iLeft.r_360,
		iL360.l_360,iL360.left,iL360.right,iL360.r_360,
		iRight.l_360,iRight.left,iRight.right,iRight.r_360);
	}


  /*
	iLeft.left=iLeft.right=iLeft.l_360=0;
	
	iRight.left=iRight.right=iRight.l_360=0;
	iL360.left=iL360.right=iL360.l_360=0;	
	iLeft.r_360=iRight.r_360=iL360.r_360=0;
	return ;
	*/		
	//���㣬ȥ������������ݣ�����ƫ��ƫ�ҡ�
	switch (dock_area)
	{
		case DOCK_AREA_NO:
		//	log_printf("[DOCK_AREA_NO]m_cHw1=%d,%d\r\n",sys->m_cHw1,iLeft.l_360);
			if(sys->mState == GO_RIGTH)
			{
				if(sys->m_cHw1 < 1000)		//��һȦ����
				{
					if(iLeft.l_360)		//���������
					{
						dock_area = DOCK_AREA_LR;	
						log_printf("[DOCK_AREA_NO]found left side.we will turn right...\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						p_site = &l_site;
						sta_site=0	 ;
						motor_run(GO_RIGTH, cfg->dock_pwm, 2000, GO_STOP);
						
						//dock_area=20;
					}else if(iRight.r_360)  //�����ұ�
					{
						log_printf("[DOCK_AREA_NO]found RIGHT side.we will turn left...\r\n");
						dock_area = DOCK_AREA_LR;		
						motor_run(GO_STOP, 0, 0, 0);
						p_site = &r_site;
						sta_site=0	 ;
						motor_run(GO_LEFT, cfg->dock_pwm, 2000, GO_STOP);
						
					}
					/*
					else if(iLeft.left &&  iRight.right) 
					{
						log_printf("[DOCK_AREA_NO]found the middle.\r\n");
						motor_run(GO_STOP, 0, 0, 0);
						dock_area = DOCK_AREA_M;
						//motor_run(GO_FORWARD, DOCK_PWM, 3000, GO_STOP);
						sta_midle =0;						
					}
					*/
				}
			}
			break;
		case DOCK_AREA_LR:
			switch(docking_site())
			{
				case 1:		//�ҵ��м����
					dock_area = DOCK_AREA_M;
					//motor_run(GO_FORWARD, DOCK_PWM, 3000, GO_STOP);
					sta_midle =0;
					break;
				case 2:		//ʧ��

					break;
				default :
					break;
			}
			break;
		case DOCK_AREA_M:
		/**/
			switch(docking_middle())
			{
				case 1:
					dock_area = DOCK_OK;
					break;
				case 2:
					break;
				default:
					break;
			}
			
			break;
		case DOCK_OK:
			c=0;

			for(i=0;i<60;i++)
			{
				if((GPIOE->IDR & 1))
					c++;
				for(j=0;j<10;j++) ;
			}
			if(c >=50)
			{
				log_printf("charge lost,init docking...\r\n\r\n");
				docking_init();
			}
			
			break;
		default:
			break;
	}


	iLeft.left=iLeft.right=iLeft.l_360=0;
	
	iRight.left=iRight.right=iRight.l_360=0;
	iL360.left=iL360.right=iL360.l_360=0;	
	iLeft.r_360=iRight.r_360=iL360.r_360=0;
	
}


struct r_ird_t iLeft,iRight,iL360;
#define MAX_RX_BIT		8


void rx_ird(struct r_ird_t *rx_ird)
{

		 uint32_t ccr,t_now;
		 int idx;
		 t_now= sys->tird + STIME->CNT;		//��ȡ��ǰ��ʱ��
		ccr = t_now-rx_ird->t_last;				//����ʱ���
		rx_ird->t_last=t_now;

	//	log_printf("ccr:%d\r\n",ccr);
		//ʱ��̫���ˣ����¿�ʼ����,�ڶ��������أ��������룬ȥ��
		if(ccr >=300)
		{
			rx_ird->rx_idx1=rx_ird->rx_byte1=rx_ird->rx_rbe1=0;
		}

		

		if(rx_ird->rx_idx1 > 0  && rx_ird->rx_idx1 <=MAX_RX_BIT)  //ȥ����������
		{
			  idx=rx_ird->rx_idx1-1;

			  if(ccr >=60 && ccr < 135)		// 0 ��1000Ϊ����
			  {
				// if(ccr<60 || ccr >80)
				 //	rx_ird->rx_rbe1++;
				 	//log_printf("far ccr=%d\r\n",ccr);
			  }
			  else if(ccr >=145 && ccr < 200)	  // 1 ��1200Ϊ����
			  {
				rx_ird->rx_byte1 |=(1 <<idx);
				// if(ccr<90 || ccr >130)
				 //	rx_ird->rx_rbe1++;
				 				
			  }else
			  {
				//log_printf("rx error,idx=%d,ccr=%d\r\n",rx_ird->rx_idx1,ccr);
			  	rx_ird->rx_idx1=rx_ird->rx_byte1=rx_ird->rx_rbe1=0;
			  	return ;
			  }

	
			  
			  if(rx_ird->rx_idx1 ==MAX_RX_BIT)
			  {
			  	//if(sys->trace)
			  //	log_printf("rx:%X,%d\r\n",rx_ird->rx_byte1,rx_ird->rx_rbe1);
			  //	insert_irda(sys->rx_byte1);
			  /**/
			  	if(rx_ird->rx_byte1==0xE1)
			  		rx_ird->right++;
			  	else if(rx_ird->rx_byte1==0xD2)
			  		rx_ird->l_360++;
			  	else if(rx_ird->rx_byte1==0xC3)
			  		rx_ird->left++;		
		  		else if(rx_ird->rx_byte1==0xB4)
			  		rx_ird->r_360++;		
			  	
			  	rx_ird->rx_idx1=rx_ird->rx_byte1=rx_ird->rx_rbe1=0;
			  }

			
		}
		rx_ird->rx_idx1++;
}
#endif

//�ҳ������������Ǹ�����
int find_big_count( int *n ,int nNum )
{

	int i,j;
	int nTmp;
	int p[MAX_ADJ_XY] ;
	int idx=0;

	for(i=0;i<MAX_ADJ_XY;i++)
		p[i] = 0;
	for ( i = 0; i < nNum; i++)
	{
	
	    for ( j = 0; j < nNum;j++)
	    {
	        if (n[i] == n[j])
	        {
	            p[i]++;
	        }
	    }
	}
	nTmp = p[0];
	idx = 0;
	for ( i = 0; i< nNum;i++)
	{
	    if (p[i] >= nTmp)
	    {
	        nTmp = p[i];
	        idx = i;
	    }
	}
	if(idx >=MAX_ADJ_XY)
		return n[0];
	else
	 return n[idx];
}
 

void init_robt_run_ctrl(int deg,int ctrl)
{
	
	//������
	if(sys->walk_ctrl )	//�Ѿ������ˣ���ֱ��ˢ�¼���������
		mstimeout(&sys->t_work,0);
	if(ctrl)
	{
		mstimeout(&sys->t_work,0);
		if(deg == 0)	//ֱ��
		{
			motor_run(GO_FORWARD, 550, 0, 0);
		}else if(deg == 180)
		{
			motor_run(GO_BACK,550,0,0);
		}else if(deg == 90 || deg == 270)
		{
			turn_to_deg(deg);
			motor_run(GO_FORWARD, 550, 0, 0);
		}
	}
	sys->walk_ctrl = ctrl;
}
void robot_run_ctrl(void)
{
	if(sys->walk_ctrl)	//��������
	{
		if(mstimeout(&sys->t_work,500))
		{
			motor_run(GO_STOP,0,0,0);
			sys->walk_ctrl = 0;
		}

	}
}
uint8_t in_area(int16_t x,int16_t bx,int16_t ex)
{
	int16_t min,max;
	max = bx >= ex?bx:ex;
	min = bx <= ex?bx:ex;
	if(x >= min && x<=max)
	{
		return 1;
	}
	return 0;
}

