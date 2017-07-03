#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f4xx.h" 
#include "Types.h"

#define I2C_SCK	      GPIO_Pin_6  //out
#define I2C_SDA	      GPIO_Pin_7  //bidirection

#define WIZ_RESET		  GPIO_Pin_3	// out
#define WIZ_SCS			  GPIO_Pin_4	// out

#define WIZ_SCLK			GPIO_Pin_5	// out
#define WIZ_MISO			GPIO_Pin_6	// in
#define WIZ_MOSI			GPIO_Pin_7	// out
// Port C
#define WIZ_INT				GPIO_Pin_4	//
//=================================================
 typedef struct _CONFIG_MSG
{
	uint8  Mac[6];
	uint8  Lip[4];
	uint8  Sub[4];
	uint8  Dns[4];	
  uint8  Gw[4];
	uint8  nopt;
	uint8  Sw_ver[3];
	uint8  Rip[4];				/*remote IP‘∂≥ÃIPµÿ÷∑*/	
 	uint16 Rport;		
}
CONFIG_MSG;


typedef struct _CONFIG_TYPE_DEF
{
	uint16 port;
	uint8 destip[4];
}CHCONFIG_TYPE_DEF;

	
#define SOCK_CONFIG		2	// UDP
#define SOCK_DNS		  2	// UDP
#define SOCK_DHCP		  3	// UDP
#define SOCK_SMTP		  7	// UDP
#define SOCK_NTP      0 // UDP
#define SOCK_HTTPS    4 // UDP

#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	30	// 30sec

#define ON	1
#define OFF	0

#define HIGH		1
#define LOW		0

// SRAM address range is 0x2000 0000 ~ 0x2000 4FFF (20KB)
#define TX_RX_MAX_BUF_SIZE	2048

//#define TX_BUF	0x20004000
//#define RX_BUF	(TX_BUF+TX_RX_MAX_BUF_SIZE)
extern uint8 TX_BUF[TX_RX_MAX_BUF_SIZE];
extern uint8 RX_BUF[TX_RX_MAX_BUF_SIZE];


//#define ApplicationAddress 	0x08004000

#endif
