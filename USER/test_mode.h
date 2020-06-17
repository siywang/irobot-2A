
#ifndef __TEST_MODE__
#define __TEST_MODE__

//����ϵͳ����汾�������ϱ����������
#define FW_VERSION	180

#define USE_ON_COMPUTER 0

#if 0
enum __tproc_code{
	TPC_STOP_ALL = 0,//ֹͣ���м��
	TPC_START_ALL,//��ʼ���м��
	TPC_CD_FRT,//ǰ�����̼��
	TPC_WHL_L,//����ּ��
	TPC_WHL_R,//�Һ��ּ��
	TPC_SMT_L,//���ˢ���
	TPC_SMT_R,//�ұ�ˢ���
	TPC_MT_MID,//��ɨ������
	TPC_MT_FAN,//������
	TPC_IR_FRT,//ǰײ������
	TPC_IR_BOT,//�Եغ�����
	TPC_IRDA_CHRG,//�س������
	TPC_BUM_SW,//ǰײ���������
	TPC_SWSENSOR,//�������������(Ĩ��/����ǽ/����/��������)
	TPC_CHRG_TST,//�����
};
#else
#define TPC_STOP_ALL	0//ֹͣ���м��
#define TPC_START_ALL	1//��ʼ���м��
#define TPC_CD_FRT	2//ǰ�����̼��
#define TPC_WHL_L	3//����ּ��y
#define TPC_WHL_R	4//�Һ��ּ��y
#define TPC_SMT_L	5//���ˢ���n
#define TPC_SMT_R	6//�ұ�ˢ���n
#define TPC_RESET	8//������״̬�µ�����ָ��һ��
#define TPC_MT_MID	7//��ɨ������
#define TPC_IR_FRT	9//ǰײ������
#define TPC_IR_BOT	10//�Եغ�����
#define TPC_IRDA_CHRG	11//�س������
#define TPC_BUM_SW		12//ǰײ���������
#define TPC_SWSENSOR	13//�������������(����ǽ/����/��������/������/������/������/��������)
#define TPC_CHRG_TST	14//�����
#define TPC_DPPORT_TST	15//�Կض˿ڼ��
#define TPC_GYRO_TST	16//�����Ǽ��
#define TPC_MT_FAN		17//������

#define TPC_CT_KEY		18//�������
#define TPC_CT_REMOTE		19//ң�ؼ��
#define TPC_COM_DISPLAY		20//�Կؼ��
#define TPC_IDLE	0xfa//������״̬,�����м������ɣ��л������״̬���ȴ�д��SN
#define TPC_SETSN	0xfb//д��SN��,����Ϊ20�ֽڵ��ַ���,����ת�����ܴ洢
#define TPC_GETSN	0xfc//����SN��
#define TPC_TRANS_PACK	0xfd//ת��PC�·�����װ�ĸ�ͨ·������
						//para = 0 ��������,t_res_output��ÿ1λ����һ������Ķ���(����U�ι������λתһȦ)
						//para = 1 ��0�ű����̵ļ���
						//t_res_output ���ø�����Ĺ���״̬(1Ϊ���ڶ���(����) 0Ϊͣ�� 2Ϊ��ת),
						//			t_res_output 	bit15-14 ǰ�ֵ�� 
						//							bit13-12 �����൲����(1Ϊ��λ���� 2Ϊ��λ���� 0Ϊ��ֹ)
						//							bit11-8 ģ����ײ���1-4
						//							bit7-bit3 �̵������1-5��״̬
						//							bit2-bit0 �ؼ���1-3״̬
						//�Ϸ���PCʱʹ�õ���TEST_PROC�ṹ�� ��t_res��t_ret_value���uint16_t λ����������t_res_output,ΪС�˸�ʽ
						
#define TPC_HEART_BEEP	0xfe//�����ź�,ÿ�뷢һ��
#endif
#define TPC_BUM_SW_L (TPC_BUM_SW+38)
#define TPC_BUM_SW_ML (TPC_BUM_SW+39)	//����
#define TPC_BUM_SW_M (TPC_BUM_SW+40)
#define TPC_BUM_SW_MR (TPC_BUM_SW+41)	//����
#define TPC_BUM_SW_R (TPC_BUM_SW+42)

#define IR_FRT		1



#define SWSENSOR_ULTRA	0
#define SWSENSOR_VW		1
#define SWSENSOR_LEV_L	2
#define SWSENSOR_LEV_R	3
#define SWSENSOR_DUSTFULL	4//����


