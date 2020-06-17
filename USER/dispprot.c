
#include "sys.h"
#if 0
uint8_t dp_errcode[] =
{
	0,
	11,
	1,
	4,
	8,
	3,
	8,
	5,
	3,
	0,
	0,
	0,
};
#endif

//ȫ�ֵ�ԤԼ����,ֻ����ת��WIFI���Կ�֮���໥�����ԤԼ����
T_SCH_DATA dp_schdata;

//ԤԼ���ݲ��飬����Ƿ��зǷ�������
int8_t dp_checksyscfgdata(T_SCH_DATA *ptr_sch_data)
{
	uint8_t i;
	int8_t md = 0;
	//uint8_t temp;
	if(ptr_sch_data->week_en & 0x80)
	{
		ptr_sch_data->week_en = 0;
		md = 1;
	}
	for(i = 0;i < 7;i ++)
	{
		if((ptr_sch_data->hour[i] & 0x1f) > 23)
		{
			ptr_sch_data->hour[i] = 9;
			//ptr_sch_data->hour[i] = (ptr_sch_data->hour[i] & 0x60) + 9;
			md = -1;
		}
		if(ptr_sch_data->min[i] > 59)
		{
			ptr_sch_data->min[i] = 0;
			md = -1;
		}
	}
	return md;
}

//����ԤԼ���ݵ��Կ�(������ԤԼ���ݷ�װ������WIFI֮��Ĵ������)
void dp_send_schdata(void)
{
	dp_tx_data(DISP_CMD_SET_SCHEDULE_TIME,sizeof(T_SCH_DATA),(uint8_t *)&dp_schdata);
}

//����һ��ԤԼ���ݵ�dp_schdata
void dp_set_schdata(uint8_t wday,uint8_t hour,uint8_t minute,uint8_t powermode,uint8_t en)
{
	if(wday > 6)return;
	if(en)
		dp_schdata.week_en |= (1 << wday);
	else
 		dp_schdata.week_en &= ~(1 << wday);
 	if(powermode > 2)powermode = 0;
 	dp_schdata.hour[wday] = hour | (powermode << 5);
 	dp_schdata.min[wday] = minute;
}

void dp_get_schdata_conv2wifi(uint8_t *wifi_sch)
{
	uint8_t i, n_powermode;
	T_WIFI_SCH_DATA *p_wifi_sch;

	p_wifi_sch = (T_WIFI_SCH_DATA *)wifi_sch;

	for(i = 0;i < 7;i ++)
	{
		n_powermode = (dp_schdata.hour[i] >> 5) & 0x03;
		p_wifi_sch[i].powermode = (n_powermode == 2)?0:(n_powermode + 1);
		p_wifi_sch[i].hour = dp_schdata.hour[i] & 0x1f;
		p_wifi_sch[i].minute = dp_schdata.min[i];
		p_wifi_sch[i].en = ((dp_schdata.week_en & (1 << i)) != 0)?1:0;
	}
}

uint8_t dp_camera_error = 0;
static DP_PROTDAT dp_protdattx = {DP_PROTHEAD_1,DP_PROTHEAD_2,0,0,0,0,0,0,0,0,0};


//��ͨ��,week��λ��ʾ,bit0����һ,bit1���ܶ���,�˺�������������ת��Ϊʵ��������,ȡֵ0-6,��ʾ��1������,0xffΪ��Ч��ֵ
//���week bits�з�ֻ��һλΪ1,����Ч,����0xff
uint8_t dp_weekbit2dec(uint8_t weekbit)
{
	uint8_t weekdec = 0;
	while(weekbit)
	{
		if(weekbit & 0x01)
		{
			if(weekbit == 0x01)return weekdec;
			return 0xff;
		}
		weekbit >>= 1;
		weekdec ++;
	}
	return 0xff;
}

//�ڶ��������ݻ������ҵ�ָ���������Ӵ�
//�ҵ������±�
//δ�ҵ�����-1
int16_t dp_findheadindata(uint8_t *ptrData, uint16_t len,uint8_t *ptrDatCmp,uint16_t Cmplen)
{
	uint16_t i,j;

	if(Cmplen > len)
		return -1;

	for(i = 0;i < len - Cmplen;i ++)
	{
		for(j = 0;j < Cmplen;j ++)
		{
			if(ptrDatCmp[j] != ptrData[i + j])
				break;
		}
		if(j == Cmplen)
			return i;
	}

	return -1;
}

