#include <stdio.h> 
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "config.h"
#include "util.h"
#include "md5.h"

//CONFIG_MSG  ConfigMsg;																	
extern CONFIG_MSG Config_Msg;                             /*配置结构体*/
/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
extern uint8 MAC[6];  //MAC Address
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def; 
//TX MEM SIZE- SOCKET 0-7:2KB
//RX MEM SIZE- SOCKET 0-7:2KB
extern uint8 txsize[8];
extern uint8 rxsize[8];
extern uint8 IP_Atuo;

/**
*@brief		自动配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{	
  /*复制DHCP获取的配置信息到配置结构体*/
	//memcpy(Config_Msg.Lip,DHCP_GET.Lip, 4);
  //memcpy(Config_Msg.Sub,DHCP_GET.Sub, 4);
  //memcpy(Config_Msg.Gw, DHCP_GET.Gw, 4);
	//memcpy(Config_Msg.Dns,DHCP_GET.Dns,4);
	/*将IP配置信息写入W5500相应寄存器*/	
  setSUBR(Config_Msg.Sub);
  setGAR(Config_Msg.Gw);
  setSIPR(Config_Msg.Lip);
}

/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	setSHAR(Config_Msg.Mac);	/**/
	//memcpy(DHCP_GET.Mac, Config_Msg.Mac, 6);
}

