/**
  ******************************************************************************
  * @file    delay.h
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

#ifndef __DELAY_H__
#define __DELAY_H__ 	

#include "stm32f4xx.h"

void delay_Init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void delay_num(uint32_t num);
void delay_nop(void);

#endif

/************************************END OF FILE*******************************/





























