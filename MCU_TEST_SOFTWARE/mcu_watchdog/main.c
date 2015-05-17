#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;
void delay_1us(unsigned int num)
{
	unsigned int i,j;
	for(i=0;i<num;i++)
	{
		for(j=0;j<1000;j++);
	}
} 
	 
void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;
//	CLKCFG=0x00;
	WDTREL=0x7f;
//	IEN0=0xC0;
//	IEN1=0x40;
	WDT=1;
	SWDT=1;
	EAL=1;
//	delay_1us(100);
	
	while(1)
	{
		P0=~P0;
	//	SWDT=1;
		delay_1us(1);	
		//IEN1=0x40;
	}
}

