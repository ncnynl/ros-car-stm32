#include "sample_function.h"
#include "stm32f10x.h"
#include "stdbool.h"
#include "UltrasonicWave.h"
#include <stdio.h>
//红外传感器采样信号接收变量,发生置高
extern bool Red_Left,Red_Front_Middle,Red_Front_Right,Red_Front_Left,Red_Right;

//碰撞传感器采样信号接收变量,发生值低
int seq_bumper[3]={0};                    //存储三个碰撞传感器的信号值


void red_sample(void)
{
	Red_Left=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15);
	Red_Front_Left=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14);
	Red_Front_Middle=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13);
	Red_Front_Right=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12);
	Red_Right=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11);
}

void bumper_sample(void)
{
	seq_bumper[0]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);     //左
	seq_bumper[1]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);    //前
	seq_bumper[2]=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);    //右
	
	//printf(" %d %d %d\r\n",seq_bumper[0],seq_bumper[1],seq_bumper[2]);
}

