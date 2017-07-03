/*
*   DFU: device fireware upgrade
*   dfu.c
*/
#include "stdio.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"
#include "core_cm4.h"
#include "uart.h"
#include "delayus.h"
#include "AT24C02.h"
#include "cmd.h"
#include "w5200.h"
#include "ds18b20.h"
#include "dfu.h"

/* 变量 ----------------------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;

struct _duf_info dfu_info;


void SoftwareReset(void)
{
    __set_FAULTMASK(1); //close all interrupts;
    NVIC_SystemReset();//
}

void start_ok(void)
{
    int i=0;
    
    Tx_Buffer[i++] = 'S';Tx_Buffer[i++] = 'T';Tx_Buffer[i++] = 'A';Tx_Buffer[i++] = 'R';Tx_Buffer[i++] = 'T';
    Tx_Buffer[i++] = ' ';
    Tx_Buffer[i++] = 'O';Tx_Buffer[i++] = 'K';
    
    Tx_Buffer[i++] = '\n'; 
    
    S_tx_process(0, i);      
}

void end_ok(void)
{
    int i=0;
    
    Tx_Buffer[i++] = 'E';Tx_Buffer[i++] = 'N';Tx_Buffer[i++] = 'D';
    Tx_Buffer[i++] = ' ';
    Tx_Buffer[i++] = 'O';Tx_Buffer[i++] = 'K';
    
    Tx_Buffer[i++] = '\n'; 
    
    S_tx_process(0, i);      
}

void rec_ok(void)
{
    int i=0;
    
    Tx_Buffer[i++] = 'R';Tx_Buffer[i++] = 'C';Tx_Buffer[i++] = 'V';
    Tx_Buffer[i++] = ' ';
    Tx_Buffer[i++] = 'O';Tx_Buffer[i++] = 'K';
    
    Tx_Buffer[i++] = '\n'; 
    
    S_tx_process(0, i);      
}

void rec_err(void)
{
    int i=0;
    
    Tx_Buffer[i++] = 'R';Tx_Buffer[i++] = 'C';Tx_Buffer[i++] = 'V';
    Tx_Buffer[i++] = ' ';
    Tx_Buffer[i++] = 'E';Tx_Buffer[i++] = 'R';Tx_Buffer[i++] = 'R';
    
    Tx_Buffer[i++] = '\n'; 
    
    S_tx_process(0, i);      
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

#if 1 //test
void test(void)
{
    unsigned char c3[3] = {0x11, 0x22, 0x33}, c4[4] = {0x11, 0x22, 0x33, 0x44}, 
                    c9[9] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,0x99};
    uint32_t c[10] = {0};
    
    array_charToint(c3, c, 3);
    array_charToint(c4, c, 4);
    array_charToint(c9, c, 9);
}
#endif

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

/* 
    Ver: V1.0 
    note: no argus check;
    1: erase fail
        
*/
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
            {
                return 1;
            }
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


//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
void iap_init(void)
{
    uint8_t retval;
    
#if 0 //test
    test();
#endif

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOJ,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK,ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);  
    
    init_dfu();
    
    //usart1 init;
    USART_Configuration(); //usart1_config();    
    
    Delayus_Configuration(); //delayus_config();   //delay us 
    
    //init the eeprom;
    AT24C02_Configuration(); //I2C_PORT_Config();
    
    Delayus(10000);
    
    printf("IAP init ok...\r\n");
    
    //I2C_Write(DFU_EE_ADDR, 0xff); //DFU_FLAG
    //Delayus(10000);
    
    retval = I2C_Read(DFU_EE_ADDR);
    //if no update, jump to APP;
    if (retval != (uint8_t)DFU_FLAG)
    {
        //printf("ApplicationAddress=%d, %d\r\n", *(vu32*)ApplicationAddress, ((*(vu32*)ApplicationAddress) & 0x2FFE0000));
        //judge weather have code in the address.
        if (((*(vu32*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
        {
            printf("goto APP now!\r\n");

            //跳转至用户代码
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;

            //初始化用户程序的堆栈指针
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);
            Jump_To_Application();
        }
        else //prevent program run fly;
        {
            printf("DUF_FLAG = %d\r\n", retval);
            printf("No APP\r\n");
            while (1)
            {
                I2C_Write(DFU_EE_ADDR, DFU_FLAG);
                Delayus(10000);
                SoftwareReset();
            }
        }
    }
    printf("waiting for update...\r\n");
}

