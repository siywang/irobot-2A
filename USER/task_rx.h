
#ifndef _TASK_RX_H_
#define _TASK_RX_H_

#define PID_PEC	100.0f

#define BEGIN_IMAGE				0xA5
#define MSG_IMU_RX				0x01
#define MSG_IMU_SET_XY			0x02
#define MSG_IMU_SET_YAW			0x03
#define MSG_TURN_DEG			0x04
#define MSG_TURN_TO_DEG			0x05
#define MSG_WALK_DIST			0x06		//ֱ������
#define MSG_WORK				0x07		//��ʼ�������������������ʼ��
#define MSG_WALK_DEG			0x08		//�޸Ļ����н��еĽǶȣ����н�״̬��Ч
#define MSG_NEAR_WALL			0x09		//�ر�
#define MSG_ZGO_CLEAN			0x0A		//������ɨ
#define MSG_ARC_TURN			0x0B		//����ת��
#define MSG_GO_STOP				0x0C		//ֹͣ��·
#define MSG_NAVI_POINT			0x0D		//������·����
#define MSG_NEAR_WALK			0x0E		//�ر߻���ͼʱ��ֱ��
#define MSG_CLEAN_OK			0x0F		//��ɨ���
#define MSG_WALK_POINT			0x10		//��������
#define MSG_GO_BUM				0x11		//������ײ��Ϣ
#define MSG_ZMAP_OBST			0x12		//�·���ͼ�ϰ�������
#define MSG_DOOR				0x13		//��������Ϣ
#define MSG_WALK_PATH			0x14		//������·��
#define MSG_ROTA_WALK_PATH		0x15		//��ת·��������
#define MSG_NAVI_BORAD_INIT_OK	0x16		//������ϵ�������ʼ��OK
#define MSG_VIRTUAL_WALL		0x17		//��������ǽ
#define MSG_EMPTY_CLEAN_ZMAP	0x18		//���������ɨ�ĵ�ͼ��·��
#define MSG_ZGO_STOP			0x1A		//������ɨֹͣ
#define MSG_IMU_SET_XY_OFS		0x1B		//����XYƫ���� XY_OFS
#define MSG_HEARTBEAT			0x1D		//������������
#define MSG_GO_DIS_BUM			0x1E		//����ǰ�����پ�����ײ
#define MSG_APP_CMD				0x1C		//APP���� ͸����Ϣ
#define MSG_WIFI_STA			0x1F		//WIFIģ��״̬
#define MSG_APP_NAVI_POINT		0x20		//ָ��ɨ(ȥ)��
#define MSG_APP_AREA_CLEAN		0x21		//������ɨ		������ɨģʽ
#define MSG_APP_FORBIDDEN_AREA	0x22		//��������
#define MSG_APP_VIRTUAL_WALL	0x23		//����ǽ����
#define MSG_APP_CONF_INFO		0x24		//��ȡ��������
#define MSG_VW_BUM				0x25		//����ǽ��ײ��Ϣ
#define MSG_ROOM_AREA_POINT		0x26		//���ַ���������ɨ��Χ��,����������ɨģʽ
#define MSG_LIDAR_DIST   		0x27  		//�����״������
#define MSG_FIRMWARE_UPGRADE	0x30		//�̼�����
#define MSG_IMU_SET_LEAVE_DIS	0x40
#define MSG_WALK_OK				0x50		//ֱ�ж������
#define MSG_TURN_OK				0x51		//��תOK
#define MSG_ROBOT_TURN   		0x52		// 1�������ڹ�����ɨ,ͣ�����������״̬,ɨ��ͼ
#define WALK_RES_OK				0x00		//ֱ����ɣ�û����ײ
#define WALK_RES_BUM			0x01		//ֱ����ײ��
#define WALK_RES_IR				0x02		//ֱ�к�����ײ��
#define WALK_RES_STOP			0x03		//���⵼��Ҫ��ֹͣ
#define INFO_STOP				0x00
#define INFO_WALK				0x01
#define INFO_TURN				0x02
#define INFO_NAVI_OK			0x03		//�������
#define INFO_NEAR				0x04		//�ӱ�
#define INFO_NAVI_POINT			0x05		//����Ŀ���
#define INFO_NAVI_BOARD_START_WORK		0x06		//�������������忪ʼ���� ����/app��ʼ������ɨ
#define	INFO_HALT				0x07		//��ͣ
#define	INFO_OVER				0x08		//��ɨ���
#define	INFO_HANG				0x09		//����������
#define	INFO_CLEAN_CONDITION	0x0A		//��ɨ�����Ϣ
#define	INFO_CONSUMABLES		0x0B		//�Ĳ���Ϣ
#define	INFO_ALARM				0x0C		//�澯��Ϣ
#define INFO_NAVI_BUM			0x0D		//������ײ
#define INFO_DOOR_NAVI			0x0E		//���������Ǳ�ȥ������ɨ��ͼ
#define INFO_ZGO_STA			0x0F		//��ʼ�µ���ɨ,Ҫ����λ��ɨ��ͼ
#define INFO_WIFI_CONFIG		0x10		//wywy ����ģʽ�ϱ�
#define INFO_DOCK_POINT			0x11		//���׮����
#define INFO_DOCK_STA			0x12		//���س����ģʽ
#define INFO_LINE_WALK			0x13		//����ֱ������
#define INFO_NY3P_PLAY 			0x14		//��������
#define INFO_IMU_VER   			0x15		//IMU�汾
#define EAI_DIST_OFFSET	8//���������������е�ƫ����
#define EAI_DIST_BUFFERLENTH	64//0x40(&0x3f)
#define EAI_DIST_ANDHEX	0x3f//ѭ����������
struct imu_rx_data_t
{
	uint8_t		begin;
	uint8_t		crc;
	uint8_t		msg;		//������
	uint8_t		sta;
	uint8_t		ret;		//ֱ�еķ���
	uint8_t		sSta;		//ϵͳ״̬
	uint8_t		r;			//Ԥ��
	uint8_t		mSta;		//�˶�״̬clean_sta 		��ɨ״̬
	int16_t		x;
	int16_t		y;
	float		yaw;
};

typedef struct h_ctrl_t
{
	//uint8_t begin[2];
	uint8_t msg;
	uint8_t crc;
	uint16_t pwm;
	uint16_t hw;
	uint16_t speed;
}ctrl_t;

typedef struct h_pid_set_t
{
	uint8_t 	msg;
	uint8_t 	crc;
	uint16_t	kp1;
	uint16_t	ki1;
	uint16_t 	kd1;
	uint16_t	kp2;
	uint16_t	ki2;
	uint16_t 	kd2;	
}pid_set_t;
typedef struct h_rx_cfg_t
{
	uint8_t 	msg;
	uint8_t 	crc;
	uint16_t	cfg[40];	
}rx_cfg_t;
void rx_usart(uint8_t chr);

void tx_msg(uint8_t *buff,int len);
void test_seft_task(uint8_t type);

#endif
