#include "stm32f4xx.h" 
#include "stm32f4xx_flash.h"
#include "delay.h"
#include "config.h"
#include "w5500.h"
#include "socket.h"
#include "util.h"
#include "loopback.h"
#include "led.h"
#include <stdio.h>
#include <string.h>
#include "my_spi.h"
#include "w5500_conf.h"
#include "at24cxx.h"
#include "dfu.h"

#define tick_second 1

extern  uint8 ch_status[MAX_SOCK_NUM];
extern  CHCONFIG_TYPE_DEF Chconfig_Type_Def;   //端口号对应的通信方式
extern  CONFIG_MSG Config_Msg;

extern  uint32_t presentTime;

extern  uint8  allow_flag_far; 
extern  uint8  tcpc_flag;

void    crc8(uint8 *buf,unsigned int len);

uint16  any_port = 1000;

extern  uint8 Tr_flg;
extern  uint8 Port0;
extern  uint8 Port1;

extern  CONFIG_MSG Config_Msg;

extern  uint8 test;

uint8   tcps_enable=0;
uint8   tcpc_enable=0;
uint8   Tx_Buffer[1024];

uint32_t int_arr[CODE_BYTE_NUM/4+1];

/* 变量 ----------------------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t  JumpAddress;

struct _duf_info dfu_info;

void SoftwareReset(void)
{
    __set_FAULTMASK(1);    //close all interrupts;
    NVIC_SystemReset();   
}

void start_ok(void)
{
    int j=0;
    Tx_Buffer[j++] = 'S'; Tx_Buffer[j++] = 'T'; Tx_Buffer[j++] = 'A'; Tx_Buffer[j++] = 'R';Tx_Buffer[j++] = 'T';
    Tx_Buffer[j++] = ' ';
    Tx_Buffer[j++] = 'O'; Tx_Buffer[j++] = 'K';
    Tx_Buffer[j++] = '\n'; 
    send(4,Tx_Buffer, j);	
}

void rec_err(void)
{
    int j=0;
    Tx_Buffer[j++] = 'R'; Tx_Buffer[j++] = 'C'; Tx_Buffer[j++] = 'V';
    Tx_Buffer[j++] = ' ';
    Tx_Buffer[j++] = 'E'; Tx_Buffer[j++] = 'R'; Tx_Buffer[j++] = 'R';
    Tx_Buffer[j++] = '\n'; 
    send(4,Tx_Buffer, j);	    
}

void end_ok(void)
{
    int j=0;
    Tx_Buffer[j++] = 'E'; Tx_Buffer[j++] = 'N'; Tx_Buffer[j++] = 'D';
    Tx_Buffer[j++] = ' ';
    Tx_Buffer[j++] = 'O'; Tx_Buffer[j++] = 'K';
    Tx_Buffer[j++] = '\n'; 
    send(4,Tx_Buffer, j);	   
}

void rec_ok(void)
{
    int j=0;
    Tx_Buffer[j++] = 'R';Tx_Buffer[j++] = 'C';Tx_Buffer[j++] = 'V';
    Tx_Buffer[j++] = ' ';
    Tx_Buffer[j++] = 'O';Tx_Buffer[j++] = 'K';
    Tx_Buffer[j++] = '\n'; 
    send(4,Tx_Buffer, j);	    
}

uint32_t GetSector(uint32_t Address)
{
		uint32_t sector = 0;
		
		if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
		{
			sector = FLASH_Sector_0;  
		}
		else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
		{
			sector = FLASH_Sector_1;  
		}
		else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
		{
			sector = FLASH_Sector_2;  
		}
		else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
		{
			sector = FLASH_Sector_3;  
		}
		else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
		{
			sector = FLASH_Sector_4;  
		}
		else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
		{
			sector = FLASH_Sector_5;  
		}
		else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
		{
			sector = FLASH_Sector_6;  
		}
		else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
		{
			sector = FLASH_Sector_7;  
		}
		else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
		{
			sector = FLASH_Sector_8;  
		}
		else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
		{
			sector = FLASH_Sector_9;  
		}
		else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
		{
			sector = FLASH_Sector_10;  
		}
		else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
		{
			sector = FLASH_Sector_11;  
		}
		return sector;
}


int FLASH_WriteInt(uint32_t DestAddr, uint32_t code[], uint32_t n)
{
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    int i;
    
    uint32_t SectorNum;
    
    SectorNum = GetSector(DestAddr);
    
    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();
    
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    //erase the page of address; 
    if (FLASH_EraseSector(SectorNum, VoltageRange_3) == FLASH_COMPLETE)
    {
        for (i = 0; i < n; i++)
        {
            FLASHStatus = FLASH_ProgramWord(DestAddr, code[i]);
            if (FLASHStatus != FLASH_COMPLETE)
            { return 1; }
            DestAddr += 4;
        }
    }
    else 
    {
        return 1;
    }
    FLASH_Lock();
    return 0;
}

void init_dfu(void)
{
    int i;
    dfu_info.start_flag = 0;
    dfu_info.end_flag = 0;
    dfu_info.rec_cnt = 0;
    dfu_info.code_len = 0;
    for (i = 0; i < CODE_BYTE_NUM; i++)
    {
        dfu_info.code_byte[i] = 0;
    }
}

void iap_init(void)
{
    uint8_t retval[1]={0};
 
    init_dfu();
    
    AT24CXX_ReadnByte(100,retval,1);
		
    //if no update, jump to APP;
    if (retval[0] != 0x33)
    {
        //printf("ApplicationAddress=%d, %d\r\n", *(vu32*)ApplicationAddress, ((*(vu32*)ApplicationAddress) & 0x2FFE0000));
        //judge weather have code in the address.
        if (((*(vu32*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
        {
              //printf("goto APP now!\r\n");
              //跳转至用户代码
              JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
              Jump_To_Application = (pFunction) JumpAddress;

              //初始化用户程序的堆栈指针
              __set_MSP(*(__IO uint32_t*) ApplicationAddress);
              Jump_To_Application();
        }
        else //prevent program run fly;
        {
            //printf("DUF_FLAG = %d\r\n", retval);
            //printf("No APP\r\n");
            while (1)
            {
							retval[0]=0x33;
							AT24CXX_Write(100,retval,1);
              delay_us(10000);
              SoftwareReset();
            }
        }
    }
   // printf("waiting for update...\r\n");
}

uint8_t data_check(int len,u8 *rx)
{
    uint8_t retval = 0;
    int i;
    
    for (i = 0; i < len; i++)
    {
        //retval ^= Rx_Buffer[2+i]; 
			  retval ^= *(rx+2+i);
    }
    return retval;
}

/* if no 4 bytes, high byte should be FF */
int array_charToint(unsigned char *cArr, uint32_t *intArr, int n)
{
    int i = 0, j = 0, cnt = 0, remd = 0;
    union {
        unsigned char c[4];
        unsigned int  n;
    }num;
    
    cnt = n/4;
    remd = n%4;
    
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < 4; j++)
        {
            num.c[j] = cArr[j+4*i];
        }
        intArr[i] = num.n;
    }
    
    if (remd != 0)
    {
        for (j = 0; j < remd; j++)
        {
            num.c[j] = cArr[j+4*i];
        }
        for (j = remd; j < 4; j++)
        {
            num.c[j] = 0xFF;
        }
        intArr[i] = num.n;
    }
    return (i);
}

