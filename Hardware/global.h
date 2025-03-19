#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "math.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Timer.h"
#include "Encoder.h"
#include "Motor_Run.h"
#include "Serial.h"
#include "pid.h"
#include "gray_track.h"
#include "Buzzer.h"
#include "OLED.h"
#include "JY901S.h"
#include "detect_position.h"
#include "Task.h"


#define MAX_DUTY 500
#define ANGLE_DUTY 300

extern uint8_t motorA_dir;
extern uint8_t motorB_dir;

extern int16_t Encoder_countA;
extern int16_t Encoder_countB;

extern uint8_t Serial_RxData;		//定义串口接收的数据变量
extern uint8_t Serial_RxFlag;	

extern pid_t motorA;
extern pid_t motorB;
extern pid_t angle;
extern pid_t trackLIne;

extern uint16_t SoundLight_flag;
extern uint16_t SoundLight_time;

extern uint16_t Task;
extern uint16_t first_flag;
extern uint16_t start_flag;
extern uint16_t Line_flag;
extern uint16_t angle_flag;
extern float angle_initial;
extern uint16_t workstep;
extern float basespeed;
extern uint16_t angle_record1;
extern uint16_t angle_record2; 
extern uint16_t angle_record3; 
extern uint16_t angle_record4;
extern int currentValue;
extern float p;
extern float i;
extern float d;

extern float ini;

#endif
