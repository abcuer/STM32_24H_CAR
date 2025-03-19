#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_SetBits(GPIOC, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13);
}

void LED_Blue_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
}

void LED_Blue_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
}

void LED_Green_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_15);
}

void LED_Green_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}
