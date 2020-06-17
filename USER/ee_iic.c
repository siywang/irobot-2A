#include "sys.h"

enum ENUM_TWI_REPLY
{
	TWI_NACK=0,
	TWI_ACK=1,
};
enum ENUM_TWI_BUS_STATE
{
	TWI_READY=0,
	TWI_BUS_BUSY=1,
	TWI_BUS_ERROR=2,
};

void I2C_EE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	// Configure I2C1 pins: SCL and SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void TWI_delay(void)
{
	uint8_t i=10; //i=10��ʱ1.5us//��������Ż��ٶ� ����������͵�5����д��
	while(i--);
}

/**************************************************************************
��ʱms����ʱ�ĺ�����CYCLECOUNTER / 72000000
***************************************************************************/
void DelayMs(uint16_t ms)
{
	uint16_t iq0;
	uint16_t iq1;
	for(iq0 = ms; iq0 > 0; iq0--)
	{
		for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000
	}
}

uint8_t TWI_Start(void)
{
	EE_SDAH;
	EE_SCLH;
	TWI_delay();
	if(!EE_SDAread)
		return TWI_BUS_BUSY; //SDA��Ϊ�͵�ƽ������æ,�˳�?
	EE_SDAL;
	TWI_delay();
	if(EE_SDAread) 
		return TWI_BUS_ERROR; //SDA��Ϊ�ߵ�ƽ�����߳���,�˳�?
	EE_SCLL;
	TWI_delay();
	return TWI_READY;
}

void TWI_Stop(void)
{
	EE_SDAL;
	EE_SCLL;
	TWI_delay();
	EE_SCLH;
	TWI_delay();
	EE_SDAH;
	TWI_delay();
}

void TWI_Ack(void)
{
	EE_SCLL;
	TWI_delay();
	EE_SDAL;
	TWI_delay();
	EE_SCLH;
	TWI_delay();
	EE_SCLL;
	TWI_delay();
}

void TWI_NoAck(void)
{
	EE_SCLL;
	TWI_delay();
	EE_SDAH;
	TWI_delay();
	EE_SCLH;
	TWI_delay();
	EE_SCLL;
	TWI_delay();
}

uint8_t TWI_WaitAck(void) //����Ϊ:=1��ACK,=0��ACK
{
	EE_SCLL;
	TWI_delay();
	EE_SDAH;
	TWI_delay();
	EE_SCLH;
	TWI_delay();
	if(EE_SDAread)
	{
		EE_SCLL;
		return 0;
	}
	EE_SCLL;
	return 1;
}
void TWI_SendByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
	uint8_t i=8;
	while(i--)
	{
		EE_SCLL;
		TWI_delay();
		if(SendByte&0x80)
			EE_SDAH;
		else
			EE_SDAL;
		SendByte<<=1;
		TWI_delay();
		EE_SCLH;
		TWI_delay();
	}
	EE_SCLL;
}

uint8_t TWI_ReceiveByte(void) //���ݴӸ�λ����λ//
{
	uint8_t i=8;
	uint8_t ReceiveByte=0;
	EE_SDAH;
	while(i--)
	{
		ReceiveByte <<= 1;
		EE_SCLL;
		TWI_delay();
		EE_SCLH;
		TWI_delay();
		if(EE_SDAread)
		{
			ReceiveByte |= 0x01;
		}
	}
	EE_SCLL;
	return ReceiveByte;
}

//���أ�3д��ɹ���0д������ַ����1����æ��2����
//д��1�ֽ�����SendByte����д������WriteAddress����д���ַ
uint8_t TWI_WriteByte(uint8_t SendByte, uint8_t WriteAddress)
{
	uint8_t i;
//	uint16_t j;

	i = TWI_Start();
	if(i)
		return i;
	TWI_SendByte( ADDR_24CXX & 0xFE);
	//д������ַ ?д�룺��ַ���λ��0����ȡ����ַ���λ��1
	if(!TWI_WaitAck())
	{
		TWI_Stop();
		return 0;
	}
	TWI_SendByte(WriteAddress);	//������ʼ��ַ
	TWI_WaitAck();
	TWI_SendByte(SendByte);	//д����
	TWI_WaitAck();
	TWI_Stop();	
	//ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
	DelayMs(12);	//д����ʱ 12ms? д���ڴ���10ms����
	return 3;
}

