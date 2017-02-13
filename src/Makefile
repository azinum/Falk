#
# Makefile
# Author: Azinum
# Date: 2016-11-30
#

PLATFORM= none

PLATFORMS= mac linux

CC= gcc

CFLAGS= -std=c99 -Os

LIBS= -ldl -lm

all: none

clean:
	rm *.o

help:
	@echo "Avaiable options are: $(PLATFORMS)."

mac:
	@echo "Compiling for mac..."
	$(CC) -o Falk *.c $(CFLAGS) $(LIBS)
	@echo "Compilation complete"

linux:
	@echo "Compiling for Linux..."
	$(CC) -o Falk *.c $(CFLAGS) $(LIBS) -rdynamic
	@echo "Compilation complete"

none:
	@echo "No option is selected. Using default option (Mac)"
	mac
