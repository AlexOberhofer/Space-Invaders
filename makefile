CC=clang
CFLAGS=-I.
DEPS=disassembler.h i8080.h
TARGET=invaders

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makeinvaders: src/disassembler.o src/i8080.o src/invaders.o
	@mkdir -p bin
	$(CC) -g -std=c11 -Wall -o ./bin/$(TARGET) ./src/disassembler.c ./src/i8080.c ./src/invaders.c

clean:
	rm ./bin/$(TARGET)
	rm ./src/disassembler.o
	rm ./src/i8080.o
	rm ./src/invaders.o
