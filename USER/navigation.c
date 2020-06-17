/********************************************************************
 * ��Ȩ : ���ݰ������ܿƼ����޹�˾
 * �ļ� : main.c
 * �汾 : V2.0.0
 * ���� : Τ����
 * ʱ�� : 2016-2-13
 * ˵�� : ������ɨ��������������
 * ��Ȩ : ��Ȩ���С���ֹ������
 *		  ���ݰ������ܿƼ����޹�˾(http://www.idleintel.com/)��������֪ʶ��Ȩ���������Ȩ
 *		  ���뾭����Ȩ����ʹ��
 *********************************************************************/

#include "sys.h"

SHARE_RAM_U share_ram_buff;
////navigat_t *navigat,navigation;		//��������
navigat_t *navigat;		//��������
//RmCln_t g_rand_clean,*rcln;
motor_hal_t *m_hal,motor_hal;		//��������
extern const	char copyright[];

/********************************************************************
 * �������� : navigat_init
 * ��    �� : flage :
 *				3 - ֻ��ʼ���ڴ�
 *				4 - ��ʼ���������������ǡ�
 *				���� - ������ʼ����
 *
 * ��    �� : ��
 * ˵    �� : ��ʼ����������
 *********************************************************************/
void navigat_init(char flage)
{
//	short i;
	uint8_t found_charge=0;
//	int16_t tx,ty;

	if(flage == 3)
	{
		navigat = &(share_ram_buff.navigat_buffer);
		memset((char *)navigat,0x00,sizeof((share_ram_buff.navigat_buffer)));		//�����������
		////navigat = &navigation;
		////memset((char *)navigat,0x00,sizeof(navigation));
		navigat->whele_stop = 0;
		return ;
	}
	log_printf("===navigat_init=======\r\n");
	//ny3p_play(VOICE_DIDI);
	delay(100);
	reset_gyro(1);
//	 log_printf("dust pwm=============1111====%d\r\n",sys->sState);

	sys->c_loop = 0;
//ȡ�����еĸ澯
	//dp_tx_data(DP_SYS_STA, 0, 0, 0,0);
	timer2_init();
	STOP_ALL_MOTOR();
	MOTOR_POWER_OFF();
	delay_ms(10);
	SET_DST_MOTER(400);
	delay_ms(5);
	MOTOR_POWER_ON();
	 sys->kdis_yaw = 0;
		 sys->cdis_yaw = 0;
	sys->c_walk_drop = 0;
	sys->t_work = msTmr;		//��¼��ʼ��ʱ��
	sys->t_navi_work=0;
    sys->timeout_navi_work = 0;
    sys->shut_down_motor = 0;
	sys->dock_near = 0;
//	sys->yaw_ofset = 0;
	sys->m_area = 0;			//������ɨ�������
	sys->pause = 0;				//��ͣ
//	sys->yaw_ofset = 0;
	//navigat->c_org_navi = 0;
//	sys->navi_run = 0;		//���ⲻ�澯
	navigat->whele_stop = 0;
	navigat->gun_cnt = 0;	//wy190615 ��ˢ����ֵ
	sys->err_sta = 0;
	//sys->low_power = 0;
	navigat->ntox = navigat->ntoy = 0;
	sys->c_go_bum = 0;

	sys->t_agitator_inspection = cfg->t_agitator_inspection;
	sys->t_agitator_replacemen = cfg->t_agitator_replacemen;


	//usart2_init_info();
//	rout->index = rout->max_len = rout->xline_len = 0;
	msTmr = 0;


	//m_hal->angle = 0;
	//delay(100);
	//delay_sensor(100);
//if(flage != 5)
	if(sys->sState == SYS_IDLE)		////
		return ;


//l_begin_navi_init:
	sys->c_charge_power = sys->c_low_power = 0;
	//��ѹ�����ݵ�ѹ����PWM

	if(sys->volt >  15000)
		sys->pwm = 750;
	else
		sys->pwm = 600;
    sys->adjust_gyro_time = sys->t_navi_work;		///������У׼ʱ��

#if SYS_VER	== VER_KESMAN
	navigat->suspend =FALSE;
#endif
	if(sys->sState == SYS_NAVIGAT)
		LED3_ON();
	log_printf("[navigat_init]suspend=%d,power=%d,pwm=%d,flage=%d\r\n",navigat->suspend,sys->volt,sys->pwm,flage);
	if(navigat->suspend ==FALSE )
	{
		//������ɨ�����¼�ӳ��׮����
		if(flage == 4)
		{
			found_charge = navigat->from_charge;
//			tx = X_NOW;
		//	ty = Y_NOW;
		}
		////memset((char *)navigat,0x00,sizeof(navigation));
		memset((char *)navigat,0x00,sizeof((share_ram_buff.navigat_buffer)));
					copyright_www_idleintel_com("No copy allowed.Copyright idleintel,please tel:13757122544",
										 &kgyro,SPI1_ReadWriteByte,spi_cs,log_printf,delay);
			copyright_idleintel_no_one_is_allowed_to_copy(\
		"���棬�˴����Ȩ�麼�ݰ������ܿƼ����޹�˾���У�δ����Ȩ�κ��˲������⿽���Ķ������µ�13757122544",\
			"http://www.idleintel.com/");

		copyright_idleintel_illegal_copy_tell_13757122544();
		if(flage == 4)
		{
			navigat->found_charge = found_charge;
		}
	}

	navigat->radius = RADIUS;
	navigat->obstacle 	= 1;
	navigat->scan		= 2;

	ir_dock_insight(1);
	sys->right_pwm = sys->left_pwm = 680;//cfg->go_forward_pwm;
	navigat->angle = 0;



		delay_ms(8);
		www_idleintel_com();
		log_printf("[navigat_init]angle=%f\r\n",sys->angle);
	printf_power_sta();
	navigat->t_gyro_adj = sys->t_navi_work;		//ˢ��ʱ��

	if(navigat->suspend == TRUE /*&&  !DOCK_DETECT()*/)
	{
		//�ӳ��׮����
		if(DOCK_DETECT())
			navigat->from_charge = TRUE;
		sys->t_work = msTmr;		//��¼��ʼ��ʱ��
		goto l_nvi_begin ;
	}
	navigat->mx = navigat->my = 0;		//�ϵ������ĵ�
	navigat->side = LEFT_SIDE;
	navigat->navi_side = 0;
	navigat->distance = 0;
	navigat->angle = 0;
	//navigat->navilen=navigat->tolen = 0;
	navigat->tolen = 0;
	navigat->c_avr[0] = navigat->c_avr[1] = 10;	//�տ�ʼ��������Ҫ�ó�ʼ��ֵ��ֵ  navigat->x_avr
	navigat->x_avr[0] = navigat->x_avr[1] = -1000;
	navigat->c_near = 0;
	navigat->y_min_draw = navigat->y_max_draw = 0;
	//navigat->ajust_widows = 8;		//�տ�ʼ��ʱ�򣬽����Ļ�������Ϊ7��Ҳ����˵��7���ڣ���Ϊ�������������

	navigat->x_org = navigat->y_org = 0;
	navigat->x_org_f = navigat->y_org_f = 0;


	navigat->c_navi_err = 0;


	navigat->backClean = 1;

	//for(i=0;i<DOOR_LEN;i++)
	//	navigat->door_x[i]   = navigat->door_y[i]   = 0;
	navigat->first_bum = 1;
	navigat->tx = navigat->ty = 100;
	navigat->x_dock = 0;
	navigat->c_turn = navigat->c_go = 0;
	navigat->max_tx = navigat->max_ty=navigat->min_tx=navigat->min_ty = 100;
	//navigat->right_side_cln = 0;
	//��⵽�����
	timer1_init();					//ʱ��1������������ˢ
	timer2_init();					//������ɨ������������s



	{
		if(sys->sState == SYS_FOCUS)
		{
			MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,FOCUS_DUST_PWM);
		}
		else
		{
			MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value);//modified by wonton2004 20170118
		}
	}

	MOTOR_POWER_ON();


