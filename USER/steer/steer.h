/**
  ******************************************************************************
  * @file    steer.h
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-05-19
  * @brief   
  ******************************************************************************
  * @attention
  * 提供舵机及参数初始化及关闭函数.
	* 提供状态转换pwm函数
  ******************************************************************************
  */

#ifndef __STEER_H
#define	__STEER_H

#include "stm32f4xx.h"

void steer_Init(void);
void steer_Close(void);
uint32_t steer_PWM(void);
void steer_Parameter(void);

#endif /* __STEER_H */
