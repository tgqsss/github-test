#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED�˿ڶ���
#define LED_ON()   GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LED_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_0)

//˫�����оƬʹ�ܹܽ�
#define TCE1_L()   GPIO_ResetBits(GPIOD,GPIO_Pin_0)
#define TCE1_H()   GPIO_SetBits(GPIOD,GPIO_Pin_0)

//SPI2����
#define SPI2_CLK_L()   GPIO_ResetBits(GPIOD,GPIO_Pin_4)
#define SPI2_CLK_H()   GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define SPI2_MOSI_L()  GPIO_ResetBits(GPIOD,GPIO_Pin_5)
#define SPI2_MOSI_H()  GPIO_SetBits(GPIOD,GPIO_Pin_5)

#define SPI2_MISO_V()  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6) 

//SPI3����
#define SPI3_CLK_L()   GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define SPI3_CLK_H()   GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define SPI3_MOSI_L()  GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define SPI3_MOSI_H()  GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define SPI3_MISO_V()  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11) 


void GPIO_InitAll(void);
void LED_Init(void);//��ʼ��		
void LED_Run(void);  
#endif