l_nvi_begin:
	if(flage)
	{
		if(DOCK_DETECT())			//�������
		{
//			int16_t x,y;
			log_printf("from dock\r\n");
			navigat->from_charge = TRUE;
			navigat->found_charge = 1;
			//MOTOR_CTRL(0,0,0,0);
			//MOTOR_CTRL_SIDE(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R);
	navigat->dock_x = 100;
			navigat->dock_y = 100;

			///����Ӧ����Ϊ��������,��ʱ������2018-09-14 jzz
			log_printf("sState_auxi:%d\r\n",sys->sState_auxi);
		//	set_osbt(navigat->tx +1, navigat->ty, 1, 1, "dock");
			//set_osbt(navigat->tx +1, navigat->ty+1, 1, 1, "dock2");	//û�취��Ҫ��һ��

			navigat->is_walk = TRUE;
			set_compress_mem(navigat->tx, navigat->ty, 1, navigat->scan);
			set_compress_mem(navigat->tx-1, navigat->ty, 1, navigat->scan);
			set_compress_mem(navigat->tx-2, navigat->ty, 1, navigat->scan);


			motor_back_off(100);
			if(sys->sState == SYS_IDLE)/////2018-05-24 jzz
				return;
			navigat->y_org_f = 0;
			//STOP_ALL_MOTOR();

			motor_run(GO_STOP,0,0,0);
			delay_ms(100);

			//MOTOR_POWER_ON();
			//MOTOR_CTRL_SIDE(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R);
			turn_to_deg(180);
			//STOP_ALL_MOTOR();
			//MOTOR_POWER_OFF();
			delay_ms(10);

			//timer1_init();					//ʱ��1������������ˢ
			//timer2_init();					//������ɨ������������s
		}
		//else
		{
			//pwifistream->updata_dock_flag = 1;		//���³����������1
			//updata_stream_cache_data(100,100,0,POINT_DOCK);		//����Ϊ�����
		}
	}

	if(sys->sState == SYS_IDLE)		//////2018-05-24 jzz
		return;
	log_printf("[navigat_init]angle=%3.1f,sta=%d,supend=%d,mxy=(%d,%d,0)shut=%d\r\n",sys->angle,sys->sState,navigat->suspend,navigat->mx,navigat->my,sys->shut_down_motor);
	//init_router();


	if(flage != 4)
	{
		//delay_ms_sensers(300);
		sys->t_work = msTmr;		//��¼��ʼ��ʱ��
		sys->work_tmr = 0;
	}

	log_printf("dust pwm=%d\r\n",sys->dust_pwm_lev);
	//sys->dust_pwm_value == MID_DUST_PWM;		//����������Ĭ�����е�
	if(sys->dust_pwm_lev  == DUST_MODE_TURBO)
	{
			DUST_MOTOR_RANK3();
			//DUST_MOTOR_RANK2();
			sys->run_speed = HIGHT_SPEED;
			log_printf("turbo mode\r\n");
	}else
	 if(sys->dust_pwm_lev == DUST_MODE_SILENT)
	 {
			DUST_MOTOR_RANK1();
			sys->run_speed = LOW_SPEED;
			log_printf("silent mode\r\n");
	 }
	else
	{
		DUST_MOTOR_RANK2();
		sys->run_speed = MIDLE_SPEED;
		log_printf("normal mode\r\n");
	}

    //delay_ms(100);
	navigat->mx = navigat->my = 0;
	navigat->navi_ok = 0;
	sys->t_gyro_ofst = msTmr;
	navigat->side_sta = 0;
