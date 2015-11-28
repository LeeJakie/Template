/**
  ******************************************************************************
  * @file    bluetooth.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-18
  * @brief   蓝牙模块
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "bluetooth.h"

uint8_t data_to_send[9] = {0xc0, 0xfb, 0, 0, 0, 0, 0, 0, 0xdd}; //发送缓冲区
uint8_t data_buf[9] = {0};     //接收缓冲区

/**
  * @brief  蓝牙GPIO初始化
  * @param  无
  * @retval 无
  */
void bluetooth_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  蓝牙USART初始化
  * @param  无
  * @retval 无
  */
void bluetooth_USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  蓝牙NVIC初始化
  * @param  无
  * @retval 无
  */
void bluetooth_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  蓝牙初始化
  * @param  无
  * @retval 无
  */
void bluetooth_Init(void)
{
	bluetooth_GPIO_Config();
	bluetooth_USART_Config();
	bluetooth_NVIC_Config();
}

/**
  * @brief  关闭蓝牙
  * @param  无
  * @retval 无
  */
void bluetooth_Close(void)
{
	USART_Cmd(USART1, DISABLE);
}

/**
  * @brief  蓝牙发送模块
  * @param  发送的字节数(一个字节8位)
  * @retval 无
  */
void Uart1_Put_Buf(uint8_t count)
{
	uint8_t i;
	for(i=0;i<count;i++)
 	{
		USART_SendData(USART1, data_to_send[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	}
}

/**
  * @brief  蓝牙接收模块
  * @param  无
  * @retval 无
  */
void USART1_IRQHandler(void)
{
	uint8_t com_data;
	if(USART1->SR & USART_SR_ORE)   //ORE中断
	{
		com_data = USART1->DR;
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		com_data = USART1->DR;
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	}
}

/************************************END OF FILE*******************************/
