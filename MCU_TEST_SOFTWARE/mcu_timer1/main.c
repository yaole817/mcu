#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;	 
void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;
	TMOD=0x10;//设置定时器1为工作方式1 
//	PCON |= 0x10; 
	TH1=(65535-46080)/256;
	TL1=(65535-46080)%256;
   // PCON = 0x00;
  //  DPL=0xff;
//	DPH=0x01;
	EAL = 1;
    ET1 = 1;
    TR1 = 1;
	
	//SER0_EN=1;
//	P0=0xff;
	
	while(1)
	{
		
	}
}
void timer0() interrupt 3
{
	TH1=0xf4;
	TL1=0x44;
	P0=~P0;
	
}
/*
void delay_1us(unsigned int num)
{
	unsigned int i;
	for(i=0;i<num;i++);
} */