/*
���
:
����		����		����		����
2264		2289		73			31.013698630136986301369863013699
2174		2202		70			31.057142857142857142857142857143
2779		2822		91			30.53846153846154

*/

#include "sys.h"


//LASER *laser,Laser;
#define  MAX_LASER_RX_LEN		128
uint8_t laser_rx_buff[MAX_LASER_RX_LEN]={0};
int laser_len = 0;

volatile uint16_t l_dist;




void init_laser(void)
{
	//laser = &Laser;
#if !LASER_EN
	l_dist = 250;		//���ü��⣬����̽�ⲻ��
#endif
}

void begin_scan(navigat_t *navi)
{
//#if LASER_EN
	int i;
	LASER *laser = &navi->laser;
/*
	laser->s_count=0;
	for(i=0;i<MAX_LASER_SCAN;i++)
	{
		laser->s_x[i] = laser->s_x_org[i] =0;
		laser->s_y[i] = laser->s_y_org[i] =0;
	}

	laser->lst_tx 	= laser->lst_ty =0;
	laser->t_x_org 	= laser->t_y_org = 0;
	laser->count 	= 0;
	*/
	laser->c_scan	= 0;
	//laser->dist_0 = laser->dist_90 = laser->dist_180 =laser->dist_270 = 0;
	laser->c_dist = 0;
	if(l_dist > 0 && l_dist <MAX_LASR_DIST)
	{
		for(i=0;i<3;i++)
		{
			laser->m_dist[laser->c_dist] = l_dist;
			laser->s_angle[laser->c_dist++] = sys->angle;
		}
	}

//#endif
}


/*
���˶��м���ɨ������ϰ�������
*/

int laser_calc_move(navigat_t *navi,int max_dist)
{
	static int x=0,y=0;
//	struct laser_point_t *plaser;
#if TEST_FOR_LABO
	if(l_dist > 0 && l_dist <= max_dist  &&
	 (dis_float(sys->angle,90) < 10 || dis_float(sys->angle,0) < 10  || dis_float(sys->angle,180) < 10  || dis_float(sys->angle,90) < 270 ))
#else
	if(l_dist > 0 && l_dist <= max_dist )
#endif
	{

		int x_obst,y_obst;
		int m_x_org,m_y_org;
		int tx,ty;
		int dist ;
		//log_printf("%d,%3.3f\r\n",l_dist,sys->angle);
		coordinate_calcu(0);			//���������ֵ�����
		//�����Բ�ĵ�mm���������
		m_x_org = navi->x_org / CM_PER_HW;
		m_y_org = navi->y_org / CM_PER_HW;
		//�ϰ����mm����
		dist = l_dist+RADIUS_CM;

		x_obst = m_x_org + (int)((float) dist * sin(format_agle(sys->angle, ANGLE_360)*D2R));
		y_obst = m_y_org - (int)((float) dist	* cos(format_agle(sys->angle, ANGLE_360)*D2R));
		//ӳ���ͼ����
		tx = (x_obst / LASER_RES)	+	100;
		ty = (y_obst / LASER_RES)	+	100;
		if(x != tx || y!= ty)
		{
			set_osbt_l(m_x_org,m_y_org,tx,ty,x_obst ,y_obst ,1,0,dist,sys->angle,"scan");
			x = tx;
			y = ty;
		}
		l_dist = 0;
		return dist;

	}
	return l_dist;

}
#if 0
char motor_go_draw_map(navigat_t *navi)
{
	//	int xx=0,yy=0,x1=0,y1=0;
		int calue=0;

		uint8_t sta;
		int llx=0,lly=0;
		float lost_angle=NO_ANGLE_V;
//		int lx=X_NOW,ly=Y_NOW;
	//	int bx = X_NOW;
//		int by = Y_NOW;
//		int bx = X_NOW;
///		int nx1,ny1;
//		float lagle;		//��¼�����ӱ߽Ƕȣ����ڼ����Ƿ�ת�����
		uint16_t	gSta;
		uint32_t	t_begin;//��ʼ��ʱ��
		int dist;
		//��¼�ӱ߿�ʼ�������Ƿ��ӱ����
		int x_begin=0,y_begin=0;
		char n_sta=0;
	//	int ccc=0;

//		int x_begin_line=0,y_begin_line = 0;		//һ���߿�ʼ��X��Y������
		//float angle_line=NO_ANGLE_V;							//һ���ߵĽǶ�
	//	int ret;
	//	int c_lost=0;
//		u8 irData_bak[6];
//		int c_dock = 0;

	//	float begin_angle = sys->angle; 	//��ʼ�ĽǶ�
		NEAR_WALL *near = &navigat->near;
	//	motor_run(GO_ROUND,600,0,0);

		pid_init(GO_FORWARD_PWM);
		navigat->out =navigat->angle;
		motor.c_left_hw = motor.c_right_hw = 0;
		motor_run(GO_FORWARD, GO_FORWARD_PWM, 0, 0);
		init_near_wall_navi(NO_SIDE_NEAR);
		//init_near_wall_navi(NO_SIDE_NEAR);	//�տ�ʼ����Ҫһֱ�ر�
		navigat->is_walk =1;
	//	log_printf("[motor_go_edgeways]sta=%d,type=%d,is_save=%d,now=(%d,%d,%d,%d)\r\n",navigat->near.n_sta,type,is_save,X_NOW,Y_NOW,navigat->x_org,navigat->y_org);
		//navigat->wheel_dir = 1;
		sta = sys->sState = SYS_NAVIGAT;
//		lagle = sys->angle;
		mstimeout(&t_begin,0);
		while(1)
		{

			proc_uart_task();
			get_sensers(&sys->gSta);
			if(sys->sState !=sta)
			{
				motor_run(GO_STOP,0,0,0);
				return RET_NEAR_ERROR;
			}
			dist = laser_calc_move(navi,MAX_LASR_DIST);

			//��ײ
			if(sys->gSta & (MASK_BUM_MIDL) || MIDLE_IRDA()  )
			{

				gSta = sys->gSta;	//��¼��ײ��״̬
				motor_run(GO_STOP,0,0,0);
				log_printf("\r\n----draw map bum(%d,%d,%d,%3.1f),gsta=%d,irda=(%d,%d,%d,%d,%d,%d,%d)dist=%d\r\n",navigat->tx,navigat->ty,motor.c_left_hw,sys->angle,
								sys->gSta,sys->g_sta[0],sys->g_sta[1],sys->g_sta[2],sys->g_sta[3],sys->g_sta[4],sys->g_sta[5],sys->g_sta[6],dist);

				www_idleintel_com();
				delay_ms(200);
				coordinate_calcu();

				if(sys->gSta & (MASK_BUM_MIDL))
				{
					motor_back_off(10);
					coordinate_calcu();
				}
				coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty); //����ϵת��
				//log_printf("cleft=%d\r\n",motor.c_left_hw);

				//while(1);


				navigat->distance = 0;
				motor.c_left_hw = 0;

				if(x_begin ==0)
				{
					x_begin = X_NOW;
					y_begin = Y_NOW;
					n_sta = 1	;
				}
				if(near->n_sta ==NO_SIDE_NEAR)
				{

					init_near_wall_navi(RIGHT_SIDE_NEAR);
				}

				//robot_turn_deg(GO_LEFT,650,WALL_LOST_DEG);

				if((gSta & MASK_BUM_LEFT) || MIDLE_HAVE_IRDA2())
				{
					//log_printf("midbum,60deg\r\n");
					robot_turn_deg(GO_LEFT,650,WALL_LOST_DEG);
				}else if((gSta & MASK_BUM_RIGHT ) && RIGHT_IRDA2() )
				{

					robot_turn_deg(GO_LEFT,650,12);
				}else
				{
					//log_printf("midbum,30deg\r\n");
					//robot_turn_deg(GO_LEFT,650,30);
					robot_turn_deg(GO_LEFT,650,25);
				}

				coordinate_calcu();
				motor_run(GO_STOP,0,0,0);
				log_printf("after bk(%d,%d,%d,%3.3f,%3.3f,%3.3)\r\n==============\r\n",navigat->tx,navigat->ty,motor.c_left_hw,navigat->x_org_f,navigat->y_org_f,sys->angle);
				//if(ccc++ >=5)
				//	while(1);

				if(disXY(X_NOW,100) <=2  && disXY(Y_NOW,100)<=2 && n_sta ==1)
				{
					n_sta = 2;
					log_printf("to begin point..\r\n");
				}
				if(disXY(X_NOW,x_begin) <=2  && Y_NOW == y_begin && n_sta ==2)
				{

					log_printf("scan OK\r\n");
					motor_run(GO_STOP,0,0,0);
					return 0;
				}

				navigat->is_walk = 1;
				pid_init(GO_FORWARD_PWM);
				navigat->out =sys->angle;
				motor_run(GO_FORWARD, GO_FORWARD_PWM, 0, 0);
				navigat->near.pid->c_lost = 0;
				navigat->wheel_dir = 1;
				//��¼����λ��
//				x_begin_line = X_NOW;
//				y_begin_line = Y_NOW;
			//	motor_run(GO_RIGHT_RD,0,0,0);
			//	navigat->near_sta = LOST_WALL_RIGHT;
			}

			if(TIM5->CNT >=5000)
			{
				TIM5->CNT = 0;
				www_idleintel_com();

				navigat->out = format_agle(navigat->out,ANGLE_360);
				proc_line_pid(navigat->out);
				navigat_near_wall_pid(&navigat->out,5);
				if(sys->g_sta[6] < cfg->min_right_adc)
				{
					if(lost_angle ==NO_ANGLE_V)
					{
						lost_angle = sys->angle;
						log_printf("lost angle=%3.1f\r\n",sys->angle);
					}
					if(near->n_sta !=NO_SIDE_NEAR)
					{
						if(dis_float(lost_angle , sys->angle) >90)
						{
							log_printf("lost angle over,angle=%3.1f,not near\r\n",sys->angle);
							init_near_wall_navi(NO_SIDE_NEAR);

						}
					}
				}else
				{
					if(near->n_sta ==NO_SIDE_NEAR)
					{
						/*
						if(x_begin ==0)
						{
							x_begin = X_NOW;
							y_begin = Y_NOW;
							n_sta = 1	;
						}
						*/
						init_near_wall_navi(RIGHT_SIDE_NEAR);
					}
					lost_angle = NO_ANGLE_V;
				}


				if(calue++ >=40)
				{
					coordinate_calcu(); 													//�����ԭʼ������ϵ
					coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty); //����ϵת��

					if(disXY(X_NOW,100) <=2  && disXY(Y_NOW,100)<=2 && n_sta ==1)
					{
						n_sta = 2;
						log_printf("to begin point..\r\n");
					}
					if(disXY(X_NOW,x_begin) <=2  && Y_NOW == y_begin  && n_sta ==2)
					{

						log_printf("scan OK\r\n");
						motor_run(GO_STOP,0,0,0);
						return 0;
					}
					if(llx!=navigat->tx || lly!=navigat->ty)
					{
#if WALK_PRINTF
						log_printf("*(%d,%d,%3.1f,0)-[%3.1f,%3.1f,0]*\r\n",navigat->tx,navigat->ty,sys->angle,navigat->x_org_f,navigat->y_org_f);
#endif
						llx = navigat->tx;
						lly = navigat->ty;
					}
				}

			}

		}
		//delay_ms(1000);

}


#endif
/*
������
*/
char motor_laser_go_forwark(int hw)
{
	//uint16_t g_sta;
//	uint16_t last_ird;
//	int tmr=0;//,tg=0;
	//int	c_stop = 0;		//ֹͣ�Ĵ���
	static int c_calc = 0;
	static int scan_x = 0,scan_y=0;
	int ret =1;
//	int y_dir,side_s=0;
	uint8_t  sta = sys->sState ;
	int lost_dist=0;
	int c_obst=0;		//�����ƶ����ϰ���ĸ���



	pid_init(GO_FORWARD_PWM);

	www_idleintel_com();  //ȡһ�½Ƕ�
	motor.c_left_hw = motor.c_right_hw = motor.c_front_hw = 0;
	//motor.c_left_hw = motor.total_left_hw = motor.total_right_hw=0;

	navigat->side_obst = 0;
	navigat->c_side_obst = 0;
	sys->slip =0;

	navigat->out = navigat->angle;
	navigat->adj_run_ret = FALSE;

	navigat->walk_dis = 0;
	navigat->is_walk = 1;
	sys->gsta= 0;

	TIM5->CNT = 0;
	init_near_wall_navi(NO_SIDE_NEAR); //NO_SIDE_NEAR

	log_printf("[motor_go_fw](%d,%d,)-(%d,%d),(%3.3f,%3.3f)a=%3.1f,ldist=%d\r\n",navigat->tx,navigat->ty,navigat->x_org,navigat->y_org,navigat->x_org_f,navigat->y_org_f,sys->angle,l_dist);
	//CHECK_NAVI_STA_RT(0);
#if !CALE_BY_BKHW
	bx = navigat->x_org_f;		//��¼��ԭʼ������
	by = navigat->y_org_f;
#endif
	motor_run(GO_FORWARD, GO_FORWARD_PWM, 0, 0);
	c_calc = 40;
	while(1)
	{

		get_sensers(&sys->gSta);			//
		proc_uart_task();
		if(sys->sState != sta || sys->sState == SYS_IDLE)
		{
			log_printf("SYS_IDLE2\r\n");
			motor_run(GO_STOP,0,0,0);
			delay(100);
			ret = RET_DRAW_MAP_NEAR_ERR;
			break;
		}
		//��ʧ
		if(laser_calc_move(navigat,MAX_LASR_DIST) == 0)
		{
			//log_printf("lost wall(%d)\r\n",lost_dist);
			if(lost_dist ==0)
				lost_dist = WALK_DIST();
			//Ҫ�����ϰ���󣬶�ʧ�����ܹ���
			if(c_obst >=5)
			{
				if((WALK_DIST() - lost_dist ) > 700)		//��ʧ��30������
				{
					motor_run(GO_STOP,0,0,0);
					delay_ms_sensers(200);
					ret = RET_DRAW_MAP_NEAR_LOST;
					break;

				}
			}
		}else
			c_obst++;
		if(TIM5->CNT >=5000)
		{

			TIM5->CNT = 0;


			www_idleintel_com();
			//======================�г�����==========================================
			/*
			if(WALK_DIST() > (hw-5) && hw >0)	//�������
			{
				motor_run(GO_STOP,0,0,0);
				ret = 1;
				break;
			}
			*/



			navigat->out = format_agle(navigat->angle,ANGLE_360);
			proc_line_pid(navigat->angle);
			//navigat_near_wall_pid(&navigat->angle,5);

			c_calc++;

			//�������꣬���ڼ�¼�߹��ĵط�
			//һ�δ��20����������
			if(c_calc >=50)
			{
				c_calc = 0;

				coordinate_calcu(0);														//�����ԭʼ������ϵ
				coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��

				if(scan_x != navigat->tx || scan_y != navigat->ty)
				{
					scan_x = navigat->tx;
					scan_y = navigat->ty;
					//set_scan_point(navigat->tx,navigat->ty,1,RC_GRID);
					log_printf("*(%d,%d,%3.1f,0)[%d,%d,%3.3f,%3.3f]*\r\n",navigat->tx,navigat->ty,sys->angle,navigat->x_org,navigat->y_org,navigat->x_org_f,navigat->y_org_f);
				}

			}
		}

		if((sys->gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT)) ||(sys->gSta & (MASK_BUM_LEFT | MASK_BUM_LEFT2 | MASK_BUM_RIGHT | MASK_BUM_RIGHT2))||  (MIDLE_HAVE_IRDA() && WALK_DIST() > 100) )		//��ײ��
		{
            	sys->gsta = sys->gSta ;

			log_printf("[go_forw]bum!gSta=%d(%d),ir=(%d,%d,%d)%d,dist=%d!!",sys->gSta,sys->gsta,sys->g_sta[2] ,sys->g_sta[3] ,sys->g_sta[4],MIN_MIDLE_IDA,l_dist);

			motor_run(GO_STOP,0,0,0);



			if(sys->gSta & (MASK_BUM_LEFT | MASK_BUM_RIGHT))
			{
				navigat->is_bum = 1;

				delay_ms_sensers(200);
				motor_back_off(20);
				delay_ms_sensers(100);
			}else
				delay_ms_sensers(200);
			ret = RET_DRAW_MAP_NEAR_BUM;
			break;
			//delay_ms_sensers(100);
			/*
			log_printf("turn left 90\r\n");
			robot_turn_deg(GO_LEFT,650,90);
			init_near_wall_navi(NO_SIDE_NEAR); //NO_SIDE_NEAR
			pid_init(GO_FORWARD_PWM);
			log_printf("go fw\r\n");
			lost_dist= 0;
			delay_ms_sensers(300);
			gyro_offset_manage(500);
			www_idleintel_com();
			navigat->out = sys->angle;
			motor_run(GO_FORWARD, GO_FORWARD_PWM, 0, 0);
			*/

		}
	}

	motor_run(GO_STOP,0,0,0);
	return ret;
}


