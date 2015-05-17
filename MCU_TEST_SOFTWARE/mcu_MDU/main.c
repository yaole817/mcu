#include <REG51XC.H>
#include <simae3801.H>
//int DIV16(int a, int b);
//int DIV32(long a,int b);
int MUL(int a,int b);	 
void main(void)
{  

	PORT0_OUT=0xff;
	PORT0_IN=0x00;

	if(MUL(4,8)==32)
	{
		P0=0xaa;
	}
	else 
	{
		P0=0x00;
	}
	while(1)
	{
		
	}
}
/*int DIV16(int a, int b)
{
	int result=0;
	MD0=a&0x00ff;
	MD1=(a&0xff00)>>8;
	MD4=b&0x00ff;
	MD5=(b&0xff00)>>8;
	while((ARCON&0x80)==1);
	result=(int)MD4;
	result=(int)((MD5<<8)|result);
	return result;
}  */
int MUL(int a,int b)
{
	int result=0;
	MD0=a&0x00ff;
	MD4=b&0x00ff;
	MD1=(a&0xff00)>>8;
	MD5=(b&0xff00)>>8;
	while((ARCON&0x80)==1);
	result=(int)MD0;
	result=(int)((MD1<<8)|result);
	return result;
	
}
/*int DIV32(long a,int b)
{
	int result=0;
	MD0=a&0x000000ff;
	MD1=(a&0x0000ff00)>>8;
	MD2=(a&0x00ff0000)>>16;
	MD2=(a&0xff000000)>>24;
	MD4=b&0x00ff;
	MD5=(b&0xff00)>>8;
	while((ARCON&0x80)==1);
	result=(int)MD0;
	result=(int)((MD1<<8)|result);
	return result;
} */
