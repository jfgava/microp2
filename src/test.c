// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#include "adc.h"
#include "pwm.h"
#include "galileo2io.h"
#include "rw_io.h"

#include <errno.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int run=1;
static void quit(int signal)
{
        run=0;
        pwmTeardown();
}

#define BUTTON_1 6	// IO4 with PullDown
#define IO2 13 // IO2 output

float map(float,float,float,float,float);

int main(){
	float angle=0;
	int on=0;
	struct sigaction act;
	unsigned char c;
	static struct pollfd pfd;
	int i=0;
	
	if((pfd.fd=open("/sys/class/gpio/gpio6/value",O_RDONLY)) < 0)
	{
			perror("Opening gpio6:");
			return -1;
	}

	/* Clear old values */
	read(pfd.fd,&c,1);

	pfd.events=POLLPRI;
	
	act.sa_handler = quit;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGTERM,&act,NULL);
		
	//servoSetup(PWM_1, 0);
	pwmSetup(50,0.07);
	
	while(run){
		// Read rotary voltage and convert to angle value
		angle = map(readADC(A0),0,5,-90,90);
		
		// Toggle LED
		if(poll(&pfd,1,0)){ 
			lseek(pfd.fd,0,SEEK_SET);        
            read(pfd.fd,&c,1);
			printf("Angle: %f° | %c \n",angle, c);
			on=!on;
			if(on)	digitalWrite(IO2,HIGH);
			else 	digitalWrite(IO2,LOW);
			usleep(100000);
		}
		
		if(i%100==0)
			puts("TEST");
		i++;
		// Syncronize rotary and servo
		//servoSetAngle(PWM_1,angle);	
		pwmDutyCycle(map(angle,-90,90,0.03,0.13));
			
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
