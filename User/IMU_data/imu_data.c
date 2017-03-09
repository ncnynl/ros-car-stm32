#include "imu_data.h"

int once_delay=1;
float acc[3],w[3],angle[3],T;    //加速度，角速度，角度，温度
extern unsigned char Re_buf[11],temp_buf[11],counter;
extern unsigned char sign;       //串口接收数据后开启换算过程的标志位
static unsigned char Temp[11];   //数据缓存

void imu_data(void)              //陀螺仪的优先级要高于计算脉冲的优先级；静止的时候陀螺仪的准确度大于运动时，是因为抖动
{
//	 if(once_delay)
//	 {
//		 delay_ms(200);
//		 once_delay=0;
//	 }
	
	 if(sign)
   {  
      memcpy(Temp,Re_buf,11);
      sign=0;
      if(Re_buf[0]==0x55)       //检查帧头
      {  
        switch(Re_buf[1])
        {
           case 0x51: //标识这个包是加速度包
              acc[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16*9.8;      //X轴加速度
              acc[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16*9.8;      //Y轴加速度
              acc[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16*9.8;      //Z轴加速度
              T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //温度
              break;
           case 0x52: //标识这个包是角速度包
              w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
              w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
              w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
              T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //温度
              break;
           case 0x53: //标识这个包是角度包
              angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
              angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
              angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
              T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //温度

              //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
              break;
              default:  break;
        }
			//printf("X角度：%.2f  Y角度：%.2f  Z角度：%.2f  X速度：%.2f  Y速度：%.2f  Z速度：%.2f\r\n",angle[0],angle[1],angle[2],w[0],w[1],w[2]);
			 //printf("%.2f %.2f %.2f\r\n",acc[0],acc[1],acc[2]);
     }        
  }
//  delay_ms(50);
			
}
