#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

void WIZ_SPI_Init(void);
void WIZ_CS(uint8_t val); 	    													  
void SPI1_SetSpeed(u8 SpeedSet);  //����SPI1�ٶ�   
u8 SPI1_SendByte(u8 TxData);      //SPI1���߶�дһ���ֽ�
		 
#endif