//���أ�0д������ַ����1����æ��2����,
//����1�ֽ�����
//ReadAddress����������ַ
uint8_t TWI_ReadByte( uint8_t ReadAddress)
{
uint8_t i,temp;

i = TWI_Start();

if(i)
	return i;
TWI_SendByte((ADDR_24CXX & 0xFE));
//д������ַ����ִ��һ��αд����
if(!TWI_WaitAck())
{
	TWI_Stop();
	return 0;
}
TWI_SendByte(ReadAddress);//������ʼ��ַ
TWI_WaitAck();
TWI_Start();
TWI_SendByte((ADDR_24CXX & 0xFE)|0x01);//��������ַд�룺��ַ���λ��0����ȡ����ַ���λ��1
TWI_WaitAck();//*pDat = TWI_ReceiveByte();
temp = TWI_ReceiveByte();
TWI_NoAck();
TWI_Stop();
return temp;//���ص������0��1��2������������ͬ�ˣ��ٿ���һ��
}


/***************************************************************************
��24c256��д����ֽ�psrc_data��ָ��Ҫд�����������ָ��adr��24c256��Ҫд�����ݵ��׵�ַnbyte��д����ֽ�������ֵ:? 0��ִ����ϣ�1��ִ�г��ִ����β��У�C02ֻ��һ����ַadr��C256���и�λ��ַhadr�͵�λ��ַladr
***************************************************************************/
uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data,uint8_t adr,uint8_t nbyte)
{
uint8_t i;
for(;nbyte!=0;nbyte--)
{
i = TWI_Start();

if(i)
return i;
TWI_SendByte( ADDR_24CXX & 0xFE);//д������ַ
if(!TWI_WaitAck())
{
TWI_Stop();
return 0;
}
TWI_SendByte(adr);	//������ʼ��ַ
TWI_WaitAck();
TWI_SendByte(*psrc_data);		//д����
TWI_WaitAck();
psrc_data++;//ָ���д���ݵ�ָ���1
adr++;//��24C08�Ĳ�����ַ��1
TWI_Stop();//ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
DelayMs(12);//д����ʱ 12ms? д���ڴ���10ms����
}
return 0;
}

/***************************************************************************
��24c02������ֽ�pdin_data��ָ��Ҫ����������ݵ������ָ��
adr��24c02��Ҫ�������ݵ��׵�ַ
nbyte���������ֽ�������ֵ:? 0��ִ����ϣ�1��ִ�г��ִ���
***************************************************************************/
uint8_t I2C_EE_BufferRead(uint8_t *pdin_data,uint8_t adr,uint8_t nbyte)
{
	uint8_t i;
	i = TWI_Start();
	if(i)
		return i;

	TWI_SendByte((ADDR_24CXX & 0xFE));//д������ַ����ִ��һ��αд����
	if(!TWI_WaitAck())
	{
		TWI_Stop();
		return 0;
	}
	TWI_SendByte(adr);	//������ʼ��ַ
	TWI_WaitAck();
	TWI_Start();
	TWI_SendByte((ADDR_24CXX & 0xFE)|0x01); //��������ַд�룺��ַ���λ��0����ȡ����ַ���λ��1
	TWI_WaitAck();
	while(nbyte!=1)//����ǰ(nbyte-1)���ֽ�
	{
		*pdin_data = TWI_ReceiveByte();//ѭ����24C02�ж����ݣ�����pdin_data��ָ�Ĵ洢����
		TWI_Ack();//IICӦ��
		pdin_data++;//ָ��洢�������ݵĴ洢��ָ���1
		nbyte--;//ʣ��Ҫ������ֽڼ�1
	};
	*pdin_data = TWI_ReceiveByte();//�������һ���ֽ�
	TWI_NoAck();//IIC��Ӧ�����
	TWI_Stop();
	return 0;
 }

 /*
 void TWI_24CXX_Write(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
 {
	 uint16_t i;
	 for(i=0;i<nLen;i++)
	 {
	 	TWI_WriteByte(*(pDat+i), nAddr+i);
	 }
 }

 void TWI_24CXX_Read(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
 {
	 uint16_t i;
	 for(i=0; i<nLen; i++)
	 	*(pDat+i) = TWI_ReadByte(nAddr+i);
 }
 */
