#ifndef __gray_track_h_
#define __gray_track_h_

#define R3 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define R2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define R1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define C1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
#define L1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define L2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define L3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)

void gray_init(void);
//void track(void);
void Get_Light_TTL(void);

#endif
