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
					每5ms结束后开始计数
 *param: None
 *reval: None
*/
void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = 499; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=8399;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	//以下两个不用设置
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //PB6复用位定时器4
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //PB7复用位定时器4
	  
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=60000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;	//上升沿捕获
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICInit(TIM4, &TIM4_ICInitStructure);
		
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_SetCounter(TIM4, 30000);
  TIM_Cmd(TIM4,ENABLE ); 	//使能定时器5

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	//以下两个不用设置
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOB6 | 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	//以下两个不用设置
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //推挽复用
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PB

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); //PB6复用位定时器4
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //PB7复用位定时器4
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=60000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;	//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	TIM_SetCounter(TIM3, 30000);
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
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
