#ifndef __CMD_H__
#define __CMD_H__


#include "stm32f4xx.h"

struct _cmd_info
{
    int cmd_num;
    int end_posi;
};

typedef enum  
{
    NO_CMD,
    REDA_IDN,
    RESET_DEV,
    SET_CH_ON,
    SET_CH_OFF,
    GET_SINGLE_CH_STATUS,
    GET_ALL_CH_STATUS,
    SET_CH_PA,
    GET_SINGLE_CH_PA,
    GET_ALL_CH_PA,
    SET_MUL_CH_ON,
    SET_MUL_CH_OFF,
    SET_MUL_CH_PA,
}cmd_value;

extern __IO uint8_t RxDat[100];
extern __IO uint8_t RxComplete;

int set_attenuation(int tx, int sx, uint16_t attn_val);

void uart_process(void);
void lan_process(void);

#endif
