/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.h
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v2.3.8
** �ա���: 2018��1��17��
** �衡��: 1:�������volatile��ֹ�������Ż�
           2:���#error��ʾ
		   
** ��  ��: v2.3.7
** �ա���: 2017��4��18��
** �衡��: 1:�Ż����ڶ��н��մ���
		   
** ��  ��: v2.3.6
** �ա���: 2016��7��21��
** �衡��: 1:�޸���ȡ����ʱ�����
           2:���hex_to_bcdת������
		   
** ��  ��: v2.3.5
** �ա���: 2016��6��3��
** �衡��: 1:�޸ķ���Э��汾Ϊ0x01
           2:�̼���������ƫ�����޸�Ϊ4�ֽ�

** ��  ��: v2.3.4
** �ա���: 2016��5��26��
** �衡��: 1:�Ż����ڽ�������
           2:�Ż�������������,ȡ��enum���Ͷ���

** ��  ��: v2.3.3
** �ա���: 2016��5��24��
** �衡��: 1:�޸�mcu��ȡ����ʱ�亯��
           2:���wifi���ܲ���

** ��  ��: v2.3.2
** �ա���: 2016��4��23��
** �衡��: 1:�Ż��������ݽ���
           2:�Ż�MCU�̼���������
           3:�Ż��ϱ�����

** ��  ��: v2.3.1
** �ա���: 2016��4��15��
** �衡��: 1:�Ż��������ݽ���

** ��  ��: v2.3
** �ա���: 2016��4��14��
** �衡��: 1:֧��MCU�̼���������

** ��  ��: v2.2
** �ա���: 2016��4��11��
** �衡��: 1:�޸Ĵ������ݽ��շ�ʽ

** ��  ��: v2.1
** �ա���: 2016��4��8��
** �衡��: 1:����ĳЩ��������֧�ֺ���ָ�����ѡ��

** ��  ��: v2.0
** �ա���: 2016��3��29��
** �衡��: 1:�Ż�����ṹ
           2:��ʡRAM�ռ�
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_

/******************************************************************************
                            �û������Ϣ����
******************************************************************************/
/******************************************************************************
                            1:�޸Ĳ�Ʒ��Ϣ                
******************************************************************************/
#if MAP_VERSION
#if MAP_VERSION_V == 1
#define PRODUCT_KEY "HDCHze2wkUzxKr4I"    //"hBP3VvF9DMkXQ3FO"//    //����ƽ̨������Ʒ�����ɵ�16λ�ַ���ƷΨһ��ʶ
#elif MAP_VERSION_V == 2
#define PRODUCT_KEY "SQlCb5OAF7c8ABgz"
#elif MAP_VERSION_V == 3
#define PRODUCT_KEY "lggtlhsevxavadx5"//"sq3iuotbitcz3gyk"//
#endif
#else
#define PRODUCT_KEY "hBP3VvF9DMkXQ3FO"
#endif
//������ʽѡ��,Ĭ��ΪCONFIG_MODE_DEFAULT,ֻ����ѡһ
#define CONFIG_MODE     CONFIG_MODE_DEFAULT             //Ĭ��������ʽ
//#define CONFIG_MODE     CONFIG_MODE_LOWPOWER            //�͹���������ʽ
//#define CONFIG_MODE     CONFIG_MODE_SPECIAL             //����������ʽ

