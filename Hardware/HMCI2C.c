#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void HMCI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_10, (BitAction)BitValue);		
	Delay_us(10);											
}

void HMCI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)BitValue);		
	Delay_us(10);												
}

uint8_t HMCI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);		
	Delay_us(10);											
	return BitValue;											
}

void HMCI2C_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	/*??????*/
	GPIO_SetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_11);			
}

// 起始信号
void HMCI2C_Start()
{
	HMCI2C_W_SDA(1);							
	HMCI2C_W_SCL(1);							
	HMCI2C_W_SDA(0);							
	HMCI2C_W_SCL(0);
}

// 终止信号 
void HMCI2C_Stop(void)
{
	HMCI2C_W_SDA(0);							
	HMCI2C_W_SCL(1);						
	HMCI2C_W_SDA(1);							
}

// 主机发送一个字节
void HMCI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				
	{
		HMCI2C_W_SDA(Byte & (0x80 >> i));	
		HMCI2C_W_SCL(1);						
		HMCI2C_W_SCL(0);					
	}
}

// 主机接收一个字节
uint8_t HMCI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;				
	HMCI2C_W_SDA(1);							
	for (i = 0; i < 8; i ++)				
	{
		HMCI2C_W_SCL(1);						
		if (HMCI2C_R_SDA() == 1){Byte |= (0x80 >> i);}	
		HMCI2C_W_SCL(0);						
	}
	return Byte;						
}

// 主机应答 
void HMCI2C_SendAck(uint8_t AckBit)
{
	HMCI2C_W_SDA(AckBit);					
	HMCI2C_W_SCL(1);						
	HMCI2C_W_SCL(0);							
}

// 主机不应答 
void HMCI2C_NotSendAck()
{
	HMCI2C_W_SDA(1);
	HMCI2C_W_SCL(1);	
	HMCI2C_W_SCL(0);	
}

// 等待从机应答 
uint8_t HMCI2C_WaitAck(void)
{
	uint8_t AckBit;						
	HMCI2C_W_SDA(1);						
	HMCI2C_W_SCL(1);						
	AckBit = HMCI2C_R_SDA();					
	HMCI2C_W_SCL(0);						
	return AckBit;							
}

