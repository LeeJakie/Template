/**
  ******************************************************************************
  * @file    motor.h
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-05-19
  * @brief   
  ******************************************************************************
  * @attention
  * 提供电机初始化及关闭函数。
	* 
  ******************************************************************************
  */

#ifndef __MOTOR_H
#define	__MOTOR_H

#include "stm32f4xx.h"

void motor_Init(void);
void motor_Close(void);
void motor_Parameter(void);

#endif /* __MOTOR_H */