#define MCU_VER "1.0.0"                                 //�û�������汾,����MCU�̼�����,MCU�����汾���޸�
/******************************************************************************
                          2:MCU�Ƿ���Ҫ֧�̼�����                  
����Ҫ֧��MCU�̼�����,�뿪���ú�
MCU�ɵ���mcu_api.c�ļ��ڵ�mcu_firm_update_query()������ȡ��ǰMCU�̼��������
                        ********WARNING!!!**********
��ǰ���ջ�����Ϊ�رչ̼����¹��ܵĴ�С,�̼�������Ϊ256�ֽ�
����Ҫ�����ù���,���ڽ��ջ���������
******************************************************************************/
//#define         SUPPORT_MCU_FIRM_UPDATE                 //����MCU�̼���������(Ĭ�Ϲر�)
/******************************************************************************
                         3:�����շ�����:
                    �統ǰʹ��MCU��RAM����,���޸�Ϊ24
******************************************************************************/
//#ifndef SUPPORT_MCU_FIRM_UPDATE
//#define WIFI_UART_QUEUE_LMT             16              //���ݽ��ն��д�С,��MCU��RAM����,����С
//#define WIFI_UART_RECV_BUF_LMT          24              //�����û�DP���ݴ�С����,�������24
//#else
#define WIFI_UART_QUEUE_LMT             256             //���ݽ��ն��д�С,��MCU��RAM����,����С
#define WIFI_UART_RECV_BUF_LMT          300             //�̼�����������,��󻺴�,�������260
//#endif

#define WIFIR_UART_SEND_BUF_LMT         256//24              //�����û�DP���ݴ�С����,�������24
/******************************************************************************
                        4:����ģ�鹤����ʽ
ģ���Դ���:
          wifiָʾ�ƺ�wifi��λ��ť����wifiģ����(����WIFI_CONTROL_SELF_MODE��)
          ����ȷ����WF_STATE_KEY��WF_RESET_KEY
MCU�Դ���:
          wifiָʾ�ƺ�wifi��λ��ť����MCU��(�ر�WIFI_CONTROL_SELF_MODE��)
          MCU����Ҫ����λwifi�ĵط�����mcu_api.c�ļ��ڵ�mcu_reset_wifi()����,���ɵ���mcu_get_reset_wifi_flag()�������ظ�λwifi���
          ���������wifiģʽmcu_api.c�ļ��ڵ�mcu_set_wifi_mode(WIFI_CONFIG_E mode)����,���ɵ���mcu_get_wifi_work_state()������������wifi���
******************************************************************************/
//#define         WIFI_CONTROL_SELF_MODE                       //wifi�Դ�������LEDָʾ��;��ΪMCU��簴��/LEDָʾ����رոú�
//#ifdef          WIFI_CONTROL_SELF_MODE                      //ģ���Դ���
  #define     WF_STATE_KEY            14                    //wifiģ��״ָ̬ʾ�����������ʵ��GPIO�ܽ�����
  #define     WF_RESERT_KEY           0                     //wifiģ�����ð����������ʵ��GPIO�ܽ�����
//#endif

/******************************************************************************
                      5:MCU�Ƿ���Ҫ֧��Уʱ����                     
����Ҫ�뿪���ú�,����Protocol.c�ļ���mcu_write_rtctimeʵ�ִ���
mcu_write_rtctime�ڲ���#err��ʾ,��ɺ�������ɾ����#err
mcu��wifiģ����ȷ������ɵ���mcu_get_system_time()��������Уʱ����
******************************************************************************/
//#define         SUPPORT_MCU_RTC_CHECK                //����Уʱ����

/******************************************************************************
                      6:MCU�Ƿ���Ҫ֧��wifi���ܲ���                     
����Ҫ�뿪���ú�,����mcu����Ҫwifi���ܲ��Դ�����mcu_api.c�ļ���mcu_start_wifitest
����protocol.c�ļ�wifi_test_result�����ڲ鿴���Խ��,
wifi_test_result�ڲ���#err��ʾ,��ɺ�������ɾ����#err
******************************************************************************/
//#define         WIFI_TEST_ENABLE                //����WIFI���⹦��



#define WIFI_COOR_MAX_NUM	10
/******************************************************************************
                      9:�Ƿ�֧����������                 
******************************************************************************/
//#define         WIFI_STREAM_ENABLE              //֧����������ع���
//#ifdef WIFI_STREAM_ENABLE
#define         STREM_PACK_LEN                 256
//#endif

#define 		STREAM_MAX_PACK					10




