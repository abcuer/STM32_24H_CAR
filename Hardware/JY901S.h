#ifndef __JY901S_H
#define __JY901S_H

#include "stm32f10x.h" 

void jy901s_ReceiveData(uint8_t RxData);

extern float Roll,Pitch,Yaw;

#endif