//	navigat->navi_to_dock = 0;
	navigat->dock_adj_y = 0;
	navigat->clean_ok = 0;
	navigat->suspend = FALSE;
	navigat->m_navi_run = 0;
	sys->navi_run = 1;
	navigat->c_v_go = 0;
	navigat->c_m_bum = navigat->c_z_bum = 0;
	navigat->sheepflod = 0;
	if(sys->sState == SYS_NAVIGAT)
		LED3_ON();
	sys->t_flash_halt = 0;
	sys->t_halt_on = 1;
	log_printf("[navigat_init]end\r\n");
}
/********************************************************************
 * �������� : init_navigate_gobal
 * ��    �� : ��,
 * ��    �� : ��
 * ˵    �� : ��ʼ����������
 *********************************************************************/
void init_navigate_gobal(void)
{
//		int16_t dir;

		CHECK_NAVI_STA_ONLY();
		CHECK_NAVI_STA();
		//if((msTmr - sys->t_gyro_ofst) >60000) // 3����У׼һ��
  #if ADJ_GYRO

				  if(sys->t_navi_work < 1200)
				  {
					  uint32_t timout;
					  if(sys->t_navi_work < 300)
						  timout = 60000; //һ����
					  else
						  timout = 180000;
					  if((msTmr - sys->t_gyro_ofst) >timout) // 3����У׼һ��
					  {
						  log_printf("gyro ofst,tmr=%d,timeout=%d\r\n",sys->t_navi_work ,timout);
						  sys->t_gyro_ofst = msTmr;
						  motor_run(GO_STOP,0,0,0);
						  STOP_ALL_MOTOR();
						  //gyro_offset_manage(0);
						  delay_ms_tm5(100);
						  mpu6500_date_offset(300);
  						log_printf("ofs=%d\r\n",sys->gyro_offset);

						  MOTOR_POWER_ON();
						  if(sys->sState == SYS_FOCUS)
						  {
							  MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,FOCUS_DUST_PWM);
						  }
						  else
						  {
							  MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value);//modified by wonton2004 20170118
						  }
							delay_ms(100);
							log_printf("dust level=%d,%d\r\n",sys->dust_level,sys->dust_pwm_lev);
							if(sys->dust_pwm_lev==DUST_MODE_TURBO)
							{
									DUST_MOTOR_RANK3();
							}else
							 if(sys->dust_pwm_lev ==DUST_MODE_SILENT)
							 {
									DUST_MOTOR_RANK1();

							 }
							else
							{
								DUST_MOTOR_RANK2();
							}
							/*
							if(sys->shut_down_motor == 1)
							{
								log_printf("shut down motor...\r\n");
								DUST_MOTOR_OFF();
								AGITATOR_PWM = 0;
						  }
						  */

					  }
				  }

