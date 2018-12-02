// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define A0 0
#define A1 1
#define A2 2
#define A3 3

// Continuous ADC
#define N_SENSORS 4
#define DATA_POINTS 100
#define SAMPLING_PERIOD 1e-2

double scale[N_SENSORS];

/* Assumes the data format for Galileo Gen2 */
struct sensors
{
        uint16_t s1;		/* be:u12/16>>0 */
        uint16_t s2;		/* be:u12/16>>0 */
        uint16_t s3;		/* be:u12/16>>0 */
        uint16_t s4;		/* be:u12/16>>0 */
        int64_t timestamp;	/* le:s64/64>>0 */
};

struct sensors data[DATA_POINTS];

float readADC(int adc_name);

// Continuous ADC
int continuosADC();

#endif
