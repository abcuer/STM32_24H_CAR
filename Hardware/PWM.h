#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare2(uint16_t Compare);  //TIM2通道1
void PWM_SetCompare3(uint16_t Compare);  //TIM2通道2

/*
	控制占空比，控制运行速度    
	TB6612电机驱动模块   	PWM1 PWM2
*/

#endif
