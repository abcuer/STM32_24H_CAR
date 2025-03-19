#include "stm32f10x.h" 
#include "global.h"

int16_t Encoder_countA = 0;
int16_t Encoder_countB = 0;
int16_t last_Encoder_countA = 0;
int16_t last_Encoder_countB = 0;

pid_t motorA;
pid_t motorB;
pid_t angle;
pid_t trackLIne;

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


void speed_pid_control(void)
{
	Encoder_countA = EncoderA_Get();
	Encoder_countB = EncoderB_Get();
	Encoder_countA = Encoder_countA * 0.4 + last_Encoder_countA * 0.6;
	Encoder_countB = Encoder_countB * 0.4 + last_Encoder_countB * 0.6;
	last_Encoder_countA = Encoder_countA;
	last_Encoder_countB = Encoder_countB;
	if(motorA_dir) motorA.now = Encoder_countA;				  else motorA.now = -Encoder_countA;
	if(motorB_dir) motorB.now = Encoder_countB;					else motorB.now = -Encoder_countB;
	A = Encoder_countA;
	B = Encoder_countB;
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
	pid->error[0] =(pid->target - pid->now);

	
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
}

/**
 * @brief  指数加权移动平均滤波
 * @param  input 输入值
 * @param  filtered_value 上次滤波后的值
 * @param  alpha 滤波系数
 * @retval 滤波后结果
 */
float EWMA_filter(float input, float filtered_value, float alpha)
{
    return alpha * input + (1.0 - alpha) * filtered_value;
}

