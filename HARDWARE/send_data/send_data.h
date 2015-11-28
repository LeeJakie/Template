/**
  ******************************************************************************
  * @file    send_data.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   下位机发送数据模块
  ******************************************************************************
  * @attention
	* 适用于匿名上位机
  ******************************************************************************
  */

#ifndef __SEND_DATA_H
#define __SEND_DATA_H

#include "stm32f4xx.h"

void Data_Send_Speed(void);
void Data_Send_PWM(void);
void Data_Send_PID(void);
void Data_Send_Position(void);

#endif
/************************************END OF FILE*******************************/
