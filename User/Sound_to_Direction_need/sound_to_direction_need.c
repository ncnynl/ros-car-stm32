#include "sound_to_direction_need.h"

int sequence[]={0,0,0,0};             //存储传感器接送顺序的序列
int number=0;                         //上述序列的标号参数
int caculate_direction=0;             //是否进行角度计算标志位
int time1,time2,time3;                //第一个传感器有信号接受到第二个、第三个、第四个之间的时间间隔
int time_s,time1_1,time2_1,time3_1;   //开始计数时计数器的值，第一次、第二次、第三次外部中断触发时计数器的值
int flag_update=0;                    //前后两次声音传感器有信号接收之间计数器是否溢出标志位
int caculate_once;                    //是否计算一次控制位
extern int angle_decision;  

int jishu=0;   //测试参数

void TIM8_Init(int pre_scalar)
{
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 NVIC_InitTypeDef  NVIC_InitStructure;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
   TIM_DeInit(TIM8);
	
	 //使得arr的值尽量的大
   TIM_TimeBaseStructure.TIM_Period=65535;		 								/* 自动重装载寄存器周期的值(计数值) */
   TIM_TimeBaseStructure.TIM_Prescaler=pre_scalar;				    /* 时钟预分频数 72M/360 ,周期5us*/ 
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		  /* 采样分频 */
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  /* 向上计数模式 */
   TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	 TIM_ITConfig(TIM8, TIM_IT_Update ,ENABLE  );               //使能或者失能指定的TIM中断
   TIM_ClearFlag(TIM8, TIM_FLAG_Update);
	 TIM_ARRPreloadConfig(TIM8, ENABLE);                        //使能TIMx在ARR上的预装载寄存器

	 TIM_Cmd(TIM8, ENABLE);                                     //使能TIMx外设
	
	 NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;         //TIM8中断
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //从优先级3级
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
	 NVIC_Init(&NVIC_InitStructure);                            //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void TIM8_UP_IRQHandler(void)                                 //ITM8的溢出中断服务函数 
{
	 if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)         //检查指定的TIM中断发生与否:TIM 中断源 
	 {
	 TIM_ClearITPendingBit(TIM8, TIM_IT_Update);                //清除TIMx的中断待处理位:TIM 中断源 
	 flag_update=1;
	 }
}

//void IWDG_Init(u8 prer,u16 rlr)   //看门狗复位整个系统！！！重新考虑用法，先不使用
//{
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
//	
//	IWDG_SetPrescaler(prer);  //设置IWDG预分频值:设置IWDG预分频值为64
//	
//	IWDG_SetReload(rlr);  //设置IWDG重装载值
//	
//	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
//	
//	IWDG_Enable();  //使能IWDG
//}

//void IWDG_Feed(void)
//{   
// 	IWDG_ReloadCounter();										   
//}


EXTI_InitTypeDef EXTI_InitStructure;