/******************************************************************************
                        1:dp���ݵ����к����¶���
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
//��ɨ����(ֻ�·�)
#define DPID_START				1

//����ģʽ(���·����ϱ�)
//��ֵ��
//standby������ģʽ�� chargego:�س�ģʽ��dry_rub:�ɲ�,wet_wipe:ʪ��
#define DPID_TYPE				2

//��ɨ���(ֻ�ϱ�)
//��ֵ��
//��Ҫ����һλС����ʾ
#define DPID_CLEAN_AREA 			3

//��ɨʱ��(ֻ�ϱ�)
//��ֵ��
//MCU�ϱ�������ɨʱ�䣬ʵʱ����
#define DPID_CLEAN_TIME			4

//����ɨ���(ֻ�ϱ�)
//��ֵ��
//��Ҫ����һλС����ʾ
#define DPID_TOTAL_AREA 			5

//����ɨʱ��(ֻ�ϱ�)
//��ֵ��
//MCU�ϱ�������ɨʱ�䣬ʵʱ����
#define DPID_TOTAL_TIME			6

//���� 0~5 (ֻ�ϱ�)
#define DPID_VOICEMODULEVOLUME				7

//���ģʽ(���·����ϱ�)
//��ֵ��
#define DPID_FAN					8

//��������(���·����ϱ�)
//��ֵ��
#define DPID_SPEAKER				9

//����ǽ����(���·����ϱ�)
//raw��
#define DPID_WALL				10

//��ͼ��Χ(ֻ�·�)
//raw��
#define DPID_MAP					11

//��ɨ��¼(ֻ�ϱ�)
//raw��
#define DPID_COORDINATE			12

//������ɨ(���ϱ����·�)
//raw��
#define DPID_SPOT				13

//�汾��(ֻ�ϱ�)
//������
#define DPID_VERSION				14

//�ر���ɨ����(���·����ϱ�)
//bool��
#define DPID_EDGE_CLEANING		15

//����������(���·����ϱ�)
//��ֵ��
#define DPID_SENSITIVITY			16

//�����ٶ�(���·����ϱ�)
//��ֵ��
#define DPID_WHEEL				17

//���ģʽ(���·����ϱ�)
//��ֵ��
#define DPID_CHARGE				18

//����(ֻ�ϱ�)
//��ֵ��
#define DPID_BATTERY				19

//�澯(ֻ�ϱ�)
//��ֵ��
#define DPID_WARNING			20

//����(ֻ�·�)
//��ֵ��
//"ǰ�������ˣ���ת����ת��ֹͣ��
//Ĭ���߼�����ס������·� ǰ/��/��ת/��ת����ָ̧���·�ֹͣ"
#define DPID_DIRECTION_CONTROL 	21

//��ɨ���(ֻ�ϱ�)
//��ֵ��
#define DPID_FINISH				22

//���������(ֻ�ϱ�)
//raw��
#define DPID_CHARGESTATION		23

//��ͣ��ɨ(ֻ�·�)
//��ֵ��
#define DPID_PAUSE_RESTART 		24

//Ѱ���豸����(ֻ�·�)
//��ֵ��
#define DPID_FIND_SOUND			25

//���س����(ֻ�·�)
//��ֵ��
#define DPID_RETURN_TO_BASE 		26

//��̺��⿪��(ֻ�·�)
//��ֵ��
#define DPID_FLOOR_SENSOR_STATUS 	27

//�ƹ�״̬(���·�)
//bool(��/��)
#define DPID_LIGHT_STATUS 	28

//double clean mode(���·����ϱ�)
//bool
#define DPID_DOUBLECLEAN_MODE 	29

//map status(ֻ�ϱ�)
//bool
#define DPID_MAP_STATUS 	30

//robot position(ֻ�ϱ�)
//int*2(int,int)
#define DPID_ROBOT_POSITION 	31

//floor Sensitivity Level(���·����ϱ�)
//int(0~5)
#define DPID_FLOORSENSITIVITY_LEVEL 	32

//floor Sensor Status(ֻ�·�)
//bool
#define DPID_FLOORSENSOR_STATUS 	33

//dirty Map(ֻ�ϱ�)
//�ṹ��
// X1(0~500), Y1(0~500), level(0~2)
#define DPID_DIRTYMAP 	34

//dirty Map(ֻ�ϱ�)
//�ṹ��
// (wd,h,m,powermode,enable)
#define DPID_SCHEDULE 	35

//strainer_clear



#define WIFICONFIG 			0
#define WIFI_STREAM_RECONNECTION_EN				0//1


#define WIFI_STREAM_STATE_IDEL					0     //wifi��״̬
#define WIFI_STREAM_STATE_OPEN					1     //WiFi����
#define WIFI_STREAM_STATE_START					2     //WiFi����ʼ
#define WIFI_STREAM_STATE_STREAM				3
#define WIFI_STREAM_STATE_STREAMEN				4
#define WIFI_STREAM_STATE_OVER					5
//#define WIFI_STREAM_STATE_WAIT					6



#define WIFI_STREAM_STATE_UPDATA_ALL			9

#define STATE_NULL 		0
#define STATE_START 	1
#define STATE_SUSS	 	2
#define STATE_ERR	 	3


typedef struct h_spotattribute_t
{
	unsigned char x;
	unsigned char y;
	unsigned char spot_attrib;
}spotattribute_t;

typedef struct h_cleantimerecord_t
{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	//unsigned int natmr;		
}cleantimerecord_t;

typedef struct h_wifistreamdata_t
{
	unsigned char wifiStreamState;													//wifi������״̬
	unsigned char send,write;		
	unsigned char sync_flg;			

	unsigned int pack_offset;														//�ϱ���ƫ����
	unsigned int record_navi_work_time;												//�ϱ���ɨʱ��	
	cleantimerecord_t clean_time_record;											//��ɨʱ��
	spotattribute_t stream_cache[STREAM_MAX_PACK*20];								//200�������
	unsigned char stream_work_start;												//��������־	
	unsigned char cleanover,lastpack;
	unsigned char repeat;
	unsigned char updata_dock_flag;
	unsigned char wifiStreamStart;
	unsigned char wifiAreaNoCalc;
	
}wifistreamdata_t;

extern wifistreamdata_t *pwifistream,wifistreamdata;		//������

#define POINT_CURRENT	0x00//0x00   ��ǰ��
#define POINT_OBSTACLE	0x01//0x01   �ϰ���
#define POINT_SCAN		0x02//0x02   ��ɨ��
#define POINT_DOCK		0x03//0x03   �����


typedef struct _t_wifi_sch_data{
		unsigned char weekday;
		unsigned char hour;
		unsigned char minute;
		unsigned char powermode;
		unsigned char en;
}T_WIFI_SCH_DATA;
#define SIZEOF_WIFISCHDATA	5	
#define SIZEOF_WIFISCHDATA_ARRAY	(SIZEOF_WIFISCHDATA * 7)	

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�
������� : ��
���ز��� : ��
ʹ��˵�� : MCU����ʵ�ָú����������ϱ�����
*****************************************************************************/
void all_data_update(void);


