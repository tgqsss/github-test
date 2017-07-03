#include "config.h"
#include "at24cxx.h"
#include "myiic.h"
#include "util.h"
#include "delay.h"

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	  IIC_Init();
}

//��AT24CXXָ����ַ����n������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
void AT24CXX_ReadnByte(uint16 ReadAddr1,uint8 * buf, uint8 num) 
{				  
		//uint8 temp=0;		
	  uint8 icnt=0;
    uint8 ctrl=0;
	  
    ctrl=0XA0+(((ReadAddr1/256)<<1)&0X0E);
	  IIC_Start();  
	  IIC_Send_Byte(ctrl);   //����������ַ0XA0,д���� 	 

		IIC_Wait_Ack(); 
		IIC_Send_Byte(ReadAddr1%256);   //���͵͵�ַ
		IIC_Wait_Ack();	    
		IIC_Start(); 
    //IIC_Send_Byte(0xA1);         //�������ģʽ			
		IIC_Send_Byte(ctrl+0x01);      //�������ģʽ			   
		IIC_Wait_Ack();	
    for(icnt=0;icnt<num;icnt++)	
		 {
			 if(icnt==(num-1))
       {*(buf+icnt)=IIC_Read_Byte(0);}
			 else
			 {*(buf+icnt)=IIC_Read_Byte(1);} 
     }		   
		IIC_Stop();//����һ��ֹͣ����	 
}


//��AT24CXXָ����ַд����������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WritenByte(uint16 WriteAddr1,uint8 * buf, uint8 num)
{		
	  uint8 ctrl2=0;
	  uint8 icnt=0;
	  ctrl2=0XA0+(((WriteAddr1/256)<<1)&0X0E);
	  IIC_Start(); 
		IIC_Send_Byte(ctrl2);            //����������ַ0XA0
		IIC_Wait_Ack();	   
		IIC_Send_Byte(WriteAddr1%256);   //���͵͵�ַ
		IIC_Wait_Ack(); 	 
    for(icnt=0;icnt<num;icnt++)	
	  {
		  IIC_Send_Byte(*(buf+icnt));   //�����ֽ�							   
		  IIC_Wait_Ack();  	
	  }
		IIC_Stop();//����һ��ֹͣ���� 
		delay_ms(5);	 
}


//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(uint16 WriteAddr,uint32 DataToWrite,uint8 Len)
{  	
		u8 t;
		for(t=0;t<Len;t++)
		{
//			AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
			;
		}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
		u8 temp;
//		temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
		if(temp==0X55)return 0;		   
		else//�ų���һ�γ�ʼ�������
		{
//			AT24CXX_WriteOneByte(255,0X55);
//			temp=AT24CXX_ReadOneByte(255);	  
			if(temp==0X55)return 0;
		}
		return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
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

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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





