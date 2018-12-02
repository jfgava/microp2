// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

#include <i2cutil.h>
#include <jhdlcd.h>
#include "lcd.h"


void quit(int signal)
{
        run=0;
}

void i2c_error(const char *msg)
{
    perror(msg);
    exit(-errno);
}

int initLCD(){ // Return file_descriptor
	int fd;
	
	//Acesso fácil á interface I2C
	if((fd=open("/dev/i2c-0",O_WRONLY)) < 0) i2c_error("Opening /dev/i2c-0");
	usleep(30000);	/* Wait for 30 ms after power on */
	
	/* LCD initialization */
	if(ioctl(fd,I2C_SLAVE,LCD_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");

	i2c_write_reg(fd,LCD_C0,LCD_FUNCTIONSET | LCD_2LINE);
	usleep(40);  /* Wait for more than 39 us */

	i2c_write_reg(fd,LCD_C0,LCD_DISPLAYSWITCH | LCD_DISPLAYON | 
			LCD_CURSOROFF | LCD_BLINKOFF);
	usleep(40);	/* Wait for more then 39 us */

	i2c_write_reg(fd,LCD_C0,LCD_SCREENCLEAR);
	usleep(1600);	/* Wait for more then 1.53 ms */
	
	i2c_write_reg(fd,LCD_C0,LCD_INPUTSET | LCD_ENTRYLEFT | LCD_DECREMENT);

	/* Backlight initialization */
	if(ioctl(fd,I2C_SLAVE,BL_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");
	i2c_write_reg(fd,BL_MODE1,0);
	i2c_write_reg(fd,BL_LEDOUT,BL_RED_GRPPWM | BL_GREEN_GRPPWM | 
			BL_BLUE_GRPPWM);
	i2c_write_reg(fd,BL_MODE2,BL_DMBLNK);
	
	i2c_write_reg(fd,BL_RED,255);
    i2c_write_reg(fd,BL_GREEN,0);
    i2c_write_reg(fd,BL_BLUE,0);
	
	return fd;
}

void setRGB(int fd,int red, int green, int blue){ //RGB BL
	if((red>=0 && red<256) && (green>=0 && green<256) && (blue>=0 && blue<256)){
		if(ioctl(fd,I2C_SLAVE,BL_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");
		i2c_write_reg(fd,BL_MODE1,0);
		i2c_write_reg(fd,BL_LEDOUT,BL_RED_GRPPWM | BL_GREEN_GRPPWM | 
		BL_BLUE_GRPPWM);
		i2c_write_reg(fd,BL_MODE2,BL_DMBLNK);
		
		i2c_write_reg(fd,BL_RED,red);	
		i2c_write_reg(fd,BL_GREEN,green);
		i2c_write_reg(fd,BL_BLUE,blue);
	}
	else
		printf("Invalid RGB values: R %d, G %d, B %d\n",red,green,blue);
}

void writeLCD(int fd, char msg[]){ // Write string
	int n,i;
	
	n=strlen(msg);
	if(ioctl(fd,I2C_SLAVE,LCD_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");
	for(i=0;i < n;i++) i2c_write_reg(fd,LCD_RS,msg[i]);
}

void writeAddrLCD(int fd, unsigned addr, char msg[]){
	int i,n;
	
	n=strlen(msg);
	if(ioctl(fd,I2C_SLAVE,LCD_ADDR) < 0) i2c_error("ioctl on /dev/i2c-0");
	i2c_write_reg(fd,LCD_C0,LCD_DDRAMADDRSET | addr);
	for(i=0;i < n;i++) i2c_write_reg(fd,LCD_RS,msg[i]);
}

void finishLCD(int fd){
	close(fd);
}