//add by jzz 2018-06-25
void wifi_updata_area_data(void);
void wifi_daemon_pro(unsigned char sta);
void init_stream_clean_id(void);
void updata_stream_cache_data(unsigned char tx,unsigned char ty,unsigned char state,unsigned char spot_attrib);


//add by jzz 2018-06-25 end


//#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : ��
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[]);
//#endif

//#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
�������� : wifi_test_result
�������� : wifi���ܲ��Է���
������� : result:wifi���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi);
//#endif

//#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
�������� : mcu_firm_update_handle
�������� : MCU����̼�����ģʽ
������� : value:�̼�������
           position:��ǰ���ݰ����ڹ̼�λ��
           length:��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length);
//#endif

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
value:dp���ݻ�������ַ
length:dp���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void);

ErrorStatus update_coordinate(void);
ErrorStatus update_machmode(unsigned short mode);

ErrorStatus update_wheelspeed(void);

void wifi_sync_state_pro(uint8_t wifi_state);

void wifi_updata_clean_record(void);
////void update_dock_point(void);
extern void update_map_over(void);
//���������
extern ErrorStatus update_dock_coord(signed short coord_x,signed short coord_y);
//������ɨ �Խ�����
extern ErrorStatus update_spot_area(XY_T coord[]);
void wifi_set_apmode(void);
#endif

