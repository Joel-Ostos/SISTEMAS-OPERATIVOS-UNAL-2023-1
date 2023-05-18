CC = gcc
TARGET = p1-odProgram 
SOURCE = p1-odProgram.c

all: $(TARGET)

p1-odProgram: p1-odProgram.c conversor
	gcc  p1-odProgram.c -o p1-odProgram
conversor: conversor.c
	gcc conversor.c -o conversor
clean:
	rm -f $(TARGET)
	rm -r conversor

.PHONY: all clean

