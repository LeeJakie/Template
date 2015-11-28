/**
  ******************************************************************************
  * @file    steer.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-05-19
  * @brief   电机模块
  ******************************************************************************
  * @attention
  * 提供电机初始化函数和关闭函数。
	* 使用PA8，PB13,PB12,TIM1通道1，2,TIM2，4，拥有最高优先级。
  ******************************************************************************
  */

#include "status.h"
#include "motor.h"
#include "encoder.h"
#include "math.h"
 

/**
  * @brief  初始化GPIO
  * @param  无
  * @retval 无
  */
static void motor_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM1);
}


/**
  * @brief  初始化TIM1
  * @param  无
  * @retval 无
  */
static void motor_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_DeInit(TIM1);
	
	TIM_TimeBaseStructure.TIM_Period = 16799;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 
  TIM_OCInitStructure.TIM_Pulse = 9999;	   
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;   
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;   
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	
//	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
//	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
//	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//	TIM_BDTRInitStructure.TIM_DeadTime = 0x90;
//	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

  TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

/**
  * @brief  初始化电机
  * @param  无
  * @retval 无
  */
void motor_Init(void)
{
	motor_GPIO_Config();
	motor_Mode_Config();
}

/**
  * @brief  关闭电机
  * @param  无
  * @retval 无
  */
void motor_Close(void)
{
	TIM_Cmd(TIM1,DISABLE);
}

/**
  * @brief  电机参数更新
  * @param  无
  * @retval 无
  */
void motor_Parameter(void)
{	
	SpeedL_Er_Lalast = SpeedL_Er_Last;
	SpeedL_Er_Last = SpeedL_Er_New;	
	SpeedL_Er_New = SpeedL_Ex - SpeedL;
	
	SpeedR_Er_Lalast = SpeedR_Er_Last;
	SpeedR_Er_Last = SpeedR_Er_New;	
	SpeedR_Er_New = SpeedR_Ex - SpeedR;
}