int laser_move_scan(void)
{
	int ret ;
	sys->sState = SYS_NAVIGAT;
	navigat->angle = 0;
	while(1)
	{
		ret = motor_laser_go_forwark(0);
		if(ret == RET_DRAW_MAP_NEAR_ERR)
		{
			motor_run(GO_STOP,0,0,0);
			log_printf("draw map err\r\n");
			break;
		}
			/*
		if(ret == RET_DRAW_MAP_NEAR_BUM)
		{
			log_printf("draw map,bum\r\n");
			turn_to_deg(format_agle(navigat->angle + 90,ANGLE_360));
		}else if(ret == RET_DRAW_MAP_NEAR_LOST)
		{
			log_printf("draw map,lost\r\n");
			turn_to_deg(format_agle(navigat->angle - 90,ANGLE_360));
		}else
			break;
		*/
		motor_run(GO_STOP,0,0,0);
		delay_ms_sensers(300);
	}
	log_printf("draw map over\r\n");
	while(1);
//	motor_run();
}

/*
int front_laser_scan(navigat_t *navi,int type)
{

	int dist;
	int ldist;



	//�⵽����
	if(l_dist > 0 && l_dist <= 120)
	{

		LASER *laser = &navi->laser;
		ldist = l_dist;
		//�����׼��������
		if(laser->c_scan == 0)
		{
			laser->c_scan++;
			l_dist =0;
			//log_printf("break out dis=%d\r\n",l_dist);
			return ldist;
		}

		//log_printf("laser->m_dist[%d]=%d;laser->s_angle[%d]=%3.5ff\r\n",laser->c_dist,l_dist,laser->c_dist,sys->angle);
		if(laser->c_dist < MAX_LASER_SCAN)
		{
			laser->m_dist[laser->c_dist] = l_dist;
			laser->s_angle[laser->c_dist++] = sys->angle;
		}

		l_dist = 0;
		return ldist;
	}
//#endif
	return 0;
}
*/


/*
����ɨ����룬�洢����ͽǶȡ�
 type  0 -  תȦ��   1  -- ֱ��
*/

int laser_scan_dist(navigat_t *navi,int type)
{
//#if LASER_EN


//	int x_obst,y_obst;	//ԭʼ���꣬1mm���������
//	int tx,ty;			//ӳ������
	//log_printf("%d\r\n",l_dist);
	int dist;
	int ldist;

	/*
	if(type == LASER_CALC_TURN)
		dist = MAX_LASR_DIST;
	else
		dist = 60;
	*/
	dist = MAX_LASR_DIST;

	//�⵽����
	if(l_dist > 0 && l_dist <= dist)
	{

		LASER *laser = &navi->laser;
		ldist = l_dist;
		//�����׼��������
		if(laser->c_scan == 0)
		{
			laser->c_scan++;
			l_dist =0;
			//log_printf("break out dis=%d\r\n",l_dist);
			return ldist;
		}

		//����ǶȺ;���
		//log_printf("laser->m_dist[%d]=%d;laser->s_angle[%d]=%3.5ff\r\n",laser->c_dist,l_dist,laser->c_dist,sys->angle);
		if(laser->c_dist < MAX_LASER_SCAN)
		{
			laser->m_dist[laser->c_dist] = l_dist;
			laser->s_angle[laser->c_dist++] = sys->angle;
		}
		//log_printf("%d,%3.1f\r\n",l_dist,sys->angle);


		//laser->c_scan++;
#if 0
		x_obst = navi->x_org / CM_PER_HW + (int)((float)(RADIUS_CM + l_dist) * navi->cosA);
		y_obst = navi->y_org / CM_PER_HW + (int)((float)(RADIUS_CM + l_dist) * navi->sinA);

		tx = (x_obst / 10)	+	100;
		ty = (y_obst / 10) 	+ 	100;
		//log_printf("*(%d,%d,%3.1f,0)[%d,%d,%d]*",tx,ty,sys->angle,x_obst,y_obst,l_dist);
		/*

		if(laser->s_count < MAX_LASER_SCAN)
		{
			laser->s_x[laser->s_count] = tx;
			laser->s_y[laser->s_count] = ty;
			laser->s_x_org[laser->s_count] = x_obst;
			laser->s_y_org[laser->s_count++] = y_obst;
		}
		//�洢*/

		//��ͬ�Ŵ�����ͬ��Ҫ������
		if(laser->lst_tx == tx && laser->lst_ty == ty)
		{
			laser->t_x_org += x_obst;
			laser->t_y_org += y_obst;
			laser->count ++;
			//log_printf("c=%d\r\n",laser->count);

		}else		//��¼�ϴε�ֵ
		{
			//log_printf("not same\r\n");
			//������һ�ε�
			if(laser->count)
			{
				//��������
				if(laser->s_count < MAX_LASER_SCAN)
				{
					laser->s_x[laser->s_count] = tx;
					laser->s_y[laser->s_count] = ty;
					laser->s_x_org[laser->s_count] = x_obst;
					laser->s_y_org[laser->s_count] = y_obst;
					laser->s_angle[laser->s_count] = sys->angle;
					laser->s_count++;
				}
				//log_printf("#(%d,%d,%3.1f,s)[%d,%d,%d]#\r\n",
				//	laser->lst_tx,laser->lst_ty,sys->angle,laser->t_x_org / laser->count,laser->t_y_org / laser->count,l_dist);
				//set_osbt_l(laser->lst_tx,laser->lst_ty,laser->t_x_org / laser->count,laser->t_y_org / laser->count,1,0,"scan");
			}
			//�洢��������
			laser->lst_tx = tx ;
			laser->lst_ty = ty ;
			//����������

			laser->t_x_org = laser->t_y_org = 0;


			laser->count = 0;

		}

		//set_osbt_l(tx,ty,x_obst,y_obst,1,0,"lasr");

#endif
		l_dist = 0;
		return ldist;
	}
//#endif
	return 0;
}


/*
�ж��Ƿ���һ��ֱ��
type  CHECK_X_LINE  	0x00		//Y�᲻��������X���һ����
	  CHECK_Y_LINE		0x01		//xh

	  x,y��ǰ�ĵ�
	  c - ������ɢ�ķ�Χ�����Ҹ����������Ҹ����������ܹ�5��������1�������ܹ�3��
*/

#define OBST_XX(X,Y)  (OBST_XY(x,y) || OBST_XY(x,y-1)|| OBST_XY(x,y+1))	//X��ǰ��
#define OBST_YY(X,Y)  (OBST_XY(x,y) || OBST_XY(x-1,y)|| OBST_XY(x+1,y))	//X��ǰ��


OBST * get_side_obst_xx(int x,int y)
{
	if(OBST_XY(x,y) )
		return get_obst(x,y);
	if(OBST_XY(x,y-1))
		return get_obst(x,y-1);
	if(OBST_XY(x,y+1))
		return get_obst(x,y+1);
	return NULL;
}

OBST * get_side_obst_yy(int x,int y)
{
	if(OBST_XY(x,y) )
		return get_obst(x,y);
	if(OBST_XY(x+1,y))
		return get_obst(x,y-1);
	if(OBST_XY(x-1,y))
		return get_obst(x,y+1);
	return NULL;
}



/*
ȡ���ĸ��ǵľ���
*/
int get_angle_4(LASER *laser,int *dist_0,int *dist_90,int *dist_180,int *dist_270)
{
	int i;
	int c0=0,c90=0,c180=0,c270=0;
	*dist_0 = *dist_90 =  *dist_180 = *dist_270 = 0;
	for(i = 0;i<laser->c_dist;i++)
	{

		if(dis_float(laser->s_angle[i],0) < 4)
		{
			if(*dist_0==0)			//û�и�ֵ�������ֵ
			{
				c0++;
				*dist_0 += laser->m_dist[i];
				log_printf("dist0=%d\r\n",laser->m_dist[i]);
			}
			else 		//֮ǰ��⵽�ˣ����жϺ�ߵ��Ƿ���첻���������̫����Ҳ��Ϊʧ��
			{
				if(disXY(laser->m_dist[i],*dist_0 ) > 10)
				{
					*dist_0  = NO_DIST;
					c0 = 0;
					log_printf("errdist0=%d\r\n",*dist_0);
				}
			}
		}
		//�仯̫������Ϊ�ⲻ������
		if(dis_float(laser->s_angle[i],180) < 4)
		{
			if(*dist_180 ==0)
			{
				c180++;
				*dist_180 += laser->m_dist[i];
				log_printf("dist180=%d\r\n",laser->m_dist[i]);
			}
			else
			{
				if(disXY(laser->m_dist[i],*dist_180) > 10)
				{
					*dist_180 = NO_DIST;
					c180 = 0;
					log_printf("errdist180=%d\r\n",laser->m_dist[i]);
				}
				}
		}
			if(dis_float(laser->s_angle[i],90) < 4)
			{
				if(*dist_90 ==0)
				{
					c90++;
					*dist_90 += laser->m_dist[i];
					log_printf("dist90=%d\r\n",laser->m_dist[i]);
				}
				else
				{
					if(disXY(laser->m_dist[i],*dist_90) > 10)
					{
						log_printf("errdist90=%d\r\n",laser->m_dist[i]);
						*dist_90 = NO_DIST;
						c90 = 0;
					}
				}
			}
			if(dis_float(laser->s_angle[i],270) < 4)
			{
				if(*dist_270 ==0)
				{
					*dist_270 += laser->m_dist[i];
					c270++;
					log_printf("dist270=%d\r\n",laser->m_dist[i]);
				}
				else
				{
					if(disXY(laser->m_dist[i],*dist_270) > 10)
					{
						*dist_270  = NO_DIST;
						c270 = 0;
						log_printf("errdist270=%d\r\n",laser->m_dist[i]);
					}
				}
			}

		}
	if(c0 >=3)
	{
		*dist_0 /=c0;
	}else
		*dist_0 = 0;

	if(c90 >=3)
	{
		*dist_90 /=c90;
	}else
		*dist_90 = 0;

	if(c180 >=3)
	{
		*dist_180 /=c180;
	}else
		*dist_180 = 0;

	if(c270 >=3)
	{
		*dist_270 /=c270;
	}else
		*dist_270 = 0;
	return 0;

}


/*
	����X,Y��0�㣬90�㣬180�㣬270��ľ���������
int dist0,int dist90,int dist270,int dist180  0�㣬90�㣬180�㣬270��ľ��룬�о������ȥ�������
dis ������ϰ���֮���������Χ�ڲ��ܽ���

g_obst  //�м������λ�ô洢�����������ݣ���һ��Ҫ�ع�
g_len	//�м�����ĸ���


*/
/*
int save_storage(OBST *obst,int *g_len,int tx,int ty,int x_org,int y_org)
{
	if(*g_len < MAX_OBST_STORAGE-1)
	{
		g_obst[*g_len].x = X_NOW;
		g_obst[*g_len].y = Y_NOW;
		g_obst[*g_len].x_org = x_org;
		g_obst[(*g_len)++].y_org = y_org;
		return 1;
	}
	return 0;
}
*/
/*
����ֹʱ�̵Ĳ�࣬
��Ҫ��Ϊ�˴���
���� 0�㣬90�㣬180�㣬270�������Ƕ��ϵ��ϰ���
û���ϰ����ʱ�����ϰ�����ϰ����ʱ�򣬽�����


void proc_static_laser(navigat_t *navi,int dist,float angle,int tx,int ty,int m_x_org,int m_y_org)
{
	int x_dir =0,y_dir =0;
	int x_obst,y_obst;
	int x_dist,y_dist;
	int x,y;
	int i,ret;
	OBST *obst;
	log_printf("-----------------------\r\n");
	log_printf("[proc_static_laser]dist=%d,angle=%3.1f,",dist,angle);
	if(dis_float(angle,0) < 5 || dis_float(angle,360) < 5)
	{
		log_printf("0dir,");
		x_dir = 1;
	}else if(dis_float(angle,180) < 5 )
	{
		log_printf("180dir,");
		x_dir = -1;
	}else if(dis_float(angle,90) < 5 )
	{
		log_printf("90dir,");
		y_dir = 1;
	}else if(dis_float(angle,270) < 5 )
	{
		log_printf("270dir,");
		y_dir = -1;
	}else
	{
		log_printf("anther dir\r\n");
	}
	//log_printf("\r\n");
	//����MM����
	//m_x_org = ORG_TO_MM(navigat->x_org);
	//m_y_org = ORG_TO_MM(navigat->y_org);
	//�����X��Y������ľ���
	x_dist	= (int)((float)(RADIUS_CM + dist) * cos(format_agle(angle, ANGLE_360)*D2R));
	y_dist	= (int)((float)(RADIUS_CM + dist)	* sin(format_agle(angle, ANGLE_360)*D2R));
	//������ϰ����λ�á�
	x_obst 	= m_x_org + x_dist;
	y_obst 	= m_y_org + y_dist;
	log_printf("calc x_dist=%d,y_dist=%d\r\n",x_dist,y_dist);
	log_printf("now org(%d,%d),obst org(%d,%d)\r\n",m_x_org,m_y_org,x_obst,y_obst);
	//============0�㣬180�㷽��===========
	if(y_dir ==0 && x_dir !=0)
	{
		//==============================��һ����һ��ֱ�ߵĽ��� ========================
		x = X_NOW + x_dir;
		y = Y_NOW;
		//�ҵ��ϰ���
		for(i=0;i<LINE_LEN;i++)
		{
			if(OBST_XY(x,y) )
			{
				obst = get_obst(x,y);
				log_printf("found dir0 obst(%d,%d,)\r\n",x,y);
				break;
			}
			x +=x_dir;
		}

		if(obst != NULL)
		{
			//��һ���ߣ��Ƚ��ϸ�
			ret = check_line(CHECK_Y_LINE,obst->x,obst->y,2);		//�����Y�����������Ƿ����ϰ���
			log_printf("x found obst(%d,%d,%d,%d,%d,%d)",obst->x,obst->y,obst->x_org,obst->y_org,obst->tx_org,obst->ty_org);
			//�ж�ɨ���ķ���һ��
			if((obst->tx_org < obst->x_org && m_x_org < obst->x_org) ||
				(obst->tx_org > obst->x_org && m_y_org > obst->x_org) )
			{
				int x_org;
				int dis_x;
				dis_x = disxy(obst->x_org ,x_obst);			//ȡ������Ĳ�ֵ mmΪ��λ


				log_printf("[ajust_laser]xdirlin,ret=%d,obst=(%d,%d,%d,%d),now=(%d,%d,%d,%d),disx=%d\r\n",ret,
					obst->x,obst->y,obst->x_org,obst->y_org,navi->tx,navi->ty,m_x_org  ,m_y_org  ,dis_x);
				x_org = (obst->x_org  - x_dist) * CM_PER_HW + CM_PER_HW / 3;
				if ( ret  && dis_x > 1 && dis_x < 80)	//�Ƚ���
				{
					log_printf("a big line,ajust ,x_org :%d -> %d\r\n",navi->x_org,m_x_org);
					ajust_xy_org(x_org,0,100,AJUST_X);
					return ;
				}
			}else
				log_printf("dist0 not the same side\r\n");
		}
		//======================================�ڶ������ж��ϰ��������Ƿ����ϰ������ϰ���==================



	}

}
*/

