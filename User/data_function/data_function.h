#ifdef __DATA_FUNCTION_H
#define __DATA_FUNCTION_H

float encoder_data_left(void);   //做码盘数据
float encoder_data_right(void);  //右码盘数据
void ultrasound_data_front(bool isOn);  //前面六个超声波数据，参数表示从左到右的序号

int ultrasound_data_back(void);  //后面超声波数据

#endif
