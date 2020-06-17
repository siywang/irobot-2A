/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: system.c
**��        ��: wifi���ݴ�����
**ʹ �� ˵ �� : �û�������ĸ��ļ�ʵ������
**
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
#define SYSTEM_GLOBAL
#include "sys.h"
//#include "wifi.h"	//
//#include "protocol.h"
//
//
extern const DOWNLOAD_CMD_S download_cmd[];

/*****************************************************************************
�������� : set_wifi_uart_byte
�������� : дwifi_uart�ֽ�
������� : dest:��������ʵ��ַ;
           byte:д���ֽ�ֵ
���ز��� : д����ɺ���ܳ���
*****************************************************************************/
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
  unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
  
  *obj = byte;
  dest += 1;
  
  return dest;
}
/*****************************************************************************
�������� : set_wifi_uart_buffer
�������� : дwifi_uart_buffer
������� : dest:Ŀ���ַ
           src:Դ��ַ
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
  unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
  
  my_memcpy(obj,src,len);
  
  dest += len;
  return dest;
}
/*****************************************************************************
�������� : wifi_uart_write_data
�������� : ��wifi uartд����������
������� : in:���ͻ���ָ��
           len:���ݷ��ͳ���
���ز��� : ��
*****************************************************************************/
static void wifi_uart_write_data(unsigned char *in, unsigned short len)
{
  if((NULL == in) || (0 == len))
  {
    return;
  }
  
  while(len --)
  {
    uart_transmit_output(*in);
    in ++;
  }
}
/*****************************************************************************
�������� : get_check_sum
�������� : ����У���
������� : pack:����Դָ��
           pack_len:����У��ͳ���
���ز��� : У���
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;
  
  for(i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }
  
  return check_sum;
}
/*****************************************************************************
�������� : wifi_uart_write_frame
�������� : ��wifi���ڷ���һ֡����
������� : fr_type:֡����
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len)
{
  unsigned char check_sum = 0;
  
  wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
  wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
  wifi_uart_tx_buf[PROTOCOL_VERSION] = 0x00;
  wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
  wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
  wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;
  len += PROTOCOL_HEAD;
  check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
  wifi_uart_tx_buf[len - 1] = check_sum;
  wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}
void wifi_uart_write_cmd(unsigned char fr_type, unsigned short len)
{
  unsigned char check_sum = 0;
  wifi_uart_tx_buf[HEAD_FIRST] = 0x55;
  wifi_uart_tx_buf[HEAD_SECOND] = 0xaa;
/*
#if MAP_VERSION
  wifi_uart_tx_buf[PROTOCOL_VERSION] = 0x03;
#else
  wifi_uart_tx_buf[PROTOCOL_VERSION] = 0x01;
#endif
*/
  wifi_uart_tx_buf[PROTOCOL_VERSION] = 0x03;//Candy MCU->WIFI �˴���Ϊ03
  wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
  wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
  wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;
  
  len += PROTOCOL_HEAD;
  check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
  wifi_uart_tx_buf[len - 1] = check_sum;
  /*if(0x28 == wifi_uart_tx_buf[FRAME_TYPE])
  {
	  log_printf("[wifi]");
	  for(unsigned char iii = 0; iii < len; iii++)
	  {
		log_printf("%02x ",wifi_uart_tx_buf[iii]);
	  }
	  log_printf("\r\n");
  }*/
  //
  wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}
