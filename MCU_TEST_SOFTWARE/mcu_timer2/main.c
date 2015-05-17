#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;	 
void T2_init(void);
void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;
//	TMOD=0x10;//设置定时器1为工作方式1 
//	PCON |= 0x10; 
//	TH1=(65535-46080)/256;
//	TL1=(65535-46080)%256;
   // PCON = 0x00;
  //  DPL=0xff;
//	DPH=0x01;
//	
 //   ET2 = 1;
//    TR2 = 1;
	
	//SER0_EN=1;
//	P0=0xff;
	T2_init();
	//CRCL=0xF0;
	//CCL1=0xF2;
	//CCL2=0xF4;
	//CCL3=0xF6;

	while(1)
	{
		
	}
}
void timer0() interrupt 5
{
/*	if((IRCON&0x01)==1)
	{
		IRCON=IRCON&0xfe;
	}
	if((IRCON&0x02)==1)
	{
		 IRCON=IRCON&0xfd;
	//	 TH2=0xFA;
	//	 TL2=0xF0;
		 
	}	 */
	IRCON=0;
	P0=~P0;

	
}
void T2_init(void)
{
	TL2=0xF0;
	TH2=0xFA;
//	CCEN=0xff;
	T2CON=0x01;
	CCEN=0x00;
	IEN0=0xA0;
	IEN1=0x80;
	//EAL = 1;
//	CC
//	IEN1=0x00;
}
/*
void delay_1us(unsigned int num)
{
	unsigned int i;
	for(i=0;i<num;i++);
} */