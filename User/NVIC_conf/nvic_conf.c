#include "stm32f10x.h"
#include "nvic_conf.h"

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;	
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//将中断矢量放到Flash的0地址
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级配置的模式,第1组:抢占优先级0(0:7),抢占优先级1(0:7),

    /****************************使能串口1中断，并设置优先级***********************/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;					   //USART1全局中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //先占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	           //从占优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  		 			   //使能中断
    NVIC_Init(&NVIC_InitStructure);	     	
		  	
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;                //B(右)码盘中断函数，在encoder.c中定义使用
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel =TIM4_IRQn;                //A(左)码盘中断函数，在encoder.c中定义使用
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
                                                        
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}	
