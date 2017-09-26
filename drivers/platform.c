#include <rtthread.h>
#include "board.h"

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
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}



void rt_platform_init(void * para)
{
	/*dac PA4 channel1 init*/
	dac_hw_init();
	
}
