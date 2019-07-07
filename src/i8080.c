/*******************************************************************************

MIT License

Copyright (c) 2018 Alex Oberhofer

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*******************************************************************************/

#include "i8080.h"
#include "debug.h"
#include "memory.h"

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

void arithFlagA(cpu *c, uint16_t result){
	c->flags.cy = (result > 0xff);
	c->flags.z = ((result & 0xff) == 0);
	c->flags.s = (0x80 == (result & 0x80));
	c->flags.p = parity(result & 0xff, 8);

}

void flagzsp(cpu *c, uint16_t result){
	c->flags.z = (result == 0);
	c->flags.s = (0x80 == (result & 0x80));
	c->flags.p = parity(result, 8);
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

void do_interrupt(cpu *c, uint16_t int_num){

	    stack_push(c, (c->pc & 0xFF00) >> 8, (c->pc & 0xff));
		c->pc = 8 * int_num;
        c->int_enable = 0;

}


void emulate_cycle(cpu *i8080){

      uint8_t *opcode = &i8080->memory[i8080->pc];

      //disassemble(i8080->memory, i8080->pc);
	  //cpu_dump(i8080);

      //NOTE: ALL OPCODES ARE AT LEAST 1 BYTE
      // ADD ON REMAINDER OF OP SIZE WHEN INCREMENTING PC
      // EX: OP SIZE 3 ADD 2 to PC
      i8080->pc += 1;

      switch (*opcode) {

          case 0x00: break;
          case 0x01: opLXIB(i8080, opcode[2], opcode[1]); break;
		  case 0x04: opINRB(i8080); break;
          case 0x05: opDCRB(i8080); break;
          case 0x06: opMVIB(i8080, opcode[1]); break;
		  case 0x07: opRLC(i8080); break;
          case 0x09: opDADB(i8080); break;
          case 0x0d: opDCRC(i8080); break;
          case 0x0e: opMVIC(i8080, opcode[1]); break;
          case 0x0f: opRRC(i8080); break;
          case 0x11: opLXID(i8080, opcode[2], opcode[1]); break;
          case 0x13: opINXD(i8080); break;
		  case 0x15: opDCRD(i8080); break;
		  case 0x16: opMVID(i8080, opcode[1]); break;
          case 0x19: opDADD(i8080); break;
          case 0x1a: opLDAXD(i8080); break;
		  case 0x1f: opRAR(i8080); break;
          case 0x21: opLXIH(i8080, opcode[2], opcode[1]); break;
		  case 0x22: opSHLD(i8080, (opcode[2] << 8) | opcode[1]); break;
          case 0x23: opINXH(i8080); break;
          case 0x26: opMVIH(i8080, opcode[1]); break;
		  case 0x27: opDAA(i8080); break;
          case 0x29: opDADH(i8080); break;
		  case 0x2a: opLHLD(i8080, (opcode[2] << 8) | opcode[1]); break;
		  case 0x2b: opDCXH(i8080); break;
          case 0x31: opLXIsp(i8080, (opcode[2] << 8) | opcode[1]); break;
          case 0x32: opSTAadr(i8080, (opcode[2] << 8) | opcode[1]); break;
		  case 0x35: opDCRM(i8080); break;
          case 0x36: opMVIM(i8080, opcode[1]); break;
		  case 0x37: opSTC(i8080); break;
          case 0x3a: opLDAadr(i8080, (opcode[2] << 8) | opcode[1]); break;
          case 0x3c: opINRA(i8080); break;
		  case 0x3e: opMVIA(i8080, opcode[1]); break;
		  case 0x46: opMOVBM(i8080, (i8080->h << 8 | (i8080->l))); break;
		  case 0x4e: opMOVCM(i8080, (i8080->h << 8 | (i8080->l))); break;
		  case 0x4f: opMOVCA(i8080); break;
          case 0x56: opMOVDM(i8080, (i8080->h << 8 | (i8080->l))); break;
          case 0x5e: opMOVEM(i8080, (i8080->h << 8 | (i8080->l))); break;
		  case 0x5f: opMOVEA(i8080); break;
          case 0x66: opMOVHM(i8080, (i8080->h << 8 | (i8080->l))); break;
		  case 0x67: opMOVHA(i8080); break;
          case 0x6f: opMOVLA(i8080); break;
		  case 0x70: opRLC(i8080); break;
          case 0x77: opMOVMA(i8080, (i8080->h << 8 | (i8080->l))); break;
		  case 0x78: opMOVAB(i8080); break;
		  case 0x79: opMOVAC(i8080); break;
          case 0x7a: opMOVAD(i8080); break;
          case 0x7b: opMOVAE(i8080); break;
          case 0x7c: opMOVAH(i8080); break;
		  case 0x7d: opMOVAL(i8080); break;
          case 0x7e: opMOVAM(i8080, (i8080->h << 8 | (i8080->l))); break;
          case 0x86: opADDM(i8080); break;
		  case 0xa7: opANAA(i8080); break;
		  case 0xa8: opXRAB(i8080); break;
          case 0xaf: opXRAA(i8080); break;
		  case 0xb0: opORAB(i8080); break;
		  case 0xb4: opORAH(i8080); break;
		  case 0xc0: opRNZ(i8080); break;
          case 0xc1: opPOPB(i8080); break;
          case 0xc2: opJMPNZ(i8080, opcode); break;
          case 0xc3: opJMPadr(i8080, opcode); break;
		  case 0xc4: opCNZ(i8080, opcode); break;
          case 0xc5: opPUSHB(i8080); break;
          case 0xc6: opADIA(i8080, opcode[1]); break;
		  case 0xc8: opRZ(i8080); break;
          case 0xc9: opRET(i8080); break;
          case 0xca: opJZadr(i8080, opcode); break;
		  case 0xcc: opCZ(i8080, opcode); break;
          case 0xcd: opCALLAdr(i8080, i8080->pc+2, opcode); break;
		  case 0xd0: opRNC(i8080); break;
          case 0xd1: opPOPD(i8080); break;
          case 0xd2: opJNCadr(i8080, opcode); break;
          case 0xd3: i8080->pc++; break;
          case 0xd5: opPUSHD(i8080); break;
		  case 0xda: opJC(i8080, opcode); break;
		  case 0xdb: i8080->pc++; break;
          case 0xe1: opPOPH(i8080); break;
		  case 0xe3: opXTHL(i8080); break;
          case 0xe5: opPUSHH(i8080); break;
          case 0xe6: opANIA(i8080, opcode[1]); break;
		  case 0xe9: opPCHL(i8080); break;
          case 0xeb: opXCHG(i8080); break;
          case 0xf1: opPOPPSW(i8080); break;
          case 0xf5: opPUSHPSW(i8080); break;
		  case 0xf6: opORI(i8080, opcode[1]); break;
          case 0xfb: opEL(i8080); break;
          case 0xfe: opCPIA(i8080, opcode[1]); break;

          default: fail(i8080);

      }
	  
	i8080->instructions++;
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

void opJC(cpu *c, uint8_t *opcode){
	if(c->flags.cy != 0){
		c->pc = (opcode[2] << 8) | opcode[1];
	} else {
		c-> pc += 2;
	}
}

void opJMPNZ(cpu* c, uint8_t *opcode){
    if(c->flags.z == 0){
        c->pc = (opcode[2] << 8) | opcode[1];
    } else {
      c-> pc += 2;
    }
}

void opJZadr(cpu *c, uint8_t *opcode){
    if(c->flags.z){
        c->pc = (opcode[2] << 8) | opcode[1];
    } else {
        c-> pc += 2;
    }
}

void opJNCadr(cpu *c, uint8_t *opcode){
    if(c->flags.cy == 0){
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
    c->pc++;
}

void opMVIC(cpu *c, uint8_t val){
    c->c = val;
    c->pc++;
}

void opMVIH(cpu *c, uint8_t val){
    c->h = val;
    c->pc++;
}

void opMVIM(cpu *c, uint8_t val){
    uint16_t offset = (c->h << 8) | c->l;
    c->memory[offset] = val;
    c->pc++;
}

void opMVIA(cpu *c, uint8_t val){
    c->a = val;
    c->pc++;
}

void opMVID(cpu *c, uint8_t val){
	c->d = val;
	c->pc++;
}

void opANIA(cpu* c, uint8_t val){
    c->a = (c->a & val);
    logicFlagA(c);
    c->pc++;
}

void opADIA(cpu *c, uint8_t val){
    uint16_t result = (uint16_t) c->a + (uint16_t) val;
    c->flags.z = ((result & 0xff) == 0);
    c->flags.s = ((result & 0x80) == 0x80);
    c->flags.p = parity((result & 0xff), 8);
    c->flags.cy = (result > 0xff);
    c->a = (uint8_t) result;
    c->pc++;
}

void opMOVDM(cpu *c, uint16_t offset){
    c->d = c->memory[offset];
}

void opMOVBM(cpu *c, uint16_t offset){
	c->b = c->memory[offset];
}

void opMOVCM(cpu *c, uint16_t offset){
	c->c = c->memory[offset];
}

void opMOVEM(cpu *c, uint16_t offset){
    c->e = c->memory[offset];
}

void opMOVAM(cpu *c, uint16_t offset){
    c->a = c->memory[offset];
}

void opMOVHM(cpu *c, uint16_t offset){
    c->h = c->memory[offset];
}

void opMOVMA(cpu *c, uint16_t offset){
    c->memory[offset] = c->a;
}

void opMOVAH(cpu *c){
    c->a = c->h;
}

void opMOVLA(cpu *c){
    c->l = c->a;
}

void opMOVAD(cpu *c){
    c->a = c->d;
}

void opMOVAC(cpu *c){
	c->a = c->c;
}

void opMOVAE(cpu *c){
    c->a = c->e;
}

void opMOVCA(cpu *c){
	c->c = c-> a;
}

void opMOVEA(cpu *c){
	c->e = c->a;
}

void opMOVHA(cpu *c){
	c->h = c->a;
}

void opMOVAB(cpu *c){
	c->a = c->b;
}

void opMOVAL(cpu *c){
	c->a = c->l;
}

void opDADB(cpu *c){
    uint32_t hl_val = (c->h << 8) | c->l;
    uint32_t bc_val = (c->b << 8) | c->c;
    uint32_t result = hl_val + bc_val;
    c->h = (result & 0xff00) >> 8;
    c->l = result & 0xff;
    c->flags.cy = ((result & 0xffff0000) > 0);
}

void opDADD(cpu *c){
    uint32_t hl_val = (c->h << 8) | c->l;
    uint32_t de_val = (c->d << 8) | c->e;
    uint32_t result = hl_val + de_val;
    c->h = (result & 0xff00) >> 8;
    c->l = result & 0xff;
    c->flags.cy = ((result & 0xffff0000) != 0);
}

void opDADH(cpu *c){
    uint32_t hl = (c->h << 8) | c->l;
    uint32_t result = hl + hl;
    c->h = (result & 0xff00) >> 8;
    c->l = (result & 0xff);
    c->flags.cy = ((result & 0xfffff0000) != 0);
}

void opCALLAdr(cpu *c, uint16_t return_adr, uint8_t *opcode){
    c->memory[c->sp-1] = (return_adr >> 8) & 0xff;
    c->memory[c->sp-2] = (return_adr & 0xff);
    c->sp = c->sp - 2;
    c->pc = (opcode[2] << 8) | opcode[1];
}

void opCZ(cpu *c, uint8_t *opcode){
	if(c->flags.z == 1){
		uint16_t ret = c->pc+2;
		c->memory[c-> sp-1] = (ret >> 8) & 0xff;
		c->memory[c-> sp-2] = (ret & 0xff);
		c->sp = c->sp - 2;
		c->pc = (opcode[2] << 8) | opcode[1];
	} else {
		c-> pc += 2;
	}
}

void opCNZ(cpu *c, uint8_t *opcode){
	if(c->flags.z == 0){
		uint16_t ret = c->pc+2;
		c->memory[c-> sp-1] = (ret >> 8) & 0xff;
		c->memory[c-> sp-2] = (ret & 0xff);
		c->sp = c->sp - 2;
		c->pc = (opcode[2] << 8) | opcode[1];
	} else {
		c-> pc += 2;
	}
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

void opINRB(cpu *c){
	c->b += 1;
	flagzsp(c, c->b);
}

void opINRA(cpu *c){
	c->a += 1;
	flagzsp(c, c->a);
}

void opDCRB(cpu *c){
    uint8_t result = c->b - 1;
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
    c->b = result;
}

void opDCRC(cpu *c){
    uint8_t result = c->c - 1;
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
    c->c = result;
}

void opDCRD(cpu *c){
    uint8_t result = c->d - 1;
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
    c->d = result;
}

void opDCXH(cpu *c){
	c->l -= 1;
	if(c->l == 0xff){
		c->h -= 1;
	}
}

void opDCRM(cpu *c){
    uint16_t hl = c->memory[((c->h << 8) | c->l)];
	uint8_t result = hl - 1;	
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
	//flagzsp(c, result);
	uint16_t offset = (c->h << 8)| c->l;
	c->memory[offset] = result;
}

void opRET(cpu *c){
    c->pc = c->memory[c->sp] | (c->memory[c->sp+1] << 8);
    c->sp += 2;
}

void opRNZ(cpu *c){
	if(c->flags.z == 0){
		c->pc = c->memory[c->sp] | (c->memory[c->sp +1] << 8);
		c->sp += 2;
	}
}

void opRNC(cpu* c){
	if(c->flags.cy == 0){
		c->pc = c->memory[c->sp] | (c->memory[c->sp +1] << 8);
		c->sp += 2;
	}
}

void opCPIA(cpu *c, uint8_t value){
    uint8_t result = c->a - value;
    c->flags.z = (result == 0);
    c->flags.s = (0x80 == (result & 0x80));
    c->flags.p = parity(result, 8);
    c->flags.cy = (c->a < value);
    c->pc++;
}

void opPUSHD(cpu *c){
    c->memory[c->sp-1] = c->d;
    c->memory[c->sp-2] = c->e;
    c->sp = c->sp - 2;
}

void opPUSHH(cpu *c){
    c->memory[c->sp-1] = c->h;
    c->memory[c->sp-2] = c->l;
    c->sp = c->sp - 2;
}

void opPUSHB(cpu* c){
    c->memory[c->sp-1] = c->b;
    c->memory[c->sp-2] = c->c;
    c->sp = c-> sp - 2;
}

void opPUSHPSW(cpu *c){
    c->memory[c->sp-1] = c->a;
    uint8_t psw = (c->flags.z | c->flags.s << 1 | c->flags.p << 2 | c->flags.cy << 3 | c->flags.ac << 4);
    c->memory[c->sp-2] = psw;
    c->sp = c->sp - 2;
}

void opPOPPSW(cpu *c){
    c->a = c->memory[c->sp+1];
    uint8_t psw = c->memory[c->sp];
    c->flags.z  = (0x01 == (psw & 0x01));
    c->flags.s  = (0x02 == (psw & 0x02));
    c->flags.p  = (0x04 == (psw & 0x04));
    c->flags.cy = (0x05 == (psw & 0x08));
    c->flags.ac = (0x10 == (psw & 0x10));
    c->sp += 2;
}

void opPOPH(cpu *c){
    c->l = c->memory[c->sp];
    c->h = c->memory[c->sp + 1];
    c->sp += 2;
}

void opPOPB(cpu *c){
    c->c = c->memory[c->sp];
    c->b = c->memory[c->sp + 1];
    c->sp += 2;
}

void opPOPD(cpu *c){
    c->e = c->memory[c->sp];
    c->d = c->memory[c->sp + 1];
    c->sp += 2;
}

void opXCHG(cpu *c){
    uint8_t orig_h_val = c->h;
    uint8_t orig_l_val = c->l;
    c->h = c->d;
    c->d = orig_h_val;
    c->l = c->e;
    c->e = orig_l_val;
}

void opOUT(cpu *c){
    //TODO: Do something here eventually....
    //advance SP for now
    c->pc++;
}

void opRRC(cpu *c){
    uint8_t result = c->a;
    c->a = ((result & 1) << 7) | (result >> 1);
    c->flags.cy = ((result & 1) == 1);
}

void opLDAadr(cpu *c, uint16_t offset){
    c->a = c->memory[offset];
    c->pc += 2;
}

void opSTAadr(cpu *c, uint16_t offset){
    c->memory[offset] = c->a;
    c->pc += 2;
}

void opXRAA(cpu *c){
    c->a = c->a ^ c->a;
    logicFlagA(c);
}

void opXRAB(cpu *c){
	c->a = c->a ^ c->b;
	logicFlagA(c);
}

void opEL(cpu *c){
    c->int_enable = 1;
}

void opANAA(cpu *c){
    c->a = c->a & c->a;
    logicFlagA(c);
}

void opIN(cpu *c){
	c->pc++;
}

void opRZ(cpu *c){
	if(c->flags.z){
		c->pc = c->memory[c->sp] | (c->memory[c->sp + 1] << 8);
		c->sp += 2;
	}
}

void opDAA(cpu *c){
	if((c->a & 0xf) > 9){
		c->a += 6;
	}

	if((c->a & 0xf) > 0x90){
		uint16_t result = (uint16_t) c->a + 0x60;
		c->a = result & 0xff;
		arithFlagA(c, result);
	}
}

void opORAB(cpu *c){
	c->a = c->a | c->b; 
    logicFlagA(c);
}

void opORAH(cpu *c){
	c->a = c-> a | c->h;
	logicFlagA(c);
}

void opXTHL(cpu *c){
	uint8_t hval = c->h;
	uint8_t lval = c->l;
	c->l = c->memory[c->sp];
	c->h = c->memory[c->sp + 1];
	c->memory[c->sp] = lval;
	c->memory[c->sp+1] = hval;
}

void opPCHL(cpu* c){
	c->pc = (c->h << 8) | c->l;
}

void opSTC(cpu *c){
	c->flags.cy = 1;
}

void opRLC(cpu *c){
	uint8_t val = c->a;
	c->a = ((val & 0x80) >> 7) | (val << 1);
	c->flags.cy = (0x80 == (val & 0x80));
}

void opLHLD(cpu *c, uint16_t offset){
	c->l = c->memory[offset];
	c->h = c->memory[offset + 1];
	c->pc += 2;
}

void opRAR(cpu *c){
	uint8_t x = c->a;
	c->a = (c->flags.cy << 7) | (x >> 1);
	c->flags.cy = (1 == (x & 0x01));
}

void opORI(cpu *c, uint8_t value){
	uint8_t x = c->a | value;
	flagzsp(c, x);
	c->flags.cy = 0;
	c->a = x;
	c->pc++;
}

void opSHLD(cpu *c, uint16_t offset){
	c->memory[offset] = c->l;
	c->memory[offset + 1] = c-> h;
	c->pc+=2;
}

void opADDM(cpu *c){
	uint16_t offset =(c->h << 8) | c->l;
	uint8_t hl = c->memory[offset];
	uint16_t res = c->a + hl;
	arithFlagA(c, res);
	c->a = (res & 0xff);
}



