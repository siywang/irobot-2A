#include "sys.h"

volatile unsigned short *timer_sync = NULL;

extern SHARE_RAM_U share_ram_buff;

static FlagStatus frame_recv_finish = RESET;
////static unsigned char frame_buffer[FRAME_SIZE*2] = {0xFF};
static unsigned short frame_type = 0;
static unsigned char upgrade_stage = 0;
////static unsigned char flash_fifo_buff[FIFO_NUM][FLASH_PAGE] = {0};
static unsigned char fi_num = 0;
static unsigned char fo_num = 0;
static unsigned short program_data_page = 0;
static FlagStatus intel_usart = RESET;

void xmodem_default(void)
{
	upgrade_stage = 0;
	fi_num = 0;
	fo_num = 0;
	program_data_page = 0;
	memset(&(share_ram_buff.upgrade_buffer.flash_fifo_buff[0][0]), 0xFF, (FIFO_NUM * FLASH_PAGE));
	memset(share_ram_buff.upgrade_buffer.frame_buffer, 0, (FRAME_SIZE * 2));
	
	////GPIO_SetBits(LED_POWER_TWINKLE1_PORT, LED_POWER_TWINKLE1_PIN);
	////GPIO_SetBits(LED_DOCK_TWINKLE1_PORT, LED_DOCK_TWINKLE1_PIN);
}

typedef union
{
	unsigned char uc[2];
	unsigned short us;
}FLASH_UNIT_U;

typedef union
{
	unsigned char uc[4];
	unsigned int ui;
}FLASH_CRC_U;

void befor_usart_init(FlagStatus intelchannel)
{
	intel_usart = intelchannel;

	if(SET == intel_usart)
	{
		//��DMA��ʽ �����ж�
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		USART_Cmd(USART2, DISABLE);

		DMA_InitTypeDef DMA_InitStructure;

		/* DMA1 Channel6 (triggered by USART1 Rx event) Config */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 ,  ENABLE);

		/* DMA1 Channel5 (triggered by USART1 Rx event) Config */
		DMA_DeInit(DMA1_Channel6);
		DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&USART2->DR;// ��ʼ�������ַ���൱�ڡ��ļҿ�ݡ�  
		DMA_InitStructure.DMA_MemoryBaseAddr =(u32)share_ram_buff.upgrade_buffer.frame_buffer;// �ڴ��ַ���൱�ڼ��Ź�
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ������Դ����Ϊ�տ��
		DMA_InitStructure.DMA_BufferSize = FRAME_SIZE ;// ���������������Ӵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ�������������Ӷ�Ӧ�Ŀ�ݲ���
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// �ڴ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�����ֽڿ�ȣ��������������С�������������㣬���ǰ�����㣩 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ��ֽڿ�ȣ���������װ��ݵĶ���(�����������ǰ����ʽ��з�װ)
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ����ģʽ�������˾Ͳ��ڽ����ˣ�������ѭ���洢
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;// ���ȼ��ܸߣ���Ӧ��ݾ��ǼӼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // �ڴ�������ͨ�ţ������ڴ浽�ڴ� 
		DMA_Init(DMA1_Channel6, &DMA_InitStructure);// �Ѳ�����ʼ������������ݹ�˾��Э��

		DMA_Cmd(DMA1_Channel6, ENABLE);// ����DMA�������ݹ�˾ǩ����ͬ����ʽ��Ч

		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);

		//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);  //ʹ�ܿ����ж�
		USART_Cmd(USART2, ENABLE);
	}
	else
	{
		//��DMA��ʽ �����ж�
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

		USART_Cmd(USART1, DISABLE);

		DMA_InitTypeDef DMA_InitStructure;

		/* DMA1 Channel6 (triggered by USART1 Rx event) Config */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 ,  ENABLE);

		/* DMA1 Channel5 (triggered by USART1 Rx event) Config */
		DMA_DeInit(DMA1_Channel5);
		DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&USART1->DR;// ��ʼ�������ַ���൱�ڡ��ļҿ�ݡ�  
		DMA_InitStructure.DMA_MemoryBaseAddr =(u32)share_ram_buff.upgrade_buffer.frame_buffer;// �ڴ��ַ���൱�ڼ��Ź�
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ������Դ����Ϊ�տ��
		DMA_InitStructure.DMA_BufferSize = FRAME_SIZE ;// ���������������Ӵ�С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ�������������Ӷ�Ӧ�Ŀ�ݲ���
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// �ڴ����
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�����ֽڿ�ȣ��������������С�������������㣬���ǰ�����㣩 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ��ֽڿ�ȣ���������װ��ݵĶ���(�����������ǰ����ʽ��з�װ)
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ����ģʽ�������˾Ͳ��ڽ����ˣ�������ѭ���洢
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;// ���ȼ��ܸߣ���Ӧ��ݾ��ǼӼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // �ڴ�������ͨ�ţ������ڴ浽�ڴ� 
		DMA_Init(DMA1_Channel5, &DMA_InitStructure);// �Ѳ�����ʼ������������ݹ�˾��Э��

		DMA_Cmd(DMA1_Channel5, ENABLE);// ����DMA�������ݹ�˾ǩ����ͬ����ʽ��Ч

		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

		//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);  //ʹ�ܿ����ж�
		USART_Cmd(USART1, ENABLE);
	}
}

