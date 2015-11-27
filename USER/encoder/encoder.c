/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_encoder.c 
* Author             : IMS Systems Lab  
* Date First Issued  : 21/11/07
* Description        : This file contains the software implementation for the
*                      encoder unit
******************** (C) COPYRIGHT 2007 STMicroelectronics ********************/

/* Includes ------------------------------------------------------------------*/
#include "encoder.h"
#include "status.h"
/* Private define ------------------------------------------------------------*/
#define Pulse_20cm 5730  // number of pulses per revolution 
/* Private variables ---------------------------------------------------------*/		
s16 Error_times=0;

/*
 *berif: Initialize the timer of the encoding module(Left & Right)
					ÿ5ms������ʼ����
 *param: None
 *reval: None
*/
void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = 499; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=8399;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*
 *berif: Initialize the counter of the encoding module(Left)
 *param: None
 *reval: None
*/
void TIM4_Init()
{
	TIM_ICInitTypeDef 				TIM4_ICInitStructure;
	GPIO_InitTypeDef 					GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//����������������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //PB6����λ��ʱ��4
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //PB7����λ��ʱ��4
	  
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=60000;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;	//�����ز���
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);
		
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_SetCounter(TIM4, 30000);
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��5

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

/*
 *berif: Initialize the counter of the encoding module(Right)
 *param: None
 *reval: None
*/
void TIM3_Init()
{
	TIM_ICInitTypeDef  			 TIM3_ICInitStructure;
	GPIO_InitTypeDef 				 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef 				 NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//����������������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	//����������������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PB

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //PB6����λ��ʱ��4
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //PB7����λ��ʱ��4
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=60000;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;	//�����ز���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_SetCounter(TIM3, 30000);
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��5

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

/*
 *berif: Initialize the encoding module
 *param: None
 *reval: None
*/
void Init_Encoder(void)
{
	TIM2_Init();		//Timer
	TIM3_Init();		//Counter(R)
	TIM4_Init();		//Counter(L)
}

/*
 *berif: Timer interrupt request function
 *param: None
 *reval: None
*/
void TIM2_IRQHandler(void)			//5ms
{  
  /* Clear the interrupt pending flag */
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
  {
		TIM_Cmd(TIM4, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		SpeedL = 30000 - TIM_GetCounter(TIM4);
		SpeedR = 30000 - TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM4, 30000);
		TIM_SetCounter(TIM3, 30000);
		TIM_Cmd(TIM4, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  }
}

/*
 *berif: Counter interrupt request function
 *param: None
 *reval: None
*/
void TIM4_IRQHandler(void)			
{  
  /* Clear the interrupt pending flag */
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  
  {
		Error_times += 1;
		TIM_SetCounter(TIM2, 0);
		TIM_SetCounter(TIM3, 30000);
		TIM_SetCounter(TIM4, 30000);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  }
}
void TIM3_IRQHandler(void)			
{  
  /* Clear the interrupt pending flag */
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
  {
		Error_times += 1;
		TIM_SetCounter(TIM2, 0);
		TIM_SetCounter(TIM3, 30000);
		TIM_SetCounter(TIM4, 30000);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  }
}
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
