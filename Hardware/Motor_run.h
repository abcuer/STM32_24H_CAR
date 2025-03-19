#ifndef __MOTOR_RUN_H
#define __MOTOR_RUN_H

/*
左轮: Pin_14Pin_15  PWM2(PIn_2)
右轮: Pin_3   Pin_5 	PWM1(Pin_1)
*/

void Motor_Run_Init(void);
void Speed_L(uint16_t Speed_l); /*左轮调速*/
void Speed_R(uint16_t Speed_r); /*右轮调速*/
void motorA_duty(int duty);
void motorB_duty(int duty);


//void Car_Straight(void);
//void Car_Back(void);
//void Car_Left(void);
//void Car_Right(void);
//void Car_Stop(void);

#endif
