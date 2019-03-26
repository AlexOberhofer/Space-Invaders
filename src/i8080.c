#include "i8080.h"
#include "debug.h"

void fail(cpu *i8080){
      printf("Error: Instruction not implemented. Exiting...\n");
      printf("DIAG INFO: \n");
      i8080->pc--;
      cpu_dump(i8080);
      exit(1);
}

cpu* init_8080(void){
      cpu* c = calloc(1, sizeof(cpu));
      c->memory = calloc(0x10000, 1);
      c-> sp &= 0x0;
      c-> pc &= 0x0;
      return c;
}

void emulate_cycle(cpu *i8080){

      int cycles = 4;

      uint8_t *opcode = &i8080->memory[i8080->pc];

      disassemble(i8080->memory, i8080->pc);
      printf("\n");

      i8080->pc += 1;

      switch (*opcode) {
          case 0x00: break;
          case 0x01: opLXIB(i8080, opcode[2], opcode[1]); break;
          case 0x05: fail(i8080); break;
          case 0x06: fail(i8080); break;
          case 0x09: fail(i8080); break;
          case 0x0d: fail(i8080); break;
          case 0x0e: fail(i8080); break;
          case 0x0f: fail(i8080); break;
          case 0x11: fail(i8080); break;
          case 0x13: fail(i8080); break;
          case 0x19: fail(i8080); break;
          case 0x1a: fail(i8080); break;
          case 0x21: fail(i8080); break;
          case 0x23: fail(i8080); break;
          case 0x26: fail(i8080); break;
          case 0x29: fail(i8080); break;
          case 0x31: fail(i8080); break;
          case 0x32: fail(i8080); break;
          case 0x36: fail(i8080); break;
          case 0x3a: fail(i8080); break;
          case 0x3e: fail(i8080); break;
          case 0x56: fail(i8080); break;
          case 0x5e: fail(i8080); break;
          case 0x66: fail(i8080); break;
          case 0x6f: fail(i8080); break;
          case 0x77: fail(i8080); break;
          case 0x7a: fail(i8080); break;
          case 0x7b: fail(i8080); break;
          case 0x7c: fail(i8080); break;
          case 0x7e: fail(i8080); break;
          case 0xa7: fail(i8080); break;
          case 0xaf: fail(i8080); break;
          case 0xc1: fail(i8080); break;
          case 0xc2: fail(i8080); break;
          case 0xc3: opJMPadr(i8080, opcode); break;
          case 0xc5: fail(i8080); break;
          case 0xc6: fail(i8080); break;
          case 0xc9: fail(i8080); break;
          case 0xcd: fail(i8080); break;
          case 0xd1: fail(i8080); break;
          case 0xd3: fail(i8080); break;
          case 0xd5: fail(i8080); break;
          case 0xe1: fail(i8080); break;
          case 0xe5: fail(i8080); break;
          case 0xe6: fail(i8080); break;
          case 0xeb: fail(i8080); break;
          case 0xf1: fail(i8080); break;
          case 0xf5: fail(i8080); break;
          case 0xfb: fail(i8080); break;
          case 0xfe: fail(i8080); break;

          default: fail(i8080);

      }
}

void opLXIB(cpu* c, uint8_t bval, uint8_t cval) {
    c->c = cval;
    c->b = bval;
    c->pc += 2;
}

void opJMPadr(cpu* c, uint8_t* opcode){
    c->pc = (opcode[2] << 8) | opcode[1];
}
