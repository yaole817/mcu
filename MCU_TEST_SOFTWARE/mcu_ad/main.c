#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;
sfr		ADC_CONTROL		=	0xD9;
sfr		ADC_CONFIG		=	0xDA;
sfr		ADC_DATA_H		=	0xDB;
sfr		ADC_DATA_L		=	0xDC;

void ADInit(void);	 
void main(void)
{  
	PORT0_OUT=0xff;
	PORT0_IN=0x00;

//	PORT1_OUT=0xff;
//	PORT1_IN=0x00;

//	PORT2_OUT=0xff;
//	PORT2_IN=0x00;

	PORT3_OUT=0xff;
	PORT3_IN=0x00;
	P0=0;
	P3=0;

	ADInit();
	while(1);
}

void ADInit(void)
{
	ADC_CONTROL=0x05;
	ADC_CONFIG=0x00;
	while((ADC_DATA_H&0x10));
	P3=ADC_DATA_H;
	P0=0x55;//ADC_DATA_L;		
}


