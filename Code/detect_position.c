#include "stm32f10x.h"                  // Device header
#include "global.h"

int16_t baise_time1 = 0;
int16_t speed_err;

uint16_t SoundLight_flag = 0;
uint16_t SoundLight_time = 0;

void initialize(void) 
{
		LED_Blue_ON();
		Delay_ms(1000);
		LED_Blue_OFF();
		LED_Green_ON();
		float ang1 = Yaw;
		Delay_ms(200);
		float ang2 = Yaw;
		Delay_ms(200);
		float ang3 = Yaw;
		angle_initial = (ang1 + ang2 + ang3) / 3;
		first_flag = 1;
		LED_Green_OFF();
		if(angle_initial< 0) ini = - angle_initial;
		else  ini =  angle_initial;
		Delay_ms(200);
}
 
void SoundLight(void)
{
	if(SoundLight_flag == 0)
	{
		Buzzer_ON();
		LED_Blue_ON();
		SoundLight_flag = 1;
	}
}

void UpdateSoundLight(void)
{
	if(SoundLight_flag == 1)
	{
		SoundLight_time ++;
		if(SoundLight_time >= 10)
		{
				Buzzer_OFF();
				LED_Blue_OFF();
				SoundLight_flag = 0;
				SoundLight_time = 0;
		}
	}
}

// 使得得到的角度在 -180 到 +180之间 
//void normalize_angle(float *angle)
//{
//    *angle = fmod(*angle + 180, 360);
//    if(*angle < 0) *angle += 360;
//    *angle -= 180;
//}

///* 使得得到的角度在 0 到 360 之间 */
void normalize_angle(float *angle)
{
    *angle = fmod(*angle, 360);
    if(*angle < 0) *angle += 360;
}

// 角度矫正函数
//void angle_pid_control(float ang)
//{
//	angle.target = ang;
//	if(angle.target < 0 || angle.target > 360) normalize_angle(&angle.target);
//	angle.now = yaw_gyro;
//	offset = angle.now-angle.target;
//	if(offset <= 0.0) offset = -offset;
//	if(angle_flag == 1 && offset > 0.1)
//	{
//		LED_Green_ON();
//		pid_cal(&angle);
//		pidout_limit(&angle);
//		motor_target_set(angle.out , -angle.out);
//		if(offset < 0.5)
//		{
//			if(offset < 0.7)
//			{
//				LED_Green_OFF();
//				angle_flag = 0;
//				return;
//			}
//		}
//	}
//}



void Trace(void)	
{
		Get_Light_TTL();
		baise_time1 ++;
		if(L3==0&&L2==0&&L1==0&&C1==0&&R1==0&&R2==0&&R3==0)
		{
			if(baise_time1 > 2850)
			{
				Line_flag = 0;
			}
		}
		if(L3==0&&L2==0&&L1==0&&C1==1&&R1==0&&R2==0&&R3==0)											// 000 1 000
		{
			speed_err = 0;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==1&&C1==1&&R1==0&&R2==0&&R3==0)										// 001 1 000
		{
			speed_err = -4;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==1&&R1==1&&R2==0&&R3==0)							// 000 1 100
		{
			speed_err = 4;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==1&&C1==0&&R1==0&&R2==0&&R3==0)							// 001 0 000
		{
			speed_err = -5;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==0&&R1==1&&R2==0&&R3==0)							// 000 0 100
		{
			speed_err = 5;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==1&&L1==1&&C1==0&&R1==0&&R2==0&&R3==0)							// 011 0 000
		{
			speed_err = -6;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==0&&R1==1&&R2==1&&R3==0)							// 000 0 110
		{
			speed_err = 6;
			baise_time1 = 0;
		}				
		else if(L3==0&&L2==1&&L1==0&&C1==0&&R1==0&&R2==0&&R3==0)							// 010 0 000 
		{
			speed_err = -7;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==0&&R1==0&&R2==1&&R3==0)							// 000 0 010	
		{	
			speed_err =	7;
			baise_time1 = 0;
			; 
		}
		else if(L3==1&&L2==1&&L1==0&&C1==0&&R1==0&&R2==0&&R3==0)							// 110 0 000
		{
			speed_err = -8;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==0&&R1==0&&R2==1&&R3==1)							// 000 0 011
		{
			speed_err = 8;
			baise_time1 = 0;
		}
		else if(L3==1&&L2==0&&L1==0&&C1==0&&R1==0&&R2==0&&R3==0)							// 100 0 000 	
		{
			speed_err = -9;
			baise_time1 = 0;
		}
		else if(L3==0&&L2==0&&L1==0&&C1==0&&R1==0&&R2==0&&R3==1)							// 000 0 001 
		{
			speed_err = 9;
			baise_time1 = 0;
		}
		motor_speed_control(basespeed, speed_err);
}