#endif




		/*
		if(navigat->angle == 0)
		{
			motor_run(GO_STOP,0,0,0);
			delay_ms_sensers(100);
			gyro_offset_manage(10);
			sys->t_gyro_ofst = msTmr;
		}else
			delay_ms(50);
		*/
		delay_ms(5);
		//delay_ms(20);

		www_idleintel_com();

		//�ұ�
		if(RIGHT_IRDA() && (navigat->angle == 0 ||navigat->angle == 180))
		{
//			dir = navigat->angle == 0?-1:1;
//			set_osbt(navigat->tx,navigat->ty +dir,1,0,"obs_bg_r");
			navigat->osbt_x = navigat->tx;
		}
		if(LEFT_IRDA())
		{
//			dir = navigat->angle == 0?1:-1;
//			set_osbt(navigat->tx,navigat->ty +dir,1,0,"obs_bg_l");
			navigat->osbt_x = navigat->tx;
		}
		log_printf("-(%d,%d,%3.1f,0)-[%d,%d,%d,%3.1f]* begin_walk \r\n",navigat->tx,navigat->ty,navigat->angle,sys->g_sta[2],sys->g_sta[3],sys->g_sta[4],sys->angle);

		log_printf("====================================================\r\n");
		log_printf("=  z_go[motor_go_forwark]=\r\n");
		log_printf("====================================================\r\n");

		motor.c_right_hw = motor.c_front_hw = 0;		//���ڼ����Ƿ��
		sys->slip  = 0;
	//	sys->last_near_angle = 800;
		motor.total_left_hw = motor.total_right_hw = 0;
		navigat->side_obst = 0;
		navigat->c_side_obst = 0;
	//	reset_fw_fifo();
		MIDLE_ADC() = 0;
		sys->lst_distance = navigat->distance;
		if(navigat->c_side_bum==0)		//������ײ��ʱ�򣬲����㣬������ײ�����һ�������
			navigat->scan_x = 0;
		navigat->out = navigat->angle;
	//	sys->c_lost_wall_right	= 0;
		//sys->c_near_wall_right 	= 0;
	//	sys->c_lost_wall_left	= 0;
		//sys->c_near_wall_left 	= 0;
		navigat->first_bum = 0;
		navigat->arc_z_go = 0;

}
/*
	���������߳�
*/
#define GO_STA_NAVIGAT	0x01
#define GO_STA_SIDE_BUM	0x02
#if 1
/********************************************************************
 * �������� : proc_navigat_task
 * ��    �� : ��
 * ��    �� : ��
 * ˵    �� : �����߳�
 *********************************************************************/
void proc_navigat_task(uint16_t sta)
{

	short bx=X_NOW,by=Y_NOW;

	motor_go_forwark(-1,NO_SIDE_NEAR,NULL);			//ֱ��D
	CHECK_NAVI_STA_ONLY();
	if(proc_z_go_bum(sys->gsta,bx,by)==0)					//1���???����
	{

	}

	init_navigate_gobal();
	CHECK_NAVI_STA_ONLY();






}
#else
#if 0//ICLEAN_HW_VERSION  == 0x1203
#define NAVI_GYRO_CRCT	(0.0f)//(-4.0f)//(1.89f)//(-3.7f)//(-2.0f)//3.2
#define NAVI_GYRO_TURN	4//8
#else
	#if 1//ICLEAN_MAC_TYPE == 1//������
	#define NAVI_GYRO_CRCT	(0.0f)//(-4.0f)
	#define NAVI_GYRO_TURN	8
	#else
	#define NAVI_GYRO_CRCT	(2.7f)//(6.5f)
	#define NAVI_GYRO_TURN	8
	#endif