uint8_t dp_checksum(DP_PROTDAT *ptrData)
{
	uint8_t chksum = 0;
	uint8_t i;

	for(i = 3;i < DP_PROT_COMMLENGTH;i ++)
	{
		chksum += ((uint8_t *)ptrData)[i];
	}
	return chksum;
}
uint8_t dp_checksum2(uint8_t *ptrData,uint8_t len)
{
	uint8_t chksum = 0;
	uint8_t i;

	for(i = 0;i < len;i ++)
	{
		chksum += ptrData[i];
	}
	return chksum;
}
/*
void dp_tx_data(uint8_t msg,uint8_t option1,uint8_t option2,uint8_t option3,uint8_t option4)
{
#if !LASER_EN
	if(SYS_TEST_BD != sys->sState)
	{
	DP_PROTDAT *ptrProtDat = &dp_protdattx;
	//ptrProtDat->head1 = DP_PROTHEAD_1;
	//ptrProtDat->head2 = DP_PROTHEAD_2;
	memset((uint8_t *)(ptrProtDat + 2),0,DP_PROT_COMMLENGTH - 2);
	log_printf("[dp_tx_data]msg=%02x,opt1=%d,opt2=%d,opt3=%d,op4=%d\r\n",msg,option1,option2,option3,option4);
	ptrProtDat->data.opt.opt1 = option1;
	//if(DP_SYS_STA == msg)
	//	ptrProtDat->data.opt.opt2 = dp_errcode[option2];
	//else
		ptrProtDat->data.opt.opt2 = option2;
	ptrProtDat->data.opt.opt3 = option3;
	ptrProtDat->data.opt.opt4 = option4;
	ptrProtDat->cmd = msg;
	ptrProtDat->chksum = dp_checksum(ptrProtDat);
	usart4_write((uint8_t *)ptrProtDat, DP_PROT_COMMLENGTH);
	}
#endif
}
*/
//ִ�н��յ���ָ��
//�˺�������У����,��Ҫ����֮��������
#if 0
void dp_cmd_exec(DP_PROTDAT *ptrProtDat)
{
	if(sys->sState == SYS_TEST_BD && tm_mode == 1)//��������ģʽ��,ң�������Կ��·�������ָ����������ڲ���
	{
		if(ptrProtDat->cmd != 0x0a )
			set_remote_flag(0x02);
		set_remote_flag(0x01);
		return;
	}
	if(sys->power_on ||  ptrProtDat->cmd ==DP_CMD_KEY_POWER )
	{
//--------------------------------------------------------modified by wonton2004 20170118
		//if(ptrProtDat->cmd ==DP_CMD_KEY_POWER  && ptrProtDat->data.powerstatus.power)
			//ny3p_play(VOICE_SELECT_M);
		if(ptrProtDat->cmd ==DP_CMD_KEY_POWER)
		{
			if(ptrProtDat->data.powerstatus.power)
				ny3p_play(VOICE_SELECT_M);
			else
				//ny3p_play(VOICE_DIDI);//
				if(sys->sState == SYS_CHARGE)
					ny3p_play(VOICE_CHARING);
				else
					ny3p_play(VOICE_M_SLEEP);
		}
//------------------------------------------------------------------------------------end
		else if(ptrProtDat->cmd != DP_CMD_KEY_MANUAL && ptrProtDat->cmd != DP_CMD_KEY_SETSCHEDULE &&
				ptrProtDat->cmd != DP_CMD_KEY_STATUS)
			ny3p_play(VOICE_DIDI);
		sys->wifistatechange = 1;
	}
	else
	{
		if(sys->sState != SYS_TEST_BD)
			log_printf("power off,%d,%d\r\n",sys->power_on,ptrProtDat->cmd ==DP_CMD_KEY_POWER);
		//return ;
	}
	/*
	if(EXTERAL_AC_DETECT())
	{
		log_printf("EXTERAL_AC_DETECT\r\n");
		return ;
	}
	*/
	if( READ_PSW_DET() )
	{
		;//ny3p_play(VOICE_START_CHARGE);


	}
	else
	{
		/*
		if(sys->work_errcode  == MWERR_POWEROFF)
			sys->work_errcode = MWERR_NONE;
			*/
	switch(ptrProtDat->cmd)
	{
		case DP_CMD_KEY_POWER:
			/*
			if( ptrProtDat->data.powerstatus.power ==0 || ptrProtDat->data.powerstatus.power == 1)
			{
				sys->power_on = ptrProtDat->data.powerstatus.power ;
				log_printf("power_on=%d\r\n",sys->power_on);


			}else
			{
				log_printf("power on error\r\n");
			}
			*/
			//�ֳ����ԣ�����Ϩ����Ļ
			log_printf("power_on=%d\r\n",ptrProtDat->data.powerstatus.power);
			sys->power_on = 1;
			//�����ֵ���
				if(sys->f_work_mode == MWO_SPOT)
					sys->up_work_mod = MWO_TURBO;
				else
					sys->up_work_mod = sys->f_work_mode;
			//dp_tx_data(PD_MSG_WORK_STA, 0x80, sys->work_mod,0,0);
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			break;
		case DP_CMD_KEY_PLAY:
			all_key_led_off();

			//ֻ�������к�ֹͣ
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			log_printf("work msg,go=%d,sta=%d,optin=%d\r\n",ptrProtDat->data.work.go,ptrProtDat->data.work.sta,ptrProtDat->data.work.option);
				if(0)//if(ptrProtDat->data.work.option & MWO_AUTO == 0)
				{
					if(ptrProtDat->data.opt.opt2 & 0x07)
					{
						sys->work_mod = ptrProtDat->data.opt.opt2;			//ģʽ����
						sys->f_work_mode = ptrProtDat->data.opt.opt2;
					}
					else if(ptrProtDat->data.opt.opt2 & MWO_FULL2GO)
					{
						log_printf("full and go\r\n");
						sys->full2go = 1;
						sys->f_work_mode = ptrProtDat->data.opt.opt2;
					}
					else if(!(ptrProtDat->data.opt.opt2 & MWO_FULL2GO))
					{
						log_printf("no full and go\r\n");
						sys->full2go = 0;
						sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
						break;
					}
				}
			if(ptrProtDat->data.work.go == ROBOT_STOP || ptrProtDat->data.work.go == ROBOT_WORK)
			{
				if(EXTERAL_AC_DETECT())
				{
					log_printf("EXTERAL_AC_DETECT2,out\r\n");
					return ;
				}
				if(ptrProtDat->data.work.go == ROBOT_WORK)
				{
					//���е�ʱ��У��״̬
					if(ptrProtDat->data.work.sta !=SYS_NAVIGAT && ptrProtDat->data.work.sta != SYS_FOCUS && ptrProtDat->data.work.sta !=SYS_DOCK && ptrProtDat->data.work.sta != SYS_RANDOM)
					{	ny3p_play(VOICE_DRIP);
						log_printf("error work sta=%d\r\n",ptrProtDat->data.work.sta);
						break;
					}

				}
				log_printf("go=%d,sta=%d\r\n",ptrProtDat->data.work.go,ptrProtDat->data.work.sta);
				if(sys->sState == SYS_NAVIGAT || sys->sState == SYS_FOCUS || sys->sState == SYS_DOCK)
				{
					log_printf("stop robot\r\n");
					/*
					if(sys->sState == SYS_NAVIGAT)
						navigat->suspend = TRUE;
					*/
					//micony 2017-06-12
					navigat->suspend = FALSE;
					sys->sState = SYS_IDLE;
					motor_run(GO_STOP,0,0,0);
					log_printf("MOTOR_POWER_OFF 1\r\n");
					MOTOR_POWER_OFF();
					ny3p_play(VOICE_M_STOP);										///����¼��Ϊֹͣ��ɨ
					sys->sState_auxi = 0;
						if(sys->spot_flag == 0)
						{
					sys->t_halt_on=1;
					sys->t_flash_halt = 600000;		//10����
						}
                    LED3_OFF();

					break;
				}else //if(ptrProtDat->data.work.go == ROBOT_WORK)
				{
					log_printf("robot work,sta=%d,msata=%dr\n",ptrProtDat->data.work.sta,sys->mState);
					/*
					if(sys->mState!= GO_STOP)
					{
						motor_run(GO_STOP,0,0,0);
						STOP_ALL_MOTOR();
						ny3p_play(VOICE_DIDI);
						break;
					}
					*/
					if(sys->sState == ptrProtDat->data.work.sta)					///״̬��ͬ����Ӧ����   2018-08-23 jzz
						break;

					if(sys->sState != SYS_IDLE && ptrProtDat->data.work.sta == SYS_RANDOM)///״̬���ǿ��� �����Կ��·�����ģʽ ����Ӧ����  2018-08-23 jzz
						break;
				//	ny3p_play(VOICE_DIDI);


#if 0
					if(ptrProtDat->data.work.sta == SYS_DOCK)		//---2018-09-26
						sys->sState_auxi = 1;
					else
						sys->sState_auxi = 0;
#else
					if(ptrProtDat->data.work.sta == SYS_DOCK)		//---2018-09-26
					{
						if(sys->sState == SYS_DOCK || sys->sState == SYS_CHARGE)
						{
							sys->sState_auxi = 1;
							//ny3p_play(VOICE_M_DOCK);
								//delay_sensor(300);
						}
						if(sys->sState != SYS_DOCK && sys->sState != SYS_CHARGE)
						{
							if(sys->sState == SYS_IDLE && sys->t_idle !=0)//���������Ŀ���״̬
      							sys->wifiAreaNoCalc_en = 1;//���س䰴������ʱ,��������ǿ���״̬,����س��ϱ����������,ֱ��Ϊ0,�������ɨģʽ,�ϱ�ʵ�ʼ������
							sys->sState = SYS_DOCK;
							sys->sState_auxi = 1;
						}
						if(sys->sState == SYS_CHARGE)
						{
								//ny3p_play(VOICE_CHARING);
							sys->sState_auxi = 0;
						}
						//sys->wifistatechange = 1;
					}
					else
					{
						//delay(300);
						sys->sState_auxi = 0;
							//sys->work_mod =  ptrProtDat->data.work.option;
							if(ptrProtDat->data.work.sta == SYS_FOCUS)
								sys->f_work_mode = MWO_SPOT;
							//log_printf("work_mod:(option)%d,sSta:(sta)%d\r\n",ptrProtDat->data.work.option,ptrProtDat->data.work.sta);
						//sys->work_mod |= ptrProtDat->data.work.option;
					init_sys_sta(ptrProtDat->data.work.sta);
					}
#endif

					break;
				}
			}else
				log_printf("error mechine sta=%d\r\n",ptrProtDat->data.work.go);
			break;
		case DP_CMD_VIOC:
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			log_printf("vioc cmd,option=%d,",ptrProtDat->data.opt.opt1);
			if(ptrProtDat->data.opt.opt1==1 || ptrProtDat->data.opt.opt1==0)
			{
			  if(ptrProtDat->data.opt.opt1==1)		//�ر�����
					ny3p_play(VOICE_DIDI);
			  sys->voice = ptrProtDat->data.opt.opt1;
			  log_printf("%d\r\n",sys->voice);
			}
			log_printf("\r\n");
			if(sys->voice == 0)						//������
				ny3p_play(VOICE_DRIP);
			break;
//--------------------------------------------------------add by wonton2004 20170118
		case DP_CMD_KEY_MANUAL://ң���������̵��ֶ�ģʽ,��������Ҫ���ǰ/��/��/���ߵ���ش���
			all_key_led_off();
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			log_printf("manual mode:%d\r\n",ptrProtDat->data.opt.opt1);
			if(sys->sState != SYS_IDLE || //ֻ���ڿ���ģʽ��
				(sys->sState == SYS_CHARGE && ((sys->charge_sta & AC_CHARGE) != 0)))//�����ڳ�����ϲ����ܷ����̿���
			{
				log_printf("I'm busy now!\r\n");

				break;
			}
				sys->f_work_mode = MWO_MANUAL;
			motor_single_run(ptrProtDat->data.opt.opt1+1);
			break;
		case DP_CMD_KEY_MODE://�������������ֻ��ȡǿ��ģʽ������,��������/ѡ���Ȳ���
				log_printf("DP_CMD_KEY_MODE,opt1=%d,opt2=%d,sta=%d\r\n",ptrProtDat->data.opt.opt1,ptrProtDat->data.opt.opt2,sys->sState );
					if(ptrProtDat->data.opt.opt2 & 0x07)
					{
						sys->work_mod = ptrProtDat->data.opt.opt2;			//ģʽ����
						sys->f_work_mode = ptrProtDat->data.opt.opt2;
					}
					else if(ptrProtDat->data.opt.opt2 & MWO_FULL2GO)
					{
						log_printf("full and go\r\n");
						sys->full2go = 1;
						sys->f_work_mode = ptrProtDat->data.opt.opt2;
					}
					else if(!(ptrProtDat->data.opt.opt2 & MWO_FULL2GO))
					{
						log_printf("no full and go\r\n");
						sys->full2go = 0;
						sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
						break;
					}
			/*
				sys->wifistatechange = 1;
				sys->dis_statechange = 1;
			//	log_printf("force mode:%02x ,doorsill:%02x \r\n",ptrProtDat->data.opt.opt2& MWO_FORCE,ptrProtDat->data.opt.opt2 & MWO_DOORSILL);
				ny3p_play(VOICE_DIDI);
				{
					uint8_t n_modified = 0;
					if(ptrProtDat->data.opt.opt2 & MWO_FORCE)//����ǿ��ģʽ
					{
						if(sys->dust_pwm_value == NORM_DUST_PWM)
						{
							ny3p_play(VOICE_M_SPEED_MODE);
							n_modified = 1;
						}
						sys->dust_pwm_value = FORCE_DUST_PWM;
					}
					else
					{
						if(sys->dust_pwm_value == FORCE_DUST_PWM)
						{
							sys->dust_pwm_value = NORM_DUST_PWM;
							n_modified = 1;
						}
					}
					if(n_modified && ((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS)))// || (sys->sState==SYS_NEAR)
					{
						SET_DST_MOTER(sys->dust_pwm_value);
					}
					//add 2019-03-15 �����ż�ģʽ
					//sys->work_mod |= ptrProtDat->data.opt.opt2 & MWO_DOORSILL;
					sys->work_mod = (sys->work_mod & MWO_MOP) | ( ptrProtDat->data.opt.opt2 & MWO_DOORSILL);
				}
				*/
			break;
		case DP_CMD_KEY_SETSCHEDULE://����ԤԼ����ɹ�����ʾ��
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			log_printf("schedule...%d\r\n",ptrProtDat->data.opt.opt4);
			if(ptrProtDat->data.opt.opt4 == 1)
				ny3p_play(VOICE_M_ORDER);
			else if(ptrProtDat->data.opt.opt4 == 2)
				ny3p_play(VOICE_M_ORDER_OUT);
			else if(ptrProtDat->data.opt.opt4 == 3)
				ny3p_play(VOICE_M_ORDER_SAVE);
			break;
		case DP_CMD_KEY_STATUS://
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			if(sys->sState != SYS_TEST_BD)
				log_printf("DP_CMD_KEY_STATUS:%d-%d-%d",ptrProtDat->data.opt.opt1,
														ptrProtDat->data.opt.opt2,
														ptrProtDat->data.opt.opt3);
			if((ptrProtDat->data.opt.opt1 & 0x01) != 0)
			{
				//uint8_t work_sta;
				//work_sta = WORK_STA_ERR /*| (sys->wifi << WORK_STA_WIFI) */| (sys->silent << WORK_STA_SILENT);//������ add by wonton2004 20170118
//				sys->work_errcode = MWERR_DISPSTA;//�Կض�����ͷ����
				dp_camera_error = 1;
				if(sys->sState != SYS_TEST_BD)
					log_printf("CAMERA NOT FOUND!\r\n");
			}
			else
			{
				dp_camera_error = 0;
			}
			#if 0
			ny3p_play(VOICE_DIDI);
			{
				uint8_t n_modified = 0;
				if(ptrProtDat->data.opt.opt2 & MWO_FORCE)//����ǿ��ģʽ
				{
					if(sys->dust_pwm_value == NORM_DUST_PWM)
					{
						ny3p_play(VOICE_M_SPEED_MODE);
						n_modified = 1;
					}
					sys->dust_pwm_value = FORCE_DUST_PWM;
				}
				else
				{
					if(sys->dust_pwm_value == FORCE_DUST_PWM)
					{
						sys->dust_pwm_value = NORM_DUST_PWM;
						n_modified = 1;
					}
				}
				if(n_modified && ((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS)))// || (sys->sState==SYS_NEAR)
				{
					SET_DST_MOTER(sys->dust_pwm_value);
				}
			}
			#endif
			break;
		case DP_CMD_KEY_SELFTEST:	//�����Լ�
			sys->wifistatechange = 1;
			sys->dis_statechange = 1;
			log_printf("self testing...%d\r\n",ptrProtDat->data.opt.opt4);
			sys->sState = SYS_TEST_SEFT;
			ny3p_play(VOICE_DIDI);
			test_seft_task(0);
					delay_ms(500);
					ny3p_play(VOICE_DIDI);
					delay_ms(500);
					ny3p_play(VOICE_DIDI);
					delay_ms(500);
			break;
		case DP_CMD_KEY_WIFI:		//ң����WIFI����
			//sys->dis_statechange = 1;
			//sys->wifistatechange = 1;

			if(mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)
			{
				sys->wifi_config_state = SMART_CONFIG;
			}
			else if(mcu_get_wifi_work_state() == SMART_CONFIG_STATE)
			{
				sys->wifi_config_state = AP_CONFIG;
			}
			else if(mcu_get_wifi_work_state() == AP_STATE)
			{
				sys->wifi_config_state = SMART_CONFIG;
			}
			else
			{
					sys->wifi_config_state = SMART_CONFIG;
			}
			//mcu_set_wifi_mode(SMART_CONFIG);

			ny3p_play(VOICE_DIDI);
			/*
			if(mcu_get_wifi_work_state() == SMART_CONFIG_STATE)
			{
				n_wifi_work_state = AP_CONFIG;
			}
			*/
			PRINTF("[WIFI]wifi_mode_state:%d, set:%d,wifistate:%d\r\n",mcu_get_wifi_work_state(),sys->wifi_config_state,sys->wifi_state);
#if 0///test
			uint32_t n_wifi_retry;
			uint32_t tmr;

			tmr=msTmr;

			log_printf("start_tmr:%d\r\n",tmr);
			//mcu_set_wifi_mode(sys->wifi_config_state);
			//log_printf("tmr:%d",tmr);
			for(n_wifi_retry = 0;n_wifi_retry < 5000000;n_wifi_retry++)
			{
				mcu_set_wifi_mode(sys->wifi_config_state);
				delay_ms(100);
				wifi_uart_service();
				if(mcu_get_wifimode_flag() == SET_WIFICONFIG_SUCCESS)
				{
					//if( (mcu_get_wifi_work_state()==SMART_CONFIG_STATE && sys->wifi_config_state == SMART_CONFIG) ||
					//	(mcu_get_wifi_work_state()==AP_STATE && sys->wifi_config_state == AP_CONFIG)
					//)
					{
						log_printf("end_tmr:%d,total_tmr:%d,wifi_mode_state:%d\r\n",msTmr,msTmr-tmr,mcu_get_wifi_work_state);
						break;
					}
				}
				else
				{


				}
			}
#else
			uint8_t n_wifi_retry;
			msWifiTmr = 0;
			sys->wifistatechange_tmr = msWifiTmr;
			log_printf("cofig_start_tmr:%d\r\n",msWifiTmr-sys->wifistatechange_tmr);
			for(n_wifi_retry = 0;n_wifi_retry < 100;n_wifi_retry++)
			{
				mcu_set_wifi_mode(sys->wifi_config_state);
				//sys->wifistatechange_tmr = msWifiTmr;
					//delay_ms(100);
				wifi_uart_service();
				if(mcu_get_wifimode_flag() == SET_WIFICONFIG_SUCCESS)
					break;
			}
			log_printf("n_wifi_retry:%d\r\n",n_wifi_retry);

#endif
			break;
//-------------------------------------------------------------------------------end
			case DP_CMD_FAN_MODE:
			{
//				uint8_t n_modified = 0;
				log_printf("DP_CMD_FAN_MODE,opt=%d,sta=%d\r\n",ptrProtDat->data.opt.opt1,sys->sState );
#if 0
				if(ptrProtDat->data.opt.opt1 & MWO_SILENT)
				{
					log_printf("silent mode,\r\n");
					sys->dust_pwm_lev = DUST_MODE_SILENT;
					sys->run_speed = LOW_SPEED;
					if((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS))
					{
						DUST_MOTOR_RANK1();
						log_printf("dust1\r\n");
					}
				}
				else if(ptrProtDat->data.opt.opt1 &  MWO_NORMAL)
				{
					log_printf("normal mode\r\n");
					sys->dust_pwm_lev = DUST_MODE_NORMAL;
					DUST_MOTOR_RANK2();
					sys->run_speed = MIDLE_SPEED;
					if((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS))
					{
						DUST_MOTOR_RANK2();
						log_printf("dust2\r\n");

					}

				}
				else if(ptrProtDat->data.opt.opt1 &  MWO_TURBO)
				{
					log_printf("turbo mode\r\n");
					sys->dust_pwm_lev = DUST_MODE_TURBO;
//					n_modified = 1;
					sys->run_speed = HIGHT_SPEED;
					if((sys->sState ==SYS_NAVIGAT) || (sys->sState==SYS_FOCUS))
					{
						DUST_MOTOR_RANK3();
						log_printf("dust3\r\n");

					}
				}

				sys->full2go = (ptrProtDat->data.opt.opt1 & MWO_FULL2GO)?1:0;
				if(sys->full2go)
				{
					log_printf("full and go...now=%d\r\n",sys->sState);
					if(sys->sState == SYS_NAVIGAT)		//������ɨ״̬�£����س��׮
					{
						init_charge(SYS_NAVIGAT);
					}
				}
#else
					if(ptrProtDat->data.opt.opt1 & 0x07)
					{
						sys->work_mod = ptrProtDat->data.opt.opt1;			//ģʽ����
						sys->f_work_mode = ptrProtDat->data.opt.opt1;
					}
					else if(ptrProtDat->data.opt.opt1 & MWO_FULL2GO)
					{
						log_printf("full and go\r\n");
						sys->full2go = 1;
						sys->f_work_mode = ptrProtDat->data.opt.opt1;
					}
					else if(!(ptrProtDat->data.opt.opt1 & MWO_FULL2GO))
					{
						log_printf("no full and go\r\n");
						sys->full2go = 0;
						sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
						break;
					}
#endif
				log_printf("dust_pwm_lev-----:%d,run speed=%d\r\n",sys->dust_pwm_lev,sys->run_speed);

			}
			break;
			case DP_CMD_KEY_SETTIME:
			{
				log_printf("SETTIME:%02d:%02d:%02d-w%d\r\n",ptrProtDat->data.time.hour,ptrProtDat->data.time.min,ptrProtDat->data.time.sec,ptrProtDat->data.time.week);

				//SetSysTime(ptrProtDat->data.timedata.hour,ptrProtDat->data.timedata.minute,ptrProtDat->data.timedata.week);
			}
			break;
		default:
			break;
	}
	}
	/*
	switch(ptrProtDat->cmd)
	{
		case DP_CMD_SET_TIME:
			SetSysTime(ptrProtDat->data.timedata.hour,ptrProtDat->data.timedata.minute,ptrProtDat->data.timedata.week);
			break;
		case DP_CMD_SET_ORDERTIME:
			if(ptrProtDat->data.timedata.week == 0)
				SetMacStatus(GetMacStatus() & (~MWO_ORDER));
			else
				SetMacStatus(GetMacStatus() | MWO_ORDER);
			ModeLeds();
			break;
		case DP_CMD_SET_STATUS:
			SetMacStatus(ptrProtDat->data.morkstatus.status);
			ModeLeds();
			break;
		case DP_CMD_SET_BTRY:
			TM1638_SetBattery(ptrProtDat->data.battery.bat);
			SetMacBattery(ptrProtDat->data.battery.bat);
			LedSetWordMode(LWM_CHARGE);
			break;
		default:
			break;
	}
	*/
}
#else
#endif
#if 1
void dp_cmd_exec2(DP_PROTDAT *ptrProtDat)
{
	if(sys->sState == SYS_TEST_BD && tm_mode != TM_MODE_BOARD)//��������ģʽ��,ң�������Կ��·�������ָ����������ڲ���
	{
		if(ptrProtDat->cmd == DISP_CMD_REMOTE )
			set_remote_flag(0x02);
		set_remote_flag(0x01);
		ny3p_play(VOICE_START_CHARGE);
		return;
	}
	if(sys->pair_sta == REMOTE_PAIR_ING && ptrProtDat->cmd != DISP_CMD_PAIR_REMOTE)//ң����ƥ��ģʽ,����Ӧ�����Կ�����
		return;
	if(READ_PSW_DET() && (sys->sState== SYS_CHARGE))	//�ػ����״̬��,����Ӧ�Կ�
	{
		ny3p_play(VOICE_START_CHARGE);
		return;
	}
	switch(ptrProtDat->cmd)
	{
		case DISP_CMD_REMOTE:		//ң����
			log_printf("DISP_CMD_REMOTE:");
			{
				uint8_t remote_key = ptrProtDat->data[0];//��ȡ�������ֶ�
				log_printf("%d\r\n",remote_key);
				if(sys->power_on == 0 && remote_key != REMOTE_CODE_POWER)
				{
					log_printf("power_off,please press power key\r\n");
					return;
				}
				switch(remote_key)
				{
					case REMOTE_CODE_POWER:
						log_printf("_POWER\r\n");

							/*
						if( ptrProtDat->data.powerstatus.power ==0 || ptrProtDat->data.powerstatus.power == 1)
						{
							sys->power_on = ptrProtDat->data.powerstatus.power ;
							log_printf("power_on=%d\r\n",sys->power_on);
						}
						else
						{
							log_printf("power on error\r\n");
						}
						*/
						//�ֳ����ԣ�����Ϩ����Ļ
						//log_printf("power_on=%d\r\n",ptrProtDat->data.powerstatus.power);
						if(sys->sState != SYS_IDLE && sys->sState != SYS_CHARGE)		//�ǿ���״̬�²���Ӧ��Դ��,��Ϊ�������
						{
							log_printf("no idle no charge,no action\r\n");
							break;
						}
						if(sys->power_on)
						{
						    log_printf("all_key_led_off(1)\r\n");
							all_key_led_off();
							sys->power_on = 0;
							sys->wifi_led_enable = 0;
						}
						else
						{
							sys->power_on = 1;
							sys->wifi_led_enable = 1;
							if(sys->sState == SYS_IDLE)
							{
								dp_led_light_contrl(2,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);	//����
							}
						}
						//����״̬,����Ӧ����BUG,������Գ���BUG,���п��оͷֿ������
						if(sys->sState == SYS_CHARGE)	//������ģʽ��,ǿ����Ļ��������,�����û�йػ�״̬
						{
							sys->power_on = 1;
							if(msChargeTmr>TMR_CHARGEIDLE_INTO_LOWPOWER)	//�����̬��,Ϣ���Ļ��ͻ���,
							{
								log_printf("wake up led\r\n");
								dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);	//����
							}
							else
							{
								log_printf("led into sleep\r\n");
								msChargeTmr = TMR_CHARGEIDLE_INTO_LOWPOWER+1;	//���������Ϊ��ʵ������led״̬��ʼ��,�������ɲ���led״̬����,һֱ����
								dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);	//����
								msChargeTmr = TMR_CHARGEIDLE_INTO_LOWPOWER+1;	//���ģʽ�����Ϣ��
							}
						}
						ny3p_play(VOICE_SELECT_M);
						//�����ֵ���
						sys->wifistatechange = 1;
						sys->dis_statechange = 1;
					break;
					case REMOTE_CODE_UP:
						log_printf("_UP\r\n");

						sys->wifistatechange = 1;
						sys->dis_statechange = 1;
						if(sys->sState != SYS_IDLE || //ֻ���ڿ���ģʽ��
							(sys->sState == SYS_CHARGE && ((sys->charge_sta & AC_CHARGE) != 0)))//�����ڳ�����ϲ����ܷ����̿�����������Կ���
						{
							log_printf("I'm busy now!\r\n");
							break;
						}
						all_key_led_off();
						if(sys->prs<3)
						{
							log_printf("low power!\r\n");
							break;
						}
						sys->f_work_mode = MWO_MANUAL;
						motor_single_run(GO_FORWARD);
					break;
					case REMOTE_CODE_LEFT:
						log_printf("_LEFT\r\n");
						sys->wifistatechange = 1;
						sys->dis_statechange = 1;
						if(sys->sState != SYS_IDLE || //ֻ���ڿ���ģʽ��
							(sys->sState == SYS_CHARGE && ((sys->charge_sta & AC_CHARGE) != 0)))//�����ڳ�����ϲ����ܷ����̿���
						{
							log_printf("I'm busy now!\r\n");
							break;
						}
						all_key_led_off();
						if(sys->prs<3)
						{
							log_printf("low power!\r\n");
							break;
						}
						sys->f_work_mode = MWO_MANUAL;
						motor_single_run(GO_LEFT);
					break;
					case REMOTE_CODE_RIGHT:
						log_printf("_RIGHT\r\n");

						sys->wifistatechange = 1;
						sys->dis_statechange = 1;
						if(sys->sState != SYS_IDLE || //ֻ���ڿ���ģʽ��
							(sys->sState == SYS_CHARGE && ((sys->charge_sta & AC_CHARGE) != 0)))//�����ڳ�����ϲ����ܷ����̿���
						{
							log_printf("I'm busy now!\r\n");
							break;
						}
						all_key_led_off();
						if(sys->prs<3)
						{
							log_printf("low power!\r\n");
							break;
						}
						sys->f_work_mode = MWO_MANUAL;
						motor_single_run(GO_RIGTH);
					break;
					case REMOTE_CODE_DOWN:
						log_printf("_DOWN\r\n");

						sys->wifistatechange = 1;
						sys->dis_statechange = 1;
						if(sys->sState != SYS_IDLE || //ֻ���ڿ���ģʽ��
							(sys->sState == SYS_CHARGE && ((sys->charge_sta & AC_CHARGE) != 0)))//�����ڳ�����ϲ����ܷ����̿���
						{
							log_printf("I'm busy now!\r\n");
							break;
						}
						all_key_led_off();
						if(sys->prs<3)
						{
							log_printf("low power!\r\n");
							break;
						}
						sys->f_work_mode = MWO_MANUAL;
						motor_single_run(GO_BACK);
					break;
					case REMOTE_CODE_OK:
						log_printf("_OK\r\n");
						if(sys->power_on == 0)
							break;
#if 1
					if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
					{
					ny3p_play(VOICE_DIDI);
					}
					else
						ny3p_play(VOICE_BEE_SHORT);
					if(sys->sState == SYS_CHARGE)	//������ģʽ��,ǿ����Ļ��������,�����û�йػ�״̬
					{
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
					}
					  sys->t_beep_on = 0;
					 // sys->power_on = 1;
				   if(sys->power_on == 0)
				   {
					   sys->power_on = 1;
					   ny3p_play(VOICE_SELECT_M);
					   init_sys_sta(SYS_IDLE);
					   log_printf("work_errcode:%04\r\n ",sys->work_errcode );
				   }
				   else
				   {
					   if(EXTERAL_AC_DETECT())
					   {
						   log_printf("EXTERAL_AC_DETECT,out\r\n");
						   break;
						   //return 0;
					   }
					   /*
					   if(sys->sState ==  SYS_DOCK)
					   {
						   log_printf("find dock now,please stop dock first\r\n");
						   break;
					   }
					   */
					   motor_run(GO_STOP,0,0,0);
					   if(sys->sState == SYS_NAVIGAT  )
						{
						   log_printf("key4 %d..\r\n",sys->sState);
							all_key_led_off();
						  //  LED1_OFF();
						   sys->sState = SYS_IDLE;
						   if(sys->spot_flag == 0)
						   {
							   sys->t_halt_on = 1;
							   sys->t_flash_halt = 600000;	   //10����
						   }
						   LED3_OFF();
						   init_sys_sta(SYS_IDLE);
						   STOP_ALL_MOTOR();
						   //delay_sensor(2000);
						}
						else //if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
						{
							all_key_led_off();
						   log_printf("key0 %d..\r\n",sys->sState);
						   //LED1_ON();
						   LED3_ON();
						   sys->gey_key_clear_err = 1;
						   init_sys_sta(SYS_NAVIGAT);
						   //ny3p_play(VOICE_M_NAVI);
						   delay_ms(500);
						}
				   }

#else
						ny3p_play(VOICE_SELECT_M);
						all_key_led_off();
						sys->t_beep_on = 0;
						//sys->power_on = 1;
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
						if(sys->power_on == 0)
						{
							sys->power_on = 1;
							//ny3p_play(VOICE_SELECT_M);
							init_sys_sta(SYS_IDLE);
							log_printf("work_errcode:%04x\r\n ",sys->work_errcode );
						}
						else
						{
							if(EXTERAL_AC_DETECT())
							{
								log_printf("EXTERAL_AC_DETECT,out\r\n");
								//return 0;
								break;
							}
							if(sys->sState == SYS_NAVIGAT || sys->sState == SYS_FOCUS || sys->sState == SYS_RANDOM
							    || sys->sState ==  SYS_DOCK  || sys->sState ==  SYS_NEAR)
							{
								log_printf("sta %d..\r\n",sys->sState);
								if(sys->spot_flag == 0)
								{
									sys->t_halt_on = 1;
									sys->t_flash_halt = 600000;		//10����
								}
								sys->sState = SYS_IDLE;
								LED3_OFF();
								init_sys_sta(SYS_IDLE);
								STOP_ALL_MOTOR();
							}
							else if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
							{
								log_printf("key0 %d..\r\n",sys->sState);
								//LED1_ON();
								LED3_ON();
								init_sys_sta(SYS_NAVIGAT);
								//ny3p_play(VOICE_M_NAVI);
								delay_ms(500);
							}
						}
#endif
					break;
					case REMOTE_CODE_DOCK:
						log_printf("_DOCK\r\n");
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
						{
							ny3p_play(VOICE_DIDI);
						}
						else
						ny3p_play(VOICE_BEE_SHORT);

			#if 1
						all_key_led_off();
						//dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
						sys->t_beep_on = 0;
						if(sys->sState == SYS_CHARGE)	//������ģʽ��,ǿ����Ļ��������,�����û�йػ�״̬
						{
							dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
						}
						/*
						if(sys->sState == SYS_NAVIGAT)
						{
							log_printf("clean now,please stop fisrt\r\n");
							break;
						}
						*/
						if(sys->sState == SYS_DOCK || sys->sState == SYS_DOCK_NEAR)
						{
						   // LED2_OFF();
							 LED4_OFF();
							log_printf("key charge1,%d..\r\n",sys->sState);
							init_sys_sta(SYS_IDLE);
							//init_sys_status(sys->sState,SYS_FOCUS);
							//STOP_ALL_MOTOR();
							break;
						}
						else if(sys->sState == SYS_CHARGE)
						{
							log_printf("key charge2,%d..\r\n",sys->sState);
							break;
						}
						else
						{
							all_key_led_off();
							// LED2_ON();
							  LED4_ON();
							  sys->gey_key_clear_err = 1;
							 log_printf("key charge3,%d..\r\n",sys->sState);
							init_sys_sta(SYS_DOCK);
						}

			#else
						{
							all_key_led_off();
							dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
							sys->t_beep_on = 0;
							if(sys->sState == SYS_DOCK)
							{
								// LED2_OFF();
								LED4_OFF();
								log_printf("key charge1,%d..\r\n",sys->sState);
								init_sys_sta(SYS_IDLE);
								//init_sys_status(sys->sState,SYS_FOCUS);
								//STOP_ALL_MOTOR();
								break;
							}else if(sys->sState == SYS_CHARGE)
							{
								log_printf("key charge2,%d..\r\n",sys->sState);
								break;
							}
							else
							{
								// LED2_ON();
								LED4_ON();
								log_printf("key charge3,%d..\r\n",sys->sState);
								init_sys_sta(SYS_DOCK);
							}
						}
		#endif
					break;
					case REMOTE_CODE_SCHDL:
						log_printf("_SCHDL\r\n");
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
						{
							ny3p_play(VOICE_DIDI);
						}
						else
						ny3p_play(VOICE_BEE_SHORT);
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
					break;
					case REMOTE_CODE_FULL_GO:
						log_printf("_FULL_GO\r\n");
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
						{
							ny3p_play(VOICE_DIDI);
						}
						else
						ny3p_play(VOICE_BEE_SHORT);
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
		#if 0           //2020 05 06 ��ͻ�Ҫ��ر�   full2go
						if(sys->sState == SYS_IDLE)
						{
							log_printf("sysidle set full2go no no no..\r\n");
							//����״̬��ֻ����ȡ��,����������
							if(sys->full2go)
							{
								log_printf("no full and go\r\n");
								sys->full2go = 0;
								sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
							}
							break;
						}
		#endif
						if(sys->full2go == 0)
						{
							log_printf("full and go\r\n");
							sys->full2go = 1;
							sys->f_work_mode = MWO_FULL2GO;
						//����ֻ���ڳ�����ɨģʽ�²���Ӧfull2go����,1,���ʹ�ܳ�����ʼ��ɨ,2,��ɨ״ֱ̬�ӽ���س�״̬.
							if(sys->sState == SYS_CHARGE)
							{
								log_printf("charge now,wait charge done\r\n");
								break;
							}
							// LED2_ON();
							if((sys->sState != SYS_DOCK))
							{
								all_key_led_off();
								LED4_ON();
								log_printf("key charge3,%d..\r\n",sys->sState);
								init_sys_sta(SYS_DOCK);
							}
						}
						else
						{
							log_printf("no full and go\r\n");
							sys->full2go = 0;
							sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
							break;
						}
					break;
					case REMOTE_CODE_MODE:
						log_printf("_MODE\r\n");
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
							ny3p_play(VOICE_DIDI);
						else
						ny3p_play(VOICE_BEE_SHORT);
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
						/*
						if(ptrProtDat->data.opt.opt2 & 0x07)
						{
							sys->work_mod = ptrProtDat->data.opt.opt2;			//ģʽ����
							sys->f_work_mode = ptrProtDat->data.opt.opt2;
						}
						else if(ptrProtDat->data.opt.opt2 & MWO_FULL2GO)
						{
							log_printf("full and go\r\n");
							sys->full2go = 1;
							sys->f_work_mode = ptrProtDat->data.opt.opt2;
						}
						else if(!(ptrProtDat->data.opt.opt2 & MWO_FULL2GO))
						{
							log_printf("no full and go\r\n");
							sys->full2go = 0;
							sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
							break;
						}
						*/
						sys->full2go = 0;
						switch(sys->work_mod)
						{
							case MWO_SILENT:
								sys->f_work_mode = sys->work_mod = MWO_NORMAL;
							break;
							case MWO_NORMAL:
								sys->f_work_mode = sys->work_mod = MWO_TURBO;
							break;
							case MWO_TURBO:
								sys->f_work_mode = sys->work_mod = MWO_SILENT;
							break;
							default:
							break;
						}
						//sys->f_work_mode = work_mode;	//
						/*
						if(ptrProtDat->data.opt.opt2 & 0x07)
						{
							sys->work_mod = ptrProtDat->data.opt.opt2;			//ģʽ����
							sys->f_work_mode = ptrProtDat->data.opt.opt2;
						}
						else if(ptrProtDat->data.opt.opt2 & MWO_FULL2GO)
						{
							log_printf("full and go\r\n");
							sys->full2go = 1;
							sys->f_work_mode = ptrProtDat->data.opt.opt2;
						}
						else if(!(ptrProtDat->data.opt.opt2 & MWO_FULL2GO))
						{
							log_printf("no full and go\r\n");
							sys->full2go = 0;
							sys->f_work_mode = sys->work_mod;		//full2goң�����ָ�
							break;
						}
						*/
					break;
					case REMOTE_CODE_TIM:
						log_printf("_TIM\r\n");
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
							ny3p_play(VOICE_DIDI);
						else
						ny3p_play(VOICE_BEE_SHORT);
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
					break;
					case REMOTE_CODE_FOCUS:
						log_printf("_SPOT\r\n");
						all_key_led_off();
						if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
						{
						ny3p_play(VOICE_DIDI);
						}
						else
							ny3p_play(VOICE_BEE_SHORT);
						dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
			#if 1
					   sys->t_beep_on = 0;
					   if(sys->sState == SYS_CHARGE)   //������ģʽ��,ǿ����Ļ��������,�����û�йػ�״̬
					   {
						   dp_led_light_contrl(0,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
					   }
					   LED3_OFF();
					   LED4_OFF();
					   log_printf("--------------------------\r\n");
					  // sys->power_on = 1;
					   if (sys->spot_flag++ >=1)
					   {
						   sys->spot_flag=0;
						   log_printf("key spot_flag 11 %d,%d..\r\n",sys->sState,sys->spot_flag);
					   }
					   if(1 == sys->spot_flag)
					   {
						   log_printf("key spot_flag 12 %d,%d..\r\n",sys->sState,sys->spot_flag);
						   // init_focus_task();
						   // sys->sState  = SYS_FOCUS;
						   //sys->sState_auxi = 0;
						   //proc_focus_task();
						   // sys->power_go = 1;
						   //ny3p_play(VOICE_M_CLN_HEART);
						   //init_focus_task();
						   //s->sState = SYS_FOCUS;
						   //init_sys_status(SYS_FOCUS,SYS_IDLE);
						   //init_sys_status(SYS_FOCUS,sys->sState);
						   if(sys->sState == SYS_NAVIGAT)
						   {
							   log_printf("navi stop...\r\n");
							   motor_run(GO_STOP,0,0,0);
							   delay_ms(200);
						   }
						   if(sys->sState == SYS_CHARGE)//���״̬����������ɨ
						   {
							   log_printf("charge...out\r\n");
							   break;
						   }
							//LED3_ON();
						   LED1_ON();
						   sys->gey_key_clear_err = 1;
						   sys->sState = SYS_FOCUS;
						   sys->f_work_mode = MWO_SPOT;
						   log_printf("set f_work_mode:%d\r\n",sys->f_work_mode);
						   log_printf("key12  focus  %d..\r\n",sys->sState);
						   //if(sys->sState != SYS_IDLE)
						   //sys->sState = SYS_FOCUS;
						   break;
					   }
					   if(0 == sys->spot_flag)
					   {
					  // if (sys->sState  != SYS_FOCUS)
							{
							 //LED3_OFF();
							   LED1_OFF();
							   sys->sState	= SYS_IDLE;
							   log_printf("key spot_flag 13 %d,%d..\r\n",sys->sState,sys->spot_flag);
							   if(sys->full2go)
								   sys->f_work_mode = MWO_FULL2GO;
							   else
								   sys->f_work_mode = sys->work_mod;	   //������ɨģʽ �����ָ�
							   log_printf("set f_work_mode:%d\r\n",sys->f_work_mode);
							}
					   }
				  // break;
		 #else
						sys->t_beep_on = 0;
						log_printf("--------------------------\r\n");
						//sys->power_on = 1;
						if(sys->sState == SYS_CHARGE)
						{
							log_printf("charge now,dont spot clean\r\n");
							sys->spot_flag=0;
							break;
						}
						if (1 <= sys->spot_flag++)
						{
							sys->spot_flag=0;
							log_printf("key spot_flag 11 %d,%d..\r\n",sys->sState,sys->spot_flag);
						}
	                    if(1 == sys->spot_flag)
	                    {
						    LED1_ON();
						    log_printf("key spot_flag 12 %d,%d..\r\n",sys->sState,sys->spot_flag);
						    // init_focus_task();
						    // sys->sState  = SYS_FOCUS;
						  	//sys->sState_auxi = 0;
					        //proc_focus_task();
						    // sys->power_go = 1;
						    //ny3p_play(VOICE_M_CLN_HEART);
							//init_focus_task();
							//s->sState = SYS_FOCUS;
							//init_sys_status(SYS_FOCUS,SYS_IDLE);
	                        //init_sys_status(SYS_FOCUS,sys->sState);
							if(sys->sState == SYS_NAVIGAT)
							{
								log_printf("navi stop...\r\n");
								motor_run(GO_STOP,0,0,0);
								delay_ms(200);
							}
							if(sys->sState == SYS_CHARGE)
							{
								log_printf("charge...out\r\n");
								LED1_OFF();
								break;
							}
	                      	sys->sState = SYS_FOCUS;
	                      	sys->f_work_mode = MWO_SPOT;
	                      	log_printf("set f_work_mode:%d\r\n",sys->f_work_mode);
				            log_printf("key12  focus  %d..\r\n",sys->sState);
							break;
				        }
	                   	if(0 == sys->spot_flag)
						{
							// if (sys->sState  != SYS_FOCUS)
							{
								LED1_OFF();
								sys->sState  = SYS_IDLE;
								log_printf("key spot_flag 13 %d,%d..\r\n",sys->sState,sys->spot_flag);
								if(sys->full2go)
									sys->f_work_mode = MWO_FULL2GO;
								else
								sys->f_work_mode = sys->work_mod;		//������ɨģʽ �����ָ�
								log_printf("set f_work_mode:%d\r\n",sys->f_work_mode);
							}
						}
		#endif
					break;
					default:
					break;
				}
			}
		break;
		case DISP_CMD_SCHEDULE_CLEAN:
			log_printf("DISP_CMD_SCHEDULE_CLEAN\r\n");
			{
				uint8_t work_mode = ptrProtDat->data[0];//��ȡ�������ֶ�
				if(work_mode == 0)
					work_mode = MWO_NORMAL;
				sys->work_mod = work_mode;		//ԤԼ������,����ԤԼ��ģʽ,��������,ɾ�����д���
				sys->f_work_mode = work_mode;	//
				all_key_led_off();
				sys->t_beep_on = 0;
				sys->power_on = 1;
				dp_led_light_contrl(2,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
				if(sys->power_on == 0)
				{
					sys->power_on = 1;
					ny3p_play(VOICE_SELECT_M);
					init_sys_sta(SYS_IDLE);
					log_printf("work_errcode:%04x\r\n ",sys->work_errcode );
				}
				else
				{
					if(EXTERAL_AC_DETECT())
					{
						log_printf("EXTERAL_AC_DETECT,out\r\n");
						//return 0;
						break;
					}
					if(sys->sState == SYS_NAVIGAT || sys->sState == SYS_FOCUS || sys->sState == SYS_RANDOM
					    || sys->sState ==  SYS_DOCK  || sys->sState ==  SYS_NEAR)
					{
						#if 0
						log_printf("sta %d..\r\n",sys->sState);
						if(sys->spot_flag == 0)
						{
							sys->t_halt_on = 1;
							sys->t_flash_halt = 600000;		//10����
						}
						sys->sState = SYS_IDLE;
						LED3_OFF();
						init_sys_sta(SYS_IDLE);
						STOP_ALL_MOTOR();
						#else
						log_printf("clean now..dont action\r\n");
						break;
						#endif
					}
					else if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
					{
						log_printf("key0 %d..\r\n",sys->sState);
						//LED1_ON();
						LED3_ON();
						sys->gey_key_clear_err = 1;
						init_sys_sta(SYS_NAVIGAT);
						delay_ms(500);
					}
				}
			}
		break;
		case DISP_CMD_SET_TIME:			//����RTC,
			log_printf("DISP_CMD_SET_TIME:\r\n");
			if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
				ny3p_play(VOICE_DIDI);
			else
			ny3p_play(VOICE_BEE_SHORT);
			dp_led_light_contrl(2,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
			{	//����RTC,������Ҫ�ϱ����Կ�
				time_t *t;
				struct tm set_tm;
				t = (time_t *)&ptrProtDat->data[0];
				set_tm = Time_ConvUnixToCalendar(*t);
				log_printf("%02d:%02d:%02d\r\n",set_tm.tm_hour,set_tm.tm_min,set_tm.tm_sec);
				Time_SetUnixTime(*t);
			}
		break;
		case DISP_CMD_SET_SCHEDULE_TIME:
			log_printf("DISP_CMD_SET_SCHEDULE_TIME\r\n");
			if(sys->sState == SYS_IDLE || sys->sState == SYS_CHARGE)
				ny3p_play(VOICE_DIDI);
			else
			ny3p_play(VOICE_BEE_SHORT);
			dp_led_light_contrl(2,sys->sState,0,sys->charge_sta,sys->prs,sys->work_errcode,sys->wifi_state);
			{//ԤԼʱ��
				uint8_t week=0;
				T_SCH_DATA *sch = (T_SCH_DATA *)ptrProtDat->data;
				for(week=0;week<7;week++)
				{
					if(sch->week_en & BIT8_STA(week))
					{
						if(((sch->hour[week]>>5)&0x03) == 0x02)	//D6D5 �Կؾ���02�ĳ� ���徲��00
						{
							sch->hour[week] &= 0x9f; 				//00
						}
						else if(((sch->hour[week]>>5)&0x03) == 0x00)//D6D5 �Կ�����00 �ĳ� ��������01
						{
							sch->hour[week] |= 0x20;
						}
						else if(((sch->hour[week]>>5)&0x03) == 0x01)//D6D5 �Կ�ǿ��01 �ĳ� ����ǿ��02
						{
							sch->hour[week] &= 0x9f;
							sch->hour[week] |= 0x40;
						}
						log_printf("week_%d: mode%02d_%02d:%02d\r\n",week,((sch->hour[week]>>5)&0x03),(sch->hour[week]&0x1f),sch->min[week]);
					//��WIFI,û��ģʽ,����WIFI������ͨ���Կ����õ�ԤԼģʽ
					}
				}
				dp_schdata = *sch;
			}
		break;
		case DISP_CMD_PAIR_REMOTE:
			{	//ң�����������������,����01���Կ�֪ͨ���,���Կ��·�02��ʾ��Գɹ�,�ɹ����ϱ�00,�˳����ģʽ.
				uint8_t pair_sta = ptrProtDat->data[0];
				log_printf("DISP_CMD_PAIR_REMOTE:%d\r\n",pair_sta);
				switch(pair_sta)
				{
					case REMOTE_PAIR_ING:
						if(sys->pair_sta == REMOTE_PAIR_ING)
							log_printf("dp_pair_remote_ing\r\n");
						else
							log_printf("dp_pair_remote_ing err pair_sta:%d\r\n",sys->pair_sta);
					break;
					case REMOTE_PAIR_OK:
						if(sys->pair_sta == REMOTE_PAIR_ING)
						{
						log_printf("REMOTE_PAIR_OK\r\n");
							sys->pair_sta = REMOTE_PAIR_OK;
						}
						else
							log_printf("dp_pair_remote err pair_sta:%d\r\n",sys->pair_sta);
					break;
					default:
					break;
				}
			}
		break;
		default:
		break;
	}
}
#endif
/*
#define DP_PROTHEAD_1	0xaa
#define DP_PROTHEAD_2	0X99

//����Э���������
uint8_t pd_buff[32];
void dp_rx_task(void)
{
	int len;
	DP_PROTDAT *ptrProtDat
	int i;
	if(d_len >=9 && d_len <31)
	{
		len = d_len;
		memcpy((char *)&pd_buff,(char *)len,d_len);
		d_len = 0;

		for(i=0;i<len;i++)
		{
			if(pd_buff[i] == DP_PROTHEAD_1  && pd_buff[i+1] == DP_PROTHEAD_2)
				break;
		}
		if(i < len)
		{
			ptrProtDat = (DP_PROTDAT *)&pd_buff[i] ;
			if(ptrProtDat->chksum != dp_checksum(ptrProtDat))
			{
				log_printf("[dp_commrxtask]chk sum error\r\n");
				return;
			}
		}

	}
}

*/


void dp_commrxtask(void)
{
	DP_PROTDAT *ptrProtDat = &dp_protdattx;
	uint8_t *ptrData = (uint8_t *)ptrProtDat;
	uint8_t datahasread = 0;
	static uint8_t datact = 0;//��¼�Ѷ����ݵ��±�
	static uint8_t waitfortail = 0;//���ֻ���˹�������,��ֵ��1,ֱ������һ��������

	uint8_t len = 0;
	uint8_t get_frame_flag = 0;
	do{
		if(!waitfortail)//û��δ���������,�Ӱ�ͷʶ�������
		{
			datahasread = usart4_read(ptrData + datact,1);
		//	log_printf("%02X ",*(ptrData + datact));
			if(datahasread)
			{
				if(sys->sState != SYS_TEST_BD)
					log_printf("[dp_commrxtask]---1:%d,%02x\r\n",datahasread,ptrData[datact]);
				//else
				//	delay_ms(5);
			}
			if(datahasread == 1 && ptrData[datact] == DP_PROTHEAD_1)
			{
				if(sys->sState != SYS_TEST_BD)
					log_printf("[dp_commrxtask]---1\r\n");
				//else
				//	delay_ms(5);
				datact ++;
				datahasread = usart4_read(ptrData + datact,1);
				if((datahasread == 1) &&  (ptrData[datact] == DP_PROTHEAD_2))
				{
					//
					datact ++;
					//delay_ms(5);
					datahasread = usart4_read(ptrData + datact,1);
					len=ptrData[datact];
					datact ++;
					waitfortail = 1;
					if(sys->sState != SYS_TEST_BD)
						log_printf("[dp_commrxtask]get head\r\n");
					//else
					//	delay_ms(5);
				}
				else
					datact = 0;
			}
			else
			{
				datact = 0;
			}
		}
		else//����ж�������ͷ,����Ҫ��������ʣ�µ��ֽ�,���һ�������İ�
		{
			//datahasread = usart4_read(ptrData + datact,DP_PROT_COMMLENGTH - datact);
			datahasread = usart4_read(ptrData + datact,(len+2)); //���ϰ�β6d 7d����

			datact += datahasread;	//��ȡ��֡����
			if(ptrData[datact-1] == DP_PROTEND_1 && ptrData[datact-2] == DP_PROTEND_2)
			{
				if(sys->sState != SYS_TEST_BD)
				{
//					int i;
					//log_printf("rx:");

					//for(i=0;i<datact;i++)
					//	log_printf("%02X  ",*(ptrData + i));
					//log_printf("\r\n");
				}
				//else
				//	delay_ms(10);
				get_frame_flag = 1;
				break;
			}
		}
	}while(datahasread);

	//if(datact < DP_PROT_COMMLENGTH)//���ݰ�������,����,�ȴ���һ�ֽ���
	if(get_frame_flag==0)
		return;
	get_frame_flag = 0;
	datact = 0;
	waitfortail = 0;
	//if(ptrProtDat->chksum != dp_checksum(ptrProtDat))
	if(ptrProtDat->chksum != dp_checksum2(&ptrProtDat->cmd,(len-1)))
	{
		if(sys->sState != SYS_TEST_BD)
			log_printf("[dp_commrxtask]chk sum error\r\n");
		//else
		//	delay_ms(5);

		return;
	}

	if(sys->sState != SYS_TEST_BD)
	{
		log_printf("[dp_commrxtask]");
		for(datahasread = 0;datahasread < (len+5);datahasread ++)
		{
			log_printf("%02x ",ptrData[datahasread]);
		}
		log_printf("\r\n");
	}

	//dp_cmd_exec(ptrProtDat);
	dp_cmd_exec2(ptrProtDat);
	sys->t_idle =0;
}


void dp_tx_data(uint8_t msg,uint8_t pack_len,uint8_t *data)
{
	uint8_t send_buff[32]={0};
	uint8_t i=0,sum=0;
	send_buff[0] = 0x7e;
	send_buff[1] = 0x5d;
	send_buff[2] = pack_len+2;
	send_buff[4] = msg;
	sum = msg;
	for(i=0;i<pack_len;i++)
	{
		send_buff[5+i] = data[i];
		sum+=data[i];
	}
	send_buff[3] = sum;	//chk
	send_buff[5+pack_len] = 0x6d;
	send_buff[5+pack_len+1] = 0x7d;
	usart4_write(send_buff, (5+pack_len+2));
}
void set_led_sta(uint8_t *led_action,uint16_t *led_keep_tmr,uint8_t led_n,uint8_t sta,uint16_t k_tmr)
{
	uint8_t move_flag = 0;
	uint8_t byte_half = 0;
	byte_half = led_n/2;
	if((led_n+2)%2)
		move_flag = 1;
	if(move_flag)
		led_action[byte_half] &= 0x0f;//����4λ��λ
	else
		led_action[byte_half] &= 0xf0;//����4λ��λ
	led_action[byte_half] |= (move_flag?(sta<<4):sta);
	led_keep_tmr[led_n] = k_tmr;
}
uint8_t get_led_sta(uint8_t *led_action,uint8_t led_n)
{
	uint8_t move_flag = 0;
	uint8_t byte_half = 0;
	uint8_t sta = 0;
	byte_half = led_n/2;
	if((led_n+2)%2)
		move_flag = 1;
	if(move_flag)
		sta = (led_action[byte_half] >>4) & 0x0f;//ȥ��4λ
	else
		sta = (led_action[byte_half] & 0x0f);	//ȥ��4λ
	return sta;
}
//״̬Ϊ��/��(����˸)/״̬δ�����仯��,ͳһͨ��02�����Կ�
//״̬Ϊ��˸�ĵ�,��Ҫͨ��04����ͬ����˸�Ʒ��͸��Կ�
void dp_led_light_contrl(uint8_t type,uint8_t sSta,uint8_t work_mode,uint8_t change_sta,uint8_t prs,uint16_t work_errcode,uint8_t wifi_state)
{
	static uint8_t s_power_on = 0;
	static uint8_t s_work_mode=0,s_wifi_sta=0xff,s_change_sta = 0,s_prs=0,s_charg_prs = 0,s_dustfull=0xff,s_led_888=0;
	static uint16_t s_work_err=0xffff;
	static uint8_t nopowercharge = 0,charge_idle_flag=0;	//��糬��3min,������ȫ���־
	static uint16_t last_led_bty_tmr = 0;
	static uint8_t s_led_action[LEN_CMD_ALL_LED_LIGHT]={LED_UNKNOW};
	static uint16_t s_led_keep_tmr[LED_CONTRAL_NUB]={KEEP_TMR_0_S};
	uint16_t i=0;
	uint8_t dustfull=0;
	uint8_t led_action[LEN_CMD_ALL_LED_LIGHT]={LED_UNKNOW};
	uint16_t led_keep_tmr[LED_CONTRAL_NUB] = {KEEP_TMR_0_S};
	uint8_t send_en =0;	//
//ģʽ��---------״̬Ϊ��/��(����˸)/״̬δ�����仯��,ͳһͨ��02�����Կ�
	if(type != 1 )
	{
		if(msChargeTmr>TMR_CHARGEIDLE_INTO_LOWPOWER || type == 2)	//�����˲Żָ�,����ˢ��
		{
			sys->wifi_led_enable = 1;
			log_printf("wake up...,update all led\r\n");
			s_power_on = 0;
			s_work_mode=0,s_wifi_sta=0xff,s_change_sta = 0,s_prs=0,s_charg_prs = 0,s_dustfull=0xff,s_led_888=0;
			s_work_err=0xffff;
			charge_idle_flag = 0;
			nopowercharge = 0;
			last_led_bty_tmr = 0;
			for(i=0;i<LEN_CMD_ALL_LED_LIGHT;i++)
				s_led_action[i] = LED_UNKNOW;
			for(i=0;i<LED_CONTRAL_NUB;i++)
				s_led_keep_tmr[i] = KEEP_TMR_0_S;
		}
		msChargeTmr = 0;
		return;
	}
	//�ػ����״̬��,ֻ���Ƶ�ص�
	if(READ_PSW_DET() && (sSta == SYS_CHARGE))
	{
		/////ֻ���Ƶ�ص�
		if(nopowercharge == 0)		/// ������ȫ���־
		{
			sys->wifi_led_enable = 0;//wifi ledʹ����0
			ny3p_play(VOICE_DIDI);
			send_en = 1;
			log_printf("READ_PSW_DET, close all light\r\n");
			nopowercharge = 1;		//������ȫ��
			set_led_sta(led_action,led_keep_tmr,STAT_LED_ALL,LED_ON,KEEP_TMR_0_S);
			s_work_mode = 0;
			s_work_err = 0;
			s_wifi_sta = 0;
			s_dustfull = 0;
			s_led_888 = 0;
			charge_idle_flag=0;
			set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_OFF,KEEP_TMR_0_S);
			set_led_sta(led_action,led_keep_tmr,STAT_LED_NA,LED_UNACTION,KEEP_TMR_0_S);
			///....1
			if(s_change_sta != change_sta || s_charg_prs != prs)
			{
				s_change_sta = change_sta;
				s_charg_prs = prs;
				s_prs = 0;
				if(change_sta & CHARG_DONE) //����ǳ�����
				{
					last_led_bty_tmr = KEEP_TMR_120_S;
					set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_120_S); ///���������д2min
				}
				else// if(change_sta & AC_CHARGE || change_sta & DOCK_CHARGE)	//���ڳ����
				{
					last_led_bty_tmr = KEEP_TMR_0_S;
					if(prs<LOW_POW_PRS_TWS)	//�͵�ѹ
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
					else
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_0_5HZ,KEEP_TMR_0_S);
				}
			}
			else
				set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_UNACTION,last_led_bty_tmr);
		}
		//ֻ���Ƴ��������Ʊ���
		else
		{
			msChargeTmr = 0;
			///....2
			if(s_change_sta != change_sta || s_charg_prs != prs)
			{
				send_en = 1;
				s_change_sta = change_sta;
				s_charg_prs = prs;
				s_prs = 0;
				if(change_sta & CHARG_DONE) //����ǳ�����
				{
					last_led_bty_tmr = KEEP_TMR_120_S;
					set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_120_S); ///���������д2min
				}
				else// if(change_sta & AC_CHARGE || change_sta & DOCK_CHARGE)	//���ڳ����
				{
					last_led_bty_tmr = KEEP_TMR_0_S;
					if(prs<LOW_POW_PRS_TWS)	//�͵�ѹ
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
					else
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_0_5HZ,KEEP_TMR_0_S);
				}
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_UNACTION,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,STAT_LED_NA,LED_UNACTION,KEEP_TMR_0_S);
			}
		}
	}
	else
	{
		if((s_power_on != sys->power_on) || (s_work_mode != work_mode) || (s_work_err != work_errcode) || (s_wifi_sta != wifi_state) ||
		((sSta == SYS_CHARGE) && ((s_change_sta != change_sta) || (s_charg_prs != prs)) ) || 	//���ģʽ�� ���״̬�ı���ߵ����ı�
		((sSta != SYS_CHARGE) && (s_prs != prs)) || (sSta == SYS_CHARGE && msChargeTmr > TMR_CHARGEIDLE_INTO_LOWPOWER)) 		//�ǳ��״̬�µ����仯
		{
			if(sSta == SYS_CHARGE && msChargeTmr > TMR_CHARGEIDLE_INTO_LOWPOWER)	//��粢�Ҵ���3min  ,������߼���Ҫ��������
	{
			/////ֻ���Ƶ�ص�
				if(charge_idle_flag == 0)		/// ������ȫ���־
				{
					send_en = 1;
					log_printf("chargetmr timeout 3min, close all light\r\n");
					charge_idle_flag = 1;		//������ȫ��
					set_led_sta(led_action,led_keep_tmr,STAT_LED_ALL,LED_ON,KEEP_TMR_0_S);
					s_work_mode = 0;
					s_work_err = 0;
					s_wifi_sta = 0;
					s_dustfull = 0;
					s_led_888 = 0;
					nopowercharge = 0;
					set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_OFF,KEEP_TMR_0_S);
					set_led_sta(led_action,led_keep_tmr,STAT_LED_NA,LED_UNACTION,KEEP_TMR_0_S);
					///....1
		if(s_change_sta != change_sta || s_charg_prs != prs)
		{
			s_change_sta = change_sta;
			s_charg_prs = prs;
						s_prs = 0;
			if(change_sta & CHARG_DONE)	//����ǳ�����
			{
				last_led_bty_tmr = KEEP_TMR_120_S;
				set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_120_S); ///���������д2min
			}
			else// if(change_sta & AC_CHARGE || change_sta & DOCK_CHARGE) 	//���ڳ����
			{
				last_led_bty_tmr = KEEP_TMR_0_S;
							if(prs<LOW_POW_PRS_TWS)	//�͵�ѹ
					set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
				else
					set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_0_5HZ,KEEP_TMR_0_S);
			}
		}
					else
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_UNACTION,last_led_bty_tmr);
				}
				//ֻ���Ƴ��������Ʊ���
		else
				{
					///....2
					if(s_change_sta != change_sta || s_charg_prs != prs)
					{
						send_en = 1;
						s_change_sta = change_sta;
						s_charg_prs = prs;
						s_prs = 0;
						if(change_sta & CHARG_DONE)	//����ǳ�����
						{
							last_led_bty_tmr = KEEP_TMR_120_S;
							set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_120_S); ///���������д2min
						}
						else// if(change_sta & AC_CHARGE || change_sta & DOCK_CHARGE) 	//���ڳ����
						{
							last_led_bty_tmr = KEEP_TMR_0_S;
							if(prs<LOW_POW_PRS_TWS)	//�͵�ѹ
								set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
							else
								set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_0_5HZ,KEEP_TMR_0_S);
						}
						set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_UNACTION,KEEP_TMR_0_S);
						set_led_sta(led_action,led_keep_tmr,STAT_LED_NA,LED_UNACTION,KEEP_TMR_0_S);
	}
				}
			}
	else	//���С��3min,���߷ǳ��״̬
	{
				charge_idle_flag = 0;
				nopowercharge = 0;
	///all��,���ػ�����˼
		if(s_power_on != sys->power_on)
		{
					send_en = 1;
			s_power_on = sys->power_on;
			if(s_power_on)		//����״̬
					{
				set_led_sta(led_action,led_keep_tmr,STAT_LED_ALL,LED_ON,KEEP_TMR_0_S);
					}
			else			//�ػ�״̬
				set_led_sta(led_action,led_keep_tmr,STAT_LED_ALL,LED_OFF,KEEP_TMR_0_S);
		}
		else
		{
			set_led_sta(led_action,led_keep_tmr,STAT_LED_ALL,LED_UNACTION,KEEP_TMR_0_S);
			if(s_power_on == 0)
				return;
		}
	///888��
				if(s_led_888 == 0)
				{
					send_en = 1;
					s_led_888 = 1;
					set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_ON,KEEP_TMR_0_S);
				}
				else
					set_led_sta(led_action,led_keep_tmr,STAT_LED_888,LED_UNACTION,KEEP_TMR_0_S);
	//״̬��
	if(s_work_mode!= work_mode)
	{
					send_en = 1;
		s_work_mode = work_mode;
					log_printf("work_mode_led:%02x\r\n",work_mode);
		switch(work_mode)
		{
			case MWO_SILENT:
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_ON,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
			break;
			case MWO_NORMAL:
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_ON,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
			break;
			case MWO_TURBO:
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_ON,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
			break;
			case MWO_MANUAL:
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_ON,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_OFF,KEEP_TMR_0_S);
			break;
			case MWO_FULL2GO:
				set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_OFF,KEEP_TMR_0_S);
				set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_ON,KEEP_TMR_0_S);
			break;
			default:
			break;
		}
	}
	else	//û�仯,
	{
		set_led_sta(led_action,led_keep_tmr,MODE_LED_SILENT,LED_UNACTION,KEEP_TMR_0_S);
		set_led_sta(led_action,led_keep_tmr,MODE_LED_NORMAL,LED_UNACTION,KEEP_TMR_0_S);
		set_led_sta(led_action,led_keep_tmr,MODE_LED_TURBO,LED_UNACTION,KEEP_TMR_0_S);
		set_led_sta(led_action,led_keep_tmr,MODE_LED_MANUAL,LED_UNACTION,KEEP_TMR_0_S);
		set_led_sta(led_action,led_keep_tmr,MODE_LED_FULL2GO,LED_UNACTION,KEEP_TMR_0_S);
	}
