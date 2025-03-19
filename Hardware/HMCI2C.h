#ifndef _HMCI2C_H
#define _HMCI2C_H

#include "stm32f10x.h"                  // Device header

void HMCI2C_Init(void);
void HMCI2C_Start(void);
void HMCI2C_Stop(void);
void HMCI2C_SendByte(uint8_t byte);
uint8_t HMCI2C_ReceiveByte(void);
void HMCI2C_SendAck(void);
void HMCI2C_NotSendAck(void);
uint8_t HMCI2C_WaitAck(void);

#endif


