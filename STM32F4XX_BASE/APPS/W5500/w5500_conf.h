#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f4xx.h" 
#include "stdio.h"
#include "types.h"

typedef  void (*pFunction)(void);

#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460											 /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

extern uint8 dhcp_ok;																				/*DHCP获取成功*/
extern uint32	dhcp_time;																		/*DHCP运行计数*/
extern vu8	ntptimer;																				/*NPT秒计数*/

/*MCU配置相关函数*/
void gpio_for_w5500_config(void);														/*SPI接口reset 及中断引脚*/
void timer2_init(void);																			/*STM32定时器2初始化*/
void timer2_isr(void);																			/*定时器中断执行的函数*/
void reboot(void);																					/*STM32软复位*/
void write_config_to_eeprom(void);													/*写配置信息到EEPROM中*/
void read_config_from_eeprom(void);													/*从EEPROM中读出信息*/

/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*写入一个8位数据到W5500*/
uint8 IINCHIP_READ(uint32 addrbsb);													/*从W5500读出一个8位数据*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*向W5500写入len字节数据*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/

/*需要用定时的的应用函数*/
void dhcp_timer_init(void);																	/*dhcp用到的定时器初始化*/
void ntp_timer_init(void);																	/*npt用到的定时器初始化*/


#endif
