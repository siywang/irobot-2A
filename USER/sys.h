
#ifndef _SYS_H_
#define _SYS_H_

#define VER_ROMMAT			0	//����
#define VER_KESMAN			1	//����˹��
#define VER_SMALLBLACKMACH	2	//С�ڼ�
#define VER_BIGWHITEMACH	3	//��׼�
#define VER_WHITEBLUEMACH	4	//������


#define SYS_VERSION			20191126.01f//"V2.10-20190307"

//�汾����
/*
[V2.10-20190307]
�޸ı�־Ϊ micony20190307

1��2019-03-07 Ϊ����������������ȥ�Ϻ������Ϻ��칫���޸İ汾��Ӳ���汾Ϊ����100��ǰײ�����汾������Ϊ���ݷ���������
2���س�������޸�Ϊ 9,13,12,����س���ߡ���ֵ��ǣ�֮ǰ�Ĳ�����9 8 9 ����Ȼ˵�ǳɹ��ģ����Ϻ����ֲ��ɹ�����֪�ι�
	cfg->dock_delay = 9;
	cfg->dock_l_count =13;	
	cfg->dock_r_count = 12; 	//�洢��FLASH
3������������Ҫ��1000����Ȼǰײ�������࣬������Ҫ��1000�������ӱ�������
	
	#define IR_D_VALUE 1000      //1000

4��is_unclear_side�����޸ģ��������Ϻ��������ظ���ȥ�����ҵ����Ǹ����򣬲�֪����û����������

	if(sys->t_navi_work > 1200)
	{
		max_c_ucln =	12;
		max_c_cln	=	3;
	}else
	{
		max_c_ucln	= 4;		//���δ��ɨ����
		max_c_cln	= 0;		//�����ɨ���� micony20190307 �Ϻ��칫�Ҳ��ԣ����ִ�һ��������������Ͳ���ȥ��ɨ�ˡ��Ӷ��İ칫�ҵ�����ߵİ칫�ң��Ͳ���ȥɨ�ˡ�
	}
	*c_unclean		= c_ucln;
	*c_clean		= c_cln;

5����ͼ�ر߼���ǰ���Ƿ�ɨ�����ӡ��������ڱ߽����ﷴ����ɨ��
		//micony20190307 ���Ϻ��칫��,����Y�������������㣬�������ϰ��㣬����δ��ɨ��
		if(point_sta(m_edge->tx[len],m_edge->ty[len]+y_dir) != OBST_STA && point_sta(m_edge->tx[len-1],m_edge->ty[len-1]+y_dir) != OBST_STA &&
			point_sta(m_edge->tx[len],m_edge->ty[len]+y_dir*2) != OBST_STA && point_sta(m_edge->tx[len-1],m_edge->ty[len-1]+y_dir*2) != OBST_STA)
		{
			log_printf("CLR_FRONT,near(%d,%d,)\r\n",m_edge->tx[len],m_edge->ty[len]);
			return 2;
		}
6���ܶ��رߺ���û�� navigat->near.pid->c_lost_flag = 0;			//ʧȥǽ�ı�־
	������ײ��û���ر�������
7��ǰײ�м��ֵ�޸�Ϊ����1200,BORD_FRONT_100 ���뿪��
#if BORD_FRONT_100
			cfg->midle_adc   	= 1200;
#else
			cfg->midle_adc   	= 2300;
#endif	

*/


#define SYS_VER					VER_ROMMAT
#define VER_BORD_2				0	//�ڶ��汾
#define HOOVER					1	//����汾
#define ICLEAN_HW_ULTRASONIC	0	//�������ؼ�
#define SIMPLIFY				0	//����ͻ����԰�
#define ENABLE_VOICE			0	//�Ƿ�������

#define SINGLE_LED_DOCK			0	//0:˫��1:����
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include    <stdarg.h>
#include    <stdlib.h>
#include    <string.h>		//20190422 WY add
#include    <stdio.h>		//20190423 WY add
#include "ee_iic.h"

#include "usart.h"
#include "bootloader.h"

#include "pid.h"
#include "gyro.h"
#include "navigation.h"					  
#include "bsp.h"
#include "string.h"
#include "SysTick.h"
#include "motor.h"
#include "iclean.h"
#include "ringbuffer.h"


#include "dispprot.h"
#include "mcu_api.h"
#include "wifi.h"
#include "protocol.h"
#include "system.h"
#include "cfg.h"
#include "sensers.h"
#include "task_rx.h"
#include "key.h"

#include "adc.h"

#include "i2c.h"

#include "math.h"
#include "randclean.h"

#include "charge.h"
#include "libatcharge.h"
#include "ny3p.h"

#include "laser.h"
#include "spi.h"

#include "rtc_lib.h"
#include "key.h"

#include "map.h"
#include "action.h"
#include "targets.h"
#include "calibra.h"
#include "route.h"
#include "test_mode.h"
#include "stdio.h"
#include "mpu6500.h"

#define TRUE	0x01
#define FALSE	0x00

#define MAX(a,b) ((a) > (b) ? (a) : (b)) 
#define MIN(a,b) ((a) < (b) ? (a) : (b)) 

#define disable_irq()  {__ASM  volatile ("cpsid i");}
#define enable_irq()   {__ASM  volatile ("cpsie i");}
float disfloat(float x,float y);
void init_sys_sta(uint8_t sta);
uint32_t disxy_32(uint32_t x,uint32_t y);

#endif