//��ص� STAT_LED_BTYPRS
		if(sSta == SYS_CHARGE)	//�����̬��:1���ڳ��,2������
		{
					///....3
			if(s_change_sta != change_sta || s_charg_prs != prs)
	{
						send_en = 1;
		s_change_sta = change_sta;
				s_charg_prs = prs;
						s_prs = 0;
				if(change_sta & CHARG_DONE)	//����ǳ�����
						{
							last_led_bty_tmr = KEEP_TMR_120_S;
					set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_120_S); ///���������д2min
						}
				else// if(change_sta & AC_CHARGE || change_sta & DOCK_CHARGE) 	//���ڳ����
				{
							last_led_bty_tmr = KEEP_TMR_0_S;
					if(prs<20)	//�͵�ѹ
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
					else
			set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_0_5HZ,KEEP_TMR_0_S);
				}
			}
			else
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_UNACTION,last_led_bty_tmr);
	}
		else		//�ǳ��״̬��
	{
		if(s_prs != prs)
		{
						send_en = 1;
						s_change_sta = 0;
						s_charg_prs = 0;
			s_prs = prs;
						last_led_bty_tmr = KEEP_TMR_0_S;
						if(prs<LOW_POW_PRS_TWS)
				set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_TW_1HZ,KEEP_TMR_0_S);
			else
							set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_ON,KEEP_TMR_0_S); 	//��̬Ϊ��
							//set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_OFF,KEEP_TMR_0_S); 	//��̬Ϊ��
		}
			else
						set_led_sta(led_action,led_keep_tmr,STAT_LED_BTYPRS,LED_UNACTION,last_led_bty_tmr);
	}
