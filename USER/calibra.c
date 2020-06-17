#include "sys.h"



/*
У׼��ͼ

*/

//�м���û���ϰ��û��δ��ɨ������Ч��һ����
uint8_t line_valid(int16_t bx,int16_t by,int16_t ex)
{
	int16_t bx1,ex1,x,c=0;
	
	bx1 = bx < ex?bx:ex;
	ex1 = bx < ex?ex:bx;	
	
	//�м����ϰ����δ��ɨ�ģ�������Ч�ġ�
	/*
	������ʱ�������¶��˵�����������δ��ɨ�����ϰ������û�н�����
	���ŰѶ��˵��ж�ȥ��������
	*/
	for(x = bx1+1; x<ex1;x++)
	{
		if(OBST_XY(x,by) )
		{
			log_printf("line_valid:(%d,%d,)\r\n",x,by);
		//	if(x == bx1 || x == ex1
			return 0;
		}
		if(clean_sta(x,by)==0)
		{
			log_printf("line_valid_no_clean:(%d,%d,)\r\n",x,by);
			c++;
		}
		if(c >=2)
			return 0;
	}
	return 1;
}

uint8_t ajust_ofset_y(int16_t x,int16_t y,int16_t bx1,int16_t ex1,uint8_t check_len,int16_t walk_len,int16_t x_dir)
{
	if(OBST_XY(x,y) )			//���ϰ���
	{
		
		log_printf("[ajust_ofset_y]found obst=(%d,%d,),(%d,%d,)->(%d,%d,),check=%d,len=%d,dir=%d\r\n",x,y,bx1,y,ex1,y
					,check_len,walk_len,x_dir);
		if(line_valid(bx1,y,ex1)==0) //�м�û���ϰ���
		{
			log_printf("not vline\r\n");
			return 0;
		}
		log_printf("line ok");
		if(check_len)		//ҪУ�鳤��
		{
			int16_t len;
			len = get_linex_len(x-x_dir,y);
			log_printf("check len(%d,%d,)len=%d,walk=%d\r\n",x-x_dir,y,len,walk_len);
			if(dis_xy(len,walk_len) <=2)
			{
				log_printf("len ok ,ajust\r\n");
				ajust_xy_map(X_NOW,y,AJUST_Y);
				return 1;
			}
		}else
		{
			ajust_xy_map(X_NOW,y,AJUST_Y);
			return 1;
		}

	}
	return 0;

}
/*
�ƶ�X�ῴ�Ƿ����
һ��Ҫ
*/
uint8_t ajust_ofset_x(int16_t bx,int16_t by,int16_t ex,int16_t x_ofs,int16_t walk_len)
{
	int16_t bx1,ex1,len;
	//�����ƫ����
	bx1 = bx+x_ofs;
	ex1 = ex+x_ofs;
	log_printf("[ajust_ofset_x](%d,%d,)-(%d,%d,)x_ofs=%d,wlen=%d,ofs=(%d,%d,)\r\n",bx,by,ex,by,x_ofs,walk_len,bx1,by);
	//�����ϰ�������м���δ��ɨ�ģ��Ƿ����ߡ�
	if(line_valid(bx1,by,ex1)==0)
	{
		log_printf("line error\r\n");
		return 0;
	}
	//��������Ǻϣ���OK
	if(OBST_XY(bx1-1,by) && OBST_XY(ex1+1,by))
	{
		log_printf("obstOK\r\n");
		return 1;
	}
	//���˶����ϰ������Ҫ�����Ǻ�
	len = get_linex_len(bx1,by);
	log_printf("peerlen=%d,my=%d\r\n",len,walk_len);
	if(walk_len > 0 && dis_xy(walk_len,len) >=2)
	{
		log_printf("len error\r\n");
		return 0;
	}
	log_printf("OK,AJUST..X\r\n");
	ajust_xy_map(X_NOW+x_ofs,Y_NOW,AJUST_X);
	return 1;
	
}
//��⴩���ϰ����״̬
int16_t  check_cross_obst_sta(int16_t bx,int16_t ex,int16_t yy,int16_t x_dir,int16_t *ox,int16_t *oy)
{
	int16_t g_cross = 0;
	int16_t i = 0,x_cros,c_clr=0;
	/*
	while(1)
	{
		
		if(x_cros == ex)					//��ͷ��
			break;
		if(SCANE_XY(x_cros,yy))		//�Ѿ���ɨ���ĸ���
			c_clr++;

		
		if(i++>=50)
		{
			log_printf("i error\r\n");
			break;
		}
		x_cros+=x_dir;
	}
	*/
	log_printf("[cross_obst_sta](%d,%d,)->(%d,%d,)xdir=%d\r\n",bx,yy,ex,yy,x_dir);
	x_cros = bx + x_dir;
	while(1)
	{
		if(x_cros == ex)					//��ͷ��
			break;
		if(SCANE_XY(x_cros,yy))		//�Ѿ���ɨ���ĸ���
			c_clr++;

		if(OBST_XY(x_cros,yy))		//�ϰ����
		{
			log_printf("cross obst(%d,%d,)\r\n",x_cros,yy);
			g_cross = 1;
			//�ж���������Խ�ġ����ǰ��������ɨ�����Ҳ��Ǵ���һ����
			if(obst_line_90_270_light(x_cros,yy,1,1)) //������һ����
			{	
				log_printf("cross line\r\n");
				g_cross = 2;
			}
			*ox = x_cros;
			*oy = yy;
			break;
		}
		
		if(i++>=50)
		{
			log_printf("i error\r\n");
			break;
		}
		x_cros+=x_dir;
	}

	if(c_clr <=2 && g_cross == 0)
	{
		log_printf("few clean,not cross,clr=%d\r\n",c_clr);
		return -1;
	}
	return g_cross;
	/*
	//�����ϰ�������ǲ��ǵ���δ��ɨ������
	if(g_cross)
	{
		x_cros+=x_dir;
		c = i= 0;
		while(1)
		{
			if(x_cros == ex)					//��ͷ��
				break;
			if(clean_sta(x_cros,yy) ==0)
				c++;
			if(i++>=10)
			{
				log_printf("i2 error\r\n");
				break;
			}
				
		}
		if( c>=3 && clean_sta(x_cros,yy) ==0 && c_clr >3)
		{
			log_printf("cross to unclr\r\n");
			g_cross = 3;
		}
	}
	
	return g_cross;*/
}


