
#include "hex2ram.h"
#include "com_download.h"

unsigned int length_totle=0;

unsigned char hex2char(unsigned char *p)
{
	unsigned char temp=0;
	 unsigned char value=0;
	if('9'>=*p&&*p>='0')
		temp=*p-'0';
	if('F'>=*p&&*p>='A')
		temp=*p-'A'+10;
	if('f'>=*p&&*p>='a')
		temp=*p-'a'+10;
	//printf("%x\n",temp);
	value=(temp)*16;
	p=p+1;
	if('9'>=*p&&*p>='0')
		temp=*p-'0';
	if('F'>=*p&&*p>='A')
		temp=*p-'A'+10;
	if('f'>=*p&&*p>='a')
		temp=*p-'a'+10;
	value=value+temp;
	//printf("%x\n",temp);

	return value;
}

unsigned short cal_crc(unsigned char *ptr,unsigned char len) /*CRC-CCITT:X16+X12+X5+1*/
{
	unsigned char  i;
	unsigned short  crc_c=0xffff;
	while(len--)
	{
		for(i=0x80; i!=0; i>>=1)
		{
			if((crc_c&0x8000)!=0) {crc_c<<=1; crc_c^=0x1021;} 
			else crc_c<<=1; 
			if((*ptr&i)!=0) crc_c^=0x1021; 
		}
		ptr++;
	}
	return(crc_c);
}
int read_hex_file(FILE *hex_file,unsigned char *ram)
{
   fseek(hex_file, 0, SEEK_END);
   int size = ftell(hex_file);
   fseek(hex_file, 0, SEEK_SET);
   int count = 0;
   while (count < size) 
  {
	count += fread(ram + count, 1, size - count, hex_file);
//	cout << "read out " << count << endl;
	}
	 printf("%s",ram);
   return size;
}
int hex2ram(FILE *hex_file,unsigned char *ram)
{
	// get file size
  unsigned char checksum;
  unsigned short loop_comtrol_i;
  unsigned short loop_comtrol_j;
  unsigned char *origin_file;
  unsigned char *target_file;
  unsigned char origin_file_ended=0;
  unsigned char number;
  unsigned short addr;
  unsigned short length;
  unsigned short max_length=0;
  unsigned int value;
  fseek(hex_file, 0, SEEK_END);
  int size = ftell(hex_file);
  fseek(hex_file, 0, SEEK_SET);
//  cout << "hex file size = " << size << endl;

  // read out file content
  unsigned char *content = (unsigned char *)malloc(size);
  int count = 0;
  while (count < size) 
  {
	count += fread(content + count, 1, size - count, hex_file);
//	cout << "read out " << count << endl;
  }
  printf("%s",content);
  origin_file=content;
  target_file=ram;
	while(!origin_file_ended)
	{
		if(*origin_file==':')
		{
			*origin_file++;
			//printf("%x\n",hex2char(origin_file));
			number=hex2char(origin_file);//读开头的长度信息
			checksum=number;
			origin_file=origin_file+2;
			//printf("%d\n",number);
			if(number==0)
			{
				origin_file_ended=1;
			}
			else
			{
				addr=hex2char(origin_file)*256;//读开头的地址
				origin_file=origin_file+2;
				checksum+=addr/256;
				addr+=hex2char(origin_file);
				origin_file=origin_file+2;
				checksum+=addr;
				
				printf("%X  ",addr);
				length=(number+addr-BIAS)/8+1;
				if (length>max_length) max_length=length;
				printf("%d  ",max_length);
				value=hex2char(origin_file);//读类型
				origin_file=origin_file+2;
				checksum+=value;
				if(value==1)
				{
					origin_file_ended=1;
				}
				for(loop_comtrol_i=0;loop_comtrol_i<number;loop_comtrol_i++)
				{
					value=hex2char(origin_file);
					origin_file=origin_file+2;
					checksum+=value;
					ram[addr-BIAS+loop_comtrol_i]=value;
					//printf("%02X",ram[addr-BIAS+loop_comtrol_i]);
				}
				value=hex2char(origin_file);//校验
				origin_file=origin_file+2;
				checksum^=0xff;
				checksum+=1;
				printf("\n");
				if(checksum!=value)
				{
					printf("*error:Hex file check fail!!!\n");
					return 0;
				}
				//origin_file_ended=1;
				value=hex2char(origin_file);
				origin_file=origin_file+2;
				//value=hex2char(origin_file);
				//origin_file=origin_file+2;	
			}

			
		}

	}
	//max_length*=4;
	for(loop_comtrol_i=0;loop_comtrol_i<max_length;loop_comtrol_i++)
		{
			printf("0x%04X  ",BIAS+loop_comtrol_i*0x08);
			data_packet[loop_comtrol_i].addr=(unsigned short)(loop_comtrol_i*0x08);
			
			for(loop_comtrol_j=0;loop_comtrol_j<8;loop_comtrol_j++)
			{
				printf("%02X",ram[loop_comtrol_i*8+loop_comtrol_j]);
				data_packet[loop_comtrol_i].data[loop_comtrol_j]=ram[loop_comtrol_i*8+loop_comtrol_j];
				
			}
		//	length_totle=loop_comtrol_i;
			data_packet[loop_comtrol_i].length=8;
			printf("\n");
		}
	max_length=loop_comtrol_i+1;
	return max_length;
}

void print_datapacket(DATAPACKET *temp)
{
	char loop;
	printf("%d %X ",(*temp).length,(*temp).addr);
	for(loop=0;loop<32 && loop<(*temp).length;loop++)
		printf("%02X",(*temp).data[loop]);
	printf("  checksum=%x\n",(*temp).checksum);
}
unsigned short read_line(unsigned char *origin_file,DATAPACKET *temp)
{  //hex data  is    : + length + addr + type + data + checksum 
	unsigned char rd_buf;

	unsigned char checksum=0;
	unsigned int type;

	unsigned char   loop_i;
	if(*origin_file==':')
	{
		*origin_file++;
		rd_buf=hex2char(origin_file);//读开头的长度信息
		(*temp).length=rd_buf;
		checksum+=rd_buf;
		origin_file=origin_file+2;
		if(0==rd_buf)
		{
			return 777;//file ended!!!!
		}
		else 
		{
			rd_buf=hex2char(origin_file);
			(*temp).addr=rd_buf*256;//读开头的地址
			origin_file=origin_file+2;
			checksum+=rd_buf;

			rd_buf=hex2char(origin_file);
			(*temp).addr+=rd_buf;
			origin_file=origin_file+2;
			checksum+=rd_buf;
			//printf("%X  ",addr);

			type=hex2char(origin_file);//读类型
			origin_file=origin_file+2;
			checksum+=type;
			if(type==1)
		    {
				return 888;//file ended!!!!
				}
			for(loop_i=0;loop_i<(*temp).length;loop_i++)
			{
				(*temp).data[loop_i]=hex2char(origin_file);
				origin_file=origin_file+2;
				checksum+=(*temp).data[loop_i];
			//	printf("%02X",value);
				}
				(*temp).checksum=hex2char(origin_file);//校验
				origin_file=origin_file+2;
				checksum^=0xff;
				checksum+=1;
				printf("\n");
				if((*temp).checksum!=checksum)
				{
					printf("*error:Hex file check fail!!!\n");
					return 0;
				}
				//origin_file_ended=1;
				//value=hex2char(origin_file);
				origin_file=origin_file+2;
		}
		return 111;
	}
}
