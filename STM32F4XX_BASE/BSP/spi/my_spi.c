#include "stm32f4xx.h" 
#include "my_spi.h"
#include "led.h"

u8 SpiReadata =0;

//SPI延时
void My_Spi_Delay(u16 ticks)
{
	u16 m=0;
	for(m=0;m<ticks;m++)
	{
	   ;
	}
}

//SPI初始化
void My_Spi_Inital(void)
{
	//CPOL =0; 时钟极性0 空闲时时低电平有效
	SPI2_CLK_L();
	SPI3_CLK_L();
	//CPHA =0; 相位数据，从设备在第一个上升沿采集数据,
}

//发送字节低位在前，高位在后
//CPOL = 0 //极性0,SPI管脚平时低电平
//CPHA = 0 //相位0,上升沿SLAVE采集数据，下降沿SLAVE发送数据

void My_Spi2_SRdat(u8 *SDAT, u8 *RDAT ,u8 len)
{
	u8 i=0,j=0;
	for(j=0;j<len;j++){
		
			for(i=0;i<8;i++){
				
				SPI2_CLK_L();
				//准备发送数据
				if(*SDAT&(1<<i))
				{ SPI2_MOSI_H(); }
				else
				{ SPI2_MOSI_L(); }
				
				My_Spi_Delay(30);   //发送数据建立延时
				SPI2_CLK_H();       //产生上升沿
				My_Spi_Delay(30);   //高电平保持时间
				SPI2_CLK_L();       //产生下降沿
				My_Spi_Delay(5);    //接收数据建立延时
				
				 //读取数据
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

//发送字节低位在前，高位在后
//CPOL = 0 //极性0,SPI管脚平时低电平
//CPHA = 0 //相位0,上升沿SLAVE采集数据，下降沿SLAVE发送数据
void My_Spi3_SRdat(u8 *SDAT, u8 *RDAT ,u8 len)
{
	u8 i=0,j=0;
	for(j=0;j<len;j++){
		
			for(i=0;i<8;i++){
				
				SPI3_CLK_L();
				//准备发送数据
				if(*SDAT&(1<<i))
				{ SPI3_MOSI_H(); }
				else
				{ SPI3_MOSI_L(); }
				
				My_Spi_Delay(30);   //发送数据建立延时
				SPI3_CLK_H();       //产生上升沿
				My_Spi_Delay(30);   //高电平保持时间
				SPI3_CLK_L();       //产生下降沿
				My_Spi_Delay(5);    //接收数据建立延时
				
				 //读取数据
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
