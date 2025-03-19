#include "stm32f10x.h"                  // Device header
#include "global.h"

uint8_t Key1 = 0;	//按键1 Task
uint8_t Key2 = 0; //按键2 start_flag

int16_t Encoder_countA = 0;
int16_t Encoder_countB = 0;

uint16_t Task = 0;
uint16_t first_flag = 0;
uint16_t start_flag = 0;

// 角度环p，i，d
float p = 0.28;
float	i = 0.0;
float d = 0.0;

int main(void)
{
	Delay_ms(200);
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

	pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
	pid_Init(&motorB, DELTA_PID, 0.1435, 0.052, 0);	//pid_Init(&motorA, DELTA_PID, 0.4, 0.08, 2.15);
	pid_Init(&angle, POSITION_PID, p, i, d);
	
	while(1)
	{
		//motor_target_set(65,65);	
		//angle_correct(0);
		if(start_flag == 1)
		{
			if(first_flag == 1)
			{
				if(Task == 1) Task_1();
				if(Task == 2) Task_2();
				if(Task == 3) Task_3();
			}	
		}
	}
}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		// 速度环控制
		speed_pid_control();
		
		// 航向角获取
		Yaw_detect();
		
		// 任务控制
		Key1 = Key_GetNum1();
		Key2 = Key_GetNum2();
		if(Key1 == 1) Task ++;
		if(Key2 == 1) start_flag = 1;
		if(Task > 4) Task = 0;
		
		// 初始化
		initialize();

		UpdateSoundLight();
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}


