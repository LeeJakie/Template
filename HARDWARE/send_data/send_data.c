/**
  ******************************************************************************
  * @file    send_data.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   ��λ����������ģ��
  ******************************************************************************
  * @attention
	* ��������������վ(��λ��)
  ******************************************************************************
  */

#include "send_data.h"
#include "bluetooth.h"
#include "status.h"
#include <string.h>

/**
  * @brief  ����С���������ٶ�
  * @param  ��
  * @retval ��
  */
void Data_Send_Speed(void)
{
	int32_t temp;
	data_to_send[2] = 0xe1;
	
	
	if(SpeedL < 0)
	{
		temp = -SpeedL;
		data_to_send[3] = 0x00;
	}
	else 
	{
		temp = SpeedL;
		data_to_send[3]=0x01;
	}
	data_to_send[4]=0x04;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	if(SpeedR < 0)
	{
		temp = -SpeedR;
		data_to_send[3] = 0x00;
	}
	else 
	{
		temp = SpeedR;
		data_to_send[3]=0x01;
	}
	data_to_send[4]=0x05;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
}

/**
  * @brief  ����������PWM�����PWM
  * @param  ��
  * @retval ��
  */
void Data_Send_PWM(void)
{
	int32_t temp;
	data_to_send[2] = 0xe1;
	
	
	temp = TIM1->CCR1;
	data_to_send[3]=0x01;
	data_to_send[4]=0x0C;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = TIM1->CCR2;
	data_to_send[3]=0x01;
	data_to_send[4]=0x0D;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = TIM8->CCR1;
	data_to_send[3]=0x01;
	data_to_send[4]=0x0E;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
}

/**
  * @brief  ����λ��״̬
  * @param  ��
  * @retval ��
  */
void Data_Send_Position(void)
{
	int32_t temp;
	data_to_send[2] = 0xe1;
	
	
	temp = Position;
	data_to_send[3]=0x01;
	data_to_send[4]=0x0F;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
}

/**
  * @brief  ����PID���ݵ���λ��
  * @param  ��
  * @retval ��
  */
void Data_Send_PID(void)
{
	int32_t temp;
	data_to_send[2] = 0xe2;
	
	
	temp = SpeedP*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA3;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = SpeedI*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA4;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = SpeedD*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA5;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = SteerP*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA6;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = SteerI*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA7;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
	
	
	temp = SteerD*1000;
	data_to_send[3]=0x01;
	data_to_send[4]=0xA8;
	data_to_send[5]=(u8)(temp/65536);
	data_to_send[6]=(u8)(temp/256);
	data_to_send[7]=(u8)(temp%256);
	Uart1_Put_Buf(9);
}

/************************************END OF FILE*******************************/