#define NORM_SENSOR_BIN		0//���п���
#define NORM_SENSOR_KEY_WIFI	1//WIFI����
#define NORM_SENSOR_KEY_SPOT	2//������ɨ����
#define NORM_SENSOR_KEY_START	3//��ʼ/ֹͣ����
#define NORM_SENSOR_KEY_DOCK	4//�س䰴��
#define NORM_SENSOR_REMOTE		5//ң����
#define NORM_SENSOR_DISPLAY		6//�Կ�


#define TPC_ERR_NOERROR	0//�޴���
#define TPC_ERR_FORWARD	1//�����ת��ת������
#define TPC_ERR_BACK	2//�����ת����
#define TPC_ERR_DIGI_IN	3//�������������, ��:��ײû��Ӧ
#define TPC_ERR_DIGI_OUT	4//�������������,��:����LED������
#define TPC_ERR_COMM	5//ͨ�Ŵ���
#define TPC_ERR_OC		6//�������������
#define TPC_ERR_IRLED	7//������մ���
#define TPC_ERR_TB		8//���԰����
#define TPC_ERR_BATNE	9//��ز�����
#define TPC_ERR_BATOT	10//��ع���
#define TPC_ERR_DOCK	11//���������·����
#define TPC_ERR_DCJACK	12//DC������·����
#define TPC_ERR_BATCH	13//��س��ʧ��
#define TPC_ERR_TYRO	14//�����Ǵ���

//��λ��������ͨ�ŵĽṹ��
typedef struct __test_mode_proc{
	unsigned char code;//����ָ����
	unsigned char t_progress;//������,��λ���·�0Ϊ��ʼ���,��λ���ظ���100Ϊ�������/��Ϊ�������ļ���(ѭ��)
	unsigned char t_res;//�����,���궨�� TPC_ERR_XX
	unsigned char t_ret_value;//���صĸ�������,ֻ���ں���ȶ�·ͬʱ�����ʹ��,������ʾ�������ͨ����(bit0-bit7����ͨ��0-7)
								// ����ǶԵغ���,bit0-2Ϊ���˵ĸ�·����,bit3-5ΪԶ�˵ĸ�·����
								//����ǿ�����,��ײ�ź�:bit0 1 2 3 4//�� ��1 �� ��1 ��
								//�س� bit0 1 2 ������
								//����������: bit0-bit6�ֱ�Ϊ:(������/����ǽ/������/������/��������)
	unsigned char chksum;//У��λ,����û�б�ʹ��
}TEST_PROC;

typedef struct __testbdio_uart{	
	uint16_t output_sta_1;//��������ʽ���������ź�ģ������˿�
	uint16_t output_sta_2;//��8Ϊ������԰���״̬1�˶��� 0��λ,��8λΪĿ��״̬0:B 1:A, //A:01 B:00 ->b11 ->a10		
}TBDIO_UART;



//��λ��������ͨ�ŵ��������ṹ��
typedef struct __test_mode_mac_beep_proc{
	unsigned char code;//����ָ����
	unsigned char t_progress;//������,��λ���·�0Ϊ��ʼ���,��λ���ظ���100Ϊ�������/��Ϊ�������ļ���(ѭ��)
	unsigned char t_res;//�����,���궨�� TPC_ERR_XX
	unsigned char t_ret_value;//���صĸ�������,ֻ���ں���ȶ�·ͬʱ�����ʹ��,������ʾ�������ͨ����(bit0-bit7����ͨ��0-7)
								// ����ǶԵغ���,bit0-2Ϊ���˵ĸ�·����,bit3-5ΪԶ�˵ĸ�·����
								//����ǿ�����,��ײ�ź�:bit0:����ײ�쳣,bit1:����ײ�쳣
								//			   ����������: bit0-bit5�ֱ�Ϊ:Ĩ��/����ǽ/����/��������/������/������
	unsigned short ir_frt_value[10];//ǰײ����ֵ�ϴ�,��3���ǶԵغ����Զ��
	unsigned short bat_volt;//��ص�ѹ
	unsigned short bat_curr;//��صĳ�����
	unsigned short fw_ver;//��������汾��
	unsigned char bat_chstate;//��صĳ��״̬ 0Ϊδ��� 1ΪDC��׳�� 2Ϊ���׮���
	unsigned char chksum;//У��λ,����û�б�ʹ��
}TEST_BEEP_PROC;


