/**
  ******************************************************************************
  * @file    delay.h
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





























