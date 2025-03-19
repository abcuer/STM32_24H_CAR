#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);				/* 将数据写入寄存器 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress);										/* 读取寄存器的数据 */

void MPU6050_Init(void);																				/* 初始化MPU6050 */
uint8_t MPU6050_GetID(void);																		/* 确认连接设备是MPU6050 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,  /*读取加速度计和陀螺仪数据 */
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
