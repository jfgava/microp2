TARGET=test
CC=$(CROSS_COMPILE)gcc
FLAGS=-O2 -Wall -MMD
INCLUDE=-I../include
LIBDIR=-L../lib
LIBS=-lp2 -lgalileo2 -lm

LDFLAGS=$(LIBDIR) $(LIBS)
CFLAGS=$(FLAGS) $(INCLUDE)

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LDFLAGS)
	
clean:
	rm -f *.d

distclean:
	rm -f $(TARGET)
