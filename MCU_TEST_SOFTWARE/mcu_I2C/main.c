#include <REG51XC.H>
#include <simae3801.H>
//void delay_1us(unsigned int num);
//unsigned char i=0;
void I2C_init(void);
void SendI2CByte(unsigned char c);	 
void main(void)
{  
	PORT1_OUT=0xff;
	PORT1_IN=0x00;
//	TMOD=0x10;//���ö�ʱ��0Ϊ������ʽ1 
//	PCON |= 0x10; 
//	TH1=(65535-46080)/256;
//	TL1=(65535-46080)%256;
   // PCON = 0x00;
  //  DPL=0xff;
//	DPH=0x01;
//	EAL = 1;
  //  ET1 = 1;
    //TR0 = 1;
	
	//SER0_EN=1;
//	P0=0xff;
	I2C_init();
	SendI2CByte(0x55);

	while(1)
	{
			
	}
}
/*void timer0() interrupt 1
{
	TH0=0xf4;
	TL0=0x44;
	P0=~P0;
	
}	  */
void I2C_init(void)
{
	I2CCON 	&= 	01110100;     //���ƼĴ���,ʱ��Ϊ��ʱ�ӵ�1/256	
	I2CSTA 	 =	0x08;			
}
void SendI2CByte(unsigned char c)
{
	
	I2CDAT	 = 	c;

	I2CCON	|=	0x20;	   //����start�ź�

//	I2CCON	|=	0x10;	   //����stop�ź�
	
//	I2CCON	|=	0x20;

	I2CCON 	|= 	0x40;     //���ƼĴ���,��I2C�ж�		
}
/*
void delay_1us(unsigned int num)
{
	unsigned int i;
	for(i=0;i<num;i++);
} */