#endif
//�����͵���
void proc_navigat_task(uint16_t sta)
{
	//m_hal->out = sys->angle;

	sys->c_turn_navi++;
	log_printf("==========\r\n[navi_task]fw...c=%d\r\n==========\r\n",sys->c_turn_navi);
	#if 0//ICLEAN_MAC_TYPE == 1
	motor_go_forwark(0,400,NO_SIDE_NEAR,&m_hal->out,NULL);		//ֱ��
	#elif 1//ICLEAN_MAC_TYPE == 0
	motor_go_forwark_2a(0,200,NO_SIDE_NEAR,&m_hal->out,NULL);		//ֱ��
	#else
	motor_go_forwark(0,100,NO_SIDE_NEAR,&m_hal->out,NULL);		//ֱ��
	#endif

	if(sys->c_turn_navi >= NAVI_GYRO_TURN)
	{
		if(sys->angle > 150 && sys->angle < 210)
		{
	#if 0//ICLEAN_HW_VERSION == 0x1203
			float n_angle;

			n_angle = 0.1f*WALK_DIST()/7;
			sys->yaw -=n_angle;
			log_printf("\r\n\r\ncorrect angle...%3.3f\r\n",n_angle);
	#else
			sys->yaw -=	NAVI_GYRO_CRCT;
			//log_printf("correct angle...\r\n");
			log_printf("\r\n\r\ncorrect angle...%3.3f\r\n",NAVI_GYRO_CRCT);
	#endif
			sys->c_turn_navi = 0;
		}
	}
	delay_ms(100);
	log_printf("angle=%3.2f\r\n",sys->angle);
	//��ζ̾��룬���ӱ�һ��
	if(WALK_DIST()< 2000)		//30 һmm 50cm��ʱ�����У����ӱ�
		rcln->short_distance++;
	else
		rcln->short_distance =0;



	if(rcln->short_distance >=10)	//����Ҫ�ĵ��Ժ� 10->4 for test
	{
		//uint8_t n_edgeways_ret = 0;
		log_printf("[proc_navigat_task]short dist,near wall\r\n");
		//ny3p_play(VOICE_M_SPEED_MODE);
		//motor_run(GO_STOP,0,0,0);
		//while(1);
		//n_edgeways_ret = motor_go_edgeways(NO_SIDE_NEAR,0,60000,0);	//�ӱ�һ����
		motor_go_edgeways_2a(NO_SIDE_NEAR,0,60000,0);	//�ӱ�һ����
		//motor_go_edgeways(n_sta_back ,bx,by,GO_NEAR_TYPE_ADJ,0);
		//log_printf("[proc_navigat_task]edgeways exit:%d\r\n",n_edgeways_ret);
		//motor_run(GO_STOP,0,0,0);
		//while(1);
		rcln->short_distance = 0;
		return ;
	}
	#ifdef ICLEAN_ROUND_CC_V12
#if 1//ICLEAN_MAC_TYPE == 1
	{
		uint16_t n_crsyards = ICLEAN_NAVI_CROSSMAP_YARDS;
		if(sys->work_mod & 0x04)n_crsyards += (n_crsyards >> 1);
		nearwall_cross_map(n_crsyards);									//90���߹�ȥ�ľ���
	}
#else
	nearwall_cross_map(ICLEAN_NAVI_CROSSMAP_YARDS);									//90���߹�ȥ�ľ���
#endif
	//nearwall_cross_map(200);									//90���߹�ȥ�ľ���
	//log_printf("[proc_navigat_task]nearwall_cross_map\r\n");
	//motor_run(GO_STOP,0,0,0);
	//while(1);
	#else
	//nearwall_cross_map(400);									//90���߹�ȥ�ľ���
	nearwall_cross_map(navigat->y_org+400,sys->angle,GO_TYPE_NO_SAVE,20);
	#endif


}

#endif

/********************************************************************
 * �������� : proc_z_go_bum
 * ��    �� :
 * ��    �� : 0 - ��ɨ��ͷ����Ҫ������1-����������ɨ
 * ˵    �� : ������ɨ��ֱ����ײ�󣬹�����ɨ����������0������Ҫ����
 *********************************************************************/
