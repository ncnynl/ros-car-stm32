#include "gpio_conf.h"

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   /*定义一个GPIO_InitTypeDef类型的结构体*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB, ENABLE); 	/*开启GPIOA和GPIOC和GPIOE的外设时钟*/
                                                    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_11;  // 电机方向																
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	/*设置引脚速率为50MHz */   
    GPIO_Init(GPIOC, &GPIO_InitStructure);	        /*调用库函数，初始化GPIOC*/	
    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;	//碰撞检测 Left  Middle Right
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   /*设置引脚模式为通用推挽输出*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	/*设置引脚速率为50MHz */   
//    GPIO_Init(GPIOA, &GPIO_InitStructure);	        /*调用库函数，初始化GPIOC*/
//        
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_14;	//红外检测
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   /*设置引脚模式为通用推挽输出*/
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	/*设置引脚速率为50MHz */   
//    GPIO_Init(GPIOE, &GPIO_InitStructure);	        /*调用库函数，初始化GPIOC*/	
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//    GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高
    
}
	