//�������ߵ��ϰ������ж�����ȥ�Ǳ���ɨ���Ǳ�����ɨ����ϰ������������ȥ�Ǳ�
int get_navi_deg(int16_t tx,int16_t ty,int16_t *ox)
{
	int16_t x1,x2,i;
	x1 =tx-1;
	x2 =tx+1;

	for(i=0;i<10;i++)
	{
		if(point_sta(x1,ty) != UNCLN_STA)
		{
			log_printf("found obstdir-(%d,%d,)\r\n",x1,ty);
			*ox = x1-1;		//�������һ��	
			return 180;
		}
		if(point_sta(x2,ty) != UNCLN_STA)
		{
			log_printf("found obstdir+(%d,%d,)\r\n",x1,ty);
			*ox = x2+1;		//�������һ��	
			return 0;

		}
		x1--;
		x2++;
	}
	*ox = tx+3;
	return 0;
	
}

/*
������δ��ɨ����߷�����һ�£�����һ��λ��
*/
int unclr_piont_walk(uint8_t old_side)
{
	char  ret;
	int16_t y_dir = navigat->side == LEFT_SIDE?1:-1;
	int16_t ox;
	int deg;
	uint8_t side = 0;


	deg =get_navi_deg(X_NOW,Y_NOW+y_dir,&ox);
	log_printf("[unclr_piont_walk]angle=%d,side=%d\r\n",deg,navigat->side);
#if !FORCE_SIDE_NAVI
	//У�鷽��
	uint8_t sta_left,sta_right;
	sta_left = point_sta(X_NOW,Y_NOW+1);
	sta_right = point_sta(X_NOW,Y_NOW-1);
	if(sta_left == UNCLN_STA && sta_right !=UNCLN_STA)
		side = LEFT_SIDE;
	else if(sta_left != UNCLN_STA && sta_right == UNCLN_STA)
		side = RIGHT_SIDE;
	log_printf("sta=%d,%d,side=%d\r\n",sta_left,sta_right,side);
	if(side != 0)
	{
		navigat->side = side;
		log_printf("set side =%d\r\n",navigat->side);
		
	}
		
#endif
	
	//����������
	//if(X_NOW == tox && Y_NOW == toy)
	{
		//У�鷽��
		if(navigat->side == LEFT_SIDE)
		{
			ret = walk_map_cord(navigat->tx,navigat->ty+2,NO_ANGLE_V,0);
			motor_go_forwark(100,NO_SIDE_NEAR,NULL);
		}
		else
		{
			ret =walk_map_cord(navigat->tx,navigat->ty-2,NO_ANGLE_V,0);
			//�ӳ�����������Ҹ�ת���ұߣ����߰�񣬱���װ�ϳ��׮
			if(old_side == LEFT_SIDE && navigat->from_charge && dis_xy(Y_NOW,navigat->dock_y) < 3)
			{
				motor_go_forwark(200,NO_SIDE_NEAR,NULL);
			}
		}

		
	}
	log_printf("[unclr_piont_walk]walk ret=%d,gsta=%d\r\n",ret,sys->gsta);
	//�߽�ȥ�ˣ���ͼ����
	if(ret >0)
	{

	}
/*
	//��ײ�ˣ��ӱ߽�ȥһ��
	if(ret ==0)
	{
		ret = nearwall_cross_map(navigat->y_org+400*y_dir,angle,0,20);	//�ӱ�
	}
	*/
	navigat->m_navi_run = 2;	//������λ��һ�����У���ҪУ׼��
	log_printf("[unclr_piont_walk]turn to=%d,and walk to(%d,%d,)\r\n",deg,ox,Y_NOW);
	turn_to_deg(deg);
	//��ͷ�߼�����Ȼ���ͷ�����������ߵĻ����ᵼ��������Խ��Խ���ĩ��δ��ɨ�㡣
	walk_map_cord(ox,Y_NOW,NO_ANGLE_V,0);
	log_printf("[unclr_piont_walk]walk ok(%d,%d,)turn back\r\n",X_NOW,Y_NOW);
	//��ͷ�߻�ȥ
	if(deg == 0)
		turn_to_deg(180);
	else
		turn_to_deg(0);

	return 1;
}
/*
У׼��ͼ
0 - ����
1 - ���ˡ�
*/
int16_t calibrat_map(int16_t tx,int16_t ty)
{
	int16_t ny=0,nx=0;

	int16_t xret=0;
	int16_t xx1,xx2 ,yy1,yy2,i,ox,oy;
//	int16_t x1=0;
	uint8_t adj = 0;
	uint8_t dir1=1,dir2=1;
	int16_t ret=0;
	//=========================================��һ��Y����ߣ�������X��
	
	xx1 = tx;
	yy1 = yy2 = ty;
	ny = 0;
	dir1=dir2=1;
	log_printf("[calibrat_map](%d,%d,)\r\n",tx,ty);
	//�ҵ�����һ�ӱ߹�ȥ�ĵ㡣
	for(i = 0;i<8;i++)
	{
		yy2--;
		//=============================����������==============================
		if(OBST_XY(xx1+1,yy1) || OBST_XY(xx1-1,yy1) || OBST_XY(xx1,yy1))		//�����߲�������
		{
			dir1 = 0;										//���������ߣ���������
		}
		if(dir1)
		{
			xret =obst_line_on_90_270(xx1,yy1,12,&ox,&oy);
			if(xret ==2 || xret==-2)   //�����һ���ϸ���ߣ���ֱ���˳��ˡ�
			{
				ny = yy1;
				log_printf("found liney=(%d,%d,)\r\n",xx1,yy1);
				break;
			}
			
		}
		
		//================================����������====================================
		if(OBST_XY(xx1+1,yy2) || OBST_XY(xx1-1,yy2)||OBST_XY(xx1,yy2))		//�����߲�������
		{
			dir2 = 0;										//���������ߣ���������
		}
		if( dir2)
		{
			xret =obst_line_on_90_270(xx1,yy2,12,&ox,&oy);
			if(xret == 2 || xret == -2)   //�����һ���ϸ���ߣ���ֱ���˳��ˡ�
			{
				ny = yy2;
				log_printf("found liney2=(%d,%d,)\r\n",xx1,yy2);
				break;
			}		
			
		}
		
		yy1 ++;
	}
	//===��һ��ֱ�ߣ����߹�ȥ==========
	if(ny > 0)
	{
		adj = walk_adj_x(tx,ny,xret,ox,oy);
	
		if(adj == AJUST_X)		//ֻ����X�ᣬ�򿴿��ܷ����Y��
		{
			navigat->c_v_go = 0;
			//if(ajust_y_from_x(X_NOW,Y_NOW) == AJUST_Y)  ���������������з���
 			//	return 1;
 			ret = 1;
		}
		if(adj >0)
			ret = 1;
		else
			ret = 0;
		/*
		if(adj == (AJUST_X | AJUST_Y))
		{
			log_printf("[calibrat_map]ajust xy ok1\r\n");
			return 1;
		}
		*/
	}

		
	//=====================����һ��X����ߣ���������Y�ᡣ=================================
	log_printf("[calibrat_map]Analy X\r\n");
	xx2 = xx1 = tx;
	yy1 = ty;
	nx = 0;
	//�ҵ�����һ�ӱ߹�ȥ�ĵ㡣
	for(i = 0;i<8;i++)
	{
		xx2--;

		//=============================����������==============================
		if(OBST_XY(xx1,yy1+1) || OBST_XY(xx1,yy1-1) || OBST_XY(xx1,yy1) || clean_sta(xx1,yy1) ==0)		//�����߲�������
		{
			dir1 = 0;										//���������ߣ���������
		}
		if( dir1)
		{
			xret =obst_line_on_0_180(xx1,yy1,12,&ox,&oy);
			if(xret ==2 || xret==-2)   //�����һ���ϸ���ߣ���ֱ���˳��ˡ�
			{
				nx = xx1;
				log_printf("found linex=(%d,%d,)\r\n",xx1,yy1);
				break;
			}
		}
		
		//================================����������====================================
		if(OBST_XY(xx2,yy1+1) || OBST_XY(xx2,yy1-1) || OBST_XY(xx2,yy1) || clean_sta(xx2,yy1) ==0)		//�����߲�������
		{
			dir2 = 0;										//���������ߣ���������
		}
		if( dir2)
		{
			xret =obst_line_on_0_180(xx2,yy1,12,&ox,&oy);
			if(xret == 2 || xret == -2)   //�����һ���ϸ���ߣ���ֱ���˳��ˡ�
			{
				nx = xx2;
				log_printf("found linex2=(%d,%d,)\r\n",xx2,yy1);
				break;
			}	
		}
		
		xx1 ++;
	}
	//===��һ��ֱ�ߣ����߹�ȥ==========
	if(nx > 0)
	{
		adj = walk_adj_y(nx,ty,xret,ox,oy);
		if(adj > 0)
			return 1;
		else
			return 0;

	}
	return ret;
}
#define MAX_WAL_HW_ADJ		(10*HW_GRID)
/*
�ߵ���λ��Ȼ�����һ��ֱ��ȥ����X��
*/
uint8_t walk_adj_y(int16_t tox,int16_t toy,int16_t dir,int16_t ox,int16_t oy)
{
	char ret;
	int16_t y_dir=0;
	int bdeg;		//�����ĽǶȣ�������ȥ��û����ײ����������ǶȻ�����
	log_printf("[walk_adj_y](%d,%d,)->(%d,%d,%d),(%d,%d,)\r\n",X_NOW,Y_NOW,tox,toy,dir,ox,oy);
	//ֻ��ֱ�ߣ����߹�ȥ��
	if(dir !=2 && dir !=-2)
	{
		log_printf("[walk_adj_y]err dir(%d)\r\n",dir);
		return 0;
	}
	//Y�᲻��
	if(toy != Y_NOW)
	{
		log_printf("[walk_adj_y]toy error\r\n");
		return 0;
	}
	//========��һ�������Ǳ��㣬��Ҫ�ƶ�һ�£����� walk_map_cord_only �ߵ��㡣==============
	if(tox != X_NOW )
	{
		ret = walk_map_cord_only(tox,toy,1000,10);
		//��ʵ��ʧ���ˣ��ǲ��ǿ���Ҳ����һ��?
		if(ret ==0)
		{
			log_printf("[walk_adj_y]walk bum error\r\n");
			return AJUST_MOVE;
		}
	}

	//Y+����
	if(dir ==2)
	{
		y_dir = -1;
		log_printf("[walk_adj_y]turn to 90\r\n");
		turn_to_deg(90);
		bdeg = 270;
	}else
	{
		y_dir = 1;
		log_printf("[walk_adj_y]turn to 270\r\n");
		turn_to_deg(270);
		bdeg = 90;
	}
	//=========== �ڶ�������ֱ�߳��ȥ==================
	ret = motor_go_forwark_only(MAX_WAL_HW_ADJ,NO_SIDE_NEAR);
	//��ײ�ˣ���������
	if(ret ==0 && sys->gsta !=0)
	{
//		uint8_t c_ret=0;
		log_printf("[walk_adj_y]bum ajd x\r\n");
		//����������ӽ���X�ᣬ��ʵ������������ģ�
		if(IR_MIDLE_BUM())		//�м���ײ���������
		{
			int16_t y;
			y = oy + y_dir;
			log_printf("[walk_adj_y]mid bum ajust y\r\n");
			ajust_xy_map(X_NOW,y,AJUST_Y);
			navigat->c_v_go = 0;
		}else		//�����м����ײ���򲻽�������ʵ����ʱ��Ҳ���Խ����ġ���߿��ǽ�һ�����Ż�
		{
			log_printf("[walk_adj_y]not mid bum(%d,%d,%d)\r\n",sys->g_sta[2],sys->g_sta[3],sys->g_sta[4]);
			return AJUST_MOVE;
		}
		return AJUST_Y;
		/*
		//�ж��ǲ���T���Σ���һ����ͷ����
		c_ret = check_line_x_sta(ox,oy,5,dir);
		log_printf("[walk_adj_y]ret=%d\r\n",c_ret);
		
		if(c_ret == LINE_X_STA_T_LEFT || c_ret == LINE_X_STA_L_LEFT)
		{
			turn_to_deg_v(180);
		}else if(c_ret == LINE_X_STA_L_RIGHT || c_ret == LINE_X_STA_T_RIGHT)
		{
			turn_to_deg_v(0);
		}else
		{
			log_printf("cret error\r\n");
			return AJUST_Y;
		}	
		*/
	}else
	{
		log_printf("[walk_adj_y]not bum turn to %d and back\r\n",bdeg);
		turn_to_deg(bdeg);
		motor_go_forwark_only(MAX_WAL_HW_ADJ,NO_SIDE_NEAR);
	}
	return AJUST_MOVE;
}