/* ............................................................... 
*   return 1: fail;
*   return 0: success;
*   len: data length;
*/
int save_code(int len,u8 *rx)
{
    int i;
    if ((dfu_info.rec_cnt % 128) != 0) //save to dfu buffer;
    {
        for (i = 0; i < len; i++)
        {
            //dfu_info.code_byte[1024*(dfu_info.rec_cnt-1) + i] = Rx_Buffer[i+2];
            //dfu_info.code_byte[dfu_info.code_len + i] = Rx_Buffer[i+2];
					  dfu_info.code_byte[dfu_info.code_len + i] = *(rx+i+2);
        }
        dfu_info.code_len +=len;
    }
    else
    {
    }
    return 0;
}

int save_to_flash(void)
{
    int n, retval;
    //uint32_t int_arr[CODE_BYTE_NUM/4+1]; //512*128

    n = array_charToint(dfu_info.code_byte, int_arr, dfu_info.code_len);

    retval = FLASH_WriteInt(1024*128*(dfu_info.rec_cnt/128) + ApplicationAddress, int_arr, n);
  
	  if (retval == 1)
    {
        return 1;
    }
		
    return 0;
}

int data_process(u8 *Rx_Buffer)
{
    uint16_t data_len;
	
    u8 end[1] = {0xff};
		
    if (0 == strncmp((char*)Rx_Buffer, "END\n",  4))
    {
        if (save_to_flash() == 0)
        {
            end_ok();
        }
        
        //I2C_Write(DFU_EE_ADDR, 0xFF);//set eeprom flag;
				AT24CXX_Write(100,end,1);
        delay_us(30000);
        SoftwareReset();
    }
    else
    {
        data_len = (Rx_Buffer[0]<<8) + (Rx_Buffer[1]);
        if (data_len > (1024+1))
        {
            return 1;
        }
        if (data_check(data_len-1,Rx_Buffer) != Rx_Buffer[data_len+1])
        {
            return 1;
        }
        dfu_info.rec_cnt++;
       
        if (save_code(data_len-1,Rx_Buffer))
        {
            dfu_info.rec_cnt--;
            return 1;
        }
    }
		
    return 0;
}

