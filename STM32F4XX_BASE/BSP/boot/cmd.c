/*
*   cmd.c
*/
#include "stdio.h"
#include "cmd.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "dfu.h"

//lan
__IO uint8_t RxDat[100];
__IO uint8_t RxComplete=0x00;

//0:temp--034; 1:setswitch--054
__IO uint8_t radio3200_err_code[10];



/*
*   letter positioning; 
*   single command;
*/
struct _cmd_info cmd_num_f(void)
{
    int i, flag = 0;
    struct _cmd_info cmd;
    
    cmd.cmd_num = 0;
    cmd.end_posi = -1;
    
    for (i = 0; i < 100; i++)
    {
        if (RxDat[i] == '\n')
        {
            flag = 1;
            cmd.end_posi = i;
            break;
        }
        if (i == 99)
        {
            cmd.cmd_num = 0;
            cmd.end_posi = -1;
            return cmd;
        }
    }
    
    if (flag == 1)
    {
        for (i = 0; i < 80; i++)
        {
            if (RxDat[i] == '*')
            {
                cmd.cmd_num = 1;
                return cmd; ; //*IDN?
            }
            else if (RxDat[i] == 'R')
            {
                if ((RxDat[i+16] == ':') && (RxDat[i+6] == 'P'))
                {
                    cmd.cmd_num = 6;
                    cmd.end_posi = i+16;
                    return cmd; ; //set channel
                }
                if (RxDat[i+22] == '\n')
                {
                    cmd.cmd_num = 7;
                    cmd.end_posi = i+22;
                    return cmd; ; //query channel
                }
                if (RxDat[i+11] == 'E')
                {
                    cmd.cmd_num = 33;
                    cmd.end_posi = i+11;
                    return cmd; ; //remote update;
                }
            }
            else if (RxDat[i] == 'D')
            {
                if (RxDat[i+15] == ':')
                {
                    cmd.cmd_num = 2;
                    cmd.end_posi = i+15;
                    return cmd; ; //temp
                }
            }
            else if (RxDat[i] == 'S')
            {
                if (RxDat[i+12] == '?')
                {
                    cmd.cmd_num = 3;
                    return cmd; ; //sys error
                }
                if (RxDat[i+18] == '\n')
                {
                    cmd.cmd_num = 4;
                    return cmd; ; //cls error
                }
                if (RxDat[i+15] == '\n')
                {
                    cmd.cmd_num = 5;
                    return cmd; ; //sys ver
                }
                if (RxDat[i+7] == 'P')
                {
                    cmd.cmd_num = 8;
                    return cmd; ; //reset
                }
            }
        }
    }
    
    return cmd;
}

void empty_RxDat(void)
{
    int i;
    for (i = 0; i < sizeof(RxDat); i++)
    {
        RxDat[i] = 0;
    }
}

////////////////////////////////////////////////////////
int find_char(char c, int start)
{
    while (RxDat[++start] != c) 
    {
        if (start >= 80)
        {
            return -1;
        }
    }
    
    return (start);
}

struct _chchange_info 
{
    int cnt;
    int posi[20];
};


struct _chchange_info count_char(char c, int start, int end)
{
    struct _chchange_info ch_info;
    int i;
    
    ch_info.cnt = 0;
    for (i = 0; i < sizeof(ch_info.posi)/sizeof(int); i++)
    {
        ch_info.posi[i] = 0;
    }
    
    while (start <= end)
    {
        if (RxDat[++start] == c)
        {
            ch_info.cnt++;
            ch_info.posi[ch_info.cnt] = start;
        }
    }    
    
    return ch_info;
}
//0:ok, 1:err
int check_tx(int tx)
{
    if ((tx >= 1) && (tx <= 4))
    {
        return 0;
    }
    else
        return 1;//err;
}
int check_sx(int tx)
{
    if ((tx >= 1) && (tx <= 16))
    {
        return 0;
    }
    else
        return 1;//err;
}
//all_zero:1, 0:normal;   && (arr[i] >= little) && (arr[i] <= max))
int all_zero(int *arr, int n, int little, int max)
{
    int i;
    
    for (i = 0; i < n; i++)
    {
        if (arr[i] == 0)
        {
            break;
        }
        if ((arr[i] < little) || (arr[i] > max))
        {
            return 1;
        }
    }
    
    return 0;
}

