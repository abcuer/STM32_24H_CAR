#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

//void UART1_Init(uint32_t bound);
void UART3_Init(uint32_t bound);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


/*
	读取串口值 显示输出值
	实现OLED屏显示功能
*/
#endif
