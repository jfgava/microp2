#include "adc.h"
#include "pwm.h"
#include "rw_io.h"
#include "lcd.h"

#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2cutil.h>
#include <jhdlcd.h>

#define BUTTON_1 6	// IO4 with PullUp

float map(float,float,float,float,float);

int main(){
	int fd_lcd;
	float angle, angle_ref=0, rad, diff;
	char msg[40];
	struct sigaction act;
	
	act.sa_handler = quit;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGTERM,&act,NULL);
		
	servoSetup(PWM_1, 0);
	fd_lcd = initLCD();
	
	while(run){
		// Read rotary voltage and convert to angle value
		angle = map(readADC(A0),0,5,-90,90);
		rad = map(angle,-90,90,-M_PI/2,M_PI/2);
		
		// Set reference angle if button pressed
		if(!digitalRead(BUTTON_1)){ 
			sprintf(msg,"%f",angle);
			angle_ref = angle;
			writeAddrLCD(fd_lcd,0x40,msg);
		}
		// Syncronize rotary and servo
		servoSetAngle(PWM_1,map(readADC(A0),0,5,-90,90));
		
		// Write angle first line LCD
		sprintf(msg,"%f - %f",angle,rad);
		writeAddrLCD(fd_lcd,0,msg);
		
		// Set Blacklight according to angle_error value
		diff=angle-angle_ref;
		if(diff > 9)
			setRGB(fd_lcd,255,0,0); // RED_BL
		else if(diff < -9)
			setRGB(fd_lcd,255,255,0); // YELLOW_BL
		else
			setRGB(fd_lcd,0,255,0); // GREEN_BL		
			
		usleep(10000); // 10ms delay
	}
	
	return 0;
}

float map(float val, float v1, float v2, float v3, float v4){
	float calc;
	
	if(val>v2)
		val=v2;
	else if(val<v1)
		val=v1;
	
	return calc=(val-v1)*(v4-v3)/(v2-v1)+v3;
}