uint8_t walk_adj_x(int16_t tox,int16_t toy,int16_t dir,int16_t ox,int16_t oy)
{
	char ret;
	int16_t x_dir=0;
//	int bdeg;

	log_printf("[walk_adj_x](%d,%d,)->(%d,%d,%d),(%d,%d,)\r\n",X_NOW,Y_NOW,tox,toy,dir,ox,oy);
	//ֻ��ֱ�ߣ����߹�ȥ��
	if(dir !=2 && dir !=-2)
	{
		log_printf("[walk_adj_x]err dir(%d)\r\n",dir);
		return 0;
	}
	//Y�᲻��
	if(tox != X_NOW)
	{
		log_printf("[walk_adj_x]tox error\r\n");
		return 0;
	}
	//========��һ�������Ǳ��㣬��Ҫ�ƶ�һ�£����� walk_map_cord_only �ߵ��㡣==============
	if(toy != Y_NOW )
	{
		ret = walk_map_cord_only(tox,toy,1000,10);
		//��ʵ��ʧ���ˣ��ǲ��ǿ���Ҳ����һ��?
		if(ret ==0)
		{
			log_printf("[walk_adj_x]walk bum error\r\n");
			return AJUST_MOVE;		//�ƶ���
		}
	}

	//Y+����
	if(dir ==2)
	{
		x_dir = -1;
		log_printf("[walk_adj_x]turn to 90\r\n");
		turn_to_deg(0);
//		bdeg = 180;
	}else
	{
		x_dir = 1;
		log_printf("[walk_adj_x]turn to 270\r\n");
		turn_to_deg(180);
	//	bdeg =  0;
	}
	//=========== �ڶ�������ֱ�߳��ȥ==================
	ret = motor_go_forwark_only(MAX_WAL_HW_ADJ,NO_SIDE_NEAR);
	//��ײ�ˣ���������
	if(ret ==0 && sys->gsta !=0)
	{
//		uint8_t c_ret=0;
		log_printf("[walk_adj_x]bum ajd x\r\n");
		//����������ӽ���X�ᣬ��ʵ������������ģ�
		if(IR_MIDLE_BUM())		//�м���ײ���������
		{
			int16_t x;
			x = ox + x_dir;
			log_printf("[walk_adj_x]mid bum ajust X(%d,%d,)->(%d,%d,)\r\n",X_NOW,Y_NOW,x,Y_NOW);
			if(dis_xy(x,X_NOW) <=3)
			{
				ajust_xy_map(x,Y_NOW,AJUST_X);
				navigat->c_v_go = 0;
			}else
			{
				log_printf("big dist no adjX \r\n");
				
			}
		}else		//�����м����ײ���򲻽�������ʵ����ʱ��Ҳ���Խ����ġ���߿��ǽ�һ�����Ż�
		{
			log_printf("[walk_adj_x]not mid bum(%d,%d,%d)\r\n",sys->g_sta[2],sys->g_sta[3],sys->g_sta[4]);
			return AJUST_MOVE;
		}
		return AJUST_X;
		/*
		//�ж��ǲ���T���Σ���һ����ͷ����
		c_ret = check_line_x_sta(ox,oy,5,dir);
		log_printf("[walk_adj_x]ret=%d\r\n",c_ret);
		
		if(c_ret == LINE_X_STA_T_LEFT || c_ret == LINE_X_STA_L_LEFT)
		{
			turn_to_deg_v(180);
		}else if(c_ret == LINE_X_STA_L_RIGHT || c_ret == LINE_X_STA_T_RIGHT)
		{
			turn_to_deg_v(0);
		}else
		{
			log_printf("cret error\r\n");
			return AJUST_Y;
		}	
		*/
	}
	return AJUST_MOVE;
}

