/**
  ******************************************************************************
  * @file    main.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   主函数
  ******************************************************************************
  * @attention
	* 
  ******************************************************************************
  */

#include "delay.h"
#include "stm32f4xx.h"
#include "status.h"
#include "bluetooth.h"
#include "send_data.h"
#include "auto_receive_send_data.h"
#include "motor.h"
#include "steer.h"
#include "infrared.h"
#include "PID.h"
#include "encoder.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	delay_Init(168);
	bluetooth_Init();
	ARSD_Init();
	steer_Init();
	motor_Init();
	infrared_Init();
	Init_Encoder();
	Parameter_Init();
	while(1)
	{
		
	}
}
/*********************************************END OF FILE**********************/
