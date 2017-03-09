#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"
#include "PID.h"
#include "delay.h"

#define U16_MAX    ((u16)65535u)
#define U32_MAX    ((u32)4294967295uL)


#define SPEED_SAMPLING_TIME  9    // (9+1)*500usec = 5MS   ,200hz
#define SPEED_BUFFER_SIZE 3       //左右轮速度缓存数组大小

#define ENCODER1_TIMER TIM3   // 电机B码盘采集定时器 TIM3
#define ENCODER2_TIMER TIM4   // 电机A码盘采集定时器 TIM4
#define ENCODER2_PPR           (u16)(4096)  // 电机2码盘线数
#define ENCODER1_PPR           (u16)(4096)  // 电机2码盘线数

#define ICx_FILTER      (u8) 6 // 6<-> 670nsec   编码器模式设置参数

#define SPEED_SAMPLING_FREQ (u16)(2000/(SPEED_SAMPLING_TIME+1))  //200hz，小车速度采样频率
//#define SPEED_SAMPLING_FREQ (u16)(10000/(SPEED_SAMPLING_TIME+1)) //1000hz

static unsigned short int hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;//电机编码数采集时间间隔

void ENC_Init(void);//电机处理初始化

void ENC_Init1(void);//设置电机B TIM3编码器模式PA6 PA7 左电机
void ENC_Init2(void);//设置电机A TIM4编码器模式PB6 PB7 右电机

s16 ENC_Calc_Rot_Speed1(void);//计算电机B的编码数
s16 ENC_Calc_Rot_Speed2(void);//计算电机A的编码数

void ENC_Clear_Speed_Buffer(void);//速度存储器清零
void ENC_Calc_Average_Speed(void);//计算三次电机的平均编码数

void Gain1(void);//设置电机B PID调节 PA1
void Gain2(void);//设置电机A PID调节 PA2

#endif 
