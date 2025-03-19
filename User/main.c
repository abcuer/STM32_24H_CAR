#include "stm32f10x.h"                  // Device header
#include "global.h"

uint8_t Key1 = 0;	//按键1 Task
uint8_t Key2 = 0; //按键2 start_flag

int16_t Encoder_countA = 0;
int16_t Encoder_countB = 0;
int16_t AX,AY,AZ,GX,GY,GZ;

float roll_gyro, pitch_gyro, yaw_gyro; 
float roll_acc, pitch_acc, yaw_acc; 	 												 
float roll_Kalman, pitch_Kalman, yaw_Kalman;

// 任务指令 
uint16_t Task = 0;
uint16_t first_flag = 0;
uint16_t start_flag = 0;
uint16_t angle_flag = 0;
uint16_t SoundLight_flag = 0;
uint16_t SoundLight_time = 0;
uint16_t workstep = 0;
uint16_t angle_initial = 0;
uint16_t angle_record1 = 0, angle_record2 = 0, angle_record3 = 0, angle_record4 = 0;
float basespeed = 0;

float p = 0.5;
float	i = 0.0;
float d = 0.0005;

int main(void)
{
	LED_Init();
	Key_Init();
	Buzzer_Init();
	Timer4_Init();
	Motor_Run_Init();
	EncoderA_Init();
	EncoderB_Init();
	Serial_Init();
	gray_init();
	MyI2C_Init();
	MPU6050_Init();
	HMCI2C_Init();
	HMC5883L_Init();
	
	pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
	pid_Init(&motorB, DELTA_PID, 0.14, 0.052, 0);
	pid_Init(&angle, POSITION_PID, p, i, d);
	
	while(1)
	{
		Serial_Printf("yaw_Kalman:%.2f\r\n", yaw_Kalman);
		//angle_detect_01();
		if(start_flag == 1)
		{
			if(first_flag == 1)
			{
				if(Task == 1)
				{
					//LED_Green_ON();
					//SoundLight();
					angle_flag = 1;
					while(angle_flag)
					{
						angle_detect_01();
					}
					SoundLight();
					LED_Blue_ON();
					motor_target_set(0, 0);
					Task = 0;
				}
				if(Task == 2)
				{
					switch(workstep)
					{
						case 0:
							pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
							pid_Init(&motorB, DELTA_PID, 0.14, 0.052, 0);
							pid_Init(&angle, POSITION_PID, p, i, d);
							angle_record1 = angle_initial - 180;
							//angle_record1 = 90;
							workstep ++;
							break;
						
						case 1: //断路1
							SoundLight_flag = 0;
							Line_flag = 0;
							basespeed = 65;
							while(Line_flag == 0)
							{
								Get_Light_TTL();
								motor_target_set(basespeed, basespeed);
								//duanlu(); 
							}
							workstep ++;
							break;

						case 2: // 圆弧1
							SoundLight();
							Line_flag = 1;
							basespeed = 30;
							while(Line_flag == 1) 
							{	
								Get_Light_TTL();
								Trace();
							}
							workstep ++;
							break;
							
						case 3:	// 断路2
							SoundLight();
							pid_Init(&angle, POSITION_PID, p, i, d);
							angle_flag = 1;	// 开启角度矫正
							while(angle_flag == 1) angle_detect_01(); // 问题在angle_detect();
							Line_flag = 0;
							while(angle_flag == 0 && Line_flag == 0)
							{							
								Get_Light_TTL();
								motor_target_set(65, 65);
							}
							workstep ++;
							break;
							
						case 4:	// 圆弧2
							SoundLight();
							basespeed = 30;
							Line_flag = 1;
							while(Line_flag == 1)
							{		
								Get_Light_TTL();
								Trace();
							}
							workstep ++;
							break;
							
						case 5:	// 停止
							SoundLight();
							motor_target_set(0, 0);
							Task = 0, basespeed = 0, workstep = 0;;
							start_flag = 0, first_flag = 0, Line_flag = 0, angle_flag = 0;		
							angle_record1 = 0, angle_record2 = 0, angle_record3 = 0, angle_record4 = 0;
							break;
					}
				}
			}	
		}
	}
}


void TIM4_IRQHandler(void)
{
	Key1 = Key_GetNum1();
	Key2 = Key_GetNum2();
	if(Key1 == 1) Task ++;
	if(Key2 == 1) start_flag = 1; // 任务执行完毕，startflag = 0;
	if(Task > 4) Task = 0;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		HMC5883L_GetData();

		// 陀螺仪角度
		roll_gyro += (float)GX/ 16.4 * 0.005;
		pitch_gyro += (float)GY/ 16.4 * 0.005;	
		yaw_gyro += (float)GZ/ 16.4 * 0.005;	
		
		// 加速度计角度
		roll_acc = atan((float)AY/AZ) * 57.296;
		pitch_acc = - atan((float)AX/AZ) * 57.296;
		yaw_acc = atan((float)AY/AX) * 57.296;
		
		// 磁力计角度
		yaw_hmc = atan2((float)hmc_x, (float)hmc_y) * 57.296;
		
		// 卡尔曼滤波融合
		roll_Kalman = Kalman_Filter(&KF_Roll, roll_acc, (float)GX/ 16.4);
		pitch_Kalman = Kalman_Filter(&KF_Pitch, pitch_acc, (float)GY/ 16.4);
		yaw_Kalman = Kalman_Filter(&KF_Yaw, yaw_hmc, (float)GZ/ 16.4);
		
		if(start_flag == 1)
		{
			if(first_flag == 0)
			{
				LED_Blue_ON();
				Delay_ms(1000);
				LED_Blue_OFF();
				LED_Green_ON();
				Delay_ms(200);
				yaw_Kalman = 0;
				float ang1 = yaw_Kalman;
				Delay_ms(200);
				float ang2 = yaw_Kalman;
				Delay_ms(200);
				float ang3 = yaw_Kalman;
				angle_initial = (ang1 + ang2 + ang3)/3.0;
				first_flag = 1;
				LED_Green_OFF();
				Delay_ms(200);
			}
		}

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
		
		Encoder_countA = EncoderA_Get();
		Encoder_countB = EncoderB_Get();
		pid_control();
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}