/*
���һ���ߣ��Ƕ�ͷ����T����
*/
/*
uint8_t check_line_x_sta(int16_t ox,int16_t oy,int16_t len,int16_t y_dir)
{
	int16_t x,i;
	for(x = ox;x >= (ox - len);x--)
	{
		if(OBST_XY(x,oy+y_dir) && OBST_XY(x,oy+y_dir*2))
		{
			log_printf("[check_line_x_sta]found Tleft(%d,%d,%d)\r\n",x,oy,y_dir);	
			return LINE_X_STA_T_LEFT;
		}
		//����ɨ
		if(SCANE_XY(x,oy-2) && SCANE_XY(x,oy-1) && SCANE_XY(x,oy) && SCANE_XY(x,oy+1) && SCANE_XY(x,oy+2) )
		{
			log_printf("[check_line_x_sta]found Lleft(%d,%d,%d)\r\n",x,oy,y_dir);	
			return LINE_X_STA_L_LEFT;
		}
	}
	for(x = ox;x <= (ox + len);x++)
	{
		if(OBST_XY(x,oy+y_dir) && OBST_XY(x,oy+y_dir*2))
		{
			log_printf("[check_line_x_sta]found Tright(%d,%d,%d)\r\n",x,oy,y_dir);	
			return LINE_X_STA_T_RIGHT;
		}
		//����ɨ
		if(SCANE_XY(x,oy-2) && SCANE_XY(x,oy-1) && SCANE_XY(x,oy) && SCANE_XY(x,oy+1) && SCANE_XY(x,oy+2) )
		{
			log_printf("[check_line_x_sta]found Lright(%d,%d,%d)\r\n",x,oy,y_dir);	
			return LINE_X_STA_L_RIGHT;
		}
	}
	return LINE_X_STA_NO;
}

uint8_t check_line_y_sta(int16_t ox,int16_t oy,int16_t len,int16_t x_dir)
{
	int16_t y,i;
	for(y = oy;y >= (oy - len);y--)
	{
		if(OBST_XY(ox + x_dir,y) && OBST_XY(ox +x_dir *2,y))
		{
			log_printf("[check_line_x_sta]found Tlow(%d,%d,%d)\r\n",ox,y,x_dir);	
			return LINE_X_STA_T_LOW;
		}
		//����ɨ
		if(SCANE_XY(ox-2,y) && SCANE_XY(ox-1,y) && SCANE_XY(ox,y) && SCANE_XY(ox+1,y) && SCANE_XY(ox+2,y) )
		{
			log_printf("[check_line_x_sta]found Llow(%d,%d,%d)\r\n",ox,y,x_dir);	
			return LINE_X_STA_L_LOW;
		}
	}
	for(y = oy;y <= (oy + len);y++)
	{
		if(OBST_XY(ox + x_dir,y) && OBST_XY(ox +x_dir *2,y))
		{
			log_printf("[check_line_x_sta]found Thight(%d,%d,%d)\r\n",ox,y,x_dir);	
			return LINE_X_STA_T_RIGHT;
		}
		//����ɨ
		if(SCANE_XY(ox-2,y) && SCANE_XY(ox-1,y) && SCANE_XY(ox,y) && SCANE_XY(ox+1,y) && SCANE_XY(ox+2,y) )
		{
			log_printf("[check_line_x_sta]found Lhight(%d,%d,%d)\r\n",ox,y,x_dir);	
			return LINE_X_STA_L_HIGHT;
		}
	}
	return LINE_X_STA_NO;
}
*/
#define MAX_Y_LEN		5
/*
��У׼��X���ǰ���£�����Y�ᡣ


uint8_t ajust_y_from_x(int16_t tx,int16_t ty)
{
	int16_t x1,x2,y;
	uint8_t dir1=1,dir2=1;
	int16_t c=0;
	int16_t tox=0,ox=0,oy=0;
	//�ҳ�X���ȥ�ķ���
	x1 = tx;
	x2 = tx -1;

	//X,�����������ߣ�
	while(1)
	{
		if(c++ >= 10)
		{
			log_printf("[ajust_y_from_x]c over\r\n");
			return 0;
		}
		//==============Y�����������ߣ��ҵ��ϰ���===================
		//===��Y+������===
		if(OBST_XY(x1,ty) || OBST_XY(x1,ty+1) || OBST_XY(x1,ty-1))	//�����ϰ����ˡ������������ߣ����ǲ�����ߣ������������ߣ������ʺϹ�ȥ
		{
			dir1 = 0;
		}
		if(dir1)
		{
			for(y = ty;y<=(MAX_Y_LEN+ty);y++)
			{
				if(OBST_XY(x1,y))		//�ҵ��ϰ�����˳�
				{

					log_printf("[ajust_y_from_x]found obst(%d,%d,)\r\n",x1,y);
					tox = x1;
					ox 	= x1;
					oy	= y;
					break;
				}
				if(OBST_XY(x1+1,y) || OBST_XY(x1-1,y))		//����������ϰ�����ǲ��������ߣ���Ҳ�˳��������ˡ�
				{
					log_printf("[ajust_y_from_x]found side obst(%d,%d,)\r\n",x1,y);
					break;
				}
			}
		}
		//�ҵ�Ҫȥ�����ĵ��ˡ����˳���ȥ����
		if(tox >0 && ox >0 && oy > 0)
		{
			log_printf("FoundA\r\n");
			break;
		}
		//===��Y��������===
		if(OBST_XY(x2,ty) || OBST_XY(x2,ty+1) || OBST_XY(x2,ty-1))	//�����ϰ����ˡ������������ߣ����ǲ�����ߣ������������ߣ������ʺϹ�ȥ
		{
			dir2 = 0;
		}
		if(dir2)
		{
			for(y = ty-1;y >= (ty - MAX_Y_LEN);y--)
			{
				if(OBST_XY(x2,y))		//�ҵ��ϰ�����˳�
				{

					log_printf("[ajust_y_from_x]found obst2(%d,%d,)\r\n",x2,y);
					tox = x2;
					ox 	= x2;
					oy	= y;
					break;
				}
				if(OBST_XY(x2+1,y) || OBST_XY(x2-1,y))		//����������ϰ�����ǲ��������ߣ���Ҳ�˳��������ˡ�
				{
					log_printf("[ajust_y_from_x]found side obst(%d,%d,)\r\n",x2,y);
					break;
				}
			}
		}
		//�ҵ�Ҫȥ�����ĵ��ˡ����˳���ȥ����
		if(tox >0 && ox >0 && oy > 0)
		{
			log_printf("FoundA\r\n");
			break;
		}
		x1++;
		x2--;
	}

	if(tox >0 && ox >0 && oy > 0)
	{
		int16_t y_dir;
		int dist;
		log_printf("[ajust_y_from_x]found adj(%d,%d,)->(%d,%d,)->(%d,%d)\r\n",X_NOW,Y_NOW,tox,Y_NOW,ox,oy);
		if(tox != tx)		//��Ҫ��һ�£�
		{
			if(walk_map_cord_only(tox,ty,1000,10) == 0)
			{
				log_printf("bumA\r\n ");
				return 0;
			}
		}
		if(ox != X_NOW || oy == Y_NOW)
		{
			log_printf("Xerr\r\n");
			return 0;
		}
		if(oy > Y_NOW)
		{
			dist = (oy - Y_NOW + 5) * HW_GRID;
			turn_to_deg(90);
			y_dir = -1;
		}else
		{
			dist = ( Y_NOW - oy + 5) * HW_GRID;
			turn_to_deg(270);
			y_dir = 1;
		}
		//��ײ�ˣ������
		if(motor_go_forwark_only(dist,NO_SIDE_NEAR) == 0)
		{
			log_printf("[ajust_y_from_x]bum,now=(%d,%d,),targe=(%d,%d,)ajust\r\n",X_NOW,Y_NOW,X_NOW,oy + y_dir);
			ajust_xy_map(X_NOW,oy+y_dir,AJUST_Y);
			return AJUST_Y;
		}else
			return 0;
		
		

	}
	return 0;
}
*/

