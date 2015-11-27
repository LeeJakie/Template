/**
  ******************************************************************************
  * @file    bluetooth.h
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   蓝牙模块
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f4xx.h"

extern uint8_t data_to_send[9]; //发送缓冲区,请将要发送到数据先添加到这里
extern uint8_t data_buf[9];     //接受缓冲区,无需清空

void bluetooth_Init(void); //蓝牙初始化
void Uart1_Put_Buf(uint8_t count); //选择发送data_to_send数组里的前几个数据

#endif
/************************************END OF FILE*******************************/
