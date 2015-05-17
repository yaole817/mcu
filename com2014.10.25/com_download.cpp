#include "com_download.h"
#include "hex2ram.h"
HANDLE fh;


using namespace std;     
void init_com(char *port)
{
  //char *port = "COM1";
  char *options = "115200,n,8,1";
  DCB dcb;
  COMMTIMEOUTS cmt;

  fh = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
  if (fh == INVALID_HANDLE_VALUE) {
    cerr<<"open port error\n";
    exit(0);
  }

  // configure options
  memset(&dcb, 0, sizeof(dcb));
  dcb.DCBlength = sizeof(dcb);
  assert(BuildCommDCB(options, &dcb) != 0);
  assert(SetCommState(fh, &dcb) != 0);
  assert(SetupComm(fh, 1024, 1024) != 0);

  //set time out
  cmt.ReadIntervalTimeout = TIME_OUT;
  cmt.ReadTotalTimeoutMultiplier = TIME_OUT;
  cmt.ReadTotalTimeoutConstant = TIME_OUT;
  cmt.WriteTotalTimeoutConstant = 1000;
  cmt.WriteTotalTimeoutMultiplier = 1000;
  assert(SetCommTimeouts(fh, &cmt) != 0);

  cerr<<"sucess open COM\n";
}

void close_com()
{
	CloseHandle(fh);
}

unsigned char wait_ack()
{
	unsigned char ack_buf=0;
	DWORD wc;
	ReadFile(fh, &ack_buf,1, &wc, NULL);
	return ack_buf;
}
void read_com(unsigned char *buf,int len)
{
	DWORD wc;
	ReadFile(fh, &buf,len, &wc, NULL);
}

