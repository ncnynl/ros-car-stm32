#include "follow_wall_left.h"

int distance_left=0,distance_front=0;  //左侧和前方超声波传感器试试距离值

int ultrasound_data_front(int);        //有返回值的函数使用前需要再次声明？

void follow_wall_left(void)
{
	distance_left=ultrasound_data_front(0);
	distance_front=ultrasound_data_front(2);
	if((distance_left<distance_max)&&distance_left>distance_min)
	{
		if(distance_front<distance_max)
			Set_carRight(1000);
		else
			Set_carFoward(1000);
	}
	else if(distance_left>=distance_max)
	{
		if(distance_front<distance_max)
			Set_carRight(1000);
		else
			Set_carFoward(1000);
	}
	else 
	{
		if(distance_front<distance_max)
			Set_carRight(1000);
		else
			Set_carFoward(1000);
	}
}
