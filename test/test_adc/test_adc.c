#include "adc.h"
#include "pwm.h"
#include "rw_io.h"

#include <byteswap.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
#undef DATA_POINTS
#undef N_SENSORS
#undef SAMPLING_PERIOD

#define N_SENSORS 4
#define DATA_POINTS 1000
#define SAMPLING_PERIOD 1e-3
*/

/*
int readSensorValues(int sensor, int samples){
	int i;
	
	printf("Values for sensor %d\n",sensor);
	for(i=0;i<samples;i++){
		printf("%f-%f ",bswap_16(data[i].s[sensor])*scale[sensor],(data[i].timestamp-data[0].timestamp)*1e-9);
	}
	puts("");
	
	return 0;
}*/

int main(){
	int samples;
	
	if(!continuosADC(SETUP)){ 
		puts("Error setup");
		return -1;
	}
	
	if((samples = continuosADC(READ)) < 1){
		printf("Error read ADC %d\n",samples);
		return -1;
	}
		
	//readSensorValues(0,samples);

	continuosADC(FINISH);
	
	writeContinuosADC("cont_adc.log",samples);
		
	return 0;
}

