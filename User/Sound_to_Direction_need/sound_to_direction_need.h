#ifndef __SOUND_TO_DIRECTION_NEED_H
#define __SOUND_TO_DIRECTION_NEED_H

#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
#include "cstring"

//每个声音传感器的序号
#define number_f 1
#define number_b 2
#define number_l 3
#define number_r 4

#define need_derivation 36    //高速时钟36MHz，除此数为了将时间单位转化成微秒

void TIM8_Init(int pre_scalar);  //高速计数时钟，参数为预分频系数
void IWDG_Init(u8 prer,u16 rlr);
void EXTIX_Init(void);     //用作处理声音传感器信号接收的外部中断
void All_init(void);

#endif