uint8_t proc_z_go_bum(uint16_t sta,short bx,short by)
{
	short 		dir =0;//,y_dir;//,indx;
	float		angle=0;


	log_printf("\r\n====================================================\r\n");
	log_printf("[idleBUM](%d,%d,%3.1f)ir=(%d,%d,%d),t=%d,cgo_bum=%d,ctarge=%d,cdrop=%d\r\n",navigat->tx,navigat->ty,sys->yaw,sys->g_sta[0],sys->g_sta[3],sys->g_sta[6],sys->t_navi_work,sys->c_go_bum,navigat->c_target_go);
	log_printf("====================================================\r\n");
	//mpu6500_date_offset(200);
  	//log_printf("ofs=%d\r\n",sys->gyro_offset);
  	//navigat->c_go++;
	/*
  	if(cfg->gyro_ofs1 > 1&& cfg->gyro_ofs1 < 100 &&  cfg->gyro_ofs2 > -10 &&  cfg->gyro_ofs2 < 10)*/
  	if(navigat->c_go >=cfg->gyro_ofs1)
  	{
  		sys->yaw +=cfg->gyro_ofs2;
  		sys->angle = sys->yaw;
  		log_printf("inc yaw\r\n");
  		navigat->c_go = 0;
  	}

	get_gyro_ofs();
	navigat->lst_angle 	= navigat->angle;					//��?��顤??��,��?o?o����??a??��??��?��?��?����?��?��?????��?��?2??����?
	dir 				= navigat->angle==0?1:-1;			//??????�̡�??��?��??����?��?������?���??��-��?��?����o����??��?��1?��??��?
	angle 				= navigat->lst_angle ==0?180:0;		//??3???��?��?��|??��?��????��?��

	CHECK_NAVI_STA_RT(1);			//���䨬???��?��?��??��?����?��??������??��?��??������??��?���?����?wifi�̨�??��?��?��???���䨬???��?��??����?3??��
//	PRINTF_XY();					//�䨰��??��??



	cfg->t_agitator_inspection = sys->t_agitator_inspection+sys->t_navi_work / 60;
	cfg->t_agitator_replacemen = sys->t_agitator_replacemen+sys->t_navi_work / 60;
/*
	static uint32_t t_agi_tmr = 0;
	if(t_agi_tmr!= sys->t_navi_work / 60)
	{
		t_agi_tmr = sys->t_navi_work / 60;
		cfg->t_agitator_inspection++;
		cfg->t_agitator_replacemen++;
	}
	*/
	log_printf("t_agitator=%d,%d\r\n",cfg->t_agitator_inspection,cfg->t_agitator_replacemen);
	if(cfg->t_agitator_inspection >=300/*cfg->t_agi_inspection_max*/)  // 5��Сʱ
	{
		if((sys->work_errcode & MWERR_AGITATOR_INSPECTIONG) == 0)
		{
			log_printf("agitator_inspection error\r\n");
			sys->work_errcode |= MWERR_AGITATOR_INSPECTIONG;
			//save_cfg();
		}
	}
	if(cfg->t_agitator_replacemen >=/*cfg->t_agi_replacemen_max*/3000)  // 50��Сʱ
	{
		if((sys->work_errcode & MWERR_AGITATOR_REPLACEMENT) == 0)
		{
			log_printf("agitator_replacemen error\r\n");
			//cfg->t_agitator_replacemen = 0;
			sys->work_errcode |= MWERR_AGITATOR_REPLACEMENT;
			//save_cfg();
		}
	}



	//set_obst(X_NOW+dir,Y_NOW,1);	//����????��-��?
	log_printf("#(%d,%d,%3.1f,s)-[0]#s2,drop=%d\r\n",X_NOW+dir,Y_NOW,sys->angle,sys->c_walk_drop);	//�䨰��???��-��?
	sys->c_go_bum++;
	if((sys->c_go_bum >40 || navigat->c_target_go >=5 ))
	{
		log_printf("change dir or nearwall\r\n");
		sys->c_go_bum = 0;
		if(sys->c_walk_drop < MAX_DROP_COUNT)
		{
			motor_go_edgeways(LEFT_SIDE_NEAR,0,0,0,0);
			turn_to_deg(0);
		}else
		{
			log_printf("change dir...\r\n");
			navigat->side = navigat->side == LEFT_SIDE?RIGHT_SIDE:LEFT_SIDE;
			nearwall_cross_map(INC_HW,angle);	//?����?1���?D��?��
		}
		navigat->c_target_go =0;

	}else
		nearwall_cross_map(INC_HW,angle);	//?����?1���?D��?��
        sys->m_area= (sys->t_navi_work/60)*0.8;   //ʵʱ�������

	return 1;
}

