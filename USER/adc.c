#include "sys.h"

#include "stm32f10x.h"
/**************************************************第1步：方向信号采集，配置所用的ADC***************************************/

#if 1
#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 adc_converted_value[ADC_CHANNEL_NUM+1];
//__IO u16 ADC_ConvertedValueLocal;


/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC0，pc1
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)
{
/*
第二硬件版本
14	PA3	左边红外碰撞传感器	ADC
15	PA4	左中红外碰撞传感器	ADC
16	PA5	左中红外碰撞传感器	ADC
17	PA6	中间红外碰撞传感器	ADC
18	PA7	右中红外碰撞传感器	ADC
19	PB0	右中红外碰撞传感器	ADC
20	PB1	右边红外碰撞传感器	ADC
第三硬件版本
PB1	左边红外碰撞传感器	ADC9
PB0	左中红外碰撞传感器	ADC8
PB0	左中红外碰撞传感器	ADC8
PA7	左中红外碰撞传感器	ADC7
PA3	中间红外碰撞传感器	ADC3
PA4	右中红外碰撞传感器	ADC4
PA5	右中红外碰撞传感器	ADC5
PA6	右边红外碰撞传感器	ADC6

PA0	左边防跌落	ADC 		ADC0
PA1	中间防跌落	ADC			ADC1
PA2	右边防跌落	ADC			ADC2

久昌版本
/--------------------------
PB1 左边红外测距传感器	ADC9
PA5	左边红外碰撞传感器	ADC5
PA3	中间红外测距传感器	ADC3
PA4	右边红外碰撞传感器	ADC4
PA6	右边红外测距传感器	ADC6

PA0	左边防跌落			ADC0
PA2	右边防跌落			ADC2
----------------------------/

	PC4	中扫电机的过流检测（H/L电平检测）	ADC
	PC5	是吸尘电机的过流检测（H/L电平检测）


PC3		AD13 	充电电流采样
PC2		AD12 	充电电压

PC0		AD10 	后轮右电机电流
PC1     AD11	后左轮电机电流

*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PC1,输入时不用设置速率


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PC4	中扫电机的过流检测�  PC5	是吸尘电机的过流检测
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)adc_converted_value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    /*设计内存地址加一*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUM;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);

	PC3		AD13 充电电流采样
	PC2		AD12 充电电压
	PC0		后轮右电机电流
	PC1     后左轮电机电流
 */

	//中扫电机和吸尘电机的AD口
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 16, ADC_SampleTime_55Cycles5);	//充电电流
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 15, ADC_SampleTime_55Cycles5);	//充电电压

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 14, ADC_SampleTime_55Cycles5);	//后轮右电机电流
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 13, ADC_SampleTime_55Cycles5);	//后轮左电机电流

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 12, ADC_SampleTime_55Cycles5);	//灰检
	////ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_55Cycles5);//中扫电机过流检测

	/*
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 7, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);
	*/
	//七路前置红外
#if VER_BORD_2
/*
14	PA3	左边红外碰撞传感器	ADC
15	PA4	左中红外碰撞传感器	ADC
16	PA5	左中红外碰撞传感器	ADC
17	PA6	中间红外碰撞传感器	ADC
18	PA7	右中红外碰撞传感器	ADC
19	PB0	右中红外碰撞传感器	ADC
20	PB1	右边红外碰撞传感器	ADC

*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);	//左边
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_55Cycles5);	//中间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7, ADC_SampleTime_55Cycles5);	//右边
#else

#if (1 == HOOVER)
/*
PA5	左边红外测距传感器	ADC9	1
PB1	左中红外碰撞传感器	ADC8	2
PA3	中间红外碰撞传感器	ADC3	4
PA6	右中红外碰撞传感器	ADC4	5
PA4	右边红外测距传感器	ADC6	7
*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5); //左边
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_55Cycles5);	//碰撞左
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); //LAOBANBEN中间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5); //中间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);	//碰撞右
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5); //右边
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_55Cycles5);//前面三个灯
#else
/*
PB1	左边红外碰撞传感器	ADC9	1
PB0	左中红外碰撞传感器	ADC8	2
PA7	左中红外碰撞传感器	ADC7	3
PA3	中间红外碰撞传感器	ADC3	4
PA4	右中红外碰撞传感器	ADC4	5
PA5	右中红外碰撞传感器	ADC5	6
PA6	右边红外碰撞传感器	ADC6	7

*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5); //左边
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); //中间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_55Cycles5); //右边
#endif
#endif

#if (1 == HOOVER)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 10, ADC_SampleTime_55Cycles5);

#else
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 8, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 9, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 10, ADC_SampleTime_55Cycles5);
#endif
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void adc1_init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


#else
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t adc_converted_value[ADC_CHANNEL_NUM+1];//ad值暂存数组


void adc_init(void )
{

/* Private function prototypes -----------------------------------------------*/
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能 MDA 重要！！！
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);// 使能 ADC1 重要！！！


  //ADC_InitTypeDef  ADC_InitStructure;
  //PC1/2/3 作为模拟通道输入引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //模拟输入引脚
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //模拟输入引脚
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	/*DMA1 channel1 configuration ----------------------------------------------*/
  	DMA_DeInit(DMA1_Channel1);			   //将DMA的通道1寄存器重设为缺省值
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	  //定义DMA外设基地址为ADC1
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_converted_value; //定义DMA内存基地址
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	  //外设作为数据传输的来源
  	DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;	  //定义指定DMA通道的DMA缓存的大小
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;		//内存地址寄存器不变
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//数据宽度为16位
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//数据宽度为16位
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	  //工作在循环缓存模式
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA通道1拥有高优先级
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//DMA通道1没有设置为内存到内存传输
  	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  	/* Enable DMA1 channel1 */
  	DMA_Cmd(DMA1_Channel1, ENABLE);

  	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //独立工作模式
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //扫描模式
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   //连续转换模式
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件控制转换
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //右对齐
  	ADC_InitStructure.ADC_NbrOfChannel =ADC_CHANNEL_NUM;
  	ADC_Init(ADC1, &ADC_InitStructure);

  	/* ADC1 regular channel_8,configuration */
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5); //转换时间为17.1US
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);

  	/* Enable ADC1 DMA */
  	ADC_DMACmd(ADC1, ENABLE);

  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);

  	/* Enable ADC1 reset calibaration register */
  	ADC_ResetCalibration(ADC1);

  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibaration */
  	ADC_StartCalibration(ADC1);

  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));

  	/* Start ADC1 Software Conversion */
  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
#endif