#define IDX_DIR0		0
#define IDX_DIR90		1
#define IDX_DIR180		2
#define IDX_DIR270		3
int ajust_xy_by_laser(navigat_t *navi ,int dist,float angle,int dis)
//void proc_static_laser(navigat_t *navi,int dist,float angle)
{
#if LASER_EN
	int x,y;
	OBST	*obst,*m_obst[4];
//	int x_obst,y_obst;
	int x_org,y_org;
	int ret,ret1;
	//int m_x_org,m_y_org;
	int i;//,j;
	int dist0, dist90, dist270, dist180;
//���� <���������㷨>����ҳ
	//0�ȷ�����Ч���� X+����


	//��֤X��

	dist0=dist90=dist270=dist180=0;
	if(dis_float(angle,0) < 2)
		dist0 = dist;
	else if(dis_float(angle,90) < 2)
		dist90 = dist;
	else if(dis_float(angle,180) < 2)
		dist180 = dist;
	else if(dis_float(angle,90) < 2)
		dist270 = dist;
	else
		return 0;
//	log_printf("-");
	log_printf("[laser]dis=%d,deg=%3.1f%d\r\n",dist,angle);


	//=================�ĸ���λ�ǵ��ϰ���=================
	for(i=0;i<4;i++)
	{
		m_obst[i] = NULL;
	}
	//x+����
	x = X_NOW;y=Y_NOW;
	if(dist0 > 0 && dist0 <= (MAX_LASR_DIST+RADIUS_CM))
	{
		for(i=0;i<LINE_LEN;i++)
		{
			if(OBST_XY(x,y) && OBST_XY(x,y-1) && OBST_XY(x,y+1))
			{
				//������һ���Ǹ����ֱ�ߣ��Һ��û����ɨ�ģ���ȥ��ߵ�
			//	if(( OBST_XY(x+1,y+1) OBST_XY(x+1,y+2) && OBST_XY(x+1,y+3) &&  OBST_XY(x+1,y+4)) ||
			//		(OBST_XY(x+1,y-1) &&  OBST_XY(x+1,y-2) && OBST_XY(x+1,y-3) &&  OBST_XY(x+1,y-4)))
				m_obst[IDX_DIR0] = get_obst(x,y);
				log_printf("dir0(%d,%d,)\r\n",x,y);
				break;
			}
			//����ӣ����ӵĻ�������Ƚϴ������
			if(OBST_XY(x,y) || OBST_XY(x,y-1) || OBST_XY(x,y+1))
			{
				break;
			}
			/*
			//�ȽϿ����ĵط�����У�飬����û���ϰ�����������ܲ�׼
			��������������⣬������ϰ��������ˣ��ͻ������⡣
			���ӹ���Ҳ������⡣
			if(OBST_XY(x,y+1) || OBST_XY(x,y-1) )
			{
				log_printf("found dir0side err(%d,%d,)\r\n",x,y);
				break;
			}
			*/
			x++;
		}
	}
	//x+����
	x = X_NOW;y=Y_NOW;
	if(dist180 > 0 && dist180 <= (MAX_LASR_DIST+RADIUS_CM))
	{
		for(i=0;i<LINE_LEN;i++)
		{
			if(OBST_XY(x,y) && OBST_XY(x,y-1) && OBST_XY(x,y+1))
			{
				m_obst[IDX_DIR180] = get_obst(x,y);
				log_printf("dir180(%d,%d,)\r\n",x,y);
				break;
			}
			if(OBST_XY(x,y) || OBST_XY(x,y-1) || OBST_XY(x,y+1))
			{
				//m_obst[IDX_DIR180] = get_obst(x,y);
				//log_printf("dir180(%d,%d,)\r\n",x,y);
				break;
			}
			//�ȽϿ����ĵط�����У�飬����û���ϰ�����������ܲ�׼
			/*
			��������������⣬������ϰ��������ˣ��ͻ������⡣
			���ӹ���Ҳ������⡣
			if(OBST_XY(x,y+1) || OBST_XY(x,y-1) )
			{
				log_printf("found dir180side err(%d,%d,)\r\n",x,y);
				break;
			}
			*/
			x--;
		}
	}

	//y+����
	x = X_NOW;y=Y_NOW;
	if(dist90 > 0 && dist90 <= (MAX_LASR_DIST+RADIUS_CM))
	{
		for(i=0;i<LINE_LEN;i++)
		{
			//(OBST_XY(x,y))
			if(OBST_XY(x,y) && OBST_XY(x-1,y) && OBST_XY(x+1,y))
			{
				m_obst[IDX_DIR90] = get_obst(x,y);
				log_printf("dir90(%d,%d,)\r\n",x,y);
				break;
			}
			if(OBST_XY(x,y) || OBST_XY(x-1,y) || OBST_XY(x+1,y))
			{
			//	m_obst[IDX_DIR90] = get_obst(x,y);
			//	log_printf("dir90(%d,%d,)\r\n",x,y);
				break;
			}
			/*

			//�ȽϿ����ĵط�����У�飬����û���ϰ�����������ܲ�׼
			��������������⣬������ϰ��������ˣ��ͻ������⡣
			���ӹ���Ҳ������⡣
			if(OBST_XY(x+1,y) || OBST_XY(x-1,y) )
			{
				log_printf("found dir90 side err(%d,%d,)\r\n",x,y);
				break;
			}
			*/
			y++;
		}
	}

	//y+����
	x = X_NOW;y=Y_NOW;
	if(dist270 > 0 && dist270 <= (MAX_LASR_DIST+RADIUS_CM))
	{
		for(i=0;i<LINE_LEN;i++)
		{

			if(OBST_XY(x,y) && OBST_XY(x-1,y) && OBST_XY(x+1,y))
			{
				m_obst[IDX_DIR270] = get_obst(x,y);
				log_printf("dir270(%d,%d,)\r\n",x,y);
				break;
			}
			if(OBST_XY(x,y) || OBST_XY(x-1,y) || OBST_XY(x+1,y))
				break;
			/*
			//�ȽϿ����ĵط�����У�飬����û���ϰ�����������ܲ�׼
			��������������⣬������ϰ��������ˣ��ͻ������⡣
			���ӹ���Ҳ������⡣

			if(OBST_XY(x+1,y) || OBST_XY(x-1,y) )
			{
				log_printf("found dir270 side err(%d,%d,)\r\n",x,y);
				break;
			}
			*/
			y--;
		}
	}

//	*g_len = 0;
	//0���о��룬�����ϰ���
	if(dist0 > 0 && dist0 <=MAX_LASR_DIST && m_obst[IDX_DIR0] !=NULL)
	{
		int dis_x;
//		idx_obst = 0;
		//һ���ߣ��������
		dist0 +=RADIUS_CM;									//���ϰ뾶

		//�����ϰ����Ƿ���һ����
		obst = m_obst[IDX_DIR0];
		ret = 0;//check_line(CHECK_Y_LINE,obst->x,obst->y,1);		//�����Y������һ���Ƿ����ϰ���
		ret1 = check_line(CHECK_Y_LINE,obst->x,obst->y,1);		//�����Y�����������Ƿ����ϰ���

		//���mm���������
		x_org = navi->x_org / CM_PER_HW;		//ȡ��mm������
		y_org = navi->y_org / CM_PER_HW;

		//�ж�ɨ���ķ���һ��
		if((obst->tx_org < obst->x_org && x_org < obst->x_org) ||
			(obst->tx_org > obst->x_org && x_org > obst->x_org) )
		{

			dis_x = disxy(obst->x_org ,(x_org + dist0));			//ȡ������Ĳ�ֵ mmΪ��λ

			log_printf("[laser]0ret=%d,obst=(%d,%d,)dis=%d,dx=%d\r\n",ret1,
				obst->x,obst->y,dist0,dis_x);
			x_org = (obst->x_org  - dist0) * CM_PER_HW + CM_PER_HW / 3;
			//if(( ret == 1 && dis_x > 1 && dis_x < 20) || 	//ֻ�������ϰ��������΢���̵㣬20ms����
			  if ( ret1 == 1 && dis_x > 1 && dis_x < 20)	//�Ƚ���
			{
				//log_printf("a big line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,100,AJUST_X);
			}else /*if((ret1  || ret1) && dis_x > 1 && dis_x < 40 )
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,90,AJUST_X);
			}else	*/if(dis_x > 1 && dis_x < 7)			//������У��
			{

				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,60,AJUST_X);
			}
		}else
			log_printf("dist0 not the same side\r\n");
		/**/


	}
	if(dist180 > 0 && dist180 <=MAX_LASR_DIST && m_obst[IDX_DIR180] !=NULL)
	{
		int dis_x;
//		idx_obst = 0;
		//һ���ߣ��������
		dist180 +=RADIUS_CM;									//���ϰ뾶

		//�����ϰ����Ƿ���һ����
		obst = m_obst[IDX_DIR180];
		//ret = check_line(CHECK_Y_LINE,obst->x,obst->y,1);		//�����Y������һ���Ƿ����ϰ���
		ret1 = check_line(CHECK_Y_LINE,obst->x,obst->y,1);		//�����Y�����������Ƿ����ϰ���

		//���mm���������
		x_org = navi->x_org / CM_PER_HW;		//ȡ��mm������
		y_org = navi->y_org / CM_PER_HW;
		if((obst->tx_org < obst->x_org && x_org < obst->x_org) ||
			(obst->tx_org > obst->x_org && x_org > obst->x_org) )
		{
			dis_x = disxy(obst->x_org ,(x_org - dist180));			//ȡ������Ĳ�ֵ mmΪ��λ


			log_printf("[ajust_laser]180ret=%d,obst=(%d,%d,),dist=%d,disx=%d\r\n",ret1,
				obst->x,obst->y,dist180,dis_x);
			x_org = (obst->x_org  + dist180) * CM_PER_HW + CM_PER_HW / 3;
			if(ret1 ==1 && dis_x > 1 && dis_x < 40)	//һ��ֱ�ߣ��������
			{
				//log_printf("a big line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,100,AJUST_X);
			/*}else if((ret1  || ret1) && dis_x > 1 && dis_x < 40)
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,90,AJUST_X);*/
			}else	if(dis_x > 1 && dis_x < 7)			//������У��
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(x_org,0,60,AJUST_X);
			}
		}else
			log_printf("dist180 not the same side\r\n");


	}

	//Y+����
	if(dist90 > 0 && dist90 <=MAX_LASR_DIST && m_obst[IDX_DIR90] !=NULL)
	{
		int dis_x;
//		idx_obst = 0;
		//һ���ߣ��������
		dist90 +=RADIUS_CM;									//���ϰ뾶

		//�����ϰ����Ƿ���һ����
		obst = m_obst[IDX_DIR90];
		ret = 0;//check_line(CHECK_X_LINE,obst->x,obst->y,1);		//�����Y������һ���Ƿ����ϰ���
		ret1 = check_line(CHECK_X_LINE,obst->x,obst->y,1);		//�����Y�����������Ƿ����ϰ���

		//���mm���������
		x_org = navi->x_org / CM_PER_HW;		//ȡ��mm������
		y_org = navi->y_org / CM_PER_HW;
		if((obst->ty_org < obst->y_org && y_org < obst->y_org) ||
			(obst->ty_org > obst->y_org && y_org > obst->y_org) )
		{

			dis_x = disxy(obst->y_org ,(y_org + dist90));			//ȡ������Ĳ�ֵ mmΪ��λ


			log_printf("[ajust_laser]90dirlin,ret1=%d,ret2=%d,obst=(%d,%d,%d,%d),now=(%d,%d,%d,%d),dist=%d,disx=%d\r\n",ret,ret1,
				obst->x,obst->y,obst->x_org,obst->y_org,navi->tx,navi->ty,x_org  ,y_org  ,dist0,dis_x);

			y_org = (obst->y_org  - dist90) * CM_PER_HW + CM_PER_HW / 3;
			if(ret1 ==1 && dis_x > 1 && dis_x < 50)	//һ��ֱ�ߣ��������
			{
				log_printf("a big line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,100,AJUST_Y);
			}else if((ret1  || ret1) && dis_x > 1 && dis_x < 40)
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,90,AJUST_Y);
			}/**/	else	if(dis_x > 1 && dis_x < 7)			//������У��
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,60,AJUST_Y);
			}
		}else
			log_printf("dist90 not the same side\r\n");


	}


	//Y+����
	if(dist270 > 0 && dist270 <=MAX_LASR_DIST && m_obst[IDX_DIR270] !=NULL)
	{
		int dis_x;
//		idx_obst = 0;
		//һ���ߣ��������
		dist270 +=RADIUS_CM;									//���ϰ뾶

		//�����ϰ����Ƿ���һ����
		obst = m_obst[IDX_DIR270];
		//ret = check_line(CHECK_X_LINE,obst->x,obst->y,1);		//�����Y������һ���Ƿ����ϰ���
		ret1 = check_line(CHECK_X_LINE,obst->x,obst->y,1);		//�����Y�����������Ƿ����ϰ���

		//���mm���������
		x_org = navi->x_org / CM_PER_HW;		//ȡ��mm������
		y_org = navi->y_org / CM_PER_HW;
		if((obst->ty_org < obst->y_org && y_org < obst->y_org) ||
			(obst->ty_org > obst->y_org && y_org > obst->y_org) )
		{
			dis_x = disxy(obst->y_org ,(y_org - dist270));			//ȡ������Ĳ�ֵ mmΪ��λ


			log_printf("[ajust_laser]270dirlin,ret1=%d,ret2=%d,obst=(%d,%d,%d,%d),now=(%d,%d,%d,%d),dist=%d,disx=%d\r\n",ret,ret1,
				obst->x,obst->y,obst->x_org,obst->y_org,navi->tx,navi->ty,x_org  ,y_org  ,dist0,dis_x);

			y_org = (obst->y_org  + dist270) * CM_PER_HW + CM_PER_HW / 3;
			if(ret1 ==1 && dis_x > 1 && dis_x < 50)	//һ��ֱ�ߣ��������
			{
				log_printf("a big line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,100,AJUST_Y);
			}else if((ret1  || ret1) && dis_x > 1 && dis_x < 40)
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,90,AJUST_Y);
			}else	if(dis_x > 1 && dis_x < 7)			//������У��
			{
				log_printf("a line,ajust ,x_org :%d -> %d\r\n",navi->x_org,x_org);
				ajust_xy_org(0,y_org,60,AJUST_Y);
			}
		}else
			log_printf("dist270 not the same side\r\n");

	}