int16_t map_edge_x(int16_t xx,int16_t yy,int16_t x_dir,int16_t y_dir,int16_t xlen,int16_t *ox,int16_t *oy,uint8_t *ret,uint8_t *clrsta)
{
	int16_t i,x,y,nx;
//	int16_t oox,ooy;
//	int16_t flat = 0;		//����ƽ̹ ϵ��
	int16_t lenght=0;		//���ȡ�
	uint8_t csta[8];
//	uint8_t isOK=0;
	*ret = 0;
	*clrsta = 0;
	if(SCANE_XY(xx,yy) ==0)
	{
		log_printf("[map_edge]not a scan(%d,%d,)\r\n",xx,yy);
		return  0 ;
	}
	//��X������������ߣ��ҵ������ߵı߽�
	x = xx;
	y = yy;
	for(i=0;i<xlen;i++)
	{
		if(clean_sta(x,y) == 0 || OBST_XY(x,y)==1)
		{
			//log_printf("[map_edge]found obst(%d,%d,)\r\n",x,y);
			break;
		}
		x+=x_dir;
		
	}
	if(i >=xlen)
	{
		log_printf("[map_edge]not found obst\r\n");
		return 0 ;
	}
	//���Ÿ����ߵı߽磬�����ҳ���ͼ�ĵı߽�  
	while(1)
	{
		y+=y_dir;
		lenght++;

		nx = 0;
		
		//����ȡ7����//csta[3]���м�ĵ�
		for(i=0;i<7;i++)
			csta[i] = point_sta(x-3+i,y);		//
		//=============��������������Լ���=============
		if(  (/*csta[3] == UNCLN_STA || */csta[3] == OBST_STA) && (csta[3-x_dir]==SCAN_STA && csta[3-x_dir*2]==SCAN_STA))  //��һ���������ã��պ����ϰ���Һ�߶�����ɨ��
		{
			nx = x;
		}else if((/*csta[3+x_dir] == UNCLN_STA ||*/ csta[3+x_dir] == OBST_STA) && (csta[3]==SCAN_STA && csta[3-x_dir]==SCAN_STA))//�ڶ����������һ��
		{
			nx = x+x_dir;
		}else if((/*csta[3-x_dir] == UNCLN_STA || */csta[3-x_dir] == OBST_STA) && (csta[3-2*x_dir]==SCAN_STA && csta[3-2*x_dir]==SCAN_STA))
		{
			nx = x-x_dir;
		}
		if(nx >0)			//һ���ߣ�
		{
			drow_point(nx,y,3);
			continue;
		}else
		{
			*ox = x;
			*oy = y;

			//T����
			if((csta[3] == UNCLN_STA || csta[3] == OBST_STA) && csta[3-x_dir] == OBST_STA && csta[3-x_dir*2] == OBST_STA)
			{
				*ret = MAP_LINE_T; 
				return lenght;
			}
			//һ����
			if(csta[0] == SCAN_STA && csta[1] == SCAN_STA && csta[2] == SCAN_STA && csta[3] == SCAN_STA && csta[4] == SCAN_STA && 
				csta[5] == SCAN_STA && csta[6] == SCAN_STA )
			{
				*ret = MAP_LINE_L; 
				if(clean_sta(x+x_dir,y-y_dir)==0 && clean_sta(x+x_dir*2,y-y_dir) == 0)
				{
					log_printf("found uncln(%d,%d,)\r\n",x+x_dir,y-y_dir);
					*clrsta = 1;
				}
				return lenght;
			}
			return lenght;
		}

	}
	
	
}


