#include "contact.h"

/***********************************************  输出  *****************************************************************/

/***********************************************  输入  *****************************************************************/

extern struct PID Control_left;//左轮PID参数，适于新电机4096
extern struct PID Control_right;//右轮PID参数，适于新电机4096

/***********************************************  变量  *****************************************************************/

/*******************************************************************************************************************/

void LeftMovingSpeedW(unsigned int val)//左轮方向和速度控制函数
{     
    if(val>10000)
    {  
        GPIO_SetBits(GPIOC, GPIO_Pin_6);	
        GPIO_ResetBits(GPIOC, GPIO_Pin_7);	
        
        Control_left.OwenValue=(val-10000);//PID调节的目标编码数			
    }
    else if(val<10000)
    {  
        GPIO_SetBits(GPIOC, GPIO_Pin_7);	
        GPIO_ResetBits(GPIOC, GPIO_Pin_6);	
        
        Control_left.OwenValue=(10000-val);//PID调节的目标编码数	 
    }	
    else
    {
         GPIO_SetBits(GPIOC, GPIO_Pin_6);	
         GPIO_SetBits(GPIOC, GPIO_Pin_7);
        
         Control_left.OwenValue=0;//PID调节的目标编码数
    }					
}

void RightMovingSpeedW(unsigned int val2)//右轮方向和速度控制函数
{    
    if(val2>10000)
    {  
        /* motor A 正转*/
        GPIO_SetBits(GPIOC, GPIO_Pin_10);	
        GPIO_ResetBits(GPIOC, GPIO_Pin_11); 
        
        Control_right.OwenValue=(val2-10000);//PID调节的目标编码数
    }
    else if(val2<10000)
    {  
        /* motor A 反转*/
        GPIO_SetBits(GPIOC, GPIO_Pin_11);	
        GPIO_ResetBits(GPIOC, GPIO_Pin_10);	
        
        Control_right.OwenValue=(10000-val2);//PID调节的目标编码数	 
    }	
    else
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_10);	
        GPIO_SetBits(GPIOC, GPIO_Pin_11);
        
        Control_right.OwenValue=0;//PID调节的目标编码数
    }												
}

void car_control(float rightspeed,float leftspeed)//小车速度转化和控制函数
{
    float k2=17.179;         //速度转换比例,转/分钟	
    
    //将从串口接收到的速度转换成实际控制小车的速度？还是PWM？
    int right_speed=(int)k2*rightspeed;
    int left_speed=(int)k2*leftspeed;
    
    RightMovingSpeedW(right_speed+10000);
    LeftMovingSpeedW(left_speed+10000);
}

//void Contact_Init(void)//左右轮方向和速度初始化
//{
//	LeftMovingSpeedW(12000); //电机B
//	RightMovingSpeedW(12000);//电机A	
//}

