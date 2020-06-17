
#if !USE_ON_COMPUTER
#include "sys.h"
#include "task_rx.h"
#include "libatcharge.h"
#endif

#include "test_mode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t tm_mode = 0;//1;//����ģʽѡ��,0Ϊ���ģʽ,�������ڲ��Լ��ϵĳ���,1Ϊ��������ģʽ,������װ�����Բ����

TBDIO_UART rec_mac_proc_dat,test_mac_proc_dat;


//���԰��ϴ������ݴ�ŵ���ʱ����
struct __testbd_dat{
	uint8_t unread_flag;//δ����־,������µ������ϱ�,��ֵ��1,�ڱ���ȡ����,����0

	uint8_t pwr_sw;//��Դ�����Ƿ�� 1Ϊ�Ѵ� 0Ϊδ��
	uint8_t mtoc_whl_l;//����ֵ���Ķ�·�����Ƿ�ͨ�� 1Ϊ�Ѷ�· 0Ϊδ��·
	uint8_t mtoc_whl_r;//�Һ��ֵ���Ķ�·�����Ƿ�ͨ�� 1Ϊ�Ѷ�· 0Ϊδ��·
	uint8_t mtoc_smt_l;//���ˢ����Ķ�·�����Ƿ�ͨ��
	uint8_t mtoc_smt_r;//�ұ�ˢ����Ķ�·�����Ƿ�ͨ��
	uint8_t mtoc_mt_fan;//����Ķ�·�����Ƿ��(δʹ��)
	uint8_t mtoc_mt_mid;//���ˢ����Ķ�·�����Ƿ�ͨ��

	uint8_t bum_sw;//ǰ��ײ����ģ������,bit0Ϊ�� bit1Ϊ��,1��ʾ�ر�(�յ�Ϊ�͵�ƽ),0��ʾ��(�յ�Ϊ�ߵ�ƽ)
	uint8_t bum_sw_led;//ǰ��ײ�����LED��Դ���,����Ϊ0,������Ϊ1,bit0Ϊ��,bit1Ϊ��
	uint8_t sensors_sw;//��������ʽ��������ģ�������ź�bit0-bit5����ΪĨ��/����ǽ/����/��������/������/������
	uint8_t sensors_sw_led;//��������ʽ������LED��Դ���bit0-bit3����ΪĨ��/��/��/��������/��/��

	uint8_t bat_info;//��صĽ��������Ϣ:0�������԰�Ͽ� 1��ؽ�������,24V�ӵ�DC�� 2��ؽ�������,24V�ӵ���紥Ƭ
					//3��ؽ�������,24Vδ��������
	uint8_t nc;//δʹ��

	uint16_t cd_smt_l;//���ˢ������
	uint16_t cd_smt_r;//�ұ�ˢ������
	uint16_t cd_mt_mid;//��ɨ������

};
struct __testbd_dat tmd_rxbd_dat;

void tx_dp_msg(uint8_t *buff,int len);
void proc_bd_test_mode(uint8_t *buf,uint8_t len);
void proc_com_test_mode(uint8_t *buf,uint8_t len);
void tx_com_dat(uint8_t code, uint8_t progress, uint8_t error_code, uint8_t ret_value);
void tx_com_mac_dat(TBDIO_UART *proc_dat);
void mac_dat_pro(uint8_t type);
void tx_com_beep(uint8_t progress, uint8_t error_code, uint8_t ret_value,uint16_t *irvalue,uint16_t *irbuttom);
//void tx_dp_dat(uint8_t code, uint8_t para, uint8_t res_out, uint8_t res_in);
void tx_dp_dat(uint8_t code, uint8_t para, uint8_t para1);
void tx_dp_pc_dat(uint8_t code, uint8_t para, uint8_t para1);
void data_print(uint8_t * pdat,uint16_t len);
uint8_t get_chksum(uint8_t *pdata, uint16_t length);

void tx_com_burnin(uint8_t side_dir, uint16_t timer,uint32_t bum_left_ct,uint32_t ir_left_ct,uint32_t bum_right_ct,uint32_t ir_right_ct);
void tx_sn(uint8_t code ,uint8_t *sn);

#define TMOD_COMRX_BUFFSIZE	64

static uint8_t tmd_buf[TMOD_COMRX_BUFFSIZE];
static uint8_t tmd_bd_buf[TMOD_COMRX_BUFFSIZE];

static uint8_t tmod_rx_sta = 0;
static uint8_t tmod_rx_len = 0;
static uint8_t tmod_bd_rx_sta = 0;
static uint8_t tmod_bd_rx_len = 0;

//UART���շ�����
//com_dp: ����ͬʱ�������ӿڵ���,������λ��ͨ�ŵ�UART1�Լ�����԰�ͨ�ŵ�UART4(ԭDP��),��Ҫ���Ӵ˱�������Ӧ
//        ��ͬ��Э���������,����λ��ͨ��,ֵΪ0,����԰�ͨ��ֵΪ1
void tmod_rx_uart(uint8_t com_dp, uint8_t chr)
{
	uint8_t *n_pbuf;
	uint8_t *n_prxsta;
	uint8_t *n_polen;
		//log_printf("%02X ",chr);

	if(!com_dp)
	{
		n_pbuf = tmd_buf;
		n_prxsta = &tmod_rx_sta;
		n_polen = &tmod_rx_len;
	}
	else
	{
		n_pbuf = tmd_bd_buf;
		n_prxsta = &tmod_bd_rx_sta;
		n_polen = &tmod_bd_rx_len;
	}

	switch(*n_prxsta)
	{
		case 0:
			if(chr == 0x7E)
			{
				*n_prxsta=1;
			}
			break;
		case 1:
			if(chr ==0x5D)
			{
				*n_prxsta=2;
				memset(n_pbuf,0x00,TMOD_COMRX_BUFFSIZE);
				*n_polen=0;
			}
			else
			 *n_prxsta =0;
			break;
		case 2:
			n_pbuf[(*n_polen)++]=chr;
			if(*n_polen >=TMOD_COMRX_BUFFSIZE)
			{
				log_printf("rx error\r\n");
				*n_polen=0;
				*n_prxsta=0;
				break;
			}
			//7E 5D 05 00 6D 7D
			if(chr == 0x7D && n_pbuf[*n_polen-2]==0x6D)	//�������
			{
				//log_printf("\r\nfjd\r\n\r\n");
				if(!com_dp)//����λ����ͨ��
					proc_com_test_mode(n_pbuf,*n_polen);
				else
					proc_bd_test_mode(n_pbuf,*n_polen);
				*n_polen=0;
				*n_prxsta=0;
			}
			break;

		default:
				*n_polen=0;
				*n_prxsta=0;
				break;


	}
}

#define BDTEST_LOGPRINT	0

#define BDTEST_SMT_PWMVALUE	900//��ˢPWMת��ֵ,����,ֵԽ��ת��ԽС,��ΧΪ1000-0

uint8_t bdtest_fantest = 0;//����ģʽ�·������ʹ��,�����ת�ټ��ź�ǰ�����̵�A������жϺ��ص�,���Դ���Ϊ���
uint16_t tmod_fancd = 0;//��������̼�����,���Ƶ�30000,�����򱣳�30000
static uint8_t bdtest_sta = TPC_STOP_ALL;//�˱���������ǲ��Թ����е�ĳ����������,����ָ��,�˱����봮��ָ���޹�,ֻ��ʹ������֮��ͬ�ĺ궨��
static uint8_t bdtest_sta_child = 0;//���Բ����С״̬,0Ϊ��ʼ

#define BDTEST_CHG_SUCC_MIN	20//��������������Χ�ڵļ�����Сֵ,��������������ֵ,��Ϊ�������
#define BDTEST_CHG_FAIL_MAX	10//���������޵ļ������ֵ,������ֵ,��Ϊ���ʧ��
#define BDTEST_CHG_START_MAX	200//��ʼ��絽�ﵽԤ���������޶�ʱ��,����������ʱ�仹δ�ﵽԤ��ֵ,��ʾ���ʧ��
uint16_t bdtest_chgcurr = 0;//�������ļ�¼,���������ﵽԤ��ֵ��100maʱ,����20�����ϵķ�Χ�ڼ�����Ϊ�������
									//������ܼƳ���10�����ϵĵ������޼���,��Ϊ���ʧ��
									//������30��ʱ,���޼�������������������������Ϊ�������
//static uint8_t bdtest_chgsucceed_counter = 0xff;//�����������
//static uint8_t bdtest_chgfailed_counter = 0xff;//��糬�޼���
//static uint16_t bdtest_chgstart_counter = 0;//��翪ʼ����������ֵ֮ǰ�ļ���,��ֵ�������BDTEST_CHG_START_MAX,��ʾһֱ��δ�ﵽԤ��ֵ,���ʧ��

#if 0
//��¼��ǰ�ĳ�����,�ⲿ����ʹ��
void bdtest_set_curr(uint16_t curr)
{bdtest_chgcurr = curr;}
#endif
//�Եغ�������õ���ʱ����
//static uint8_t tm_ir_bot_cc;//,tm_ir_bot_j;//,tm_ir_bot_i
///static int32_t tm_ir_bot_k1;//,tm_ir_bot_k2;
ErrorStatus test_bd_com = ERROR;		//�������ʱ���Կ�/ң����ش��ڵ�״̬
uint8_t r_key_err = 0x03;

void test_mode_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOE,&GPIO_InitStructure);

       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_Init(GPIOD,&GPIO_InitStructure);
}