#if 0
int unclr_piont_walk(uint8_t old_side)
{
	char  ret;
	int16_t y_dir = navigat->side == LEFT_SIDE?1:-1;
//	uint8_t side;
	float angle = NO_ANGLE_V;		//�����Ҫȥ�ĵĽǶ�
	if(OBST_XY(X_NOW+1,Y_NOW))		//ǰ�����ϰ��һ����ײ��ʱ����180���ߣ�
		angle = 180;
	if(OBST_XY(X_NOW-1,Y_NOW))		//ǰ�����ϰ��һ����ײ��ʱ����180���ߣ�
		angle = 0;
	if(angle ==NO_ANGLE_V)
	{
		if(SCANE_XY(X_NOW-2,Y_NOW+y_dir) == 0 && SCANE_XY(X_NOW-3,Y_NOW+y_dir) == 0 )
			angle = 180;
		else 
			angle = 0;
	}
	log_printf("[unclr_piont_walk]angle=%3.1f\r\n",angle);
	//����������
	//if(X_NOW == tox && Y_NOW == toy)
	{
		//У�鷽��
		if(navigat->side == LEFT_SIDE)
			ret = walk_map_cord(navigat->tx,navigat->ty+1,NO_ANGLE_V,0);
		else
		{
			ret =walk_map_cord(navigat->tx,navigat->ty-1,NO_ANGLE_V,0);
			//�ӳ�����������Ҹ�ת���ұߣ����߰�񣬱���װ�ϳ��׮
			if(old_side == LEFT_SIDE && navigat->from_charge)
			{
				motor_go_forwark(200,NO_SIDE_NEAR,NULL);
			}
		}

		
	}
	log_printf("[unclr_piont_walk]walk ret=%d,gsta=%d\r\n",ret,sys->gsta);

	//��ײ�ˣ��ӱ߽�ȥһ��
	if(ret ==0)
	{
		ret = nearwall_cross_map(navigat->y_org+400*y_dir,angle,0,20);	//�ӱ�
	}
	
	navigat->m_navi_run = 2;	//������λ��һ�����У���ҪУ׼��
	if((SCANE_XY(X_NOW+1,Y_NOW)==1 && SCANE_XY(X_NOW-1,Y_NOW)==0)	|| 	//�����ɨ,�ұ�δ��ɨ
		(clean_sta(X_NOW+1,Y_NOW)==0 && SCANE_XY(X_NOW+1,Y_NOW)==1 && SCANE_XY(X_NOW-1,Y_NOW)==0)	)
	{
		turn_to_deg(180);	
	}else
		turn_to_deg(0);
	return 1;
}
#endif
/*
�˶�����ϵ�ͼ


int16_t fitting_on_map(void)
{
	uint8_t adj_xy=0;
	//�ȿ��������Ƿ����ϰ��

	//0�ȷ���
	if(disfloat(sys->angle,0) < 15)
	{	//�������ϰ����û�к���
		if(OBST_XY(X_NOW,Y_NOW+1) ==1 && LEFT_ADC() < cfg->min_left_adc)
		{
			log_printf("0_dir(%d,%d,),side obst but not ir\r\n",X_NOW,Y_NOW);
			
		}
	}
	
	if(OBST_XY(X_NOW,Y_NOW-1)==1)
	{
		//���ϰ����������
		if ((navigat->angle == 0 && RIGHT_ADC() < cfg->min_right_adc) ||
		     (navigat->angle == 180 && LEFT_ADC() < cfg->min_left_adc))
		{
			log_printf("found OBST but not near..\r\n");
			return CHECK_SIDE_NOT_NEAR;
		}else
			return CHECK_SIDE_MAP_OK;
	}else if(OBST_XY(X_NOW,Y_NOW + 1) ==1)
	{
		if ((navigat->angle == 0 && LEFT_ADC() < cfg->min_left_adc) ||
			 (navigat->angle == 180 && RIGHT_ADC() < cfg->min_right_adc))
		{
			log_printf("found OBST but not near2..\r\n");
			return CHECK_SIDE_NOT_NEAR;
		}else
			return CHECK_SIDE_MAP_OK;

	}
	//�к��⣬����û���ϰ���
	if(LEFT_ADC() > cfg->min_left_adc)
	{
		if((navigat->angle == 0 && OBST_XY(X_NOW,Y_NOW+1) ==0 ) || 
			(navigat->angle == 180 && OBST_XY(X_NOW,Y_NOW-1) ==0 ) 
		{
			return CHECK_SIDE_NOT_OBST;
		}
	}
	if(RIGHT_ADC() > cfg->min_right_adc)
	{
		if((navigat->angle == 0 && OBST_XY(X_NOW,Y_NOW-1) ==0 ) || 
			(navigat->angle == 180 && OBST_XY(X_NOW,Y_NOW+1) ==0 ) 
		{
			return CHECK_SIDE_NOT_OBST;
		}

	}	
}
*/

