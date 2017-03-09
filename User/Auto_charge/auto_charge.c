#include "auto_charge.h"
#include "stdio.h"

extern bool Red_Left,Red_Front_Middle,Red_Front_Right,Red_Front_Left,Red_Right;   //存储红外检测信号
int check[3]={100,100,100};  //用作后面超声波数据监测
int checkup[3]={100,100,100};
int i=2;  //超声波数据监测矩阵参数
int Flag_Ultra_charge=1;  //充电功能是否停止标志位
int Ultra_charge=100; //存储后面超声波获得数据
int Red_flag_rr=1,Red_flag_ll=1,Red_flag_r=0,Red_flag_l=0,Red_flag_stop=1;  //自动充电过程中的各个标志位

int ultrasound_data_back(void);  //后面超声波数据函数声明，又返回值得函数需要调用前另行声明


void auto_charge(void)
{
	  if(Flag_Ultra_charge)  //用作最后充电的停止条件
		{
    	//	Ultra_charge=	ultrasound_data_back();
			
// 	    printf("%d\r\n",ultrasound_data_back());
			
			check[i]=Ultra_charge;
			i--;
			if(i==-1)
			{	
				i=2;
				checkup[1]=check[1];
				checkup[2]=check[2];
				checkup[0]=check[0];
 //       Flag_Ultra_charge=0;				
			}
			
		}
		
		if((checkup[0]<7)&&(checkup[1]<7)&&(checkup[2]<7)) //连续三个数据均小于9mm时，机器人停止
		{
//			printf("tingzhi\r\n");
			Set_carStop();
			Red_flag_stop=0;       //机器人停止标志位  
			Flag_Ultra_charge=0;
		}
		else
		{
			Red_flag_stop=1;
			Flag_Ultra_charge=1;			
		}
		
	  if(Red_flag_stop)  
		{

			if(Red_Right&&Red_flag_rr)    //当右侧传感器接收到信号时，置1相应标志位
			{	
				Red_flag_r=1;
			}
			
			if(Red_Left&&Red_flag_ll)   //当左侧传感器接收到信号时，置1相应标志位
			{
				Red_flag_l=1;
			}
		
			if(Red_flag_r||Red_flag_l)   //当左侧有信号接收时
			{
				 if(Red_flag_r&&Red_flag_l)     //处理左右两侧连续有信号接收的情况
				 {
					  if(Red_Right) 
					  {
						   Red_flag_l=0;
					  }
					  else 
						Red_flag_r=0;		
				 }
			
				 if(Red_flag_r)    //右侧接收到信号后，左转，当后面三个接收器均有信号时执行
				 {		
					  Set_carRight(1000);
					  if(Red_flag_r&&Red_Front_Middle&&Red_Front_Right&&Red_Front_Left)
					  {	
						   Red_flag_r=0;
						   Red_flag_rr=0;
						   Red_flag_ll=0;
						   Set_carBack(1000);
					  }
											
				 }
				 else	          //否则，右转，当后面三个有信号接收时直行
				 {		
					  Set_carLeft(1000);
					  if(Red_flag_l&&Red_Front_Middle&&Red_Front_Left&&Red_Front_Right)
					  {
						   Red_flag_l=0;
						   Red_flag_rr=0;
						   Red_flag_ll=0;                         
						   Set_carBack(1000);
					  }		
				 }
			 }	
			 else if((!Red_flag_rr)&&(!Red_flag_ll))
			 {
					if(Red_Front_Left&&Red_Front_Middle&&Red_Front_Right)
					{
							Set_carBack(1000);
					}
					if((!Red_Front_Left)&&Red_Front_Middle&&(!Red_Front_Right))
					{
							Set_carBack(1000);
					}
					if((!Red_Front_Left)&&Red_Front_Middle&&Red_Front_Right)
					{						
							Set_carRight(1000);
					}
					if(Red_Front_Left&&Red_Front_Middle&&(!Red_Front_Right))
					{
							Set_carLeft(1000);
					}
			 }
			 else
			 {
				  Set_carFoward(1000);
				 
			 }

		 }		
		 else   
				 Set_carStop();		
}
