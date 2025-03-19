#include "stm32f10x.h"                  // Device header
#include "global.h"




uint16_t Line_flag = 0;
uint16_t angle_flag = 0;
uint16_t workstep = 0;
uint16_t angle_initial = 0;
uint16_t angle_record1 = 0, angle_record2 = 0, angle_record3 = 0, angle_record4 = 0;
float basespeed = 0;



void Task_1(void)
{
	switch(workstep)
	{
		case 0:
			Line_flag = 1;
			basespeed = 45;
			while(Line_flag == 1)
			{
				//track_pid_control1();
				track_pid_control2();
				Get_Light_TTL();
				//Serial_Printf("%f,%f\n", motorA.now, motorB.now);
			}
			motor_target_set(-1, 1);
			workstep ++;
			break;
		
		case 1: 
			SoundLight();
			LED_Blue_ON();
			motor_target_set(0, 0);
			Task = 0;
			workstep = 0;
			start_flag = 0;
			first_flag = 0;
			break;
	}
}

void Task_2(void)
{
	switch(workstep)
	{
		case 0:
			pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
			pid_Init(&motorB, DELTA_PID, 0.1435, 0.052, 0);
			pid_Init(&angle, POSITION_PID, p, i, d);
			angle_record1 = angle_initial - 180;
			//angle_record1 = 90;
			workstep ++;
			break;
		
		case 1: //??1
			SoundLight_flag = 0;
			Line_flag = 0;
			basespeed = 65;
			while(Line_flag == 0)
			{
				Get_Light_TTL();
				motor_target_set(basespeed, basespeed);
			}
			workstep ++;
			break;

		case 2: // ??1
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
			
		case 3:	// ??2
			SoundLight();
			pid_Init(&angle, POSITION_PID, p, i, d);
			angle_flag = 1;	// ??????
			while(angle_flag == 1) angle_pid_control(angle_record1); // ???angle_correct();
			Line_flag = 0;
			while(angle_flag == 0 && Line_flag == 0)
			{							
				Get_Light_TTL();
				motor_target_set(55, 55);
			}
			workstep ++;
			break;
			
		case 4:	// ??2
			SoundLight();
			basespeed = 30;
			Line_flag = 1;
			while(Line_flag == 1)
			{		
				Get_Light_TTL();
				Trace();
			}
			motor_target_set(-1, 1);
			workstep ++;
			break;
			
		case 5:	// ??
			SoundLight();
			angle_flag = 1;
			while(angle_flag) angle_pid_control(0);
			motor_target_set(0, 0);
			Task = 0, basespeed = 0, workstep = 0;;
			start_flag = 0, first_flag = 0, Line_flag = 0, angle_flag = 0;		
			angle_record1 = 0, angle_record2 = 0, angle_record3 = 0, angle_record4 = 0;
			break;
	}
}

void Task_3(void)
{
	switch(workstep)
	{
		case 0:
			pid_Init(&motorA, DELTA_PID, 0.1428, 0.052, 0);
			pid_Init(&motorB, DELTA_PID, 0.1435, 0.052, 0);
			pid_Init(&angle, POSITION_PID, p, i, d);
			angle_record2 = 45;
			angle_record3 = 135;
			workstep ++;
			break;
		
		case 1: //??1
			angle_flag = 1;
			while(angle_flag == 1) angle_pid_control(angle_initial + angle_record2);
			Line_flag = 0;
			while(angle_flag == 0 && Line_flag == 0)
			{							
				Get_Light_TTL();
				motor_target_set(65, 65);
			}
			workstep ++;
			break;

		case 2: // ??1
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
			
		case 3:	// ??2
			SoundLight();
			pid_Init(&angle, POSITION_PID, p, i, d);
			angle_flag = 1;	
			while(angle_flag == 1) angle_pid_control(angle_initial - angle_record3); // ??????
			Line_flag = 0;
			while(angle_flag == 0 && Line_flag == 0)
			{							
				Get_Light_TTL();
				motor_target_set(65, 65);
			}
			SoundLight();
			workstep ++;
			break;
			
		case 4:	// ??2
			SoundLight();
			basespeed = 30;
			Line_flag = 1;
			while(Line_flag == 1)
			{		
				Get_Light_TTL();
				Trace();
			}
			motor_target_set(-1, 1);
			workstep ++;
			break;
			
		case 5:	// ??
			SoundLight();
			motor_target_set(0, 0);
			Task = 0, basespeed = 0, workstep = 0;;
			start_flag = 0, first_flag = 0, Line_flag = 0, angle_flag = 0;		
			angle_record1 = 0, angle_record2 = 0, angle_record3 = 0, angle_record4 = 0;
			break;
	}

}
	
