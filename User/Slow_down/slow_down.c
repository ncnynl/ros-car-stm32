#include "slow_down.h"

int distance_test=0;

int ultrasound_data_back(void);

void slow_down(void)
{
	distance_test=ultrasound_data_back();
	
	if(distance_test>30)
		Set_carFoward(1000);
	else if(distance_test>20)
		Set_carFoward(800);
	else if(distance_test>10)
		Set_carFoward(400);
	else
		Set_carStop();
}
