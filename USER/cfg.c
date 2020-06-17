/*
#include "stm32f10x.h"
#include "cfg.h"

#include <string.h>
#include "usart.h"
#include "motor.h"
*/
#include "stm32f10x_flash.h"
#include "sys.h"

struct cfg_t *cfg;


char cfg_buf[MAX_CFG_LEN];

struct sys_debug_t sys_debug;



uint16_t NOMORL_PWM,SLOW_PWM;

void read_Flashbuf(u32 addr,u8 *p,u16 n)
{
	while(n--)
	{
		*(p++)=*((u8*)addr++);
	}
}



void init_cfg(void)
{
	char is_save=0;
	cfg=(struct cfg_t*)cfg_buf;

	//reset_cfg();//����ʹ�ã�����ɾ��
	read_Flashbuf(CFG_ADDER,(u8 *)cfg_buf,MAX_CFG_LEN);
	if(cfg->magic !=MAGIC)
	{
		//log_printf("read flash error,reset it\r\n");
		reset_cfg();
	}
	if(cfg->cliff_max ==0xFFFF || 		//���´���������
	cfg->cliff_ala_max==0xFFFF ||
	cfg->cliff_ala_tmr==0xFFFF ||
	cfg->t_drop_max==0xFFFF ||
	cfg->v_dust_max==0xFFFF ||
	cfg->v_dust_cnt==0xFFFF )
	{
		log_printf("reset cfg...\r\n");
		reset_cfg();

	}
/*

	if(cfg->dock_m_pwm <100 && cfg->dock_m_pwm > 999)
	{
		cfg->dock_m_pwm=770;
		log_printf("dock_m_pwm error,reset\r\n");
		is_save =1;
	}
	if(cfg->dock_speed<2000 && cfg->dock_m_pwm > 6000)
	{
		cfg->dock_speed=4000;
		log_printf("dock_speed error,reset\r\n");
		is_save =1;
	}

	//if(cfg->pwm_normal<100 && cfg->pwm_normal > 999)
	if(cfg->pwm_normal<100 || cfg->pwm_normal > 999)
	{
		cfg->pwm_normal=650;
		log_printf("pwm_normal error,reset\r\n");
		is_save =1;
	}
	//if(cfg->pwm_slow<100 && cfg->pwm_slow > 999)
	if(cfg->pwm_slow<100 || cfg->pwm_slow > 999)
	{
		cfg->pwm_slow=720;
		log_printf("pwm_normal error,reset\r\n");
		is_save =1;
	}
*/


	//if(cfg->ajust_agle < 10 && cfg->ajust_agle > 100)
	if(cfg->ajust_agle < 10 || cfg->ajust_agle > 100)
	{
		cfg->ajust_agle = 25;
		log_printf("ajust_agle error,reset\r\n");
		is_save =1;
	}
	if( cfg->gyro_ajust > 100)
	{
		cfg->gyro_ajust = 25;
		is_save =1;

	}
	//----------------------------add by wonton2004 20170518
	if(cfg->stop_timr == 0xffff)
	{
		cfg->stop_timr = 10;
	}
	if(cfg->t_agitator_inspection > 2000)
	{
		cfg->t_agitator_inspection = 0;
		is_save = 1;
	}
	if(cfg->t_agitator_replacemen> 2000)
	{
		cfg->t_agitator_replacemen = 0;
		is_save = 1;
	}
	//cfg->have_irda = 300;
	//cfg->line_err = 5;
	NOMORL_PWM = 650;
	SLOW_PWM = 750;

//	cfg->go_forward_pwm = 580;
//	cfg->go_back_pwm	= 650;
//	cfg->go_route_pwm	= 450;
	//if(cfg->slow_adc < 100 || cfg->slow_adc >=4000)
	//{
	//	cfg->slow_adc = 600;
	//	is_save = 1;
	//}
#if 0


	if(cfg->midle_adc< 100 || cfg->midle_adc >=4000)
	{
		cfg->midle_adc = 1800;
		is_save = 1;
	}
	if(cfg->min_right_adc  < 10 || cfg->min_right_adc > 4000)
	{
		cfg->min_right_adc = 600;
		is_save = 1;
	}

	if(cfg->lock_right_adc  < 10 || cfg->lock_right_adc > 4000)
	{
		cfg->lock_right_adc = 1200;
		is_save = 1;
	}

	if(cfg->max_right_adc  < 10 || cfg->max_right_adc > 4000)
	{
		cfg->max_right_adc = 3000;
		is_save = 1;
	}


	if(cfg->side_right_adc  < 10 || cfg->side_right_adc > 4000)
	{
		cfg->side_right_adc = 800;
		is_save = 1;
	}

	if(cfg->lost_right_adc  < 10 || cfg->lost_right_adc > 4000)
	{
		cfg->lost_right_adc = 300;
		is_save = 1;
	}



	if(cfg->min_left_adc  < 10 || cfg->min_left_adc > 4000)
	{
		cfg->min_left_adc = 600;
		is_save = 1;
	}

	if(cfg->lock_left_adc  < 10 || cfg->lock_left_adc > 4000)
	{
		cfg->lock_left_adc = 1200;
		is_save = 1;
	}

	if(cfg->max_left_adc  < 10 || cfg->max_left_adc > 4000)
	{
		cfg->max_left_adc = 3000;
		is_save = 1;
	}


	if(cfg->side_left_adc  < 10 || cfg->side_left_adc > 4000)
	{
		cfg->side_left_adc = 800;
		is_save = 1;
	}

	if(cfg->lost_left_adc  < 10 || cfg->lost_left_adc > 4000)
	{
		cfg->lost_left_adc = 300;
		is_save = 1;
	}
	if(cfg->m_left_adc < 10 || cfg->m_left_adc > 3000)
	{
		cfg->m_left_adc = 1300;
		is_save = 1;
	}
	if(cfg->m_right_adc < 10 || cfg->m_right_adc > 3000)
	{
		cfg->m_right_adc = 800;
		is_save = 1;
	}
	if(cfg->max_left2_adc< 100 || cfg->max_left2_adc > 5000)
	{
		cfg->max_left2_adc = 2000;
		is_save = 1;
	}
	if(cfg->max_right2_adc< 100 || cfg->max_right2_adc > 5000)
	{
		cfg->max_right2_adc = 2000;
		is_save = 1;
	}
#else
	#if 1
	if(cfg->midle_adc< 100 || cfg->midle_adc >=4000)
	{
		cfg->midle_adc = 2000;
		is_save = 1;
	}
	if(cfg->min_right_adc  < 10 || cfg->min_right_adc > 4000)
	{
		cfg->min_right_adc = 1000;
		is_save = 1;
	}

	if(cfg->lock_right_adc  < 10 || cfg->lock_right_adc > 4000)
	{
		cfg->lock_right_adc = 1200;
		is_save = 1;
	}

	if(cfg->max_right_adc  < 10 || cfg->max_right_adc > 4000)
	{
		cfg->max_right_adc = 3400;
		is_save = 1;
	}

	if(cfg->side_right_adc  < 10 || cfg->side_right_adc > 4000)
	{
		cfg->side_right_adc = 1000;
		is_save = 1;
	}
/*
	if(cfg->lost_right_adc  < 2 || cfg->lost_right_adc > 4000)
	{
		cfg->lost_right_adc = 100;
		is_save = 1;
	}
	*/



	if(cfg->min_left_adc  < 10 || cfg->min_left_adc > 4000)
	{
		cfg->min_left_adc = 1000;
		is_save = 1;
	}

	if(cfg->lock_left_adc  < 10 || cfg->lock_left_adc > 4000)
	{
		cfg->lock_left_adc = 1200;
		is_save = 1;
	}
	if(cfg->max_left_adc  < 10 || cfg->max_left_adc > 4000)
	{
		cfg->max_left_adc = 3400;
		is_save = 1;
	}


	if(cfg->side_left_adc  < 10 || cfg->side_left_adc > 4000)
	{
		cfg->side_left_adc = 1000;
		is_save = 1;
	}
	/*
	if(cfg->lost_left_adc  < 1 || cfg->lost_left_adc > 4000)
	{
		cfg->lost_left_adc = 100;
		is_save = 1;
	}
	*/
	if(cfg->m_left_adc < 10 || cfg->m_left_adc > 3000)
	{
		cfg->m_left_adc = 450;
		is_save = 1;
	}
	if(cfg->m_right_adc < 10 || cfg->m_right_adc > 3000)
	{
		cfg->m_right_adc = 450;
		is_save = 1;
	}

	//if(cfg->max_right2_adc< 100 || cfg->max_right2_adc > 5000)
	{
		//cfg->max_right2_adc = 2200;
		//is_save = 1;
	}
	if(cfg->c_pid_near <2 || cfg->c_pid_near > 200)
	{
		cfg->c_pid_near = 12;
		is_save = 1;
	}
	if(cfg->a_pid_near < 20 || cfg->a_pid_near > 500)
	{
		cfg->a_pid_near = 40;
		is_save = 1;
	}
	#else
	if(cfg->midle_adc< 100 || cfg->midle_adc >=4000)
	{
		cfg->midle_adc = 700;
		is_save = 1;
	}
	if(cfg->min_right_adc  < 10 || cfg->min_right_adc > 4000)
	{
		cfg->min_right_adc = 400;
		is_save = 1;
	}

	if(cfg->lock_right_adc  < 10 || cfg->lock_right_adc > 4000)
	{
		cfg->lock_right_adc = 900;
		is_save = 1;
	}

	if(cfg->max_right_adc  < 10 || cfg->max_right_adc > 4000)
	{
		cfg->max_right_adc = 2000;
		is_save = 1;
	}


	if(cfg->side_right_adc  < 10 || cfg->side_right_adc > 4000)
	{
		cfg->side_right_adc = 800;
		is_save = 1;
	}

	if(cfg->lost_right_adc  < 10 || cfg->lost_right_adc > 4000)
	{
		cfg->lost_right_adc = 300;
		is_save = 1;
	}



	if(cfg->min_left_adc  < 10 || cfg->min_left_adc > 4000)
	{
		cfg->min_left_adc = 400;
		is_save = 1;
	}

	if(cfg->lock_left_adc  < 10 || cfg->lock_left_adc > 4000)
	{
		cfg->lock_left_adc = 900;
		is_save = 1;
	}

	if(cfg->max_left_adc  < 10 || cfg->max_left_adc > 4000)
	{
		cfg->max_left_adc = 2000;
		is_save = 1;
	}


	if(cfg->side_left_adc  < 10 || cfg->side_left_adc > 4000)
	{
		cfg->side_left_adc = 800;
		is_save = 1;
	}

	if(cfg->lost_left_adc  < 10 || cfg->lost_left_adc > 4000)
	{
		cfg->lost_left_adc = 300;
		is_save = 1;
	}
	if(cfg->m_left_adc < 10 || cfg->m_left_adc > 3000)
	{
		cfg->m_left_adc = 450;
		is_save = 1;
	}
	if(cfg->m_right_adc < 10 || cfg->m_right_adc > 3000)
	{
		cfg->m_right_adc = 450;
		is_save = 1;
	}
	if(cfg->max_left2_adc< 100 || cfg->max_left2_adc > 5000)
	{
		cfg->max_left2_adc = 650;
		is_save = 1;
	}
	if(cfg->max_right2_adc< 100 || cfg->max_right2_adc > 5000)
	{
		cfg->max_right2_adc = 650;
		is_save = 1;
	}
	#endif
#endif


	if(cfg->gyro_ofs1 >300)
	{
		cfg->gyro_ofs1 = 13;
		is_save = 1;
	}
	if(cfg->gyro_ofs2 >300)
	{
		cfg->gyro_ofs2 = 13;
		is_save = 1;
	}


	//test
//	cfg->agle_180_offset = 900;		//����0.9��


	//PID��ز�����ȱʡֵ��ʼ��-----------------add by wonton2004 20170518
	if(cfg->kp1 == 0xffff)
	{
		cfg->kp1 = 8;
	}
	if(cfg->ki1 == 0xffff)
	{
		cfg->ki1 = 2;
	}
	if(cfg->kd1 == 0xffff)
	{
		cfg->kd1 = 10;
	}
	if(cfg->kp2 == 0xffff)
	{
		cfg->kp2 = 15200;
	}
	if(cfg->ki2 == 0xffff)
	{
		cfg->ki2 = 60;
	}
	if(cfg->kd2 == 0xffff)
	{
		cfg->kd2 = 23550;
	}
//    if(cfg->setlostturnangle>300)
	{
	//	cfg->setlostturnangle = 7;
	//	is_save = 1;


    }


	if(cfg->l_c_turn >100)
		{
			cfg->l_c_turn = 12;
			is_save = 1;
		}

	if(cfg->dock_l_count >100)
		   {
			   cfg->dock_l_count = 13;
			   is_save = 1;
		   }
	if(cfg->dock_r_count >100)
		   {
			   cfg->dock_r_count = 12;
			   is_save = 1;
		   }
	//if(cfg->setsidemaxpwm>1000)
		{
	//		cfg->setsidemaxpwm = 875;
		//	is_save = 1;
	    }
	//if(cfg->setsideminpwm>800)
		{
		//	cfg->setsideminpwm = 500;
		//	is_save = 1;
	    }
//	if(cfg->max_near_lost>50)
		{
	//		cfg->max_near_lost = 10;
		//	is_save = 1;
	    }

      if(cfg->t_phts_led > 60000)
      {
		cfg->t_phts_led = 3000;		//3����
		is_save = 1;
      }
       if(cfg->v_phts_led> 10000)
      {
		cfg->v_phts_led = 4000;
		is_save = 1;
      }

	if(cfg->dust_full_time_alarm>4200)
			{
				cfg->dust_full_time_alarm = 420;
				is_save = 1;
			}
	if(cfg->spot_speed_maxc ==0)
	{
		cfg->spot_speed_maxc = 50040;
		is_save= 1;
	}
	if(cfg->spot_step_disa ==0)
	{
		cfg->spot_step_disa = 2090;
		is_save= 1;
	}


			/*
  if(cfg->strainer_use_time_alarm>50)
			{
				cfg->strainer_use_time_alarm = 10;
				is_save = 1;
			}
  if(cfg->side_rush_use_time_alarm>50)
			{
				cfg->side_rush_use_time_alarm = 10;
				is_save = 1;
			}
  if(cfg->main_rush_use_time_alarm>50)
			{
				cfg->main_rush_use_time_alarm = 10;
				is_save = 1;
			}

*/











	//log_printf("dock_fw:%d,fw_chw=%d,hw_back=%d,hw_turn\r\n",cfg->dock_hw_forwak,cfg->dock_hw_forwak,cfg->dock_hw_turn);
//	log_printf("dock_pwm:%d,dock_m_pwm=%d,dock_speed=%d\r\n",cfg->dock_pwm,cfg->dock_m_pwm,cfg->dock_speed);
//	log_printf("pwm_normal:%d,pwm_slow=%d\r\n",cfg->pwm_normal,cfg->pwm_slow);
	log_printf("ajust_agle=%d,gyro_ajust=%d\r\n",cfg->ajust_agle,cfg->gyro_ajust);
//	log_printf("midle_adc=%d,slow_adc=%d\r\n",cfg->midle_adc,cfg->slow_adc);
//	log_printf("max_left_adc=%d,max_right_adc=%d\r\n",cfg->max_left_adc,cfg->max_right_adc);
	//log_printf("dock_delay=%d,dock_l_count=%d,dock_r_count%d\r\n",cfg->dock_delay,cfg->dock_l_count,cfg->dock_r_count);
	if(is_save)
		save_cfg();
}



