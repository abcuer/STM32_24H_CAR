#ifndef __PID_h_
#define __PID_h_

enum
{
  POSITION_PID = 0,  // 位置式
  DELTA_PID,         // 增量式
};

typedef struct
{
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;   
	
	uint32_t pid_mode;

}pid_t;

void pid_Init(pid_t *pid, uint32_t mode, float p, float i, float d);
void motor_speed_control(int Speed, int err);
void motor_target_set(int tarA, int tarB);
//void angle_target_set(float ang_tar);
void speed_pid_control(void);
//void angle_target_set(float ang);
void angle_pid_control(float ang, float now);
void angle_pid(float angle_tar, float basespeed);

void pid_cal(pid_t *pid);
void pidout_limit(pid_t *pid);
void angle_limit(pid_t *pid);

float EWMA_filter(float input, float filtered_value, float alpha);
//float Yaw_error(float Target, float Now);
#endif