#endif
	return 0;

}
//�ܱ��м����ϰ���
int side_obst_count(int x,int y)
{
	int c=0;
	if(OBST_XX(x,y))	c++;
	if(OBST_XX(x+1,y))	c++;
	if(OBST_XX(x-1,y))	c++;

	if(OBST_XX(x,y+1))	c++;
	if(OBST_XX(x+1,y+1))	c++;
	if(OBST_XX(x-1,y+1))	c++;

	if(OBST_XX(x,y-1))	c++;
	if(OBST_XX(x+1,y-1))	c++;
	if(OBST_XX(x-1,y-1))	c++;
	return c;

}
/*
����ͼ���غ϶�,�����غϵĸ�����
*//*
int map_coincide(struct obst_fifo_t *obst_f)
{
	int i=0,c=0;
	for(i=0;i<obst_f->len;i++)
	{
		if( OBST_XY(obst_f->obst[i].x,obst_f->obst[i].y))
			c++;
	}
	return c;

}

�ҵ�һ���ߣ�
*/

int search_laser_line(struct obst_fifo_t *obst_f,int x,int y)
{
	int i;
	int kx=0,ky=0;
	for(i=0;i<obst_f->len;i++)
	{
		if(obst_f->obst[i].x ==x)
			kx++;
		if(obst_f->obst[i].y == y)
			ky++;
	}
	if(kx >=3)
		return 1;
	if(ky >=3)
		return 2;
	return 0;
}

/*

ɨ��һȦ���������غϵ�ֱ�ߣ����Ƿ���Ҫ����
x_ok, y_ok�� X�ᣬY���Ƿ�����ȷ�ģ�
���X,Y����ȷ���򲻽���X,Y�ᣬ�������ϰ���ĵ㣬
���X,Y�᲻��ȷ�������XY��

 x_ok,y_okҲ�ǳ��Σ�ָʾ�ں�������Ƿ񱻽���������0��������������1��������ˡ�
 ��������ˣ��ϲ������Ҫ��������ajust_xy_scan_obst
 m_x_org  m_y_org �������������mm����
*/

int ajust_xy_scan_obst(struct obst_fifo_t *obst_f,int *x_ok,int *y_ok,int *m_x_org,int *m_y_org)
{
#if LASER_EN
	int i,j,k;
	int x1;//,x2,x3,x4;
	int y1;//,y2,y3,y4;
	int dir[2];
	int ret =0;  //����ֵ���غϵ�ֱ��
	int line=0;
//	int c_obst = 0;
	//�洢����������ֵ
	int x_ajust[MAX_ADJ_XY],y_ajust[MAX_ADJ_XY];
	int c_x_adj=0,c_y_adj=0;
	OBST *obst;
	log_printf("[ajust_xy_scan]obst_len=%d,x_ok=%d,y_ok=%d,mx=%d,my=%d\r\n",obst_f->len,*x_ok,*y_ok,*m_x_org,*m_y_org);
	for(i=0;i< (obst_f->len);i++)
	{
		//Ѱ��һ����
		line = search_laser_line(obst_f,obst_f->obst[i].x,obst_f->obst[i].y);
		//==============X����ȣ�һ��ƽ��Y����ߣ�===============================
		if(line ==1)
		 {

			log_printf("xline(%d,%d,)\r\n", obst_f->obst[i].x,obst_f->obst[i].y);

			//��ǰɨ������ֱ�ߵ��ϰ�������꣬X�᲻�䣬Y��仯
			x1 = obst_f->obst[i].x;
			y1 = obst_f->obst[i].y;
			//y2 = obst_f->obst[i+1].y;
			//y3 = obst_f->obst[i+2].y;
			//y4 = obst_f->obst[i+3].y;
			//0�ȷ��򣬵�ǰ����ϰ���С�ķ�������180�㷽����

			if(X_NOW < x1)
			{
				dir[0] = -1;
				dir[1] = 1;
			}
			else
			{
				dir[0] = 1;
				dir[1] = -1;
			}
			for(k=0;k<2;k++)
			{
				x1 = obst_f->obst[i].x;
				//===============�ҵ�������һ��ֱ��
				for(j=0;j<5;j++)
				{
#if 0
					if(OBST_XY(x1,y1) && OBST_XY(x1,y2)  && OBST_XY(x1,y3) && OBST_XY(x1,y4)/* &&
						SCANE_XY(x1+dir[k],y1)==0 && SCANE_XY(x1+dir[k],y2)==0  && SCANE_XY(x1+dir[k],y3)==0 &&
						SCANE_XY(x1+dir[k]*2,y1)==0 && SCANE_XY(x1+dir[k]*2,y2)==0  && SCANE_XY(x1+dir[k]*2,y3)==0 */) //�����δ��ɨ�ģ����ܽ���
#else
					if(check_line(CHECK_Y_LINE,x1,y1,2))
#endif
					{
						log_printf("other xline(%d,%d,)\r\n",x1,y1);
						//if(x1 != obst_f->obst[i].x)		//��һ�£���Ҫ����
						{
							ret++;				//ֱ�߲��غ�
							if(*x_ok >0)		//�����Ѿ��������� �ˣ����ڽ����ˣ�ֻҪ�䶯�ϰ��㼴��
							{
								if(disxy(x1,obst_f->obst[i].x)<=2)	//���첻����ĵ㲻ȡ
								{
									log_printf("may be the same point ,not get(%d,%d,)\r\n",obst_f->obst[i].x,obst_f->obst[i].y);
									obst_f->obst[i].x = obst_f->obst[i].y = 0;
									obst_f->obst[i+1].x = obst_f->obst[i+1].y = 0;
									obst_f->obst[i+2].x = obst_f->obst[i+2].y = 0;
									obst_f->obst[i+3].x = obst_f->obst[i+3].y = 0;
								}
							}else //û�б����������������
							{
								//��������
								obst = get_obst(x1 ,y1);
								if(obst !=NULL)
								{
									//int tx,ty;		//������ϰ���Ĳ��Ե��λ�ã��͵�ǰ��λ���Ƿ���һ�£�����һ�£����ܽ���
									//tx = MM_TO_XY(obst->tx_org);
									//ty = MM_TO_XY(obst->ty_org);
									//log_printf("obst scan point(%d,%d,)\r\n",tx,ty);
									if(c_x_adj < MAX_ADJ_XY -1)
									{
										if(k > 0)		//�����ϰ����ں�ߣ����ж��ϰ����ǲ�������һ������ɨ���
										{
											if((obst->tx_org < obst->x_org && *m_x_org < obst->x_org) ||
												(obst->tx_org > obst->x_org && *m_x_org > obst->x_org) )
											{
												x_ajust[c_x_adj++] = obst_f->obst[i].x_org - obst->x_org;
												log_printf("same x_ajust:%d,%d,%d,%d\r\n",x_ajust[c_x_adj-1],c_x_adj,obst_f->obst[i].x_org , obst->x_org);
											}else
												log_printf("not same side\r\n");
										}else
										{
											x_ajust[c_x_adj++] = obst_f->obst[i].x_org - obst->x_org;
											log_printf("x_ajust:%d,%d,%d,%d\r\n",x_ajust[c_x_adj-1],c_x_adj,obst_f->obst[i].x_org , obst->x_org);
										}
									}else
										log_printf("x-ajust-out\r\n");
								}
							}
						}
						break;
					}
					x1 += dir[k];
				}

			}
				//


		 }else
		 // Y���غ�
		 if(line ==2 )
		 {
			//�����غϵĴ���
		//	i+=4;
		/*
			c_obst = 0; 	//�غϵĸ���
			if(OBST_XY(obst_f->obst[i].x,obst_f->obst[i].y))		c_obst++;
			if(OBST_XY(obst_f->obst[i+1].x,obst_f->obst[i+1].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+2].x,obst_f->obst[i+2].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+3].x,obst_f->obst[i+3].y))	c_obst++;
			//if(c_obst >=3)		// 3�����ϵĵ��غϣ�����ΪX�������Ǻ�
			//	break;
			*/
			log_printf("y line (%d,%d,)\r\n", obst_f->obst[i].x,obst_f->obst[i].y);

			x1 = obst_f->obst[i].x;
			y1 = obst_f->obst[i].y;
			//x2 = obst_f->obst[i+1].x;
			//x3 = obst_f->obst[i+2].x;
		//	x4 = obst_f->obst[i+3].x;

			if(Y_NOW< y1)
			{
				dir[0] = -1;
				dir[1] = 1;
			}
			else
			{
				dir[0] = 1;
				dir[1] = -1;
			}
			for(k=0;k<2;k++)
			{
				y1 = obst_f->obst[i].y;
				//===============�ҵ�������һ��ֱ��
				for(j=0;j<5;j++)
				{
#if 0
					//log_printf("check(%d,%d,)(%d,%d,)(%d,%d,)(%d,%d,)\r\n",x1,y1,x2,y1,x3,y1,x4,y1);
					if(OBST_XY(x1,y1) && OBST_XY(x2,y1)  && OBST_XY(x3,y1) && OBST_XY(x4,y1)/* &&
						SCANE_XY(x1,y1+dir[k]) == 0 && SCANE_XY(x2,y1+dir[k]) == 0 &&SCANE_XY(x3,y1+dir[k]) == 0 &&
						SCANE_XY(x1,y1+dir[k]*2) == 0 && SCANE_XY(x2,y1+dir[k]*2) == 0 &&SCANE_XY(x3,y1+dir[k]*2) == 0*/)
#else
					if(check_line(CHECK_X_LINE,x1,y1,2))

#endif
					{
						log_printf("other y line(%d,%d,)\r\n",x1,y1);
						{
							ret++;				//ֱ�߲��غ�
							{
								//��������
								obst = get_obst(x1 ,y1);
								if(obst !=NULL)
								{
									if(c_y_adj < MAX_ADJ_XY -1)
									{
										if(k > 0)
										{
											if((obst->ty_org > obst->y_org && *m_y_org > obst->y_org) ||
												(obst->ty_org < obst->y_org && *m_y_org < obst->y_org))
											{
												y_ajust[c_y_adj++] = obst_f->obst[i].y_org - obst->y_org;
												log_printf("same y_ajust:%d,%d,%d,%d\r\n",y_ajust[c_y_adj-1],c_y_adj,obst_f->obst[i].y_org , obst->y_org);
											}else
												log_printf("not same\r\n");
										}else
										{
											y_ajust[c_y_adj++] = obst_f->obst[i].y_org - obst->y_org;
											log_printf("y_ajust:%d,%d,%d,%d\r\n",y_ajust[c_y_adj-1],c_y_adj,obst_f->obst[i].y_org , obst->y_org);
										}
									}else
										log_printf("c_y_adj error\r\n");
								}
							}
						}
						break;
					}
					y1 += dir[k];
				}


		 	}
		}

	}
	ret = 0;
	if(c_x_adj  && c_x_adj < (MAX_ADJ_XY-3))
	{
		int min=find_big_count(x_ajust,c_x_adj);
		//log_printf("xjust=%d\r\n",min);
		//if(( min < 10 &&  min >-10) || c_x_adj >=3)
		{
			log_printf("[ajust x scan]c=%d,ajust=%d,x_org=%d->",c_x_adj,min,*m_x_org);
			*m_x_org -= min;
			log_printf("%d",*m_x_org);
			ajust_xy_org(*m_x_org * CM_PER_HW ,0,100,AJUST_X);
			ret =1;
		}
	}
	if(c_y_adj  && (c_x_adj < MAX_ADJ_XY-3))
	{
		int min=0;
		min=find_big_count(y_ajust,c_y_adj);
		//log_printf("yjust=%d\r\n",min);
		//if(( min < 10 &&  min >-10) || c_y_adj >=3)
		{
			log_printf("[ajust_y_scan]c=%d,ajust=%d,y_org=%d->\r\n",c_y_adj,min,*m_y_org);
			*m_y_org -= min;
			log_printf("%d",*m_y_org);
			ajust_xy_org(0,*m_y_org * CM_PER_HW ,100,AJUST_Y);
			ret = 1;
		}

	}
	log_printf("\r\n");




	return ret;
#else
		return 0;
#endif
}
/*
�ж�ɨ���б��X,Y���غϵ�������غϱ�������OK��
*x_ok,int *y_ok  ���غϵĴ��� ����

*/
void check_ajust_sta(struct obst_fifo_t *obst_f,int *x_ok,int *y_ok)
{
	int i;
	int c_obst=0;
	*x_ok = *y_ok = 0;
	for(i=0;i<obst_f->len-4;i++)
	{
		//===============4�� һ�� ��X���غ�===============================
		if(obst_f->obst[i].x == obst_f->obst[i+1].x && obst_f->obst[i].x == obst_f->obst[i+2].x  &&
		   obst_f->obst[i].x == obst_f->obst[i+3].x )
		 {
			//�����غϵĴ���
			i+=3;
			c_obst = 0; 	//�غϵĸ���
			if(OBST_XY(obst_f->obst[i].x,obst_f->obst[i].y))		c_obst++;
			if(OBST_XY(obst_f->obst[i+1].x,obst_f->obst[i+1].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+2].x,obst_f->obst[i+2].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+3].x,obst_f->obst[i+3].y))	c_obst++;
			if(c_obst >=3)		// 3�����ϵĵ��غϣ�����ΪX�������Ǻ�
			{
				log_printf("line x ok(%d,%d,)\r\n",obst_f->obst[i].x,obst_f->obst[i].y);
				(*x_ok) ++;		//X�������Ǻϣ�����Ҫ�����ˡ�
			}
		 }
		 // Y���غ�
		 if(obst_f->obst[i].y == obst_f->obst[i+1].y && obst_f->obst[i].y == obst_f->obst[i+2].y  &&
		   obst_f->obst[i].y == obst_f->obst[i+3].y )
		 {
			//�����غϵĴ���
			i+=3;
			c_obst = 0; 	//�غϵĸ���
			if(OBST_XY(obst_f->obst[i].x,obst_f->obst[i].y))		c_obst++;
			if(OBST_XY(obst_f->obst[i+1].x,obst_f->obst[i+1].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+2].x,obst_f->obst[i+2].y))	c_obst++;
			if(OBST_XY(obst_f->obst[i+3].x,obst_f->obst[i+3].y))	c_obst++;
			if(c_obst >=3)		// 3�����ϵĵ��غϣ�����ΪX�������Ǻ�
			{
				log_printf("line y ok(%d,%d,)\r\n",obst_f->obst[i].x,obst_f->obst[i].y);
				(*y_ok) ++;		//X�������Ǻϣ�����Ҫ�����ˡ�
			}
		 }


	}

}

int in_obst_f(struct obst_fifo_t *obst_f,int x,int y)
{
	int i;
	for(i=0;i<obst_f->len;i++)
	{
		if(!(obst_f->obst[i].x == 0 && obst_f->obst[i].y==0))	//��ȥ���ˣ�������ʱ��ȥ���ˡ�
		{
			if(obst_f->obst[i].x == x && obst_f->obst[i].y == y)
				return 1;
		}
	}
	return 0;
}

