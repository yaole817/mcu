#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//char i=0;	 
void delay_1us(unsigned int num);

void main(void)
{  
//	char tyy=0;
//	char t=0;
//	char tt=0;
//	char ttt=0;
//	long text=0;
//	int g=0.0;
	PORT0_OUT=0xff;
	PORT0_IN=0x00;

	PORT1_OUT=0xff;
	PORT1_IN=0x00;

	PORT2_OUT=0xff;
	PORT2_IN=0x00;

	PORT3_OUT=0xff;
	PORT3_IN=0x00;
	

//	P0=0xff;
	
	while(1)
	{
		P0=0;
		P0=0xff;
//		delay_1us(1000);
		
//		P1=0;
//		P1=0xff;
//		t=65535/255;
//		t=65535%255;
//		tt=t+tt;
//		tt=t*tt;
//		tt=t-tt;
	//	P1=P1&0x0f;
		
//		P2=0;
//		P2=0xff;
	//	P2=P2&0x0f;

	//	for(t=0;t<255;t++);
	//	if(t<4)t=0;
	//	P2=0x34;
	//	P2++;		

	//	tt=t*tt;
	//	tt=t-tt;

//		P3=0;
//		P3=0xff;
	//	P3++;
//		g=(int)(t+tt);

//		P2=t;
		//P3=0;
	//	P0=0;
	//	P0=0xff;

	}
}

void delay_1us(unsigned int num)
{
	unsigned int i;
	for(i=0;i<num;i++);
} 