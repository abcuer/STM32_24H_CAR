#include "stm32f10x.h"                  // Device header

//#define tim1 TIM2

//????->1s

void Timer4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//1??
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//????
	TIM_TimeBaseInitStruct.TIM_Period= 100 - 1;//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=7200-1;//PSC
	/* 10ms �ж�*/
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4, ENABLE);
}
