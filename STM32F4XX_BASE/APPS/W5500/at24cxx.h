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
//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE AT24C08

//IIC���в�������
void IIC_Init(void);          //��ʼ��IIC��IO��				 
void IIC_Start(void);				  //����IIC��ʼ�ź�
void IIC_Stop(void);	  		  //����IICֹͣ�ź�
void IIC_Send_Byte(uint8 txd);	  //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 			  //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					  //IIC����ACK�ź�
void IIC_NAck(void);				  //IIC������ACK�ź�

void AT24CXX_WriteLenByte(uint16 WriteAddr,uint32 DataToWrite,u8 Len); //ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(uint16 ReadAddr,uint8 Len);					           //ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(uint16 WriteAddr,uint8 *pBuffer,int NumToWrite);	   //��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(uint16 ReadAddr,uint8 *pBuffer,uint16 NumToRead);    //��ָ����ַ��ʼ����ָ�����ȵ�����
void AT24CXX_ReadnByte(uint16 ReadAddr1,uint8 * buf, uint8 num);
void AT24CXX_WritenByte(uint16 WriteAddr1,uint8 * buf, uint8 num);


u8 AT24CXX_Check(void);  //�������
void AT24CXX_Init(void); //��ʼ��IIC


#endif
