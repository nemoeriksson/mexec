
CC = gcc

CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic -Wmissing-declarations \
		-Wmissing-prototypes -Wold-style-definition

OBJ_COMPLINE = $(CC) $< -c $(CFLAGS)

all: main.o parser.o error_handler.o
	$(CC) $^ $(CFLAGS) -o mexec

main.o: main.c error_handler.h parser.h
	$(OBJ_COMPLINE)

parser.o: parser.c error_handler.h
	$(OBJ_COMPLINE)

process_handler.o: process_handler.c 
	$(OBJ_COMPLINE)

error_handler.o: error_handler.c
	$(OBJ_COMPLINE)

clean:
	rm -f all *.o 
