//

#ifndef LCD_H
#define LCD_H

static volatile int run=1;

void quit(int signal);
void i2c_error(const char *msg);
int initLCD();
void setRGB(int fd,int red, int green, int blue);
void writeLCD(int fd, char msg[]);
void writeAddrLCD(int fd, unsigned addr, char msg[]);
void finishLCD(int fd);
	

#endif
