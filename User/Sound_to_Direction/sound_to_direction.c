#include "sound_to_direction.h"
#include "sound_to_direction_need.h"
#include "contact.h"

double thlta=0;                                //计算获得的声源相对于机器人正前方的角度
int angle_decision=0;                          //决定是否在角度模式下机器人旋转
double time0_1=0,time0_2=0,time0_3=0;          //转化为毫秒后的事件
extern int time1,time2,time3;  
extern int time_s,time1_1,time2_1,time3_1;  
extern int sequence[],caculate_once;

void area_decision(void)
{
	 if(sequence[0]==number_f)               //前方传感器先接收到信号
	 {
			if(sequence[1]==number_l)
			{
				 angle_decision=1;
			}
			if(sequence[1]==number_r)
			{
				 angle_decision=2;
			}
	  }
		if(sequence[0]==number_l)              //左方传感器先接收到信号
		{
			 if(sequence[1]==number_f)
			 {
					angle_decision=3; 
			 }
			 if(sequence[1]==number_b)
			 {
					angle_decision=4;
			 }
		}
		if(sequence[0]==number_r)              //右方传感器先接收到信号
		{
			 if(sequence[1]==number_f)
			 {
				  angle_decision=5;
			 }
			 if(sequence[1]==number_b)
			 {
					angle_decision=6;
			 }
		}
		if(sequence[0]==number_b)              //后方传感器先接收到信号
		{
			 if(sequence[1]==number_l)
			 {
					angle_decision=7;	  
			 }			
			 if(sequence[1]==number_r)
			 {
					angle_decision=8;	
			 }
		}
}

void area_decision_angle(void)
{
	 if(caculate_once)
	 {
      int thlta=angle_caculate();
			caculate_once=1;   
	 }
	 if(sequence[0]==number_f)                  //前方传感器先接收到信号
	 {
			if(sequence[1]==number_l)
			{
				if(thlta>15)                          //此条件在机器人旋转时，作用是在前方十五度时机器人不进行旋转
			  {
					 angle_decision=1;
				}
				else
				{
					 angle_decision=0;
				}
			}
			if(sequence[1]==number_r)
			{
				 if(thlta>15)
				 {
						angle_decision=2;
				 }
				 else
				 {
						angle_decision=0;
				 }
			}
		}
		if(sequence[0]==number_l)                //左方传感器先接收到信号
		{
			 if(sequence[1]==number_f)
			 {
					angle_decision=3; 
			 }
			 if(sequence[1]==number_b)
			 {
					angle_decision=4;
			 }
		}
		if(sequence[0]==number_r)                //右方传感器先接收到信号
		{
			 if(sequence[1]==number_f)
			 {
					angle_decision=5;
			 }
			 if(sequence[1]==number_b)
			 {
					angle_decision=6;
			 }
		}
		if(sequence[0]==number_b)                //后方传感器先接收到信号
		{
			 if(sequence[1]==number_l)
			 {
					angle_decision=7;	  
			 }
			 if(sequence[1]==number_r)
			 {
					angle_decision=8;	
			 }
		}
}

int angle_caculate(void)                      //角度计算函数
{
	double angle=0;
	time0_1=time1/need_derivation;              //将各个时间转化成秒
	time0_2=time2/need_derivation;
	time0_3=time3/need_derivation;
	
	angle=atan(fabs(time0_1-time0_2)/time0_3);  //角度近似计算公式
	angle=(int)(angle*180/pi);                  //将角度以弧度形式转换为角度形式
	
	return angle;
}

void sound_to_direction_area(void)         //机器人区域旋转函数，功能仅仅是判断区域然后进行旋转
{
	 area_decision();
	 switch(angle_decision)                   //测试阶段没有码盘返回数据，需要重置的标志位在以下重置，具体角度还需要考虑一下
	 {
			case 1:set_stop();break;   
			case 2:set_stop();break;
			case 3:Set_CarTurn(8500,11500,67);break;
			case 4:Set_CarTurn(8500,11500,112);break;
			case 5:Set_CarTurn(11500,8500,67);break;
			case 6:Set_CarTurn(11500,8500,112);break;
			case 7:Set_CarTurn(8500,11500,157);break;
			case 8:Set_CarTurn(11500,8500,157);break;
			case 0:set_stop();break;
	 }

}

void sound_to_direction_angle(void)           //函数功能仅仅是按照计算角度进行旋转
{
	  area_decision_angle();
		switch(angle_decision)                   //测试阶段没有码盘返回数据，需要重置的标志位在以下重置
		{
			 case 1:Set_CarTurn(8500,11500,(int)thlta);break;
			 case 2:Set_CarTurn(11500,8500,(int)thlta);break;
			 case 3:Set_CarTurn(8500,11500,(90-(int)thlta));break;
			 case 4:Set_CarTurn(8500,11500,(90+(int)thlta));break;
			 case 5:Set_CarTurn(11500,8500,(90-(int)thlta));break;
			 case 6:Set_CarTurn(11500,8500,(90+(int)thlta));break;
			 case 7:Set_CarTurn(8500,11500,(180-(int)thlta));break;
			 case 8:Set_CarTurn(11500,8500,(180-(int)thlta));break;
			 case 0:Set_carStop();break;
		}
	} 
