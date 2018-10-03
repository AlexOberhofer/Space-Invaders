CC=clang
CFLAGS=-I.
DEPS=disassembler.h
TARGET=disassembler

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makeinvaders: src/$(TARGET).o
	@mkdir -p bin
	$(CC) -g -std=c11 -Wall -o ./bin/$(TARGET) ./src/$(TARGET).c

clean:
	rm ./bin/disassembler
	rm ./src/$(TARGET).o