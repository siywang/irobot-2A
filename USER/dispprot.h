#ifndef _DISPPORT_H_
#define _DISPPORT_H_
//����ͨ��Э��

//���ջ����С
#define DP_RXBUFFER_LEN	512

//��ֵ��
#define DP_KEY_PWR	TM_KEY_PWR//0x02
#define DP_KEY_MODE	TM_KEY_MODE//0x01
#define DP_KEY_PLAY	TM_KEY_PLAY//0x04


//#define PD_MSG_TIMER_SYNC		0x81
#define PD_MSG_WORK_STA		0x84
#define DP_MSG_BATY			0x85
#define DP_SYS_STA			0x83



#define ROBOT_STOP	0x00
#define ROBOT_WORK	0x01

#define FAN_MODE_LOW	0x01
#define FAN_MODE_MID	0x02
#define FAN_MODE_HIG	0x00
//���
//bit7=1Ϊ���巢�͵�����
//bit7=0Ϊ�Կط��͵�����

//�����¼�(�Կ�=>����)
#define DP_CMD_INVALID		0x00	//��Ч����
#define DP_CMD_KEY_POWER	0x01	//���ػ�����,����Ϊ1�ֽ�,0��ʾ�˼����º�Ϊ�ػ�,1��ʾ����
#define DP_CMD_KEY_MODE		0x02	//ģʽ�л�����,����Ϊ������ʱ�Ĺ���ģʽ,��ѡ��,��2�ֽ�,��MachineWorkMode&MachineWorkOptions
#define DP_CMD_KEY_PLAY		0x03	//ִ�л�ֹͣ����,����Ϊ3�ֽ�,byte0:0��ʾ�˼����º�Ϊֹͣ,1��ʾִ�е�ǰ��ģʽ
																//byte1:��ǰ�Ĺ���ģʽ,��MachineWorkMode
																//byte2:��ǰ�Ĺ���ģʽ,��MachineWorkOptions
#define DP_CMD_KEY_MANUAL	0x04	//�ֶ���ɨģʽ,����Ϊ1�ֽ�,byte0:0/1/2/3�ֱ��ʾǰ/��/��/��	
#define DP_CMD_KEY_SETTIME		0x06	//���õ�ǰʱ��
#define DP_CMD_KEY_SETSCHEDULE	0x07	//����ԤԼʱ��,�������������ʾ��,ԤԼ�����Ǵ����Կذ��,���÷���������
#define DP_CMD_VIOC			0x08	//����
#define DP_CMD_KEY_DIDI		0x09	//����������ȶ�һ��,����һЩ��ָ�������������,����û�����	
#define DP_CMD_KEY_STATUS		0x0A	//�Կ��·�����ͷ��������һЩ�����豸��״̬��Ϣ
#define DP_CMD_KEY_SELFTEST		0x0B	//���������Լ�����

#define DP_CMD_KEY_WIFI		0x0C		//WIFI����
#define DP_CMD_FAN_MODE		0x0D		//����ģʽ����

//�����¼�
#define DP_CMD_SET_TIME			0x81	//����ʱ��,����3�ֽ� 0-2�ֱ�Ϊ ʱ(24Сʱ��ʽ)/��/����(����Ϊλ�ĸ�ʽ,��bit0����һ,bit6������)
#define DP_CMD_SET_ORDERTIME	0x82	//����ԤԼʱ��,����3�ֽ�,����ͬDP_CMD_SET_TIME,����λʹ��:��bit2=1��ʾ������Ҫ��ɨ
#define DP_CMD_SET_STATUS		0x83	//���ͻ�����Ϣ(��������),����2�ֽ�,byte0: ��Ϣ������,��MachineStatus
																		// byte1: ���������,��1�ŵ�99,���MachineStatus��errorλΪ0,�˲���Ч
#define DP_CMD_SET_MODE			0x84	//����ģʽ,����2�ֽ�,��DP_CMD_KEY_MODE
#define DP_CMD_SET_BTRY			0x85	//���͵�ص���,����1�ֽ�,����Ϊ0-100,��λΪ%