void navigat_reset(void)
{
	int x1_org,y1_org;
	int x_org,y_org;
	int x_org_l,y_for_l;
	float x_org_f,y_org_f;
	float x_org_r,y_org_r;
	float x_org_t,y_org_t;

	short x,y;
	short	tx,ty;
	short x_dock;				//��Ҫ�����ӵ�λ��
	int16_t dock_x;	//���׮������
	int16_t dock_y;
	uint8_t side;

	x1_org = navigat->x1_org;
	y1_org = navigat->y1_org;
	x_org  = navigat->x_org;
	y_org  = navigat->y_org;
	x_org_l= navigat->x_org_l;
	y_for_l= navigat->y_for_l;
	x_org_f= navigat->x_org_f,
	y_org_f= navigat->y_org_f;
	x_org_r= navigat->x_org_r,
	y_org_r= navigat->y_org_r;
	x_org_t= navigat->x_org_t,
	y_org_t= navigat->y_org_t;
	side   = navigat->side;

	x = navigat->x;
	y = navigat->y;
	tx = navigat->tx,
	ty = navigat->ty;
	x_dock = navigat->x_dock;				//��Ҫ�����ӵ�λ��
	dock_x = navigat->dock_x;	//���׮������
	dock_y = navigat->dock_y;

	memset((char *)navigat,0x00,sizeof(struct h_navigat_t));
	copyright_www_idleintel_com("No copy allowed.Copyright idleintel,please tel:13757122544",
								 &kgyro,SPI1_ReadWriteByte,spi_cs,log_printf,delay);

	copyright_idleintel_no_one_is_allowed_to_copy(\
		"���棬�˴����Ȩ�麼�ݰ������ܿƼ����޹�˾���У�δ����Ȩ�κ��˲������⿽���Ķ������µ�13757122544",\
		"http://www.idleintel.com/");
	copyright_idleintel_illegal_copy_tell_13757122544();

	init_gyro_l();

	navigat->x1_org = x1_org;
	navigat->y1_org = y1_org;
	navigat->x_org = x_org;
	navigat->y_org = y_org;
	navigat->x_org_l = x_org_l;
	navigat->y_for_l = y_for_l;
	navigat->x_org_f = x_org_f;
	navigat->y_org_f = y_org_f;
	navigat->x_org_r = x_org_r,
	navigat->y_org_r = y_org_r;
	navigat->x_org_t = x_org_t,
	navigat->y_org_t = y_org_t;

	navigat->x = x;
	navigat->y = y;
	navigat->tx = tx;
	navigat->ty = ty;
	navigat->x_dock = x_dock;				//��Ҫ�����ӵ�λ��
	navigat->dock_x = dock_x;	//���׮������
	navigat->dock_y = dock_y;
	//navigat->force_z_go = 5;

	navigat->side = side == LEFT_SIDE?RIGHT_SIDE:LEFT_SIDE;

	navigat->radius = RADIUS;
	navigat->obstacle 	= 1;
	navigat->scan		= 2;
	sys->dock_near = 0;
//	sys->yaw_ofset = 0;
	sys->navi_run = 0;		//���ⲻ�澯
	sys->err_sta = 0;
	log_printf("navigat_reset...\r\n");
}



//======================================������ɨ============================================

#define FOCUS_INC	20
#define FOCUS_BASE	50

typedef struct _focus_t
{
	int 	cnt;
	int 	distance;		//�ߵľ���
//	char	index;			//���

}FOCUS_T;

FOCUS_T focus;

#define MAX_FOCUS_DEGRE_COUNT		4
float fos_degs[MAX_FOCUS_DEGRE_COUNT];//={0,90,180,270};
//int fos_count=0,focus.distance=0;
void init_focus_task(void)
{
	focus.distance = FOCUS_BASE*WHELE_HW;
//	focus.index = 0;
	focus.cnt = 0;

	focus.distance = FOCUS_BASE*WHELE_HW;
	navigat_init(0);
	focus.cnt =0;


	fos_degs[0] = 0;
	fos_degs[1] = 90;
	fos_degs[2] = 180;
	fos_degs[3] = 270;


	log_printf("init focus ok\r\n");
}
/*
������ɨ���߳�
*/