#pragma arm section code = "RAMCODE"
void dma_recevie_from_usart(void)
{
	static FlagStatus frame_head_ok = RESET;
	DMA_Channel_TypeDef * usart_dma = (SET == intel_usart) ? (DMA1_Channel6) : (DMA1_Channel5);	
	uint32_t rx_num = FRAME_SIZE - (uint16_t)(usart_dma->CNDTR);////DMA_GetCurrDataCounter(usart_dma);	

	if(RESET == frame_recv_finish)
	{
		if(0 < rx_num)
		{		
			if(RESET == frame_head_ok)
			{
				if(XMODEM_SOH == share_ram_buff.upgrade_buffer.frame_buffer[0])			//128���Ȱ�
				{
					frame_type = SIZE_128_B;
					frame_head_ok = SET;
				}
				else if(XMODEM_STX == share_ram_buff.upgrade_buffer.frame_buffer[0])		//1k���Ȱ�
				{
					frame_type = SIZE_1K_B;
					frame_head_ok = SET;
				}
				else if(XMODEM_EOT == share_ram_buff.upgrade_buffer.frame_buffer[0])		//������
				{
					frame_recv_finish = SET;
					return;
				}
			}
		}

		if((SET == frame_head_ok) && ((frame_type + SIZE_HEAD + SIZE_CRC) == rx_num))
		{
			frame_recv_finish = SET;
			frame_head_ok = RESET;
			usart_dma->CCR &= (uint16_t)(~((uint16_t)0x0001));////DMA_Cmd(usart_dma,DISABLE);  		
			usart_dma->CNDTR = FRAME_SIZE;////DMA_SetCurrDataCounter(usart_dma,FRAME_SIZE);  		
			usart_dma->CCR |= ((uint16_t)0x0001);////DMA_Cmd(usart_dma,ENABLE);  		
		}
	}
}

//д����flash�ӿں���
void program_backup_page(unsigned char *scr, unsigned int page_address)
{
	FLASH_UNIT_U u16_byte;
	unsigned short u16_num = 0;

	//�����ж�
	{__ASM  volatile ("cpsid i");};

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ

	//flash����
	FLASH_ErasePage(page_address);

	//flash����	
	for(u16_num = 0; u16_num < 1024; u16_num++)
	{
		u16_byte.uc[0] = *(scr + (2 * u16_num));
		u16_byte.uc[1] = *(scr + (2 * u16_num) + 1);
		FLASH_ProgramHalfWord(page_address + 2 * u16_num, u16_byte.us);
	}

	//flash����
	FLASH_Lock();

	//�����ж�
	{__ASM  volatile ("cpsie i");};
}

