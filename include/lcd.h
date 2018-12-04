// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#ifndef LCD_H
#define LCD_H

static volatile int run=1;

void quit(int signal);
void i2c_error(const char *msg);

/**
 * bl_on: 1 (enable) or 0 (disable)
 * if bl_on=1 red/green/blue set Backlight color
 * return file descriptor
 * 
 */
int initLCD(int bl_on, int red, int green, int blue);

/**
 * fd: file descriptor initialized in initLCD()
 * red/green/blue set Backlight RGB color
 *
 */
void setRGB(int fd,int red, int green, int blue);

/**
 * msg[]: message to write to LCD
 * line: line to write text. LCD_1LINE or LCD_2LINE
 */
void writeLCD(int fd, char msg[], unsigned char line);

/**
 * addr: position to write
 * msg[]: message to write to LCD
 * line: line to write text. LCD_1LINE or LCD_2LINE
 *
 */
void writeAddrLCD(int fd, unsigned addr, char msg[], unsigned char line);


/**
 * Finalize file descriptor initialized in initLCD()
 *
 */
void finishLCD(int fd);
	

#endif
