#ifndef _I2C_H_
#define _I2C_H_

#include <reg51.h>
#include <intrins.h>
#define uchar char



#define I2C_SCL_L  I2CCLK=0
#define I2C_SCL_H  I2CCLK=1


#define I2C_SDA_L  I2CDATA=0
#define I2C_SDA_H  I2CDATA=1

#define I2C_SDA_OUT   _nop_()
#define I2C_SDA_IN    _nop_()

#define I2C_SDA_RD   I2CDATA

#define LCD_ADDR 0
#define WRITE_CMD  0X6C
#define READ_CMD   0X6D

sbit  I2CDATA=P1^2;
sbit  I2CCLK=P1^1;
sbit  I2CEN=P1^0;

extern void wr_data(short addr,uchar *data_buf,uchar len);
extern void rd_data(short addr,uchar *data_buf,uchar len);
extern void delay(int x);
extern void erase_flash_chip();
#endif