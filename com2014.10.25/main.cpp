#include<iostream>
#include<cstdio>
#include<cassert>
#include<string>
#include<windows.h>
#include <stdlib.h>
#include "hex2ram.h"
#include "com_download.h"
using namespace std;

struct DATAPACKET data_packet[2000]={0};
unsigned char ram_data[40000];
int main(int argc, char **argv)
{
  // open hex file/
  //assert(argc == 3);
  int loop_i;
  int length;
  int port_count;
  char port_flag[51];
  unsigned char *current_pointer=ram_data;
  DATAPACKET current_packet;
  unsigned i;
  
  FILE *hex_file = fopen(argv[1], "rb");
  if (hex_file == NULL) {
    cerr<<"open hex file " << argv[1] << " error\n";
    exit(0);
  }
 // length=read_hex_file(hex_file,ram_data);
  length= hex2ram(hex_file,ram_data);
  fclose(hex_file);
  init_com((argv[2]));
  getchar();//
  FLASH_ERASE();
  i=0;
  while(i!=length)
 // while(read_line(current_pointer,&current_packet)==111)
  {
		
		print_datapacket(&data_packet[i]);
        getchar();//
		DOWNLOAD_PACKET( &data_packet[i]);
		current_pointer++;//move
		i++;
	/*	while(*current_pointer!=':')
			current_pointer++;*/
  }

  return 0;

}
