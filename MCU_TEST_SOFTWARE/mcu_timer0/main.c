#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;	 
void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;
	TMOD=0x01;//设置定时器0为工作方式1 
//	PCON |= 0x10; 
	TH0=(65535-46080)/256;
	TL0=(65535-46080)%256;
   // PCON = 0x00;
  //  DPL=0xff;
//	DPH=0x01;
	EAL = 1;
    ET0 = 1;
    TR0 = 1;
	
	//SER0_EN=1;
//	P0=0xff;
	
	while(1)
	{
		
	}
}
void timer0() interrupt 1
{
	TH0=0xf4;
	TL0=0x44;
	P0=~P0;
	
}
/*
void delay_1us(unsigned int num)
{
	unsigned int i;
	for(i=0;i<num;i++);
} */