#ifndef __AT24Cxx_H
#define __AT24Cxx_H
#include "stm32f4xx.h" 
#include "types.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C08

//IIC所有操作函数
void IIC_Init(void);          //初始化IIC的IO口				 
void IIC_Start(void);				  //发送IIC开始信号
void IIC_Stop(void);	  		  //发送IIC停止信号
void IIC_Send_Byte(uint8 txd);	  //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 			  //IIC等待ACK信号
void IIC_Ack(void);					  //IIC发送ACK信号
void IIC_NAck(void);				  //IIC不发送ACK信号

void AT24CXX_WriteLenByte(uint16 WriteAddr,uint32 DataToWrite,u8 Len); //指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(uint16 ReadAddr,uint8 Len);					           //指定地址开始读取指定长度数据
void AT24CXX_Write(uint16 WriteAddr,uint8 *pBuffer,int NumToWrite);	   //从指定地址开始写入指定长度的数据
void AT24CXX_Read(uint16 ReadAddr,uint8 *pBuffer,uint16 NumToRead);    //从指定地址开始读出指定长度的数据
void AT24CXX_ReadnByte(uint16 ReadAddr1,uint8 * buf, uint8 num);
void AT24CXX_WritenByte(uint16 WriteAddr1,uint8 * buf, uint8 num);


u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC


#endif
