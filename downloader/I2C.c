#include "i2c.h"
void delay(int x)
{	
	int i;
	for(i=0;i<x;i++);		
}
void i2c_start()
{
	I2C_SDA_OUT;
	I2C_SDA_H;
	delay(3);
    I2C_SCL_H;
	delay(3);
	I2C_SDA_L;
	delay(3);
	I2C_SCL_L;
	delay(3);
}
void i2c_stop()
{
	I2C_SDA_OUT;
	I2C_SCL_L; 
	I2C_SDA_L;   	
	delay(3);
	I2C_SCL_H;
	delay(3); 
	I2C_SDA_H;
	delay(3);
}
void wait_ack()
{
	I2C_SDA_OUT;
	I2C_SCL_L;
	I2C_SDA_H;
	I2C_SDA_IN;
	delay(10);
	I2C_SCL_H;
	delay(3);
	//while(I2C_SDA_RD);
	delay(3);
	I2C_SDA_L;
	I2C_SCL_L;	
}
void ack()
{
	I2C_SDA_OUT;
	I2C_SCL_L;
	delay(3);
	I2C_SDA_L;
	delay(3);
	I2C_SCL_H;
	delay(3);
	I2C_SCL_L;	
}
void noack()
{
	I2C_SDA_OUT;
	I2C_SCL_L;
	delay(3);
	I2C_SDA_H;
	delay(3);
	I2C_SCL_H;
	delay(3);
	I2C_SCL_L;
}
 
void wr_byte(uchar temp)
{
	uchar i;
	I2C_SDA_OUT;
	for(i=0;i<8;i++)
	{
		I2C_SCL_L;
		if(temp&(0x80>>i))
		{
			I2C_SDA_H;
		}
		else 
		{
			I2C_SDA_L;
		}
		delay(3);
		I2C_SCL_H;
		delay(3);
		}
	I2C_SCL_L;
} 
char rd_byte()
{
	uchar i;
	uchar temp;
	I2C_SDA_IN;
	for(i=0;i<8;i++)
	{
		I2C_SCL_L;
		I2C_SDA_H;
		delay(3);
		I2C_SCL_H;
		delay(3);
		if(I2C_SDA_RD) 
			temp|=(0x80>>i);
		else 
			temp&=~(0x80>>i);
		delay(3);
		}
	I2C_SDA_L;
	I2C_SCL_L;
	return temp;
}
void wr_data(short addr,uchar *data_buf,uchar len)
{
	uchar buf;
	uchar i;
	i2c_start();

	wr_byte(WRITE_CMD);
	wait_ack();

	buf=addr>>8;
	wr_byte(buf);
	wait_ack();

	buf=addr&0xff;
	wr_byte(buf);
	wait_ack();
	//i2c_start();
	for(i=0;i<len;i++)
	{
		wr_byte(data_buf[i]);
		wait_ack();
	}
	i2c_stop();
		
}

void rd_data(short addr,uchar *data_buf,uchar len)
{	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	uchar buf;
	uchar i;
	i2c_start();

	wr_byte(WRITE_CMD);
	wait_ack();

	buf=addr>>8;
	wr_byte(buf);
	wait_ack();

	buf=addr&0xff;
	wr_byte(buf);
	wait_ack();

	i2c_start();
	delay(100);
	i2c_start();
	/*
	delay(3);
	I2C_SCL_H;
	delay(3);
	I2C_SCL_L;
		delay(3);
	I2C_SCL_H;
	delay(3);
	I2C_SCL_L;*/
	
	
	wr_byte(READ_CMD);
	wait_ack();
	
	for(i=0;i<len-1;i++)
	{
		data_buf[i]=rd_byte();
		ack();
	}
	data_buf[i]=rd_byte();
	noack();
	//ack();
	i2c_stop();
		
}


void erase_flash_chip()
{	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

	i2c_start();

	wr_byte(WRITE_CMD);
	wait_ack();

	wr_byte(0xF3);
	wait_ack();
	
	wr_byte(0);
	wait_ack();


	i2c_stop();
}