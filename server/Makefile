CROSS_COMPILE ?= cc

CC = $(CROSS_COMPILE)

all: aesdsocket

aesdsocket: aesdsocket.c
	${CC} -g aesdsocket.c -o aesdsocket
clean:
	rm -f aesdsocket.o aesdsocket