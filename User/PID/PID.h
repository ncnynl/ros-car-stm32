#ifndef __PID_H
#define __PID_H

#include "stdio.h"

struct PID
{
    float Kp;
    float Ki;
    float Kd;
    float error_0;//基波分量
    float error_1;//一次谐波分量
    float error_2;//二次谐波分量
    long  Sum_error;
    float OutputValue;//实际输出量
    float OwenValue;//零误差时的标准输出量
};

float PID_calculate( struct PID *Control,float CurrentValue);      //位置PID计算

#endif 
