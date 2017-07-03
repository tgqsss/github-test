/* dfu.h */

#ifndef __DFU_H__
#define __DFU_H__

#include "stm32f4xx.h"

#define ApplicationAddress    0x8020000

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* ¿‡–Õ…˘√˜ -----------------------------------------------------------------*/
typedef  void (*pFunction)(void);

#define CODE_BYTE_NUM  1024*60

struct _duf_info
{
    int start_flag;  
    int end_flag;
    int rec_cnt;
    int code_len;
    unsigned char code_byte[CODE_BYTE_NUM];    
};

extern struct _duf_info dfu_info;

void iap_init(void);
void start_ok(void);
void end_ok(void);
void rec_ok(void);
void rec_err(void);
void SoftwareReset(void);

int FLASH_WriteInt(uint32_t DestAddr, uint32_t code[], uint32_t n);
int array_charToint(unsigned char *cArr, uint32_t *intArr, int n);

#endif