void test_mode_ul(FlagStatus able)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	if(SET == able)
	{
		GPIO_InitStructure.GPIO_Pin =I2C_ULTRASONIC_CLK_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C_ULTRASONIC_CLK_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =I2C_ULTRASONIC_DATA_PIN ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C_ULTRASONIC_DATA_PORT, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =I2C_ULTRASONIC_CLK_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C_ULTRASONIC_CLK_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =I2C_ULTRASONIC_DATA_PIN ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C_ULTRASONIC_DATA_PORT, &GPIO_InitStructure);
		GPIO_ResetBits(I2C_ULTRASONIC_DATA_PORT, I2C_ULTRASONIC_DATA_PIN);
	}
}
void proc_bdtest_task(void)
{
	static uint8_t bd_timer = 0xff;
	static uint8_t bd_heart_timer = 0;
	static uint8_t bd_heart_ct = 0;
//	static uint8_t bdtest_sta_old = TPC_STOP_ALL;
	static uint16_t bd_batcharge_timer = 0;
	static uint8_t bd_stopmode_trigger = TPC_STOP_ALL;//TPC_STOP_ALL״̬���´�����־,�����ж�������TPC_STOP_ALL״̬
	static uint8_t dustbox_order = 0;
	uint32_t mid_close_adc=0,mid_open_adc=0,mid_du_adc=0;
	uint8_t key_err = 0;
	uint16_t adc_t = 0;

	if(TIM5->CNT < 50000)//50ms��ʱ
		return;
	TIM5->CNT = 0;

	if(bd_heart_timer ++ >= 20)//����������,ÿ��һ��
	{
		if(READ_DUSTBOX_DET() && (0 == dustbox_order))
		{
			dustbox_order = 1;
		}
		else if((!READ_DUSTBOX_DET()) && (1 == dustbox_order))
		{
			dustbox_order = 2;
		}
		bd_heart_timer = 0;
		tx_com_dat(TPC_HEART_BEEP, bd_heart_ct ++, TPC_ERR_NOERROR, 0);
	}
	//return;
	if(bdtest_sta != TPC_STOP_ALL)
		bd_stopmode_trigger = bdtest_sta;
	////log_printf("\r\n[wywy]bd test:%d-%d\r\n",bdtest_sta,bdtest_sta_child);
	switch(bdtest_sta)
	{
		case TPC_STOP_ALL:
			//if(bdtest_sta_old != bdtest_sta)
			if(bd_stopmode_trigger != TPC_STOP_ALL)
			{
				//if(bd_stopmode_trigger == TPC_IDLE)
				{
					tx_com_dat(TPC_STOP_ALL, 100 , TPC_ERR_NOERROR, 0);//�Ϸ��������
					tx_dp_dat(TBD_STOP_ALL, 0, 0);//��24V��DC�����ӿ�
				}
				bd_stopmode_trigger = TPC_STOP_ALL;
				TargetSysReset();
				sys->sState = SYS_IDLE;
			}
			MOTOR_POWER_OFF_NPRI();//�رյ����Դ
			break;
		case TPC_START_ALL://����Ҫ����������Ƿ�����/����԰��ͨ���Ƿ�����
			//MOTOR_CTRL(1000, 1000, 0, 0);
			MOTOR_POWER_OFF_NPRI();
			tx_dp_dat(TBD_START_ALL, 0, 0);//��24V��DC�����ӿ�
			bdtest_sta = TPC_MT_FAN;//TPC_WHL_L;////TPC_WHL_L;//TPC_GYRO_TST;//TPC_WHL_L;//TPC_SMT_L;;
			bdtest_sta_child = 0;
			test_bd_com= ERROR;
			r_key_err = 0x03;
			dustbox_order = 0;
			#if BDTEST_LOGPRINT
			log_printf("\r\n[wywy]bd test start!\r\n");
#endif
			break;
		case TPC_MT_FAN:
			switch(bdtest_sta_child)
			{
				case 0://��ʼ���Է��
					{
						bdtest_sta_child = 1;
						bdtest_fantest = 1;
						tmod_fancd = 0;						

						tx_com_dat(TPC_MT_FAN, 25 , TPC_ERR_NOERROR, 0);
						tx_dp_dat(TBD_FAN, 0, 0);
						delay_ms_tm5(300);

						tx_com_dat(TPC_MT_FAN, 50 , TPC_ERR_NOERROR, 0);

						DUST_MOTOR_RANK3();
						delay_ms(300);
						tx_com_dat(TPC_MT_FAN, 75 , TPC_ERR_NOERROR, 0);
										
						DUST_MOTOR_OFF();
						delay_ms(300);
						tx_com_dat(TPC_MT_FAN, 100 , TPC_ERR_NOERROR, 0);
						//log_printf("\r\n[wywy]FAN success!\r\n");

						tmod_fancd=0;
						bd_timer = 0;
						bdtest_sta_child = 1;
					}
				#if BDTEST_LOGPRINT
					bdtest_sta_child = 2;
					break;
				case 2:				
					TEST_MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_WHL_L;
					bdtest_sta_child = 0;
					bdtest_fantest = 0;
					break;
			}
			break;			
		case TPC_WHL_L://���ּ��
			switch(bdtest_sta_child&0x0f)
			{
				case 0://��ʼ��������
					bdtest_sta_child = 1;
					MOTOR_POWER_ON();
					motor.c_left_hw = 0;
					motor_wheel_stop(RIGHT_WHEEL);
					motor_wheel_forward(LEFT_WHEEL,600);

					tx_com_dat(TPC_WHL_L, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					tx_dp_dat(TBD_MTOC_WHL_L, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					bd_timer = 0;
					break;
				case 1://��ǰת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(LEFT_WHEEL);
					#if BDTEST_LOGPRINT
					log_printf("forward:%d\r\n",motor.c_left_hw);
					#endif

					if(motor.c_left_hw < 200)//����������
					{
						MOTOR_POWER_OFF_NPRI();
						tx_com_dat(TPC_WHL_L, 50, TPC_ERR_FORWARD, 7);
						bdtest_sta = TPC_WHL_R;//������һ������
						bdtest_sta_child = 0;
						break;
					}
					tx_com_dat(TPC_WHL_L, 50, TPC_ERR_NOERROR, 8);
					bdtest_sta_child = 2;
					motor.c_left_hw = 0;
					motor_wheel_backward(LEFT_WHEEL,600);
					bd_timer = 0;
					break;
				case 2://���ת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(LEFT_WHEEL);

					#if BDTEST_LOGPRINT
					log_printf("back:%d\r\n",motor.c_left_hw);
					#endif
					//log_printf("\r\n[wywy]Left Whell back:%d\r\n",motor.c_left_hw);

					if(motor.c_left_hw < 200)//����������
					{
						MOTOR_POWER_OFF_NPRI();
					#if BDTEST_LOGPRINT
						log_printf("TPC_ERR_BACK\r\n");
						delay_ms(10);
					#endif
						tx_com_dat(TPC_WHL_L, 75, TPC_ERR_BACK, (uint8_t)(motor.c_left_hw >> 8));
						bdtest_sta = TPC_WHL_R;//������һ������
						bdtest_sta_child = 0;
						break;
					}
					tx_com_dat(TPC_WHL_L, 75, TPC_ERR_NOERROR, 0);//�ϱ�����

					bdtest_sta_child = 3;
					//MOTOR_POWER_ON();
					motor.c_left_hw = 0;

					#if BDTEST_LOGPRINT
						log_printf("BACK PASS\r\n");
						delay_ms(10);
					#endif
					tx_dp_dat(TBD_MTOC_WHL_L, 1, 0);//�·�����ֹ��ز���,����·�ĵ������
					//bd_timer = 0;
					tmd_rxbd_dat.unread_flag = 0;//�����ݽ��յĻ���δ����־��0
					break;
				case 3:
					if(bd_timer ++ < 20 && !tmd_rxbd_dat.unread_flag)break;//��ʱδ�յ�����
					bd_timer = 0;
					if(!tmd_rxbd_dat.unread_flag)
					{
						tx_com_dat(TPC_DPPORT_TST, 100, TPC_ERR_COMM, 0);//�ϱ����Ȼ��ϱ�����������
					}
					tmd_rxbd_dat.unread_flag = 0;
					if(tmd_rxbd_dat.mtoc_whl_l)
					{
						uint8_t i;
						uint16_t n_mt_curr;

						tx_com_dat(TPC_DPPORT_TST, 100, TPC_ERR_NOERROR, 0);//�ϱ����Ȼ��ϱ�����������
						motor_wheel_forward(LEFT_WHEEL,600);
						delay_ms(100);
						for(i = 0;i < 18;i ++)
						{
							delay_ms(5);
							n_mt_curr = I_MOTER_LEFT();
						#if BDTEST_LOGPRINT
							//log_printf("curr:%d-%d-%d\r\n",n_mt_curr,MAX_MOTOR_I,i);
							log_printf("curr:%d\r\n",n_mt_curr);
							delay_ms(10);
						#endif

							if(n_mt_curr >= 40)
							{
								motor_wheel_stop(LEFT_WHEEL);
								break;
							}
						}

						tx_com_dat(TPC_WHL_L, 100, (i < 18) ? (TPC_ERR_NOERROR) : (TPC_ERR_OC), i);//�ϱ����Ȼ��ϱ�����������
						//if(18 > i)
						//	log_printf("\r\n[wywy]Left Whell success\r\n");
						//else
						//	log_printf("\r\n[wywy]Left Whell error\r\n");
						if(i < 18)
						{
						#if BDTEST_LOGPRINT
							delay_ms(100);
							bdtest_sta_child = 4;
							log_printf("oc succeeded\r\n");
						#else
							bdtest_sta = TPC_WHL_R;//������һ������
							bdtest_sta_child = 0;
						#endif
						}
						#if BDTEST_LOGPRINT
						else
						{
							bdtest_sta_child = 4;
							log_printf("\r\n11111\r\n");
							tx_dp_dat(TBD_MTOC_WHL_L, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
							break;
						}
						#endif
					}
					else
					{
						tx_com_dat(TPC_DPPORT_TST, 100, TPC_ERR_COMM, 0);//�ϱ����Ȼ��ϱ�����������
					}
					tx_dp_dat(TBD_MTOC_WHL_L, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
				#if BDTEST_LOGPRINT
					break;
				case 4:
					log_printf("$");
					motor_wheel_stop(LEFT_WHEEL);
					MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_WHL_R;
					bdtest_sta_child = 0;
					motor_wheel_stop(LEFT_WHEEL);
					MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_WHL_R://���ּ��
			switch(bdtest_sta_child&0x0f)
			{
				case 0://��ʼ��������
					bdtest_sta_child = 1;
					MOTOR_POWER_ON();
					motor.c_right_hw = 0;
					motor_wheel_stop(LEFT_WHEEL);
					motor_wheel_forward(RIGHT_WHEEL,600);

					tx_com_dat(TPC_WHL_R, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					tx_dp_dat(TBD_MTOC_WHL_R, 0, 0);//�·��Һ��ֹ��ز���,����·�ĵ���Ͽ�
					bd_timer = 0;
					break;
				case 1://��ǰת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(RIGHT_WHEEL);
					#if BDTEST_LOGPRINT
					log_printf("forward:%d\r\n",motor.c_right_hw);
					#endif
					//log_printf("\r\n[wywy]Right Whell forward:%d\r\n",motor.c_right_hw);

					if(motor.c_right_hw < 200)//����������
					{
						MOTOR_POWER_OFF_NPRI();
						tx_com_dat(TPC_WHL_R, 50, TPC_ERR_FORWARD, 0);
						#if BDTEST_LOGPRINT
						bdtest_sta_child = 4;
						#else
						bdtest_sta = TPC_MT_MID;//������һ������
						bdtest_sta_child = 0;
						#endif
						break;
					}
					tx_com_dat(TPC_WHL_R, 50, TPC_ERR_NOERROR, 0);
					bdtest_sta_child = 2;
					motor.c_right_hw = 0;
					motor_wheel_backward(RIGHT_WHEEL,600);
					bd_timer = 0;
					break;
				case 2://���ת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(RIGHT_WHEEL);

					#if BDTEST_LOGPRINT
					log_printf("back:%d\r\n",motor.c_right_hw);
					log_printf("\r\n[wywy]Right Whell back:%d\r\n",motor.c_right_hw);
					#endif

					if(motor.c_right_hw < 200)//����������
					{
						MOTOR_POWER_OFF_NPRI();
						tx_com_dat(TPC_WHL_R, 75, TPC_ERR_BACK, 0);//�ϱ��������ת

						#if BDTEST_LOGPRINT
						bdtest_sta_child = 4;
						#else
						bdtest_sta = TPC_MT_MID;//������һ������
						bdtest_sta_child = 0;
						#endif
						break;
					}
					tx_com_dat(TPC_WHL_R, 75, TPC_ERR_NOERROR, 0);//�ϱ�����

					bdtest_sta_child = 3;
					motor.c_left_hw = 0;

					tx_dp_dat(TBD_MTOC_WHL_R, 1, 0);//�·�����ֹ��ز���,����·�ĵ������
					bd_timer = 0;
					tmd_rxbd_dat.unread_flag = 0;//�����ݽ��յĻ���δ����־��0
					break;
				case 3:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;
					tmd_rxbd_dat.unread_flag = 0;
					if(1)////(tmd_rxbd_dat.mtoc_whl_r)
					{
						uint8_t i;
						uint16_t n_mt_curr;

						motor_wheel_forward(RIGHT_WHEEL,600);
						delay_ms(100);
						for(i = 0;i < 18;i ++)
						{
							delay_ms(5);
							n_mt_curr = I_MOTER_RIGHT();
						#if BDTEST_LOGPRINT
							//log_printf("curr:%d-%d-%d\r\n",n_mt_curr,MAX_MOTOR_I,i);
						log_printf("\r\n[wywy]Right Whell curr:%d\r\n",n_mt_curr);
						#endif
							//if(I_MOTER_LEFT() >= MAX_MOTOR_I)
							//if(n_mt_curr >= 100)
							if(n_mt_curr >= 40)
							{
								motor_wheel_stop(RIGHT_WHEEL);
								break;
							}
						}

						tx_com_dat(TPC_WHL_R, 100, (i < 18) ? (TPC_ERR_NOERROR):(TPC_ERR_OC), 0);//�ϱ����Ȼ��ϱ�����������
						#if BDTEST_LOGPRINT
						if(18 > i)
							log_printf("\r\n[wywy]Right Whell success\r\n");
						else
							log_printf("\r\n[wywy]Right Whell error\r\n");
						#endif
						//log_printf("\r\n 55555\r\n");
						if(i < 8)
						{
						#if BDTEST_LOGPRINT
							bdtest_sta_child = 4;
							log_printf("oc succeeded\r\n");
						#else
							bdtest_sta = TPC_MT_MID;//������һ������
							bdtest_sta_child = 0;
						#endif
						}
						#if BDTEST_LOGPRINT
						else
						{
							bdtest_sta_child = 4;
							log_printf("\r\n11111\r\n");
							tx_dp_dat(TBD_MTOC_WHL_R, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
							break;
						}
						#endif
					}

					tx_dp_dat(TBD_MTOC_WHL_R, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					//bdtest_sta = TPC_WHL_R;
					//bdtest_sta_child = 0;
					//MOTOR_POWER_OFF_NPRI();
				#if BDTEST_LOGPRINT
					break;
				case 4:
					log_printf("$");
					motor_wheel_stop(RIGHT_WHEEL);
					MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_MT_MID;
					bdtest_sta_child = 0;
					motor_wheel_stop(DOUBLE_WHEEL);
					MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_MT_MID://���Թ�ˢ���
			
			
			bdtest_sta_child = 0;
			navigat->gun_cnt=0;
			tx_dp_dat(TBD_MTOC_MT_MID, 0, 0);
			tx_com_dat(TPC_MT_MID, 33, TPC_ERR_NOERROR, 0);
			TEST_MOTOR_POWER_ON();
			SET_MID_MOTER(500);
			delay_ms(500);
			for(uint8_t i=0;i<10;i++)	
			{
				delay_ms(100);
				adc_t = I_MID_ADC();
				mid_open_adc += adc_t;
			}
			#if BDTEST_LOGPRINT
			log_printf("\r\n[wywy]Mid open adc 0:%d\r\n",mid_open_adc);
			#endif
			tx_com_dat(TPC_MT_MID, 67, TPC_ERR_NOERROR, 0);

			tx_dp_dat(TBD_MTOC_MT_MID, 1, 0);
			delay_ms(500);
			for(uint8_t i=0;i<10;i++)	
			{
				delay_ms(100);
				adc_t = I_MID_ADC();
				mid_du_adc += adc_t;
			}
			#if BDTEST_LOGPRINT
			log_printf("\r\n[wywy]Mid open adc 1:%d\r\n",mid_du_adc);
			#endif
			tx_com_dat(TPC_MT_MID, 67, TPC_ERR_NOERROR, 0);
			
			////TEST_MOTOR_POWER_OFF_NPRI();
			tx_dp_dat(TBD_MTOC_MT_MID, 0, 0);
			SET_MID_MOTER(0);
			delay_ms(500);		
			for(uint8_t i=0;i<10;i++)
			{
				delay_ms(100);
				adc_t = I_MID_ADC();				
				mid_close_adc += adc_t;
			}	//δ���е���	
			#if BDTEST_LOGPRINT
			log_printf("\r\n[wywy]Mid close adc:%d\r\n",mid_close_adc);

			log_printf("\r\n[wywy]Mid code:%d\r\n",navigat->gun_cnt);
			#endif
			if(disXY(mid_open_adc,mid_close_adc)<1000 &&disXY(mid_open_adc,mid_close_adc)>0)////200)
			{
				tx_com_dat(TPC_MT_MID, 100, TPC_ERR_NOERROR, 0);//OK
			#if BDTEST_LOGPRINT
				log_printf("\r\n[wywy]Mid success\r\n");
			#endif
			}
			else
			{
				tx_com_dat(TPC_MT_MID, 100, TPC_ERR_FORWARD, 0);//����
			#if BDTEST_LOGPRINT
				log_printf("\r\n[wywy]Mid error\r\n");
			#endif
			}
			tmod_fancd=0;					
			bdtest_sta = TPC_IR_FRT;//������һ������
			bdtest_sta_child = 0;
			break;
		case TPC_IR_FRT://ǰײ���(��ײ)������,�Եغ���������һ����
			switch(bdtest_sta_child)
			{
				case 0:
					{
						uint8_t cc,bot;
						int32_t k1,k2;
						uint8_t i,j;

						sys->c_ir_adc = 0;
						for(cc = 0;cc < 9;cc ++)
						{
							for(i=0;i<10;i++)
							{
								sys->m_sta[i][cc] = adc_converted_value[i];
								//log_printf(",%d",sys->m_sta[i][sys->c_ir_adc] );
							}
							//log_printf("\r\n");

							if(cc <=3 )
							{
								GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
								GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
								//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); //�͵�ƽ���ص�
								//�ż�
								//NEAR_LAN_ON();
								//FAR_LAN_OFF();
							}
							else
							{

								GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
								GPIO_SetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
								//GPIO_SetBits(PORT_IR_CTRL2,PIN_IR_CTRL2);
								//NEAR_LAN_OFF();
								//FAR_LAN_ON();
							}
							delay_ms(5);
						}

						cc = bot = 0;//cc������¼ǰײ����ļ����,j������¼�Եغ���ļ����
						for(i=0;i<10;i++)
						{
							k1 = k2 = 0;
							for(j=0;j<4;j++)
								k1 +=sys->m_sta[i][j];
							for(j=4;j<8;j++)
								k2 +=sys->m_sta[i][j];


							if(i >=7)
							{
								sys->g_buton[0][i-7] = k2 / 4; //sys->g_sta[i];
								//if(sys->g_butoncg_buton > 300)//���˺���
								if(sys->g_buton[0][i-7] < 3300 || sys->g_buton[0][i-7] > 3700)//���˺���
								{
									bot |= 1 << (i - 7);
								}
								/*sys->g_buton[1][i-7] = k1 / 4;
								if(sys->g_buton[1][i-7] > 1500)//Զ�˺���
								{
									bot |= 1 << (i - 7 + 3);
								}*/
								//log_printf("\r\n[wywy]G Ir:(%d)%d-%d\r\n",i,sys->g_buton[0][i-7],sys->g_buton[1][i-7]);
								//log_printf("\r\n%d\t%d\r\n",sys->g_buton[0][i-7],sys->g_buton[1][i-7]);
							}else
							{
								if(i == 1 || i == 5)
									continue;
								if(i == 2)
								{
									if(k1 > k2 && ((k1 - k2) /4) < 5000)
										sys->g_sta[i] = (k1 - k2) >> 2;
									else
										sys->g_sta[i] = 0;
									if(sys->g_sta[i] < 1500 || sys->g_sta[i] > 3000)//���˺���
									{
										cc |= 1 << i;
										//log_printf("err:%d-%d\r\n",i,sys->g_sta[i]);
										//delay_ms(10);
									}
								}
								else
								{
									sys->g_sta[i] = (k1) >> 2;
									if(sys->g_sta[i] < 3300 || sys->g_sta[i] > 3700)//���˺���
									{
										cc |= 1 << i;
										//log_printf("err:%d-%d\r\n",i,sys->g_sta[i]);
										//delay_ms(10);
									}
								}

							}
						//log_printf("%d,%d,%d,%d\r\n",i,k1 / 4,k2 /4,(k1-k2 ) / 4);
						}
						#if 0 //���ԵĴ�ӡ��Ϣ
						log_printf("\r\nirda:%02x\r\nirbt:%02x\r\n",cc,bot);
						delay_ms(500);
						#endif
						//delay_ms(500);
						#if 1
						#if 0
						delay_ms(500);
						log_printf("\r\nir_frt:\r\n");
						for(i = 0;i < 7;i ++)
						{
							log_printf("\t%d",sys->g_sta[i]);
						}
						log_printf("-%02X\r\nir_bot:\r\n",cc);
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[0][i]);
						}
						log_printf("-\t-");
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[1][i]);
						}
						log_printf("-%02X\r\n\r\n",bot);
						#else

						if(cc)
						{
							tx_com_dat(TPC_IR_FRT, 100 , TPC_ERR_IRLED, cc);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_IR_FRT, 100 , TPC_ERR_NOERROR, 0);//�ϱ����ȼ�������Ϣ
						}

						if(bot)
						{
							tx_com_dat(TPC_IR_BOT, 100 , TPC_ERR_IRLED, bot);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_IR_BOT, 100 , TPC_ERR_NOERROR, 0);//�ϱ����ȼ�������Ϣ
						}
						#endif
						#endif
					}
					//�ر����к���LED,�Խ��͹���
					FAR_LAN_OFF();
					NEAR_LAN_OFF();
					GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
					//GPIO_ResetBits(PORT_IR_CTRL2,PIN_IR_CTRL2); //�͵�ƽ���ص�
					#if 1//�����ܲ���״̬��,��״̬ת����Ҫ�ر�һ��
					bdtest_sta = TPC_IRDA_CHRG;
					bdtest_sta_child = 0;
					#endif
					break;
				case 1:
				default:
					bdtest_sta_child = 0;
					bdtest_sta = TPC_IRDA_CHRG;
					break;
			}
			break;
		case TPC_IRDA_CHRG:
			switch(bdtest_sta_child)
			{
			case 0:
				{
					////uint8_t n_irdata[3] = {0};

					////read_ir_original_data(n_irdata);//��һ�κ���,��֮ǰ��ֵ���
				}
				bdtest_sta_child = 1;
				bd_batcharge_timer = 0;//�ѱ�����ʱ�������һ��,���������ղ��������źŵļ���
				break;
			case 1:
				{
					uint8_t n_irdata[4] = {0};
					uint8_t n_irres = 0;
					//uint8_t n_irknk = 0;
					//uint8_t i,cc;
					read_ir_original_data(n_irdata);
					//read_ir_data(n_irdata,0);
			#if BDTEST_LOGPRINT
					log_printf("\r\n[wywy]Dock Ir:(%d)%d,%d,%d\r\n",
					bd_batcharge_timer,
					n_irdata[0],n_irdata[1],n_irdata[2]);
			#endif
					//n_irres = (n_irdata[0] | n_irdata[1] | n_irdata[2]) & 0x01;//ȡ�����ź�
					//n_irres = (n_irdata[0] | n_irdata[1] | n_irdata[2] | n_irdata[3]);//ȡ�����ź�
					//n_irres ^= 0x01;
					if((n_irdata[0]&0x1f) == 0)	n_irres |= 0x01;
					if((n_irdata[1]&0x1f) == 0)	n_irres |= 0x02;
					if((n_irdata[2]&0x1f) == 0)	n_irres |= 0x04;
					if((n_irdata[3]&0x1f) == 0)	n_irres |= 0x08;
					
					if((n_irdata[0]&0xe0) != 0xe0)n_irres |= 0x01;
					if((n_irdata[1]&0xe0) != 0xe0)n_irres |= 0x02;
					if((n_irdata[2]&0xe0) != 0xe0)n_irres |= 0x04;
					if((n_irdata[3]&0xe0) != 0xe0)n_irres |= 0x08;

					#if 0
					log_printf("\r\n %02x %02x %02x res:%02x\r\n",n_irdata[0],n_irdata[1],n_irdata[2],n_irres);
					//log_printf("\r\n ---%02x---\r\n",n_irdata[0]);
					delay_ms(200);
					#else
					if(n_irres != 0 && bd_batcharge_timer ++ > 2)
					{
						#if 0
						log_printf("\r\n %02x %02x %02x res:%02x\r\n",n_irdata[0],n_irdata[1],n_irdata[2],n_irres);
						//log_printf("\r\n ---%02x---\r\n",n_irdata[0]);
						delay_ms(200);
						#else
						bd_batcharge_timer = 0;
						bdtest_sta_child = 0;
						bdtest_sta = TPC_BUM_SW;
						tx_com_dat(TPC_IRDA_CHRG, 100 , (n_irres!=0) ? (TPC_ERR_IRLED):(TPC_ERR_NOERROR), n_irres);//�ϱ����ȼ�������Ϣ
						#endif
					}
					else if(n_irres == 0)
					{
						bd_batcharge_timer = 0;
						bdtest_sta_child = 0;
						bdtest_sta = TPC_BUM_SW;
						tx_com_dat(TPC_IRDA_CHRG, 100 , (n_irres!=0) ? (TPC_ERR_IRLED):(TPC_ERR_NOERROR), n_irres);//�ϱ����ȼ�������Ϣ
					}
					#endif
				}
				#if 1//�����ܲ���״̬��,��״̬ת����Ҫ�ر�һ��
				#else
				bdtest_sta_child = 1;
				#endif
				break;
			case 2:
			default:
				break;
			}
			break;
		case TPC_BUM_SW://��ȡ��ײ��������
			switch(bdtest_sta_child&0x0f)
			{
				case 0://��ȡ�����޹�ʱ��GPIO������������,���������״̬��,��ʱӦ������ײ(�ߵ�ƽ)
					tx_dp_dat(TBD_BUM_SW, 0, 0);//������ײ�ſ�
					delay(1000);
					{
						uint8_t n_res = 0;
						uint8_t i;

						////��1
						for(i=0;i<20;i++)
						{
							if(((GPIOD->IDR) & GPIO_Pin_0)!=0)
								break;
						}
						if(i==20)
							n_res |= 0x01;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_12) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x02;

						//��1
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_5) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x04;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_6) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x08;

			#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]bum:(%d)%d-%d-%d-%d\r\n",bdtest_sta_child,
						((GPIOD->IDR) & GPIO_Pin_0),
						((GPIOE->IDR) & GPIO_Pin_12),
						((GPIOE->IDR) & GPIO_Pin_5),
						((GPIOE->IDR) & GPIO_Pin_6));
			#endif
						#if 0
						log_printf("\r\n double bum:%02x\r\n",n_res);
						delay_ms(100);
						#endif
						if(n_res)
						{
							tx_com_dat(TPC_BUM_SW, 25 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_BUM_SW, 20 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
					}

					bdtest_sta_child = 1;
					tx_dp_dat(TBD_BUM_SW, 1, 0x01);//����ײ�̽ӵ���
					bd_timer = 0;
					break;
				case 1:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.bum_sw == 0x01)
					{
						uint8_t i;
						uint8_t n_res = 0;

						////��1
						for(i=0;i<20;i++)
						{
							if(((GPIOD->IDR) & GPIO_Pin_0)==0)
								break;
						}
						if(i==20)
							n_res |= 0x01;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_12) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x02;

						//��1
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_5) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x04;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_6) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x08;
			#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]bum:(%d)%d-%d-%d-%d\r\n",bdtest_sta_child,
						((GPIOD->IDR) & GPIO_Pin_0),
						((GPIOE->IDR) & GPIO_Pin_12),
						((GPIOE->IDR) & GPIO_Pin_5),
						((GPIOE->IDR) & GPIO_Pin_6));
			#endif
						#if 0
						log_printf("\r\nleft bum:%02x\tled:%d\r\n",n_res,tmd_rxbd_dat.bum_sw_led);
						delay_ms(100);
						#endif
						if(n_res)
						{
							tx_com_dat(TPC_BUM_SW, 40 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_BUM_SW, 40 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
					}
					//else
					//	tx_com_dat(TPC_BUM_SW, 50, TPC_ERR_TB, 0);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_BUM_SW, 1, 0x02);//����ײ�ſ�,����ײ�̽ӵ���
					bdtest_sta_child = 2;
					break;
				case 2:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.bum_sw == 0x02)
					{
						uint8_t i;
						uint8_t n_res = 0;

						////��1
						for(i=0;i<20;i++)
						{
							if(((GPIOD->IDR) & GPIO_Pin_0)!=0)
								break;
						}
						if(i==20)
							n_res |= 0x01;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_12) ==0)
								break;
						}
						if(i==20)
							n_res |= 0x02;

						//��1
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_5) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x04;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_6) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x08;
			#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]bum:(%d)%d-%d-%d-%d\r\n",bdtest_sta_child,
						((GPIOD->IDR) & GPIO_Pin_0),
						((GPIOE->IDR) & GPIO_Pin_12),
						((GPIOE->IDR) & GPIO_Pin_5),
						((GPIOE->IDR) & GPIO_Pin_6));
			#endif
						#if 0
						log_printf("\r\nright bum:%02x\r\n",n_res);
						delay_ms(100);
						#endif
						if(n_res)
						{
							tx_com_dat(TPC_BUM_SW, 60 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_BUM_SW, 60 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
					}
					else
						tx_com_dat(TPC_BUM_SW, 60, TPC_ERR_TB, 0);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_BUM_SW, 1, 0x04);
					bdtest_sta_child = 3;
					break;
				case 3:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.bum_sw == 0x04)
					{
						uint8_t i;
						uint8_t n_res = 0;

						////��1
						for(i=0;i<20;i++)
						{
							if(((GPIOD->IDR) & GPIO_Pin_0) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x01;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_12) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x02;

						//��1
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_5) ==0)
								break;
						}
						if(i==20)
							n_res |= 0x04;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_6) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x08;
			#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]bum:(%d)%d-%d-%d-%d\r\n",bdtest_sta_child,
						((GPIOD->IDR) & GPIO_Pin_0),
						((GPIOE->IDR) & GPIO_Pin_12),
						((GPIOE->IDR) & GPIO_Pin_5),
						((GPIOE->IDR) & GPIO_Pin_6));
			#endif
						#if 0
						log_printf("\r\nright bum:%02x\r\n",n_res);
						delay_ms(100);
						#endif
						if(n_res)
						{
							tx_com_dat(TPC_BUM_SW, 80 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_BUM_SW, 80 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
					}
					else
						tx_com_dat(TPC_BUM_SW, 80, TPC_ERR_TB, 0);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_BUM_SW, 1, 0x08);//������ײ�ſ�
					bdtest_sta_child = 4;
					break;
				case 4:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.bum_sw == 0x08)
					{
						uint8_t i;
						uint8_t n_res = 0;

						////��1
						for(i=0;i<20;i++)
						{
							if(((GPIOD->IDR) & GPIO_Pin_0) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x01;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_12) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x02;

						//��1
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_5) !=0)
								break;
						}
						if(i==20)
							n_res |= 0x04;

						//��2
						for(i=0;i<20;i++)
						{
							if(((GPIOE->IDR) & GPIO_Pin_6) ==0)
								break;
						}
						if(i==20)
							n_res |= 0x08;
			#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]bum:(%d)%d-%d-%d-%d\r\n",bdtest_sta_child,
						((GPIOD->IDR) & GPIO_Pin_0),
						((GPIOE->IDR) & GPIO_Pin_12),
						((GPIOE->IDR) & GPIO_Pin_5),
						((GPIOE->IDR) & GPIO_Pin_6));
			#endif
						#if 0
						log_printf("\r\nright bum:%02x\r\n",n_res);
						delay_ms(100);
						#endif
						if(n_res)
						{
							tx_com_dat(TPC_BUM_SW, 100 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_BUM_SW, 100 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
					}
					else
						tx_com_dat(TPC_BUM_SW, 100, TPC_ERR_TB, 0);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_BUM_SW, 0, 0);//������ײ�ſ�
				#if 0
					bdtest_sta_child = 3;
					break;
				case 3:
					break;
				#endif
				default:
					bdtest_sta = TPC_SWSENSOR;
					bdtest_sta_child = 0;
					bdtest_fantest = 0;
					break;
			}
			break;
		case TPC_SWSENSOR://��ȡ��������ʽ����������
			switch(bdtest_sta_child&0x0f)
			{
				case 0://��ȡδ���̽ӵ���ʱ�ĸ�����������
					test_mode_ul(SET);
					delay_ms_tm5(500);
					SCLL;
					{
						uint8_t n_res = 0;
						uint8_t i;

						for(i=0;i<20;i++)//������
						{
							if(READ_DATA() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x01;

						for(i=0;i<20;i++)//����ǽ,�궨���Ƿ���...
						{
							if(READ_VWALL_DET() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x02;

						for(i=0;i<20;i++)//������
						{
							if(LEFT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x10;

						for(i=0;i<20;i++)//������
						{
							if(RIGHT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x20;
						#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]qi ta:(%d)%d,%d,%d-%d\r\n",bdtest_sta_child,
						READ_DATA(),
						READ_VWALL_DET(),
						LEFT_MOTOR_LEAVE(),
						RIGHT_MOTOR_LEAVE());
						log_printf("\r\n case 0:%02x\r\n",n_res);
						delay_ms(100);
						#else
						if(n_res)
						{
							tx_com_dat(TPC_SWSENSOR, 25 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_SWSENSOR, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
						#endif
					}

					////tx_dp_dat(TBD_SWSENSOR, 1, 0x01);//������
					SCLH;
					bd_timer = 0;
					bdtest_sta_child = 1;
					break;
				case 1:
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)
					{
						uint8_t n_res = 0;
						uint8_t i;

						for(i=0;i<20;i++)//������
						{
							if(READ_DATA() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x01;

						for(i=0;i<20;i++)//����ǽ
						{
							if(READ_VWALL_DET() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x02;

						for(i=0;i<20;i++)//������
						{
							if(LEFT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x10;

						for(i=0;i<20;i++)//������
						{
							if(RIGHT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x20;
						#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]qi ta:(%d)%d,%d,%d-%d\r\n",bdtest_sta_child,
						READ_DATA(),
						READ_VWALL_DET(),
						LEFT_MOTOR_LEAVE(),
						RIGHT_MOTOR_LEAVE());
						log_printf("\r\n case 2:%02x\r\n",n_res);
						delay_ms(100);
						#else
						if(n_res)
						{
							tx_com_dat(TPC_SWSENSOR, 45 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_SWSENSOR, 45 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
						#endif
					}

					tx_dp_dat(TBD_SWSENSOR, 1, 0x02);//����ǽ
					SCLL;
					bd_timer = 0;
					bdtest_sta_child = 2;
					break;
				case 2:
					//bdtest_sta_child = (bdtest_sta_child & 0xf0) + 3;
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.sensors_sw== 0x02)
					{
						uint8_t n_res = 0;
						uint8_t i;

						for(i=0;i<20;i++)//������
						{
							if(READ_DATA() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x01;

						for(i=0;i<20;i++)//����ǽ
						{
							if(READ_VWALL_DET() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x02;

						for(i=0;i<20;i++)//������
						{
							if(LEFT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x10;

						for(i=0;i<20;i++)//������
						{
							if(RIGHT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x20;
						#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]qi ta:(%d)%d,%d,%d-%d\r\n",bdtest_sta_child,
						READ_DATA(),
						READ_VWALL_DET(),
						LEFT_MOTOR_LEAVE(),
						RIGHT_MOTOR_LEAVE());
						log_printf("\r\n case 2:%02x\r\n",n_res);
						delay_ms(100);
						#else
						if(n_res)
						{
							tx_com_dat(TPC_SWSENSOR, 60 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_SWSENSOR, 60 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
						#endif
					}
					else
						tx_com_dat(TPC_SWSENSOR, 60, TPC_ERR_TB, tmd_rxbd_dat.sensors_sw);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_SWSENSOR, 1, 0x10);//�����ּ���������
					bd_timer = 0;
					bdtest_sta_child = 3;
					break;
				case 3:
					//bdtest_sta_child = (bdtest_sta_child & 0xf0) + 6;
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.sensors_sw== 0x10)
					{
						uint8_t n_res = 0;
						uint8_t i;

						for(i=0;i<20;i++)//������
						{
							if(READ_DATA() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x01;

						for(i=0;i<20;i++)//����ǽ
						{
							if(READ_VWALL_DET() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x02;

						for(i=0;i<20;i++)//������
						{
							if(LEFT_MOTOR_LEAVE() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x10;

						for(i=0;i<20;i++)//������
						{
							if(RIGHT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x20;
						#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]qi ta:(%d)%d,%d,%d-%d\r\n",bdtest_sta_child,
						READ_DATA(),
						READ_VWALL_DET(),
						LEFT_MOTOR_LEAVE(),
						RIGHT_MOTOR_LEAVE());
						log_printf("\r\n case 5:%02x\r\n",n_res);
						delay_ms(100);
						#else
						if(n_res)
						{
							tx_com_dat(TPC_SWSENSOR, 90 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_SWSENSOR, 90 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
						#endif
					}
					else
						tx_com_dat(TPC_SWSENSOR, 90, TPC_ERR_TB, tmd_rxbd_dat.sensors_sw);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_SWSENSOR, 1, 0x20);//�����ּ���������
					bd_timer = 0;
					bdtest_sta_child = 4;
					break;
				case 4:
					//bdtest_sta_child = (bdtest_sta_child & 0xf0);
					if(bd_timer ++ < 20)break;//��ʱδ�յ�����
					bd_timer = 0;

					if(1)////(tmd_rxbd_dat.sensors_sw== 0x20)
					{
						uint8_t n_res = 0;
						uint8_t i;

						for(i=0;i<20;i++)//������
						{
							if(READ_DATA() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x01;

						for(i=0;i<20;i++)//����ǽ
						{
							if(READ_VWALL_DET() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x02;

						for(i=0;i<20;i++)//������
						{
							if(LEFT_MOTOR_LEAVE() != 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x10;

						for(i=0;i<20;i++)//������
						{
							if(RIGHT_MOTOR_LEAVE() == 0)//�ſ�ʱӦΪ�ߵ�ƽ,�����⵽�͵�ƽ���ж�Ϊ����쳣
								break;
						}
						if(i==20)
							n_res |= 0x20;
						#if BDTEST_LOGPRINT
						log_printf("\r\n[wywy]qi ta:(%d)%d,%d,%d-%d\r\n",bdtest_sta_child,
						READ_DATA(),
						READ_VWALL_DET(),
						LEFT_MOTOR_LEAVE(),
						RIGHT_MOTOR_LEAVE());
						log_printf("\r\n case 6:%02x\r\n",n_res);
						delay_ms(100);
						#else
						if(n_res)
						{
							tx_com_dat(TPC_SWSENSOR, 100 , TPC_ERR_DIGI_IN, n_res);//�ϱ����ȼ�������Ϣ
						}
						else
						{
							tx_com_dat(TPC_SWSENSOR, 100 , TPC_ERR_NOERROR, 0);//�ϱ�����
						}
						#endif
					}
					else
						tx_com_dat(TPC_SWSENSOR, 100, TPC_ERR_TB, tmd_rxbd_dat.sensors_sw);//�ϱ����Ȼ��ϱ�����������

					tx_dp_dat(TBD_SWSENSOR, 0, 0);//����ǽ����������
					bd_timer = 0;
					bdtest_sta_child = 0;
				#if BDTEST_LOGPRINT
					bdtest_sta_child = 7;
					break;
				case 7:
					break;
				#endif
				default:
					test_mode_ul(RESET);
					bdtest_sta = TPC_CHRG_TST;
					bdtest_sta_child = 0;
					MOTOR_POWER_OFF_NPRI();
					bdtest_fantest = 0;
					break;
			}
			break;
		case TPC_CHRG_TST://������
			switch(bdtest_sta_child&0x0f)
			{
				#if 1
				case 0:
					tx_dp_dat(TBD_CHRG_TST, 3, 0);//��24V��DC�����ӿ�
					bdtest_sta_child = 1;
					break;
				case 1://�·������԰�,Ҫ����԰彫��ص���ؽŽ��뵽������
					bdtest_sta_child = 2;
					//�Ȳ�����ص��¶�,������Ƿ���ڻ����,���DC������������źŽ��Ƿ�����
					{
						uint16_t n_bat_temp = LiBat_GetBatTemper();//��ȡ����¶�
						uint8_t n_error_code = TPC_ERR_NOERROR;
						#if 0
						if(n_bat_temp == 1500)//��ز�����,�˳�����ģʽ
						{
							//bdtest_sta_child = 0;
							//bdtest_sta = TPC_STOP_ALL;
							n_error_code = TPC_ERR_BATNE;
							//break;
						}
						else if(/*n_bat_temp < 0 || */n_bat_temp > 400)//��ع���,�˳�����ģʽ
						{
							//bdtest_sta_child = 0;
							//bdtest_sta = TPC_STOP_ALL;
							n_error_code = TPC_ERR_BATOT;
							//break;
						}
						#else
						n_bat_temp = 200;
						#endif
						//���������DC�����ĵ�·�Ƿ�����,δ�����״̬��,��ӦΪ�͵�ƽ
						if(DOCK_DETECT())
						{
							//bdtest_sta_child = 0;
							//bdtest_sta = TPC_STOP_ALL;
							n_error_code = TPC_ERR_DOCK;
							//break;
						}
						if(EXTERAL_AC_DETECT())
						{
							n_error_code = TPC_ERR_DCJACK;
							//break;
						}

						#if BDTEST_LOGPRINT
						log_printf("\r\n 1:%d,DOCK:%d-DC:%d\r\n",n_error_code,DOCK_DETECT(),EXTERAL_AC_DETECT());
						#endif
						delay_ms(100);
						tx_com_dat(TPC_CHRG_TST, 25 , n_error_code, 0);
						if(n_error_code != TPC_ERR_NOERROR)
						{
							bdtest_sta_child = 0;
							bdtest_sta = TPC_GYRO_TST;
							//bdtest_sta_child = 4;
							break;
						}
					}
					tmd_rxbd_dat.unread_flag = 1;
					tx_dp_dat(TBD_CHRG_TST, 1, 0);//��24V��DC�����ӿ�
					bd_timer = 0;
					break;
				case 2://�·������԰�,Ҫ����԰彫��ص���ؽŽ��뵽������
					if(bd_timer ++ < 20 && !tmd_rxbd_dat.unread_flag)break;//��ʱδ�յ�����
					bd_timer = 0;
					if(!tmd_rxbd_dat.unread_flag)//��ʱδ�յ�����,����԰�(�Կض˿�)ͨ��ʧ��
					{
						tx_com_dat(TPC_DPPORT_TST, 0, TPC_ERR_COMM, 0);//����ͨ�Ŵ���
						tx_dp_dat(TBD_CHRG_TST, 1, 0);//��24V��DC�����ӿ�
						bdtest_sta_child = (bdtest_sta_child & 0xf0) + 2;
						bdtest_sta_child += 0x10;
						if(bdtest_sta_child > 0x50)
							bdtest_sta_child = 0x0f;
						//tmd_rxbd_dat.unread_flag = 0;
						//bdtest_sta_child = 4;
						break;
					}
					else
					{
						uint8_t n_error_code = TPC_ERR_NOERROR;

						delay_ms(200);
						tmd_rxbd_dat.unread_flag = 0;
						
						if(DOCK_DETECT())//�����źż�����
							n_error_code = TPC_ERR_DOCK;
						if(EXTERAL_AC_DETECT() == 0)
							n_error_code = TPC_ERR_DCJACK;
						
						//log_printf("\r\n 1:%d,DOCK:%d-DC:%d\r\n",n_error_code,DOCK_DETECT(),EXTERAL_AC_DETECT());
						//delay_ms(100);
						tx_com_dat(TPC_CHRG_TST, 50 , n_error_code, 0);
					}
					bdtest_sta_child = 3;
					tx_dp_dat(TBD_CHRG_TST, 2, 0);//��24V�򵽳������Ƭ�ӿ�
					bd_timer = 0;
					break;
				case 3://���DC�����źż���Ƿ�����
					if(bd_timer ++ < 20 && !tmd_rxbd_dat.unread_flag)break;//��ʱδ�յ�����
					bd_timer = 0;
					if(!tmd_rxbd_dat.unread_flag)//��ʱδ�յ�����,����԰�(�Կض˿�)ͨ��ʧ��
					{
						tx_com_dat(TPC_DPPORT_TST, 0, TPC_ERR_COMM, 0);//����ͨ�Ŵ���
						//tx_dp_dat(TBD_CHRG_TST, 2, 0);//��24V��DC�����ӿ�
						bdtest_sta_child = (bdtest_sta_child & 0xf0) + 3;
						bdtest_sta_child += 0x10;
						if(bdtest_sta_child > 0x50)
							bdtest_sta_child = 0x0f;
						//tmd_rxbd_dat.unread_flag = 0;
						//bdtest_sta_child = 4;
						break;
					}
					else
					{
						uint8_t n_error_code = TPC_ERR_NOERROR;
						
						delay_ms(200);
						tmd_rxbd_dat.unread_flag = 0;
						
						if(DOCK_DETECT() == 0)//�����źż�����
							n_error_code = TPC_ERR_DOCK;
						if(EXTERAL_AC_DETECT())
							n_error_code = TPC_ERR_DCJACK;
						
						//log_printf("\r\n 2:%d\r\n",n_error_code);
						//log_printf("\r\n 1:%d,DOCK:%d-DC:%d\r\n",n_error_code,DOCK_DETECT(),EXTERAL_AC_DETECT());
						//delay_ms(100);
						tx_com_dat(TPC_CHRG_TST, 100 , n_error_code, 0);
						//LiBat_HalInit();
//						bdtest_chgfailed_counter = 0xff;
//						bdtest_chgsucceed_counter = 0xff;
//						bdtest_chgstart_counter = 0;
						bd_batcharge_timer = 0;
						//delay_ms(500);
					}
					
					tx_dp_dat(TBD_CHRG_TST, 0, 0);//��24V��DC�����ӿ�
					bdtest_sta_child = 0;//0
					//log_printf("\r\nLB_CHS_ERROR:%d\r\n",n_charge_res);
					bdtest_sta = TPC_GYRO_TST;
					//bdtest_sta_child = 4;
					bd_timer = 0;
					break;
				#if 0
				case 4:
					log_printf("^");
					delay_ms(1000);
					break;
				#endif
				default:
					bdtest_sta = TPC_GYRO_TST;
					bdtest_sta_child = 0;
					MOTOR_POWER_OFF_NPRI();
					bdtest_fantest = 0;
					break;
				#else
				case 0:
					bdtest_sta_child = 1;
					tx_com_dat(TPC_CHRG_TST, 100, TPC_ERR_NOERROR, 0);//�ϱ�����
					break;
					if(DOCK_DETECT())
					{				
						if(CHARGE_DONE_DETECT() == 0 || CHARGE_ING_DETECT() == 0)	
							tx_com_dat(TPC_CHRG_TST, 100, TPC_ERR_NOERROR, 0);//�ϱ�����
						else
							tx_com_dat(TPC_CHRG_TST, 100, TPC_ERR_DOCK, 0);//�ϱ�����
					}	
					else
					{
						tx_com_dat(TPC_CHRG_TST, 100, TPC_ERR_DCJACK, 0);//�ϱ�����				
					}
					log_printf("\r\n[wywy]chong dian:%d\r\n",DOCK_DETECT());
					break;
				default:
					bdtest_sta = TPC_GYRO_TST;
					bdtest_sta_child = 0;
					MOTOR_POWER_OFF_NPRI();
					bdtest_fantest = 0;
					break;
				#endif
			}
			break;
		case TPC_GYRO_TST:
			switch(bdtest_sta_child)
			{
			case 0:
				#if 0
				{
					uint8_t n_gyro_res = 0;
					uint8_t n_err_res = TPC_ERR_DIGI_IN;
					SPI_CS(0);
					SPI1_ReadWriteByte(0xB5);
					n_gyro_res = SPI1_ReadWriteByte(0);
					SPI_CS(1);

					if(n_gyro_res == 0x70)//��������
					{
						n_err_res = TPC_ERR_NOERROR;
					}
					else if(n_gyro_res == 0)//�����ǰ��쳣��δ��
					{
						n_err_res = TPC_ERR_COMM;//����ͨ���쳣
					}
					else if(n_gyro_res == 0x7A)//������IC�쳣
					{
						n_err_res = TPC_ERR_DIGI_IN;//����ͨ���쳣
					}
					else
					{
						n_err_res = TPC_ERR_DIGI_OUT;//����ͨ���쳣
					}
					#if 0
					bdtest_sta_child = 1;
					log_printf("n_gyro_res:%02x\t err:%d\r\n",n_gyro_res,n_err_res);
					#else
					//bdtest_sta = TPC_STOP_ALL;
					tx_com_dat(TPC_GYRO_TST, 100 , n_err_res, 0);//�Ϸ��������
					tx_com_dat(TPC_STOP_ALL, 100 , TPC_ERR_NOERROR, 0);//�Ϸ��������
					tx_dp_dat(TBD_STOP_ALL, 0, 0);//��24V��DC�����ӿ�
					#endif
				}
				#else
				{
					if(sys->gyro_init_ok)
						tx_com_dat(TPC_GYRO_TST, 100, TPC_ERR_NOERROR, 0);//�ϱ�����
					else
						tx_com_dat(TPC_GYRO_TST, 50, TPC_ERR_TYRO, 1);//�ϱ�����
					bdtest_sta = TPC_CT_KEY;
					bdtest_sta_child = 0;
				}
				#endif
			break;
			case 1:
				break;
			default:
				break;
			}
			break;
		//case TPC_IDLE:
				
		//	break;
		case TPC_CT_KEY:
			switch(bdtest_sta_child)
			{
				case 0:
					tx_dp_dat(TBD_KEY, 0, 0);
					delay(1000);
					key_err = 0;
					if(0 != KEY_RD_SPOT())		//������ɨ
					{
						key_err |= 0x04;
					}

					if(0 != KEY_RD_WIFI())		//�����
					{
						key_err |= 0x08;
					}
					#if BDTEST_LOGPRINT
					log_printf("\r\n[wywy]key:(%d)%d-%d\r\n",bdtest_sta_child,
					KEY_RD_SPOT(),
					KEY_RD_WIFI());
					#endif
					//key_err |= r_key_err;
					if(0 != key_err)
					{
						tx_com_dat(TPC_CT_KEY, 30, TPC_ERR_DIGI_IN, key_err);//�ϱ�����
						////bdtest_sta_child = 3;
					}
					else
					{
						tx_com_dat(TPC_CT_KEY, 30, TPC_ERR_NOERROR, 0);//�ϱ�����
						////tx_dp_dat(TBD_KEY, 1, 0x04);
						////bdtest_sta_child = 1;
						////delay(500);
					}
					tx_dp_dat(TBD_KEY, 1, 0x04);
					bdtest_sta_child = 1;
					delay(1000);
				break;
				case 1:
					key_err = 0;
					if(0 == KEY_RD_SPOT())		//������ɨ
					{
						key_err |= 0x04;
					}

					if(0 != KEY_RD_WIFI())		//�����
					{
						key_err |= 0x08;
					}
					#if BDTEST_LOGPRINT
					log_printf("\r\n[wywy]key:(%d)%d-%d\r\n",bdtest_sta_child,
					KEY_RD_SPOT(),
					KEY_RD_WIFI());
					#endif
					//key_err |= r_key_err;
					if(0 != key_err)
					{
						tx_com_dat(TPC_CT_KEY, 70, TPC_ERR_DIGI_IN, key_err);//�ϱ�����
						////bdtest_sta_child = 3;
					}
					else
					{
						tx_com_dat(TPC_CT_KEY, 70, TPC_ERR_NOERROR, 0);//�ϱ�����
						////tx_dp_dat(TBD_KEY, 1, 0x08);
						////bdtest_sta_child = 2;
						////delay(500);
					}
					tx_dp_dat(TBD_KEY, 1, 0x08);
					bdtest_sta_child = 2;
					delay(1000);
				break;
				case 2:
					key_err = 0;
					if(0 != KEY_RD_SPOT())		//������ɨ
					{
						key_err |= 0x04;
					}

					if(0 == KEY_RD_WIFI())		//�����
					{
						key_err |= 0x08;
					}
					#if BDTEST_LOGPRINT
					log_printf("\r\n[wywy]key:(%d)%d-%d\r\n",bdtest_sta_child,
					KEY_RD_SPOT(),
					KEY_RD_WIFI());
					#endif
					//key_err |= r_key_err;
					if(0 != key_err)
						tx_com_dat(TPC_CT_KEY, 100, TPC_ERR_DIGI_IN, key_err);//�ϱ�����
					else
						tx_com_dat(TPC_CT_KEY, 100, TPC_ERR_NOERROR, 0);//�ϱ�����
					bdtest_sta_child = 3;
				break;
				default:
					tx_dp_dat(TBD_KEY, 0, 0);
					bdtest_sta_child = 0;
					bdtest_sta = TPC_CT_REMOTE;
				break;
			}
			break;
		case TPC_CT_REMOTE:
			//log_printf("\r\n[wywy]Remote;%d\r\n", test_bd_com);
			if(SUCCESS == test_bd_com)
			{
				tx_com_dat(TPC_CT_REMOTE, 100 , TPC_ERR_NOERROR, 0);
				tx_com_dat(TPC_COM_DISPLAY, 100 , TPC_ERR_NOERROR, 0);
			}
			else
			{
				tx_com_dat(TPC_CT_REMOTE, 100 , TPC_ERR_COMM, 0);
				tx_com_dat(TPC_COM_DISPLAY, 100 , TPC_ERR_COMM, 0);
			}
			bdtest_sta = TPC_STOP_ALL;
			bdtest_sta_child = 0;
			break;
		default:
			bdtest_sta = TPC_STOP_ALL;
			break;
	}
//	bdtest_sta_old = bdtest_sta;
}

//���԰��ͨ�ų���
void proc_bd_test_mode(uint8_t *buf,uint8_t len)
{
	TEST_BD_PROC *proc_dat;

	proc_dat = (TEST_BD_PROC *)buf;

	//data_print(buf,len);
	if(
#if TBD_STOP_ALL > 0
	proc_dat->code >= TBD_STOP_ALL &&
#endif
		proc_dat->code <= TBD_KEY)
		tmd_rxbd_dat.unread_flag = 1;//����δ����ǩ
	switch(proc_dat->code)
	{
		case TBD_STOP_ALL://�˳�����ģʽ,���԰彫������ĵ�Դ���عر�,��λ����������⵽��������ʱ,����Ϊ������˳���ر�,�������,��������������ϱ�����Ϊ��Դ����������
			tx_msg(buf,sizeof(TEST_PROC));
			TargetSysReset();
			break;
		case TBD_START_ALL://�������ģʽ
			tx_msg(buf,sizeof(TEST_PROC));
			break;
		case TBD_MTOC_WHL_L:
			tmd_rxbd_dat.mtoc_whl_l = proc_dat->t_res_output;
			//log_printf("Get Data TBD_MTOC_WHL_L\r\n");
			break;
		case TBD_MTOC_WHL_R:
			tmd_rxbd_dat.mtoc_whl_r = proc_dat->t_res_output;
			break;
		case TBD_MTOC_SMT_L:
			tmd_rxbd_dat.mtoc_smt_l = proc_dat->t_res_output;
			break;
		case TBD_MTOC_SMT_R:
			tmd_rxbd_dat.mtoc_smt_r = proc_dat->t_res_output;
			break;
		case TBD_MTOC_MT_MID:
			tmd_rxbd_dat.mtoc_mt_mid = proc_dat->t_res_output;
			break;
		case TBD_SMT_L:
			tmd_rxbd_dat.cd_smt_l = proc_dat->t_res_input;
			//log_printf("\r\n cd_smt_l:%d-%d\r\n",tmd_rxbd_dat.cd_smt_l,proc_dat->t_res_input);
			break;
		case TBD_SMT_R:
			tmd_rxbd_dat.cd_smt_r = proc_dat->t_res_input;
			break;
		case TBD_MT_MID:
			tmd_rxbd_dat.cd_mt_mid = proc_dat->t_res_input;
			break;
		case TBD_BUM_SW:
			tmd_rxbd_dat.bum_sw = proc_dat->t_res_output;
			tmd_rxbd_dat.bum_sw_led = proc_dat->t_res_input;
			break;
		case TBD_SWSENSOR:
			tmd_rxbd_dat.sensors_sw = proc_dat->t_res_output;
			tmd_rxbd_dat.sensors_sw_led = proc_dat->t_res_input;
			break;
		default:
			break;
	}
}

//===============================================================================================================
//�ϻ����Գ����

#define BURNIN_LOG_EN	0
#define BURNIN_EX_TIME	600//�����رߵ��л�����,��λΪ��

static uint16_t burnin_work_near_timer;//�����ӱߵĹ���ʱ�������
static uint16_t burnin_work_timer;//����ʱ�������,��λΪ��,ǰ20����Ϊ���ر�,��20����Ϊ���ر�,�ܹ�����40���Ӻ����̽���,���ص�idleģʽ
static uint32_t bummech_ct,bumir_ct;//��е��ײ(���Ӵ�ʽ��ײ)������ ������ײ������
static uint32_t bummech_left_ct,bumir_left_ct;//���ر߼�����
static uint32_t bummech_right_ct,bumir_right_ct;//���ر߼�����

#if 1
//����N����
void burnin_back_off(int hw)
{
	//float dt;
//	int c;

	int i,tmr;
//	int dist=0
#if CALE_BY_BKHW
	int distance=motor.c_left_hw - hw * WHELE_HW;
#else
	int distance=navigat->distance - hw;
#endif

	CHECK_NAVI_STA();
	motor_run(GO_STOP,0,0,0);
	delay_ms(5);
	TIM5->CNT = 0;
	//motor.c_left_hw = 0;
	tmr = hw > 50 ?2000:500;

	motor_run(GO_BACK, BACK_OFF_PWM, 0, 0);
//	navigat->wheel_dir =-1;
	//navigat->is_walk = 1;

	i=0;

//	log_printf("befor bk,left(%3.1f,%3.1f)right(%3.1f,%3.1f)org(%d,%d)\r\n",navigat->x_org_f,navigat->y_org_f,navigat->x_org_r,,navigat->y_org_r,navigat->x_org,navigat->y_org);
	while(1)
	{

		if(TIM5->CNT >=10000)
		{
			TIM5->CNT = 0;
		//	www_idleintel_com();
			//log_printf("k");
			if(WALK_DIST()< distance || i++ >= tmr)
			{
				motor_run(GO_STOP,0,0,0);
				break;
			}
		}
	}
	//navigat->is_walk = 0;
	motor_run(GO_STOP,0,0,0);
	www_idleintel_com();
	//calc_gyro();
	burnin_coordinate_calcu(0);					//�����µ�����
	//log_printf("c2\r\n");
	coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
	//log_printf("o&");
	//gyro_whlmap();
	//log_printf("\r\n");
	//while(1);
//	log_printf("last bk,left(%3.1f,%3.1f)right(%3.1f,%3.1f)org(%d,%d)\r\n",navigat->x_org_f,navigat->y_org_f,navigat->x_org_r,,navigat->y_org_r,navigat->x_org,navigat->y_org);
	//log_printf("*(%d,%d,%f,0)-[%d,%d,%3.1f]\r\n\r\n",navigat->tx,navigat->ty,navigat->angle,navigat->x_org,navigat->y_org,sys->angle);
	//log_printf("hw=%d,x:%d,y%d,angle:%f,tx:%d,ty:%d\r\n",hw,navigat->x_org,navigat->y_org,navigat->angle,navigat->tx,navigat->ty);
	save_line_xy(navigat->x1_org,navigat->y1_org);
}


//�����Ƿ�360��תȦȦ
uint8_t burnin_near_round_360(float *m_angle,int16_t c_m_angle)
{
	int16_t i;
	uint8_t quadrant_1 = 0,quadrant2=0,quadrant3=0,quadrant4=0;
	uint8_t c_quadrant=0;
	//log_printf("[near_round_360]\r\n");
	for(i=0;i<MAX_C_M_ANGLE;i++)
	{

		if( m_angle[i] == 0)
			continue;
		if(m_angle[i] >=0 && m_angle[i] <=90)
			quadrant_1++;
		if(m_angle[i] >90 && m_angle[i] <=180)
			quadrant2++;
		if(m_angle[i] >180 && m_angle[i] <=270)
			quadrant3++;
		if((m_angle[i] >270 && m_angle[i] <=360) || m_angle[i] < 0)
			quadrant4++;
		//log_printf("quadrant=%3.1f,%d,%d,%d,%d\r\n",m_angle[i],quadrant_1,quadrant2,quadrant3,quadrant4);
		if(c_quadrant >=4 && i > 3)
		{
 			if((get_quadrant(m_angle[0]) == get_quadrant(m_angle[i]) && get_quadrant(m_angle[0]) == get_quadrant(m_angle[i-1])) ||
 			   (get_quadrant(m_angle[1]) == get_quadrant(m_angle[i]) && get_quadrant(m_angle[0]) == get_quadrant(m_angle[i-1]))  ||
 			   (get_quadrant(m_angle[2]) == get_quadrant(m_angle[i]) && get_quadrant(m_angle[0]) == get_quadrant(m_angle[i-1]))  ||
 			    (get_quadrant(m_angle[4]) == get_quadrant(m_angle[i]) && get_quadrant(m_angle[0]) == get_quadrant(m_angle[i-1]))||
 			     (get_quadrant(m_angle[5]) == get_quadrant(m_angle[i]) && get_quadrant(m_angle[0]) == get_quadrant(m_angle[i-1])))
			{
				//log_printf("found round...\r\n");
				for(c_m_angle = 0;c_m_angle<MAX_C_M_ANGLE;c_m_angle++)
					m_angle[c_m_angle] = 0;
				c_m_angle = 0;
				return 1;
			}
		}else
		{
			c_quadrant =count_quadrant(quadrant_1,quadrant2,quadrant3,quadrant4);	//ȡ������

		}

	}
	//log_printf("quadrant=%d,%d,%d,%d\r\n",quadrant_1,quadrant2,quadrant3,quadrant4);
	/**/
	if(c_quadrant >=4 && (quadrant_1 >=3 || quadrant2 >=3 || quadrant3 >=3 || quadrant4>=3))
	{
		//log_printf("found round2...\r\n");
		for(c_m_angle = 0;c_m_angle<MAX_C_M_ANGLE;c_m_angle++)
				m_angle[c_m_angle] = 0;
		c_m_angle = 0;
		return 1;
	}

	return 0;
}

//????????
extern GRYO_T *gyro;
extern gyro_t	*mgyro;
void burnin_gyro_mapwhl(void)
{

	if(gyro->isOK ==0)
	{
		*(mgyro->angle)  = 0;
		//g_printf("Warning!!No copy allowed.Copyright idleintel\r\n");
		return ;
	}
//	disable_irq();

	*(mgyro->x_org_f)  = *(mgyro->x_org) - *(mgyro->radius) * gyro->sinA ; // WHELE_HW;
	*(mgyro->y_org_f)  =  *(mgyro->y_org) + *(mgyro->radius) * gyro->cosA ;// WHELE_HW;

	*(mgyro->x_org_r)  = *(mgyro->x_org) + *(mgyro->radius) * gyro->sinA ; // WHELE_HW;
	*(mgyro->y_org_r)  =  *(mgyro->y_org) - *(mgyro->radius) * gyro->cosA ;// WHELE_HW;

	*(mgyro->x_org_t)  = (*(mgyro->x_org) + *(mgyro->radius) * gyro->cosA)*ORG_TO_FRONT ; // WHELE_HW;
	*(mgyro->y_org_t)  =  (*(mgyro->y_org) + *(mgyro->radius) * gyro->sinA)*ORG_TO_FRONT ;// WHELE_HW

	//*(mgyro->x_org_t)  = (navigat->k_x_org + *(mgyro->radius) * gyro->cosA)*ORG_TO_FRONT ; // WHELE_HW;
	//*(mgyro->y_org_t)  =  (navigat->k_y_org + *(mgyro->radius) * gyro->sinA)*ORG_TO_FRONT ;// WHELE_HW
//	enable_irq();

}

void burnin_robot_turn_deg(uint8_t dir,int pwm,float agle)
{
//	int pwm=600;
	//int dg_turn=deg - 2.5;
//	int c=0;
	float dis_agle,b_agle;
	float out_deg;
	int midl=0;
	uint32_t t;
	www_idleintel_com();
	//calc_gyro();
	printf_power_sta();
	//log_printf("turn_charge,dir=%s,agle=%f\r\n",get_motor_sta(dir),agle);
	motor.c_left_hw = motor.c_right_hw = 0;
	//log_printf("1\r\n");
	CHECK_NAVI_STA();

	b_agle = sys->angle;
	//log_printf("2\r\n");
	//burnin_coordinate_calcu();
	//gyro_whlmap();
	navigat->is_walk = 0;
	//log_printf("3\r\n");
	turn_round_pid(0);
	//log_printf("4\r\n");
	motor_run(dir,pwm,0,0);
	sys->right_pwm = sys->left_pwm = pwm;
	TIM5->CNT = 0;
	//���м���������ת����
	if(MIDLE_HAVE_IRDA2() || *(navigat->near.pid->adc2) >= MAX_IR_SIDE2 )
		midl = 1;

	out_deg = NO_ANGLE_V;
	t=0;
	burnin_coordinate_calcu(0);
	while(1)
	{
		get_sensers(&sys->gSta);
		CHECK_NAVI_STA();
		//laser_calc_move(navigat,MAX_LASR_DIST);
#if LASER_SIDE
		laser_scan_dist(navigat,LASER_CALC_GO_FW);
#endif
		if(TIM5->CNT >=5000)
		{

			TIM5->CNT = 0;
			turn_round_pid(TURN_SPEED);
			www_idleintel_com();
			//calc_gyro();

			dis_agle = b_agle -sys->angle;
			if(dis_agle > 180)
				dis_agle = 360 - dis_agle;
			if(dis_agle <-180)
				dis_agle += 360;
			if(dis_agle >=agle || dis_agle <=-agle)
				break;
			//�м�û�к��⣬���˳�
			if(out_deg == NO_ANGLE_V)
			{
				if(agle == WALL_LOST_DEG && midl && (dis_agle >=20 || dis_agle <=-20))
				{
					if(sys->g_sta[2] < 300 && sys->g_sta[3] < 150 && sys->g_sta[4] < 280 &&
						*(navigat->near.pid->adc2) < MAX_IR_SIDE2)
					{

						//out_deg = sys->angle;

						if(*(navigat->near.pid->adc2) < 1700)
						{
							//log_printf("found V_angle=%3.1f,%d\r\n",sys->angle,*(navigat->near.pid->adc2));
							break;
						}
						//log_printf("\r\n");
					}
				}
			}else
			{
				if(disfloat(sys->angle,out_deg) > 5)	//��ת10��
				{
					//log_printf("VOK,v_agle=%3.1f\r\n",sys->angle);
					break;
				}
			}
			if(t++>=3000)
			{
				//log_printf("timeout\r\n");
				break;
			}


		}

		//log_printf("%3.1f,%3.1f\r\n",sys->angle,dis_agle);
	}
	motor_run(GO_STOP,0,0,0);
	DELAY_MS(100);
	//delay_ms_sensers(200);
#if LASER_SIDE
		calc_scan_laser(navigat,1);
#endif
//	www_idleintel_com();
	//gyro_whlmap();
	//gyro_mapwhl();
	burnin_gyro_mapwhl();
	//navigat->is_walk =1;
	//log_printf("..OK,c=%d,%d,agl=%3.1f\r\n",motor.c_left_hw,motor.c_right_hw,sys->angle);

}
#define BURN_MAX_KXXYY		80
static int16_t b_kxx[BURN_MAX_KXXYY];
static int16_t b_kyy[BURN_MAX_KXXYY];
char burnin_check_round_bum(uint8_t type)
{
	static uint16_t idx=0;
	short i;
	if(type ==0)
	{
		idx = 0;
		return 0;
	}
	if(idx >=BURN_MAX_KXXYY)
		idx = 0;
	b_kxx[idx] = X_NOW;
	b_kyy[idx++] = Y_NOW;

	for(i=1;i<BURN_MAX_KXXYY;i++)
	{
		if(dis_xy(b_kxx[0],b_kxx[i]) > 5)
			return 0;
		if(dis_xy(b_kyy[0],b_kyy[i]) > 5)
			return 0;
	}
	return 1;

}
#if CALE_BY_FRON
int burnin_coordinate_calcu(uint8_t type)
{

	static int cc=0;
//	static int16_t cc=0;
	int32_t x_org1;//,x_org2;//,
	int32_t y_org1;//,y_org2;//,;
#if WALK_PRINTF_CALC
	static  short xx=0,yy=0;
#endif

	int32_t x_org2;	//����
	int32_t y_org2;	//����
	int32_t x_org3,y_org3;	//ǰ��
	float x_org_t,y_org_t;


	float x_org_f,y_org_f;
	float x_org_r,y_org_r;
	cc++;
	if(cc <2 && type==1)
		return 1;
	cc = 0;
//
	disable_irq();
	x_org_f = navigat->x_org_f ;
	y_org_f = navigat->y_org_f;

	x_org_r = navigat->x_org_r;
	y_org_r = navigat->y_org_r;

 	x_org_t= navigat->x_org_t;
 	y_org_t= navigat->y_org_t;
 	enable_irq();
 	//����
 	x_org1	=	x_org_calc(x_org_f);
 	y_org1	= 	y_org_calc(y_org_f);
 	//ʹ��ǰ��
	x_org3 = x_org_calc_f(x_org_t);
	y_org3 = y_org_calc_f(y_org_t);

	//ʹ������
	x_org2	=	x_org_calc_r(x_org_r);
	y_org2	= 	y_org_calc_r(y_org_r);


	/**/
	if(motor.c_left_hw > 100)
	{
		if(disXY(x_org1,x_org2) > 30 || disXY(y_org1,y_org2) > 30)
		{
			navigat->x_org	= 	x_org1;
			navigat->y_org	=  	y_org1;
			//log_printf("[coor_calcu]err x=%d,%d,y=%d,%d,%d\r\n",x_org1,x_org2,y_org1,y_org2,motor.c_left_hw);
			navigat->x_org	= 	(x_org1+x_org2+x_org3) / 3;
			navigat->y_org	=  	(y_org1+y_org2+y_org3) / 3;
			gyro_whlmap();
			return 0;
		}



		if(disXY(x_org1,x_org3) > 30 || disXY(y_org1,y_org3) > 30)
		{
			//log_printf("[coor_calcu]errX=%d,%d,Y=%d,%d,%d\r\n",x_org1,y_org1,x_org3,y_org3,motor.c_left_hw);
			navigat->x_org	= 	(x_org1+x_org2+x_org3) / 3;
			navigat->y_org	=  	(y_org1+y_org2+y_org3) / 3;
			gyro_whlmap();
			return 0;
		}
	}
	/**/
	//if(cc++>=100)
	{
				//log_printf("%d %d|%d %d|%d %d\r\n",x_org1,y_org1,x_org2,y_org2,x_org3,y_org3);
				//	cc = 0;
	}



	navigat->x_org	= 	x_org1;
	navigat->y_org	=  	y_org1;
	gyro_whlmap();

/*
	log_printf("*(%d,%d,%3.1f)[%d,%d|%d,%d|%d,%d]*\r\n",navigat->tx,navigat->ty,sys->angle,
					navigat->x_org,navigat->y_org,x_org1,y_org1,x_org2,y_org2);

	log_printf("*%3.3f[%d,%d|%d,%d|%d,%d](%d,%3.1f,%3.1f)(%d,%3.1f,%3.1f)*\r\n",sys->angle,
						navigat->x_org,navigat->y_org,x_org1,y_org1,x_org2,y_org2,
						motor.c_left_hw,navigat->x_org_f,navigat->y_org_f,
						motor.c_right_hw,navigat->x_org_r,navigat->y_org_r);


	log_printf("*(%d,%d,%3.3f)[%d,%d|%d,%d](%d,%d,%d)*\r\n",navigat->tx,navigat->ty,sys->angle,
						navigat->x_org,navigat->y_org,x_org2,y_org2,
						motor.c_right_hw,motor.c_right_hw,navigat->distance);
	*/
#if WALK_PRINTF_CALC
	if(xx != X_NOW || yy != Y_NOW)
	{
		xx 	= X_NOW;
		yy	= Y_NOW;
		//log_printf("*(%d,%d,%3.1f,0)[%d,%d,%d,%d,%d]*\r\n",navigat->tx,navigat->ty,sys->angle,navigat->x_org,navigat->y_org,
		//												x_org2,y_org2,navigat->distance);
	}

#endif



	return 1;



}
#else
int burnin_coordinate_calcu(uint8_t type)
{


	int32_t x_org1;//,x_org2;//,
	int32_t y_org1;//,y_org2;//,;
#if WALK_PRINTF_CALC
	static  short xx=0,yy=0;
#endif

#if CALE_ADJ_RIGHT
	int32_t x_org2;//,
	int32_t y_org2;//,;
#endif
#if CALE_BY_FRONT
//	static int distanc=0;
	int32_t x_org_front,y_org_front;
	float x_org_t,y_org_t;
	//int ret=0; asd

#endif

	float x_org_f,y_org_f;
#if CALE_ADJ_RIGHT
	float x_org_r,y_org_r;
#endif
//
	disable_irq();
	x_org_f = navigat->x_org_f ;
	y_org_f = navigat->y_org_f;
#if CALE_ADJ_RIGHT
	x_org_r = navigat->x_org_r;
	y_org_r = navigat->y_org_r;
#endif
 #if CALE_BY_FRONT
 	x_org_t= navigat->x_org_t;
 	y_org_t= navigat->y_org_t;
 #endif
 	enable_irq();

 	x_org1	=	x_org_calc(x_org_f);
 	y_org1	= 	y_org_calc(y_org_f);
 	//ʹ��ǰ��
#if CALE_BY_FRONT
	x_org_front = x_org_calc_f(x_org_t);
	y_org_front = y_org_calc_f(y_org_t);

	navigat->k_x_org	= 	x_org_front;
	navigat->k_y_org	=  	y_org_front;
	coord_org2map(navigat->k_x_org,navigat->k_y_org,&navigat->kx,&navigat->ky);
#endif
	//log_printf("%d %d %d %d %d %d\r\n",);
	//ʹ������У׼
#if CALE_ADJ_RIGHT
		if(navigat->adj_run == FALSE)
		{
			x_org2	=	x_org_calc_r(x_org_r);
			y_org2	= 	y_org_calc_r(y_org_r);

			//TEST MICONY
			//micony 2017-12-27��֪��Ϊ�Σ������ӱߵ�ʱ���������ô�
			//if(sys->nsta == NO_SIDE_NEAR)
			{
				if(disXY(x_org1,x_org2) > 160 || disXY(y_org1,y_org2) > 160)
				{
					navigat->x_org	= 	x_org1;
					navigat->y_org	=  	y_org1;
					//log_printf("[coor_calcu]err x=%d,%d,y=%d,%d\r\n",x_org1,x_org2,y_org1,y_org2);
					return 0;
				}
			}
		}


#if CALE_BY_FRONT		//ʹ��ǰ�ֲ���У׼
		//navigat->x_org	= 	(x_org1 + x_org_front)>> 1;
		//navigat->y_org	=  	(y_org1 + y_org_front)>> 1;
		if(disXY(x_org1,x_org_front) > 20 || disXY(y_org1,y_org_front) > 20)
		{
			//log_printf("[coor_calcu]errX=%d,%d,Y=%d,%d\r\n",x_org1,x_org2,x_org_front,y_org_front);
			return 0;
		}
		gyro_fwhlmap();
#endif

		//navigat->x_org	= 	(x_org1 +x_org2)>>1 ;// + (int32_t)((float)(x_org2 - x_org1)*0.6f);
		//navigat->y_org	=  	(y_org1 +y_org2)>>1 ;//+ (int32_t)((float)(y_org2 - y_org1)*0.6f);
		navigat->x_org	= 	x_org1;
		navigat->y_org	=  	y_org1;

#else
		navigat->x_org	= 	x_org1;
		navigat->y_org	=  	y_org1;

#endif

/*
	log_printf("*(%d,%d,%3.1f)[%d,%d|%d,%d|%d,%d]*\r\n",navigat->tx,navigat->ty,sys->angle,
					navigat->x_org,navigat->y_org,x_org1,y_org1,x_org2,y_org2);

	log_printf("*%3.3f[%d,%d|%d,%d|%d,%d](%d,%3.1f,%3.1f)(%d,%3.1f,%3.1f)*\r\n",sys->angle,
						navigat->x_org,navigat->y_org,x_org1,y_org1,x_org2,y_org2,
						motor.c_left_hw,navigat->x_org_f,navigat->y_org_f,
						motor.c_right_hw,navigat->x_org_r,navigat->y_org_r);


	log_printf("*(%d,%d,%3.3f)[%d,%d|%d,%d](%d,%d,%d)*\r\n",navigat->tx,navigat->ty,sys->angle,
						navigat->x_org,navigat->y_org,x_org2,y_org2,
						motor.c_right_hw,motor.c_right_hw,navigat->distance);
	*/
#if WALK_PRINTF_CALC
	if(xx != X_NOW || yy != Y_NOW)
	{
		xx 	= X_NOW;
		yy	= Y_NOW;
		//log_printf("*(%d,%d,%3.1f,0)[%d,%d,%d,%d,%d]*\r\n",navigat->tx,navigat->ty,sys->angle,navigat->x_org,navigat->y_org,
		//												x_org2,y_org2,navigat->distance);
	}

#endif



	return 1;



}


//�ж��Ƿ�ʱ(���趨��ʱ��)
//ע�⣺msec�������Ϊ0ʱ�����
uint8_t burnin_mstimeout(uint32_t *timep,uint32_t msec)
{
	uint32_t time,diff;
	uint8_t result=0;
	time=msTmr;
	diff=time- *timep;
	if(msec==0)
	{
		*timep=time;
		result=1;
	}
	else if(diff>=msec)
	{
		//diff += (msec + (msec >> 1));
		result = (uint8_t)(diff/msec);
		*timep=time;
	}
	return result;
}

#endif

int burnin_near_wall_pid(float *agle,int c_lost)
{
	static int c_pid=0;
	NEAR_WALL *near;

	near = &navigat->near;
	if(c_pid ++ >=2)
	{
		c_pid = 0;
		//if(near->n_sta !=NO_SIDE_NEAR && near->pid != NULL)		//ǿ���ӱ�
		{
			return (near_wall_pid(1,near->pid,agle));
		}/*else
		{
			if(near_wall_pid(0,&l_near_pid,agle) ==0)
			{
				return (near_wall_pid(0,&r_near_pid,agle));
			}
			return 1;
		}*/

	}
	return 1;

}
#endif
/*****************************************************************************
 * ��������:
 * ��    ��:	type  :
 *						 GO_NEAR_TYPE_NO		0x00		//��ͨ�ӱ�
 *						 GO_NEAR_TYPE_NAVI		0x01		//�м�����ܵ������յ㣬���˳�ȥ����
 *						 GO_NEAR_TYPE_ADJ	   0x02
 *��������:	�ӱߵ������ҵط������������Ŀ�ĵأ����˳�
 *
 *****************************************************************************/
 static uint32_t burninbeep_timer = 0;
char motor_go_burnin(uint8_t n_sta ,short tox,short toy,uint8_t type,uint8_t is_save)
{
//	int xx=0,yy=0,x1=0,y1=0;
	int calue=0;
//	int by = navigat->y_org;
//	uint8_t gSta;
	uint8_t sta;
//	int	lx=0,ly=0;
	short llx=0,lly=0;
//	short lx=X_NOW,ly=Y_NOW;
//	int bx = X_NOW;
	short by = Y_NOW;
//	short bx = X_NOW;
//	short nx1,ny1;
	float lagle;		//��¼�����ӱ߽Ƕȣ����ڼ����Ƿ�ת�����
	uint16_t	gSta;
	uint32_t	t_begin;//��ʼ��ʱ��
	int ret_calc=1;
	uint16_t c_dock_data=0;
	int16_t c_lost=0;
	uint16_t c_round=0;		//תȦ�Ĵ���


	short x_begin_line=0,y_begin_line = 0;		//һ���߿�ʼ��X��Y������

	uint8_t sec1s_ct = 0;// 1��ļ�����
	///uint8_t time5ms_flag = 0;//5msʱ���־


	u8 irData_bak[6];

	NEAR_WALL *near = &navigat->near;
	float m_angle[MAX_C_M_ANGLE];			//���������20����ײ�ĽǶ�
	int16_t c_m_angle=0;
	int16_t c_near_wall = 0;

	for(c_m_angle = 0;c_m_angle<MAX_C_M_ANGLE;c_m_angle++)
		m_angle[c_m_angle] = 0;
	c_m_angle = 0;
	CHECK_NAVI_STA_RT(0);
	pd_gyro_int(GO_NEAR_PWM_FAST);
	navigat->out =navigat->angle;
	navigat->distance = 0;
	motor.c_left_hw = motor.c_right_hw = 0;
	cord_calc_store(0);
	gyro_whlmap();
	robot_whele_stop(0);
	motor_run(GO_FORWARD, GO_NEAR_PWM_FAST, 0, 0);
	if(n_sta & 0x80)
		init_near_wall_navi(n_sta & 0x7F);
	else
		init_near_wall_navi(NO_SIDE_NEAR);
	n_sta &=0x7F;
	navigat->is_walk =1;
#if BURNIN_LOG_EN
	log_printf("[motor_go_edgeways]sta=%d(%d),type=%d,is_save=%d,now=(%d,%d,)to=(%d,%d,)\r\n",navigat->near.n_sta,n_sta,type,is_save,X_NOW,Y_NOW,tox,toy);
#endif
	//navigat->wheel_dir = 1;
	sta = sys->sState;
	lagle = sys->angle;
	mstimeout(&t_begin,0);
	MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value)
#if BURNIN_LOG_EN
	printf_power_sta();
#endif
	burnin_mstimeout(&burninbeep_timer,0);
	while(1)
	{

		sec1s_ct = burnin_mstimeout(&burninbeep_timer,1000);
		proc_uart_task();
		get_sensers(&sys->gSta);
		if(sys->sState !=sta)
		{
#if BURNIN_LOG_EN
			log_printf("\r\nmode changed\r\n");
#endif
			tx_com_burnin(NEAR_BY_IRDA, burnin_work_timer, bummech_left_ct, bumir_left_ct, bummech_right_ct, bumir_right_ct);
			return 0;
			//log_printf("\r\nerr:%d\r\n",sys->work_errcode);
			//sys->sState = SYS_RANDOM;
			//MOTOR_POWER_ON();
			//motor_run(GO_FORWARD, GO_NEAR_PWM_FAST, 0, 0);
			//MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value)
		}

		if(sec1s_ct)
		//if(mstimeout(&sys->t_loop,1000))
		//if(TIM5->CNT >=5000)
		{
			//TIM5->CNT = 0;
			//time5ms_flag = 1;

			//if(sec1s_ct ++ > 199)
			{
				//sec1s_ct = 0;
				if(near->n_sta == LEFT_SIDE_NEAR)
				{
					bummech_left_ct = bummech_ct;
					bumir_left_ct = bumir_ct;
				}
				else if(near->n_sta == RIGHT_SIDE_NEAR)
				{
					bummech_right_ct = bummech_ct;
					bumir_right_ct = bumir_ct;
				}
				//log_printf("--------------------------------------\r\n");
				burnin_work_timer += sec1s_ct;
				burnin_work_near_timer += sec1s_ct;
				if(sys->work_errcode != 0)
					log_printf("\r\nerr:%d\r\n",sys->work_errcode);
				tx_com_burnin(near->n_sta, burnin_work_timer, bummech_left_ct, bumir_left_ct, bummech_right_ct, bumir_right_ct);
				if(burnin_work_near_timer > BURNIN_EX_TIME)//������,�ݶ�5����
				{
					burnin_work_near_timer = 0;
					motor_run(GO_STOP,0,0,0);
					return RET_NEAR_TIMEOUT;
				}
			}
		}
		//if(sys->work_errcode != 0 && sys->c_left==0 && sys->c_right == 0)
		//{
		//}
			//return RET_NEAR_ERROR;
		CHECK_NAVI_STA_RT(0);
		//��ײ
		if(sys->gSta & (MASK_BUM_MIDL) ||(sys->gSta & (MASK_BUM_LEFT | MASK_BUM_LEFT2 | MASK_BUM_RIGHT | MASK_BUM_RIGHT2))||  ret_calc==0 ||  c_lost >=MAX_NEAR_LOST || MIDLE_IRDA())
		{
//			gSta = sys->gSta;	//��¼��ײ��״̬

			motor_run(GO_STOP,0,0,0);
#if BURNIN_LOG_EN
			log_printf("BUM\r\n");
#endif
			//delay_ms(200);
			if(sys->gSta & (MASK_BUM_MIDL))
			{
				if(sys->fall_sta)		//���䣬��
					burnin_back_off(BACK_HW*8);
				else
					burnin_back_off(BACK_HW);
				bummech_ct ++;//��е��ײ�����ļ���

			}
			if(MIDLE_IRDA())
				bumir_ct ++;//��ǰ��������ײ�ļ���
			//delay_ms_sensers(200);
			burnin_coordinate_calcu(0);														//�����ԭʼ������ϵ
			coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty); //����ϵת��
			motor_run(GO_STOP,0,0,0);
#if BURNIN_LOG_EN
			log_printf("\r\n-----go_edgeways bum(%d,%d,%d,%f,%f),gsta=%d,irda=(%d,%d,%d,%d)angle=%3.1f,ret=%d,lost=%d,%d\r\n",navigat->tx,navigat->ty,motor.c_left_hw,navigat->x_org_f,navigat->y_org_f,
							sys->gSta,sys->g_sta[0],sys->g_sta[1],sys->g_sta[5],sys->g_sta[6],sys->angle,ret_calc,c_lost,c_m_angle);
#endif

			if(ret_calc==0)
					burnin_gyro_mapwhl();
			if(c_m_angle >=MAX_C_M_ANGLE)
				c_m_angle = 0;
			m_angle[c_m_angle++] = sys->angle;		//���ֽǶ�

			if(burnin_near_round_360(m_angle,c_m_angle))
			{
#if BURNIN_LOG_EN
				log_printf("found round...go line...\r\n");
#endif

				init_near_wall_navi(NO_SIDE_NEAR);
				for(c_m_angle = 0;c_m_angle<MAX_C_M_ANGLE;c_m_angle++)
					m_angle[c_m_angle] = 0;
				c_m_angle = 0;
				goto l_mgo_edeways_burnin;
			}



			if(!(ret_calc==0 || c_lost >=MAX_NEAR_LOST))
				c_round = 0;
			if(ret_calc==0)
					gyro_mapwhl();
			gSta = sys->gSta;
			//motor_run(GO_STOP,0,0,0);
//			if(sys->work_mod & MWO_MOP)
		//		delay_ms(200);
			//www_idleintel_com();
			lagle = sys->angle;
			//init_near_wall_navi(n_sta);		//��ײ�󣬲ſ�ʼһֱ�ر�

			navigat->distance = 0;
			motor.c_left_hw = 0;

			if( type == GO_NEAR_TYPE_DOCK)
			{
				//�ҳ��׮
				read_ir_data_timeout(irData_bak,0);

				if(irData_bak[IR_L_PIN_NUM] ||  irData_bak[IR_ML_PIN_NUM] || irData_bak[IR_R_PIN_NUM] || ir_dock_insight(1))
				{
					//int16_t i;
					//for(i=0;i<3;i++)
					{
						//delay_ms(80);
						//read_ir_data(irData_bak,0);

#if BURNIN_LOG_EN
						log_printf("[motor_go_edgeways]bum  dock(%d,%d,%d)%d\r\n",
						irData_bak[IR_L_PIN_NUM] ,irData_bak[IR_ML_PIN_NUM],irData_bak[IR_R_PIN_NUM],c_dock_data);
#endif
						//if(irData_bak[IR_L_PIN_NUM] ||  irData_bak[IR_M_PIN_NUM] || irData_bak[IR_R_PIN_NUM])
						if(1)
						{

							c_dock_data++;
#if BURNIN_LOG_EN
							log_printf("ir=%d,%d,%d,c=%d\r\n",irData_bak[IR_L_PIN_NUM] , irData_bak[IR_ML_PIN_NUM] , irData_bak[IR_R_PIN_NUM],c_dock_data);
#endif
							if(c_dock_data >=3)
							{
#if BURNIN_LOG_EN
								log_printf("found dock..\r\n");
#endif
								goto l_go_out_for_dock_burnin;
							}
						}else
							c_dock_data=0;
					}

				}else
					c_dock_data = 0;
			}


			/**************************************************************************
				���tox��toy���������ã��򵽵��ˣ���ͣ������
			*****************************************************************************/
			if(  (((X_NOW == tox || tox ==0) && (Y_NOW == toy || toy==0)) && tox && toy) || 	//������
				(type == GO_NEAR_TYPE_ADJ &&  ((by > toy && Y_NOW < toy) || (by < toy && Y_NOW > toy)))) //adj��ȥ��Y�ᳬ����
			{
#if BURNIN_LOG_EN
				log_printf("xy ok(%d,%d,%3.1f)\r\n",X_NOW,Y_NOW,sys->angle);
#endif
				motor_run(GO_STOP,0,0,0);
				//delay_ms_sensers(200);
				//burnin_coordinate_calcu();														//�����ԭʼ������ϵ
				//coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
				return RET_NEAR_OK;
			}


			if(near->n_sta ==NO_SIDE_NEAR)
			{
				init_near_wall_navi(n_sta);
			}

			if( (ret_calc==0 || c_lost >= MAX_NEAR_LOST)&& *(navigat->near.pid->adc) < navigat->near.pid->min_adc )
			{
				c_round++;
#if BURNIN_LOG_EN
				log_printf("lost or calc error(%d,%d),cround=%d\r\n",ret_calc,c_lost,c_round);
#endif
				/*
				if(c_round >=3)
				{
					log_printf("big round not near\r\n");
					init_near_wall_navi(NO_SIDE_NEAR);
					goto l_go_edeways;
				}
				*/
				if(ret_calc)
				{
					gyro_whlmap();
				}
				if(near->n_sta == RIGHT_SIDE_NEAR)
				{
					burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,NEAR_LOST_DEG);
					goto l_mgo_edeways_burnin;
				}else if(near->n_sta == LEFT_SIDE_NEAR)
				{
					burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,NEAR_LOST_DEG);
					goto l_mgo_edeways_burnin;
				}

			}

			if(near->n_sta == RIGHT_SIDE_NEAR)
			{
				if(burnin_check_round_bum(1))
					burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,12);
				else
				{
					if(BUM_LEFT(gSta) || MIDLE_HAVE_IRDA2() || RIGHT_IR_BUM2())
					{
						burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,WALL_LOST_DEG);
					}else if(sys->g_sta[6] > 900 && sys->g_sta[5] > 900)//��ǽ�ˣ�������ӱߣ���Ҫת̫��
					{
						burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,12);
					}else
						burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,25);
				}

			}else if(near->n_sta == LEFT_SIDE_NEAR)
			{
				if(burnin_check_round_bum(1))
					burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,12);

				else
				{
					if(BUM_RIGHT(gSta) || MIDLE_HAVE_IRDA2() || LEFT_IR_BUM2())
					{
						burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,WALL_LOST_DEG);
					}
					else if(sys->g_sta[1] > 900 && sys->g_sta[2] > 900)
					{
						burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,12);
					}
					else
					{
						burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,25);
					}
				}

			}
			else
			{
#if BURNIN_LOG_EN
				log_printf("RET_NEAR_ERROR,nsta=%d\r\n",n_sta);
#endif
				return RET_NEAR_ERROR;
			}
l_mgo_edeways_burnin:

			//תȦ�ˡ���
			c_lost=0;
			//burnin_coordinate_calcu();
			motor_run(GO_STOP,0,0,0);
		//	log_printf("after bk(%d,%d,%d,%3.3f,%3.3f,%3.3f)\r\n==============\r\n",navigat->tx,navigat->ty,motor.c_left_hw,navigat->x_org_f,navigat->y_org_f,sys->angle);
			//if(ccc++ >=5)
			//	while(1);
			navigat->distance = 0;
			navigat->is_walk = 1;
			pd_gyro_int(GO_NEAR_PWM_FAST);
			navigat->out =sys->angle;
			cord_calc_store(0);
			gyro_whlmap();
			motor.c_left_hw = motor.c_right_hw = navigat->distance = 0;

			motor_run(GO_FORWARD, GO_NEAR_PWM_FAST, 0, 0);
			navigat->near.pid->c_lost = 0;

			navigat->near.pid->c_lost = 0;
			navigat->near.pid->c_lost_flag = 0;			//ʧȥǽ�ı�־     2019 02 15 add
			sys->g_t_walk = 0;
			ret_calc = 1;
			//��¼����λ��
			x_begin_line = X_NOW;
			y_begin_line = Y_NOW;
			c_near_wall = 0;
		//	motor_run(GO_RIGHT_RD,0,0,0);
		//	navigat->near_sta = LOST_WALL_RIGHT;
		}

		if(TIM5->CNT >=5000)
		//if(time5ms_flag)
		{
			TIM5->CNT = 0;
			//time5ms_flag = 0;
			navigat->out = format_agle(navigat->out,ANGLE_360);
			proc_line_pid(navigat->out);
			burnin_near_wall_pid(&navigat->out,5);
			//navigat_near_wall_pid(&navigat->out,5);
			if(near->n_sta ==NO_SIDE_NEAR)
			{
				if(RIGHT_ADC() >= cfg->lock_right_adc)
					init_near_wall_navi(RIGHT_SIDE_NEAR);
				else if(LEFT_ADC() >= cfg->lock_left_adc)
					init_near_wall_navi(LEFT_SIDE_NEAR);
			}

			if(robot_whele_stop(1))
					continue;			//ֱ�ӳ���������ײ��׼��



			if( type == GO_NEAR_TYPE_DOCK )
			{
				//�ҳ��׮
			//	c_dock = 0;
				read_ir_data_timeout(irData_bak,0);
				if(irData_bak[IR_L_PIN_NUM] ||	irData_bak[IR_ML_PIN_NUM] || irData_bak[IR_R_PIN_NUM] || ir_dock_insight(1))
				{
#if BURNIN_LOG_EN
					log_printf("[motor_go_edgeways]found dock(%d,%d,%d)%d\r\n",
						irData_bak[IR_L_PIN_NUM] ,irData_bak[IR_ML_PIN_NUM],irData_bak[IR_R_PIN_NUM],c_dock_data);
#endif
					//if(c_dock_data++ >=3)
					{
#if BURNIN_LOG_EN
						log_printf("[motor_go_edgeways]found dock\r\n");
#endif
						motor_run(GO_STOP,0,0,0);
						//delay_ms_sensers(200);
						burnin_coordinate_calcu(0); 													//�����ԭʼ������ϵ
						coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty); //����ϵת��
						goto	l_go_out_for_dock_burnin;
					}

				}else
					c_dock_data = 0;
			}


			if( *(navigat->near.pid->adc) > navigat->near.pid->min_adc) //�ӱ�
			  lagle = sys->angle;
			else		//�ӱ���ʧ��ת�ĽǶȳ���180�ȣ���ʧ���˳�
			{
				if(disfloat( lagle , sys->angle) > 180)
				{
#if BURNIN_LOG_EN
					log_printf("lost over(%d,%d,%3.1f,%3.1f)\r\n",X_NOW,Y_NOW,sys->angle,lagle);
#endif
					motor_run(GO_STOP,0,0,0);
					//delay_ms_sensers(200);
					burnin_coordinate_calcu(0);														//�����ԭʼ������ϵ
					coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
					return RET_NEAR_ERROR;
				}
			}
#if CALE_BY_FRON
				ret_calc = burnin_coordinate_calcu(1);														//�����ԭʼ������ϵ
#endif
			if(calue++ >=40)
			{

				if(c_near_wall ++ >=30)
				{

					for(c_m_angle = 0;c_m_angle<MAX_C_M_ANGLE;c_m_angle++)
						m_angle[c_m_angle] = 0;
					c_m_angle = 0;
					c_near_wall = 0;
#if BURNIN_LOG_EN
					log_printf("log near\r\n");
#endif
				}



				calue = 0;
#if !CALE_BY_FRON
				ret_calc = burnin_coordinate_calcu(0);														//�����ԭʼ������ϵ
#endif
				coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
				//����0�ȣ�90�ȣ�80�ȵķ���,ˢ������
				/**/
				if(x_begin_line != X_NOW && y_begin_line != Y_NOW)
				{
//					ajust_xy_by_near_line(x_begin_line,y_begin_line,X_NOW,Y_NOW,sys->angle,LINE_TYPE_LOST,n_sta);
					x_begin_line = X_NOW;
					y_begin_line = Y_NOW;
				}


				if( *(navigat->near.pid->adc) > navigat->near.pid->min_adc) //�ӱ�
				{
					//c_near++;
					c_round = 0;

				}
				else if(near->n_sta !=NO_SIDE_NEAR)	//ǿ���ӱߣ��������ʧ�ĸ���
					c_lost ++;

				if(llx!=navigat->tx || lly!=navigat->ty)
				{
#if WALK_PRINTF	&& BURNIN_LOG_EN
					log_printf("*(%d,%d,%3.1f,0)-[%3.1f,%3.1f,0]*\r\n",navigat->tx,navigat->ty,sys->angle,navigat->x_org_f,navigat->y_org_f);
#endif
					llx = navigat->tx;
					lly = navigat->ty;
				}
				/**************************************************************************
					���tox��toy���������ã��򵽵��ˣ���ͣ������
				*****************************************************************************/
				if(  (((X_NOW == tox || tox ==0) && (Y_NOW == toy || toy==0)) && (tox || toy)) || 	//������
				(type == GO_NEAR_TYPE_ADJ &&  ((by > toy && Y_NOW < toy) || (by < toy && Y_NOW > toy)))) //adj��ȥ��Y�ᳬ����
				{
#if BURNIN_LOG_EN
						log_printf("xy ok(%d,%d,%3.1f)\r\n",X_NOW,Y_NOW,sys->angle);
#endif
						motor_run(GO_STOP,0,0,0);
						//delay_ms_sensers(200);
						burnin_coordinate_calcu(0);														//�����ԭʼ������ϵ
						coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
						return RET_NEAR_OK;
				}

			

				if(type == GO_NEAR_DRAW_MAP)		//����ͼ
				{
					if(Y_NOW < 100 &&  Y_NOW > 98 && disXY(X_NOW,100) < 3)
					{
#if BURNIN_LOG_EN
						log_printf("[motor_go_edgeways]to begin point2\r\n",X_NOW,Y_NOW);
#endif
						motor_run(GO_STOP,0,0,0);
						//delay_ms_sensers(200);
						burnin_coordinate_calcu(0);														//�����ԭʼ������ϵ
						coord_org2map(navigat->x_org,navigat->y_org,&navigat->tx,&navigat->ty);	//����ϵת��
						return RET_NEAR_OK;
					}
				}

			}
		}

	}


//	return 0;

l_go_out_for_dock_burnin:
	if(near->n_sta == RIGHT_SIDE_NEAR)
	{
		burnin_robot_turn_deg(GO_LEFT,DEG_TURN_PWM,60);
	}else if(near->n_sta == LEFT_SIDE_NEAR)
	{
		burnin_robot_turn_deg(GO_RIGTH,DEG_TURN_PWM,60);
	}
#if BURNIN_LOG_EN
	log_printf("[motor_go_fw]...\r\n");
#endif
	CHECK_IDLE_STA_RT(0);
	sys->sState = SYS_DOCK;
	motor_go_fw(2000,NO_SIDE,0);
	return 0;
	//delay_ms(1000);
}

//�ϻ����������ʼ������
void proc_burn_in_init(void)
{
	//log_printf("dir = %d,pwm=%d,speed=%d,hw=%d\r\n",dir,ctrl->pwm,ctrl->speed,ctrl->hw);
	sys->shut_down_motor = 0;
	navigat_init(0);

	MOTOR_POWER_ON();
	MOTOR_CTRL(NORM_SIDE_PWM_L,NORM_SIDE_PWM_R,NORM_MID_PWM,sys->dust_pwm_value)
	sys->sState = SYS_RANDOM;
	burnin_work_near_timer = 0;
	burnin_work_timer = 0;
	bummech_ct = bumir_ct = 0;

	bummech_left_ct = bummech_right_ct = 0;
	bumir_left_ct = bumir_right_ct = 0;
}
//�ϻ���������
void proc_burn_in_task(void)
{
	uint8_t near_dir = LEFT_SIDE_NEAR;


	tx_com_burnin(near_dir, burnin_work_timer, bummech_left_ct, bumir_left_ct, bummech_right_ct, bumir_right_ct);
	motor_go_forwark(0,NO_SIDE_NEAR,NULL);		//ֱ��
	motor_go_burnin(LEFT_SIDE_NEAR,0,0,GO_NEAR_TYPE_NO,0);
	bummech_left_ct = bummech_ct;
	bummech_ct = 0;
	bumir_left_ct = bumir_ct;
	bumir_ct = 0;

	tx_com_burnin(near_dir, burnin_work_timer, bummech_left_ct, bumir_left_ct, bummech_right_ct, bumir_right_ct);
	near_dir = RIGHT_SIDE_NEAR;
	motor_go_forwark(0,NO_SIDE_NEAR,NULL);		//ֱ��
	motor_go_burnin(RIGHT_SIDE_NEAR,0,0,GO_NEAR_TYPE_NO,0);
	bummech_right_ct = bummech_ct;
	bummech_ct = 0;
	bumir_right_ct = bumir_ct;
	bumir_ct = 0;

	tx_com_burnin(NO_SIDE_NEAR, burnin_work_timer, bummech_left_ct, bumir_left_ct, bummech_right_ct, bumir_right_ct);
	STOP_ALL_MOTOR();
	sys->sState = SYS_IDLE;
	ny3p_play(VOICE_M_FINISHED);
}

//=========================================================================================end


void sn_print(uint8_t * pdat,uint16_t len)
{
	uint16_t i;

	len --;
	log_printf("\r\n[sn %d]:",len);
	for(i = 0;i < len;i ++)
	{
		log_printf("%c",pdat[i]);
	}
	log_printf("\r\n");
	for(i = 0;i < len;i ++)
	{
		log_printf("%02X ",pdat[i]);
	}
	log_printf("\r\n%02X\r\n",pdat[i]);
}

void data_print(uint8_t * pdat,uint16_t len)
{
	uint16_t i;

	log_printf("\r\n[data_print %d]:",len);
	for(i = 0;i < len;i ++)
	{
		log_printf("%02x ",pdat[i]);
	}
	log_printf("\r\n\r\n");
}

static uint8_t mact_remote_tst_reg = 0;//bit0 �Կ� bit1 remote controller, 0xff ����
void set_remote_flag(uint8_t set)
{
	if(set == 0xff)
		mact_remote_tst_reg = 0;
	else
		mact_remote_tst_reg |= (set < 4)?set:0;
}
uint8_t get_remote_flag(void)
{
	return mact_remote_tst_reg;
}
static uint8_t norm_sensor_false_reg = 0;//�����ʹ�����/����������δ�����ļ��ɹ���ǣ��������⵽��ⰴ��δ����ʱ��IO����Ϊ1,��δ����,��ô��Ӧ��λ��1,λ˵���ο���NORM_SENSOR_XXX
static uint8_t norm_sensor_true_reg = 0;//�����ʹ�����/����������δ�����ļ��ɹ���ǣ��������⵽��ⰴ������ʱ��IO����Ϊ0,���ѱ�����,��ô��Ӧ��λ��1,λ˵���ο���NORM_SENSOR_XXX
//�ڲ���ģʽ�»�ȡ������������������,�������ݾ���λ��ʾ,
// λ������bit0-bit7�ֱ�Ϊ:Ĩ��/����ǽ/����/��������/������/������/����ײ/����ײ
uint16_t get_mactest_sensors(void)
{
	uint8_t i;
	uint16_t n_res = 0;

	if((norm_sensor_false_reg & NORM_SENSOR_BIN) == 0 && (READ_DUSTBOX_DET() != 0))//û�г���
	{
		for(i=0;i<20;i++)//Ĩ�����
		{
			if(READ_DUSTBOX_DET() == 0)
			break;
		}
		if(i == 20)
			norm_sensor_false_reg |= (1 << NORM_SENSOR_BIN);
	}
	if(((norm_sensor_true_reg & NORM_SENSOR_BIN) == 0) && (READ_DUSTBOX_DET() == 0))//�г���
	{
		for(i = 0;i < 20;i ++)
		{
			if(READ_DUSTBOX_DET() != 0)
				break;
		}
		if(i==20)
			norm_sensor_true_reg |= (1 << NORM_SENSOR_BIN);
	}

	if(((norm_sensor_false_reg & (1<<NORM_SENSOR_KEY_WIFI)) == 0) && !KEY_RD_WIFI())
	{
		for(i=0;i<20;i++)//����ǽ
		{
			if(KEY_RD_WIFI())
				break;
		}
		if(i==20)
			norm_sensor_false_reg |= (1 << NORM_SENSOR_KEY_WIFI);
	}
	if(((norm_sensor_true_reg & (1<<NORM_SENSOR_KEY_WIFI)) == 0) && KEY_RD_WIFI())
	{
		for(i=0;i<20;i++)//���м��
		{
			if(!KEY_RD_WIFI())
				break;
		}
		if(i==20)
			norm_sensor_true_reg |= (1 << NORM_SENSOR_KEY_WIFI);
	}

	if(((norm_sensor_false_reg & (1<<NORM_SENSOR_KEY_SPOT)) == 0) && !KEY_RD_SPOT())
	{
		//log_printf("NORM_SENSOR_KEY_SPOT\r\n");
		for(i = 0;i < 20;i ++)
		{
			if(KEY_RD_SPOT())
				break;
		}
		if(i == 20)
			norm_sensor_false_reg |= (1 << NORM_SENSOR_KEY_SPOT);
	}
	if(((norm_sensor_true_reg & (1 << NORM_SENSOR_KEY_SPOT)) == 0) && KEY_RD_SPOT())
	{
		for(i=0;i<20;i++)//������
		{
			if(!KEY_RD_SPOT())
				break;
		}
		if(i==20)
			norm_sensor_true_reg |= (1 << NORM_SENSOR_KEY_SPOT);
	}
	if(((norm_sensor_false_reg & (1 << NORM_SENSOR_KEY_START)) == 0) && !KEY_RD_CLEAN())
	{
		for(i = 0;i < 20;i ++)
		{
			if(KEY_RD_CLEAN())
				break;
		}
		if(i == 20)
			norm_sensor_false_reg |= (1 << NORM_SENSOR_KEY_START);
	}
	if(((norm_sensor_true_reg & (1 << NORM_SENSOR_KEY_START)) == 0) && KEY_RD_CLEAN())
	{
	for(i=0;i<20;i++)//������
	{
			if(!KEY_RD_CLEAN())
				break;
		}
		if(i == 20)
			norm_sensor_true_reg |= (1 << NORM_SENSOR_KEY_START);
	}

	if(((norm_sensor_false_reg & (1 << NORM_SENSOR_KEY_DOCK)) == 0) && !KEY_RD_DOCK())
	{
		for(i = 0;i < 20;i ++)
		{
			if(KEY_RD_DOCK())
				break;
		}
		if(i == 20)
			norm_sensor_false_reg |= (1 << NORM_SENSOR_KEY_DOCK);
	}
	if(((norm_sensor_true_reg & (1 << NORM_SENSOR_KEY_DOCK)) == 0) && KEY_RD_DOCK())
	{
		for(i = 0;i < 20;i ++)
		{
			if(!KEY_RD_DOCK())
				break;
		}
		if(i == 20)
			norm_sensor_true_reg |= (1 << NORM_SENSOR_KEY_DOCK);
	}

	uint8_t com_flag = get_remote_flag();
	////if((norm_sensor_true_reg & NORM_SENSOR_REMOTE) == 0 && (get_remote_flag()&0x02))//ң���������·�,���ﲻ��true��falseֻҪ�յ�ң�����źžͲ���ͨ��
	if(com_flag &0x02)
	{
		norm_sensor_true_reg |= (1 << NORM_SENSOR_REMOTE);
		norm_sensor_false_reg |= (1 << NORM_SENSOR_REMOTE);
	}

	if(com_flag &0x01)
	{
		norm_sensor_true_reg |= (1 << NORM_SENSOR_DISPLAY);
		norm_sensor_false_reg |= (1 << NORM_SENSOR_DISPLAY);
	}
	n_res = ((uint16_t)(norm_sensor_true_reg & norm_sensor_false_reg) << 8) + (norm_sensor_true_reg & norm_sensor_false_reg);
	//log_printf("n_res:%04X-%02X,%02X\r\n",n_res,norm_sensor_true_reg,norm_sensor_false_reg);
	return n_res;
}





////////��������	tm_mode = 1
tm_irmaxmin irmaxmin[7] = {{0,1000},
							{0,3800},
							{0,1200},
							{0,1200},
							{0,1200},
							{0,3800},
							{0,1000}};



tm_irmaxmin irtestdata[7] = {{1000,4200},
							{0,4200},
							{700,4200},
							{500,4200},
							{700,4200},
							{0,4200},
							{1000,4200}};
////////���Ʒ���� tm_mode = 2
tm_irmaxmin asse_irmaxmin[7] = {{0,1500},
							{0,3800},
							{0,1200},
							{0,1200},
							{0,1200},
							{0,3800},
							{0,1500}};
//tm_irmaxmin asse_irtestdata[7] = {{1000,4200},
tm_irmaxmin asse_irtestdata[7] = {{800,4200},
							{0,3800},
							{700,4200},
							{500,4200},
							{700,4200},
							{0,3800},
							//{1000,4200}};
							{800,4200}};

uint8_t const pn_number[] = {0x10,0x01,0x18,0x45,0x12,0x34,0x56,0x67,0x78,0x90};
static uint8_t bum_order = 0;
static uint8_t bum_over = 0;
static uint8_t bum_err = 0;
void check_bum(void)
{
	#define MAX_C_BUM		20
	uint8_t i =0;

	if(0 == bum_order)
	{
		if(((GPIOE->IDR) & GPIO_Pin_12) )	//L2	
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOE->IDR & GPIO_Pin_12)==0)
					break;
			}
			if(MAX_C_BUM > i)
				bum_over |= 0x01;
		}

		if(( (GPIOD->IDR) & GPIO_Pin_0))//L1
		{						
			for(i=0;i<MAX_C_BUM;i++)
			{
				if(((GPIOD->IDR) & GPIO_Pin_0)==0)
					break;
			}
			if(MAX_C_BUM > i)
				bum_over |= 0x02;
		}

		if( ((GPIOE->IDR) & GPIO_Pin_5)  )//R1
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if((GPIOE->IDR) & GPIO_Pin_5==0)
					break;
			}
			if(MAX_C_BUM > i)
				bum_over |= 0x04;
		}

		if(((GPIOE->IDR) & GPIO_Pin_6)) //R2
		{
			for(i=0;i<MAX_C_BUM;i++)
			{
				if(((GPIOE->IDR) & GPIO_Pin_6) ==0)

					break;
			}
			if(MAX_C_BUM > i)
				bum_over |= 0x08;
		}

		bum_err = bum_over;
		bum_order= 1;
	}
	else if(1 == bum_order)
	{
			if(0 == (bum_over & 0x01))	//L2	
			{
				if(((GPIOE->IDR) & GPIO_Pin_12) ==0)
				{
					for(i=0;i<MAX_C_BUM;i++)
					{
						if((GPIOE->IDR) & GPIO_Pin_12)
							break;
					}
					if(i>=MAX_C_BUM)
					{
						bum_err &= 0xFE;	
						bum_over |= 0x01;
					}
					else
						bum_err |= 0x01;
				}
				else
					bum_err |= 0x01;
			}
			


			if(0 == (bum_over & 0x02))		//L1
			{
				if(((GPIOD->IDR) & GPIO_Pin_0) ==0)
				{
					for(i=0;i<MAX_C_BUM;i++)
					{
						if((GPIOD->IDR) & GPIO_Pin_0)
							break;
					}
					if(i>=MAX_C_BUM)
					{
						bum_err &= 0xFD;	
						bum_over |= 0x02;
					}
					else
						bum_err |= 0x02;
				}
				else
					bum_err |= 0x02;
			}	


			if(0 == (bum_over & 0x04))	//R1
			{
				if(((GPIOE->IDR) & GPIO_Pin_5) ==0)
				{
					for(i=0;i<MAX_C_BUM;i++)
					{
						if((GPIOE->IDR) & GPIO_Pin_5)
							break;
					}
					if(i>=MAX_C_BUM)
					{
						bum_err &= 0xFB;
						bum_over |= 0x04;
					}
					else
						bum_err |= 0x04;
				}
				else
					bum_err |= 0x04;
			}	


			if(0 == (bum_over & 0x08))	//R2
			{
				if(((GPIOE->IDR) & GPIO_Pin_6) ==0)
				{
					for(i=0;i<MAX_C_BUM;i++)
					{
						if((GPIOE->IDR) & GPIO_Pin_6)
							break;
					}
					if(i>=MAX_C_BUM)
					{
						bum_err &= 0xF7;	
						bum_over |= 0x08;
					}
					else
						bum_err |= 0x08;
				}
				else
					bum_err |= 0x08;
			}
	}
}

void bum_com(void)
{
			uint8_t g_err = 0;
			if(bum_err&0x01)
				g_err|=BIT_STA(ERR_LEFT_BUM);
			if(bum_err&0x02)
			{
				g_err|=BIT_STA(ERR_MLEFT_BUM);		
				g_err|=BIT_STA(ERR_MID_BUM);		
			}	
			if(bum_err&0x04)
			{
				g_err|=BIT_STA(ERR_MRIGHT_BUM);
				g_err|=BIT_STA(ERR_MID_BUM);	
			}	
			if(bum_err&0x08)
				g_err|=BIT_STA(ERR_RIGHT_BUM);	

			tx_com_dat(TPC_BUM_SW, 20 , g_err?(TPC_ERR_DIGI_IN):(TPC_ERR_NOERROR), (uint8_t)g_err);//�ϱ����ȼ�������Ϣ	

}




//�����Ĳ��Գ���,ȫ�ֱ�������proc_bdtest_task(),��Ϊ������������ͬʱ��ʹ��
//����������Ҫʵʱ�ϱ����ֿ�������״̬,��������ʾ,�����������ڲ���ģʽ��ר�ż��
void proc_mactest_task(void)
{
	static uint8_t bd_timer = 0xff;
	static uint8_t bd_heart_timer = 0;
	static uint8_t bd_heart_ct = 0;
	static uint8_t bd_bum_timer = 0;
//	static uint8_t bdtest_sta_old = TPC_STOP_ALL;
//	static uint16_t bd_batcharge_timer = 0;
//	static uint16_t g_sta_bum=0,g_sta_idle=0,g_err=0;
	static uint8_t sw_en=0,sw_un=0;
//	static uint16_t test_ground[3][2] = {0};
	static uint8_t cc=0,bot=0;
	int32_t k1,k2;
	uint8_t charge_result = 1;

	get_irda(&sys->gSta);
	uint8_t tmp_cc=0;

	if(TIM5->CNT < 50000)//50ms��ʱ
		return;
	TIM5->CNT = 0;
	get_mactest_sensors();
	if(bd_heart_timer ++ >= 15)//����������,ÿ��һ��
	{
		uint16_t n_sensors;
		bd_heart_timer = 0;
		n_sensors = get_mactest_sensors();
		#if BDTEST_LOGPRINT == 0
		tx_com_beep(bd_heart_ct ++, (uint8_t)(n_sensors >> 8), (uint8_t)(n_sensors & 0x00ff),sys->g_sta,&sys->g_buton[0][0]);
		#endif
		//log_printf("beep:%04X\r\n",n_sensors);
	}

	switch(bdtest_sta)
	{
		case TPC_STOP_ALL:
			ny3p_play(VOICE_FAC_MODE);
			delay_ms(500);
			ny3p_play(VOICE_FAC_MODE);
			delay_ms(500);
			
			MOTOR_POWER_OFF_NPRI();//�رյ����Դ
			TargetSysReset();
			break;
		case TPC_START_ALL://����Ҫ����������Ƿ�����/����԰��ͨ���Ƿ�����
			//MOTOR_CTRL(1000, 1000, 300, 0);
			MOTOR_POWER_OFF_NPRI();
			bdtest_sta = TPC_MT_FAN;//TPC_MT_FAN;//TPC_BUM_SW;//TPC_MT_FAN;//TPC_IR_BOT;//TPC_MT_FAN;//TPC_SMT_L;;//�������ʱ���ԭ����Ҫ�ȿ����
			bdtest_sta_child = 0;
			navigat->distance = 0;
			norm_sensor_false_reg = norm_sensor_true_reg = 0;
			bum_order = 0;
			bum_over = 0;
			bum_err = 0;
			////set_remote_flag(0xff);
			#if BDTEST_LOGPRINT
			log_printf(">>>>>>>>>>>>\r\n");
			#endif
			//tx_com_dat(test_mac_proc_dat);//�ϱ�����
	
			mac_dat_pro(TPC_STOP_ALL);
			break;
		case TPC_MT_FAN:
			switch(bdtest_sta_child)
			{
				case 0://��ʼ���Է��
					{
						bdtest_sta_child = 1;
						//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);						
						bdtest_fantest = 1;
						tmod_fancd = 0;						
						uint32_t dust_close_adc=0,dust_close_v = 0,dust_open_adc=0,dust_open_v=0;
						TEST_MOTOR_POWER_ON();
						DUST_MOTOR_OFF();	//ֹͣ����		
						//delay_ms(2000);	
						delay_ms(500);	
						for(uint8_t i=0;i<10;i++)
						{
							delay_ms(100);
							dust_close_adc += V_DUST_ADC();
							dust_close_v += LiBat_GetBatVolt();
						}	//δ���е���
						TEST_MOTOR_POWER_ON();
						//��ʼ�����ת���жϵ�IO��
						DUST_MOTOR_RANK1();				
						delay_ms(500);		//���е���
						for(uint8_t i=0;i<10;i++)	
						{
							delay_ms(300);
							dust_open_adc += V_DUST_ADC();
							dust_open_v += LiBat_GetBatVolt();
						}	
										
						TEST_MOTOR_POWER_OFF_NPRI();
						DUST_MOTOR_OFF();	//ֹͣ����		
						#if 0
						if(disXY(dust_open_adc,dust_close_adc)>1000)
						{
							tmod_fancd=5000;
						}
						else
							tmod_fancd=0;
						#else
						////if((dust_close_v>dust_open_v) && ((dust_close_v - dust_open_v)>2000) )
						if(dust_close_v>dust_open_v)
							tmod_fancd=5000;
						else
							tmod_fancd=0;

						#endif
						bd_timer = 0;
						bdtest_sta_child = 1;
					}
					break;					
				case 1://��ȡ������	
					bdtest_fantest = 0;				
					//������  �е����͹�
					tx_com_dat(TPC_MT_FAN, 7 , tmod_fancd < 100 ? TPC_ERR_FORWARD:TPC_ERR_NOERROR, 1);//�ϱ����ȼ�������Ϣ

					bd_timer = 0;
					bdtest_sta_child = 3;
				#if BDTEST_LOGPRINT
					bdtest_sta_child = 2;
					break;
				case 2:				
					TEST_MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_WHL_L;
					bdtest_sta_child = 0;
					bdtest_fantest = 0;
					break;
			}
			break;
		case TPC_WHL_L://���ּ��
			switch(bdtest_sta_child)
			{
				case 0://��ʼ��������
					bdtest_sta_child = 1;
					TEST_MOTOR_POWER_ON();
					motor.c_left_hw = 0;
					motor_wheel_stop(RIGHT_WHEEL);
					motor_wheel_forward(LEFT_WHEEL,600);

					//////tx_com_dat(TPC_WHL_L, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					bd_timer = 0;
					break;
				case 1://��ǰת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(LEFT_WHEEL);
					#if BDTEST_LOGPRINT
					log_printf("forward:%d\r\n",motor.c_left_hw);
					#endif
					if(motor.c_left_hw < 200)//����������
					{
						tx_com_dat(TPC_WHL_L, 14, TPC_ERR_FORWARD, 0);
						#if BDTEST_LOGPRINT
						log_printf("TPC_WHL_L:FAIL\r\n");
						#else
						bdtest_sta = TPC_WHL_R;//������һ������
						bdtest_sta_child = 0;
						break;
						#endif
					}
					//////tx_com_dat(TPC_WHL_L, 40, TPC_ERR_NOERROR, 0);
					bdtest_sta_child = 2;
					//MOTOR_POWER_ON();
					motor.c_left_hw = 0;
					motor_wheel_backward(LEFT_WHEEL,600);
					bd_timer = 0;
					break;
				case 2://���ת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(LEFT_WHEEL);

					#if BDTEST_LOGPRINT
					log_printf("back:%d\r\n",motor.c_left_hw);
					#endif
					if(0)////(motor.c_left_hw < 200)
					{
						//MOTOR_POWER_OFF_NPRI();
						//motor_wheel_forward(LEFT_WHEEL,GO_FORWARD,600);
						tx_com_dat(TPC_WHL_L, 14, TPC_ERR_BACK, 0);//�ϱ��������ת

						#if BDTEST_LOGPRINT
						log_printf("TPC_WHL_L:FAIL\r\n");
						#else
						bdtest_sta = TPC_WHL_R;//������һ������
						bdtest_sta_child = 0;
						break;
						#endif
					}
					tx_com_dat(TPC_WHL_L, 14, TPC_ERR_NOERROR, 0);//�ϱ�����

					#if BDTEST_LOGPRINT
					delay_ms(50);
					bdtest_sta_child = 3;
					motor_wheel_stop(LEFT_WHEEL);
					log_printf("TPC_WHL_L STOP!\r\n");
					#else
					bdtest_sta = TPC_WHL_R;//������һ������
					bdtest_sta_child = 0;
					#endif
					//MOTOR_POWER_ON();
					motor.c_left_hw = 0;

					bd_timer = 0;
					tmd_rxbd_dat.unread_flag = 0;//�����ݽ��յĻ���δ����־��0
					break;
				#if BDTEST_LOGPRINT
				case 3:
					log_printf("TPC_WHL_L STOP!\r\n");
					motor_wheel_stop(LEFT_WHEEL);
					//MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_WHL_R;
					bdtest_sta_child = 0;
					motor_wheel_stop(LEFT_WHEEL);
					//MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_WHL_R://���ּ��
			switch(bdtest_sta_child)
			{
				case 0://��ʼ��������
					bdtest_sta_child = 1;
					TEST_MOTOR_POWER_ON();
					motor.c_right_hw = 0;
					motor_wheel_stop(LEFT_WHEEL);
					motor_wheel_forward(RIGHT_WHEEL,600);

					//////tx_com_dat(TPC_WHL_R, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					bd_timer = 0;
					break;
				case 1://��ǰת
					if(bd_timer ++ < 20)break;
					motor_wheel_stop(RIGHT_WHEEL);
					#if BDTEST_LOGPRINT
					log_printf("forward:%d\r\n",motor.c_right_hw);
					#endif
					if(motor.c_right_hw < 200)//����������
					{
						tx_com_dat(TPC_WHL_R, 21, TPC_ERR_FORWARD, 0);
						#if BDTEST_LOGPRINT
						log_printf("TPC_WHL_R:FAIL\r\n");
						#else
						bdtest_sta = TPC_MT_MID;//������һ������
						bdtest_sta_child = 0;
						break;
						#endif
					}
					//////tx_com_dat(TPC_WHL_R, 40, TPC_ERR_NOERROR, 0);
					bdtest_sta_child = 2;
					//MOTOR_POWER_ON();
					motor.c_right_hw = 0;
					motor_wheel_backward(RIGHT_WHEEL,600);
					bd_timer = 0;
					break;
				case 2://���ת
					if(bd_timer ++ < 20)break;
					//motor_wheel_stop(RIGHT_WHEEL);
					motor_wheel_stop(DOUBLE_WHEEL);

					#if BDTEST_LOGPRINT
					log_printf("back:%d\r\n",motor.c_right_hw);
					#endif
					if(0)////(motor.c_right_hw < 200)
					{
						//MOTOR_POWER_OFF_NPRI();
						//motor_wheel_forward(LEFT_WHEEL,GO_FORWARD,600);
						tx_com_dat(TPC_WHL_R, 21, TPC_ERR_BACK, 0);//�ϱ��������ת
						motor_wheel_stop(DOUBLE_WHEEL);

						#if BDTEST_LOGPRINT
						log_printf("TPC_WHL_R:FAIL\r\n");
						#else
						bdtest_sta = TPC_MT_MID;//������һ������
						bdtest_sta_child = 0;
						break;
						#endif
					}
					tx_com_dat(TPC_WHL_R, 21, TPC_ERR_NOERROR, 0);//�ϱ�����

					#if BDTEST_LOGPRINT
					delay_ms(50);
					bdtest_sta_child = 3;
					motor_wheel_stop(DOUBLE_WHEEL);
					log_printf("TPC_WHL_L STOP!\r\n");
					#else
					bdtest_sta_child = 0;
					bdtest_sta = TPC_MT_MID;//������һ������
					#endif
					//MOTOR_POWER_ON();
					motor.c_right_hw = 0;

					bd_timer = 0;
					tmd_rxbd_dat.unread_flag = 0;//�����ݽ��յĻ���δ����־��0
					motor_wheel_stop(DOUBLE_WHEEL);
					break;
				#if BDTEST_LOGPRINT
				case 3:
				break;
				#endif
				default:
					bdtest_sta = TPC_MT_MID;//TPC_SMT_L;
					bdtest_sta_child = 0;
					motor_wheel_stop(DOUBLE_WHEEL);
					//MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_SMT_L://�������ˢ���
			switch(bdtest_sta_child&0x0f)
			{
				case 0://��ʼ��������
					bdtest_sta_child = 1;
					TEST_MOTOR_POWER_ON();
					SET_RSIDE_MOTER(1000);
					SET_LSIDE_MOTER(BDTEST_SMT_PWMVALUE);
					//////tx_com_dat(TPC_SMT_L, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					//////tx_dp_dat(TBD_SMT_L, 1, 0);//�·����ˢ��ʼ��������,��ԭ��������0
					//////tx_dp_dat(TBD_MTOC_SMT_L, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					bd_timer = 0;
					break;
				case 1://��ȡ������
					if(bd_timer ++ < 20)break;
					//////tx_com_dat(TPC_SMT_L, 50 , TPC_ERR_NOERROR, 0);//�ϱ�����
					//////tx_dp_dat(TBD_SMT_L, 0, 0);//�·����ˢֹͣ��������,��ԭ����������0
					//////tx_dp_dat(TBD_SMT_L, 3, 0);//�·����ˢֹͣ��������,��ԭ����������0
					bd_timer = 0;
					bdtest_sta_child = 2;
					break;
				case 2:
					if(bd_timer ++ < 20 && !tmd_rxbd_dat.unread_flag)break;//��ʱδ�յ�����
					bd_timer = 0;
					{
						tmd_rxbd_dat.unread_flag = 0;
					}
					//log_printf("get mtoc_smt_l:%d\r\n",tmd_rxbd_dat.mtoc_smt_l);
					{
						uint8_t i;//, j = 0;
						uint16_t n_mt_curr;
						
						SET_LSIDE_MOTER(BDTEST_SMT_PWMVALUE);
						delay_ms(10);
						for(i = 0;i < 8;i ++)
						{
							delay_ms(5);
							n_mt_curr = I_SMTL_ADC();
						#if BDTEST_LOGPRINT
							log_printf("curr:%d-%d\r\n",n_mt_curr,i);
						#endif
							////if(n_mt_curr > 600)
							if(n_mt_curr> I_SMTL_CUR_MAX)
							{
								//j ++;
								break;
							}
						}

						tx_com_dat(TPC_SMT_L, 28, i >= 8 /*i < 5*/ ? TPC_ERR_NOERROR:TPC_ERR_OC, 0);//�ϱ����Ȼ��ϱ�����������
						if(i < 5)
						{
						#if 0
							//bdtest_sta = TPC_SMT_R;//������һ������
							bdtest_sta_child = 4;
							log_printf("*****\r\n");
						#else
							bdtest_sta = TPC_SMT_R;//������һ������
							bdtest_sta_child = 0;
						#endif
							//break;
						}
					}
				
					//////tx_dp_dat(TBD_MTOC_SMT_L, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					//bdtest_sta = TPC_WHL_R;
					//bdtest_sta_child = 0;
					//MOTOR_POWER_OFF_NPRI();
				#if 0
					break;
				case 4:
					log_printf("^");
					SET_LSIDE_MOTER(1000);
					MOTOR_POWER_OFF_NPRI();
					break;
				#endif
				default:
					bdtest_sta = TPC_SMT_R;
					bdtest_sta_child = 0;
					SET_LSIDE_MOTER(1000);
					TEST_MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_SMT_R://���Դ��ұ�ˢ���
			switch(bdtest_sta_child&0x0f)
			{
				case 0:
					bdtest_sta_child = 1;
					TEST_MOTOR_POWER_ON();
					SET_LSIDE_MOTER(1000);
					SET_RSIDE_MOTER(BDTEST_SMT_PWMVALUE);
					//////tx_com_dat(TPC_SMT_R, 25 , TPC_ERR_NOERROR, 0);//�ϱ�����
					//////tx_dp_dat(TBD_MTOC_SMT_R, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					//////tx_dp_dat(TBD_SMT_R, 1, 0);//�·����ˢ��ʼ��������,��ԭ��������0
					bd_timer = 0;
					break;
				case 1://��ȡ������
					if(bd_timer ++ < 20)break;
					//////tx_com_dat(TPC_SMT_R, 50 , TPC_ERR_NOERROR, 0);//�ϱ�����
					//////tx_dp_dat(TBD_SMT_R, 3, 0);//�·����ˢֹͣ��������,��ԭ����������0
					bd_timer = 0;
					bdtest_sta_child = 2;
					break;
				case 2:
				{
					if(bd_timer ++ < 20 && !tmd_rxbd_dat.unread_flag)break;//��ʱδ�յ�����
					bd_timer = 0;
					{
						tmd_rxbd_dat.unread_flag = 0;
					}

					
						uint8_t i;//, j = 0;
						uint16_t n_mt_curr;
						
						SET_RSIDE_MOTER(BDTEST_SMT_PWMVALUE);
						delay_ms(10);
						for(i = 0;i < 8;i ++)
						{
							delay_ms(5);
							n_mt_curr = I_SMTR_ADC();
						#if BDTEST_LOGPRINT
							log_printf("curr:%d-%d\r\n",n_mt_curr,i);
						#endif
							////if(n_mt_curr > 600)
							if(n_mt_curr > I_SMTR_CUR_MAX)
							{
								//j ++;
								break;
							}
							//log_printf("i:%d\r\n",i);
						}

						tx_com_dat(TPC_SMT_R, 35, i >= 8/*i < 5*/ ? TPC_ERR_NOERROR:TPC_ERR_OC, 0);//�ϱ����Ȼ��ϱ�����������
						if(i < 5)
						{
						#if 0
							//bdtest_sta = TPC_SMT_R;//������һ������
							bdtest_sta_child = 4;
							log_printf("**6**%d\r\n",i);
						#else
							bdtest_sta = TPC_MT_MID;//������һ������
							bdtest_sta_child = 0;
						#endif
							//break;
						}
					
					
					//////tx_dp_dat(TBD_MTOC_SMT_R, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					//bdtest_sta = TPC_WHL_R;
					//bdtest_sta_child = 0;
					//MOTOR_POWER_OFF_NPRI();
				}
				break;
				default:
					bdtest_sta = TPC_MT_MID;
					bdtest_sta_child = 0;
					SET_RSIDE_MOTER(1000);
					TEST_MOTOR_POWER_OFF_NPRI();
					break;
			}
			break;
		case TPC_MT_MID://������ɨ���		//2A��ɨ���������   2B��ɨ����������
			switch(bdtest_sta_child&0x0f)
			{
				case 0:
				{
					uint32_t mid_close_adc=0,mid_open_adc=0;
					bdtest_sta_child = 1;
					navigat->gun_cnt=0;		
					mac_dat_pro(TPC_STOP_ALL);
					TEST_MOTOR_POWER_ON();
					SET_MID_MOTER(300);////800
							
					delay_ms(1000);		//���е���
					for(uint8_t i=0;i<10;i++)	
					{
						delay_ms(100);
						mid_open_adc += I_MID_ADC();
					}										
					TEST_MOTOR_POWER_OFF_NPRI();
					SET_MID_MOTER(1000);
					////DUST_MOTOR_OFF();	//ֹͣ����		
					////SET_MID_MOTER(0);////800
					delay_ms(500);		
					for(uint8_t i=0;i<10;i++)
					{
						delay_ms(100);
						mid_close_adc += I_MID_ADC();
					}	//δ���е���						
					if(disXY(mid_open_adc,mid_close_adc)<1000 &&disXY(mid_open_adc,mid_close_adc)>200)
					{
						////tx_com_dat(TPC_MT_MID, 42, TPC_ERR_NOERROR, 0);//OK
						bdtest_sta_child = 1;
					}
					else if(disXY(mid_open_adc,mid_close_adc)>=1000)
					{
						tx_com_dat(TPC_MT_MID, 42, TPC_ERR_FORWARD, 0);//����
						tmod_fancd=0;
						bdtest_sta = TPC_IR_FRT;//������һ������
						bdtest_sta_child = 0;
					}
					else
					{
						tx_com_dat(TPC_MT_MID, 43, TPC_ERR_FORWARD, 0);//��ת
						tmod_fancd=0;
						bdtest_sta = TPC_IR_FRT;//������һ������
						bdtest_sta_child = 0;
					}									
				}
					break;
				case 1:				
				default:
				{
					uint32_t mid_open_adc_d =0;
					////bdtest_sta_child = 0;
					navigat->gun_cnt=0;		
					mac_dat_pro(TPC_MT_MID);
					delay_ms(1000);
					delay_ms(1000);		//���е���
					for(uint8_t i=0;i<10;i++)	
					{
						delay_ms(100);
						mid_open_adc_d += I_MID_ADC();
					}										
					TEST_MOTOR_POWER_OFF_NPRI();
					SET_MID_MOTER(1000);
					mac_dat_pro(TPC_STOP_ALL);
					delay_ms(500);		
					tx_com_dat(TPC_MT_MID, 42, TPC_ERR_NOERROR, 0);//OK
					tmod_fancd=0;					
					//////tx_dp_dat(TBD_MTOC_MT_MID, 0, 0);//�·�����ֹ��ز���,����·�ĵ���Ͽ�
					bdtest_sta = TPC_IR_FRT;//������һ������
					bdtest_sta_child = 0;		

					GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);
					delay_ms(500);
					//mac_dat_pro(TPC_IR_FRT);	
					//	delay(500);
				}	
				break;
			}
			break;			
		case TPC_IR_FRT://ǰײ���(��ײ)������
			switch(bdtest_sta_child)
			{
				case 0:	
					{
						uint8_t ccc;			
						//#if BDTEST_LOGPRINT
						///uint8_t bot;
						//#endif
						//int32_t k1,k2;
						uint8_t i,j;
						sys->c_ir_adc = 0;
						for(ccc = 0;ccc < 9;ccc ++)
						{
							for(i=0;i<10;i++)
							{
								//sys->m_sta[i][ccc] = adc_converted_value[i];
								if(ccc == 0)
									sys->m_sta[i][ccc] = 0;
								else
									sys->m_sta[i][ccc] = adc_converted_value[i];
								//log_printf(",%d",sys->m_sta[i][sys->c_ir_adc] );
							}
							if(ccc<=3)
							{
								GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
								GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
								
							}
							else
							{
								GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);	
								GPIO_SetBits(GPIOD,GPIO_Pin_14);	
							}
							delay(1);
							
						}
						#if 0
						for(i= 0;i < 9;i ++)
						{
							log_printf("%d\r\n",sys->m_sta[0][i]);
						}
						#endif
						cc /*= bot */= 0;//cc������¼ǰײ����ļ����,j������¼�Եغ���ļ����
						//log_printf("\r\ncc>>%d\r\n",cc);
						for(i=0;i<10;i++)//�ѶԵغ���Ҳ�����,Ŀǰ���Եغ����������ȡ���ݣ���⻷��Ϊһ�����ߵ��ϰ���
						{
							k1 = k2 = 0;
							for(j=0;j<4;j++)
								k1 +=sys->m_sta[i][j];
							for(j=5;j<9;j++)
								k2 +=sys->m_sta[i][j];

							if(i < 7)//ǰײ����
							{
								/*
								if(i == 0 || i == 3 || i == 6)
								{
									sys->g_sta[i] = k1 >> 2;
								}
								else
								*/
								{
									if(k1 > k2 && ((k1 - k2) /4) < 5000)
										sys->g_sta[i] = (k1 - k2) >> 2;
									else
										sys->g_sta[i] = 0;
								}
								//if(sys->g_sta[i] < 500 || sys->g_sta[i] > 3500)//ǰײ����,�ں춨�����ȡֵ������Χ��Ϊ���ϸ�
								//if(sys->g_sta[i] < 1000 || sys->g_sta[i] > 3800)//ǰײ����,�ں춨�����ȡֵ������Χ��Ϊ���ϸ�
								//��һ·û��ʹ��,����һ·�������Ϊ�м�ֵ
								if(tm_mode == 1)		//��������
								{
									if((sys->g_sta[i] < irmaxmin[i].min) || (sys->g_sta[i] > irmaxmin[i].max))
									{
										cc |= BIT8_STA(i);
									}
								}
								else if(tm_mode == 2)	//���Ʒ
								{
									//if(i == 0 || i == 4)
									{
										if((sys->g_sta[i] < asse_irmaxmin[i].min) || (sys->g_sta[i] > asse_irmaxmin[i].max))
										{
											cc |= BIT8_STA(i);
										}
									}
								}
										//log_printf("\r\n%02X-%d\r\n",cc,i);
							}
							else
							{//���CASEֻ��ȡ�Եغ���ֵ����TPC_IR_BOT���ж϶Եغ����Ƿ�����
								//sys->g_buton[0][i-7] = k2 >> 2; //sys->g_sta[i];
								//sys->g_buton[1][i-7] = k1 >> 2;								
								/*if(sys->g_buton[0][i-7] > 300)//���˺���
								{
									bot |= 1 << (i - 7);
								}
								if(sys->g_buton[1][i-7] > 1500)//Զ�˺���
								{
									bot |= 1 << (i - 7 + 3);
								}*/
							}
							//log_printf("%d\t%d\r\n",i,sys->g_sta[i]);
							//delay(5);
						}
						tmp_cc = cc;
						if(cc)
						{
							if(tmp_cc&BIT8_STA(2))
								cc |= BIT8_STA(4);
							else
								cc &= ~(BIT8_STA(4));
							if(tmp_cc&BIT8_STA(4))
								cc |= BIT8_STA(2);
							else
								cc &= ~(BIT8_STA(2));				
							tx_com_dat(TPC_IR_FRT, 50 , TPC_ERR_IRLED, cc);//�ϱ����ȼ�������Ϣ
						#if BDTEST_LOGPRINT
							delay(50);
							log_printf("\r\ncc------:%02X:%d(%d-%d),%d(%d-%d)\r\n",cc,sys->g_sta[0],asse_irmaxmin[0].min,asse_irmaxmin[0].max,sys->g_sta[6],asse_irmaxmin[6].min,asse_irmaxmin[6].max);
							#endif
							////bdtest_sta_child = 1;				
							////mac_dat_pro(TPC_IR_FRT);	
							//delay(500);
							////break;
						}
						/*else
						{
							bdtest_sta_child = 1;						
							mac_dat_pro(TPC_IR_FRT);	
							//delay(500);
							break;
						}
						*/
						cc = tmp_cc;	//�ָ�cc

						#if BDTEST_LOGPRINT
						delay(500);
						log_printf("\r\nir_frt:\r\n");
						for(i = 0;i < 7;i ++)
						{
							log_printf("\t%d",sys->g_sta[i]);
						}
						log_printf("\r\n\t cc:%02X\r\nir_bot:\r\n",cc);
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[0][i]);
						}
						log_printf("\r\n");
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[1][i]);
						}
						log_printf("\r\n");
						#endif
					}					
					bdtest_sta_child = 1;						
					mac_dat_pro(TPC_IR_FRT);	
					delay(500);
					
					break;
				case 1:		//�ڵ������
					{
						uint8_t ccc;
						//#if BDTEST_LOGPRINT
						///uint8_t bot;
						//#endif
						//int32_t k1,k2;
						uint8_t i,j;
						
						
						for(ccc = 0;ccc < 9;ccc ++)
						{			
							if(ccc<=3)
							{
								GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
								GPIO_ResetBits(GPIOD,GPIO_Pin_14);	//�͵�ƽ���ص�
								
							}
							else
							{
								GPIO_SetBits(PORT_IR_CTRL,PIN_IR_CTRL);	
								GPIO_SetBits(GPIOD,GPIO_Pin_14);	
							}
							delay(1);
							for(i=0;i<10;i++)
							{
								if(ccc == 0)
									sys->m_sta[i][ccc] = 0;
								else
									sys->m_sta[i][ccc] = adc_converted_value[i];
								//log_printf(",%d",sys->m_sta[i][sys->c_ir_adc] );
							}	
						}
						
						bot = 0;//cc������¼ǰײ����ļ����,j������¼�Եغ���ļ����
						//cc = 0;
						for(i=0;i<10;i++)//�ѶԵغ���Ҳ�����,Ŀǰ���Եغ����������ȡ���ݣ���⻷��Ϊһ�����ߵ��ϰ���
						{
							k1 = k2 = 0;
							for(j=0;j<4;j++)
								k1 +=sys->m_sta[i][j];
							for(j=5;j<9;j++)
								k2 +=sys->m_sta[i][j];

							if(i < 7)//ǰײ����
							{
								/*
								if(i == 0 || i == 3 || i == 6)
								{
									sys->g_sta[i] = k1 >> 2;
								}
								else
								*/
								{
									if(k1 > k2 && ((k1 - k2) /4) < 5000)
										sys->g_sta[i] = (k1 - k2) >> 2;
									else
										sys->g_sta[i] = 0;
								}
								//if(sys->g_sta[i] < 500 || sys->g_sta[i] > 3500)//ǰײ����,�ں춨�����ȡֵ������Χ��Ϊ���ϸ�
								//if(sys->g_sta[i] < 1000 || sys->g_sta[i] > 3800)//ǰײ����,�ں춨�����ȡֵ������Χ��Ϊ���ϸ�
								//��һ·û��ʹ��,����һ·�������Ϊ�м�ֵ
								if(tm_mode == 1)		//��������
								{
									if((sys->g_sta[i] < irtestdata[i].min) || (sys->g_sta[i] > irtestdata[i].max))
									{
										cc |= BIT8_STA(i);
									}
								}
								else if(tm_mode == 2)	//���Ʒ
								{
									if((sys->g_sta[i] < asse_irtestdata[i].min) || (sys->g_sta[i] > asse_irtestdata[i].max))
									{
										cc |= BIT8_STA(i);
									}
								}
							}
							else
							{
								sys->g_buton[0][i-7] = k2 >> 2; //sys->g_sta[i];
								//sys->g_buton[1][i-7] = k1 >> 2;
								if(i==7 || i==9)
								{
									if(sys->g_buton[0][i-7] < 1800)//���˺���								
										bot |= BIT8_STA(i - 7);	
								}	
								///if(sys->g_buton[1][i-7] < 1800)//Զ�˺���								
								//	bot |= 1 << (i - 7 + 3);								
							}
							//log_printf("%d\t%d\r\n",i,sys->g_sta[i]);
						}

						//���������λ�÷���,��Ҫת����ȷ��λ���ϱ�����λ�����
						#if 1
						tmp_cc = cc;
						if(tmp_cc&BIT8_STA(2))
							cc |= BIT8_STA(4);
						else
							cc &= ~(BIT8_STA(4));
						if(tmp_cc&BIT8_STA(4))
							cc |= BIT8_STA(2);
						else
							cc &= ~(BIT8_STA(2));						
						#endif
						tx_com_dat(TPC_IR_FRT, 100 , cc?TPC_ERR_IRLED:TPC_ERR_NOERROR, cc);//�ϱ����ȼ�������Ϣ

						if(bot)	//�ڵ������,�д����ϱ�
							tx_com_dat(TPC_IR_BOT, 100 , TPC_ERR_IRLED, bot);//�ϱ����ȼ�������Ϣ
						#if BDTEST_LOGPRINT
							delay(50);
							log_printf("\r\ncc------:%02X:%d(%d-%d),%d(%d-%d)\r\n",cc,sys->g_sta[0],asse_irmaxmin[0].min,asse_irmaxmin[0].max,sys->g_sta[6],asse_irmaxmin[6].min,asse_irmaxmin[6].max);
							#endif
							////����
							//{
							//	tx_com_dat(TPC_IR_BOT, (sys->g_buton[0][0] /100) , TPC_ERR_IRLED, (sys->g_buton[0][0] %100));//�ϱ����ȼ�������Ϣ
							//	tx_com_dat(TPC_IR_BOT, (sys->g_buton[0][2] /100) , TPC_ERR_IRLED, (sys->g_buton[0][2] %100));//�ϱ����ȼ�������Ϣ
							//	bot = 0;
							//}
						#if BDTEST_LOGPRINT
						delay(500);
						log_printf("\r\n case 2ir_frt:\r\n");
						for(i = 0;i < 7;i ++)
						{
							log_printf("\t%d",sys->g_sta[i]);
						}
						log_printf("\r\n\t cc:%02X\r\nir_bot:\r\n",cc);
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[0][i]);
						}
						log_printf("\r\n");
						for(i = 0;i < 3;i ++)
						{
							log_printf("\t%d",sys->g_buton[1][i]);
						}
						log_printf("\r\n");
						#endif

							///���Խ���
						bdtest_sta_child = 2;													
					}
				break;
				case 2:
				bdtest_sta_child = 0;
				bdtest_sta = TPC_IR_BOT;
					mac_dat_pro(TPC_STOP_ALL);	
				break;
				default:
					break;
			}
			break;
		case TPC_IR_BOT://ǰײ���(��ײ)������,�Եغ���������һ����
			switch(bdtest_sta_child)
			{
				case 0:		//����ڵ�,����״̬
					{
						bdtest_sta_child = 3;
						///tm_ir_bot_k1 /*= tm_ir_bot_k2 */= 0;
						
//						tm_ir_bot_cc = 4;
						//tx_com_dat(TPC_IR_BOT, 50 , TPC_ERR_NOERROR, 0);//�ϱ���ʼ�Եغ������,�ȴ��û�������
						delay_ms(500);
						k1 = k2 = 0;
						for(uint8_t ccc = 0;ccc < 4;ccc ++)
						{
							k1 += adc_converted_value[7];
							k2 += adc_converted_value[9];
							delay(20);
						}
												
						
						k1 /=4;
						k2 /=4;
						if(k1 > 1500) //���
						{
							bot |= BIT8_STA(0);
						}
						if(k2 > 1500) //���
						{
							bot |= BIT8_STA(2);
						}
						/*
						//for(i=0;i<10;i++)//�ѶԵغ���Ҳ�����,Ŀǰ���Եغ����������ȡ���ݣ���⻷��Ϊһ�����ߵ��ϰ���
						{
							//k1 = k2 = 0;			
			
							{//���CASEֻ��ȡ�Եغ���ֵ����TPC_IR_BOT���ж϶Եغ����Ƿ�����
								sys->g_buton[0][i-7] = k2 >> 2; //sys->g_sta[i];
								//sys->g_buton[1][i-7] = k1 >> 2;
								if(sys->g_buton[0][i-7] > 1500)//���˺���
								{
									bot |= 1 << (i - 7);
								}
								//if(sys->g_buton[1][i-7] > 1500)//Զ�˺���
								//{
								//	bot |= 1 << (i - 7 + 3);
								//}
							}
							//log_printf("%d\t%d\r\n",i,sys->g_sta[i]);
						}
						*/
						tx_com_dat(TPC_IR_BOT, 50 , bot ? TPC_ERR_IRLED:TPC_ERR_NOERROR, bot);//�ϱ����ȼ�������Ϣ

				
						bdtest_sta = TPC_BUM_SW;
						bdtest_sta_child = 0;
					}
					break;
				case 5:
					break;
				default:
					bdtest_sta = TPC_BUM_SW;
					bdtest_sta_child = 0;
					bd_timer = 0;
					break;
			}
			break;
		case TPC_BUM_SW:
		{
		
					bdtest_sta = TPC_SWSENSOR;					
					bdtest_sta_child = 0;
					bdtest_fantest = 0;
						
		}	
			break;
		case TPC_SWSENSOR://��ȡ��������ʽ����������
		{
				
			//switch(bdtest_sta_child)
			{
				
					mac_dat_pro(TPC_STOP_ALL);
					delay_ms(1000);
												
					if(LEFT_MOTOR_LEAVE()==0)			//�ǳ���״̬	
						sw_un &= ~(BIT8_STA(SWSENSOR_LEV_L));						
					else
						sw_un |= BIT8_STA(SWSENSOR_LEV_L);	
					if(RIGHT_MOTOR_LEAVE()==0)		//�ǳ���״̬			
						sw_un &= ~(BIT8_STA(SWSENSOR_LEV_R));	
					else
						sw_un |= BIT8_STA(SWSENSOR_LEV_R);	
		
					
					mac_dat_pro(TPC_MT_MID);//����������
					delay_ms(1000);
			/*
					//(������/����ǽ/������/������)
					if(READ_DATA() == 0)
						sw_en &= ~(BIT8_STA(SWSENSOR_ULTRA));				//�Ǵ���״̬
					else						
						sw_en |= BIT8_STA(SWSENSOR_ULTRA);	
					if(!READ_VWALL_DET())							//����ǽ��¼�Ǵ���״̬
						sw_en &= ~(BIT8_STA(SWSENSOR_VW));	
					else 
						sw_en |= BIT8_STA(SWSENSOR_VW);	
			*/		
		#if 1				
					if(LEFT_MOTOR_LEAVE() == 0) 		//�ǳ���״̬			
						sw_en &= ~(BIT8_STA(SWSENSOR_LEV_L));	
					else
						sw_en |= BIT8_STA(SWSENSOR_LEV_L);
					if(RIGHT_MOTOR_LEAVE() == 0)		//�ǳ���״̬	
						sw_en &= ~(BIT8_STA(SWSENSOR_LEV_R));	
					else
						sw_en |= BIT8_STA(SWSENSOR_LEV_R);	
		#endif		
					sw_en|=sw_un;
					if(sw_en)
					{						
						tx_com_dat(TPC_SWSENSOR, 50, TPC_ERR_DIGI_IN, sw_en);//�ϱ����ȼ�������Ϣ
					}
					else
						tx_com_dat(TPC_SWSENSOR, 100, TPC_ERR_NOERROR, sw_en);//�ϱ����ȼ�������Ϣ
					//bdtest_sta_child = 3;	
				//break;
				//case 3:
					bdtest_sta = TPC_IRDA_CHRG;
					bdtest_sta_child = 0;
					MOTOR_POWER_OFF_NPRI();
					bdtest_fantest = 0;				
					
			}
		}	
		break;
		case TPC_IRDA_CHRG:
			mac_dat_pro(TPC_STOP_ALL);
			switch(bdtest_sta_child)
			{
			case 0:
			/*
				{
					uint8_t n_irdata[3] = {0};
					read_ir_original_data(n_irdata);//��һ�κ���,��֮ǰ��ֵ���
				}
				*/
				GPIO_ResetBits(PORT_IR_CTRL,PIN_IR_CTRL);	//�͵�ƽ���ص�
				bdtest_sta_child = 1;
				delay_ms(500);
				break;
			case 1:
				{
					uint8_t n_irdata[4] = {0};
					uint8_t n_irres=0;
					//uint8_t n_irknk = 0;
					//uint8_t i,cc;
					////delay_ms(500);
					read_ir_original_data(n_irdata);		
					#if 0
					n_irres = (n_irdata[0] | n_irdata[1] | n_irdata[2]) & 0x01;//ȡ�����ź�

					if(n_irres == 0x01) 
						n_irres&=~0x01;
					else
						n_irres=0x01;
					#endif	
					
					if(!n_irdata[0])n_irres |= BIT8_STA(0);
					if(!n_irdata[1])n_irres |= BIT8_STA(1);
					if(!n_irdata[2])n_irres |= BIT8_STA(2);
					if(!n_irdata[3])n_irres |= BIT8_STA(3);
					if(n_irres == 0)////if(n_irres != 0 )
					{		
						bdtest_sta_child = 0;
//						bd_batcharge_timer = 0;
						bdtest_sta = TPC_GYRO_TST;
						tx_com_dat(TPC_IRDA_CHRG, 100 , TPC_ERR_NOERROR, n_irres);//�ϱ����ȼ�������Ϣ					
					}
					else//// if(n_irres == 0)
					{
						bdtest_sta_child = 0;
						bdtest_sta = TPC_GYRO_TST;					
						tx_com_dat(TPC_IRDA_CHRG, 100 , TPC_ERR_IRLED, n_irres);//�ϱ����ȼ�������Ϣ
					}
					delay_ms(500);
				}
			
				break;
			case 2:
			default:
				bdtest_sta = TPC_GYRO_TST;	
				bdtest_sta_child = 0;
				delay_ms(100);
				break;
			}
			break;
		
		case TPC_GYRO_TST:		
			delay_ms(500);
			switch(bdtest_sta_child)
			{
				case 0:
				if(sys->angle>=0 && sys->angle<=360)////if(sys->gyro_init_ok)
					tx_com_dat(TPC_GYRO_TST, 100, TPC_ERR_NOERROR, 0);//�ϱ�����
				else
					tx_com_dat(TPC_GYRO_TST, 50, TPC_ERR_TYRO, 1);//�ϱ�����
				bdtest_sta = TPC_CHRG_TST;
				delay_ms(100);
				break;	
			}	
#if 0			
		case TPC_CD_FRT://����ǰ������,ǰ�����̵Ĳ��Ե�������ͻ�ת��ֱ�Ӷ�ǰ�����̼������Ϳ���
			{
				int n_cd_frt = navigat->distance;
				if(n_cd_frt < 0)n_cd_frt = -n_cd_frt;
				tx_com_dat(TPC_CD_FRT, 100 , (n_cd_frt > 20) ? TPC_ERR_NOERROR:TPC_ERR_FORWARD, 0);//�ϱ�����
				//tx_com_dat(TPC_CD_FRT, 100 , TPC_ERR_NOERROR, 0);//�ϱ�����
				bdtest_sta = TPC_CHRG_TST;
				bdtest_sta_child = 0;
				bd_batcharge_timer = 0;
			}
			break;
#endif			
		break;
		case TPC_CHRG_TST://������
			
			delay_ms(1000);
			if(DOCK_DETECT())
			{				
				if((1 == CHARGE_DONE_DETECT()) && (1 == CHARGE_ING_DETECT()))
				{
					tx_com_dat(TPC_CHRG_TST, 63, TPC_ERR_DOCK, 2);//�ϱ�����
					charge_result= 0;
				}
			}
				else
			{
					tx_com_dat(TPC_CHRG_TST, 63, TPC_ERR_DOCK, 1);//�ϱ�����
				charge_result= 0;
			}	

			delay_ms(1000);
			if(0 == EXTERAL_AC_DETECT())
			{
				tx_com_dat(TPC_CHRG_TST, 63, TPC_ERR_DCJACK, 1);//�ϱ�����				
				charge_result= 0;
			}				
			if(1 == charge_result)
			{
				tx_com_dat(TPC_CHRG_TST, 63, TPC_ERR_NOERROR, 0);//�ϱ�����
			}				
			bdtest_sta = TPC_HEART_BEEP;	
			break;
		case TPC_HEART_BEEP:
			delay_ms(20);
			check_bum();
			if(20 < bd_bum_timer++)
			{
				bd_bum_timer = 0;
				bum_com();
			}
		break;
		default:
			bdtest_sta = TPC_HEART_BEEP;
			delay_ms(2);
			break;
	}
//	bdtest_sta_old = bdtest_sta;
}




//��λ����ͨ�ų���
void proc_com_test_mode(uint8_t *buf,uint8_t len)
{
	TEST_PROC *proc_dat;

	proc_dat = (TEST_PROC *)buf;

	//log_printf("\r\n\r\n");
	//data_print(buf,len);
	switch(proc_dat->code)
	{
		case TPC_STOP_ALL://�˳�����ģʽ,ϵͳ����
			tx_msg(buf,sizeof(TEST_PROC));
			bdtest_sta = TPC_STOP_ALL;
			if(tm_mode == TM_MODE_BOARD)
				tx_dp_dat(TBD_STOP_ALL, 0, 0);
			mac_dat_pro(TPC_STOP_ALL);		
			//TargetSysReset();
			break;
		case TPC_START_ALL://�������ģʽ
			//if(bdtest_sta == TPC_STOP_ALL)
			{
				bdtest_sta = TPC_START_ALL;
				tx_msg(buf,sizeof(TEST_PROC));
				if(tm_mode == TM_MODE_BOARD)
					tx_dp_dat(TBD_START_ALL, 0, 0);					
				ny3p_play(VOICE_FAC_MODE);
				delay_ms(500);
		
				
			}
			break;
		case TPC_RESET://�������ģʽ
			tx_com_dat(TPC_RESET, 0 , TPC_ERR_NOERROR, 0);
			TargetSysReset();
			break;
		case TPC_IR_BOT://�Եغ���
			if(proc_dat->t_ret_value < 5)
				bdtest_sta_child = proc_dat->t_ret_value;
			tx_com_dat(TPC_IR_BOT, proc_dat->t_progress , TPC_ERR_NOERROR, proc_dat->t_ret_value);
			break;
		case TPC_CHRG_TST://������
			if(proc_dat->t_ret_value < 5)
				bdtest_sta_child = proc_dat->t_ret_value;
			tx_com_dat(TPC_IR_BOT, proc_dat->t_progress, TPC_ERR_NOERROR, proc_dat->t_ret_value);
			break;
		case TPC_SETSN://PC������д��SN��
			//delay_ms(100);
			if(len < 20)//������ݳ���С��20������������Ч����
			{
				#if BDTEST_LOGPRINT
				log_printf("error:data len = %d\r\n",len);
				#else
				tx_sn(TPC_SETSN,NULL);
				#endif
				break;
			}
			else
			{
				uint8_t n_chksum;
				n_chksum = get_chksum(buf, SNLENGTH + 1);
				if(n_chksum == buf[22])
				{
					memcpy(cfg->sn,buf+1,21);

					//log_printf("OK!\r\n");
					//sn_print(cfg->sn, 21);
					save_cfg();
					tx_sn(TPC_SETSN,cfg->sn);
					delay_ms(500);
					TargetSysReset();
				}
				else
				{
				#if BDTEST_LOGPRINT
					log_printf("\r\nerror:chksum = %02X-%02X\r\n",n_chksum,buf[22]);
					sn_print(buf + 1, 21);
					log_printf("\r\n");
				#else
					tx_sn(TPC_SETSN,NULL);
				#endif
					break;
				}
			}
			break;
		case TPC_GETSN://������PC���������SN��
			//ny3p_play(VOICE_DIDI);
			tx_sn(TPC_GETSN,cfg->sn);
			break;

		case TPC_TRANS_PACK:
			rec_mac_proc_dat.output_sta_1 = ((((unsigned short)(proc_dat->t_res))&0x00ff)<<8) | (((unsigned short)(proc_dat->t_progress))&0x00ff);	
			rec_mac_proc_dat.output_sta_2 = ((unsigned short)(proc_dat->t_ret_value))&0x00ff;
			break;
		default:
			if(proc_dat->code >= TPC_CD_FRT && proc_dat->code <= TPC_MT_FAN)
			{
				bdtest_sta = proc_dat->code;
				bdtest_sta_child = 0;
				tx_msg(buf,sizeof(TEST_PROC));
				if(tm_mode == TM_MODE_BOARD)
					tx_dp_dat(TBD_START_ALL, 0, 0);
				if(proc_dat->code != TPC_CHRG_TST)
				{
					LiBat_ExitChargeMode();
					delay_ms(100);
					if(tm_mode == TM_MODE_BOARD)
						tx_dp_dat(TBD_CHRG_TST, 0, 0);//��24V��DC�����ӿ�
				}
			}
			break;
	}
}

//����԰�Ķ˿�(���Կض˿�)usart2��������
void tx_dp_msg(uint8_t *buff,int len)
{
	uint8_t tmp[2];

	//������֮ǰ�Ȱ�δ��������,���Ȿ�ζ�����һ�εı��
	tmd_rxbd_dat.unread_flag = 0;

	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart3_write(tmp,2);
	usart3_write(buff, len);
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart3_write(tmp,2);

}


//����λ�������ϻ�����,�˺������ڻ����ϻ�������,����һֱ�����ر����ߣ����ϱ���������ײ���ݼ�һЩ���ϴ���
void tx_com_burnin(uint8_t side_dir, uint16_t timer,uint32_t bum_left_ct,uint32_t ir_left_ct,uint32_t bum_right_ct,uint32_t ir_right_ct)
{
	uint8_t i = 0;
	uint8_t tmp[2];
	TEST_BURNIN_PROC test_proc_dat;

	test_proc_dat.code = TPC_HEART_BEEP;
	test_proc_dat.para = side_dir;
	test_proc_dat.worktimer = timer;
	test_proc_dat.bum_ct[i ++] = bum_left_ct;
	test_proc_dat.bum_ct[i ++] = ir_left_ct;
	test_proc_dat.bum_ct[i ++] = bum_right_ct;
	test_proc_dat.bum_ct[i ++] = ir_right_ct;
	test_proc_dat.fw_ver = FW_VERSION;
	test_proc_dat.bat_volt= LiBat_GetBatVolt();
	test_proc_dat.errorcode = sys->work_errcode;


	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart_write(tmp,2);
	usart_write((uint8_t *)&test_proc_dat, sizeof(TEST_BURNIN_PROC));
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart_write(tmp,2);
}

//����λ��������������Ϣ
void tx_com_beep(uint8_t progress, uint8_t error_code, uint8_t ret_value,uint16_t *irvalue,uint16_t *irbuttom)
{
	uint8_t i;
	uint8_t tmp[2];
	TEST_BEEP_PROC test_proc_dat;

	test_proc_dat.code = TPC_HEART_BEEP;
	test_proc_dat.t_progress = progress;
	test_proc_dat.t_res = error_code;
	test_proc_dat.t_ret_value = ret_value;
	test_proc_dat.bat_volt= LiBat_GetBatVolt();
	test_proc_dat.fw_ver = FW_VERSION;
	if(EXTERAL_AC_DETECT())
	{
		test_proc_dat.bat_chstate = 1;
		test_proc_dat.bat_curr = LiBat_GetChargeCurrent();
	}
	else
	{
		test_proc_dat.bat_chstate = 0;
		test_proc_dat.bat_curr = 0;
	}
	for(i = 0;i < 7;i ++)
	{
		test_proc_dat.ir_frt_value[i] = irvalue[i];
	}
	for(;i < 10;i ++)
	{
		test_proc_dat.ir_frt_value[i] = irbuttom[i - 7];
	}

	delay_ms(300);
	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart_write(tmp,2);
	usart_write((uint8_t *)&test_proc_dat, sizeof(TEST_BEEP_PROC));
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart_write(tmp,2);
}

//����λ��������Ϣ
void tx_com_dat(uint8_t code, uint8_t progress, uint8_t error_code, uint8_t ret_value)
{
	uint8_t tmp[2];
	TEST_PROC test_proc_dat;

	delay(300);
	test_proc_dat.code = code;
	test_proc_dat.t_progress = progress;
	test_proc_dat.t_res = error_code;
	test_proc_dat.t_ret_value = ret_value;
	//test_proc_dat.fw_ver = FW_VERSION;
	for(uint8_t i=0;i<2;i++)
	{
		tmp[0] = 0x7E;
		tmp[1] = 0x5D;
		usart_write(tmp,2);
		usart_write((uint8_t *)&test_proc_dat, sizeof(TEST_PROC));
		tmp[0] = 0x6D;
		tmp[1] = 0x7D;
		usart_write(tmp,2);
		delay(20);
	}
}

#define PRI 1
//ͨ����λ��ת�������԰淢����Ϣ
void tx_com_mac_dat(TBDIO_UART *proc_dat)
{
	uint8_t tmp[3];
	delay_ms(300);
#if BDTEST_LOGPRINT
	log_printf("tx_com_mac_dat:");
#endif	
	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	tmp[2] = TPC_TRANS_PACK;
	usart_write(tmp,3);
#if BDTEST_LOGPRINT		
	for(uint8_t i=0;i<3;i++)
		log_printf("%02X ",tmp[i]);
#endif			
	
	usart_write((uint8_t *)proc_dat, 3);
#if BDTEST_LOGPRINT		
	tmp[0] = (uint8_t)(proc_dat->output_sta_1&0x00ff);
	tmp[1] = (uint8_t)((proc_dat->output_sta_1&0xff00)>>8);
	tmp[2] = (uint8_t)(proc_dat->output_sta_2&0x00ff);
	for(uint8_t i=0;i<3;i++)
		log_printf("%02X ",tmp[i]);
#endif			
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;	
	usart_write(tmp,2);
#if BDTEST_LOGPRINT		
	for(uint8_t i=0;i<2;i++)
		log_printf("%02X ",tmp[i]);
	log_printf("\r\n");	
#endif		
}
void mac_dat_pro(uint8_t type)
{
	uint32_t cc=0,c2=0,c3=0;
	test_mac_proc_dat.output_sta_1 = 0;
	test_mac_proc_dat.output_sta_2 = 0;
	rec_mac_proc_dat.output_sta_1 =	0xffff;
	rec_mac_proc_dat.output_sta_2 = 0xff;
	switch(type)
	{
		case TPC_STOP_ALL:
			test_mac_proc_dat.output_sta_1 = 0;
			test_mac_proc_dat.output_sta_2 = 0;
		break;
		case TPC_IR_FRT:	//ǰײ���� ���
		{			
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_F_7)|BIT_STA(VALVE_F_8); 				//�����ŷ�
			test_mac_proc_dat.output_sta_2 = BIT_STA(MOTER_0)|BIT_STA(MOTER_1)|BIT_STA(MOTER_2);	//����� 				
		}			
			break;
		case TPC_IR_BOT:	//ǰײ���� ���  add 2020-03-13
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_F_7)|BIT_STA(VALVE_F_8); 				//�����ŷ�
		break;
		case TPC_BUM_SW_L:	//��ײ12456
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_2);	
		break;
		case TPC_BUM_SW_ML:
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_1);
		break;	
		case TPC_BUM_SW_M:
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_4);
		break;	
		case TPC_BUM_SW_MR:	//��ײ12456			
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_6); 		
		break;
		case TPC_BUM_SW_R:	//��ײ12456			
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_5); 		
		break;
		case TPC_MT_MID:
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_3); 		
		break;
		case TPC_SWSENSOR:	//(����ǽ/����/��������/������/������/������/��������)
		{	
			test_mac_proc_dat.output_sta_1 = BIT_STA(VALVE_F_9); 	//����ǽ/������/
			test_mac_proc_dat.output_sta_2 = /*BIT_STA(MOTER_3)|BIT_STA(MOTER_4)|*/BIT_STA(MOTER_5);	///������/�����ֺ�������
		}	
		break;
		default:
			return;
		//break;
	}
	tx_com_mac_dat(&test_mac_proc_dat);//�ϱ�����
	cc = 0;
	c2 = 0;
	c3 = 0;
	while(1)	//�ȴ�״̬ͬ��
	{
		delay_ms(10);
		proc_uart_task();	
		if(rec_mac_proc_dat.output_sta_1 == test_mac_proc_dat.output_sta_1 && rec_mac_proc_dat.output_sta_2 == test_mac_proc_dat.output_sta_2)
		{
#if BDTEST_LOGPRINT			
			log_printf("rec_mac_proc_dat ok\r\n");
#endif			
			break;
		}		
		if(cc++>300)//����8s
		{
#if BDTEST_LOGPRINT			
			log_printf("rec_mac_proc_dat err\r\n");
#endif				
			break;
		}
		if(c2++>20&&c3<4)
		{	
			c2 = 0;
			c3++;
			tx_com_mac_dat(&test_mac_proc_dat);//�ϱ�����
		}	
	}
}

//����԰巢����Ϣ
void tx_dp_dat(uint8_t code, uint8_t para, uint8_t para1)
{
#if (1)
	delay_ms_tm5(200);
	uint8_t tmp[2];
	TEST_BD_PROC test_bd_dat;
	//print_tbdio_i2c(&tbdio_i2c);
	test_bd_dat.code = code;
	test_bd_dat.para = para;
	test_bd_dat.para1 = para1;
	test_bd_dat.t_res_input = 0;
	test_bd_dat.t_res_output = 0;
	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart4_write(tmp,2);
	usart4_write((uint8_t *)&test_bd_dat, sizeof(TEST_BD_PROC));
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart4_write(tmp,2);

#else
	tbdio_i2c.package_flag = 0xa5;
	if(code == TBD_STOP_ALL)
		tbdio_i2c.tbstate = TBI2C_TBDSTA_IDLE;
	else
		tbdio_i2c.tbstate = TBI2C_TBDSTA_BUSY;
	switch(code)
	{
		case TBD_STOP_ALL:
			tbdio_i2c.output_sta_1 = tbdio_i2c.output_sta_2 = 0;
			tbdio_i2c.pwm_motor = 0;
			break;
		case TPC_START_ALL:
			tbdio_i2c.output_sta_1 &= ~TBI2C_O1_BAT;
			break;
		case TPC_CD_FRT://ǰ��������ʱ����
			break;
		case TBD_MTOC_WHL_L:
			if(para == 1)
			{
				tbdio_i2c.output_sta_1 |= TBI2C_O1_MTOC_WHL_L;
			}
			else
			{
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_MTOC_WHL_L;
			}
			break;
		case TBD_MTOC_WHL_R:
			if(para == 1)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_MTOC_WHL_R;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_MTOC_WHL_R;
			break;
		case TBD_MTOC_SMT_L:
			if(para == 1)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_MTOC_SMT_L;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_MTOC_SMT_L;
			break;
		case TBD_MTOC_SMT_R:
			if(para == 1)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_MTOC_SMT_R;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_MTOC_SMT_R;
			break;
		case TBD_MTOC_MT_MID:
			if(para == 1)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_MTOC_MT_MID;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_MTOC_MT_MID;
			break;
		case TBD_MTFALL_L:
			if(para == 1)
				tbdio_i2c.output_sta_2 |= TBI2C_O2_MTF_L;
			else
				tbdio_i2c.output_sta_2 &= ~TBI2C_O2_MTF_L;
			break;
		case TBD_MTFALL_R:
			if(para == 1)
				tbdio_i2c.output_sta_2 |= TBI2C_O2_MTF_R;
			else
				tbdio_i2c.output_sta_2 &= ~TBI2C_O2_MTF_R;
			break;
		case TBD_BUM_SW:
			if(para1 & 0x01)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_BUML;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_BUML;
			if(para1 & 0x02)
				tbdio_i2c.output_sta_1 |= TBI2C_O1_BUMR;
			else
				tbdio_i2c.output_sta_1 &= ~TBI2C_O1_BUMR;
			break;
		case TBD_SWSENSOR:
				tbdio_i2c.output_sta_2 = para1;
			break;
		case TBD_CHRG_TST:
			tbdio_i2c.output_sta_1 &= ~TBI2C_O1_BAT;
			tbdio_i2c.output_sta_1 |= (uint16_t)(para << 14);
			#if 0
			if(para == 3)//��ضϿ�,��15V���ص�Դ��������й���
			{
				tbdio_i2c.output_sta_1 |= TBI2C_O1_BAT;
			}
			else if(para == 1)//��ؽ���,�ҳ���Դ(24V������)����DC��ͷ
			{
				tbdio_i2c.output_sta_1 |= 0x01 << 14;
			}
			else if(para == 1)//��ؽ���,�ҳ���Դ(24V������)���������ӿ�
			{
				tbdio_i2c.output_sta_1 |= 0x02 << 14;
			}
			#endif
			break;
		default:
			return;
	}
	I2C1_writebytes(0x20, 0x00, (uint8_t *)&tbdio_i2c, SIZEOFTBDIO_I2C);
	print_tbdio_i2c(&tbdio_i2c);
	uint8_t *ptr;
	ptr = (uint8_t *)&tbdio_i2c;
	for(uint8_t cc = 0;cc < (SIZEOFTBDIO_I2C);cc ++)
	{
		log_printf("%02X ",ptr[cc]);
	}
	log_printf("\r\n");
#endif
}


//����԰巢����Ϣ,�����������PC��ת�����Թ�װ��,���������������������
void tx_dp_pc_dat(uint8_t code, uint8_t para, uint8_t para1)
{
	uint8_t tmp[2];
	TEST_BD_PROC test_bd_dat;

	test_bd_dat.code = code;
	test_bd_dat.para = para;
	test_bd_dat.para1 = para1;
	test_bd_dat.t_res_input = 0;
	test_bd_dat.t_res_output = 0;

	//data_print((uint8_t *)&test_bd_dat, sizeof(TEST_BD_PROC));

	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart_write(tmp,2);
	usart_write((uint8_t *)&test_bd_dat, sizeof(TEST_BD_PROC));
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart_write(tmp,2);
}

uint8_t get_chksum(uint8_t *pdata, uint16_t length)
{
	uint16_t i;
	uint8_t ret = 0;

	for(i = 0;i < length;i ++)
	{
		ret += pdata[i];
	}
	return ret;
}

//��PC����SN,ʹ�õ�ָ����ΪTPC_GETSN �� TPC_SETSN
//���SNΪNULL����ʾ���͵�SN��Ч
void tx_sn(uint8_t code ,uint8_t *sn)
{
	uint8_t tmp[2];
	TEST_SN_PROC sn_proc;

	sn_proc.code = code;
	if(sn)
	{
		memcpy(sn_proc.sn,sn,SNLENGTH);
	}
	else
	{
		sn_proc.sn[0] = 0xff;
	}

	sn_proc.chksum = get_chksum((uint8_t *)&sn_proc, sizeof(TEST_SN_PROC) - 1);
	//data_print((uint8_t *)&sn_proc, sizeof(TEST_SN_PROC));

	tmp[0] = 0x7E;
	tmp[1] = 0x5D;
	usart_write(tmp,2);
	usart_write((uint8_t *)&sn_proc, sizeof(TEST_SN_PROC));
	tmp[0] = 0x6D;
	tmp[1] = 0x7D;
	usart_write(tmp,2);
}
#define DIS_S 50
void walk_tesk_for_whele(void)
{
	uint8_t err=0;
	sys->shut_down_motor = 1;		//�ص��
	navigat_init(0);				//��ʼ��
    navigat->angle = 0;
	sys->sState = SYS_NAVIGAT;		//����״̬������ֱ�к������˳�
	navigat->distance = motor.c_left_hw = motor.c_right_hw = 0;	//����
	motor_go_forwark(0,NO_SIDE_NEAR,NULL);	//ֱ��


	float dis = motor.c_left_hw * 0.87f;			//�����ǰ��ֵ

	if( disXY(navigat->distance,(int)dis) >= DIS_S)		//ǰ�ֺ����ֶԱ�
	{
		err = 1;
		log_printf( "Errdis0=%d,%d\r\n",navigat->distance,(int)dis);
	}
	if( disXY(motor.c_left_hw,motor.c_right_hw) >= DIS_S)	//���ֺ����ֶԱ�
	{
		err = 1;
		log_printf( "Errdis1=%d,%d\r\n",motor.c_left_hw,motor.c_right_hw);
	}

	turn_to_deg(180);
	//delay_ms(800);//��ͷ180��
	 if(err == 1)
	{
		for(int i=0;i<20;i++)
		{
			ny3p_play(VOICE_DIDI);
			delay_ms(50);
		}

	}
	else
	{
     delay_ms(800);
	}
	navigat->distance = motor.c_left_hw = motor.c_right_hw = 0;

	motor_go_forwark(0,NO_SIDE_NEAR,NULL);	//ֱ��
	sys->sState = SYS_IDLE;
	 dis = motor.c_left_hw * 0.87f;
	if( disXY(navigat->distance,(int)dis) >= DIS_S)
	{
		err = 1;
		log_printf( "Errdis2=%d,%d\r\n",navigat->distance,(int)dis);
		}
	if( disXY(motor.c_left_hw,motor.c_right_hw) >= DIS_S)
	{
		err = 1;
     log_printf( "Errdis3=%d,%d\r\n",motor.c_left_hw,motor.c_right_hw);
     }
	//�������
	if(err == 1)
	{
		for(int i=0;i<40;i++)
		{
			ny3p_play(VOICE_DIDI);
			delay_ms(50);
		}

	}

	STOP_ALL_MOTOR();
}

