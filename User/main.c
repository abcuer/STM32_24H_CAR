#include "stm32f10x.h"                  // Device header
#include "global.h"

uint8_t Key1 = 0;	//按键1 Task
uint8_t Key2 = 0; //按键2 start_flag

int16_t tar_spe = 50; // 50 角度环 45循迹
float ini = 0;

uint16_t Task = 0;
uint16_t first_flag = 0;
uint16_t start_flag = 0;

// p，i，d
float p = 0.35;
float	i = 0.0;
float d = 0.05;

int main(void)
{	
	Delay_ms(200);
	LED_Init();
	Key_Init();
	Buzzer_Init();
	Motor_Run_Init();
	EncoderA_Init();
	EncoderB_Init();
  UART3_Init(9600);
	gray_init();
	Timer4_Init();
	
//	pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0); // 右轮
//	pid_Init(&motorB, DELTA_PID, 0.1435, 0.052, 0);	// 左轮
//	pid_Init(&motorA, DELTA_PID, 0.133, 0.1, 0.02);
//	pid_Init(&motorB, DELTA_PID, 0.135, 0.1, 0.01);
//	pid_Init(&motorA, DELTA_PID, 0.131, 0.11, 0.02);
//	pid_Init(&motorB, DELTA_PID, 0.1344, 0.11, 0.02);
//	pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
//	pid_Init(&motorB, DELTA_PID, 0.1435, 0.052, 0);
//	pid_Init(&angle, POSITION_PID, p, i, d);
//	pid_Init(&motorA, DELTA_PID, 25, 1.1, 26);
//	pid_Init(&motorA, DELTA_PID, 10, 1.7, 9.6); 75速度
//  pid_Init(&motorA, DELTA_PID, 8, 1.6, 5.5);
	pid_Init(&motorA, DELTA_PID, 8, 1.75, 5.65);  // 50速度 //45速度
	pid_Init(&motorB, DELTA_PID, 8, 1.75, 5.65);

//	pid_Init(&trackLIne, POSITION_PID, 1.45, 0, 1.5);  // 交叉循迹pid
//	pid_Init(&trackLIne, POSITION_PID, 1.45, 0, 1.5);  // 交叉循迹pid

//	pid_Init(&angle, POSITION_PID, -3.8, 0.0, -0.15);   // 角度环调参 


	while(1)
	{
		//motor_target_set(50, 50);
		//motorA_duty(100);
		//motorB_duty(100);
		
		// 任务控制
		Key1 = Key_GetNum1();
		Key2 = Key_GetNum2();
		if(Key1 == 1) start_flag = 1;
		if(Key2 == 1) Task ++;
		if(Task > 4) Task = 0;
		
		if(start_flag == 1)
		{
			if(first_flag == 1)
			{
				switch(Task)
				{
					case 1: Task_1(); break;
					case 2: Task_2(); break;
					case 3: Task_3(); break;
				}
			}	
		}
	}
}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		// 编码器获取
		speed_pid_control();
		
		//Serial_Printf("%d %d %d %d %d %d %d\r\n",L3,L2,L1,C1,R1,R2,R3);
		
		// 初始化
		if (start_flag == 1 && first_flag == 0)   initialize();
		
		UpdateSoundLight();
	
	}
}


