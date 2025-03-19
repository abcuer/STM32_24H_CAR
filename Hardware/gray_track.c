#include "stm32f10x.h"   
#include "global.h"

// 定义计时器阈值（根据实际情况调整）
#define BAISE_TIME_THRESHOLD 3000

int targetValue = 0; // 目标值设为中线位置
int currentValue = 0; // 当前值初始化
int16_t baisetime = 0;

void gray_init()
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	  GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void Get_Light_TTL(void)
//{
//	if(L3 == 1 || L2== 1 || L1 == 1 || C1 == 1 || R1 == 1 || R2 == 1 || R3 == 1)
//	{
//		Line_flag = 1;
//	}
//}

//void Get_Light_TTL(void)
//{
//	baisetime ++;
//	if(L3 == 1 || L2== 1 || L1 == 1 || C1 == 1 || R1 == 1 || R2 == 1 || R3 == 1)
//	{
//		Line_flag = 1;
//		baisetime = 0;
//	}
//	if(baisetime >= 5800)
//	{
//		//Line_flag = 0;
//		baisetime = 0;
//	}
//}

void Get_Light_TTL(void)
{
    // 读取传感器状态
    uint8_t sensor_state = (L3 || L2 || L1 || C1 || R1 || R2 || R3);

    // 如果检测到黑线
    if (sensor_state)
    {
        Line_flag = 1;      // 设置标志位
        baisetime = 0;      // 重置计时器
    }
    else
    {
        baisetime++;        // 计时器累加
    }

    // 如果长时间未检测到黑线
    if (baisetime >= BAISE_TIME_THRESHOLD)
    {
        //Line_flag = 0;      // 清除标志位
        baisetime = 0;      // 重置计时器
    }
}

void track_pid_control1(void)
{
	currentValue = (R3 * 30 + R2 * 20 + R1 * 8 + C1 * 0 + L1 * -8 + L2 * -20  + L3 * -30) / (R1 + R2 + R3 + C1 + L1 + L2 + L3);
	// 计算误差
	trackLIne.now = currentValue;
	trackLIne.target = targetValue;
	pid_cal(&trackLIne);
	// 电机输出限幅
	pidout_limit(&trackLIne);
	motor_target_set(basespeed + trackLIne.out, basespeed - trackLIne.out);
}

void track_pid_control2(void)
{
	currentValue = (R3 * 25 + R2 * 16 + R1 * 8.5 + C1 * 0 + L1 * -8.5 + L2 * -16  + L3 * -25) / (R1 + R2 + R3 + C1 + L1 + L2 + L3);
	// 计算误差
	trackLIne.now = currentValue;
	trackLIne.target = targetValue;
	pid_cal(&trackLIne);
	// 电机输出限幅
	pidout_limit(&trackLIne);
	motor_target_set(basespeed + trackLIne.out, basespeed - trackLIne.out);
}
/*
	1.串口打印灰度传感器的数值	
	2.串口打印curreValue，trackLIne.out的值, 查看数值是否变化
*/


//uint8_t get_T_ALL(void)
//{
//	uint8_t temp = 0;
//	temp += L3;
//	temp <<= 1;
//	temp += L2;
//	temp <<= 1;
//	temp += L1;
//	temp <<= 1;
//	temp += C1;
//	temp <<= 1;
//	temp += R1;
//	temp <<= 1;
//	temp += R2;
//	temp <<= 1;
//	temp += R3;
//	return temp;
//}

//char get_error(void)
//{
//	char error;
//	if(get_T_ALL() == 0x08) 						 // 0001000 正
//	{
//			error = 10;
//	}
//	else if(get_T_ALL() == 0x10)  			 // 0010000 偏右
//	{
//			error = 13;
//	}
//	else if(get_T_ALL() == 0x20)  			 // 0100000 大偏右
//	{
//			error = 14;
//	}
//	// 1000000 或 1100000 或 1110000 或 0110000 更大偏右
//	else if(get_T_ALL() == 0x40 || get_T_ALL() == 0x60 || get_T_ALL() == 0x70 || get_T_ALL() == 0x30)  
//	{
//			error = 16;
//	}
//	else if(get_T_ALL() == 0x04)         // 0000100 偏左
//	{
//			error = 7;
//	}
//	else if(get_T_ALL() == 0x02)  			 // 0000010 大偏左
//	{
//			error = 6;
//	}
//	// 0000001 或 0000011 或 0000111 或 0000110 更大偏左
//	else if(get_T_ALL() == 0x01 || get_T_ALL() == 0x03 || get_T_ALL() == 0x07 || get_T_ALL() == 0x06)
//	{
//			error = 4;
//	}
//	else
//	{
//			error = 10;
//	}
//	return error;
//}