#define DP_PROTHEAD_1	0x7e
#define DP_PROTHEAD_2	0X5d

#define DP_PROTEND_2	0X6d
#define DP_PROTEND_1	0x7d
//ͨ�����ݵ��ܳ���
#define DP_PROT_COMMLENGTH	8
#define DP_PROT_DATALENGTH	5

#if 1			///�Կ�Э��2.0
////DISP_CMD_REMOTE--------------------ң�������
#define REMOTE_CODE_INVALID		0xff//��Ч��
#define REMOTE_CODE_POWER		0x01//��Դ
#define REMOTE_CODE_UP			0x02//���ϼ�
#define REMOTE_CODE_LEFT		0x03//�����
#define REMOTE_CODE_RIGHT		0x05//���Ҽ�
#define REMOTE_CODE_DOWN		0x06//���¼�
//#define REMOTE_CODE_OK			0x30//OK
#define REMOTE_CODE_OK			0x04//OK
#define REMOTE_CODE_DOCK		0x07//�س�
#define REMOTE_CODE_SCHDL		0x08//ԤԼ
#define REMOTE_CODE_FULL_GO		0x09//������ɨ
#define REMOTE_CODE_FOCUS		0x0A//�ص���ɨ
#define REMOTE_CODE_TIM			0x0C//ʱ��
//#define REMOTE_CODE_FORCE		0x1B//ǿ��ģʽ
#define REMOTE_CODE_LIGHTS		0xb0//��Χ��
#define REMOTE_CODE_MUTE		0x2B//����
#define REMOTE_CODE_WIFI		0x40//WIFI
#define REMOTE_CODE_MODE		0x0B//ģʽѡ��
#define REMOTE_CODE_START		0x04//��ʼ/ֹͣ
////DISP_CMD_ALL_LED_LIGHT--------------------�Կ�led���
//�Կ�ָʾ�Ʊ��
#define LED_CONTRAL_NUB				12
#define DP_LED_ALL	0
#define DP_LED_1	1
#define DP_LED_2	2
#define DP_LED_3	3
#define DP_LED_4	4
#define DP_LED_5	5
#define DP_LED_6	6
#define DP_LED_7	7
#define DP_LED_8	8
#define DP_LED_9	9
#define DP_LED_10	10
#define DP_LED_11	11
//����ģʽ��״̬��Ӧָʾ��
#define STAT_LED_ALL		DP_LED_ALL
#define MODE_LED_SILENT		DP_LED_1
#define MODE_LED_NORMAL		DP_LED_2
#define MODE_LED_TURBO		DP_LED_3
#define MODE_LED_MANUAL		DP_LED_4
#define MODE_LED_FULL2GO	DP_LED_5
#define STAT_LED_BTYPRS		DP_LED_6
#define STAT_LED_ERROR		DP_LED_7
#define STAT_LED_WIFI		DP_LED_8
#define STAT_LED_DUSTFULL	DP_LED_9
#define STAT_LED_888		DP_LED_10
#define STAT_LED_NA			DP_LED_11
//ָʾ�ƶ�����
#define LED_OFF				0x00
#define LED_ON				0x01
#define LED_TW_0_5HZ		0x02
#define LED_TW_1HZ			0x03
#define LED_TW_2HZ			0x04
#define LED_TW_4HZ			0x05
#define LED_UNACTION		0x0F
#define LED_UNKNOW			0xFF
//ָʾ�ƶ���ʱ��
#define KEEP_TMR_0_S			0
#define KEEP_TMR_10_S			10
#define KEEP_TMR_120_S			120
#define KEEP_TMR_600_S			600
#define KEEP_TMR_30_MIN			1800
#define TMR_CHARGEIDLE_INTO_LOWPOWER	180000//60000	///180000