typedef struct __st_tmirmaxmin
{
	uint16_t min,max;
}tm_irmaxmin;
extern tm_irmaxmin irmaxmin[7];
extern tm_irmaxmin irtestdata[7];
extern tm_irmaxmin asse_irmaxmin[7];
extern tm_irmaxmin asse_irtestdata[7];
#define VALVE_0 0
#define VALVE_1 1//��ǰ��ײ
#define VALVE_2 2//����ײ
#define VALVE_3 3//��ˢ,��ʱ��֪��զ�ã���·��
#define VALVE_4 4//�м���ײ
#define VALVE_5 5//�Ҳ���ײ
#define VALVE_6 6//��ǰ��ײ

#define VALVE_F_7 7//�����
#define VALVE_F_8 8//�ҵ���
#define VALVE_F_9 9//����ǽ �����������

#define MOTER_0 0//����رߺ�����
#define MOTER_1 1//��ͷ���⵲����
#define MOTER_2 2//�Ҳ��رߺ�����
#define MOTER_3 3
#define MOTER_4 4
#define MOTER_5 5//β�����̵��


#define ERR_LEFT_BUM	0
#define ERR_MLEFT_BUM	1
#define ERR_MID_BUM		2
#define ERR_MRIGHT_BUM	3
#define ERR_RIGHT_BUM	4







#define BIT_STA(x)         ((uint16_t)(1<<(x)))
#define BIT8_STA(x)         ((uint8_t)(1<<(x)))

/*
//��������԰��ͨ�Žṹ��
//���԰���Ҫ�������·���ָ����з���,���۶�ȡ��������,������Ҫ�������ú�Ľ��,��ȡ�������϶���Ҫ�����ˡ�����
//���԰�ظ�����ʱ,t_res_output��t_res_input�����ֵҲ��Ҫ������,��������ʡȥ�ٶ�һ�ε��鷳
typedef struct __test_board_proc{
	unsigned char code;//����ָ����
	unsigned char para;//���ƶ��� 0ֹͣ/1��ʼ(��ת(ǰ�����̵��))/2��ת(ǰ�����̵��)/3��ȡ����(���̻���/����LED���Ƚ���),����ֵ��t_res_output��t_res_input
						// ����ǲ��Թ���,1��ʾ���������Ե������ 0��ʾ��������Ͽ�
						//����ǲ��Կ�����(��©ģʽ),0��ʾ�ſ�,����ʱ�������������ʱӦΪ�ߵ�ƽ,1��ʾ��������,��ʱӦΪ�͵�ƽ
	unsigned short t_res_output;//�������ֵ,�����ǰ���������,�򷵻����ֵ,û�оͿճ�,����Ҫ��ģ�������ײ,�򷵻�ֵӦ��Ϊ0,�������ײ����0ʱ,�˴���GPIO���Ӧ��Ҳ�Ƿ���0,������ǲ��԰����
	unsigned short t_res_input;//�����ȡ����,�����LED�Ƿ���,��ˢ/��ˢ�ȵ��������ֵ(���30000,��ֹ�������)
	unsigned short nc;
	unsigned char para1;//���ƶ��� ���ڶ�·��������ͬʱ���,������λǰײ��ģ�⿪��,bit0��ʾ����ײ,1Ϊ�ӵ�,0Ϊ�ſ�,bit1��ʾ����ײ
						// ������ڶ�·����ʽ������������,bit0-bit5�ֱ�Ϊ:Ĩ��/����ǽ/����/��������/������/������
						// ��para == 1ʱ,��Щ���ص�ģ�ⴥ����Ч
						// ��para == 0ʱ,��Щ���ص�ģ�ⴥ��ȫ���ر�,para1��ֵ������
						
	unsigned char chksum;//У���,��ʱû��ʹ��
}TEST_BD_PROC;
*/
#if !USE_ON_COMPUTER

