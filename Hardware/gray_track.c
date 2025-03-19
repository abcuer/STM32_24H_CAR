#include "stm32f10x.h"   
#include "global.h"

int basic_speed = 0;
int err = 0;
uint8_t Line_flag = 0;
uint8_t Line_flag_time = 0;

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

void Get_Light_TTL(void)
{
	if(L3 == 1 || L2== 1 || L1 == 1 || C1 == 1 || R1 == 1 || R2 == 1 || R3 == 1)
	{
		Line_flag = 1;
	}
}
