CROSS_COMPILE ?= cc

CC = $(CROSS_COMPILE)

all: aesdsocket

aesdsocket: aesdsocket.o
	${CC} aesdsocket.o -o aesdsocket
aesdsocket.o: aesdsocket.c
	${CC} -c aesdsocket.c
clean:
	rm -f aesdsocket.o aesdsocket