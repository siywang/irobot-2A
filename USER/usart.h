
#ifndef USART_H
#define USART_H

#define LOG_PRINTF    1 //0�رմ�ӡ 1 log_printf 2 ����ϵͳprintf,��ʹ��ram����

//#define SYS_DEBUG 1

typedef struct __r1371t_dat{
	uint8_t head2;//0xAA������ֽ�(��Զ����0x00)
	uint8_t index;//����������
	int16_t angle;//��ǰ�Ƕ�
	int16_t rate;//��ǰ���ٶ�
	int16_t accx;//���ٶ�x
	int16_t accy;//���ٶ�y
	int16_t accz;//���ٶ�z
	uint8_t resv;//δʹ��
	uint8_t chsum;//У���
}R1371T_FMT;

extern R1371T_FMT r1371t_dat;




typedef char OUT[64];



//#define UART2RXBUFFMAX_SIZE  512  //����������
#define UART1RXBUFFMAX_SIZE  64  //����������

#define UART3RXBUFFMAX_SIZE  64 
#define UART4RXBUFFMAX_SIZE	 64
#define UART5RXBUFFMAX_SIZE	 64

//extern uint8_t rxBuf[UART1RXBUFFMAX_SIZE];

extern int u1_len;

extern uint8_t g_buf[32];
extern int g_len;
		
void usart1_init(void);


#if LOG_PRINTF == 2
#define log_printf printf
#elif LOG_PRINTF == 1
void log_printf(const char *format,...);
#endif

extern char read_pb(uint8_t *chr);
extern void psend(uint8_t chr);
extern int read_uart1(uint8_t *str);
extern void proc_uart_task(void);
extern void usart_write(uint8_t *buff,int len);

extern void usart2_init(void) ;
extern void usart2_write(uint8_t *str,int len);
extern int usart2_read(uint8_t *data,int len);

extern void usart4_init(void);
extern int usart4_read(uint8_t *data,int len);
extern void usart4_write(uint8_t *str,int len);

extern void usart_init_info(void);

extern void usart3_init(void);
extern void usart3_write(uint8_t *str,int len);
extern void put_usart3(uint8_t ch);
#endif

