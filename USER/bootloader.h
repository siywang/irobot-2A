#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include "stm32f10x_flash.h"

#define FLASH_ADDRESS_START		0x08000000
#define FLASH_ADDRESS_BACKUP 	0x08040000

#define FLASH_PAGE 	0x800
#define FIFO_NUM		3

#define NACK_FREQ		300		//3s / 10ms
#define RECV_FREQ		10		//0.1s / 10ms
#define RECV_TIMEOUT		50		//0.5s / 10ms

#define XMODEM_SOH		0X01
#define XMODEM_STX		0X02
#define XMODEM_NACK		0X15
#define XMODEM_ACK		0X06
#define XMODEM_EOT		0X04
#define XMODEM_EOF		0X1A

#define STAGE_READY		0		//׼����У��ͷ�ʽ
#define STAGE_DOWNLOAD	1		//�������ݵ�����flash
#define STAGE_PROGRAM	2		//�������ݵ�������

//��ͷ
#define SIZE_HEAD		3
//У���
#define SIZE_CRC			1
//���ֳ��ȸ�ʽ
#define SIZE_128_B		128
#define SIZE_1K_B		1024

#define BYTES_PER_FRAME		SIZE_1K_B
#define FRAME_NUM_PER_PAGE	(FLASH_PAGE / BYTES_PER_FRAME)

//����֡���ܻ��泤��
#define FRAME_SIZE		(SIZE_HEAD + SIZE_1K_B + SIZE_CRC)

typedef struct
{
	unsigned char frame_buffer[FRAME_SIZE*2];
	unsigned char flash_fifo_buff[FIFO_NUM][FLASH_PAGE];
}
UPGRADE_BUFF_T;

//����ϵͳTIM��CNT,���ó�10ms,����Ҫ�ж�
extern volatile unsigned short *timer_sync;

//�����Ĵ���ͨ��

extern void xmodem_default(void);						//��������ģʽʱ�ĳ�ʼ��
extern void upgrade_self_task(void);						//�������̣߳��ڲ�ѭ������
extern void befor_usart_init(FlagStatus intelchannel);
extern void dma_recevie_from_usart(void);
#endif

