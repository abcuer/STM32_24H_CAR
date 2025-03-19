#include "stm32f10x.h" 
#include "pid.h"
#include "Motor_Run.h"
#include "global.h"

pid_t motorA;
pid_t motorB;
pid_t angle;

void pid_Init(pid_t *pid, uint32_t mode, float p, float i, float d)
{
                 // Device header
	pid->pid_mode = mode;
	pid->p = p;
	pid->i = i;
	pid->d = d;
}

void motor_speed_control(int Speed, int err)
{
	motor_target_set(Speed-err, Speed+err);
}

void motor_target_set(int tarA, int tarB)
{
	if(tarA >= 0) 
	{
		motorA_dir = 0;
		motorA.target = tarA;
	}
	else
	{
		motorA_dir = 1;
		motorA.target = -tarA;
	}
	if(tarB >= 0) 
	{
		motorB_dir = 0;
		motorB.target = tarB;
	}
	else
	{
		motorB_dir = 1;
		motorB.target = -tarB;
	}
}


void pid_control(void)
{
	//角度环
	// 1.设定目标角度
//	angle.target = 60;
//	// 2.获取当前角度
//	if(angle.target>=0)
//	{
//		if(angle.target<=yaw_Kalman) yaw_Kalman = angle.target;
//	}
//	else
//	{
//		if(angle.target>=yaw_Kalman) yaw_Kalman = angle.target;
//	}
//	angle.now = yaw_Kalman;
//	// 3.PID控制器计算输出
//	pid_cal(&angle);
//	//速度环
////	// 1.角度环PID输出 设定为速度环的目标值
//	if(angle.target>=0) 
//	{	
//		if(angle.now<=angle.target)
//		{
//			motor_target_set(10 + angle.out, 10 - angle.out);		
//		}
//		else if(angle.now> angle.target)
//		{
//			motor_target_set(10 - angle.out, 10 + angle.out);
//		}
//	}
//	else if(angle.target<0)
//	{
//			motor_target_set(10 + angle.out, 10 - angle.out);	
//	}	
//		else 
//		{
////			motorA_dir = 0;
////			motorB_dir = 1;
//			motor_target_set(10 + angle.out, 10 - angle.out);	
	
	// 1.根据灰度传感器设定目标值
//	track();
	// 2.获取当前速度
	if(motorA_dir) {motorA.now = Encoder_countA;}				else motorA.now = -Encoder_countA;
	if(motorB_dir) motorB.now = Encoder_countB;					else motorB.now = -Encoder_countB;
	Encoder_countA = 0;
	Encoder_countB = 0;
	// 3.输入PID控制器进行计算
	pid_cal(&motorA);
	pid_cal(&motorB);
	// 电机输出限幅
	pidout_limit(&motorA);
	pidout_limit(&motorB);	
	
	// 4.PID输出值 输入给电机
	motorA_duty(motorA.out);
	motorB_duty(motorB.out);
}


void pid_cal(pid_t *pid)
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

}

void pidout_limit(pid_t *pid)
{
	// 输出限幅
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=0)	
		pid->out=0;	
}