//�Ӵ����������ݵ�����flash
void usart_to_backup_process(void)
{
	static unsigned char cur_wr_page = 0;

	if(fo_num != fi_num)
	{
		//����
		program_backup_page(&share_ram_buff.upgrade_buffer.flash_fifo_buff[fo_num][0], FLASH_ADDRESS_BACKUP + (FLASH_PAGE * cur_wr_page++));

		//�建��
		unsigned short clear_size = 0;
		do
		{
			share_ram_buff.upgrade_buffer.flash_fifo_buff[fo_num][clear_size] = 0xFF;
		}
		while(FLASH_PAGE > ++clear_size);

		//������и���
		if(FIFO_NUM <= ++fo_num)
		{
			fo_num = 0;
		}
		program_data_page++;
	}
}

//���ݰ����
FlagStatus check_frame_format(unsigned char *buff)
{
	static unsigned char last_one = 0;
	static unsigned short wr_offset = 0;
	static unsigned char frame_num = 0;
	
	FlagStatus head_correct = RESET;
	FLASH_CRC_U frame_crc;
	unsigned char cmd_head = 0;

	frame_crc.ui = 0;

	//EOT ����SOH STX
	if(0 == cmd_head)
	{
		if((XMODEM_SOH == *buff) || (XMODEM_STX == *buff))
		{
			cmd_head = 1;
		}
		else if(XMODEM_EOT == *buff)
		{
			//�����ˣ�Ҫ����һ�����ݰ����ȥ���
			//ɨβ��û����һ��ҳ�棬ҲҪ��
			if(0  != (frame_num % FRAME_NUM_PER_PAGE))
			{
				//����һ��ҳ��
				last_one = fi_num;
				if(FIFO_NUM <= ++fi_num)
				{
					fi_num = 0;
				}
			}

			//�����
			unsigned short first_null = FLASH_PAGE - 1;
			do
			{
				if(XMODEM_EOF == share_ram_buff.upgrade_buffer.flash_fifo_buff[last_one][first_null])
				{
					share_ram_buff.upgrade_buffer.flash_fifo_buff[last_one][first_null] = 0xFF;
				}
				else if(0xFF != share_ram_buff.upgrade_buffer.flash_fifo_buff[last_one][first_null])
				{
					break;
				}
			}
			while(first_null--);

			fo_num = last_one;
			usart_to_backup_process();

			upgrade_stage = STAGE_PROGRAM;
			//flash����
			////FLASH_Lock();
			////FLASH_ReadOutProtection(ENABLE);

			return SET;
		}
	}

	//�����
	if((1 == cmd_head) && ((frame_num + 1) == *(buff + 1)))
	{
		cmd_head = 2;
	}

	//����ŷ���
	unsigned char fanma = ~(frame_num + 1);
	if((2 == cmd_head) && (fanma == *(buff + 2)))
	{
		head_correct = SET;
	}
	
	if(SET == head_correct)
	{
		if(STAGE_READY== upgrade_stage)
		{
			//flash����
			////FLASH_Unlock();
			////FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ

			upgrade_stage = STAGE_DOWNLOAD;
		}

		//wr_offset = (frame_num % FRAME_NUM_PER_PAGE)?(0):(SIZE_1K_B);
		if(0 == frame_num % FRAME_NUM_PER_PAGE)
		{
			wr_offset = 0;
		}
		else
		{			
			wr_offset += SIZE_1K_B;
		}

		for(unsigned short wr_num = 0; wr_num < frame_type; wr_num++)
		{
			share_ram_buff.upgrade_buffer.flash_fifo_buff[fi_num][wr_offset + wr_num] = *(buff + 3 + wr_num);
			frame_crc.ui += *(buff + SIZE_HEAD + wr_num);
		}

		//�˶�У���
		if(frame_crc.uc[0] != *(buff + SIZE_HEAD + frame_type))
		{
			return RESET;
		}

		//У�����ȷ ����ż�1
		frame_num++;
	
		//2k������������
		if(0 == (frame_num % FRAME_NUM_PER_PAGE))
		{
			//����һ��ҳ��
			last_one = fi_num;
			if(FIFO_NUM <= ++fi_num)
			{
				fi_num = 0;
			}
			usart_to_backup_process();
		}
		
		return SET;
	}
	else
	{
		return RESET;
	}
}

