#ifndef _HEX2RAM_H
#define _HEX2RAM_H
#include<iostream>
#include<cstdio>
#include<cassert>
#include<string>
#include<windows.h>
#include <stdlib.h>



#define BIAS 0//0x4000

#define TIME_OUT 3000


struct DATAPACKET
{ 
	unsigned char  length;
	unsigned short addr;
	unsigned char  data[16];
	unsigned char  checksum;
};


extern int hex2ram(FILE *hex_file,unsigned char *ram);
extern int read_hex_file(FILE *hex_file,unsigned char *ram);
unsigned short read_line(unsigned char *origin_file,DATAPACKET *temp);
extern int hex2ram(FILE *hex_file,unsigned char *ram);
void print_datapacket(DATAPACKET *temp);
#endif