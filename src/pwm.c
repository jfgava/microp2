// Jonas Gava, Rafael Koch Peres -- 
// ENG10032 - Microcontroladores -- Prova P2

#include "pwm.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "galileo2io.h"

static int _period = 1;

int pwmSetup(float frequency, float dcycle) {
    char str[80];
    float period;
    int fd;

    if ((frequency < 24) || (frequency > 1525)) {
        fprintf(stderr, "Error: invalid frequency value %f\n.", frequency);
        return -2;
    }

    period = (1.0 / frequency) * 1e9; // period in nanoseconds

    // Set frequency
    _period = (int)round(period);
    snprintf(str, sizeof str, "%d", _period);
    #ifndef DEBUG
        printf("Set PWM period: %s ns.\n", str);
    #endif
    pputs("/sys/class/pwm/pwmchip0/device/pwm_period",str);

	if((fd=open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle",O_WRONLY)) < 0)
    {
		perror("Opening duty_cycle:");
        return -1;
    }
    write(fd,"0\n",2);
        
    close(fd);
    
    pputs("/sys/class/pwm/pwmchip0/pwm1/enable","1");
        
    // Set duty cycle
    return pwmDutyCycle(dcycle);
}

int pwmDutyCycle(float value)
{
    int fd;
    int cycle;
    char str[80];

    value = fabs(value);

    if ((value < 0) || (value > 1)) {
        fprintf(stderr, "Error: invalid duty cycle value %f\n.", value);
        return -2;
    }

    if ((fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY)) < 0)
    {
        perror("Opening duty_cycle:");
        return -1;
    }
    
    cycle = (int)round(value * _period);

    snprintf(str, sizeof str, "%d\n", cycle);
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}

int servoSetup(int pwm_n, double start_angle){
	int duty_cycle;
    double angle;
    char str[100], path[100];
        
    angle = start_angle * M_PI/180.0;
       
    if((angle > M_PI_2) || (angle < - M_PI_2))
    {
		puts("Out of range.");
        return -1;
    }
    duty_cycle = angle/M_PI_2*850000+1500000;
                
    pputs("/sys/class/pwm/pwmchip0/device/pwm_period","20000000");
        
    snprintf(str,sizeof str,"%d\n",duty_cycle);
    snprintf(path,sizeof path,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);
    pputs(path,str);
    
    snprintf(path,sizeof path,"/sys/class/pwm/pwmchip0/pwm%d/enable",pwm_n);
    pputs(path,"1");
    
	return 1;
}

int servoSetAngle(int pwm_n, double angle_){
	int duty_cycle;
    char str[100], path[100];
    double angle;
    
    angle = angle_ * M_PI/180.0;
        
    if((angle > M_PI_2) || (angle < - M_PI_2))
    {
         puts("Out of range.");
         return -1;
    }
    duty_cycle = angle/M_PI_2*850000+1500000;
        
    snprintf(str,sizeof str,"%d\n",duty_cycle);
    snprintf(path,sizeof path,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm_n);
    pputs(path,str);
    
    return 1;
}

int pwmTeardown()
{
    pputs("/sys/class/pwm/pwmchip0/pwm1/enable","0");
    return 0;
}
