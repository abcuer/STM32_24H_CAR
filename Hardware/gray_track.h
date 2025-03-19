#ifndef __gray_track_h_
#define __gray_track_h_

#define R3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)	// 7
#define R2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)		// 6
#define R1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)		// 5
#define C1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)		// 4
#define L1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)		// 3
#define L2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)		// 2
#define L3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)  	// 1

void gray_init(void);
//void track(void);
void Get_Light_TTL(void);
void track_pid_control1(void);
void track_pid_control2(void);
//uint8_t get_T_ALL(void);
//char get_error(void);

#endif
