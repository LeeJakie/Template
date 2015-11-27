/**
  ******************************************************************************
  * @file    bluetooth.h
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   ����ģ��
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f4xx.h"

extern uint8_t data_to_send[9]; //���ͻ�����,�뽫Ҫ���͵���������ӵ�����
extern uint8_t data_buf[9];     //���ܻ�����,�������

void bluetooth_Init(void); //������ʼ��
void Uart1_Put_Buf(uint8_t count); //ѡ����data_to_send�������ǰ��������

#endif
/************************************END OF FILE*******************************/
