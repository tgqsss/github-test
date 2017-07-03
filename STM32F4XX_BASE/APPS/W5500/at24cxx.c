#include "config.h"
#include "at24cxx.h"
#include "myiic.h"
#include "util.h"
#include "delay.h"

//初始化IIC接口
void AT24CXX_Init(void)
{
	  IIC_Init();
}

//在AT24CXX指定地址读出n个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
void AT24CXX_ReadnByte(uint16 ReadAddr1,uint8 * buf, uint8 num) 
{				  
		//uint8 temp=0;		
	  uint8 icnt=0;
    uint8 ctrl=0;
	  
    ctrl=0XA0+(((ReadAddr1/256)<<1)&0X0E);
	  IIC_Start();  
	  IIC_Send_Byte(ctrl);   //发送器件地址0XA0,写数据 	 

		IIC_Wait_Ack(); 
		IIC_Send_Byte(ReadAddr1%256);   //发送低地址
		IIC_Wait_Ack();	    
		IIC_Start(); 
    //IIC_Send_Byte(0xA1);         //进入接收模式			
		IIC_Send_Byte(ctrl+0x01);      //进入接收模式			   
		IIC_Wait_Ack();	
    for(icnt=0;icnt<num;icnt++)	
		 {
			 if(icnt==(num-1))
       {*(buf+icnt)=IIC_Read_Byte(0);}
			 else
			 {*(buf+icnt)=IIC_Read_Byte(1);} 
     }		   
		IIC_Stop();//产生一个停止条件	 
}


//在AT24CXX指定地址写入连续数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WritenByte(uint16 WriteAddr1,uint8 * buf, uint8 num)
{		
	  uint8 ctrl2=0;
	  uint8 icnt=0;
	  ctrl2=0XA0+(((WriteAddr1/256)<<1)&0X0E);
	  IIC_Start(); 
		IIC_Send_Byte(ctrl2);            //发送器件地址0XA0
		IIC_Wait_Ack();	   
		IIC_Send_Byte(WriteAddr1%256);   //发送低地址
		IIC_Wait_Ack(); 	 
    for(icnt=0;icnt<num;icnt++)	
	  {
		  IIC_Send_Byte(*(buf+icnt));   //发送字节							   
		  IIC_Wait_Ack();  	
	  }
		IIC_Stop();//产生一个停止条件 
		delay_ms(5);	 
}


//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(uint16 WriteAddr,uint32 DataToWrite,uint8 Len)
{  	
		u8 t;
		for(t=0;t<Len;t++)
		{
//			AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
			;
		}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(uint16 ReadAddr,uint8 Len)
{  	
		u8 t;
		u32 temp=0;
		for(t=0;t<Len;t++)
		{
			temp<<=8;
	//		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
		}
		return temp;												    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
		u8 temp;
//		temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
		if(temp==0X55)return 0;		   
		else//排除第一次初始化的情况
		{
//			AT24CXX_WriteOneByte(255,0X55);
//			temp=AT24CXX_ReadOneByte(255);	  
			if(temp==0X55)return 0;
		}
		return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
/*void AT24CXX_Read(uint16 ReadAddr,uint8 *pBuffer,uint16 NumToRead)
{
		while(NumToRead)
		{
			*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
			NumToRead--;
		}
}*/ 

void AT24CXX_Read(uint16 ReadAddr,uint8 *pBuffer,uint16 NumToRead)
{
	  AT24CXX_ReadnByte(ReadAddr,pBuffer,NumToRead);
	  /*
		while(NumToRead)
		{
			*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
			NumToRead--;
		}*/
}

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
/*void AT24CXX_Write(uint16 WriteAddr,uint8 *pBuffer,uint16 NumToWrite)
{
		while(NumToWrite--)
		{
			AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
			WriteAddr++;
			pBuffer++;
		}
}*/

void AT24CXX_Write(uint16 WriteAddr,uint8 *pBuffer,int NumToWrite)
{
	 int page_size =16;
   uint8 rc;
   int b_to_wr;
	// uint8 tnt=0;
	
	 rc=WriteAddr%page_size;
	 if(rc!=0)
	 {
		  b_to_wr = page_size - rc; 
      if(NumToWrite < b_to_wr)
      b_to_wr = NumToWrite;
      if(b_to_wr > 0)
      {
				 AT24CXX_WritenByte(WriteAddr,pBuffer,b_to_wr);
         NumToWrite -= b_to_wr;
         WriteAddr += b_to_wr;
         pBuffer += b_to_wr;
      }
   }
	 //for(tnt=0;) 
	 while(1)
	 {
			if(NumToWrite < page_size)
       b_to_wr = NumToWrite;
      else
       b_to_wr = page_size;
       AT24CXX_WritenByte(WriteAddr,pBuffer,b_to_wr);
       NumToWrite -= b_to_wr;
       WriteAddr += b_to_wr;
       pBuffer += b_to_wr;
			 if(NumToWrite<=0)
			 {break;}
	 }
}





