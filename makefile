CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors

6502: 6502.o
6502.o: 6502.c 6502.h

clean:
	del *.o 6502.exe
