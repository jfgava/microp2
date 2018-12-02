// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#include <byteswap.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "galileo2io.h"
#include "adc.h"


float readADC(int adc_name){
	char data_str[80], path[80];
	double scale;
    int raw;
	int fd;

	sprintf(path,"/sys/bus/iio/devices/iio:device0/in_voltage%d_raw",adc_name);
	
    if((fd=open(path,O_RDONLY)) < 0)
    {
		perror("Opening in_voltage0raw:");
        return -1;
    }
	sprintf(path,"/sys/bus/iio/devices/iio:device0/in_voltage%d_scale",adc_name);
    pgets(data_str,sizeof data_str,path);
    scale=atof(data_str)/1000.0;
    
    lseek(fd,0,SEEK_SET);
    read(fd,data_str,sizeof data_str);
    raw=atoi(data_str);
    
    close(fd);
    
	return raw*scale;
}

int continuousADC()
{
        char data_str[80];
        //double scale[4];
        int fd;
        //static struct sensors data[DATA_POINTS];
        int i;
        int samples;
        //FILE *file;
        char path_str[80];

        pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","0");
        for(i=0;i < N_SENSORS;i++)
        {
                snprintf(path_str,sizeof path_str,"/sys/bus/iio/devices/iio:device0/in_voltage%d_scale",i);
                pgets(data_str,sizeof data_str,path_str);
                scale[i]=atof(data_str)/1000.0;
                
                snprintf(path_str,sizeof path_str,"/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage%d_en",i);
                pputs(path_str,"1");
        }
        pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en","1");

        snprintf(data_str,sizeof data_str,"%d",DATA_POINTS);
        pputs("/sys/bus/iio/devices/iio:device0/buffer/length",data_str);
        
#ifdef TRIG_SYSFS
        pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/trigger0/name");
        pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger",data_str);
#else
        pgets(data_str,sizeof data_str,"/sys/bus/iio/devices/trigger1/name");
        pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger",data_str);

        snprintf(data_str,sizeof data_str,"%d",(int)round(1.0/SAMPLING_PERIOD));
        pputs("/sys/bus/iio/devices/trigger1/frequency",data_str);
#endif        

        pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","1");

#ifdef TRIG_SYSFS
        for(i=0; i < DATA_POINTS;i++)
        {
                pputs("/sys/bus/iio/devices/trigger0/trigger_now","1");
                usleep(ceil(SAMPLING_PERIOD*1e6));
        }
#else
        sleep(ceil(DATA_POINTS*SAMPLING_PERIOD));
#endif
        
        pputs("/sys/bus/iio/devices/iio:device0/buffer/enable","0");
        
        pputs("/sys/bus/iio/devices/iio:device0/trigger/current_trigger","\n");

        if((fd=open("/dev/iio:device0",O_RDONLY)) < 0)
        {
                perror("Opening /dev/iio:device0:");
                return -1;
        }
        
        samples=read(fd,data,sizeof data)/sizeof(struct sensors);
        //printf("Samples: %d\n",samples);
        
        close(fd);
        
        pputs("/sys/bus/iio/devices/iio:device0/buffer/length","2");
        
        for(i=0;i < N_SENSORS;i++)
        {
                snprintf(path_str,sizeof path_str,"/sys/bus/iio/devices/iio:device0/scan_elements/in_voltage%d_en",i);
                pputs(path_str,"0");
        }
        pputs("/sys/bus/iio/devices/iio:device0/scan_elements/in_timestamp_en","0");
        
        //~ if((file=fopen(argv[1],"w")) == NULL)
        //~ {
                //~ perror("Opening output file:");
                //~ return -1;
        //~ }

        for(i=0;i < samples;i++)
        {
                data[i].s1=bswap_16(data[i].s1);
                data[i].s2=bswap_16(data[i].s2);
                data[i].s3=bswap_16(data[i].s3);
                data[i].s4=bswap_16(data[i].s4); 
                
                printf("%f\t%f\t%f\t%f\t%f\n",data[i].s1*scale[0],
									data[i].s2*scale[1],
									data[i].s3*scale[2],
									data[i].s4*scale[3],
									(data[i].timestamp-data[0].timestamp)*1e-9);               
                
                //~ fprintf(file,"%f\t%f\n",
                        //~ data[i].pot*scale[0],
                        //~ data[i].light*scale[1]);//,
                        //~ //data[i].sound*scale[2],
						//~ //data[i].temp*scale[3],
                        //~ //(data[i].timestamp-data[0].timestamp)*1e-9);
        }
        //fclose(file);
        
        return 1;
}
/*
void printADC(int samples){	
	int i;
	
    for(i=0;i < samples;i++)
    {
		printf("%f\t%f\t%f\t%f\t%f\n",data[i].pot*scale[0],
									data[i].light*scale[1],
									data[i].sound*scale[2],
									data[i].temp*scale[3],
									(data[i].timestamp-data[0].timestamp)*1e-9);
    }
}
*/
