#include "stm32f10x.h"                  // Device header
#include "global.h"

//#define GZ_OFFSETS_SIZE 4
//static const int GZ_OFFSETS[GZ_OFFSETS_SIZE] = {-11,-12,-13,-14};



int16_t AX,AY,AZ,GX,GY,GZ;
float yaw_gyro,current_yaw ;
int16_t baise_time1 = 0;
int16_t speed_err;
float offset = 0;

uint16_t SoundLight_flag = 0;
uint16_t SoundLight_time = 0;

float angle_dir = 0;

float getAverageYaw(int num_samples, int delay_ms) 
{
    float sum = 0;
    for (int i = 0; i < num_samples; i++) 
		{
        Delay_ms(delay_ms);
        sum += yaw_gyro;
    }
    return sum / num_samples;
}

void initialize(void) 
{
    if (start_flag == 1 && first_flag == 0) 
		{
        LED_Blue_ON();
        Delay_ms(1000);
        LED_Blue_OFF();
        LED_Green_ON();
        yaw_gyro = 0;
        angle_initial = getAverageYaw(3, 200);
        first_flag = 1;
        LED_Green_OFF();
        Delay_ms(200);
    }
}
 
//void Yaw_detect(void) {
//    //int16_t GX, GY, GZ; // 陀螺仪读数

//    // 获取陀螺仪数据
//	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

//    // 偏移校正示例 (假设您有GZ_OFFSETS数组)
//    for (int i = 0; i < GZ_OFFSETS_SIZE; i++) {
//        if (GZ == GZ_OFFSETS[i]) {
//            GZ += -GZ_OFFSETS[i]; // 校正GZ的值
//            break;
//        }
//    }
		
//		if( GZ > -100 && GZ < 100 ) GZ = 0;

//    // 更新陀螺仪的Yaw角计算
//    // 使用分母进行单位转换（假设单位是度/秒）
//    yaw_gyro += (float)(GZ / 5.22)  * 0.0065;
//		
//    // 互补滤波以结合陀螺仪和加速度计的数据
////    float alpha = 0.8; // 选择合适的滤波系数
////    float accel_yaw = atan2(AY, AZ) * 180 / 3.14; // 假设AY和AZ已经设置
////    current_yaw = alpha * (current_yaw + (yaw_gyro * 0.0065)) + (1 - alpha) * accel_yaw;

////    // 更新yaw_gyro为当前值
////    yaw_gyro = current_yaw;
//}


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

// ʹ�õõ��ĽǶ��� -180 �� +180֮�� 
void normalize_angle(float *angle)
{
    *angle = fmod(*angle + 180, 360);
    if(*angle < 0) *angle += 360;
    *angle -= 180;
}

/* ʹ�õõ��ĽǶ��� 0 �� 360 ֮�� */
//void normalize_angle(float *angle)
//{
//    *angle = fmod(*angle, 360);
//    if(*angle < 0) *angle += 360;
//}

// �ǶȽ�������
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
void angle_pid_control(float ang)
{
	angle.target = ang;
	tar = ang;
	if(angle.target < -180 || angle.target > 180) normalize_angle(&angle.target);
	if(ang < 0) angle_dir = 0;
	else if(ang > 0) angle_dir = 1;
	angle.now = yaw_gyro;
	angle_now = angle.now;
	offset = angle.now - angle.target;
	if(offset <= 0.0) offset = -offset; // Line_flag == 0
	if(angle_flag == 1 && offset >= 0.5)
	{
		LED_Green_ON();
		pid_cal(&angle);
		pidout_limit(&angle);
		motor_target_set(+ angle.out , - angle.out);
		if(Line_flag == 1)
		{
			LED_Green_OFF();
			angle_flag = 0;
			return;
		}
	}
}		

//		if(offset < 0.7)
//		{
//			LED_Green_OFF();
//			angle_flag = 0;
//			return;
//		}



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

