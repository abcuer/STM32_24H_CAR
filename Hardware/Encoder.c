#include "stm32f10x.h"                  // Device header

void EncoderA_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//TIM1
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535-1;//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//TIM1 Input
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM1,&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM1, &TIM_ICInitStruct);

	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM1,ENABLE);
}


void EncoderB_Init(void)
{
	// TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65535 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3, ENABLE);
}	

int16_t EncoderA_Get(void)
{
	int16_t temp;
	temp = TIM_GetCounter(TIM1);
	TIM_SetCounter(TIM1, 0);
	return temp;
}

int16_t EncoderB_Get(void)
{
	int16_t temp;
	temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return temp;
}