void clear_buf_int(int *arr, int n)
{
    int i;
    
    for (i = 0; i < n; i++)
    {
        arr[i] = 0;
    }
}

//1: there is;
int is_there_element(int arr[], int n, int ele)
{
    int i;
    
    for (i = 0; i < n; i++)
    {
        if (arr[i] == ele)
        {
            return 1;
        }
    }
    
    return 0;
}


void dfu_mode(void)
{
    int i = 0;
    
    //save the flag; DFU_EE_ADDR
    I2C_Write(100, DFU_FLAG);
    Delayus(10000);
    
    //response
    Tx_Buffer[i++] = 'R';Tx_Buffer[i++] = 'E';Tx_Buffer[i++] = 'M';Tx_Buffer[i++] = 'O'; Tx_Buffer[i++] = 'T';Tx_Buffer[i++] = 'E';
    Tx_Buffer[i++] = 'U';Tx_Buffer[i++] = 'P';Tx_Buffer[i++] = 'D';Tx_Buffer[i++] = 'A'; Tx_Buffer[i++] = 'T';Tx_Buffer[i++] = 'E';
    Tx_Buffer[i++] = ' ';
    Tx_Buffer[i++] = 'O';Tx_Buffer[i++] = 'K';
    Tx_Buffer[i++] = '\n'; 
    
    S_tx_process(0, i);    
    
    //soft reset;
    SoftwareReset();
}

uint8_t data_check(int len)
{
    uint8_t retval = 0;
    int i;
    
    for (i = 0; i < len; i++)
    {
        retval ^= Rx_Buffer[2+i]; 
    }
    
    return retval;
}

/* ............................................................... 
*   return 1: fail;
*   return 0: success;
*   len: data length;
*/
int save_code(int len)
{
    int i;
    
    if ((dfu_info.rec_cnt % 128) != 0) //save to dfu buffer;
    {
        for (i = 0; i < len; i++)
        {
            //dfu_info.code_byte[1024*(dfu_info.rec_cnt-1) + i] = Rx_Buffer[i+2];
            dfu_info.code_byte[dfu_info.code_len + i] = Rx_Buffer[i+2];
        }
        
        dfu_info.code_len +=len;
    }
    else
    {
        //save 128 packet data;
        //save_to_flash():
        //clear dfu buffer;
    }
    
    return 0;

#if 0    
    if (dfu_info.rec_cnt%2 == 0) //even;
    {
        for (i = 0; i < len; i++)
        {
            dfu_info.code_byte[i+1024] = Rx_Buffer[i+2];
        }
        n = array_charToint(dfu_info.code_byte, int_arr, 1024+len);
//printf("dfu_info.rec_cnt = %d, n = %d\r\n", dfu_info.rec_cnt, n);
        retval = FLASH_WriteInt(2048*(dfu_info.rec_cnt/2 - 1) + ApplicationAddress, int_arr, n);
        if (retval == 1)
        {
            return 1;
        }
        for (i = 0; i < 2048; i++) //clear;
        {
            dfu_info.code_byte[i] = 0;
        }
    }
    else //odd;
    {
        for (i = 0; i < len; i++)
        {
            dfu_info.code_byte[i] = Rx_Buffer[i+2];
        }
        dfu_info.code_len = len;
    }
    
    return 0;
#endif
}

int save_RxBuffer(void)
{
    int n, retval;
    uint32_t int_arr[512];
    
    n = array_charToint(dfu_info.code_byte, int_arr, dfu_info.code_len);
    retval = FLASH_WriteInt(2048*((dfu_info.rec_cnt+1)/2 - 1) + ApplicationAddress, int_arr, n);
    if (retval == 1)
    {
        return 1;
    }
    
    return 0;
}

uint32_t int_arr[CODE_BYTE_NUM/4+1]; //512*128

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

