/**********************************************************************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：野火STM32开发板
 * 硬件连接：------------------
 *          |      - TRIG      |
 *          |      - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
*********************************************************************************/
#include "UltrasonicWave.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"


u16 time6_aa = 0,length=1300;
u8 flag_0 = 0,flag_1 = 0,flag_2 = 0,flag_3 = 0,flag_4=0,flag_5=0,flag_back_ultra=0;


unsigned short int UltrasonicWave_Distance;      //计算出的距离    

/*
 * 函数名：DelayTime_us
 * 描述  ：1us延时函数
 * 输入  ：Time   	延时的时间 US
 * 输出  ：无	
 */
void DelayTime_us(int Time)    
{
   unsigned char i;
   for ( ; Time>0; Time--)
     for ( i = 0; i < 72; i++ );
}

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */

void UltrasonicWave_Configuration(void) //需要7组收发引脚，后面一个，前面从左到右共6个
{
  GPIO_InitTypeDef GPIO_InitStructure;	
       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	//控制自动充电停止的超声 ,也可做避障使用
	/************************************************************************/  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					       //PD0接TRIG    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	        

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				         //PD1接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);			
	
	
	//以下为底盘从左到右的超声	
	/************************************************************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					 //PD2接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     //PD3接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOD
	
	
  /************************************************************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					 //PD8接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				     //PD9接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOD
	
	/************************************************************************/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					 //PD10接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO D

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //PD11接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOD
	
	/************************************************************************/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //PD12接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				     //PD13接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOD
	
	/************************************************************************/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;					 //PD14接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				     //PD15接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOC

	/************************************************************************/  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //PD4接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOD, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				     //PD7接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(GPIOD,&GPIO_InitStructure);						 //初始化GPIOC
	
}