/*

�ж��Աߵ��Ƿ���һ���ϰ��ߣ��ǵĻ���������ȥ

int ajust_laser_obst(int tx,int ty,int x,int y)
{
	int i;
	//�ȿ����������ǲ���X��ƽ��
	if(check_line(CHECK_X_LINE,x-1,y,1))		//���һ����
	{
		log_printf("[ajust_laser_obst]found line_1(%d,%d)");
	}
}
*/
/*
��ɨ�����Ϣ����ϰ��������
*/
#define SCAN_DEBUG   1
void scan_to_obst_map(LASER *laser ,struct obst_fifo_t *obst_f,int m_x_org,int m_y_org)
{
	int x_obst,y_obst;
	int i,j,k;
	int tx,ty,x=0,y=0;
	int t_count = 0;
	int t_x_org=0,t_y_org=0;
	float dist;
	obst_f->len = 0;
	log_printf("[calc_slaser]now=(%d,%d)c=%d\r\n",m_x_org,m_y_org,laser->c_dist);
	for(i=0;i<laser->c_dist ;i++)
	{
		//������ϰ���ĵ㣬mmΪ��λ
		dist = laser->m_dist[i];
		dist = dist / 10.0f;
#if LASER_EN | LASER_DRAW
		x_obst = m_x_org + (int)((float)(RADIUS_CM + laser->m_dist[i]) * cos(format_agle(laser->s_angle[i], ANGLE_360)*D2R));
		y_obst = m_y_org + (int)((float)(RADIUS_CM + laser->m_dist[i])	* sin(format_agle(laser->s_angle[i], ANGLE_360)*D2R));
#endif

#if LASER_SIDE
		x_obst = m_x_org + (int)((float) (RADIUS_CM + laser->m_dist[i]) * sin(format_agle(laser->s_angle[i], ANGLE_360)*D2R));
		y_obst = m_y_org - (int)((float) (RADIUS_CM + laser->m_dist[i])	* cos(format_agle(laser->s_angle[i], ANGLE_360)*D2R));
#endif
		//ӳ���ͼ����
#if 0//LASER_DRAW
		tx = (x_obst )	+	100;
		ty = (y_obst )	+	100;
#else
	tx = (x_obst / LASER_RES)	+	100;
	ty = (y_obst / LASER_RES)	+	100;
#endif

#if SCAN_DEBUG
		log_printf("   (%d,%d,%d,%d)dist=%d,%3.1f)\r\n",tx,ty,x_obst,y_obst,laser->m_dist[i],laser->s_angle[i]);
#endif
		//��ͬ�Ŵ�����ͬ��Ҫ������
		if(x == tx && y == ty)
		{
			t_x_org += x_obst;
			t_y_org += y_obst;
			t_count ++;
#if SCAN_DEBUG
			//log_printf("c=%d\,",t_count);
#endif
		}else		//��¼�ϴε�ֵ
		{
			//log_printf("not same\r\n");
			//������һ�ε�
			if(t_count)
			{
				t_x_org /= t_count;
				t_y_org /= t_count;

				if(obst_f->len < (MAX_OBST_FIFO-1))
				{
					obst_f->obst[obst_f->len].x = x;
					obst_f->obst[obst_f->len].y = y;
					obst_f->obst[obst_f->len].x_org= t_x_org;
					obst_f->obst[obst_f->len].y_org= t_y_org;
					obst_f->dist[obst_f->len] = laser->m_dist[i];
					obst_f->deg[obst_f->len]  =laser->s_angle[i];
					obst_f->len++;
#if SCAN_DEBUG
				//log_printf("get len=%d,(%d,%d)",obst_f->len,t_x_org,t_y_org);
#endif
				}else
					log_printf("obst_f->len error\r\n");
				//��������
				//set_osbt_l(x,y,t_x_org ,t_y_org ,1,0,"scan");
			}else
			{
				//С�Ĳ�࣬��һ������Ƚ�С��һ�����಻��Ҳ��Ϊ������������
				if(obst_f->len > 2 && laser->m_dist[i] < MAX_LAST_DIST)
				{
					//֮ǰ��X,��Yһ��ֱ�ߣ��ұ��κ��ϴεĲ��첻�����ý�ȥ
					if(((obst_f->obst[obst_f->len-1].x == obst_f->obst[obst_f->len-2].x &&
						disxy(x_obst,obst_f->obst[obst_f->len-1].x_org) < 6) ||
						(obst_f->obst[obst_f->len-1].y == obst_f->obst[obst_f->len-2].y &&
						disxy(y_obst,obst_f->obst[obst_f->len-1].y_org) < 6) ) || laser->m_dist[i] < 20)
					{
						if(obst_f->len < (MAX_OBST_FIFO-1))
						{
							obst_f->obst[obst_f->len].x = x;
							obst_f->obst[obst_f->len].y = y;
							obst_f->obst[obst_f->len].x_org= x_obst;
							obst_f->obst[obst_f->len].y_org= y_obst;
							obst_f->dist[obst_f->len] = laser->m_dist[i];
							obst_f->deg[obst_f->len]  =laser->s_angle[i];
							obst_f->len++;
#if SCAN_DEBUG
						//	log_printf("get slen=%d,(%d,%d)",obst_f->len,x_obst,y_obst);
#endif
						}
					}
				}
				/*
				if(( disxy(x_obst,obst_f->obst[obst_f->len-1].x_org) < 10 && disxy(y_obst,obst_f->obst[obst_f->len-1].y_org) < 30) ||
					( disxy(x_obst,obst_f->obst[obst_f->len-1].x_org) < 30 && disxy(y_obst,obst_f->obst[obst_f->len-1].y_org) < 10))
				{
					obst_f->obst[obst_f->len].x = x;
					obst_f->obst[obst_f->len].y = y;
					obst_f->obst[obst_f->len].x_org= t_x_org;
					obst_f->obst[obst_f->len].y_org= t_y_org;
					obst_f->len++;

				}
				*/
			}

			t_count = 0;
			t_x_org = t_y_org = 0;
			//�洢��������
			x = tx ;
			y = ty ;
			//����������

		}
#if SCAN_DEBUG
	//log_printf("\r\n");
#endif
	}
	if(t_count)
	{
		t_x_org /= t_count;
		t_y_org /= t_count;

		if(obst_f->len < (MAX_OBST_FIFO-1))
		{
			obst_f->obst[obst_f->len].x = x;
			obst_f->obst[obst_f->len].y = y;
			obst_f->obst[obst_f->len].x_org= t_x_org;
			obst_f->obst[obst_f->len].y_org= t_y_org;
			obst_f->dist[obst_f->len] = laser->m_dist[i];
			obst_f->deg[obst_f->len]  =laser->s_angle[i];
			obst_f->len++;
#if SCAN_DEBUG
		log_printf("get len=%d,(%d,%d)",obst_f->len,t_x_org,t_y_org);
#endif
		}else
			log_printf("obst_f->len error\r\n");
					//��������
					//set_osbt_l(x,y,t_x_org ,t_y_org ,1,0,"scan");
	}


	/*
	//�������һ��
	if(obst_f->len >0 && obst_f->len < MAX_OBST_FIFO-1)
		if((tx != obst_f->obst[obst_f->len-1].x && obst_f->obst[obst_f->len-1].y != y ) &&
			(tx == obst_f->obst[obst_f->len-1].x || obst_f->obst[obst_f->len-1].y == y ) &&
			laser->m_dist[i] < MAX_LAST_DIST)
		{
			if(obst_f->len < (MAX_OBST_FIFO-1)  )
			{
				obst_f->obst[obst_f->len].x 	= x;
				obst_f->obst[obst_f->len].y 	= y;
				obst_f->obst[obst_f->len].x_org	= x_obst;
				obst_f->obst[obst_f->len].y_org	= y_obst;
				obst_f->dist[obst_f->len] 		= laser->m_dist[i];
				obst_f->deg[obst_f->len]  		= laser->s_angle[i];
				obst_f->len++;
			}
		}
	}*/

	//��X��һ���ߵģ���ʽ����һ����
	i=0;
	t_count = 4;
#if TEST_FOR_LABO
	t_count = 9;
#endif
	while(i <  obst_f->len)
	{
		//���ֺ�ߵĵ㣬���õ��X����������
		j=i+1;
		while(j < obst_f->len)
		{
			if(disxy(obst_f->obst[i].x_org,obst_f->obst[j].x_org) > 4)
				break;
			j++;
		}
		//��ʽ����һ����
		if((j-i) > 1)	//����ж������󣬾͸�ʽ����
		{
			for(k=i+1;k<j;k++)
			{
				obst_f->obst[k].x_org = obst_f->obst[i].x_org;
				obst_f->obst[k].x = obst_f->obst[i].x;
			}
			i = j;
		}else
			i++;
	}


	//��X��һ���ߵģ���ʽ����һ����
	i=0;
	t_count = 4;
#if TEST_FOR_LABO
	t_count = 9;
#endif
	while(i <  obst_f->len)
	{
		//���ֺ�ߵĵ㣬���õ��X����������
		j=i+1;
		while(j < obst_f->len)
		{



			if(disxy(obst_f->obst[i].y_org,obst_f->obst[j].y_org) > 4)
				break;
			j++;
		}
		//��ʽ����һ����
		if((j-i) > 1)	//����ж������󣬾͸�ʽ����
		{
			for(k=i+1;k<j;k++)
			{
				obst_f->obst[k].y_org = obst_f->obst[i].y_org;
				obst_f->obst[k].y = obst_f->obst[i].y;
			}
			i = j;
		}else
			i++;
	}
	/*
	�˲������˹µ�
	�ҵ������ĵ㣬����ĵ�����������һ�����ϵ�һ�㣬�����ȡ�����Ƕ̾���Ĺ����㣬Ҳֵ��������
	*/
	k = 0;
	for(i=0;i<obst_f->len;i++)
	{
		if(!(obst_f->obst[i].x == 0 && obst_f->obst[i].y==0))	//��ȥ���ˣ�������ʱ��ȥ���ˡ�
		{
			//һ�������ĵ�
			k=0;
			for(x = obst_f->obst[i].x -1;x <= obst_f->obst[i].x +1;x++)
			{
				for(y = obst_f->obst[i].y -1;y < obst_f->obst[i].y +1;y++)
				{
					if(in_obst_f(obst_f,x,y))
						k++;
				}
			}
			tx = ty = 0;
			if(k==1 && obst_f->dist[i] > MAX_LAST_DIST)	//�µ�
			{
				//Ѱ���ǲ���һ������������ġ�
				for(j=0;j<obst_f->len;j++)
				{
					if(obst_f->obst[j].x == obst_f->obst[i].x)
						tx++;
					if(obst_f->obst[j].y == obst_f->obst[i].y)
						ty++;
				}
				//�����㡣
				if(tx <=2 && ty <=2)
				{
					log_printf("iso(%d,%d,)\r\n",obst_f->obst[i].x,obst_f->obst[i].y);
					obst_f->obst[i].x = obst_f->obst[i].y  = 0;
				}
			}


		}

	}

#if TEST_FOR_LABO
		//ʵ���Ҳ��԰汾����ʽ����һ����


#endif
}
/*

����ɨ��������
�Ƚ���0��90��180��270�Ƕȣ�
����ϵ�ͼ
*/
int calc_scan_laser(navigat_t *navi,int save)
{
#if LASER_EN | LASER_DRAW
//	int x,y;//,tx,ty;
	int i;//,j;
//	OBST	*obst;
	LASER *laser = &navi->laser;
//	int x_org,y_org;
//	int t_x_org ,t_y_org ;
//	int t_count;
	//OBST obst_fifo[MAX_OBST_FIFO];		//���ڼ���һ��ֱ�ߵ�ʱ�������������⵼�µ��������⡣
	//int  w_fifo_p;
	struct obst_fifo_t *obst_f;		//�洢����ʱ����
//	int x_obst,y_obst;
	int m_x_org,m_y_org;//,m_x_org1,m_y_org1;//,m_x_org2,m_y_org2;		//ԭʼ��mm��λ������
//	OBST ost_storage[MAX_OBST_STORAGE];
//	int	 ost_len=0;//,coincide[0];	//�غ϶�
	int x_ok,y_ok;
	obst_f = (struct obst_fifo_t *)m_stack;
//	int c_obst=0;
//	int ret = 0,ret2;

//	int	x_adj,y_adj;		//X��Y��mm����Ľ���ֵ
//	int ex_dist0,ex_dist180,ex_dist90,ex_dist270;		// 4����λ�ľ���
	//OBST *m_obst[4];
//	int  x_trust=0,y_trust = 0;//���ζ�
	//��������Ӽ���
	www_idleintel_com();
	/*
	if(l_dist > MIN_LASER_DIST && l_dist <MAX_LASR_DIST)
	{
		for(i=0;i<3;i++)
		{
			laser->m_dist[laser->c_dist] = l_dist;
			laser->s_angle[laser->c_dist++] = sys->angle;
		}
	}
	*/

//	int dist0=0,dist90=0,dist270=0,dist180=0;
	if(laser->c_dist <=0 || laser->c_dist >=MAX_LASER_SCAN)
	{
		log_printf("[calc_scan_laser]len error=%d\r\n",laser->c_dist);
		return 0;
	}
	/***************************************************************************************************
		�ҳ�0�ȣ�90�ȣ�180�㣬270��ķ���ľ��룬����⼸��������һ���ߣ�Ŀǰ��3���㣬30�������ϣ�
		������ʵ�����е�������
		��������X,Y�ᡣ

	*****************************************************************************************************/
	//0�ȣ�90�ȣ�180�ȣ�270���ж�
	//get_angle_4(laser,&dist0,&dist90,&dist180,&dist270);


	//ajust_xy_by_laser(navi ,dist0,dist90,dist270,dist180,30,ost_storage,&ost_len);

	m_x_org = ORG_TO_MM(navi->x_org);
	m_y_org = ORG_TO_MM(navi->y_org);


//	m_x_org1 = m_x_org ;
//	m_y_org1 = m_y_org ;


	log_printf("[calc_laser]first scan...\r\n");
	scan_to_obst_map(laser ,obst_f,m_x_org,m_y_org);		//��ɨ�������ת����ϰ���������

#if !LASER_DRAW


	//check_ajust_sta(obst_f,&x_ok,&y_ok)	;					//�����ϰ����ߵ��غ϶�
	x_ok = y_ok = 0;
	if(ajust_xy_scan_obst(obst_f,&x_ok,&y_ok,&m_x_org,&m_y_org))
	{
		log_printf("[calc_laser] scan agin...\r\n");
		scan_to_obst_map(laser ,obst_f,m_x_org,m_y_org);		//�ٸ��ݽ����������ɨ��һ��
	}



	//ȡ��mm������
	m_x_org = ORG_TO_MM(navi->x_org);
	m_y_org = ORG_TO_MM(navi->y_org);
	if(laser_adj_handle(navi,obst_f,m_x_org,m_y_org))
	{
		m_x_org = ORG_TO_MM(navi->x_org);
		m_y_org = ORG_TO_MM(navi->y_org);
		x_ok = y_ok = 0;
		log_printf("adj_h ok,ajust again\r\n");
		scan_to_obst_map(laser ,obst_f,m_x_org,m_y_org);		//��ɨ�������ת����ϰ���������
		x_ok = y_ok = 0;
		if(ajust_xy_scan_obst(obst_f,&x_ok,&y_ok,&m_x_org,&m_y_org))
		{
			log_printf("[calc_laser] scan agin...\r\n");
			scan_to_obst_map(laser ,obst_f,m_x_org,m_y_org);		//�ٸ��ݽ����������ɨ��һ��
		}
	}
	//������������ݣ�������˳��ˡ�
	if(save ==0)
		return 0;
#endif
	m_x_org = ORG_TO_MM(navi->x_org);
	m_y_org = ORG_TO_MM(navi->y_org);
	//�жϣ���������
	for(i=0;i<obst_f->len;i++)
	{
	/*
			//�ĵ����ڵ�ͼ�ϲ����ϰ���
		x = obst_f->obst[i].x;
		x = obst_f->obst[i].y;
		if(OBST_XY(obst_f->obst[i].x,obst_f->obst[i].y)==0)
		{
			if(
		}
		*/
		//���԰汾��ǿ��һ��ֱ��
#if LASER_FORMART_LINE
		x = obst_f->obst[i].x;
		y = obst_f->obst[i].y;
		//�����ϰ���㣬��������ϰ����ֱ�ߣ�ȥ��
		if(OBST_XY(x,y) ==0)
		{
			if((OBST_XY(x-1,y) && OBST_XY(x-1,y+1) && OBST_XY(x-1,y-1) ) ||//һ����
				(OBST_XY(x+1,y) && OBST_XY(x+1,y+1) && OBST_XY(x+1,y-1) ) ||
				(OBST_XY(x-1,y-1) && OBST_XY(x,y-1) && OBST_XY(x+1,y-1) ) ||
				(OBST_XY(x-1,y+1) && OBST_XY(x,y+1) && OBST_XY(x+1,y+1) ))
			{
				log_printf("drop(%d,%d,)\r\n",x,y);
				obst_f->obst[i].x = obst_f->obst[i].y=0;
			}

		}
#endif
		if(!(obst_f->obst[i].x == 0 && obst_f->obst[i].y==0))	//��ȥ���ˣ�������ʱ��ȥ���ˡ�
		{

			log_printf("#(%d,%d,0.0,s)-[0,0,0]#%s\r\n",obst_f->obst[i].x,obst_f->obst[i].y);
			//set_osbt_l(m_x_org,m_y_org,obst_f->obst[i].x,obst_f->obst[i].y,obst_f->obst[i].x_org ,obst_f->obst[i].y_org ,1,0,obst_f->dist[i],obst_f->deg[i],"scan");

		}
	}

#endif
	return  0 ;



}

