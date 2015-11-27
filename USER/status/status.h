/**
  ******************************************************************************
  * @file    status.h
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-20
  * @brief   ËÄÖá×´Ì¬ÉùÃ÷
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef __STATUS_H
#define __STATUS_H

#include "stm32f4xx.h"

extern int32_t Position;
extern int32_t SpeedL;
extern int32_t SpeedR;
extern int32_t SpeedL_Ex;
extern int32_t SpeedR_Ex;
extern int32_t SpeedL_Er_New;
extern int32_t SpeedL_Er_Last;
extern int32_t SpeedL_Er_Lalast;
extern int32_t SpeedR_Er_New;
extern int32_t SpeedR_Er_Last;
extern int32_t SpeedR_Er_Lalast;
extern int32_t Steer_Er_New;
extern int32_t Steer_Er_Last;
extern int32_t Steer_Er_Lalast;
extern int32_t Motor_PWML;
extern int32_t Motor_PWMR;
extern int32_t Steer_PWM;
extern float SpeedP;
extern float SpeedI;
extern float SpeedD;
extern float SteerP;
extern float SteerI;
extern float SteerD;

#endif
/************************************END OF FILE*******************************/
