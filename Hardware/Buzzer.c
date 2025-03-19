#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void Buzzer_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	} else{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	}
}