/*
���������������
��һ�����ȸ��ݹ¶���������������ʽŵ�
*/
int laser_adj_handle(navigat_t *navi,struct obst_fifo_t *obst_f,int m_x_org,int m_y_org)
{
	int i;
	int x,y;
	int c_obst=0,c_scan=0;
	int x1,y1;
	int m_x_org1,m_y_org1;
	//�����������ĵ㣬���Ҿ��벻Ҫ̫Զ�ġ�
	for(i=0;i<obst_f->len;i++)
	{
		//�̾���Ľ������̾����ڵĹµ���ܽ���
	//	if(obst_f->dist[i] > 20)
		//	continue;
		if(disxy(X_NOW,obst_f->obst[i].x) > 3 && disxy(Y_NOW,obst_f->obst[i].y) > 3)
			continue;
		if(!(obst_f->obst[i].x == 0 && obst_f->obst[i].y==0))	//��ȥ���ˣ�������ʱ��ȥ���ˡ�
		{
			//��һ��25������Ѱ�ң�����û�й����ĵ�
			c_obst = c_scan=0;
			x1  = y1 = 0;
			for(x = obst_f->obst[i].x -3;x <= obst_f->obst[i].x +3;x++)
			{
				for(y = obst_f->obst[i].y -3;y < obst_f->obst[i].y +3;y++)
				{

					if(OBST_XY(x,y))
					{
						//����Ǳ������ϰ�����ǹ����ĵ�
						if(x == (obst_f->obst[i].x -3) || x == (obst_f->obst[i].x +3  ) || y == (obst_f->obst[i].y -3) ||  y == (obst_f->obst[i].y +3))
						{
							//log_printf("outsid obst(%d,%d,)\r\n");
							c_obst = c_scan = 0;
							break;
						}
						if(c_obst ==0)
						{
							x1 = x;
							y1 = y;
						}else if(c_obst ==1)
						{
						//	x2 = x;
						//	y2 = y;
						}
						c_obst++;
					}
					if(in_obst_f(obst_f,x,y))
						c_scan++;

				}
			}
			if(c_obst >=1 && c_obst <=2 && c_scan >=1 && c_scan <=2)
			{
				OBST *obst;
				obst = get_obst(x1,y1);
				log_printf("[laser_adj]iso obst(%d,%d,%d,%d)-(%d,%d,%d,%d)\r\n",x1,y1,obst->x_org,obst->y_org,obst_f->obst[i].x,obst_f->obst[i].y,obst_f->obst[i].x_org,obst_f->obst[i].y_org);
				//���������
				if(disxy(obst->x_org,obst_f->obst[i].x_org) <=20 && disxy(obst->y_org,obst_f->obst[i].y_org) < 20)
				{
					//m_x_org1  = obst->x_org - (int)((float)(RADIUS_CM + obst_f->dist[i])   * cos(format_agle(obst_f->deg[i], ANGLE_360)*D2R));
					//m_y_org1  = obst->y_org - (int)((float)(RADIUS_CM + obst_f->dist[i])	* sin(format_agle(obst_f->deg[i], ANGLE_360)*D2R));
					int disty = obst_f->obst[i].y_org - obst->y_org;
					int distx = obst_f->obst[i].x_org - obst->x_org;
					uint8_t	type;

					if((distx >=15 || distx <= -15)  && (disty >=15 || disty <=-15))
						return 0;

					m_x_org1 = m_x_org - distx;
					m_y_org1 = m_y_org - disty;

					log_printf("[laser_adj]ajust old=(%d,%d)->(%d,%d)\r\n",m_x_org,m_y_org,m_x_org1,m_y_org1);
					if(distx == 0 && disty ==0)
						return 0;
					if(distx == 0)
						type = AJUST_Y;
					else if (disty == 0)
						type =AJUST_X;
					else
						type = AJUST_Y | AJUST_X;
					ajust_xy_org(m_x_org1* CM_PER_HW,m_y_org1 * CM_PER_HW ,90,type);
					return 1;
				}


			}


		}
	}
	return 0;
}


/*
�������
��X���ߣ�����Y���ߣ�


int laser_fitting_curves(navigat_t *navi,struct obst_fifo_t *obst_f,int m_x_org,int m_y_org)
{
	int x,y;
	int i,j,k;
	float c;
	int x_dir,y_dir;

	//�����غ���
	k=0;
	for(i=0;i<obst_f->len;i++)
	{
		if(OBST_XX(obst_f->obst.x,obst_f->obst.y))
			k++;
	}
	c = ((float)obst_f->len * 3.0f) / 5.0f;
	c +=0.5f;

	i = (int )c;
	log_printf("[fitting_curves]len=%d,c=%d,%d\r\n",obst_f->len,k,i);
	if(k >=i)
		return 0;

	if(
	//��X���ߣ�����������ϰ����¼
	for(i=0;i<5;i++)
	{
		for(
	}

}*/
//Ϊ�˱��ڸĶ�������ȥ��̫�࣬���⵼���汾��ֱ�����µĺ���
void set_osbt_l(int tx_org,int ty_org,int x,int y,int x_org,int y_org,uint8_t value,uint8_t type,int dist,float angle,char *str)
{
/*
	if(SCANE_XY(x, y)==1 && type ==0 && value && navigat->force_obst==0)
	{
		log_printf("not set osbt(%d,%d,)%s\r\n",x,y,str);
		return ;
	}
	*/
	set_compress_mem(x, y, value, navigat->obstacle);

	if(value)
	{
#if LASER_SCAN_TEST

		//ɨ����԰汾��ɨ���ͼʱ
		set_compress_mem(x, y, 0, navigat->scan);
#endif
		insert_osbt_l(x,y,x_org,y_org,tx_org,ty_org);
		navigat->osbt_x = x;
		navigat->osbt_y = y;
		log_printf("#(%d,%d,%3.1f,s)-[%d,%d,%d]#%s\r\n",x,y,angle,x_org,y_org,dist,str);
	}
}

/*

//Y����һ����
int xy_is_line_y(int x,int y,int *y_org)
{
	if(OBST_XY(x-1,y) && OBST_XY(x-2,y) && OBST_XY(x-3,y))
	{
		obst = get_obst(x-1,y);
		if(obst !=NULL)
		{
			*y_org = obst->x_org;
			return 1;
		}
	}
	if(OBST_XY(x+1,y) && OBST_XY(x+2,y) && OBST_XY(x+3,y))
	{
		obst = get_obst(x+1,y);
		if(obst !=NULL)
		{
			*y_org = obst->x_org;
			return 1;
		}
	}
	return 0;
}
int xy_is_line_x(int x,int y,int *x_org)
{
	OBST *obst;
	if(OBST_XY(x,y-1) && OBST_XY(x,y-2) && OBST_XY(x,y-3))
	{
		obst = get_obst(x,y-1);
		if(obst !=NULL)
		{
			*x_org = obst->x_org;
			return 1;
		}
	}
	if(OBST_XY(x,y+1) && OBST_XY(x,y+2) && OBST_XY(x,y+3))
	{
		obst = get_obst(x,y+1);
		if(obst !=NULL)
		{
			*x_org = obst->x_org;
			return 1;
		}
	}
	return 0;

}
//�ж������ĺϷ��ԣ������
int laser_format_obst_line(int *x,int *y,int *x_org,int *y_org)
{
	int x_g,y_g;
	//���ϰ��㣬���˳�
	if(OBST_XY(x,y))
		return 0;
	//��һ�����ϣ�Ҳ����ȷ�ģ��˳���
	if(xy_is_line_x(x,y,&x_g) || xy_is_line_y(x,y,&x_g) )
		return 0;
	//����Y����������һ��ֱ�ߣ����ʽ����ֱ��
	if(xy_is_line_y(x-1,y,&y_g))



}
*/
/*
��ʽ���ϰ����һ��ʵ�ߣ������ڷ����ڵĲ��ԡ�


void laser_format_obst_line(LASER *laser,struct obst_fifo_t *obst_f)
{
	 int i;
	 int x,y;
	 for(i=0;i<obst_f->len-3;i++)
	 {
	 		//Y��һ���ߣ����Ҹ�����һ���ϰ����ߣ�������ʽ���ɸ��ϰ���һ����X��
	 		x = obst_f->obst[i].x;
	 		y = obst_f->obst[i].y;
	 		if(OBST_XY(x,y)==0)  	//	�����ϰ���
	 		{

	 		}
			if(obst_f->obst[i].y == obst_f->obst[i+1].y &&  obst_f->obst[i].y == obst_f->obst[i+2].y)
			{
				if(OBST_XY(x,y)
			}
	 }
}*/
void laser_test(uint8_t dir,int pwm,int hw)
{
	//laser_turn_360(GO_LEFT);
	motor.c_left_hw = 0;
	log_printf("turn ok,go\r\n");
	motor_run(GO_FORWARD, GO_FORWARD_PWM, 0, 0);
	while(motor.c_left_hw< 300);
	motor_run(GO_STOP,0,0,0);
	log_printf("go ok,turn\r\n");
	while(1)
		laser_turn_360(GO_LEFT);



#if 0
//l_motor_turn_circle:
	float bagle = sys->angle;
//	uint32_t tmr;
	char flage = 1;
	int c=0,k=0;
	int count=0;
	int hw1=0,hw2=0,lhw1=0,lhw2=0;
	int pid_pwm;
//	int i;
	int cc=0;
	log_printf("[motor_turn_circle]hw=%d,dir =%d\r\n",hw,dir);
	/*
	for(i=0;i<10;i++)
	{
		illegal_copy_tell_13757122544_gsend(0x01,0,0);		//ǿ�Ƹ�λ�Ƕ�
		delay(5);
		www_idleintel_com();
		if(dis_float(sys->angle,0)<0.5f)
			break;
		log_printf("reset agnel error(%f)\r\n",sys->angle);
		delay_ms(2);

	}
	navigat->x_org = navigat->y_org = 0;
	*/
	navigat_init(1);
	sys->sState = SYS_NAVIGAT;
l_begin_laser:
	c= k = count = 0;
	begin_scan(navigat);
	motor_turn_dir(dir ,pwm,pwm);
	navigat->distance = 0;
	TIM5->CNT =0;
	motor.c_left_hw = motor.c_right_hw = 0;
//	tmr = msTmr;
	log_printf("laser test,org(%d,%d)\r\n",navigat->x_org,navigat->y_org);
	while(1)
	{
		laser_scan_dist(navigat,LASER_CALC_TURN);
		if(TIM5->CNT >=5000)
		{
			count++;

			TIM5->CNT=0;
			www_idleintel_com();
			if(c++>4)
			{
				c=0;

				hw1 = motor.c_left_hw - lhw1;
				hw2 = motor.c_right_hw- lhw2;

				lhw1 = motor.c_left_hw ;
				lhw2 = motor.c_right_hw;
				//motor.c_left_hw = motor.c_right_hw = 0;
				pid_pwm = (hw1 - hw2)*5;
				sys->left_pwm +=pid_pwm;
				if(sys->left_pwm < 500)
					sys->left_pwm = 500;
				if(sys->left_pwm > 700)
					sys->left_pwm = 700;
				//l_motor_set_pwm(sys->left_dir,sys->left_pwm);
				*left_pwm = sys->left_pwm;
				//log_printf("%3.1f,%d,%d,%d,%d\r\n",sys->angle,hw1, hw2,pid_pwm,sys->left_pwm);

			}
			if(sys->angle > 180 && flage)
			{
				k ++;
				flage = 0;
			}
			if(sys->angle < 10)
				flage = 1;
			/**/
			if(k >= hw & dis_float(bagle , sys->angle) < 5)
			{
				motor_run(GO_STOP,0,0,0);
				break;
			}

		}

	}
	calc_scan_laser(navigat);
	gyro_whlmap();
	log_printf("cc=%d\r\n",cc);
	/**/
	cc++;
	if(cc >3)
	{
		motor_run(GO_STOP,0,0,0);
		while(1);
	}

	motor_go_forwark(HW_GRID * 5,NO_SIDE_NEAR,NULL);
	goto l_begin_laser;

	//log_printf("count=%d,angle=%3.3f,hw=%d,tmr=%d\r\n",count,sys->angle,motor.c_left_hw,msTmr - tmr);
	//goto l_motor_turn_circle;
#endif
}



