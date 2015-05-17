#include <REG51XC.H>
#include <simae3801.H>
#include <intrins.h>
#define uchar unsigned char

void InitUART(void)
{
    TMOD = 0x20;
    S0CON = 0x40;
    TH1 = 0xDF;
    TL1 = TH1;
//	SM0=0;
//	SM1=1;
    PCON = 0x00;
    EAL = 1;
    ES0 = 1;
//	ET1=1;
    TR1 = 1;
	SER0_EN=1;	
}

void SendOneByte(unsigned char c)
{
//	P0=~P0;
    S0BUF = c;
    while(!TI0);
    TI0 = 0;
	
	ES0=1;
}


void UARTInterrupt(void) interrupt 4
{  
//	RI0 = 0;
	ES0 = 0;
	RI0 = 0;
	if(S0BUF!=0xff)
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

/*void printf_string(uchar *s)
{
	while(*s!='\0')
	{
		SendOneByte(*s++);
		}	 
}		*/
/*void delay1s(void)   //Îó²î -0.049999999944us
{
    unsigned char a,b,c,n;
    for(c=191;c>0;c--)
        for(b=209;b>0;b--)
            for(a=249;a>0;a--);
    for(n=1;n>0;n--);
    _nop_();  //if Keil,require use intrins.h
}  */
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