#if 0
//��⹦����,�ڼ������ഫ����ʱ,��Ҫ�Ѻ���LED�Ĺ���״̬һ�𷵻�
enum __tbd_proc{
	TBD_STOP_ALL = 0,//�˳����ģʽ
	TBD_START_ALL,//������ģʽ
	TBD_CD_FRT,//ǰ�����̲���
	TBD_MTOC_WHL_L,//���ֹ������
	TBD_MTOC_WHL_R,//���ֹ������
	TBD_MTOC_SMT_L,//���ˢ�������
	TBD_MTOC_SMT_R,//�ұ�ˢ�������
	TBD_MTOC_MT_MID,//��ɨ����������
	TBD_MTOC_MT_FAN,//����������,(remark:��������ﲻ���������)
	TBD_MTFALL_L,//�������ּ��
	TBD_MTFALL_R,//�������ּ��
	TBD_BUM_L,//��ǰײ���������
	TBD_BUM_R,//��ǰײ���������
	TBD_CHRG_TST,//��ʼ������,��Ҫ��ԭ���ĳ��ͨ���Ͽ�,ǰ���뵽����Ĺ���ͨ��,����24V����ͨ��ѡ���ź�
}
#else
//��⹦����,�ڼ������ഫ����ʱ,��Ҫ�Ѻ���LED�Ĺ���״̬һ�𷵻�
#define	TBD_STOP_ALL 	0//�˳����ģʽ
#define	TBD_START_ALL	1//������ģʽ
#define	TBD_CD_FRT	2//ǰ�����̲���
#define	TBD_MTOC_WHL_L	3//���ֹ������
#define	TBD_MTOC_WHL_R	4//���ֹ������
#define	TBD_MTOC_SMT_L	5//���ˢ�������
#define	TBD_MTOC_SMT_R	6//�ұ�ˢ�������
#define	TBD_MTOC_MT_MID	7//��ɨ����������
#define	TBD_MTOC_MT_FAN	8//����������,(remark:��������ﲻ������������Դ����)
#define	TBD_MTFALL_L	9//�������ּ��
#define	TBD_MTFALL_R	10//�������ּ��
#define	TBD_BUM_L	11//��ǰײ���������
#define	TBD_BUM_R	12//��ǰײ���������
#define	TBD_CHRG_TST	13//��ʼ������,��Ҫ��ԭ���ĳ��ͨ���Ͽ�,ǰ���뵽����Ĺ���ͨ��,����24V����ͨ��ѡ���ź�
						//para = 0 ������ֹͣ,�ѵ�ضϿ�
						//para = 1 ����ؽ�������,24V���뵽DC����
						//para = 2 ����ؽ�������,24V���뵽�������Ƭ
						//para = 3 ����ؽ�������,24Vδ����(���ѡ��һ�㲻�ᱻ�õ�)
						//���԰�˳�����Ҫ��output���ص�ؽ��뼰24V�Ľ���״��(bit0:���/bit1:24V���뵽DC��/bit2:24V���뵽�������Ƭ)
						//			����Ҫ���ص�صĴ�����Ϣ,������δ����(δ�ɵ�����¶ȱ���Ϊ���δ����)
						
#define TBD_SMT_L	14//���Ա�ˢ����Ƿ���ת,��ʱ���԰���Ҫ��ʼ��¼���ˢ�Ļ�����,ֱ���������ȡ
#define TBD_SMT_R	15//���Ա�ˢ����Ƿ���ת,��ʱ���԰���Ҫ��ʼ��¼�ұ�ˢ�Ļ�����,ֱ���������ȡ
#define TBD_MT_MID	16//������ɨ����Ƿ���ת,��ʱ���԰���Ҫ��ʼ��¼�ұ�ˢ�Ļ�����,ֱ���������ȡ
#define TBD_BUM_SW	17//������ײ����ģ��,�����к��к���LED�Ĺ�����,��t_res_input�з���,bit0Ϊ��,bit1Ϊ��,ģ����ײ�Ŀ������������output��
#define TBD_SWSENSOR	18//���������ʹ�������ģ������,�����к��к���LED�Ĺ�����,��t_res_input�з���
						//����������: bit0-bit5�ֱ�Ϊ:Ĩ��/����ǽ/����/��������/������/������
#define TBD_FAN		19		//��������ĵ������
#define TBD_KEY		20		//��������
#define TBD_SETSN	0xfb//д��SN��,����Ϊ20�ֽڵ��ַ���,����ת�����ܴ洢
#define TBD_GETSN	0xfc//����SN��
#endif