void laser_turn_360(uint8_t dir)
{

#if LASER_EN
	static uint8_t ddir=GO_LEFT;
	float bagle = sys->angle;
//	uint32_t tmr;
//	char flage = 1;
	int c=0;//,k=0;
	int count=0;
	int count1=0;
	int hw1=0,hw2=0,lhw1=0,lhw2=0;
	int pid_pwm;
	float angle1;

	ddir = ddir == GO_RIGTH ? GO_LEFT:GO_RIGTH;

	log_printf("[laser_turn_360]dir =%d\r\n",ddir);


	sys->sState = SYS_NAVIGAT;

	c= count = 0;
	angle1 = format_agle(sys->angle+40,ANGLE_360);
	//navigat->x_org = navigat->y_org = 0;
	log_printf("laser test,org(%d,%d),angle=%f,angle1=%f\r\n",navigat->x_org,navigat->y_org,sys->angle,angle1);
	begin_scan(navigat);
	coordinate_calcu(0);
	turn_round_pid(0);
	motor.c_left_hw = motor.c_right_hw = 0;
	motor_turn_dir(ddir ,LASER_PWM,LASER_PWM);
	navigat->distance = 0;
	TIM5->CNT =0;

//	tmr = msTmr;

	while(1)
	{
		laser_scan_dist(navigat,LASER_CALC_TURN);
		if(TIM5->CNT >=5000)
		{
			count++;


			TIM5->CNT=0;
			www_idleintel_com();
			turn_round_pid(TURN_SPEED);
			/*if(flage )
			{
				if(dis_float(bagle , sys->angle) > 20)
					flage = 0;
			}

			if(sys->angle > angle1  && flage == 0)
			{
				k ++;
				flage = 0;
			}
			*/
			/*if(sys->angle < 10)
				flage = 1;
			*/
			if(count1 >= 200 && dis_float(bagle , sys->angle) < 5)
			{
				motor_run(GO_STOP,0,0,0);
				break;
			}
		//	log_printf("%d,%3.1f,%3.1f,%3.1f\r\n",count1,bagle , sys->angle,dis_float(bagle , sys->angle));
			if(count1++ > 4000)
			{
				motor_run(GO_STOP,0,0,0);
				log_printf("turn timeout\r\n");
				break;
			}

		}

	}
	calc_scan_laser(navigat,1);
	gyro_whlmap();
	log_printf("[laser_turn_360]OK\r\n\r\n");
	//while(1);

	//log_printf("count=%d,angle=%3.3f,hw=%d,tmr=%d\r\n",count,sys->angle,motor.c_left_hw,msTmr - tmr);
	//goto l_motor_turn_circle;
#endif
}


/*
	�����ֱ�У�
	�ص�
	1)���ǰ��û�п�Ԥ֪���ϰ������һ�룬Ҫɨ��һȦ��ȷ���ߵ�·�ǶԵġ�
	2)���ǰ�����ϰ����ײ��ҪУ���Ƿ���ȷ
	3)�����;����ײ������ɨ�裬У���ͼ�������ͼ����ȷ�ģ��Ҽ���ɨ�費����ͼ���������ɨ������ͼ����ײ���ϰ��
*/
int laser_go_forward(void )
{
	int i;
	int dir;
	int x,y;
	float angle;

	angle = navigat->angle;

	//��������������Ƕȣ��������ֱ��
	if(navigat->angle !=0 && navigat->angle != 180)
	{
//		*bx = X_NOW;*by = Y_NOW;
		log_printf("[laser_go_forward]not 0 or 180,go forward only\r\n");
		motor_go_forwark(0,NO_SIDE_NEAR,navi_go_forwark_check);			//ֱ��
		return 0;
	}
l_laser_go_forward:
	dir = angle ==0?1:-1;
	//ɨ��ǰ����û���ϰ�����û�У���Ҫ��1����ɨ��
	x = X_NOW;
	y = Y_NOW;
	for(i=0;i<40;i++)
	{
		if(check_line(CHECK_Y_LINE,x,y,1))
		{
			log_printf("[laser_go_forward]found obst(%d,%d)\r\n",x,y);
			break;
		}
		x += dir;
	}
	//
	if(i>=40)
	{

		int dist = 100 * CM_PER_HW;
		log_printf("[laser_go_forward]not found fw obst go 1m\r\n");
		if(motor_go_forwark(dist,NO_SIDE_NEAR,navi_go_forwark_check))			//ֱ�гɹ�
		{
			log_printf("fw ok\r\n");
			laser_turn_360(GO_LEFT);		//תһȦ�ٽ���һ�µ�ͼ
		}else
		{
			log_printf("err ok\r\n");
			laser_turn_360(GO_LEFT);		//תһȦ�ٽ���һ�µ�ͼ
			return 0;
		}

		turn_to_deg(angle);
		goto l_laser_go_forward;
	}else
	{
		 log_printf("[laser_go_forward]not found fw obst(%d,%d,)\r\n",x,y);
		 motor_go_forwark(0,NO_SIDE_NEAR,navi_go_forwark_check);
	}
	return 0;
}
#define LOST_NUBUER   50
//�������߷����
int lasr_run_ajust_check(void)
{
//	int idx;
//	TOBST *t_obst;
	//�Ƕ�ƫ�ƹ���ʧ��

	//�ر���ʧ�������˳�����ʧ5�Σ�������һ�����ӣ����˳�
	if(navigat->adj_nsta == LEFT_SIDE_NEAR )		//���ر�
	{
		if(LEFT_ADC() < IRD_LEFT_ADJ_MIN)
		{

			if(navigat->adj_c_lost++ > LOST_NUBUER && WALK_DIST() >= HW_GRID)

			{

				motor_run(GO_STOP,0,0,0);
				if(LEFT_ADC() < SIDE_LOST_LEFT)
					navigat->adj_run_ret = ADJ_SIDE_LOST;	//�������߷���ȷ
				else
					navigat->adj_run_ret = ADJ_FAIL;
				log_printf("[run_ajust_check]lost12,ret=%d\r\n",navigat->adj_run_ret);
				return 0;
			}
		}else
			navigat->adj_c_lost = 0;
	}
	if(navigat->adj_nsta == RIGHT_SIDE_NEAR )		//���ر�
	{
		if(RIGHT_ADC() < IRD_RIGHT_ADJ_MIN)
		{
		//	log_printf("adj lost r:%d\r\n",RIGHT_ADC());
			if(navigat->adj_c_lost++ > LOST_NUBUER && WALK_DIST()  >= HW_GRID)
			{
				motor_run(GO_STOP,0,0,0);
				if(RIGHT_ADC() < SIDE_LOST_RIGHT)
					navigat->adj_run_ret = ADJ_SIDE_LOST;	//�������߷���ȷ
				else
					navigat->adj_run_ret = ADJ_FAIL;
				log_printf("[run_ajust_check]lost13,ret=%d\r\n",navigat->adj_run_ret);
				return 0;
			}
		}else
		  navigat->adj_c_lost = 0;
	}

	return 1;
}

//��ǽ��һ�Σ���������
/**********************************************************************************
 �ӱ���һ��
 *********************************************************************************/
void robot_lasr_near_run(uint8_t gSta)
{
	#if LASER_SIDE
//	int y_dir;
//	int next_deg=0;
	int i,dist;
	uint8_t sta;
//	uint8_t n_sta_back;		//������ʱ���ӱߵķ���
	int bx,by;
//	int c_max=4;

//	y_dir = navigat->side == LEFT_SIDE?1:-1;

	//���˶�κ󣬲���У�飬��������ȥ�ӱ�
	if(navigat->c_go < 17)
		return ;
	//�������ײ����
	if((sys->g_sta[2] < MIN_IRDA || sys->g_sta[3] < MIN_IRDA || sys->g_sta[4] < MIN_IRDA))
	{
		return ;
	}
	if(gSta & MASK_BUM_MIDL)
		return ;

	//������ӱ�
	log_printf("[lasr_near_run]turn left...\r\n");
	robot_turn_deg(GO_LEFT,650,WALL_LOST_DEG);
	navigat->adj_nsta  = 0;
	if(LEFT_ADC() > IRD_LEFT_ADJ_MIN)
	{
		//log_printf("[lasr_near_run]left adc error(%d)\r\n",LEFT_ADC() );
		//return ;
		navigat->adj_nsta = LEFT_SIDE_NEAR;			//����ӱ�
	}
	if(RIGHT_ADC() > IRD_RIGHT_ADJ_MIN)
	{
		//log_printf("[lasr_near_run]left adc error(%d)\r\n",LEFT_ADC() );
		//return ;
		navigat->adj_nsta = RIGHT_SIDE_NEAR;			//����ӱ�
	}
	if(navigat->adj_nsta ==0)
	{
		log_printf("[lasr_near_run]near side error,left=%d,right=%d\r\n",LEFT_ADC(),RIGHT_ADC());
		return ;
	}

	//
	STORT_XY();					//�洢��ǰ�ĵ㣬һ�����ʧ�ܵ�ʱ��Ҫ�ӱ߻�������Ҫ�õ�
	navigat->adj_walk_dis = navigat->adj_c_lost=0;

	sta = navigat->sta;
	navigat->adj_run =TRUE;					//��������߷�������¼�Ѿ���ɨ��
	navigat->adj_x_org = navigat->x_org_f;
	navigat->adj_c_lost = 0;
	by = navigat->y_org;
	for(i=0;i<4;i++)
	{

		log_printf("[lasr_near_run]near go,left idr=%d,right idr=%d,by =%d\r\n",LEFT_ADC(),RIGHT_ADC(),by);
		//===================================�˶�����������=============================
		navigat->angle = sys->angle;
		motor_go_forwark(0,(NO_SIDE_NEAR ), lasr_run_ajust_check);


		log_printf("after go fw,gsta=%d,ret=%d\r\n",sys->gsta,navigat->adj_run_ret);
		if(navigat->adj_nsta == LEFT_SIDE_NEAR)	//���ر�
		{
			robot_turn_deg(GO_LEFT,650,90);		//��ͷ180����
			robot_turn_deg(GO_LEFT,650,WALL_LOST_DEG);		//��ͷ180����
		}else
		{
			robot_turn_deg(GO_RIGTH,650,90);		//��ͷ180����
			robot_turn_deg(GO_RIGTH,650,WALL_LOST_DEG);		//��ͷ180����
		}
		if(LEFT_ADC() > IRD_LEFT_ADJ_MIN)
			navigat->adj_nsta = LEFT_SIDE_NEAR;			//����ӱ�

		if(RIGHT_ADC() > IRD_RIGHT_ADJ_MIN)
			navigat->adj_nsta = RIGHT_SIDE_NEAR;			//����ӱ�
		if(navigat->walk_dis >= 3000) //����3�ף����˳���
		{
			log_printf("log walk_dis,ok\r\n");
		}
		if(navigat->adj_nsta ==0)
		{
			log_printf("[lasr_near_run]near side error,left=%d,right=%d\r\n",LEFT_ADC(),RIGHT_ADC());
			return ;
		}
	}
	log_printf("walk ok,back now y =%d\r\n",navigat->y_org);
	if(disxy(by,navigat->y_org) < 400 )	//һ�����ڣ�����ȥ��
	{
		log_printf("OK1\r\n");
	//	while(1);
	}
	//�߻�ȥ��
	if(by < navigat->y_org)
	{
		navigat->angle  = 270;
		turn_to_deg(270);
		dist =   navigat->y_org - by;
	}else
	{
		navigat->angle  = 90;
		turn_to_deg(90);
		dist =   by - navigat->y_org ;
	}
	motor_go_forwark(dist,(NO_SIDE_NEAR ), NULL);
	log_printf("[lasr_near_run]run ok\r\n");
	navigat->c_go = 0;
#endif

}
/*
���˶��У����ݼ����������
*/
int laser_move_adj(navigat_t *navi,int sta)
{
#if 0 //LASER_EN
	LASER *laser = &navi->laser;


	//��һ�ν�������¼һ��
	if(sta==0)		//
	{
		laser->last_dist= 0;
		laser->last_timer = 0;
	}
	//��Ч�ľ���
	if(l_dist >0 && l_dist < MAX_LASR_DIST)
	{
		if(laser->last_dist == 0)				//�ϴξ���Ϊ0�������¼�¼
		{
			laser->last_dist 	= l_dist;			//��¼����
			laser->last_deg 	= sys->angle;
			laser->last_timer	= msTmr;
			laser->last_x_org	= navi->x_org;		//Բ�ĵľ���
			laser->last_y_org	= navi->y_org;
		}
		//�������ʱ�䵽�ˣ�����ľ���ı��ˣ������¼���
		if((msTmr - laser->last_timer) > 34 || laser->last_dist > l_dist)
		{
			if(dis_float(sys->angle,laser->last_deg) <= 1)		// 1�����ڣ�����Ϊֱ��,����
			{
				int x_org,y_org;
				//������ݼ���������ǰ���ľ���
				x_org = (int)((float)(laser->last_dist - l_dist) * cos(format_agle(sys->angle, ANGLE_360)*D2R));
				y_org = (int)((float)(laser->last_dist - l_dist) * sin(format_agle(sys->angle, ANGLE_360)*D2R));
				x_org =	laser->last_x_org + x_org *CM_PER_HW;
				y_org = laser->last_y_org + y_org *CM_PER_HW;
				log_printf("-(%d,%d)(%d,%d)\r\n",navi->x_org,navi->y_org,x_org,y_org);

			}
			laser->last_dist	= l_dist;			//��¼����
			laser->last_deg 	= sys->angle;
			laser->last_x_org	= navi->x_org;		//Բ�ĵľ���
			laser->last_y_org	= navi->y_org;
			laser->last_timer	= msTmr;				//ˢ��ʱ�䡣
		}

	}else
		laser->last_dist = 0;
#endif
	return 0;
}
/*
ǰ����ʱ�����ײ
gsta  - ��ײ��״̬
dir   - ����Ҫת�ĽǶ�,0,��ʾ��֪��ת�ĽǶȣ��ڵ�����ʱ�򣬾Ͳ�֪���������͵�ʱ�򣬾�֪��
deg	  - ת���ĽǶȡ�
*/

int laser_forward_bum(uint8_t gsta)
{
		return 0;
}

