/**
  ******************************************************************************
  * @file    infrared.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-6-15
  * @brief   线性ccd采集程序
  ******************************************************************************
  * @attention
  * 包括引脚初始化，ADC初始化，曝光与采集程序。
	* PC11：AO，   PA2：SI，   PA3：CLK,    PG8:Key
  ******************************************************************************
  */ 
  
#include "infrared.h"
#include "delay.h"
#include "encoder.h"
#include "status.h"

/**
  * @brief  infrared引脚初始化
  * @param  无
  * @retval 无
  */
void infrared_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
	                              GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
	                              GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}


/**
  * @brief  初始化TIM6
  * @param  无
  * @retval 无
  */
void infrared_TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	TIM_TimeBaseStructure.TIM_Period=300;
	TIM_TimeBaseStructure.TIM_Prescaler=8400;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}

/**
  * @brief  初始化NVIC
  * @param  无
  * @retval 无
  */
void infrared_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}	

/**
  * @brief  infrared初始化
  * @param  无
  * @retval 无
  */
void infrared_Init(void)
{
	infrared_GPIO_Config();
	infrared_TIM6_Config();
	infrared_NVIC_Config();
}

/**
  * @brief  ccd状态书写
  * @param  无
  * @retval 无
  */
void infrared_data_read(uint8_t data)
{
	if(data == 2)
	{
		Position = 1;
		SpeedL_Ex = 0;
		SpeedR_Ex = 300;
	}
	else if(data == 3)
	{
		Position = 127;
		SpeedL_Ex = 300;
		SpeedR_Ex = 0;
	}
	else if(data >= 18 && data <= 242)
	{
		Position = data/2 - 1;
		SpeedL_Ex = 200;
		SpeedR_Ex = 200;
	}
}

/**
  * @brief  This function handles TIM6 interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_DAC_IRQHandler(void)
{
	uint8_t data = 0;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
	{
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3);
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2) << 1;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1) << 2;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0) << 3;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7) << 4;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6) << 5;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5) << 6;
		data |= GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4) << 7;
		infrared_data_read(data);
	  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  }
}

/**
  * @brief  关闭红外
  * @param  无
  * @retval 无
  */
void infrared_Close(void)
{
	TIM_Cmd(TIM6,DISABLE);
}