#if 0
int calibration_handle(navigat_t *navi,short bx,short by,uint16_t gsta)
{
	
	int i;
	if(gsta ==0 && !IR_MIDLE_BUM())
	{
		log_printf("[proc_walk_road]not bum,gsta=%d,ir=%d,%d,%d,walklen=%d\r\n",gsta,sys->g_sta[2],sys->g_sta[3],sys->g_sta[4],navigat->walk_len);
		return 0;
	}
	if(navi->angle == 0 || navi->angle == 180)		// X���߶���������
	{
		short x;
		int x_dir;
		int obst_dir = 0;
//		int n_dir;
//		short walk_grid;
//		short cross_x = 0,cross_y = 0;
		short prev_x = 0,prev_y=0;		//Ԥ����ϰ���
		short dis;
		OBST *obst;
		
		
		//��������Ǻϣ����������
		x_dir = navi->angle ==0?1:-1;
		//=====================�ҵ�Ԥ����ϰ���===================
		x = bx;		//��ʼ��
		for(i=0;i<navigat->walk_len*2;i++)
		{
			//
			if(OBST_XY(x,navi->ty) && SCANE_XY(x - x_dir,navi->ty))
			{
				prev_x = x;
				prev_y = navi->ty;
				obst = get_obst(prev_x,prev_y);
				if(obst !=NULL)
				{
					obst_dir = obst->x > obst->tx?1:-1;
				}
				break;
			}
			x+=x_dir;
		}
		
		
		if(prev_x ==0 || prev_y ==0 || obst==NULL || obst_dir != x_dir)
		{
			log_printf("[proc_walk_road]not found obst(%d,%d,)dir=%d,%d\r\n",prev_x,prev_y,obst_dir,x_dir);
			return 0;
		}
		log_printf("[proc_walk_road]found prev obst(%d,%d,)now=(%d,%d,)\r\n",prev_x,prev_y,X_NOW,Y_NOW);
		//������첻��ֱ��ʹ��,����ʹ��
		dis = dis_xy(prev_x,X_NOW);
		//һ�����ڵĲ�࣬��ȡ�ȽϿ������������
		if(dis <=1 && check_line(CHECK_Y_LINE,prev_x,prev_y,1)==1)
		{
			log_printf("[proc_walk_road]AJUST1,x_org=%d,%d\r\n",navi->x_org,obst->x_org);
			if(dis_xy(navi->x_org,obst->x_org) < 500)
			{
				ajust_xy_org(obst->x_org,navi->y_org,100,AJUST_X);
				return 1;
			}
		}
		if(dis <=2 &&  check_line(CHECK_Y_LINE,prev_x,prev_y,2)==1)
		{
			log_printf("[proc_walk_road]AJUST2,x_org=%d,%d\r\n",navi->x_org,obst->x_org);
			if(dis_xy(navi->x_org,obst->x_org) < HW_GRID*2)
			{
				ajust_xy_org(obst->x_org,navi->y_org,100,AJUST_X);
				return 1;
			}			
			
		}
		
		if(dis >=4 )
		{
			/*
			��ѯ����һ���Y�ᣬ�������ǲ�������Y�����һ���������
			��������һ����Y�ᣬ�����ǲ�����X�����ϰ������У�������Y��ƫ���ˡ�
			*/
			int ix,iy;
			short xdir[2],ydir[2]={1,-1};
			xdir[0] = x_dir;
			xdir[1] = 0;
			for(ix=0;ix<2;ix++)
			{
				for(iy=0;iy<2;iy++)
				{
					if(OBST_XY(navi->tx+xdir[ix],navi->ty+ydir[iy]) )
					{
						OBST *g_obst = get_obst(navi->tx+xdir[ix],navi->ty+ydir[iy]);
						if(g_obst !=NULL)
						{
							log_printf("[proc_walk_road]AJUST SIDE1,found obst=(%d,%d,)\r\n",navi->tx+xdir[ix],navi->ty+ydir[iy]);
							if(dis_xy(navi->x_org,g_obst->x_org) <400 && dis_xy(navi->y_org,g_obst->y_org) < 500)
							{
								ajust_xy_org(g_obst->x_org,g_obst->y_org,100,AJUST_X | AJUST_Y);
								return 1;
							}

						}
					}
				}
			}
			//һ���ߣ������һ��,�����������ϰ��������ǰû�������ӱ�ȥɨ��ͼ������������
			if(check_line(CHECK_Y_LINE,prev_x,prev_y,2))
			{
				log_printf("big dist ,near wall fillting map\r\n");
		
				fitting_map_near_wall(10,8,5,5);
			}
		}
		
		return 0;
		{
/*
			����ܴ��м��ֿ���
			1.�����ǶԵģ������ϰ���֮ǰ��ײ����Ϊ�����µ��ϰ������������������ϰ���
			2.������ˣ����ڴ򻬵�ԭ�򣬶���·��ȴ��������ˣ���ʵ�Ѿ������ϰ��������ˣ�ȴ��Ϊû��
			  ��Ҫ�������ϰ�������ȥ
			3.X���������ˣ�Y��Եģ����´����ϰ����ˡ�
			4.X�ᣬY�ᶼ���ˣ������ǶԵģ����´����ϰ������ǰ���ˡ�
			
*/
		}
		
#if 0		
		//walk_grid = bx > navi->tx?bx - navi->tx:navi->tx-bx;			//ȡ���߹���·��
		//log_printf("[proc_walk_road]walkgrid=%d\r\n",walk_grid);
		//========================��һ���������Ƿ񴩹��ϰ��� ========================
		if(navigat->is_cross_obst)
		{
			int c=0;
			
			for(i=0;i<navigat->walk_len;i++)
			{
				//���ϰ�������ϰ���ĸ���
				if(OBST_XY(navigat->x_walk[i],navigat->y_walk[i]))
				{
					c++;
					cross_x = navigat->x_walk[i];
					cross_y = navigat->y_walk[i];
				}
					
				
			}
			log_printf("[proc_walk_road]corss obst,c=%d\r\n",c);
			if(c >=3)   //��Խ��һ���ϰ�����
			{
				//short x_obst,y_obst;
				OBST *obst;
				for(i=0;i<navigat->walk_len;i++)
				{
					if(OBST_XY(navigat->x_walk[i],navigat->y_walk[i]))
					{
						//ȡ���ϰ���
						obst = get_obst(navigat->x_walk[i],navigat->y_walk[i]);
						if(obst !=NULL)
						{
							short ret,ox=0,oy=0;
							ret = get_linex_obst(obst->tx,obst->ty,&ox,&oy,x_dir);
							
							log_printf("obst=(%d,%d,),r=(%d,%d,)ret=%d,endobst=(%d,%d,)\r\n",obst->x,obst->y,obst->tx,obst->ty,ret,ox,oy);
							if(ret && ox !=0)
							{
								//X���ϰ���ĵ㣬�͵�ǰ����첻������Ϊ�ɹ�������������
								if(disxy(navigat->tx , ox) < 3 )
								{
									log_printf("AJUST k1\r\n");
									ajust_xy_org(navigat->x_org,obst->y_org,100,AJUST_Y);
									return 1;
								}
							}
						}
						
					}
				}				
			}
		}
/*
		//ǰ����û�У��еĻ�������һ�¡�
		for(x=navi->tx-1;x <= navi->tx+1;x++)
		{
			if(OBST_XY(x,navi->ty))
			{
				int obst_dir;
				OBST *obst = get_obst(x,navi->ty);
				if(obst!=0)
				{	
					obst_dir = obst->x > obst->tx?1:-1;
					if(obst_dir == x_dir)
					{
						log_printf("[proc_walk_road]found near obst(%d,%d,)\r\n",x,navi->ty);
						obst = get_obst(x,navi->ty);
						if(obst !=NULL)
						{
							ajust_xy_org(obst->x_org,obst->y_org,90,AJUST_X);
							return 1;
						}					

					}
				}
				
			}
		}
	*/
		//�����ң������ĸ��������,������
		if(cross_x > 0 && disxy(cross_x,navi->tx) >=3)
		{
			int x,y;
			//���ܱߵ�Y����
			for(y = Y_NOW - 1;y <= (Y_NOW +1);y++)
			{
				
				for(x = X_NOW-1;x<=X_NOW+1;x++)
				{
					if(OBST_XY(x,y))
					{
						OBST *obst;
						log_printf("found side near obst(%d,%d,)\r\n",x,y);
						obst = get_obst(x,y);
						if(obst !=NULL)
						{
							int obst_dir = obst->x > obst->tx?1:-1;
							short len = get_linex_len(obst->tx,obst->ty);
							log_printf("oside=%d,%d,len=%d,\r\n",x_dir,obst_dir,len);
							//����һ�£����ߵľ���һ�£���У�����
							if( obst_dir == x_dir && disxy(len,navigat->walk_len) <=3)
							{
								log_printf("AJUST K4\r\n");
								ajust_xy_org(obst->x_org,obst->y_org,100,AJUST_X | AJUST_Y);
								return 0;
							}
						}
						
					}
				}
			}

		}
	#endif		
	}

	return 0;
}
#endif
