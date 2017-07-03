#include "stm32f4xx.h" 
#include "my_spi.h"
#include "led.h"

u8 SpiReadata =0;

//SPI��ʱ
void My_Spi_Delay(u16 ticks)
{
	u16 m=0;
	for(m=0;m<ticks;m++)
	{
	   ;
	}
}

//SPI��ʼ��
void My_Spi_Inital(void)
{
	//CPOL =0; ʱ�Ӽ���0 ����ʱʱ�͵�ƽ��Ч
	SPI2_CLK_L();
	SPI3_CLK_L();
	//CPHA =0; ��λ���ݣ����豸�ڵ�һ�������زɼ�����,
}

//�����ֽڵ�λ��ǰ����λ�ں�
//CPOL = 0 //����0,SPI�ܽ�ƽʱ�͵�ƽ
//CPHA = 0 //��λ0,������SLAVE�ɼ����ݣ��½���SLAVE��������

void My_Spi2_SRdat(u8 *SDAT, u8 *RDAT ,u8 len)
{
	u8 i=0,j=0;
	for(j=0;j<len;j++){
		
			for(i=0;i<8;i++){
				
				SPI2_CLK_L();
				//׼����������
				if(*SDAT&(1<<i))
				{ SPI2_MOSI_H(); }
				else
				{ SPI2_MOSI_L(); }
				
				My_Spi_Delay(30);   //�������ݽ�����ʱ
				SPI2_CLK_H();       //����������
				My_Spi_Delay(30);   //�ߵ�ƽ����ʱ��
				SPI2_CLK_L();       //�����½���
				My_Spi_Delay(5);    //�������ݽ�����ʱ
				
				 //��ȡ����
				if(SPI2_MISO_V()){
					My_Spi_Delay(2);
					if(SPI2_MISO_V()){
						*RDAT |= 1<<i;
					}
				}
				
				if(!SPI2_MISO_V()){
					My_Spi_Delay(2);
					if(!SPI2_MISO_V()){
						*RDAT &= ~(1<<i);
					}	
				}		
				
			}
			SDAT++;
			RDAT++;
    }
}

//�����ֽڵ�λ��ǰ����λ�ں�
//CPOL = 0 //����0,SPI�ܽ�ƽʱ�͵�ƽ
//CPHA = 0 //��λ0,������SLAVE�ɼ����ݣ��½���SLAVE��������
void My_Spi3_SRdat(u8 *SDAT, u8 *RDAT ,u8 len)
{
	u8 i=0,j=0;
	for(j=0;j<len;j++){
		
			for(i=0;i<8;i++){
				
				SPI3_CLK_L();
				//׼����������
				if(*SDAT&(1<<i))
				{ SPI3_MOSI_H(); }
				else
				{ SPI3_MOSI_L(); }
				
				My_Spi_Delay(30);   //�������ݽ�����ʱ
				SPI3_CLK_H();       //����������
				My_Spi_Delay(30);   //�ߵ�ƽ����ʱ��
				SPI3_CLK_L();       //�����½���
				My_Spi_Delay(5);    //�������ݽ�����ʱ
				
				 //��ȡ����
				if(SPI3_MISO_V()){
					My_Spi_Delay(2);
					if(SPI3_MISO_V()){
						*RDAT |= 1<<i;
					}
				}
				
				if(!SPI3_MISO_V()){
					My_Spi_Delay(2);
					if(!SPI3_MISO_V()){
						*RDAT &= ~(1<<i);
					}	
				}		
			}
			SDAT++;
			RDAT++;
    }
}
