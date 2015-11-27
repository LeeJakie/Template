/**
  ******************************************************************************
  * @file    PID.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-05-19
  * @brief   PID模块
  ******************************************************************************
  * @attention
	*
  ******************************************************************************
  */
	
#include "PID.h"
#include "status.h"
#include "steer.h"
#include "motor.h"
#include "encoder.h"

/**
  * @brief  初始化TIM5
  * @param  无
  * @retval 无
  */
void parameter_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_TimeBaseStructure.TIM_Period=300;
	TIM_TimeBaseStructure.TIM_Prescaler=83;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM5,TIM_IT_Update, ENABLE);	
  TIM_Cmd(TIM5, ENABLE);
}

/**
  * @brief  初始化NVIC
  * @param  无
  * @retval 无
  */
void parameter_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  开启参数更新
  * @param  无
  * @retval 无
  */
void Parameter_Init(void)
{
	parameter_NVIC_Config();
	parameter_Mode_Config();
}

/**
  * @brief  关闭参数更新
  * @param  无
  * @retval 无
  */
void Parameter_Close(void)
{
	TIM_Cmd(TIM5,DISABLE);
}

/**
  * @brief  This function handles TIM5 interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	{
		steer_Parameter();
		motor_Parameter();
		Steer_PWM = steer_PWM() + SteerP * Steer_Er_New + SteerD * (Steer_Er_New - Steer_Er_Last);
		if(Steer_PWM > 1999)
		{
			TIM8->CCR2 = 1999;
			Steer_PWM = 1999;
		}
		else if(Steer_PWM < 999)
		{
			TIM8->CCR2 = 999;
			Steer_PWM = 999;
		}
		else
		{
		   TIM8->CCR2 = Steer_PWM;
		}
		
		if(Position != 1 && Position != 127)
		{
			if(SpeedR > 20)
			{
				Motor_PWML =  SpeedP * Steer_Er_New + SpeedD * (Steer_Er_New - Steer_Er_Last);
				TIM1->CCR1 = 4999 - Motor_PWML;
				TIM1->CCR2 = 4999 + Motor_PWML;
			}
			else
			{
				Motor_PWML =  SpeedP * Steer_Er_New + SpeedD * (Steer_Er_New - Steer_Er_Last);
				TIM1->CCR1 = 7999 - Motor_PWML;
				TIM1->CCR2 = 7999 + Motor_PWML;
			}
		}
		else if(Position == 1)
		{
			TIM1->CCR1 = 4999;
			TIM1->CCR2 = 999;
		}
		else
		{
			TIM1->CCR1 = 999;
			TIM1->CCR2 = 4999;
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}