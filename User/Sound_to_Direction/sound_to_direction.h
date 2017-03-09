#ifndef __SOUND_TO_DIRECTION_H
#define __SOUND_TO_DIRECTION_H

#include "math.h"
#include <stdio.h>

#define pi 3.1415926

void sound_to_direction_angle(void);   //角度听声辩位执行函数
void sound_to_direction_area(void);    //区域听声辩位执行函数
int angle_caculate(void);              //角度计算函数
void area_decision_area(void);         //声音相对于机器人的区域分辨函数，区域旋转使用
void area_decision_angle(void);        //声音相对于机器人的区域分辨函数，角度旋转使用

#endif
