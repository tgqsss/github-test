#include <stdio.h> 
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "config.h"
#include "util.h"
#include "md5.h"

//CONFIG_MSG  ConfigMsg;																	
extern CONFIG_MSG Config_Msg;                             /*���ýṹ��*/
/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
extern uint8 MAC[6];  //MAC Address
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def; 
//TX MEM SIZE- SOCKET 0-7:2KB
//RX MEM SIZE- SOCKET 0-7:2KB
extern uint8 txsize[8];
extern uint8 rxsize[8];
extern uint8 IP_Atuo;

/**
*@brief		�Զ�����W5500��IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{	
  /*����DHCP��ȡ��������Ϣ�����ýṹ��*/
	//memcpy(Config_Msg.Lip,DHCP_GET.Lip, 4);
  //memcpy(Config_Msg.Sub,DHCP_GET.Sub, 4);
  //memcpy(Config_Msg.Gw, DHCP_GET.Gw, 4);
	//memcpy(Config_Msg.Dns,DHCP_GET.Dns,4);
	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
  setSUBR(Config_Msg.Sub);
  setGAR(Config_Msg.Gw);
  setSIPR(Config_Msg.Lip);
}

/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	setSHAR(Config_Msg.Mac);	/**/
	//memcpy(DHCP_GET.Mac, Config_Msg.Mac, 6);
}

