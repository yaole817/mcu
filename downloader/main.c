#include <reg51.h>
#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include "i2c.h"

sfr AUXR   = 0x8E;

enum {WRITE,READ,SET_ADDR,TEST,ERASE} order,last_order;
enum {IDLE,WRITING,READING,RECEIVING,WR_RD_TEST,ERASING} status;
char step=0;
char length;
int addr;
int count_1s=0;
char data_buf[16];
char string_buf[20];
char CMD[3]={"ID"};
void InitUART(void)
{
    TMOD = 0x20;
    SCON = 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
//*********************************
//timer interupt initial
void InitTimer0(void)
{
    TMOD |= 0x01;
    TH0 = 0x0D4;
    TL0 = 0x0CD;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
}

void delay1s(void)   //Îó²î -0.000000000125us
{
    unsigned char a,b,c;
    for(c=212;c>0;c--)
        for(b=160;b>0;b--)
            for(a=80;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
    _nop_();  //if Keil,require use intrins.h
}


void SendOneByte(unsigned char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}
void send_str_len(char *p,int len)
{
	while(len-- > 0 )
	{
		SendOneByte(*p++);
		}
}
void send_string(char *p)
{
	while(*p!='\0')
		SendOneByte(*p++);
}

void main(void)
{
	 char write_data[4]={0xff,0Xff,0,0};
  // char read_data[2];
  AUXR = AUXR|0x40;  // T1, 1T Mode
	AUXR = AUXR|0x80;  // T0, 1T Mode

    InitUART();
	InitTimer0();
	while(1)
	{		 
			
			if(status==ERASING)
		  {		
		  		//i2c_write(addr,data_buf,len);
			   	erase_flash_chip();
				//sprintf(string_buf,"wr 0x%X,%d\n",addr,(int)length);
				//send_string(string_buf);
				//send_string(data_buf);
					SendOneByte(0xaa);
				  status=IDLE;		
		  }
		  if (status==RECEIVING)
		  {
		  	if(count_1s>1000)
			  {
			  		status=IDLE;
					send_string("TIMEOUT");
					//timeout();
			  }
			}
		  if(status==WRITING)
		  {		
		  		//i2c_write(addr,data_buf,len);
			   	//erase_flash_chip();
					wr_data(addr,data_buf,length);
				//sprintf(string_buf,"wr 0x%X,%d\n",addr,(int)length);
				//send_string(string_buf);
				//send_string(data_buf);
				SendOneByte(0xaa);
				status=IDLE;		
		  }
		  else if(status==WR_RD_TEST)
		  {		
				//erase_flash_chip();
				delay(10000);
				//wr_data(0x0000,write_data,4);
				delay(1000);
				rd_data(0x0000,write_data,4);
				send_str_len(write_data,4);
				status=IDLE;
		  }		
			else if(status==READING)
		  {		
		  		//i2c_read(addr,data_buf,len);
				  sprintf(string_buf,"rd adr%X,len%d\n",addr,(int)length);
			    send_string(string_buf);
			   	status=IDLE;
		  }	

		  I2CEN=1;

	}
}
void detect_CMD(char temp)
{
	//SendOneByte(temp);
	CMD[0]=CMD[1];
	CMD[1]=temp;
	last_order=order;
	if ((CMD[0]=='A')&&(CMD[1]=='D')) 
	{
		order=SET_ADDR;
		status=RECEIVING;
		CMD[0]=0;
		CMD[1]=0;
		step=0;
		count_1s=0;
		}
	if ((CMD[0]=='T')&&(CMD[1]=='E')) 
	{
		order=TEST;
		status=WR_RD_TEST;
		CMD[0]=0;
		CMD[1]=0;
		step=0;
		count_1s=0;
		}
	if ((CMD[0]=='W')&&(CMD[1]=='R')) 
	{	
		order=WRITE;
		length=0;
		status=RECEIVING;
		CMD[0]=0;
		CMD[1]=0;
		step=0;
		count_1s=0;
		}
	if ((CMD[0]=='R')&&(CMD[1]=='D')) 
	{
		order=READ;	
		status=RECEIVING;
		CMD[0]=0;
		CMD[1]=0;
		step=0 ;
		count_1s=0;
		}
	if ((CMD[0]=='E')&&(CMD[1]=='R')) 
	{
		order=ERASE;	
		status=ERASING;
		CMD[0]=0;
		CMD[1]=0;
		step=0 ;
		count_1s=0;
		}
		
		
}
void receive_data(char temp)
{

	if (status==RECEIVING)
	{	count_1s=0;//set 1s timeout count when a char received in receiving packet mode;
		if(order==SET_ADDR)
		{
			switch(step)
			{
				case 0:addr=temp;break;
				case 1:addr=addr*256+temp;
					   status=IDLE;
					   SendOneByte(0xaa);
				default:break;
			}
		   
		}
		if(order==WRITE)
		{
			switch(step)
			{
				case 0:length=temp;break;
						
				default: 
						if(step<length)
						{
							data_buf[step-1]=temp;
							}
						else
						{		
								data_buf[step-1]=temp;
								status=WRITING;
							}
				}		
			}
		 if(order==READ)
		{
			switch(step)
			{
				case 0:length=temp;status=READING;break;
				default:   break;
					
				}		
			}
		 step++;
	}
	
}
											
void UARTInterrupt(void) interrupt 4
{
	char temp;
    if(RI)
    {
        RI = 0;
        temp=SBUF;
		
		if (status==IDLE)
		{
		   detect_CMD(temp);
			}
		else 
		receive_data(temp);
		 
    }
    else
        TI = 0;
}
 //timer 1s tik tok
void timer_1s()
{
	count_1s++;
}
void Timer0Interrupt(void) interrupt 1
{
    TH0 = 0x0D4;
    TL0 = 0x0CD;
    //add your code here!
	timer_1s();
}