
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

typedef struct flags {
      uint8_t     z:1;
      uint8_t     s:1;
      uint8_t     p:1;
      uint8_t     cy:1;
      uint8_t     ac:1;
      uint8_t     pad:3;
} flags;

typedef struct cpu {
      uint8_t     a;
      uint8_t     b;
      uint8_t     c;
      uint8_t     d;
      uint8_t     e;
      uint8_t     h;
      uint8_t     l;
      uint16_t    sp;
      uint16_t    pc;
      uint8_t     *memory;
      struct flags flags;
      uint8_t     int_enable;
} cpu;

#define VRAM_START 0x2400

void fail(cpu *i8080);
void emulate_cycle(cpu *i8080);
int init_8080(cpu* c);
