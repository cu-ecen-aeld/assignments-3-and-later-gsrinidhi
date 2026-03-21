CROSS_COMPILE ?= cc

all: writer

ifeq ($(CROSS_COMPILE),aarch64-none-linux-gnu-)
 CC = aarch64-none-linux-gnu-gcc
else
 CC = cc
endif
writer: writer.o
	${CC} writer.o -o writer
writer.o: writer.c
	${CC} -c writer.c
clean:
	rm -f writer.o writer