void reset_cfg(void)
{
		memset(cfg_buf,0x00,MAX_CFG_LEN);
		cfg->magic = MAGIC;

		cfg->kp1 = 8;
	 	cfg->kp2 = 9000;///wy190601ԭֵ��1/2 10000;//11200//5000;
		cfg->ki1 = 2;
		cfg->ki2 = 10;  //30
		cfg->kd1 = 10;
		cfg->kd2 = 10000;///wy190601ԭֵ��1/2����16000;//20550;
		cfg->ajust_agle 	= 35;




		cfg->m_left_adc   	= 1500;
		cfg->m_right_adc 	= 1500;

		cfg->midle_adc   	= 1200;

		cfg->stop_timr		= 10;

		cfg->min_left_adc	= 500;
		cfg->lock_left_adc	= 800;
		cfg->max_left_adc 	= 3400;
		cfg->slow_left	= 5200;
		cfg->side_left_adc	= 1000;
		cfg->lost_left_adc	= 100;

		cfg->min_right_adc	= 500;
		cfg->lock_right_adc	= 800;
		cfg->max_right_adc	= 3400;
		cfg->slow_right	=5200;
		cfg->side_right_adc	= 1000;
		cfg->lost_right_adc = 100;
		cfg->slow_mid  = 5000;
		cfg->gyro_ofs1		= 3;
		cfg->gyro_ofs2		= 5;

		cfg->c_pid_near 	= 12;
		cfg->a_pid_near 	= 40;

		cfg->steam_clean_id= 9;



        cfg->dust_full_time_alarm=420;  //60*7=420



// 35
	  cfg->l_c_turn = 12;			//��һ��תȦ��ת�������ߵĸ���
	  cfg->dock_l_delay=300;
	  cfg->dock_l_count=5;			//ת���������ߵĸ���

	  cfg->r_c_turn=5;
	  cfg->dock_r_delay=400;
	  cfg->dock_r_count=5;			//ת���������ߵĸ���


	  cfg->h_run_speed = 350; 	//����
  	  cfg->m_run_speed = 300; 	//����ֱ����ʱ
	  cfg->l_run_speed = 250; 	//����ת�����ߵļ������

//��ˢ�ٶ�
	  cfg->h_agi_speed = 1000;       //��yuduoҪ�� �ߵ���ˢռ�ձ�100% ԭ150 20200515
	  cfg->l_agi_speed = 100;

	  cfg->turn_speed = 20;
	  cfg->nearwall_pwm = 800;
	  cfg->nearwall_hw=300;

	  cfg->nearwall_lost = 45;
	  cfg->nearwall_cnt = 80;


	  cfg->nearwall_dit = 1300;		//��ˢ�澯����
	  cfg->t_agi_max = 3000;		//��ˢ�澯ʱ��
	  cfg->i_agi_max = 300;	//��ˢ����ʱ��
	  cfg->i_left_whele_max = 50;

	  cfg->t_dust_box_max = 2000;		//���ϼ��ʱ��

	  cfg->i_whele_max = 30; 	//����澯����
	  cfg->t_whele_max = 3000; 	//����澯ʱ��

	  cfg->cliff_max = 1000;		//���´���������    1500    ��̺�²��� ��Ϊ1000  20200521
	  cfg->cliff_ala_max=2200;	//�澯����
	  cfg->cliff_ala_tmr=600;//600;	//�澯ʱ��������10����,���Գ������ó�10���ӣ��������

	  cfg->t_drop_max=299; 	//�����������ޣ�˫�����2�룬�������6��
	  cfg->v_dust_max=400; 	//�ҳ��ȷ���ֵ
	  cfg->v_dust_cnt=1000; 	//�ҳ��ȼ���

	  cfg->v_phts_led = 4000; 	  //�������������
	  cfg->t_phts_led = 3000; 	  //���������ʱ��


		cfg->spot_speed_maxc = 30040;
		cfg->spot_step_disa = 890;

		cfg->walk_pwm = 700;
		cfg->turn_pwm = 700;
		cfg->slow_pwm = 100;

		cfg->r[IC_DOCK_TURN_SPEED] = 1010;
		cfg->r[IC_DOCK_M_WALK_SPEED] = 40;;
		cfg->r[IC_DOCK_WALK_SPEED] = 50;
		cfg->r[IC_DOCK_SET_ANGLE] = 2015;
		cfg->r[IC_AGI_PID] = 15;
		cfg->r[IC_BUBER_ERR_T]=3000;		//���԰汾��Ĭ��Ϊ3����
		cfg->r[IC_DUSTBOX_MAX] = 2000;	//����������
		cfg->r[IC_DUSTBOX_T] = 100;	//���ӵ�λ

		//cfg->r[IC_GO_BACK] = 603;		//���˵ĳ�ʱ��6ϵ����3�γ�ʱ

		cfg->r[IC_DUST_MID] = 5;	//�ҳ�Ũ����
		cfg->r[IC_DUST_HIGH] = 10;	//�ҳ�Ũ�ȸ�

		cfg->r[CARPET_L_PRA] = 80;	//���ֵ�̺����
		cfg->r[CARPET_R_PRA] = 80;    //���ֵ�̺����

		cfg->r[IC_AGI_AMP_FACTOR] = 200;	  //��ɨ�澯ϵ��
		cfg->r[IC_LMOT_AMP_FACTOR] = 100;	  //���ָ澯ϵ��
		cfg->r[IC_RMOT_AMP_FACTOR] = 100;	  //���ָ澯ϵ��
		cfg->r[IC_FAN_CUR_ERR] = 2302;    		//�����������澯����

		cfg->r[IC_FAN_S_LRCUR_ERR] = 100;			//����ģʽ�����ת����
		cfg->r[IC_FAN_N_LRCUR_ERR] = 400;    		//����ģʽ�����ת����
		cfg->r[IC_FAN_T_LRCUR_ERR] = 900;    		//ǿ��ģʽ�����ת����
		cfg->r[IC_FAN_LRCUR_TMR] = 60;    			//�����תʱ���ж���ֵ

		cfg->r[IC_LED_EN] = 1;					//��ƿ���


		cfg->t_agitator_inspection=0;	//��ˢ���ʱ��
		cfg->t_agitator_replacemen=0;//��ˢ����ʱ�� 	69*2

		cfg->t_agitator_inspection = cfg->t_agitator_replacemen = 0;

		cfg->wifi_config_able = 0;

		cfg->dust_pwm_lev=DUST_MODE_NORMAL;

		cfg->speaker_able = 1;
		memset(cfg->sn,0xff,21);
		save_cfg();

		sys->t_area =0;			//��ɨ�������¼��20200521
		//���ﻹ����ɨ��ʱ��
		sys->t_clean= 0;
        //log_printf("sys->t_navi_work=%d,old=%d,sys->t_area=d%,sys->m_area=%d\r\n",sys->t_navi_work,sys->t_clean,sys->t_area ,sys->m_area);
		I2C_WriteS_24C(0,(uint8_t *)sys, ERP_LEN);
}

