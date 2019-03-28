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

void logicFlagA(cpu* i8080){
      i8080->flags.cy = i8080->flags.ac = 0;
      i8080->flags.z = (i8080-> a == 0);
      i8080->flags.s = (0x80 == (i8080->a & 0x80));
      i8080->flags.p = parity(i8080->a, 8);
}

int parity(int x, int size){
      int i = 0;
      int p= 0;
      x = (x & ((1<<size)-1));
      for(i = 0; i< size; i++){
          if(x & 0x1) {
              p++;
          }
          x = x >> 1;
      }
      return (0 == (p & 0x1));
}

void emulate_cycle(cpu *i8080){

      int cycles = 4;

      uint8_t *opcode = &i8080->memory[i8080->pc];

      disassemble(i8080->memory, i8080->pc);
      printf("\n");

      //NOTE: ALL OPCODES ARE AT LEAST 1 BYTE
      // ADD ON REMAINDER OF OP SIZE WHEN INCREMENTING PC
      // EX: OP SIZE 3 ADD 2 to PC
      i8080->pc += 1;

      switch (*opcode) {
          case 0x00: break;
          case 0x01: opLXIB(i8080, opcode[2], opcode[1]); break;
          case 0x05: opDCRB(i8080); break;
          case 0x06: opMVIB(i8080, opcode[1]); break;
          case 0x09: fail(i8080); break;
          case 0x0d: fail(i8080); break;
          case 0x0e: fail(i8080); break;
          case 0x0f: fail(i8080); break;
          case 0x11: opLXID(i8080, opcode[2], opcode[1]); break;
          case 0x13: opINXD(i8080); break;
          case 0x19: opDADD(i8080); break;
          case 0x1a: opLDAXD(i8080); break;
          case 0x21: opLXIH(i8080, opcode[2], opcode[1]); break;
          case 0x23: opINXH(i8080); break;
          case 0x26: fail(i8080); break;
          case 0x29: fail(i8080); break;
          case 0x31: opLXIsp(i8080, (opcode[2] << 8) | opcode[1]); break;
          case 0x32: fail(i8080); break;
          case 0x36: fail(i8080); break;
          case 0x3a: fail(i8080); break;
          case 0x3e: fail(i8080); break;
          case 0x56: opMOVDM(i8080, (i8080->h << 8 | (i8080->l))); break;
          case 0x5e: fail(i8080); break;
          case 0x66: fail(i8080); break;
          case 0x6f: fail(i8080); break;
          case 0x77: opMOVMA(i8080, (i8080->h << 8 | (i8080->l))); break;
          case 0x7a: fail(i8080); break;
          case 0x7b: fail(i8080); break;
          case 0x7c: fail(i8080); break;
          case 0x7e: fail(i8080); break;
          case 0xa7: fail(i8080); break;
          case 0xaf: fail(i8080); break;
          case 0xc1: fail(i8080); break;
          case 0xc2: opJMPNZ(i8080, opcode); break;
          case 0xc3: opJMPadr(i8080, opcode); break;
          case 0xc5: fail(i8080); break;
          case 0xc6: fail(i8080); break;
          case 0xc9: opRET(i8080); break;
          case 0xcd: opCALLAdr(i8080, i8080->pc+2, opcode); break;
          case 0xd1: fail(i8080); break;
          case 0xd3: fail(i8080); break;
          case 0xd5: fail(i8080); break;
          case 0xe1: fail(i8080); break;
          case 0xe5: fail(i8080); break;
          case 0xe6: opANIA(i8080, opcode[1]); break;
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

void opLXID(cpu* c, uint8_t dval, uint8_t eval){
    c->d = dval;
    c->e = eval;
    c->pc += 2;
}

void opLXIH(cpu* c, uint8_t hval, uint8_t lval){
    c->h = hval;
    c->l = lval;
    c->pc += 2;
}

void opJMPadr(cpu* c, uint8_t* opcode){
    c->pc = (opcode[2] << 8) | opcode[1];
}

void opJMPNZ(cpu* c, uint8_t *opcode){
    if(c->flags.z == 0){
        c->pc = (opcode[2] << 8) | opcode[1];
    } else {
      c-> pc += 2;
    }
}

void opLXIsp(cpu* c, uint16_t stack_val){
    c->sp = stack_val;
    c->pc +=2;
}

void opMVIB(cpu* c, uint8_t val){
    c->b = val;
    c->pc += 1;
}

void opANIA(cpu* c, uint8_t val){
    c->a = (c->a & val);
    logicFlagA(c);
    c->pc += 2;
}

void opMOVDM(cpu *c, uint16_t offset){
    c->d = c->memory[offset];
}

void opMOVMA(cpu *c, uint16_t offset){
    c->memory[offset] = c->a;
}

void opDADD(cpu *c){
    uint32_t hl_val = (c->h << 8) | c->l;
    uint32_t de_val = (c->d << 8) | c->e;
    uint32_t result = hl_val + de_val;
    c->h = (result & 0xff00) >> 8;
    c->l = result & 0xff;
    c->flags.cy = ((result & 0xffff0000) != 0);
}

void opCALLAdr(cpu *c, uint16_t return_adr, uint8_t *opcode){
    c->memory[c->sp-1] = (return_adr >> 8) & 0xff;
    c->memory[c->sp-2] = (return_adr & 0xff);
    c->sp = c->sp - 2;
    c->pc = (opcode[2] << 8) | opcode[1];
}

void opLDAXD(cpu *c) {
    uint16_t offset = (c->d << 8) | c->e;
    c->a = c->memory[offset];
}

void opINXH(cpu *c){
    c->l++;
    if(c->l == 0){
        c->h++;
    }
}

void opINXD(cpu *c){
    c->e++;
    if(c->e == 0){
        c->d++;
    }
}

void opDCRB(cpu *c){
    uint8_t result = c->b - 1;
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
    c->b = result;
}

void opRET(cpu *c){
    c->pc = c->memory[c->sp] | (c->memory[c->sp+1] << 8);
    c->sp += 2;
}