////888 ������ָʾ
#define DISP_ERR_UN		0x00
#define DISP_ERR_EN		0x01
//Э���������Ӧ���峤��
#define LEN_CMD_ALL_LED_LIGHT 		(LED_CONTRAL_NUB/2)	//�������еƵĿ������ݳ���
#define LEN_CMD_SINGLE_LED_LIGHT	4
#define LEN_CMD_ERROR_DIS			5
#define LEN_CMD_SET_TIME			4
#define LEN_CMD_PAIR_REMOTE			1

//Э��������
//	�·�<-  �ϱ�->  ˫��<->
#define DISP_CMD_REMOTE				0X01		//ң����			<-
#define DISP_CMD_ALL_LED_LIGHT		0X02		//���еƵĿ���	->	-o
#define DISP_CMD_SCHEDULE_CLEAN		0X03		//ԤԼ��ɨ�·�	<-	
#define DISP_CMD_SINGLE_LED_LIGHT	0X04		//�����ƿ���		->	-o
#define DISP_CMD_ERROR_DIS			0X05		//������ʾ		->	-o
#define DISP_CMD_SET_TIME			0X06		//����ʱ��		<->	-o
#define DISP_CMD_SET_SCHEDULE_TIME	0X07		//����ԤԼʱ��	<->
#define DISP_CMD_PAIR_REMOTE		0X08		//���ң����		<->
#define DISP_CMD_DPINFO				0x09	//����<->�Կ� 0Ϊ���Կ����ùػ�,�Կػ�����PA5���ػ�����ź�,0xffΪ������Կض�ȡ״̬,�˹���δ��
#endif



//ң�������״̬
#define	REMOTE_PAIR_NU		0	//�����ģʽ	->
#define	REMOTE_PAIR_ING 	1	//���ģʽ	<->
#define	REMOTE_PAIR_OK		2	//��Գɹ�	<-

typedef struct __sch_data{
	uint8_t week_en;//ʹ�õ�λΪbit0-6 bit0Ϊ���� bit1-bit6�ֱ��ʾ��һ������
	uint8_t hour[7];
	uint8_t min[7];
}T_SCH_DATA;


/*
union __data_union
{
	DP_PROTTIME timedata;
	DP_PROTWMODE workmode;
	DP_PROTWSTATUS morkstatus;
	DP_POWERSTATUS powerstatus;
	DP_BATTERY battery;
	DP_WORK	   work;
	DP_OPINT	opt;
	DP_TIME_T	time;
};
*/


typedef struct __dp_protdata{
	uint8_t head1;
	uint8_t head2;//��ͷ 0xaa,0x99
	uint8_t len;
	uint8_t chksum;//У���,Ϊ���ֽں�������ֽ�֮��ȡ��8λ
	uint8_t cmd;//����(�������ͷ����--0xaa 0x99)
	uint8_t data[20];
}DP_PROTDAT;

enum __dp_parseerrors{
	DP_PARSE_NOERR = 0,
	DP_PARSE_LENTH,//���ݳ��ȴ���
	DP_PARSE_CHKSUM,//У�����
	DP_PARSE_NODAT//����Ч����,��δ�ҵ���ͷ
};

extern void dp_sendkey(uint8_t keycode,uint8_t pwr, uint8_t workmode, uint8_t workoptions);
extern void dp_commrxtask(void);

extern uint8_t dp_weekbit2dec(uint8_t weekbit);
//extern void dp_tx_data(uint8_t msg,uint8_t option1,uint8_t option2,uint8_t option3,uint8_t option4);
void dp_tx_data(uint8_t msg,uint8_t pack_len,uint8_t *data);
void dp_led_light_contrl(uint8_t type,uint8_t sSta,uint8_t work_mode,uint8_t change_sta,uint8_t prs,uint16_t work_errcode,uint8_t wifi_state);
void dp_led_err_contrl(uint8_t type,uint8_t work_sta,uint16_t work_errcode);
extern uint8_t dp_camera_error;

void dp_send_schdata(void);
void dp_set_schdata(uint8_t wday,uint8_t hour,uint8_t minute,uint8_t powermode,uint8_t en);
void dp_get_schdata_conv2wifi(uint8_t *wifi_sch);
#endif