void EXTIX_Init(void)
{
 	  NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);          //外部中断，需要使能AFIO时钟
	
    //GPIOE.1 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource1);


  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //下降沿触发
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	                           //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	  EXTI_ClearITPendingBit(EXTI_Line1); 

    //GPIOE.2	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	                         //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		EXTI_ClearITPendingBit(EXTI_Line2); 

    //GPIOE.3	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		                          //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
		EXTI_ClearITPendingBit(EXTI_Line3); 

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);

   	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		                          //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
	  EXTI_ClearITPendingBit(EXTI_Line4); 
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			        //使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	                            //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			        //使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
 
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			        //使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
		
		
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			        //使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		if(number==0)
		{
//			time_s=TIM_GetCounter(TIM8);	
//			flag_update=0;		
			sequence[number]=number_f;
		}
		if(number==1)
		{
//			time1_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time1=time1_1+65535-time_s;
//				flag_update=0;
//			}
//			else
//				time1=time1_1-time_s;
			//time1=time1/need_derivation;
			sequence[number]=number_f;
		}
		if(number==2)
		{
//			time2_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time2=time2_1+65535-time1_1+time1;
//				flag_update=0;
//			}
//			else
//			time2=time2_1-time1_1+time1;
			//time2=time2/need_derivation;
			sequence[number]=number_f;
		}
		if(number==3)
		{
//			time3_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time3=time3_1+65535-time2_1+time2;
//				flag_update=0;
//			}
//			else
//			time3=time3_1-time2_1+time2;			
			//time3=time3/need_derivation;
			
			sequence[number]=number_f;
			caculate_direction=1;
			caculate_once=1;
		}
		number++;
		EXTI_ClearFlag(EXTI_Line1);
		EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);
		//jishu++;
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{
		if(number==0)
		{
			
//			time_s=TIM_GetCounter(TIM8);	
//			flag_update=0;		
			sequence[number]=number_b;
		}
		if(number==1)
		{
//			time1_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time1=time1_1+65535-time_s;
//				flag_update=0;
//			}
//			else
//				time1=time1_1-time_s;
			//time1=time1/need_derivation;
			sequence[number]=number_b;
		}
		if(number==2)
		{
//			time2_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time2=time2_1+65535-time1_1+time1;
//				flag_update=0;
//			}
//			else
//				time2=time2_1-time1_1+time1;
//			//time2=time2/need_derivation;
			sequence[number]=number_b;
		}
		if(number==3)
		{
//			time3_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time3=time3_1+65535-time2_1+time2;
//				flag_update=0;
//			}
//			else
//			time3=time3_1-time2_1+time2;
			
			sequence[number]=number_b;
			caculate_direction=1;
			caculate_once=1;
		}
		number++;
		EXTI_ClearFlag(EXTI_Line2);
		EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);

	}	
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		if(number==0)
		{
//			time_s=TIM_GetCounter(TIM8);	
//			flag_update=0;		
			sequence[number]=number_l;
		}
		if(number==1)
		{
//			time1_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time1=time1_1+65535-time_s;
//				flag_update=0;
//			}
//			else
//				time1=time1_1-time_s;
			//time1=time1/need_derivation;
			sequence[number]=number_l;
		}
		if(number==2)
		{
//			time2_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time2=time2_1+65535-time1_1+time1;
//				flag_update=0;
//			}
//			else
//				time2=time2_1-time1_1+time1;
			//time2=time2/need_derivation;
			sequence[number]=number_l;
		}
		if(number==3)
		{
//			time3_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time3=time3_1+65535-time2_1+time2;
//				flag_update=0;
//			}
//			else
//			time3=time3_1-time2_1+time2;
			
			sequence[number]=number_l;
			caculate_direction=1;
			caculate_once=1;
		}
		number++;
		EXTI_ClearFlag(EXTI_Line3);
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);

	}	
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		if(number==0)
		{
		
//			time_s=TIM_GetCounter(TIM8);
//			flag_update=0;		
			sequence[number]=number_r;
		}
		if(number==1)
		{
//			time1_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time1=time1_1+65535-time_s;
//				flag_update=0;
//			}
//			else
//				time1=time1_1-time_s;
			//time1=time1/need_derivation;
			sequence[number]=number_r;
		}
		if(number==2)
		{
//			time2_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time2=time2_1+65535-time1_1+time1;
//				flag_update=0;
//			}
//			else
//				time2=time2_1-time1_1+time1;
		//	time2=time2/need_derivation;
			sequence[number]=number_r;
		}
		if(number==3)
		{
//			time3_1=TIM_GetCounter(TIM8);
//			if(flag_update)
//			{
//				time3=time3_1+65535-time2_1+time2;
//				flag_update=0;
//			}
//			else
//			time3=time3_1-time2_1+time2;
			
			sequence[number]=number_r;
			caculate_direction=1;
			caculate_once=1;

		}
		number++;
		EXTI_ClearFlag(EXTI_Line4);
		EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  	EXTI_Init(&EXTI_InitStructure);
		jishu++;
	}	
}

void All_init(void)                 //代替机器人旋转，测试时使用
{
		EXTI_InitTypeDef EXTI_InitStructure;
		//delay_us(20000);
	  EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
		
   	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
		
   	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
		
   	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
		
		caculate_direction=0;
		angle_decision=0;
		printf("go\r\n");
		number=0;
		memset(sequence,0,5*sizeof(int));                      //数组sequence[]全部置零
		jishu=0;
}	