int data_process(void)
{
    uint16_t data_len;
    
    if (0 == strncmp((char *)Rx_Buffer, "END\n",  4))
    {
        if (save_to_flash() == 0)
        {
            end_ok();
        }
        
        I2C_Write(DFU_EE_ADDR, 0xFF);//set eeprom flag;
        Delayus(30000);
        SoftwareReset();
    }
    else
    {
        data_len = (Rx_Buffer[0]<<8) + (Rx_Buffer[1]);
        if (data_len > (1024+1))
        {
            return 1;
        }
        if (data_check(data_len-1) != Rx_Buffer[data_len+1])
        {
            return 1;
        }
        dfu_info.rec_cnt++;
       
        if (save_code(data_len-1))
        {
            dfu_info.rec_cnt--;
            return 1;
        }
    }
    
    return 0;
}


//-------------------------------------------lan process part ------------------------
void lan_process(void)
{
    socket_receive();
    
    if (RxComplete == 1)
    {
        RxComplete = 0;
        
        if (dfu_info.start_flag == 1) //receive data;
        {
            if (data_process())
            {
                rec_err();
                
            }
            else
            {
                rec_ok();
            }
        }
        else if (0 == strncmp((char *)Rx_Buffer, "START\n",  6))
        {
            dfu_info.start_flag = 1;
            start_ok();
        }
    }
}


//-------------------------------------------uart process part ------------------------------------------------
#if 1
void ret_ip_info(void)
{
    Phy_Addr[0] = I2C_Read(MAC_EE+0);
    Phy_Addr[1] = I2C_Read(MAC_EE+1);
    Phy_Addr[2] = I2C_Read(MAC_EE+2);
    Phy_Addr[3] = I2C_Read(MAC_EE+3);
    Phy_Addr[4] = I2C_Read(MAC_EE+4);
    Phy_Addr[5] = I2C_Read(MAC_EE+5);
    
	printf("IP Address:%d.%d.%d.%d\r\n",IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3]);

	printf("Net Mask:%d.%d.%d.%d\r\n",Sub_Mask[0],Sub_Mask[1],Sub_Mask[2],Sub_Mask[3]);

	printf("Net Gateway:%d.%d.%d.%d\r\n",Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3]);
    
    printf("Mac Address:%02x-%02x-%02x-%02x-%02x-%02x\r\n",Phy_Addr[0], Phy_Addr[1], Phy_Addr[2], Phy_Addr[3], Phy_Addr[4], Phy_Addr[5]);
}

/*
*   change a str to a int; 暂不考虑非法输入；
*/
int my_atoi(const char* str)
{
    int num = 0, flag = 0;
    const char* digit = NULL;
    
    if (str == NULL)
        return -1;
    digit = str;
    if (*digit == '+')
    {
        digit++;
    }
    else if(*digit == '-')
    {
        digit++;
        flag = 1; //1 -- minus;
    }
    
    while (*digit != '\0')
    {
        if (*digit >= '0' && *digit <= '9')
        {
            num = num * 10 + (*digit - '0');
            digit++;
        }
        else
        {
            num = 0;
            break;
        }
    }
    
    if (flag == 1)
        num = 0 - num;
    
    return (int)num;
}

void set_ip(void)
{
    IP_Addr[0] = my_atoi(strtok(NULL, "."));
    IP_Addr[1] = my_atoi(strtok(NULL, "."));
    IP_Addr[2] = my_atoi(strtok(NULL, "."));
    IP_Addr[3] = my_atoi(strtok(NULL, "."));
    Write_Load_Net_Parameters();   
    printf("SetIP Success");
}

void set_mask(void)
{
    Sub_Mask[0] = my_atoi(strtok(NULL, "."));
    Sub_Mask[1] = my_atoi(strtok(NULL, "."));
    Sub_Mask[2] = my_atoi(strtok(NULL, "."));
    Sub_Mask[3] = my_atoi(strtok(NULL, "."));
    Write_Load_Net_Parameters();  
    printf("SetNetMask Success");
}

void set_gateway(void)
{
    Gateway_IP[0] = my_atoi(strtok(NULL, "."));
    Gateway_IP[1] = my_atoi(strtok(NULL, "."));
    Gateway_IP[2] = my_atoi(strtok(NULL, "."));
    Gateway_IP[3] = my_atoi(strtok(NULL, "."));
    Write_Load_Net_Parameters();      
    printf("SetGateway Success");
}

