/*
*
@file		util.h
@brief	
*/

#ifndef _UTIL_H
#define _UTIL_H
#include "Types.h" 
void Set_network(void);
void Reset_W5500(void);
void itoa(uint16 n,uint8 str[5], uint8 len);
int  validatoi(char* str, int base,int* ret);
void replacetochar(char * str,char oldchar,char newchar	);

char c2d(uint8 c);
uint16 atoi16(char* str, uint16 base);
uint32 atoi32(char* str,uint16 base);
uint16 swaps(uint16 i);
uint32 swapl(uint32 l);
void mid(int8* src, int8* s1, int8* s2, int8* sub);
uint8 inet_addr_(unsigned char* addr,unsigned char *ip);
char* inet_ntoa(unsigned long addr);
char* inet_ntoa_pad(unsigned long addr);
char verify_ip_address(char* src, uint8 * ip);
uint16 htons( uint16 hostshort );
unsigned long htonl( unsigned long hostlong	);
unsigned long ntohs( unsigned short netshort);
unsigned long ntohl( unsigned long netlong);
unsigned short checksum( unsigned char * src,unsigned int len);
uint8 check_dest_in_local(u_long destip);
extern uint32 time_return(void);
#endif
