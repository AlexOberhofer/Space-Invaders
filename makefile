CC=clang
DEPS=debug.h i8080.h memory.h
TARGET=invaders

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

makeinvaders: src/debug.o src/i8080.o src/system.o src/memory.o
	@mkdir -p bin
	$(CC) -g -std=c11 -Wall -o ./bin/$(TARGET) ./src/debug.c ./src/memory.c ./src/i8080.c ./src/system.c `sdl2-config --cflags --libs`

clean:
	rm ./bin/$(TARGET)
	rm ./src/debug.o
	rm ./src/i8080.o
	rm ./src/system.o
	rm ./src/memory.o
