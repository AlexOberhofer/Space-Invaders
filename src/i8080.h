#pragma once
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
cpu* init_8080(void);
int parity(int x, int size);
void logicFlagA(cpu* i8080);

//CPU OP Functions
void opLXIB(cpu* c, uint8_t bval, uint8_t cval);
void opLXID(cpu* c, uint8_t dval, uint8_t eval);
void opLXIH(cpu* c, uint8_t hval, uint8_t lval);
void opJMPadr(cpu* c, uint8_t* opcode);
void opJMPNZ(cpu* c, uint8_t *opcode);
void opLXIsp(cpu* c, uint16_t stack_val);
void opMVIB(cpu* c, uint8_t val);
void opANIA(cpu* c, uint8_t val);
void opMOVDM(cpu *c, uint16_t offset);
void opMOVMA(cpu *c, uint16_t offset);
void opDADD(cpu *c);
void opCALLAdr(cpu *c, uint16_t return_adr, uint8_t *opcode);
void opLDAXD(cpu *c);
void opINXH(cpu *c);
void opINXD(cpu *c);
void opDCRB(cpu *c);
void opRET(cpu *c);
