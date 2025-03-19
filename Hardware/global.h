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
#include "MyI2C.h"
#include "MPU6050.h"
#include <math.h>
#include "filter.h"
#include "Buzzer.h"
#include "detect_position.h"
#include "HMCI2C.h"
#include "hmc5883l.h"

#define MAX_DUTY 100

extern uint8_t motorA_dir;
extern uint8_t motorB_dir;

extern int16_t Encoder_countA;
extern int16_t Encoder_countB;

extern pid_t motorA;
extern pid_t motorB;
extern pid_t angle;

extern int16_t AX,AY,AZ,GX,GY,GZ;
extern float roll_gyro, pitch_gyro, yaw_gyro;
extern float roll_acc, pitch_acc, yaw_acc;
extern float yaw_hmc;
extern float roll_Kalman, pitch_Kalman, yaw_Kalman;

extern uint8_t Line_flag;
extern uint16_t SoundLight_flag;
extern uint16_t angle_flag;
extern float basespeed;
extern uint16_t workstep;
extern uint16_t angle_record1;
#endif
