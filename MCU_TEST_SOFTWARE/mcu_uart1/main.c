#include <REG51XC.H>
#include <simae3801.H>
#include <intrins.h>
#define uchar unsigned char

void InitUART(void)
{
 //   TMOD = 0x20;
    S1CON = 0x1c;
    S1RELH = 0xDF;
    S1RELL = S1RELH ;
//	SM0=0;
//	SM1=1;
	T2CON=0x01;
	CCEN=0x00;
	IEN0=0xA0;
	IEN1=0x80;

 //   PCON = 0x00;
//    EAL = 1;
 //   IEN2=0x01;
//	ET1=1;
//    TR1 = 1;
//	SER0_EN=1;	
}

void SendOneByte(unsigned char c)
{
//	P0=~P0;
    S1BUF = c;
    while(!(S1CON&0x02));
    S1CON = 0xfd;
	
	IEN2=0x01;
}


void UARTInterrupt(void) interrupt 4
{  
//	RI0 = 0;
	ES0 = 0;
	RI0 = 0;
	if(S1BUF!=0xff)
		P0=~P0;
    if(RI0)
    {  
		
	//	RI0 = 0; 
       //add your code here!
    }
   // else
     //   TI0 = 0;
	ES0 = 1; 
}


void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;
	PORT1_OUT=0xff;
	PORT1_IN=0x00;	
	PORT2_OUT=0xff;
	PORT2_IN=0x00;
	PORT3_OUT=0xff;
	PORT3_IN=0x00;	 
	InitUART();
	while(1)
	{
	   SendOneByte(0xaa);
	//	P2=0;
	//	P3=0;
	//	P2=255;
	//	P3=0xff;
	//	printf_string("hello from SAMAE3801");
	//	delay1s();
	}
}