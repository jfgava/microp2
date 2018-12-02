
#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define QUAD_X4 0x3
#define RD_CNTR 0x60
#define WR_MDR0 0x88
#define WR_DTR 0x98
#define LD_CNTR 0xE0
#define CLR_CNTR 0x20
#define SS_ 10

int setCounter(int fd, int value);

int spiBegin(char *file);

int readCounter(int fd);

void clearCounter(int fd);

int spiTransfer(int fd, uint8_t msg);

#endif