/*
ת����С�ļ��⴦
agle - ���ĽǶ�
d_min - ��С�ļ���ֵ

*/
void laser_turn_deg(uint8_t dir,int pwm,float agle,int d_min)
{
//	int pwm=600;
	//int dg_turn=deg - 2.5;
//	int c=0;
	float /*dis_agle,*/b_agle;
//	float out_deg;
	char sta = 0;		//ת�Ĵ���
	int dist;
	www_idleintel_com();
	log_printf("turn_charge,dir=%s,agle=%f\r\n",get_motor_sta(dir),agle);
	motor.c_left_hw = motor.c_right_hw = 0;

#if LASER_SIDE
	begin_scan(navigat);
#endif
	b_agle = sys->angle;
	coordinate_calcu(0);
	navigat->is_walk = 0;
	motor_run(dir,pwm,0,0);
	TIM5->CNT = 0;

	while(1)
	{
		get_sensers(&sys->gSta);
		//laser_calc_move(navigat,MAX_LASR_DIST);
		dist = laser_scan_dist(navigat,LASER_CALC_GO_FW);
		if(TIM5->CNT >=5000)
		{

			TIM5->CNT = 0;

			www_idleintel_com();
			//��һ��ת
			if(dis_float(b_agle ,sys->angle) >= agle && sta ==0 )
			{
				sta = 1;//ת��ȥ
				motor_run(GO_STOP,0,0,0);
				if(dir == GO_LEFT)
					motor_run(GO_LEFT,pwm,0,0);
				else
					motor_run(GO_RIGTH,pwm,0,0);
			}
			//����ת�����ˡ�
			if(dis_float(b_agle,sys->angle) < 1.0f && sta==1)
			{
				motor_run(GO_STOP,0,0,0);
				break;
			}

		}
		if(dist > 0 && dist < d_min)
		{
			log_printf("short dist=%d\r\n",dist);
			motor_run(GO_STOP,0,0,0);
			break;
		}
		//log_printf("%3.1f,%3.1f\r\n",sys->angle,dis_agle);
	}
	motor_run(GO_STOP,0,0,0);

	delay_ms_sensers(200);
	if(l_dist < d_min)
	{
		log_printf("last short dist=%d\r\n",l_dist);
	}
	calc_scan_laser(navigat,1);
	www_idleintel_com();
	gyro_whlmap();

	//if(l_dist < d_min)
	//	while(1);
	navigat->is_walk =1;
	//log_printf("..OK,c=%d,%d,agl=%3.1f\r\n",motor.c_left_hw,motor.c_right_hw,sys->angle);

}
/*
ת��Ѱ����Сֵ
*/
void laser_turn_for_v(uint8_t dir,int pwm,float agle)
{
//	int pwm=600;
	//int dg_turn=deg - 2.5;
//	int c=0;
	float /*dis_agle,*/b_agle = 0;
//	float out_deg;
	char sta = 0;		//ת�Ĵ���
	int dist;
	int	min_dist=300;
//	int c=0;//,k=0;
//	int count=0;
//	int count1=0;
	//int hw1=0,hw2=0,lhw1=0,lhw2=0;
	//int pid_pwm;

	b_agle = 0;
	www_idleintel_com();
	log_printf("turn_charge,dir=%s,agle=%f,bagnle=%f\r\n",get_motor_sta(dir),agle,b_agle);
	motor.c_left_hw = motor.c_right_hw = 0;

#if LASER_SIDE
	begin_scan(navigat);
#endif

	coordinate_calcu(0);
	navigat->is_walk = 0;
	//motor_run(dir,pwm,0,0);
	turn_round_pid(0);
	motor.c_left_hw = motor.c_right_hw = 0;
	motor_turn_dir(dir ,LASER_PWM,LASER_PWM);
	TIM5->CNT = 0;

	while(1)
	{
		get_sensers(&sys->gSta);
		//laser_calc_move(navigat,MAX_LASR_DIST);
		dist = laser_scan_dist(navigat,LASER_CALC_GO_FW);
		if(TIM5->CNT >=5000)
		{

			TIM5->CNT = 0;
			turn_round_pid(TURN_SPEED);

			www_idleintel_com();
			//��һ��ת
			if(dis_float(b_agle ,sys->angle) >= agle && sta ==0 )
			{
				sta = 1;//ת��ȥ
				motor_run(GO_STOP,0,0,0);
				//motor.c_left_hw = motor.c_right_hw = 0;
				if(dir == GO_LEFT)
					motor_turn_dir(GO_LEFT ,LASER_PWM,LASER_PWM);
					//motor_run(GO_LEFT,pwm,0,0);
				else
					motor_turn_dir(GO_RIGTH ,LASER_PWM,LASER_PWM);
					//motor_run(GO_RIGTH,pwm,0,0);

			}
			//����ת�����ˡ�
			if(dis_float(b_agle,sys->angle) <=1.5f && sta==1)
			{
				log_printf("turn back,%3.1f\r\n",sys->angle);
				motor_run(GO_STOP,0,0,0);
				break;
			}

		}
		if(dist > 0 && dist <= min_dist && sta==1)
		{
			log_printf("short dist=%d\r\n",dist);
			motor_run(GO_STOP,0,0,0);
			break;
		}
		//log_printf("%3.1f,%3.1f\r\n",sys->angle,dis_agle);
	}
	motor_run(GO_STOP,0,0,0);

	delay_ms_sensers(200);

	calc_scan_laser(navigat,1);
	www_idleintel_com();
	gyro_whlmap();

	//if(l_dist < d_min)
	//	while(1);
	navigat->is_walk =1;
	while(1);
	//log_printf("..OK,c=%d,%d,agl=%3.1f\r\n",motor.c_left_hw,motor.c_right_hw,sys->angle);

}



void laser_shake(uint8_t dir,int pwm,float agle)
{
//	int pwm=600;
	//int dg_turn=deg - 2.5;
//	int c=0;
	float /*dis_agle,*/b_agle;
//	float out_deg;
//	int midl=0;
//	int c=0;//,k=0;
//	int count=0;
//	int count1=0;
//	int hw1=0,hw2=0,lhw1=0,lhw2=0;
//	int pid_pwm;
	www_idleintel_com();
	log_printf("turn_charge,dir=%s,agle=%f\r\n",get_motor_sta(dir),agle);
	motor.c_left_hw = motor.c_right_hw = 0;


	b_agle = sys->angle;
	coordinate_calcu(0);
	turn_round_pid(0);
	navigat->is_walk = 0;
	motor_run(dir,pwm,0,0);
	TIM5->CNT = 0;

	while(1)
	{
		get_sensers(&sys->gSta);
		//laser_calc_move(navigat,MAX_LASR_DIST);
		laser_scan_dist(navigat,LASER_CALC_GO_FW);
		if(TIM5->CNT >=5000)
		{
			TIM5->CNT = 0;
			www_idleintel_com();
			turn_round_pid(TURN_SPEED);
			if(dis_float(sys->angle,b_agle) > agle)
			{
				break;
			}
		}
	}
	motor_run(GO_STOP,0,0,0);

	//delay_ms_sensers(50);

	//www_idleintel_com();
	//gyro_whlmap();

	navigat->is_walk =1;
	log_printf("..OK,c=%d,%d,agl=%3.1f\r\n",motor.c_left_hw,motor.c_right_hw,sys->angle);

}


/*

������ɨ���ͼ
*/
void laser_turn_scan(uint8_t dir,int pwm,float agle)
{
	begin_scan(navigat);

	laser_shake(GO_LEFT,pwm,agle);
	laser_shake(GO_RIGTH,pwm,agle*2);
	laser_shake(GO_LEFT,pwm,agle);
	delay_ms_sensers(200);

	www_idleintel_com();


	calc_scan_laser(navigat,1);
	gyro_whlmap();
	//log_printf("..OK,c=%d,%d,agl=%3.1f\r\n",motor.c_left_hw,motor.c_right_hw,sys->angle);

}
//Ѱ�ҳ��ϵ�
int laser_search_drop_point(int *xout,int *yout)
{

	return 0;

}

int laser_walk_draw_map(void)
{
//	uint8_t side = LEFT_SIDE;
//	int x,y;
	//ȫ���ó���ɨ
	memset(navigat->scan,0xFF,MAX_GRID_LIN);
	laser_turn_360(GO_LEFT);


	while(1)
	{
		get_sensers(&sys->gSta);			//����������
		proc_uart_task();
	//	for(x = X_NOW ;x > x_
		if(sys->sState != SYS_NAVIGAT)
			break;


	}

	motor_run(GO_STOP,0,0,0);
	return 0;
}
/*
��ʮ�ּ���
dist  - �����ٸ�һ��70�������ڽ���������7��
*/
int laser_in_coss_line(int x,int y,int dist)
{
	int xx,yy;
	int x_cross=0,y_cross=0;
	int i;
	//��X-������
	xx = x-1;
	for(i=0;i<dist-1;i++)
	{
		if(check_line(CHECK_Y_LINE,xx,y,2) == 1)
		{
			x_cross = 1;
			break;
		}
		xx--;
	}
	if(x_cross == 0)
	{
		//��X+������
		xx = x+1;
		for(i=0;i<dist-1;i++)
		{
			if(check_line(CHECK_Y_LINE,xx,y,2) == 1)
			{
				x_cross = 1;
				break;
			}
			xx++;
		}

	}
	yy = y - 1;
	for(i=0;i<dist-1;i++)
	{
		if(check_line(CHECK_X_LINE,x,yy,2) == 1)
		{
			y_cross = 1;
			break;
		}
		yy--;
	}
	if(y_cross == 0)
	{
		//��X+������
		yy = y + 1;
		for(i=0;i<dist-1;i++)
		{
			if(check_line(CHECK_X_LINE,x,yy,2) == 1)
			{
				y_cross = 1;
				break;
			}
			yy++;
		}

	}

	return (x_cross + (y_cross << 1));
}

/*
������ת����
���жϸ�����û�д�ֱ��ֱ�ߣ�����У���ȥ��ת������
���û�У��ٸ���������ȥ�ж�һ��ֱ�߽�����
type  0 -ֻ����ʮ�ּܣ�
	  1 - �Ҳ���ʮ�ּܣ�����ֱ��
*/
int laser_cross_adj(navigat_t *navi,char type)
{
	int x = X_NOW;
	int y = Y_NOW;
	int i;
	int ret;

	//��������Ƚϼ�������һ��У�飬��Y���ߡ�
	for(i=0;i<20;i++)
	{
		if(laser_in_coss_line(x,y,6) == 3)
		{
			goto l_laser_cross_adj;
		}
		y++;
	}
	x = X_NOW;
	y = Y_NOW;
	for(i=0;i<20;i++)
	{
		if(laser_in_coss_line(x,y,6) == 3)
		{
			goto l_laser_cross_adj;
		}
		y--;
	}
	return 0;
l_laser_cross_adj:
	ret = 1;
	log_printf("[laser_cross_adj]found (%d,%d,)\r\n",x,y);
	if(x != X_NOW || y != Y_NOW)
	{
		log_printf("walk to (%d,%d,)...\r\n",x,y);
		walk_map_cord_only(x,y,1000,10);
		ret = 2;
	}
	laser_turn_360(GO_RIGTH);
	return ret;

}


#if TEST_FOR_LABO
//���԰汾����ʵ������ӱ��߳���
int test_for_near_wall(int *lturn)
{
	log_printf("[test_for_near_wall],y_now=%d,min=%d\r\n",Y_NOW,navigat->min_ty);
	if(disxy(Y_NOW,100) > 18 &&  disxy(Y_NOW,navigat->min_ty) < 30 )
	{
		log_printf("min side...\r\n");
		if(disxy(X_NOW,navigat->min_tx) < 4|| disxy(X_NOW,navigat->max_tx) < 4 )
		{
			log_printf("side to point......\r\n");
			walk_map_cord_only(X_NOW,navigat->max_ty - 5,1000,10);
			log_printf("walk ok..........\r\n");
			laser_turn_360(GO_LEFT);
			*lturn = 1;
			return 1;
		}else //��Զ���������ߵ���Ե��
		{

			int ii,stta=0;
			int x = X_NOW;
			log_printf("not side ,go to side\r\n");
			for(ii=0;ii<20;ii++)
			{
				if(OBST_XY(x,Y_NOW))
					break;
				if((disxy(x,navigat->min_tx) < 4 || disxy(x,navigat->max_tx) < 4) )
				{
					log_printf("walk to side...1\r\n");
					turn_to_deg(0);		//
					stta= 1;
					break;
				}
				x++;
			}
			x = X_NOW;
			if(stta ==0)
			{
				for(ii=0;ii<20;ii++)
				{
					if(OBST_XY(x,Y_NOW))
						break;
					if((disxy(x,navigat->min_tx) < 4 || disxy(x,navigat->max_tx) < 4) )
					{
						log_printf("walk to side...2\r\n");
						turn_to_deg(180); 	//
						stta= 1;
						break;
					}
					x--;
				}
			}
			if(stta)
			{
				motor_go_forwark(0,NO_SIDE_NEAR,NULL);
				if((disxy(X_NOW,navigat->min_tx) < 4 || disxy(X_NOW,navigat->max_tx) < 4) )
				{
					log_printf("side to point......\r\n");
					walk_map_cord_only(X_NOW,navigat->max_ty - 5,1000,10);
					log_printf("walk ok..........\r\n");
					laser_turn_360(GO_LEFT);
					*lturn = 1;
					return 1;
				}
			}
		}
	}
	return 0;
}
#endif
//===============================================����Ϊ�Ǽ���汾�ļ������============================


void laser2obst(SIDEOBST *mobst)
{

	LASER *laser;
	int16_t dist,i,j,c;
	int16_t x_obst,y_obst;
	int 	x_org,y_org;


	log_printf("[laser2obst]len=%d\r\n",laser->c_dist);
	laser = &navigat->laser;
	for(i=0;i<laser->c_dist ;i++)
	{
		dist = laser->m_dist[i] * CM_PER_HW+10;
		dist2obst(dist,laser->s_angle[i],navigat->x_org,navigat->y_org,&x_obst,&y_obst,&x_org,&y_org);
		insert_side_obst(mobst,x_obst,y_obst,laser->m_dist[i]);
		//log_printf("+(%d,%d,)\r\n",x_obst,y_obst);
	}
	for(i = 0;i< mobst->len;i++)
	{
		c=0;
		for(j=0;j<mobst->len;j++)
		{
			if(mobst->obst[i].x == mobst->obst[j].x && mobst->obst[i].y == mobst->obst[j].y)
				c++;
		}
		if(c >=3)
			log_printf("#(%d,%d,0.0,s)-[0]#last\r\n",mobst->obst[i].x,mobst->obst[i].y);
	}

}

/*

��ת���⣬���ڽ�������
*/
void laser_turn_for_adjust(uint8_t dir)
{

	static uint8_t ddir=GO_LEFT;
	float bagle = sys->angle;
//	uint32_t tmr;
//	char flage = 1;
//	int c=0;//,k=0;
	int count=0;
	int count1=0;
	//int hw1=0,hw2=0,lhw1=0,lhw2=0;
	//int pid_pwm;
	uint16_t k=0;
//	float angle1;
	SIDEOBST *mobst;

	ddir = ddir == GO_RIGTH ? GO_LEFT:GO_RIGTH;

	log_printf("[laser_for_adj]dir =%d\r\n",ddir);

	sys->sState = SYS_NAVIGAT;

 	count = 0;
	mobst = (SIDEOBST*)m_stack;
//	angle1 = format_agle(sys->angle+40,ANGLE_360);

	begin_scan(navigat);
	coordinate_calcu(0);
	gyro_whlmap();
	turn_round_pid(0);
	motor.c_left_hw = motor.c_right_hw = 0;
	motor_turn_dir(ddir ,LASER_PWM,LASER_PWM);
	navigat->distance = 0;
	TIM5->CNT =0;

	while(1)
	{
		if(navigat->laser.c_dist >=MAX_LASER_SCAN)
		{
			log_printf("c_dist error=%d\r\n",navigat->laser.c_dist);
			break;
		}
		laser_scan_dist(navigat,LASER_CALC_TURN);
		if(TIM5->CNT >=5000)
		{
			count++;

			TIM5->CNT=0;
			www_idleintel_com();
			turn_round_pid(TURN_SPEED);

			if(count1 >= 200 && dis_float(bagle , sys->angle) < 5 )
			{
				if(k>0) //k>=0 ->k > 0
				{
					motor_run(GO_STOP,0,0,0);
					break;
				}
				count1 = 0;
				k++;
			}
		//	log_printf("%d,%3.1f,%3.1f,%3.1f\r\n",count1,bagle , sys->angle,dis_float(bagle , sys->angle));
			if(count1++ > 8000 )
			{
				motor_run(GO_STOP,0,0,0);
				log_printf("turn timeout\r\n");
				break;
			}
		}
	}
	log_printf("turn ok,c=%d\r\n",navigat->laser.c_dist);
	//===========================================������任���ϰ�������
	laser2obst(mobst);
	cord_calc_store(0);
	gyro_whlmap();
	log_printf("[laser_turn_360]OK\r\n\r\n");
}

/*
int ir_scan(navigat_t *navi)
{
//		int dist;
		int ldist;


//		dist = MAX_LASR_DIST;

		//�⵽����

		{

			LASER *laser = &navi->laser;
			ldist = l_dist;
			//�����׼��������
			if(laser->c_scan == 0)
			{
				laser->c_scan++;
				l_dist =0;
				//log_printf("break out dis=%d\r\n",l_dist);
				return ldist;
			}

			//����ǶȺ;���
			//log_printf("laser->m_dist[%d]=%d;laser->s_angle[%d]=%3.5ff\r\n",laser->c_dist,l_dist,laser->c_dist,sys->angle);
			laser->m_dist[laser->c_dist] = l_dist;
			laser->s_angle[laser->c_dist++] = sys->angle;
	 	}

}
*/