//�ϱ�ACK����NACK
void report_process(FlagStatus result)
{
	USART_TypeDef *usart_ch = (SET == intel_usart) ? (USART2) : (USART1);

	if(SET == result)
	{
		usart_ch->DR = XMODEM_ACK;
	}
	else
	{
		usart_ch->DR = XMODEM_NACK;
	}
}

//����flash����������flash
void backup_to_code_process(unsigned short page_len)
{
	unsigned int address_scr = FLASH_ADDRESS_BACKUP;
	unsigned int address_dst = FLASH_ADDRESS_START;
	unsigned long code_byte = 0;

	//�����ж�
	{__ASM  volatile ("cpsid i");};

	//flash����
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ

	//flash����
	for(unsigned short page_num = 0; page_num < page_len; page_num++)
	{
		address_scr = FLASH_ADDRESS_BACKUP + (page_num * FLASH_PAGE);
		address_dst = FLASH_ADDRESS_START + (page_num * FLASH_PAGE);

		//����flash����
		FLASH_ErasePage(address_dst);

		for(unsigned short code_num = 0; code_num < 512; code_num++)
		{			
			code_byte = *((unsigned long *)(address_scr +  (4 * code_num)));
			FLASH_ProgramWord((address_dst + (4 * code_num)), code_byte);
		}

		//����flash����
		FLASH_ErasePage(address_scr);
	}

	//flash����
	FLASH_Lock();

	//�����ж�
	{__ASM  volatile ("cpsie i");};

	//��λ
	////ALL_POWER_OUT_PORT->ODR &= ~ ALL_POWER_OUT_PIN;
	 __set_FAULTMASK(1);
	 SCB->AIRCR = 0x05FA0000 | (u32)0x04;
	 while(1);
}

//���������߳�
void upgrade_self_task(void)
{
	USART_TypeDef * usart_chl = (SET == intel_usart) ? (USART2) : (USART1);
	static unsigned int tmrtmr = 0;
	*timer_sync = 0;
	
	while(1)
	{
		//10ms���һ���յ��İ�
		if(10000 <= *timer_sync)
		{
			*timer_sync = 0;
			tmrtmr++;
			dma_recevie_from_usart();		//DMA��ʽ
		}

		switch(upgrade_stage)
		{
			//ȷ��У��ͷ�ʽ
			case STAGE_READY:
				if(NACK_FREQ < tmrtmr)
				{
					tmrtmr = 0;
					if(SET == frame_recv_finish)
					{
						report_process(check_frame_format(share_ram_buff.upgrade_buffer.frame_buffer));
						frame_recv_finish = RESET;
					}
					else
					{
						usart_chl->DR = XMODEM_NACK;
					}
				}

			break;
			//д���յ����ݵ�����flash
			case STAGE_DOWNLOAD:
				if(RECV_FREQ < tmrtmr)
				{
					tmrtmr = 0;
					if(SET == frame_recv_finish)
					{
						report_process(check_frame_format(share_ram_buff.upgrade_buffer.frame_buffer));
						frame_recv_finish = RESET;
					}					
				}
			break;
			//�ѱ���flash����������flash
			case STAGE_PROGRAM:
				////ALL_POWER_OUT_PORT->ODR |= ALL_POWER_OUT_PIN;
				upgrade_stage++;
				backup_to_code_process(program_data_page);				
			break;

			default:

			break;
		}

	}
}
#pragma arm section