//�澯 STAT_LED_ERROR
	if(s_work_err != work_errcode)
	{
					send_en = 1;
		s_work_err = work_errcode;
		if(work_errcode)
			set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_TW_2HZ,KEEP_TMR_600_S);
		else
			set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_OFF,KEEP_TMR_0_S);
	}
	else
		set_led_sta(led_action,led_keep_tmr,STAT_LED_ERROR,LED_UNACTION,KEEP_TMR_0_S);
//wifi�� STAT_LED_WIFI
	if(s_wifi_sta != wifi_state )
	{
					send_en = 1;
		s_wifi_sta = wifi_state;
		if(wifi_state)
			set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_ON,KEEP_TMR_0_S);
		else
			set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_OFF,KEEP_TMR_0_S);
	}
	else
		set_led_sta(led_action,led_keep_tmr,STAT_LED_WIFI,LED_UNACTION,KEEP_TMR_0_S);
//������ STAT_LED_DUSTFULL
	if(work_errcode & MWERR_CLRDUSTBOX)
		dustfull = 1;
	else
		dustfull = 0;
	if(s_dustfull!=dustfull)
	{
					send_en = 1;
		s_dustfull = dustfull;
			if(dustfull)
		set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_TW_2HZ,KEEP_TMR_600_S);
			else
				set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_OFF,KEEP_TMR_0_S);
	}
	else
		set_led_sta(led_action,led_keep_tmr,STAT_LED_DUSTFULL,LED_UNACTION,KEEP_TMR_0_S);
	}
	//Ԥ����	�Ժ�����ﴦ��
		}
	}
