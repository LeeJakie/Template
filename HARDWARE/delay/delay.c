/**
  ******************************************************************************
  * @file    delay.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   ��ʱģ��
  ******************************************************************************
  * @attention
	* �ṩ������ϵͳʱ�ӵľ�ȷ��ʱ,�����ʱ500ms
	* �ṩ������IICЭ��ķǾ�ȷ��ʱ
	* ��ȷ��ʱ��Ҫ��ʼ��
  ******************************************************************************
  */
	
#include "delay.h"

static uint8_t fac_us=0; //us��ʱ������
static uint16_t fac_ms=0; //ms��ʱ������

/**
  * @brief  ��ʱ��ʼ��
  * @param  ϵͳʱ��Ƶ��
  * @retval ��
  */
void delay_Init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;	    
	fac_ms=(uint16_t)fac_us*1000;
}								    

/**
  * @brief  ms������ʱ
  * @param  ��Ҫ��ʱ��ms�������500
  * @retval ��
  */
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms; //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;                 //��ռ�����
	SysTick->CTRL=0x01 ;                //��ʼ����
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); //�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;                //�رռ�����
	SysTick->VAL =0X00;                //��ռ�����	  	    
}

/**
  * @brief  us������ʱ
  * @param  ��Ҫ��ʱ��us��
  * @retval ��
  */		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); //�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;                //�رռ�����
	SysTick->VAL =0X00;                //��ռ�����	 
}

/**
  * @brief  IIC����ʱ
  * @param  ��ʱ��������ʾȷ��ʱ��
  * @retval ��
  */
void delay_num(uint32_t num)
{
	uint32_t i;
	for(i=0;i<num;i++);
}

/**
  * @brief  IIC��˲ʱ��ʱ,Լ1.5us
  * @param  ��
  * @retval ��
  */
void delay_nop(void)
{
	uint8_t i=10;
	while(i--);
}
/************************************END OF FILE*******************************/
