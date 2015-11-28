/**
  ******************************************************************************
  * @file    auto_receive_send_data.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-20
  * @brief   自动收发数据模块
  ******************************************************************************
  * @attention
	* 用到定时器TIM7
	* ARSD即auto_receive_send_data
  ******************************************************************************
  */

#include "auto_receive_send_data.h"
#include "send_data.h"

/**
  * @brief  定时器7的NVIC初始化
  * @param  无
  * @retval 无
  */
void ARSD_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  定时器7设置初始化
  * @param  无
  * @retval 无
  */
void ARSD_TIM7_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
  TIM_TimeBaseInitStructure.TIM_Period = 300;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM7,ENABLE);
}

/**
  * @brief  ARSD初始化
  * @param  无
  * @retval 无
  */
void ARSD_Init(void)
{
	ARSD_NVIC_Config();
	ARSD_TIM7_Config();
}

/**
  * @brief  ARSD定时器中断处理
  * @param  无
  * @retval 无
  */
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
		Data_Send_Speed();
		Data_Send_PWM();
		Data_Send_Position();
		Data_Send_PID();
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}
/*********************************************END OF FILE**********************/
