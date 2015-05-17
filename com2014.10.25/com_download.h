#ifndef _COM_DOWNLOAD_H_
#define _COM_DOWNLOAD_H_
#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include <windows.h>
#include "hex2ram.h"

extern HANDLE fh;
extern void init_com(char *port);
extern void close_com();
char send_packet(unsigned char *packet,int packet_num);   
char dl_port(unsigned char *ram,char x,short length);//通道x下载长度为length的ram数据
void FLASH_ERASE();
int DOWNLOAD_PACKET(DATAPACKET *temp);
extern struct DATAPACKET data_packet[2000];
#endif