/*****************************************************************************
�������� : get_update_dpid_index
�������� : �����ƶ�DPID�������е����
������� : dpid:dpid
���ز��� : index:dp���
*****************************************************************************/
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
  unsigned char index;
  unsigned char total = get_download_cmd_total();
  
  for(index = 0; index < total; index ++)
  {
    if(download_cmd[index].dp_id == dpid)
    {
      break;
    }
  }
  
  return index;
}
/*****************************************************************************
�������� : data_point_handle
�������� : �·����ݴ���
������� : value:�·�����Դָ��
���ز��� : ret:�������ݴ�����
*****************************************************************************/
static unsigned char data_point_handle(const unsigned char value[])
{
  unsigned char dp_id,index;
  unsigned char dp_type;
  unsigned char ret;
  unsigned short dp_len;
  
  dp_id = value[0];
  dp_type = value[1];
  dp_len = value[2] * 0x100;
  dp_len += value[3];
  
  index = get_dowmload_dpid_index(dp_id);

  if(dp_type != download_cmd[index].dp_type && dp_type != 0)
  {
    //������ʾ
    log_printf("[IDX]%d-%d:%d-%d\r\n",index,dp_type,download_cmd[index].dp_id,download_cmd[index].dp_type);
    return FALSE;
  }
  else
  {
    ret = dp_download_handle(dp_id,value + 4,dp_len);
  }
  if(ret == FALSE)
  {
  	log_printf("[ERR]%d\r\n",dp_id);
  }
  return ret;
}
/*****************************************************************************
�������� : data_handle
�������� : ����֡����
������� : offset:������ʼλ
���ز��� : ��
*****************************************************************************/
void data_handle(unsigned short offset)
{
  unsigned short dp_len;
  ErrorStatus ret;
  unsigned short i,total_len;
  unsigned char cmd_type = wifi_uart_rx_buf[offset + FRAME_TYPE];

  log_printf("\r\n[data_handle]cmd:%d\r\n",cmd_type);
  switch(cmd_type)
  {
  case WIFI_STATE_CMD:                                  //wifi����״̬	
	{
	    wifi_work_state = wifi_uart_rx_buf[offset + DATA_START];
	    //wifi_uart_write_cmd(WIFI_STATE_CMD,0);
	    //uint8_t ding_cnt = 0;
	    switch(wifi_work_state)
	    {
	    	case WIFI_NOT_CONNECTED:
	    		//ding_cnt = 3;
	    		sys->wifi_time_sync = RESET;
	    		break;
		case WIFI_CONN_CLOUD:
			//ding_cnt = 4;
			break;
		case SMART_CONFIG_STATE:
			//ding_cnt = 1;
			break;
		case AP_STATE:
			//ding_cnt = 2;
			break;
		default:
			break;
	    }
	    log_printf("[WIFI]WIFI_STATE_CMD:%d\r\n",wifi_work_state);
	    /*
	    if(0 < ding_cnt)
	    {
	    	for(uint8_t i=0;i<ding_cnt;i++)
			{
				delay_ms_tm5(1000);
				ny3p_play(VOICE_DIDI);
			}
	    }
	    */
	}
    break;
    
  case WIFI_MODE_CMD:                                   //ѡ��smartconfig/APģʽ(wifi���سɹ�)	
    set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
    break;
    
  case DATA_QUERT_CMD:                                  //�����·�
    total_len = wifi_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    total_len += wifi_uart_rx_buf[offset + LENGTH_LOW];
    
    for(i = 0;i < total_len;)
    {
      dp_len = wifi_uart_rx_buf[offset + DATA_START + i + 2] * 0x100;
      dp_len += wifi_uart_rx_buf[offset + DATA_START + i + 3];
      //
      ret = (ErrorStatus)data_point_handle((unsigned char *)(wifi_uart_rx_buf + offset + DATA_START + i));
      
      if(SUCCESS == ret)
      {
        //�ɹ���ʾ
        log_printf("WIFI data_point_handle SUCCESS\r\n");
      }
      else
      {
        //������ʾ
        log_printf("WIFI data_point_handle ERR\r\n");
      }
      
      i += (dp_len + 4);
    }
    
    break;
    
//#ifdef SUPPORT_MCU_RTC_CHECK
  case GET_LOCAL_TIME_CMD:                             //��ȡ����ʱ��
      mcu_write_rtctime((unsigned char *)(wifi_uart_rx_buf + offset + DATA_START));
    break;
//#endif
 
//#ifdef WIFI_STREAM_ENABLE
  case STREAM_OPEN_CMD:
    stream_status = wifi_uart_rx_buf[offset + DATA_START];//���������Ƿ�ɹ�
    break;
  
  case STREAM_START_CMD:
    stream_status = wifi_uart_rx_buf[offset + DATA_START];//�����ݿ����Ƿ�ɹ�
    break;

  case STREAM_TRANS_CMD:
    stream_status = wifi_uart_rx_buf[offset + DATA_START];//�������䷵�ؽ���
    break;

  case STREAM_STOP_CMD:
    stream_status = wifi_uart_rx_buf[offset + DATA_START];//�����ݽ����Ƿ�ɹ�
    break;
//#endif    
  default:
    break;
  }
}
/*****************************************************************************
�������� : get_queue_total_data
�������� : ��ȡ����������
������� : ��
���ز��� : ��
*****************************************************************************/
unsigned char get_queue_total_data(void)
{
  if(queue_in != queue_out)
    return 1;
  else
    return 0;
}
/*****************************************************************************
�������� : Queue_Read_Byte
�������� : ��ȡ����1�ֽ�����
������� : ��
���ز��� : ��
*****************************************************************************/
unsigned char Queue_Read_Byte(void)
{
  unsigned char value;
  
  if(queue_out != queue_in)
  {
    //������
    if(queue_out >= (unsigned char *)(wifi_queue_buf + sizeof(wifi_queue_buf)))
    {
      //�����Ѿ���ĩβ
      queue_out = (unsigned char *)(wifi_queue_buf);
    }
    
    value = *queue_out ++;   
  }
  
  return value;
}

