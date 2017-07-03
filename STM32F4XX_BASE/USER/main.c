#include "main.h"
#include "delay.h"
#include <string.h>
#include "stdio.h"
#include "W5500.h"
#include "spi1.h"
#include "my_spi.h"
#include "socket.h"
#include "config.h"
#include "util.h"
#include "loopback.h"
#include "at24cxx.h"	
#include "led.h" 
#include "w5500_conf.h"
#include "timer.h"
#include "usart.h"

uint16 TCP_SERVICE_PORT=3000; //服务器端口号

//Private functions
CONFIG_MSG Config_Msg;
CHCONFIG_TYPE_DEF Chconfig_Type_Def;

//Configuration Network Information of W5500
uint32_t my_time=2; 
uint32_t presentTime=0;
uint8 Enable_DHCP = ON;

//TX MEM SIZE- SOCKET 0-7:2KB
//RX MEM SIZE- SOCKET 0-7:2KB
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 ch_status[MAX_SOCK_NUM] = { 0, };	

/** 0:close, 1:ready, 2:connected */
uint8 TX_BUF[TX_RX_MAX_BUF_SIZE];  // TX Buffer for applications
uint8 RX_BUF[TX_RX_MAX_BUF_SIZE];  // RX Buffer for applications
uint16  SRC_udp_PORT =2206;        // SRC_udp port 

//SPI数据数组
uint8 SPI_SBUF[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8 SPI_RBUF[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
//IIC临时数组
uint8 Iic_read_temp[100];	
uint8 Inform[28]={'0','0','0','.','0','0','0','.','0','0','0','.','0','0','0'};	

u8 identify[1]={0};

extern void StrBufCopy(uint8 *src,uint8 spos, uint8 *dec,uint8 dpos, uint8 len );
extern void iap_init(void);

void SPI_BUF_INIT(void)
{
	u8 i=0;
	for(i=0;i<10;i++)
  {
		SPI_SBUF[i]=0x55;
	}
}

//IP转换
void IpformCom(uint8 *buf){
	uint8 icnt = 0;
	for(icnt=0;icnt<4;icnt++){
		Inform[icnt*4] = *(buf+icnt)/100+'0';
		Inform[icnt*4+1] = (*(buf+icnt)%100)/10+'0';
		Inform[icnt*4+2] = (*(buf+icnt)%100)%10+'0';
	}
}

//
void IP_DISPLAY(void){
	 IpformCom(Config_Msg.Lip);	
	 Inform[15] = ':';	
	 Inform[16] = '0';	
	 Inform[17] = '5';	
	 Inform[18] = '0';	 
	 Inform[19] = 10;	
   USART1_SendData(Inform,20);	
}

//初始化配置
void INFORM_INTAIL(){
	
	AT24CXX_ReadnByte(220,identify,1);
	
	//已经配置成功
	if(identify[0]==0x55){
		
		AT24CXX_ReadnByte(4,Iic_read_temp,6);
		StrBufCopy(Iic_read_temp,0,Config_Msg.Mac,0,6);
		
		AT24CXX_ReadnByte(10,Iic_read_temp,4);
		StrBufCopy(Iic_read_temp,0,Config_Msg.Lip,0,4);
		
		AT24CXX_ReadnByte(14,Iic_read_temp,4);
		StrBufCopy(Iic_read_temp,0,Config_Msg.Gw,0,4);
		
		AT24CXX_ReadnByte(18,Iic_read_temp,4);
		StrBufCopy(Iic_read_temp,0,Config_Msg.Sub,0,4);
	}
	
}

//网口配置
void Net_Option_Set(void)
{
		Config_Msg.Mac[0]=0x00;     Config_Msg.Mac[1]=0x08;      Config_Msg.Mac[2]=0xDC; 
		Config_Msg.Mac[3]=0x03;     Config_Msg.Mac[4]=0x02;      Config_Msg.Mac[5]=0x03;
		Config_Msg.Lip[0]=192;      Config_Msg.Lip[1]=168;       Config_Msg.Lip[2]=0;         Config_Msg.Lip[3]=100;
		Config_Msg.Sub[0]=255;      Config_Msg.Sub[1]=255;       Config_Msg.Sub[2]=255;       Config_Msg.Sub[3]=0;
		Config_Msg.Gw[0] =192;      Config_Msg.Gw[1] =168;       Config_Msg.Gw[2] =0;         Config_Msg.Gw[3] =1; 
}

int main(void)
{  
	  LED_Init();
	
	  GPIO_InitAll();
	
	  Net_Option_Set();
	  
	  delay_init(168);   //初始化延时函数
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	
	  AT24CXX_Init();
	
	  INFORM_INTAIL();   //消息初始化配置
	
	  delay_ms(10);
	
	  iap_init();
	
	  Reset_W5500();
	
	  WIZ_SPI_Init();
	
	  delay_ms(50);
	
		set_w5500_mac();
	
	  delay_ms(50);
		
	  Set_network();
	 
		presentTime = my_time;  //For TCP client's connection request delay
	  
	  while(1){
	
		loopback_tcps(4,TCP_SERVICE_PORT);
	
	 }
}

#ifdef  USE_FULL_ASSERT

 /**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
 */

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
