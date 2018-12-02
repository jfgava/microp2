// NOMES:  
// ENG10032 - Microcontroladores -- Prova P2

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "galileo2io.h"
#include "rw_io.h"
#include "spi.h"

/*
int main(int argc,char *argv[])
{
	int ls7366r;
	int ok=1;
	int counter;
	
	if(argc != 2)
	{
		printf("Usage: %s device\n",argv[0]);
		return -1;
	}
	
	ls7366r=spiBegin(argv[1]);
	
	while(ok){	
		counter = readCounter(ls7366r);
		printf("Contador: %d\n", counter);
		printf("Digite uma opcao(1-continue, 2-reset counter, 0-exit): ");
		scanf("%d",&ok);
		if(ok==2)
			clearCounter(ls7366r);
	}
	
	sleep(1);
	
	close(ls7366r);
	
	return 0;
}
*/

int setCounter(int fd, int value){	
	digitalWrite(SS_,LOW); // SS pin ENABLE
	
	spiTransfer(fd,WR_DTR); // Command to Write to register DTR
	
	spiTransfer(fd,(value >> 24) & 0xFF); // MSB
	spiTransfer(fd,(value >> 16) & 0xFF); 
	spiTransfer(fd,(value >> 8) & 0xFF); 
	spiTransfer(fd,value & 0xFF); // LSB
	
	digitalWrite(SS_,HIGH); // SS pin DISABLE
	
	
	digitalWrite(SS_,LOW); // SS pin ENABLE
	
	spiTransfer(fd,LD_CNTR); // Transfer DTR to CNTR
	
	digitalWrite(SS_,HIGH); // SS pin DISABLE
	
	return 1;
}

int spiBegin(char *file){
	uint8_t mode=SPI_MODE_0;
	uint8_t lsb=0;
	uint8_t bpw=8;
	uint32_t rate=100000;
	int fd;
	
#ifdef DEBUG	
	printf("Configuring SPI ...\n");
#endif

	if((fd=open(file,O_RDWR))==-1)
	{
		perror("Can't open device");
		return -1;	
	}
	if(ioctl(fd,SPI_IOC_WR_MODE,&mode))
	{
		perror("Can't write clock mode");
		return -1;	
	}
	if(ioctl(fd,SPI_IOC_WR_LSB_FIRST,&lsb))
	{
		perror("Can't read LSB mode");
		return -1;	
	}
	if(ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&bpw))
	{
		perror("Can't read bits per word");
		return -1;	
	}
	if(ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ,&rate))
	{
		perror("Can't read maximal rate");
		return -1;	
	}
		
	digitalWrite(SS_,LOW); // SS pin ENABLE
	
#ifdef DEBUG
	printf("Activating quadrature x4 mode ...\n");
#endif
	
	spiTransfer(fd,WR_MDR0); // Write to register MDR0
	spiTransfer(fd,QUAD_X4); // Activate quadrature x4 mode
	
	digitalWrite(SS_,HIGH); // SS pin DISABLE
	
	return fd;
}

int readCounter(int fd){
	int count=0;
	
	digitalWrite(SS_,LOW); // SS pin ENABLE

	spiTransfer(fd,RD_CNTR); // Read Counter
	count = spiTransfer(fd,0x00); //MSB
	count = count << 8;
	count += spiTransfer(fd,0x00);
	count = count << 8;
	count += spiTransfer(fd,0x00);
	count = count << 8;
	count += spiTransfer(fd,0x00); //LSB
	
	digitalWrite(SS_,HIGH); // SS pin DISABLE
	
	return count;	
}

void clearCounter(int fd){
	digitalWrite(SS_,LOW); // SS pin ENABLE
	
	spiTransfer(fd,CLR_CNTR);// Clear Counter Register

	digitalWrite(SS_,HIGH); // SS pin DISABLE
}

int spiTransfer(int fd, uint8_t msg){
	uint8_t buff;
	int n;
	
	if(msg == 0x00){
		if((n=read(fd,&buff,sizeof(buff))) < 1)
		{
			perror("Can't read spiTransfer...");
			return -1;	
		}
		return buff;
	}
	else
	{
		if((n=write(fd,&msg,sizeof(msg))) < 1)
		{
			perror("Can't transmit data spiTransfer");
			return -1;	
		}
	}
	return 1;
}
