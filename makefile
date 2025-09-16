
CC = gcc

CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations \
		-Wmissing-prototypes -Wold-style-definition

OBJ_COMPLINE = $(CC) $(CFLAGS) -c $<

all: mexec.o parser.o process_handler.o
	$(CC) $(CFLAGS) $^ -o mexec

mexec.o: mexec.c parser.h process_handler.h
	$(OBJ_COMPLINE)

parser.o: parser.c 
	$(OBJ_COMPLINE)

process_handler.o: process_handler.c 
	$(OBJ_COMPLINE)

clean:
	rm -f all *.o 
