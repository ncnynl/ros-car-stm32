#include "tim2_5_6.h"

void TIM2_PWM_Init(void)//PWM输出初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;      
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2|GPIO_Pin_1;      // 只用到了定时器的第2、1路输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /****************信号周期0.5ms=(3599+1)/72Mhz*****************/    	
    TIM_TimeBaseStructure.TIM_Period = 3599;                   //当定时器从0计数到3599，即为3600次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = 0;	                 //设置预分频7，即为9MHz：不预0分频，即为72MHz；
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	 //设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	         //配置为PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;	                     //设置通道1的电平跳变值，输出另外一个占空比的PWM  最低500
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	                 //使能通道1 PA0
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	         //配置为PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;	                     //设置通道2的电平跳变值，输出另外一个占空比的PWM  最低500
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);	 
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);			                   // 使能TIM2重载寄存器ARR
    TIM_Cmd(TIM2, ENABLE);                                     //使能定时器2
}
/*
 * 定时器5 TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /9) * 1000 = 125us
   中断周期为 = 1/(72MHZ /9) * 2000 = 250us
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM5_Configuration(void)//速度计算定时器初始化        
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE); 
	
   /* 自动重装载寄存器周期的值(计数值) */
   TIM_TimeBaseStructure.TIM_Period=4000;  //2000hz
    // TIM_TimeBaseStructure.TIM_Period=1000;  //8000hz
    // TIM_TimeBaseStructure.TIM_Period=2000;  //4000hz

    //TIM_TimeBaseStructure.TIM_Period=800;  //10000hz的采样频率
	
   /* 累计 TIM_Period个频率后产生一个更新或者中断 */
   TIM_TimeBaseStructure.TIM_Prescaler= 8;
	
   /* 对外部时钟进行采样的时钟分频,这里没有用到 */
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
   TIM_ClearFlag(TIM5, TIM_FLAG_Update);
   TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);		
   TIM_Cmd(TIM5, ENABLE);																		    
}

void TIM1_Configuration(void)//里程计发布定时器初始化
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
   TIM_DeInit(TIM1);
	

   /* 自动重装载寄存器周期的值(计数值) */
   TIM_TimeBaseStructure.TIM_Period=500;  //  500*100us=50ms  5000*100us=500ms 
   TIM_TimeBaseStructure.TIM_Prescaler=(7200-1); //周期7200/72=100us
	
	 /* 对外部时钟进行采样的时钟分频,这里没有用到 */
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
   TIM_ClearFlag(TIM1, TIM_FLAG_Update);
   TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);		
   TIM_Cmd(TIM1, ENABLE);	
}