//字符串拷贝函数
void StrBufCopy(uint8 *src,uint8 spos, uint8 *dec,uint8 dpos, uint8 len ){
	
	 uint8 icnt =0;
	
	 for(icnt =0; icnt<len; icnt++){
		 *(dec+dpos+icnt)=*(src+spos+icnt);
	 }
	 
}

//服务器模式
void loopback_tcps(SOCKET s, uint16 port)
{
    uint16  RSR_len;
    uint8   * data_buf = TX_BUF;

    switch (getSn_SR(s))
    {
      case SOCK_ESTABLISHED:                                   /* if connection is established */
        
  			if(ch_status[s]==1) {
            ch_status[s] = 2;
        }
        if ((RSR_len = getSn_RX_RSR(s)) > 0)                   /* check Rx data */
        {
					if (RSR_len > TX_RX_MAX_BUF_SIZE)
                 RSR_len = TX_RX_MAX_BUF_SIZE;                 /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE the data size to read is MAX_BUF_SIZE. */
            recv(s, data_buf, RSR_len); 
					
					 if (dfu_info.start_flag == 1) //receive data;
            {
              if (data_process(data_buf))
              { rec_err();}
              else
              { rec_ok(); }
            }
            else if (0 == strncmp((char *)data_buf, "START\n",  6))
            {
              dfu_info.start_flag = 1;
              start_ok();
            } 
						
        }
        break;

      case SOCK_CLOSE_WAIT:                                     /* If the client request to close */
        if ((RSR_len = getSn_RX_RSR(s)) > 0)                    /* check Rx data */
        {
            if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;  /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
            /* the data size to read is MAX_BUF_SIZE. */
            recv(s, data_buf, RSR_len);          /* read the received data */
        }
        disconnect(s); ch_status[s] = 0;
        break;

      case SOCK_CLOSED:        /* if a socket is closed */
				
        if(!ch_status[s]) { ch_status[s] = 1; }
        if(socket(s,(Sn_MR_ND|Sn_MR_TCP),port,0x00) == 0) {
            ch_status[s] = 0;  /* reinitialize the socket */
        }
        break;

      case SOCK_INIT:          /* if a socket is initiated */
        listen(s);
        break;
			
      default:

        break;
    }
}

