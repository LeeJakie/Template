/**
  ******************************************************************************
  * @file    steer.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-05-19
  * @brief   ���ģ��
  ******************************************************************************
  * @attention
  * �ṩ�����ʼ�������͹رպ�����
	* �ṩ���������ʼ�������͹رպ�����
	* �ṩ״̬ת��pwm������
  ******************************************************************************
  */
	
#include "steer.h"
#include "status.h"
#include "math.h"

/**
  * @brief  ��ʼ��GPIO
  * @param  ��
  * @retval ��
  */
static void steer_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
}

/**
  * @brief  ��ʼ��TIM8
  * @param  ��
  * @retval ��
  */
static void steer_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period=9999;
	TIM_TimeBaseStructure.TIM_Prescaler=167;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1499;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
  TIM_OC2Init(TIM8,&TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
  TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
}

/**
  * @brief  ��ʼ�����
  * @param  ��
  * @retval ��
  */
void steer_Init(void)
{
	steer_GPIO_Config();
	steer_Mode_Config();
}

/**
  * @brief  �رն��
  * @param  ��
  * @retval ��
  */
void steer_Close(void)
{
	TIM_Cmd(TIM8,DISABLE);
}


/**
  * @brief  �õ������pwm�����仯�ʣ�PID����
  * @param  ��
  * @retval ��
  */
void steer_Parameter(void)
{
	Steer_Er_Lalast = Steer_Er_Last;
	Steer_Er_Last = Steer_Er_New;
	Steer_Er_New = Position - 64;
}

/**
  * @brief  ����ǰ״̬ת��Ϊpwm
  * @param  ��
  * @retval pwmֵ
  */
uint32_t steer_PWM(void)
{
	return 1499;
}