//number save;
void set_sn(void)
{
    uint8_t s1, s2, s3;
    s1 = (uint8_t)my_atoi(strtok(NULL, "."));
    s2 = (uint8_t)my_atoi(strtok(NULL, "."));
    s3 = (uint8_t)my_atoi(strtok(NULL, "."));
    
    I2C_Write(SN_3_1, s1);
    I2C_Write(SN_3_2, s2);
    I2C_Write(SN_3_3, s3);
    
    printf("SN_num = %d, %d, %d\n", s1, s2, s3);
}

int a_to_h(char *s)
{
    int i,t;
    long sum=0;
    //for(i=0;s[i];i++)
    for(i=0;i < 2;i++)
    {
        if(s[i]<='9')t=s[i]-'0';
        else  t=s[i]-'A'+10;
        sum=sum*16+t;
    }
    return sum;
}

//set mac;
void set_mac(void)
{
    Phy_Addr[0] = a_to_h(strtok(NULL, "."));
    Phy_Addr[1] = a_to_h(strtok(NULL, "."));
    Phy_Addr[2] = a_to_h(strtok(NULL, "."));
    Phy_Addr[3] = a_to_h(strtok(NULL, "."));
    Phy_Addr[4] = a_to_h(strtok(NULL, "."));
    Phy_Addr[5] = a_to_h(strtok(NULL, "."));
    
    I2C_Write(MAC_EE+0, Phy_Addr[0]);
    I2C_Write(MAC_EE+1, Phy_Addr[1]);
    I2C_Write( MAC_EE+2, Phy_Addr[2]);
    I2C_Write( MAC_EE+3, Phy_Addr[3]);
    I2C_Write( MAC_EE+4, Phy_Addr[4]);
    I2C_Write( MAC_EE+5, Phy_Addr[5]);
    
    printf("Mac Address:%02x-%02x-%02x-%02x-%02x-%02x\r\n",
              Phy_Addr[0], Phy_Addr[1], Phy_Addr[2], Phy_Addr[3], Phy_Addr[4], Phy_Addr[5]); 
}

void net_cmd_process(void)
{
    char *token = NULL;
    
    if (Usart1Rx[0] == 'i') //ifconfig;
    {
        ret_ip_info();
    }
    else 
    {
        token = strtok((char *)Usart1Rx, " ");
        if (strncmp(token, "SetIP", 5) == 0)
        {
            set_ip();   
        }
        else if (strncmp(token, "SetNetMask", 10) == 0)
        {
            set_mask();
        }
        else if (strncmp(token, "SetGateway", 10) == 0)
        {
            set_gateway();
        }
                else if (strncmp(token, "AUMI_SN", 7) == 0)
        {
            set_sn();
        }
                else if (strncmp(token, "AUMI_MAC", 8) == 0)
        {
            set_mac();
        }
    }
}

//uart_cmd
void end_point_char(void)
{
    int i, end, start = 0;    
    char flag = 1;
    
    if (Usart1Rx[0] != 'A')
    {
        for (i = CMD_BUFF_SIZE - 1; i > -1 ; i--)
        {
            if (flag == 1)
            {
                if ((Usart1Rx[i] == 'g') || (Usart1Rx[i] >= '0' && Usart1Rx[i] <= '9'))
                {
                    Usart1Rx[i+1] = '.';
                    end = i+1;
                    flag = 0;
                }
            }
            
            if ((Usart1Rx[i] == 'i') || (Usart1Rx[i] == 'S'))
            {
                start = i;
                break;
            }
        }
    }
    else if (Usart1Rx[0] == 'A')
    {
        start = 0;
        if (Usart1Rx[6] == 'N')
        {
            Usart1Rx[13] = '.';
            end = 13;
        }
        else if (Usart1Rx[6] == 'A')
        {
            Usart1Rx[26] = '.';
            end = 26;
        }
    }
    
    for (i = start; i <= end; i++)
    {
        Usart1Rx[i-start] = Usart1Rx[i];
    }
}

void uart_process(void)
{
    int i;
    
	if (uart_cmd_flag > 0)
	{        
        //find \n=0x0a to .;
        end_point_char();
        
		//reset_ip();
        net_cmd_process();

        for (i = 0; i < CMD_BUFF_SIZE; i++)
        {
            Usart1Rx[i] = 0;
        }
        uart_cmd_flag = 0;
        Usart1Counter = 0;
	}
}
#endif
