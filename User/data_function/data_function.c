#include "data_function.h"
#include "stm32f10x.h"
#include "UltrasonicWave.h"
#include "stdbool.h"

//码盘数据函数内变量
extern float Sum_L_Motor,Sum_R_Motor,Milemeter_L_Motor,Milemeter_R_Motor;
extern int Flag_milemeter;

//超声数据函数变量，超声波距离：10-110CM ；5v供电
int Ultra_dis[6]={0};  //存储前面6个超声的距离
int Ulta_number=0;
int ultra_distance=0;  //存储后面超声传感器测得的距离值

float encoder_data_left(void)
{
	if(Flag_milemeter==1)
	{
		Flag_milemeter=0;			
		Sum_L_Motor+=Milemeter_L_Motor;   //溢出情况需要进一步考虑处理
  }
	return Sum_L_Motor;
}

float encoder_data_right(void)
{
	if(Flag_milemeter==1)
	{
		Flag_milemeter=0;
		Sum_R_Motor+=Milemeter_R_Motor;		
  }
	return Sum_R_Motor;
}

// void ultrasound_data_front(isOnoff)
//{
//	if(isOnoff==true)
//	{
//		int k=0;
//		for(k=0;k<6;k++)
//		{
//			Ultra_dis[k]=	UltrasonicWave_StartMeasure(Ulta_number);
//			Ulta_number++;
//		}
//		k=0;
//		Ulta_number=0;
//		//return Ultra_dis;
//		//return Ultra_dis[re_number];
//	}

//}

int ultrasound_data_back()
{

	ultra_distance=UltrasonicWave_Measure_back();
	return ultra_distance;
}



