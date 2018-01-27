#include <rtthread.h>
#include "board.h"

static void LCD_HW_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	FSMC_NORSRAMTimingInitTypeDef ReadWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef WriteTiming;
	
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStruct;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,ENABLE); //开启引脚的时钟
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_7 | GPIO_Pin_8 | 
													  GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);   //CS
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);   //WR
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);   //RD
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
	
	//////////
	
	ReadWriteTiming.FSMC_AddressSetupTime=15;
  ReadWriteTiming.FSMC_AddressHoldTime=0;
	ReadWriteTiming.FSMC_DataSetupTime=60;
	ReadWriteTiming.FSMC_BusTurnAroundDuration=0;
	ReadWriteTiming.FSMC_CLKDivision=0;
	ReadWriteTiming.FSMC_DataLatency=0;
	ReadWriteTiming.FSMC_AccessMode=FSMC_AccessMode_A;
	
	WriteTiming.FSMC_AddressSetupTime=3;
  WriteTiming.FSMC_AddressHoldTime=0;
	WriteTiming.FSMC_DataSetupTime=4;
	WriteTiming.FSMC_BusTurnAroundDuration=0;
	WriteTiming.FSMC_CLKDivision=0;
	WriteTiming.FSMC_DataLatency=0;
	WriteTiming.FSMC_AccessMode=FSMC_AccessMode_A;
	
	
	
	FSMC_NORSRAMInitStruct.FSMC_Bank=FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStruct.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStruct.FSMC_MemoryType=FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStruct.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStruct.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStruct.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; //不使能异步等待
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low; 
	FSMC_NORSRAMInitStruct.FSMC_WrapMode=FSMC_WrapMode_Disable;							//不使能突发循环传送
	FSMC_NORSRAMInitStruct.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStruct.FSMC_WriteOperation=FSMC_WriteOperation_Enable;     //写操作使能
	FSMC_NORSRAMInitStruct.FSMC_WaitSignal=FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStruct.FSMC_ExtendedMode=FSMC_ExtendedMode_Enable;			//使用其他时序模式
	FSMC_NORSRAMInitStruct.FSMC_WriteBurst=FSMC_WriteBurst_Disable;
	
	FSMC_NORSRAMInitStruct.FSMC_ReadWriteTimingStruct=&ReadWriteTiming;
	FSMC_NORSRAMInitStruct.FSMC_WriteTimingStruct=&WriteTiming;
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStruct);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
	
}

static void dac_hw_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	/*rcc enable*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	/*init gpio*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*-----------dac config--------------*/
	DAC_InitType.DAC_Trigger = DAC_Trigger_None;	
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;	
  DAC_Init(DAC_Channel_1, &DAC_InitType);
	/*enable dac channel1*/
	DAC_Cmd(DAC_Channel_1, ENABLE);
	/*set default value to dac channel1*/
	DAC_SetChannel1Data(DAC_Align_12b_R, 4000);
}


extern rt_uint16_t r61529_read_id(void);
void rt_platform_init(void * para)
{
	rt_uint16_t id;
	/*dac PA4 channel1 init*/
	dac_hw_init();
	
	LCD_HW_init();
	
	id = r61529_read_id();
}
