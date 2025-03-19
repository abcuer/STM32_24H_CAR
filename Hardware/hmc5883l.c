#include "stm32f10x.h"                  // Device header
#include "hmc5883l.h"
#include "HMCI2C.h"

int16_t hmc_x, hmc_y, hmc_z;
float yaw_hmc;

void HMC5883L_Write(uint8_t addr, uint8_t dat)
{
	HMCI2C_Start();
	HMCI2C_SendByte(HMC5883L_ADDR);
	HMCI2C_WaitAck();
	HMCI2C_SendByte(addr);
	HMCI2C_WaitAck();
	HMCI2C_SendByte(dat);
	HMCI2C_WaitAck();
	HMCI2C_Stop();
}

uint8_t HMC5883L_Read(uint8_t addr)
{
	HMCI2C_Start();
	HMCI2C_SendByte(HMC5883L_ADDR);
	HMCI2C_WaitAck();
	HMCI2C_SendByte(addr);
	HMCI2C_WaitAck();
	HMCI2C_Stop();
	
	HMCI2C_Start();
	HMCI2C_SendByte(HMC5883L_ADDR | 0x01);
	HMCI2C_WaitAck();
	uint8_t dat = HMCI2C_ReceiveByte();
	HMCI2C_NotSendAck();
	HMCI2C_Stop();
	
	return dat;
}

void HMC5883L_Init()
{
	HMC5883L_Write(HMC5883L_CRA, 0xf8);  // 最大输出速率（75HZ）
	HMC5883L_Write(HMC5883L_CRB, 0x20);  // 默认增益
	HMC5883L_Write(HMC5883L_MR, 0x00);   // 连续测量
}

void HMC5883L_GetData()
{
	uint8_t data_h, data_l;
	data_h = HMC5883L_Read(HMC5883L_DOXMR);
	data_l = HMC5883L_Read(HMC5883L_DOXLR);
	hmc_x = data_l | (data_h << 8);

	data_h = HMC5883L_Read(HMC5883L_DOYMR);
	data_l = HMC5883L_Read(HMC5883L_DOYLR);
	hmc_y = data_l | (data_h << 8);

	data_h = HMC5883L_Read(HMC5883L_DOZMR);
	data_l = HMC5883L_Read(HMC5883L_DOZLR);
	hmc_z = data_l | (data_h << 8);
}




	