int UltrasonicWave_CalculateTime(void)  //获得单位是mm
{
  UltrasonicWave_Distance=(int)(TIM_GetCounter(TIM6)*5*34/2000);
	return UltrasonicWave_Distance;
}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
int UltrasonicWave_StartMeasure(int number)
{
	int Distance=0;
	switch(number)
	{
		 case 0:
				GPIO_SetBits(GPIOD,GPIO_Pin_2); 		  //送>10us的高电平
				DelayTime_us(10);		                      //延时10us
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
							
				while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3));	             //等待高电平
				TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
				while(flag_0==0)
				{
					 if(TIM_GetCounter(TIM6)>length)
					 {
							TIM_Cmd(TIM6, DISABLE);
							Distance=UltrasonicWave_CalculateTime();
							TIM_SetCounter(TIM6,0);	
							flag_0=1;
						}
						else
						{
							 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) == RESET)	                 //等待低电平
							 {
									TIM_Cmd(TIM6, DISABLE);	//定时器2失能
									Distance=UltrasonicWave_CalculateTime();	//计算距离
									TIM_SetCounter(TIM6,0);
									flag_0=1;
								}
						}		 	
				 }
				 flag_0=0;
				 break;
			
			case 1:
				 GPIO_SetBits(GPIOD,GPIO_Pin_8); 		  //送>10US的高电平
				 DelayTime_us(10);		                      //延时20US
				 GPIO_ResetBits(GPIOD,GPIO_Pin_8);
							
				 while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9));	             //等待高电平
				 TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
				 while(flag_1==0)
				 {
						if(TIM_GetCounter(TIM6)>length)
						{
								TIM_Cmd(TIM6, DISABLE);
								Distance=UltrasonicWave_CalculateTime();
								TIM_SetCounter(TIM6,0);	
								flag_1=1;
						 }
						 else
						 {
								if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9) == RESET)	                 //等待低电平
								{
									 TIM_Cmd(TIM6, DISABLE);	//定时器2失能
									 Distance=UltrasonicWave_CalculateTime();	//计算距离
									 TIM_SetCounter(TIM6,0);
									 flag_1=1;
								 }
							}		 	
					}
					flag_1=0;
					break;
			case 2:
					GPIO_SetBits(GPIOD,GPIO_Pin_10); 		  //送>10US的高电平
					DelayTime_us(10);		                      //延时20US
					GPIO_ResetBits(GPIOD,GPIO_Pin_10);
							
					while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11));	             //等待高电平
					TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
					while(flag_2==0)
					{
							if(TIM_GetCounter(TIM6)>length)
							{
							   TIM_Cmd(TIM6, DISABLE);
							   Distance=UltrasonicWave_CalculateTime();
							   TIM_SetCounter(TIM6,0);	
								 flag_2=1;
							}
						  else
							{
							    if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == RESET)	                 //等待低电平
									{
									   TIM_Cmd(TIM6, DISABLE);	//定时器2失能
									   Distance=UltrasonicWave_CalculateTime();	//计算距离
										 TIM_SetCounter(TIM6,0);
										 flag_2=1;
									}
							}		 	
					}
					flag_2=0;
					break;
			case 3:
					GPIO_SetBits(GPIOD,GPIO_Pin_12); 		  //送>10US的高电平
					DelayTime_us(10);		                      //延时20US
					GPIO_ResetBits(GPIOD,GPIO_Pin_12);
							
					while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13));	             //等待高电平
					TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
					while(flag_3==0)
					{
						  if(TIM_GetCounter(TIM6)>length)
							{
								  TIM_Cmd(TIM6, DISABLE);
									Distance=UltrasonicWave_CalculateTime();
									TIM_SetCounter(TIM6,0);	
									flag_3=1;
							}
							else
							{
								 if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) == RESET)	                 //等待低电平
								 {
										TIM_Cmd(TIM6, DISABLE);	//定时器2失能
										Distance=UltrasonicWave_CalculateTime();	//计算距离
										TIM_SetCounter(TIM6,0);
										flag_3=1;
								 }
							}		 	
				    }
					  flag_3=0;
					  break;
			 	case 4:
						GPIO_SetBits(GPIOD,GPIO_Pin_14); 		  //送>10US的高电平
						DelayTime_us(10);		                      //延时20US
						GPIO_ResetBits(GPIOD,GPIO_Pin_14);
							
						while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15));	             //等待高电平
						TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
						while(flag_4==0)
						{
							if(TIM_GetCounter(TIM6)>length)
							{
								TIM_Cmd(TIM6, DISABLE);
								Distance=UltrasonicWave_CalculateTime();
								TIM_SetCounter(TIM6,0);	
								flag_4=1;
							}
							else
							{
								if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) == RESET)	                 //等待低电平
								{
									TIM_Cmd(TIM6, DISABLE);	//定时器2失能
									Distance=UltrasonicWave_CalculateTime();	//计算距离
									TIM_SetCounter(TIM6,0);
									flag_4=1;
								}
							}		 	
						}
						flag_4=0;
						break;
			 case 5:
						GPIO_SetBits(GPIOD,GPIO_Pin_4); 		  //送>10US的高电平
						DelayTime_us(10);		                      //延时20US
						GPIO_ResetBits(GPIOD,GPIO_Pin_4);
							
						while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7));	             //等待高电平
						TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
						while(flag_5==0)
						{
							if(TIM_GetCounter(TIM6)>length)
							{
								TIM_Cmd(TIM6, DISABLE);
								Distance=UltrasonicWave_CalculateTime();
								TIM_SetCounter(TIM6,0);	
								flag_5=1;
							}
							else
							{
								if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7) == RESET)	                 //等待低电平
								{
									TIM_Cmd(TIM6, DISABLE);	//定时器2失能
									Distance=UltrasonicWave_CalculateTime();	//计算距离
									TIM_SetCounter(TIM6,0);
									flag_5=1;
								}
							}		 	
						}
						flag_5=0;
						break;
		}	
		return Distance;	
}

int UltrasonicWave_Measure_back(void)
{
	int Distance=0;
  GPIO_SetBits(GPIOD,GPIO_Pin_0); 		  //送>10US的高电平
	DelayTime_us(15);		                      //延时20US
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);
							
	while(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1));	             //等待高电平
	TIM_Cmd(TIM6, ENABLE);                                             //开启时钟
							
	while(flag_back_ultra==0)
	{
	  if(TIM_GetCounter(TIM6)>length)
	  {
	    TIM_Cmd(TIM6, DISABLE);
	  	Distance=UltrasonicWave_CalculateTime();
	  	TIM_SetCounter(TIM6,0);	
			flag_back_ultra=1;
		}
		else
		{
			if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1) == RESET)	                 //等待低电平
			{
				TIM_Cmd(TIM6, DISABLE);	//定时器2失能
				Distance=UltrasonicWave_CalculateTime();	//计算距离
				TIM_SetCounter(TIM6,0);
				flag_back_ultra=1;
			}
		}		 	
	}
	flag_back_ultra=0;	
	return Distance;
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