//////////
	if(send_en == 0)
		return;
//�������еƿ���
	uint8_t send_all_led_en = 0;
	for(i=0;i<LEN_CMD_ALL_LED_LIGHT;i++)
	{
		if(s_led_action[i] != led_action[i])
			send_all_led_en = 1;
	}
	if(send_all_led_en)
	{
		for(i=0;i<LEN_CMD_ALL_LED_LIGHT;i++)
			s_led_action[i] = led_action[i];
		dp_tx_data(DISP_CMD_ALL_LED_LIGHT,LEN_CMD_ALL_LED_LIGHT,led_action);
		//delay_ms(5);
	}
//������Ҫ������˸ʱ��ĵ�,��ʱ�����,�Ϳ��Խ��и���
	uint8_t sta=0;
	uint8_t send_data[LEN_CMD_SINGLE_LED_LIGHT]={0};
	for(i=0;i<LED_CONTRAL_NUB;i++)
	{
		if(s_led_keep_tmr[i] != led_keep_tmr[i])
		{
			s_led_keep_tmr[i] = led_keep_tmr[i];
			//��װһ������
			//send_dp_led_light_contrl();
			sta = get_led_sta(led_action,i);				//��ȡ��Ҫȡ��״̬
			send_data[0] = i;
			send_data[1] = sta;
			send_data[2] = (uint8_t)(led_keep_tmr[i]&0x00ff);
			send_data[3] = (uint8_t)((led_keep_tmr[i]>>8)&0x00ff);
			dp_tx_data(DISP_CMD_SINGLE_LED_LIGHT,LEN_CMD_SINGLE_LED_LIGHT,send_data);
			//delay_ms(5);
		}
	}
}
uint8_t get_u16_bit_sta(uint16_t u16_data,uint16_t u16_bit)
{
	if(u16_data & (BIT_STA(u16_bit)))
		return 1;
	else
		return 0;
}
void set_u16_bit_sta(uint16_t *u16_data,uint16_t u16_bit,uint8_t type)
{
	if(type)
		*u16_data |= (BIT_STA(u16_bit));
	else
		*u16_data &= (~(BIT_STA(u16_bit)));
}
uint8_t get_err_sta(uint16_t u16_data,uint16_t u16_bit)
{
	return get_u16_bit_sta(u16_data,u16_bit);
}
void set_err_sta(uint16_t *u16_data,uint16_t u16_bit,uint8_t type)
{
	set_u16_bit_sta(u16_data,u16_bit,type);
}
void send_dp_led_err_contrl(uint8_t err_code,uint8_t err_sta)
{
	uint8_t send_data[LEN_CMD_ERROR_DIS]={0};
	send_data[0] = err_code; 		//�澯Ҫ+1
	send_data[1] = LED_TW_2HZ;
	send_data[2] = (uint8_t)(KEEP_TMR_600_S&0x00ff);
	send_data[3] = (uint8_t)((KEEP_TMR_600_S>>8)&0x00ff);
	send_data[4] = err_sta;
	dp_tx_data(DISP_CMD_ERROR_DIS,LEN_CMD_ERROR_DIS,send_data);
}
//״̬Ϊ��/��(����˸)/״̬δ�����仯��,ͳһͨ��02�����Կ�
//״̬Ϊ��˸�ĵ�,��Ҫͨ��04����ͬ����˸�Ʒ��͸��Կ�
void dp_led_err_contrl(uint8_t type,uint8_t work_sta,uint16_t work_errcode)
{
	static uint16_t s_work_err = 0;
	uint8_t i=0;
	uint8_t err_sta = 0;
	uint8_t err_code = 0;
	//if(work_sta==SYS_IDLE)		//����ǿ���״̬,���ϱ��澯
	//	return;
	if(type != 1)
		s_work_err = 0;
	if(s_work_err != work_errcode)
	{
		log_printf("work_err:%04x\r\n",work_errcode);
		if(work_errcode)
		{
			if(!((work_errcode & MWERR_AGITATOR_INSPECTIONG) || (work_errcode&MWERR_AGITATOR_REPLACEMENT)))
			{
				if(work_sta!=SYS_IDLE)
				{
					all_key_led_off();		//�д����Ƿ�رհ�����
					log_printf("sys have err not into idle ,into idle now\r\n");
					sys->sState = SYS_IDLE;
					STOP_ALL_MOTOR();
				}
			}
		}
		for(i=0;i<16;i++)
		{
			err_sta = get_err_sta(work_errcode,i);
			if(err_sta != get_err_sta(s_work_err,i))	//�ҵ��仯�Ĵ���״̬
			{//�ж��б仯�Ľ����ϱ�,�����±��ظ澯��¼
				log_printf("work_err_b:%d\r\n",i);
				set_err_sta(&s_work_err,i,err_sta);
				err_code = i+1;		//���������Ҫ+1
				send_dp_led_err_contrl(err_code,err_sta);
				//delay_ms(5);
			}
		}
	}
}
