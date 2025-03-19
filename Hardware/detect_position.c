#include "stm32f10x.h"                  // Device header
#include "global.h"

uint16_t baise_time1 = 0;
int16_t speed_err;
float offset = 0;

void SoundLight(void)
{
	if(SoundLight_flag == 0)
	{
		Buzzer_ON();
		LED_Blue_ON();
		SoundLight_flag = 1;
	}
}

// 使得得到的角度在 -180 到 +180之间 
void normalize_angle(float *angle)
{
    *angle = fmod(*angle + 180, 360);
    if(*angle < 0) *angle += 360;
    *angle -= 180;
}

/* 使得得到的角度在 0 到 360 之间 */
//void normalize_angle(float *angle)
//{
//    *angle = fmod(*angle, 360);
//    if(*angle < 0) *angle += 360;
//}

void angle_detect(void)
{
	angle.target = angle_record1;
	// 使得 angle.target 位于 指定范围
	normalize_angle(&angle.target);
	angle.now = yaw_Kalman;
	offset = angle.now-angle.target;
	if(offset <= 0.0) offset = -offset;
	if(offset > 1.0)
	{
		LED_Green_ON();
		pid_cal(&angle);
//		if(angle.out >= 0)
//		{
//			if(angle.out > 65) angle.out = 65;
//		}
//		else 
//		{
//			if(angle.out < -65) angle.out = -65; 
//		}
		motor_target_set(angle.out , -angle.out);
	}
	else
	{
		LED_Green_OFF();
		motor_target_set(0, 0);
		angle_flag = 0;
	}
}

void angle_detect_01(void)
{
	angle.target = angle_record1;
	if(angle.target < -180 || angle.target > 180) normalize_angle(&angle.target);
	angle.now = yaw_Kalman;
	offset = angle.now-angle.target;
	if(offset <= 0.0) offset = -offset;
	if(angle_flag == 1 && offset > 0.5)
	{
		LED_Green_ON();
		pid_cal(&angle);
		motor_target_set(-angle.out , angle.out);
		if(offset < 2)
		{
			LED_Green_OFF();
			angle_flag = 0;
			return;
		}
	}
}
//		if(angle.out >= 0)
//		{
//			if(angle.out > 65) angle.out = 65;
//		}
//		else 
//		{
//			if(angle.out < -65) angle.out = -65; 
//		}
void duanlu(void) // 角度环控制
{
	if(angle.target>=0)
	{
		if(angle.target<=yaw_Kalman) yaw_Kalman = angle.target;
	}
	else
	{
		if(angle.target>=yaw_Kalman) yaw_Kalman = angle.target;
	}
	angle.now = yaw_Kalman;
	pid_cal(&angle);
	float offset = 	angle.target - angle.now;
	if(offset<0) offset = -offset;
	if(offset>0 && offset<1)	motor_target_set(basespeed, basespeed);
	float Speed1 = basespeed + angle.out;
	float Speed2 = basespeed - angle.out;
	if(Speed1 < 0) Speed1 = 0;
	if(Speed1 < 0) Speed2 = 0;
	if(angle.target>=0) 
	{	
		if(angle.now<=angle.target)
		{
			motor_target_set(Speed1, Speed2);		
		}
		else if(angle.now> angle.target)
		{
			motor_target_set(Speed2, Speed1);
		}
	}
	else if(angle.target<0)
	{
		if(angle.now > angle.target)
		{
			motor_target_set(Speed1, Speed2);	
		}
		else if(angle.now < angle.target)
		{
			motor_target_set(Speed2, Speed1);
		}
	} 	
	Get_Light_TTL();
}

void Trace(void)	
{
		Get_Light_TTL();
		baise_time1 ++;
		if(L3==0&&L2==0&&L1==0&&C1==0&&R1==0&&R2==0&&R3==0)
		{
			if(baise_time1 > 2700)
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