void save_cfg(void)
{
	u16 i=0;
	u32 r1,addr;
	u8 *p;

	log_printf("save cfg\r\n");
	i=MAX_CFG_LEN;	//�����ֽ���
	p=(u8 * )cfg_buf;		   		//���뿽����ַ
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

}

char com_str[32];
char *anly_cmd(char *cmd,int *value)
{
	int i;
	char tmp[32];
	char *p,*p1;
	if(strlen(cmd)>=32)
	{
		log_printf("len error\r\n");
		return NULL;
	}
	memset(com_str,0x00,32);
	for(i=0;i<5;i++)
	{
		if(cmd[i]=='=')
		{
			com_str[i]=0x00;
			break;
		}
	}
	log_printf("cmd=%s\r\n",com_str);
	strcpy(tmp,cmd);
	p=strchr(tmp,'=');
	if(p==NULL)
	{
		log_printf("not found =\r\n");
		return NULL;
	}
	p1=strchr(tmp,0x0D);
	if(p1==NULL)
	{
		log_printf("NOT FOUND 0x0D\r\n");
		return NULL;
	}
	*p1=0x00;
	*value=atoi(p+1);
	return com_str;


}
void proc_rx_uart(uint8_t *buff,int len)
{
	struct h_cfg_t *mcfg,scfg;
	mcfg=(struct h_cfg_t *)buff;
	if(mcfg->magic != MAGIC)
	{
		log_printf("magic error\r\n");
		return;
	}

	if(mcfg->msg==0x01)
	{
		//cfg->dock_hw_back=mcfg->dock_hw_back;			//��ͷ�Ĳ���
		//cfg->dock_hw_forwak=mcfg->dock_hw_forwak;			//ǰ��Ĳ���
		//cfg->dock_hw_turn=mcfg->dock_hw_turn;			//ת�����Ĳ���
		memcpy((char *)cfg,(char *)mcfg,sizeof(struct h_cfg_t));
		NOMORL_PWM = 680;//cfg->pwm_normal;
		SLOW_PWM = 780;//cfg->pwm_slow;
//		log_printf("dock_fw:%d,fw_chw=%d,hw_back=%d,hw_turn\r\n",cfg->dock_hw_forwak,cfg->dock_hw_forwak,cfg->dock_hw_turn);
//		log_printf("dock_pwm:%d,dock_m_pwm=%d,dock_speed=%d\r\n",cfg->dock_pwm,cfg->dock_m_pwm,cfg->dock_speed);
		//log_printf("pwm_normal:%d,pwm_slow=%d,speed_normal=%d\r\n",cfg->pwm_normal,cfg->pwm_slow,cfg->speed_normal);

		save_cfg();
		log_printf("set OK\r\n");
	}else if(mcfg->msg==0x02)	//��ȡ
	{
		memcpy((char *)&scfg,(char *)cfg,sizeof(struct h_cfg_t));
		scfg.magic =MAGIC;
		scfg.msg =0x02;
		scfg.crc =0;
		//scfg.dock_hw_back=cfg->dock_hw_back;			//��ͷ�Ĳ���
		//scfg.dock_hw_forwak=cfg->dock_hw_forwak;			//ǰ��Ĳ���
		//scfg.dock_hw_turn=cfg->dock_hw_turn;			//ת�����Ĳ���
		delay(10);
		usart_write((uint8_t *)&scfg,sizeof(struct h_cfg_t));
		delay(10);
	}else if(mcfg->msg ==0x03 && mcfg->crc==0x18)
	{
		memcpy((char *)&sys_debug,(char *)mcfg,sizeof(struct sys_debug_t));
		log_printf("sys debug=%d,pwm=%d\r\n",sys_debug.debug,sys_debug.pwm);
		if(sys_debug.debug == DEBUG_PWM)
		{
			motor_run(GO_FORWARD, sys_debug.pwm, 0, 0);
		}
	}
}