int proc_focus_task(void)
{
	int idx;
	//if(focus.distance <=20 || focus.k >=16)
	if(focus.cnt  >=16 || focus.distance < 10)
	{
		log_printf("[focus_task]dis=%d,count=%d,focus over\r\n",focus.distance,focus.cnt );
		motor_run(GO_STOP,0,0,0);
		STOP_ALL_MOTOR();
        ny3p_play(VOICE_M_FINISHED);
		sys->sState = SYS_IDLE;
		return 0;
	}
	//delay_ms(200);
	log_printf("-----------------\r\n[focus_task]go,dis=%d,idex=%d\r\n",focus.distance,focus.cnt );

	motor_go_forwark(focus.distance, NO_SIDE_NEAR, NULL);		//��·
	if(sys->sState != SYS_FOCUS)
	    return 0;

	delay_ms(50);
	//gyro_offset_manage(500);
	//www_idleintel_com();
	log_printf("--walk ok,agl=%f,dis=%d,count=%d\r\n",sys->angle,focus.distance,focus.cnt );
	if(focus.cnt  < 8)
		//focus.distance = FOCUS_BASE + focus.cnt *10;
		focus.distance += FOCUS_INC*WHELE_HW;								//�г�����
	else
		//focus.distance = FOCUS_BASE + (16 -focus.cnt )*10;
		focus.distance -= FOCUS_INC*WHELE_HW;

	focus.cnt  ++;

	idx = focus.cnt  & 3;
	log_printf("--count=%d,nx=%d,deg=%f\r\n",focus.cnt ,idx,fos_degs[idx]);
	turn_to_deg(fos_degs[idx]);						//ת���Ƕ�
	//focus.index = focus.index >=MAX_FOCUS_DEGRE_COUNT?0:focus.index;//������һ���Ƕ�
	return 1;

}


int disxy(short x,short y)
{
	return (x>y?(x-y):(y-x));
}

#if  0
///������ɨ���2A 20200521

void get_clean_area(void)
{
	uint16_t x,y;
	uint8_t sta;
	uint16_t scancount=0;
	static uint16_t c=0;

	c++;
	if(c < 2)
		return ;
	c = 0;
	for(x=navigat->min_tx-2;x<=(navigat->max_tx +3);x++)
	{
		for(y=navigat->min_ty-2;y<=(navigat->max_ty+3);y++)
		{

			sta = get_map_sta(x, y);
			//�ۼ��ܹ����ϰ��ĺ�ɨ���
			if(sta != MAP_STA_UNKN)
				scancount++;

		}
	}

	unsigned int test_area = POINT_TO_AREA(scancount);
	sys->m_area= test_area /10000 +  (test_area %10000) / 5000;
	log_printf("area=%d\r\n",sys->m_area);

#endif


/*
n_sta :���λ��Ϊ�ȼ����ȼ�0��ȼ�1���ȼ�0�ǹ����͵��߷���Ҫ��Ƚ��ϣ��ȼ�1�ȽϿ���

*/
void init_near_wall_navi(uint8_t n_sta)
{
	if(n_sta & 0x80)
		init_near_pid(1);
	else
		init_near_pid(0);

	n_sta = n_sta & 0x7F;
	navigat->near.n_sta = n_sta;
	if(n_sta == LEFT_SIDE_NEAR)
	{
		navigat->near.turn_dir = GO_RIGTH;
		navigat->near.pid = &l_near_pid;
	}
	else if(n_sta == RIGHT_SIDE_NEAR)
	{
		navigat->near.turn_dir = GO_LEFT;
		navigat->near.pid = &r_near_pid;
	}else
		navigat->near.pid = NULL;

	//navigat->near.sta=0;
	navigat->near.c_right =navigat->near.c_left = 0;
	//navigat->near.obst = 0;		//����û���ϰ���


	//init_nearpid_differ();
	init_nearpid_differ(0,0,2);

	//navigat->near.c_near =0;
	//navigat->near.c_lost = 800;
//	navigat->near.count = 0;
	//navigat->near.l_angle = sys->angle;
	//navigat->near.angle = 0;
	//navigat->near.yaw = sys->angle;
}
//����ʱ�䣬�ϱ�
void calc_timr_upload(void)
{
	uint8_t tmr;
	tmr = (msTmr - sys->t_work) / 60000;
	if(tmr != sys->work_tmr)
	{
		sys->work_tmr = tmr;		//����ʱ��  sΪ��λ
#if(0)
	///uint8_t temp_state=0;
	if(sys->sState == SYS_IDLE)
	{
		if(sys->power_on == 1)
			temp_state = sys->sState | 0x80;
		else
			temp_state = sys->sState & (~(0x80));
	}
	else
	{
		if(sys->sState_auxi == 0)
			temp_state = sys->sState | 0x80;
		else
			temp_state = SYS_DOCK | 0x80;
	}
#endif
	//dp_tx_data(PD_MSG_WORK_STA, temp_state/*sys->sState*/,sys->work_mod,sys->work_tmr,0);
//		dp_tx_data(PD_MSG_WORK_STA, sys->sState,sys->work_mod, sys->work_tmr, 0);
	}
}
