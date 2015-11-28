/**
  ******************************************************************************
  * @file    delay.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   延时模块
  ******************************************************************************
  * @attention
	* 提供了利用系统时钟的精确延时,最多延时500ms
	* 提供了用于IIC协议的非精确延时
	* 精确延时需要初始化
  ******************************************************************************
  */
	
#include "delay.h"

static uint8_t fac_us=0; //us延时倍乘数
static uint16_t fac_ms=0; //ms延时倍乘数

/**
  * @brief  延时初始化
  * @param  系统时钟频率
  * @retval 无
  */
void delay_Init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;	    
	fac_ms=(uint16_t)fac_us*1000;
}								    

/**
  * @brief  ms级别延时
  * @param  需要延时的ms数，最大500
  * @retval 无
  */
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms; //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;                 //清空计数器
	SysTick->CTRL=0x01 ;                //开始倒数
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); //等待时间到达   
	SysTick->CTRL=0x00;                //关闭计数器
	SysTick->VAL =0X00;                //清空计数器	  	    
}

/**
  * @brief  us级别延时
  * @param  需要延时的us数
  * @retval 无
  */		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); //等待时间到达   
	SysTick->CTRL=0x00;                //关闭计数器
	SysTick->VAL =0X00;                //清空计数器	 
}

/**
  * @brief  IIC用延时
  * @param  延时数，不表示确定时间
  * @retval 无
  */
void delay_num(uint32_t num)
{
	uint32_t i;
	for(i=0;i<num;i++);
}

/**
  * @brief  IIC用瞬时延时,约1.5us
  * @param  无
  * @retval 无
  */
void delay_nop(void)
{
	uint8_t i=10;
	while(i--);
}
/************************************END OF FILE*******************************/
