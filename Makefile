CC = gcc
CFLAGS = -g -Wall -lm
 
all: build

build: tema1.c
	$(CC) -o tema1 main.c tema1.c $(CFLAGS)

.PHONY : clean
clean :
	rm -f tema1
	rm -f *.out