void write_com_char(unsigned char p)
{
	DWORD wc;
	WriteFile(fh,&p,1,&wc, NULL);
}
void write_com_2char(unsigned short p)
{
	unsigned char p_h,p_l;
	p_h = (unsigned char)((p>>8)&0xff);
	p_l = (unsigned char)(p&0xff);
	write_com_char(p_h);
	write_com_char(p_l);
}
void write_com_string(unsigned char *p,char i)
{
	DWORD wc;
	WriteFile(fh,p,i, &wc, NULL);
}
unsigned short cal_crc(unsigned short addr,unsigned char *ptr,unsigned char len) /*CRC-CCITT:X16+X12+X5+1*/
{
	unsigned char  i;
	unsigned char  temp;
	unsigned short  crc_c=0xffff;
	temp=addr/256;
	for(i=0x80; i!=0; i>>=1)
		{
			if((crc_c&0x8000)!=0) {crc_c<<=1; crc_c^=0x1021;} 
			else crc_c<<=1; 
			if((temp&i)!=0) crc_c^=0x1021; 
		}
	temp=addr%256;
	for(i=0x80; i!=0; i>>=1)
		{
			if((crc_c&0x8000)!=0) {crc_c<<=1; crc_c^=0x1021;} 
			else crc_c<<=1; 
			if((temp&i)!=0) crc_c^=0x1021; 
		}
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
char send_packet(unsigned char *packet,int packet_num)
{
	unsigned short CRC;
	unsigned char ACK=0;
	char retry=5;
	while((retry--)&&(!(ACK==0xF2)))	
	{
		write_com_char(':');
		write_com_2char((short)(BIAS+packet_num*32));
		write_com_string(packet+packet_num*32,32);
		CRC=cal_crc(BIAS+packet_num*32,packet+packet_num*32,32);
		write_com_2char(CRC);
		ACK=wait_ack();
	}
	return  ACK;
}
char packet_is_blank(unsigned char *packet)
{
	int loop_i;
	char flag=1;
	for(loop_i=0;loop_i<32;loop_i++)
	{
		if(packet[loop_i]!=0)
		{
			flag=0;
			break;
		}
	}
	return flag;
}
char dl_port(unsigned char *ram,char x,short length)
{
	int loop_i;
	unsigned char ACK;
	char retry=5;
	unsigned short sent_length=0;//获取要发送的非空白packet长度
	//开始发送数据包
	for(loop_i=0;loop_i<length;loop_i++)
	{
		if(!packet_is_blank(ram+loop_i*32))
		{
			sent_length++;
		}
	}
	printf("sent_length=%d\n",sent_length);
	while((retry--)&&(!(ACK==0xF1)))
	{
		printf("try to start MCU %d download  \n",x);
		write_com_string((unsigned char *)"openport",8);//"openport"	
		write_com_char(x);//+x
		write_com_string((unsigned char *)"st_dl",5);//"st_dl"
		write_com_2char(sent_length);
		write_com_2char(sent_length);
		ACK=wait_ack();
		
	}
	if(ACK!=0xF1)
	{
			printf("*error:MCU %d not response to start_download  \n",x);
			printf("ACK=%2X",ACK);
			return 0;
	}
	printf("MCU %d succefully response to start_download  \n",x);

	//开始发送数据包
	for(loop_i=0;loop_i<length;loop_i++)
	{
		if(!packet_is_blank(ram+loop_i*32))
		{
			printf("send packet %d\n",loop_i);
			ACK=send_packet(ram,loop_i);
			if(ACK!=0xF2)
			{
				printf("*error:MCU %d not response to data_packet%d  \n",x,loop_i);
				printf("ACK=%2X",ACK);
				return 0;
				break;
			}
			
		}
	}
	//结束下载
	retry=5;
	while((retry--)&&(!(ACK==0xF3)))
	{
		printf("try to end MCU %d download  \n",x);
		write_com_string((unsigned char *)"ed_dl",5);
		write_com_2char(sent_length);
		ACK=wait_ack();
		
	}
	write_com_string((unsigned char *)"ed_dl",5);
	write_com_2char(sent_length);
	ACK=wait_ack();
	if(ACK!=0xF3)
	{
		printf("*error:MCU %d not response to end_download  \n",x);
		printf("ACK=%2X",ACK);
		return 0;
		}
	printf("download process of MCU %d is compelete!\n",x);
	return 1;
}

int SET_ADDR(unsigned short addr)
{
	unsigned char ACK;
	write_com_string((unsigned char *)"AD",2);
	write_com_2char(addr);
	ACK=wait_ack();
	printf("ACK=%2X\n",ACK);
	return 0;
}

int DOWNLOAD_PACKET(DATAPACKET *temp)
{
	unsigned char ACK;
	SET_ADDR((*temp).addr);
	
	write_com_string((unsigned char *)"WR",2);
	write_com_char((*temp).length);
	write_com_string((*temp).data,(*temp).length);
	ACK=wait_ack();
//	printf("0x%04X\n  ", (*temp).addr);
	printf("ACK=%2X\n",ACK);
	return 0;
}

void FLASH_ERASE()
{
	write_com_string((unsigned char *)"ER",2);
}
int CHECK_DATA(DATAPACKET *temp)   //读取与packet一样地址和长度的flash信息并比较  
{
	unsigned char ACK;
	unsigned char buf[16];
	unsigned char i;
	unsigned char check=1;
	SET_ADDR((*temp).addr);
	write_com_string((unsigned char *)"RD",2);
	write_com_string((*temp).data,(*temp).length);

	read_com(buf,(*temp).length);
	for(i=0;i<(*temp).length;i++)
	{
		if(buf[i]!=(*temp).data[i])
			check=0;
	};

	printf("origin_data :");
	for(i=0;i<16 && i<(*temp).length;i++)
		printf("%02X",(*temp).data[i]);
	printf("MCU_FLASH_data :");
	for(i=0;i<16 && i<(*temp).length;i++)
		printf("%02X",buf[i]);
	printf("\n");

	printf("check=%2X\n",check);
	return check;
}