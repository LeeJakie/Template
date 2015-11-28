/**
  ******************************************************************************
  * @file    status.c
  * @author  Yefan Liu
  * @version V1.0
  * @date    2015-07-20
  * @brief   Ñ²ÏßÐ¡³µ×´Ì¬
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "status.h"

int32_t Position = 64;
int32_t SpeedL= 0;
int32_t SpeedR = 0;
int32_t SpeedL_Ex = 0;
int32_t SpeedR_Ex = 0;
int32_t SpeedL_Er_New = 0;
int32_t SpeedL_Er_Last = 0;
int32_t SpeedL_Er_Lalast = 0;
int32_t SpeedR_Er_New = 0;
int32_t SpeedR_Er_Last = 0;
int32_t SpeedR_Er_Lalast = 0;
int32_t Steer_Er_New = 0;
int32_t Steer_Er_Last = 0;
int32_t Steer_Er_Lalast = 0;
int32_t Motor_PWML = 0;
int32_t Motor_PWMR = 0;
int32_t Steer_PWM = 0;
float SpeedP = 50;
float SpeedI = 0;
float SpeedD = 8;
float SteerP = 9;
float SteerI = 0;
float SteerD = 4;

/************************************END OF FILE*******************************/