//��������԰��ͨ�Žṹ��
//���԰���Ҫ�������·���ָ����з���,���۶�ȡ��������,������Ҫ�������ú�Ľ��,��ȡ�������϶���Ҫ�����ˡ�����
//���԰�ظ�����ʱ,t_res_output��t_res_input�����ֵҲ��Ҫ������,��������ʡȥ�ٶ�һ�ε��鷳
typedef struct __test_board_proc{
	unsigned char code;//����ָ����
	unsigned char para;//���ƶ��� 0ֹͣ/1��ʼ(��ת(ǰ�����̵��))/2��ת(ǰ�����̵��)/3��ȡ����(���̻���/����LED���Ƚ���),����ֵ��t_res_output��t_res_input
						// ����ǲ��Թ���,1��ʾ���������Ե������ 0��ʾ��������Ͽ�
						//����ǲ��Կ�����(��©ģʽ),0��ʾ�ſ�,����ʱ�������������ʱӦΪ�ߵ�ƽ,1��ʾ��������,��ʱӦΪ�͵�ƽ
	unsigned short t_res_output;//�������ֵ,�����ǰ���������,�򷵻����ֵ,û�оͿճ�,����Ҫ��ģ�������ײ,�򷵻�ֵӦ��Ϊ0,�������ײ����0ʱ,�˴���GPIO���Ӧ��Ҳ�Ƿ���0,������ǲ��԰����
	unsigned short t_res_input;//�����ȡ����,�����LED�Ƿ���,��ˢ/��ˢ�ȵ��������ֵ(���30000,��ֹ�������)
	unsigned char para1;//���ƶ��� ���ڶ�·��������ͬʱ���,������λǰײ��ģ�⿪��,bit0��ʾ����ײ,1Ϊ�ӵ�,0Ϊ�ſ�,bit1��ʾ����ײ
						// ������ڶ�·����ʽ������������,bit0-bit5�ֱ�Ϊ:Ĩ��/����ǽ/����/��������/������/������
						// ��para == 1ʱ,��Щ���ص�ģ�ⴥ����Ч
						// ��para == 0ʱ,��Щ���ص�ģ�ⴥ��ȫ���ر�,para1��ֵ������
						
	unsigned char chksum;//У���,��ʱû��ʹ��
}TEST_BD_PROC;


#define SNLENGTH	21
typedef struct __test_sn_proc{
	unsigned char code;//����ָ����
	unsigned char sn[SNLENGTH];//20���ַ����ȵ�SN�룬�����ǿɼ��ַ�,�ڴ�������д������е�SN�ַ�Ϊ�ߵ�4λ���������֮���ٽ��з���,FLASH�д洢��SNҲ��������е�������ͬ,ֻ����Ҫ��ӡ��־��PC����ʾʱ�ٽ��л�ԭ
						//���һ���ַ���ʾ����Ĳ��Խ��,δ֪Ϊ0xff,0Ϊͨ��,����С��200��ֵ��ʾ���Բ�ͨ���Ĵ���
	unsigned char chksum;//У���,SN��д�����������Ҫ����У���룬�Է�д��
}TEST_SN_PROC;

//�����ϻ�����ʹ�õ��ϱ��ṹ��
typedef struct __test_burnin_proc{
	unsigned char code;//����ָ����
	unsigned char para;//��ǰ״̬,���ر߻������ر�
	unsigned short worktimer;//����ʱ��
	unsigned int bum_ct[4];//0-1���ر���ײ���� 0Ϊ��е��ײ 1Ϊ������ײ 2-3Ϊ���ر���ײ����,�Դ�����
	unsigned short cpmt_health;//������������״̬,�д������Ӧλ��1
						//bit15 ���ұ�ˢ���
						//bit14 ��ɨ���
						//bit13 ���
						//bit12 ���� 
						//bit11 ����ǽ
						//bit10 ǰײ����
						//bit9 �Եغ���
						//bit8 �Կ�
						//bit7 ���
						//bit6 wifiģ��
	unsigned short bat_volt;//��ص�ѹ
	unsigned short fw_ver;//ϵͳ����汾��
	unsigned char errorcode;//�������
	unsigned char chksum;//У���,��ʱû��ʹ��
}TEST_BURNIN_PROC;
#endif

#define TM_MODE_BOARD	0//���嵥������ģʽ
#define TM_MODE_MAC	1//��������ģʽ
extern unsigned char tm_mode;

extern void tmod_rx_uart(unsigned char com_dp, unsigned char chr);
extern void proc_bdtest_task(void);
extern void proc_mactest_task(void);

extern void proc_burn_in_init(void);
extern void proc_burn_in_task(void);

extern void sn_print(unsigned char * pdat,unsigned short len);
extern void walk_tesk_for_whele(void);

extern unsigned short tmod_fancd;
extern unsigned short bdtest_chgcurr;
extern void tx_sn(unsigned char code ,unsigned char *sn);
extern unsigned char get_chksum(unsigned char *pdata, unsigned short length);
extern int burnin_coordinate_calcu(uint8_t type);
#define BDTST_SET_CURR(C)	bdtest_chgcurr=C

uint16_t get_mactest_sensors(void);
void set_remote_flag(uint8_t set);
#endif

