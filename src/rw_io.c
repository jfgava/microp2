// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#include "galileo2io.h"
#include "rw_io.h"
#include <stdio.h>
#include <stdlib.h>

int digitalWrite(int pin, char *level){
	char path[50]="";
	
	sprintf(path,"/sys/class/gpio/gpio%d/value",pin);
	
	if(! pputs(path,level)){
		printf("Error digitalWrite: pin %d\n",pin);
		return -1;
	}
		
	return 1;
}

int digitalRead(int pin){
	char path[50]="", buff[10]="";
	
	sprintf(path,"/sys/class/gpio/gpio%d/value",pin);

	if(! pgets(buff,sizeof(buff),path)){
		printf("Error digitalRead: pin %d\n",pin);
		return -1;
	}
	
	return atoi(buff);
}
