#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Run_Init(void)
{
	PWM_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
×óÂÖ: Pin_14¢PPin_15  PWM2(PIn_2)
ÓÒÂÖ: Pin_3   Pin_5 	PWM1(Pin_1)
*/
uint8_t motorA_dir = 1;
uint8_t motorB_dir = 1;

/*×óÂÖµ÷ËÙ*/
void Speed_L(uint16_t Speed_l)
{
	TIM_SetCompare3(TIM2, Speed_l);
}
/*ÓÒÂÖµ÷ËÙ*/
void Speed_R(uint16_t Speed_r)
{
	TIM_SetCompare2(TIM2, Speed_r);
}

void motorB_duty(uint16_t duty) /* ÓÒÂÖ 1 4 5 Encoder 6 7*/
{
	Speed_R(duty);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction) motorB_dir);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) !motorB_dir);
}

void motorA_duty(uint16_t duty) /* ×óÂÖ 2 14 15 Encoder 8 9 */
{
	Speed_L(duty);
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction) motorA_dir);
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction) !motorA_dir);
}

//void Car_Straight(void)
//{
//	GPIO_SetBits(GPIOA, GPIO_Pin_3);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//	GPIO_SetBits(GPIOB, GPIO_Pin_14);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
//	Speed_L(50);
//	Speed_R(50);
//}

//void Car_Back(void)
//{
//	GPIO_SetBits(GPIOA, GPIO_Pin_5);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
//	GPIO_SetBits(GPIOB, GPIO_Pin_15);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
//	Speed_L(50);
//	Speed_R(50);
//}

//void Car_Left(void)
//{
//	Car_Straight();
//	Speed_L(50);
//	Speed_R(25);
//}

//void Car_Right(void)
//{
//	Car_Straight();
//	Speed_L(25);
//	Speed_R(50);
//}

//void Car_Stop(void)
//{
//	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
//}

