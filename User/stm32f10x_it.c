#include "stm32f10x_it.h"

/***************************************************************************************************/

extern s32 hSpeed_Buffer1[],hSpeed_Buffer2[];//左右轮速度缓存数组
extern u8 main_sta;//主函数步骤执行标志位

//extern u8 bSpeed_Buffer_Index;
u8 bSpeed_Buffer_Index = 0;//缓存左右轮编码数到数组变量

//extern float Milemeter_L_Motor,Milemeter_R_Motor;      //累计电机一次运行的里程 cm		
float  Milemeter_L_Motor=0,Milemeter_R_Motor=0;//dt时间内的左右轮速度,用于里程计计算

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;   //接收状态标记	
u8 serial_rec=0x31;   //接收串口数据变量

extern float pulse;//电机A PID调节后的PWM值缓存
extern float pulse1;//电机B PID调节后的PWM值缓存

/***************************************************************************************************/

void USART1_IRQHandler(void)//串口中断函数
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //是否接受到数据
    {
		serial_rec =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
					
		if((USART_RX_STA&0x8000)==0)//接收未完成
        {
            if(USART_RX_STA&0x4000)//接收到了0x0d
            {
                if(serial_rec==0x0a)
                {
                    if((USART_RX_STA&0x3f)==8)
                    {							
                        USART_RX_STA|=0x8000;	//接收完成了 
                        main_sta|=0x04;
                        main_sta&=0xF7;
                    }
                    else
                    {
                        main_sta|=0x08;
                        main_sta&=0xFB;
                        USART_RX_STA=0;//接收错误,重新开始
                    }
                }
                else 
                {
                    main_sta|=0x08;
                    USART_RX_STA=0;//接收错误,重新开始
                }
            }
            else //还没收到0X0D
            {	
                if(serial_rec==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=serial_rec ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))
                    {
                        main_sta|=0x08;
                        USART_RX_STA=0;//接收数据错误,重新开始接收
                    }							
                }		 
            }
        }   		 
    }
}

void TIM5_IRQHandler(void)//小车速度计算定时器中断函数
{
	if ( TIM_GetITStatus(TIM5 , TIM_IT_Update) != RESET ) 
	{						      
        if (hSpeedMeas_Timebase_500us !=0)//电机编码数采集时间间隔未到
        {
            hSpeedMeas_Timebase_500us--;//开始倒数	
        }
        else    //电机编码数采集时间间隔到了
        {
            s32 wtemp2,wtemp1;
            
            hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;//恢复电机编码数采集时间间隔
            
            /************************ 1 ***************************/
            
            wtemp2 = ENC_Calc_Rot_Speed2(); //A 获取的编码数
            wtemp1 = ENC_Calc_Rot_Speed1(); //B 获取的编码数
            
//            //如果为停止指令，即左右轮速度为零，则清除速度存储器防止前后速度差太大造成小车冲转
//            if((wtemp2 == 0) && (wtemp1 == 0))
//            {
//                pulse=pulse1=0;
//            }
             
            /************************ 2 ***************************/
            
            //储存编码数（脉冲数），用于里程计计算
            Milemeter_L_Motor= (float)wtemp1; //储存脉冲数
            Milemeter_R_Motor= (float)wtemp2;
            
            main_sta|=0x02;//执行计算里程计数据步骤

            /************************ 3 ***************************/
            
            //开始缓存左右轮编码数到数组
            hSpeed_Buffer2[bSpeed_Buffer_Index] = wtemp2;
            hSpeed_Buffer1[bSpeed_Buffer_Index] = wtemp1;
            bSpeed_Buffer_Index++;//数组移位
            
            //缓存左右轮编码数到数组结束判断
            if(bSpeed_Buffer_Index >=SPEED_BUFFER_SIZE)
            {
                bSpeed_Buffer_Index=0;//缓存左右轮编码数到数组变量清零
            }
            
            /************************ 4 ***************************/
            
            ENC_Calc_Average_Speed();//计算三次电机的平均编码数
            Gain2(); //电机A转速PID调节控制 右
            Gain1(); //电机B转速PID调节控制 左
        }
        
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);//清除中断标志位    		 
	}		 
}


void TIM1_UP_IRQHandler(void)//里程计发布定时器中断函数
{
	if( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) 
	{	
		main_sta|=0x01;//执行发送里程计数据步骤
		
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);//清除中断标志位  		 
	}		 
}

/***************************************************************************************************/




