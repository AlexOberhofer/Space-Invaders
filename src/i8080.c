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
		//Push(c, (c->pc & 0xFF00) >> 8, (c->pc & 0xff));
		c->pc = 8 * int_num;
        c->int_enable = 0;

}


void emulate_cycle(cpu *i8080){

	//for(int i = 0; i <250; i++ ){

      uint8_t *opcode = &i8080->memory[i8080->pc];

      disassemble(i8080->memory, i8080->pc);


	  //cpu_dump(i8080);

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
		  case 0x07: break;
          case 0x09: opDADB(i8080); break;
          case 0x0d: opDCRC(i8080); break;
          case 0x0e: opMVIC(i8080, opcode[1]); break;
          case 0x0f: opRRC(i8080); break;
          case 0x11: opLXID(i8080, opcode[2], opcode[1]); break;
          case 0x13: opINXD(i8080); break;
		  case 0x16: opMVID(i8080, opcode[1]); break;
          case 0x19: opDADD(i8080); break;
          case 0x1a: opLDAXD(i8080); break;
          case 0x21: opLXIH(i8080, opcode[2], opcode[1]); break;
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
          case 0x3e: opMVIA(i8080, opcode[1]); break;
		  case 0x46: opMOVBM(i8080, (i8080->h << 8 | (i8080->l))); break;
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
          case 0xa7: opANAA(i8080); break;
		  case 0xa8: opXRAB(i8080); break;
          case 0xaf: opXRAA(i8080); break;
		  case 0xb0: opORAB(i8080); break;
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
          case 0xfb: opEL(i8080); break;
          case 0xfe: opCPIA(i8080, opcode[1]); break;

          default: fail(i8080);

      //}
	  
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

/**
 * DEBUG CODE BELOW - KP MILLER BACKEND https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
 **/

void UnimplementedInstruction(cpu *c){
    //do nothing... debug
}

static void WriteMem(cpu* state, uint16_t address, uint8_t value)
{
    if (address < 0x2000)
    {
        //        printf("Writing ROM not allowed %x\n", address);
        return;
    }
    if (address >=0x4000)
    {
        //       printf("Writing out of Space Invaders RAM not allowed %x\n", address);
        return;
    }

    state->memory[address] = value;
}

static uint8_t ReadFromHL(cpu* state)
{
    uint16_t offset = (state->h << 8) | state->l;
    return state->memory[offset];
}

static void WriteToHL(cpu* state, uint8_t value)
{
    uint16_t offset = (state->h << 8) | state->l;
    WriteMem(state, offset, value);
}

static void Push(cpu* state, uint8_t high, uint8_t low)
{
    WriteMem(state, state->sp-1, high);
    WriteMem(state, state->sp-2, low);
    state->sp = state->sp - 2;
    //    printf ("%04x %04x\n", state->pc, state->sp);
}

static void Pop(cpu* state, uint8_t *high, uint8_t *low)
{
    *low = state->memory[state->sp];
    *high = state->memory[state->sp+1];
    state->sp += 2;
    //    printf ("%04x %04x pop\n", state->pc, state->sp);
}

static void FlagsZSP(cpu *state, uint8_t value)
{
    state->flags.z = (value == 0);
    state->flags.s = (0x80 == (value & 0x80));
    state->flags.p = parity(value, 8);
}

static void LogicFlagsA(cpu *state)
{
	state->flags.cy = state->flags.ac = 0;
	state->flags.z = (state->a == 0);
	state->flags.s = (0x80 == (state->a & 0x80));
	state->flags.p = parity(state->a, 8);
}

static void ArithFlagsA(cpu *state, uint16_t res)
{
	state->flags.cy = (res > 0xff);
	state->flags.z = ((res&0xff) == 0);
	state->flags.s = (0x80 == (res & 0x80));
	state->flags.p = parity(res&0xff, 8);
}

int Emulate8080Op(cpu* state)
{

	//for(int i = 0; i <150; i++ ){


	unsigned char *opcode = &state->memory[state->pc];

	//printf("%d ", state->instructions);
	//disassemble(state->memory, state->pc);

#if PRINTOPS
    Disassemble8080Op(state->memory, state->pc);
#endif

	state->pc+=1;

	switch (*opcode)
	{
		case 0x00: break;	//NOP
		case 0x01: 							//LXI	B,word
			state->c = opcode[1];
			state->b = opcode[2];
			state->pc += 2;
			break;
		case 0x02: 							//STAX B
        {
            uint16_t offset=(state->b<<8) | state->c;
            WriteMem(state, offset, state->a);
        }
			break;
		case 0x03: 							//INX    B
			state->c++;
			if (state->c == 0)
				state->b++;
			break;
		case 0x04: 							//INR	B
            state->b += 1;
            FlagsZSP(state,state->b);
			break;
		case 0x05: 							//DCR    B
			state->b -= 1;
            FlagsZSP(state,state->b);
			break;
		case 0x06: 							//MVI B,byte
			state->b = opcode[1];
			state->pc++;
			break;
		case 0x07: 							//RLC
        {
            uint8_t x = state->a;
            state->a = ((x & 0x80) >> 7) | (x << 1);
            state->flags.cy = (0x80 == (x&0x80));
        }
			break;
		case 0x08: UnimplementedInstruction(state); break;
		case 0x09: 							//DAD B
        {
			uint32_t hl = (state->h << 8) | state->l;
			uint32_t bc = (state->b << 8) | state->c;
			uint32_t res = hl + bc;
			state->h = (res & 0xff00) >> 8;
			state->l = res & 0xff;
			state->flags.cy = ((res & 0xffff0000) != 0);
        }
			break;
		case 0x0a: 							//LDAX   B
        {
			uint16_t offset=(state->b<<8) | state->c;
			state->a = state->memory[offset];
        }
			break;
		case 0x0b:							//DCX B
			state->c -= 1;
			if (state->c==0xff)
				state->b-=1;
			break;
		case 0x0c:                          //INR C
            state->c += 1;
            FlagsZSP(state,state->c);
			break;

		case 0x0d: 							//DCR C
            state->c -= 1;
            FlagsZSP(state,state->c);
			break;
		case 0x0e: 							//MVI C,byte
			state->c = opcode[1];
			state->pc++;
			break;
		case 0x0f: 							//RRC
        {
            uint8_t x = state->a;
            state->a = ((x & 1) << 7) | (x >> 1);
            state->flags.cy = (1 == (x&1));
        }
			break;

		case 0x10: UnimplementedInstruction(state); break;
		case 0x11: 							//LXI	D,word
			state->e = opcode[1];
			state->d = opcode[2];
			state->pc += 2;
			break;
		case 0x12: 							//STAX D
        {
            uint16_t offset=(state->d<<8) | state->e;
            WriteMem(state, offset, state->a);
        }
			break;
		case 0x13: 							//INX    D
			state->e++;
			if (state->e == 0)
				state->d++;
			break;
		case 0x14: 							//INR D
            state->d += 1;
            FlagsZSP(state,state->d);
			break;
		case 0x15: 							//DCR    D
            state->d -= 1;
            FlagsZSP(state,state->d);
			break;
		case 0x16: 							//MVI D,byte
			state->d = opcode[1];
			state->pc++;
			break;
		case 0x17:                           //RAL
        {
            uint8_t x = state->a;
            state->a = state->flags.cy  | (x << 1);
            state->flags.cy = (0x80 == (x&0x80));
        }
			break;
		case 0x18: UnimplementedInstruction(state); break;
		case 0x19: 							//DAD    D
        {
			uint32_t hl = (state->h << 8) | state->l;
			uint32_t de = (state->d << 8) | state->e;
			uint32_t res = hl + de;
			state->h = (res & 0xff00) >> 8;
			state->l = res & 0xff;
			state->flags.cy = ((res & 0xffff0000) != 0);
        }
			break;
		case 0x1a: 							//LDAX	D
        {
			uint16_t offset=(state->d<<8) | state->e;
			state->a = state->memory[offset];
        }
			break;
		case 0x1b: 							//DCX D
			state->e -= 1;
			if (state->e==0xff)
				state->d-=1;
			break;
		case 0x1c:							//INR E
            state->e += 1;
            FlagsZSP(state,state->e);
			break;

		case 0x1d: 							//DCR    E
            state->e -= 1;
            FlagsZSP(state,state->e);
			break;
		case 0x1e: 							//MVI E,byte
			state->e = opcode[1];
			state->pc++;
			break;
		case 0x1f:                           //RAR
        {
            uint8_t x = state->a;
            state->a = (state->flags.cy << 7) | (x >> 1);
            state->flags.cy = (1 == (x&1));
        }
			break;
		case 0x20: UnimplementedInstruction(state); break;
		case 0x21: 							//LXI	H,word
			state->l = opcode[1];
			state->h = opcode[2];
			state->pc += 2;
			break;
		case 0x22: 							//SHLD
        {
            uint16_t offset = opcode[1] | (opcode[2] << 8);
            WriteMem(state, offset, state->l);
            WriteMem(state, offset+1, state->h);
            state->pc += 2;
        }
			break;
		case 0x23: 							//INX    H
			state->l++;
			if (state->l == 0)
				state->h++;
			break;
		case 0x24:							//INR	H
            state->h += 1;
            FlagsZSP(state,state->h);
			break;
		case 0x25: 							//DCR    H
            state->h -= 1;
            FlagsZSP(state,state->h);
			break;
		case 0x26:  							//MVI H,byte
			state->h = opcode[1];
			state->pc++;
			break;
		case 0x27:
            if ((state->a &0xf) > 9)
                state->a += 6;
            if ((state->a&0xf0) > 0x90)
            {
                uint16_t res = (uint16_t) state->a + 0x60;
                state->a = res & 0xff;
                ArithFlagsA(state, res);
            }
            break;
		case 0x28: UnimplementedInstruction(state); break;
		case 0x29: 								//DAD    H
        {
			uint32_t hl = (state->h << 8) | state->l;
			uint32_t res = hl + hl;
			state->h = (res & 0xff00) >> 8;
			state->l = res & 0xff;
			state->flags.cy = ((res & 0xffff0000) != 0);
        }
			break;
		case 0x2a: 								//LHLD adr
        {
            uint16_t offset = opcode[1] | (opcode[2] << 8);
            state->l = state->memory[offset];
            state->h = state->memory[offset+1];
            state->pc += 2;
        }
			break;
		case 0x2b: 								//DCX H
			state->l -= 1;
			if (state->l==0xff)
				state->h-=1;
			break;
		case 0x2c:								//INR L
            state->l += 1;
            FlagsZSP(state,state->l);
			break;
		case 0x2d: 							//DCR    L
            state->l -= 1;
            FlagsZSP(state,state->l);
			break;
		case 0x2e:  						//MVI L,byte
			state->l = opcode[1];
			state->pc++;
			break;
		case 0x2f:
			state->a = ~state->a;			//CMA
			break;
		case 0x30: UnimplementedInstruction(state); break;
		case 0x31: 							//LXI	SP,word
			state->sp = (opcode[2]<<8) | opcode[1];
			state->pc += 2;
			break;
		case 0x32: 							//STA    (word)
        {
			uint16_t offset = (opcode[2]<<8) | (opcode[1]);
            WriteMem(state, offset, state->a);
			state->pc += 2;
        }
			break;
		case 0x33: 							//INX    SP
			state->sp++;
			break;
		case 0x34: 							//INR	M
        {
			uint8_t res = ReadFromHL(state) + 1;
            FlagsZSP(state, res);
            WriteToHL(state, res);
        }
			break;
		case 0x35: 							//DCR    M
        {
			uint8_t res = ReadFromHL(state) - 1;
            FlagsZSP(state, res);
            WriteToHL(state, res);
        }
			break;
		case 0x36: 							//MVI	M,byte
        {
            WriteToHL(state, opcode[1]);
			state->pc++;
        }
			break;
		case 0x37: state->flags.cy = 1; break;
		case 0x38: UnimplementedInstruction(state); break;
		case 0x39: 							//DAD    SP
        {
			uint32_t hl = (state->h << 8) | state->l;
			uint32_t res = hl + state->sp;
			state->h = (res & 0xff00) >> 8;
			state->l = res & 0xff;
			state->flags.cy = ((res & 0xffff0000) > 0);
        }
			break;
		case 0x3a: 							//LDA    (word)
        {
			uint16_t offset = (opcode[2]<<8) | (opcode[1]);
			state->a = state->memory[offset];
			state->pc+=2;
        }
			break;
		case 0x3b: 							//DCX SP
			state->sp -= 1;
			break;
		case 0x3c: 							//INR A
            state->a += 1;
            FlagsZSP(state,state->a);
			break;
		case 0x3d:							//DCR A
            state->a -= 1;
            FlagsZSP(state,state->a);
			break;
		case 0x3e: 							//MVI    A,byte
			state->a = opcode[1];
			state->pc++;
			break;
		case 0x3f: state->flags.cy = 0; break;

		case 0x40: state->b = state->b; break;
		case 0x41: state->b = state->c; break;
		case 0x42: state->b = state->d; break;
		case 0x43: state->b = state->e; break;
		case 0x44: state->b = state->h; break;
		case 0x45: state->b = state->l; break;
		case 0x46: state->b = ReadFromHL(state); break;
		case 0x47: state->b = state->a; break;

		case 0x48: state->c = state->b; break;
		case 0x49: state->c = state->c; break;
		case 0x4a: state->c = state->d; break;
		case 0x4b: state->c = state->e; break;
		case 0x4c: state->c = state->h; break;
		case 0x4d: state->c = state->l; break;
		case 0x4e: state->c = ReadFromHL(state); break;
		case 0x4f: state->c = state->a; break;

		case 0x50: state->d = state->b; break;
		case 0x51: state->d = state->c; break;
		case 0x52: state->d = state->d; break;
		case 0x53: state->d = state->e; break;
		case 0x54: state->d = state->h; break;
		case 0x55: state->d = state->l; break;
		case 0x56: state->d = ReadFromHL(state); break;
		case 0x57: state->d = state->a; break;

		case 0x58: state->e = state->b; break;
		case 0x59: state->e = state->c; break;
		case 0x5a: state->e = state->d; break;
		case 0x5b: state->e = state->e; break;
		case 0x5c: state->e = state->h; break;
		case 0x5d: state->e = state->l; break;
		case 0x5e: state->e = ReadFromHL(state); break;
		case 0x5f: state->e = state->a; break;

		case 0x60: state->h = state->b; break;
		case 0x61: state->h = state->c; break;
		case 0x62: state->h = state->d; break;
		case 0x63: state->h = state->e; break;
		case 0x64: state->h = state->h; break;
		case 0x65: state->h = state->l; break;
		case 0x66: state->h = ReadFromHL(state); break;
		case 0x67: state->h = state->a; break;

		case 0x68: state->l = state->b; break;
		case 0x69: state->l = state->c; break;
		case 0x6a: state->l = state->d; break;
		case 0x6b: state->l = state->e; break;
		case 0x6c: state->l = state->h; break;
		case 0x6d: state->l = state->l; break;
		case 0x6e: state->l = ReadFromHL(state); break;
		case 0x6f: state->l = state->a; break;

		case 0x70: WriteToHL(state, state->b); break;		//MOV    M,B
		case 0x71: WriteToHL(state, state->c); break;		//MOV    M,C
		case 0x72: WriteToHL(state, state->d); break;		//MOV    M,D
		case 0x73: WriteToHL(state, state->e); break;		//MOV    M,E
		case 0x74: WriteToHL(state, state->h); break;		//MOV    M,H
		case 0x75: WriteToHL(state, state->l); break;		//MOV    M,L
		case 0x76:  break;                                  //HLT
		case 0x77: WriteToHL(state, state->a); break;		//MOV    M,A

		case 0x78: state->a  = state->b; break;
		case 0x79: state->a  = state->c; break;
		case 0x7a: state->a  = state->d;  break;
		case 0x7b: state->a  = state->e;  break;
		case 0x7c: state->a  = state->h;  break;
		case 0x7d: state->a  = state->l;  break;
		case 0x7e: state->a = ReadFromHL(state); break;
		case 0x7f: break;

		case 0x80:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->b; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD B
		case 0x81:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->c; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD C
		case 0x82:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->d; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD D
		case 0x83:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->e; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD E
		case 0x84:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->h; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD H
		case 0x85:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->l; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADD L
		case 0x86:  					//ADD M
        {
            uint16_t res = (uint16_t) state->a + (uint16_t) ReadFromHL(state);
            ArithFlagsA(state, res);
            state->a=(res&0xff);
        } break;
		case 0x87:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->a; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC A

		case 0x88:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->b + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC B
		case 0x89:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->c + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC C
		case 0x8a:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->d + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC D
		case 0x8b:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->e + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC E
		case 0x8c:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->h + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC H
		case 0x8d:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->l + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC L
		case 0x8e: 					//ADC M
        {
            uint16_t res = (uint16_t) state->a + (uint16_t) ReadFromHL(state) + state->flags.cy;
            ArithFlagsA(state, res);
            state->a=(res&0xff);
        } break;
		case 0x8f:  { uint16_t res = (uint16_t) state->a + (uint16_t) state->a + state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //ADC A


		case 0x90:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->b; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB B
		case 0x91:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->c; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB C
		case 0x92:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->d; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB D
		case 0x93:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->e; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB E
		case 0x94:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->h; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB H
		case 0x95:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->l; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB L
		case 0x96:  					//SUB M
        {
            uint16_t res = (uint16_t) state->a - (uint16_t) ReadFromHL(state);
            ArithFlagsA(state, res);
            state->a=(res&0xff);
        } break;
		case 0x97:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->a; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SUB A

		case 0x98:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->b - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB B
		case 0x99:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->c - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB C
		case 0x9a:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->d - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB D
		case 0x9b:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->e - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB E
		case 0x9c:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->h - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB H
		case 0x9d:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->l - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB L
		case 0x9e: 					//SBB M
        {
            uint16_t res = (uint16_t) state->a - (uint16_t) ReadFromHL(state) - state->flags.cy;
            ArithFlagsA(state, res);
            state->a=(res&0xff);
        } break;
		case 0x9f:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->a - state->flags.cy; ArithFlagsA(state, res); state->a=(res&0xff); } break;  //SBB A

		case 0xa0: state->a = state->a & state->b; LogicFlagsA(state);	break;
		case 0xa1: state->a = state->a & state->c; LogicFlagsA(state);	break;
		case 0xa2: state->a = state->a & state->d; LogicFlagsA(state);	break;
		case 0xa3: state->a = state->a & state->e; LogicFlagsA(state);	break;
		case 0xa4: state->a = state->a & state->h; LogicFlagsA(state);	break;
		case 0xa5: state->a = state->a & state->l; LogicFlagsA(state);	break;
		case 0xa6: state->a = state->a & ReadFromHL(state); LogicFlagsA(state);	break;
		case 0xa7: state->a = state->a & state->a; LogicFlagsA(state);	break;

		case 0xa8: state->a = state->a ^ state->b; LogicFlagsA(state);	break;
		case 0xa9: state->a = state->a ^ state->c; LogicFlagsA(state);	break;
		case 0xaa: state->a = state->a ^ state->d; LogicFlagsA(state);	break;
		case 0xab: state->a = state->a ^ state->e; LogicFlagsA(state);	break;
		case 0xac: state->a = state->a ^ state->h; LogicFlagsA(state);	break;
		case 0xad: state->a = state->a ^ state->l; LogicFlagsA(state);	break;
		case 0xae: state->a = state->a ^ ReadFromHL(state); LogicFlagsA(state);	break;
		case 0xaf: state->a = state->a ^ state->a; LogicFlagsA(state);	break;

		case 0xb0: state->a = state->a | state->b; LogicFlagsA(state);	break;
		case 0xb1: state->a = state->a | state->c; LogicFlagsA(state);	break;
		case 0xb2: state->a = state->a | state->d; LogicFlagsA(state);	break;
		case 0xb3: state->a = state->a | state->e; LogicFlagsA(state);	break;
		case 0xb4: state->a = state->a | state->h; LogicFlagsA(state);	break;
		case 0xb5: state->a = state->a | state->l; LogicFlagsA(state);	break;
		case 0xb6: state->a = state->a | ReadFromHL(state); LogicFlagsA(state);	break;
		case 0xb7: state->a = state->a | state->a; LogicFlagsA(state);	break;

		case 0xb8:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->b; ArithFlagsA(state, res); } break;  //CMP B
		case 0xb9:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->c; ArithFlagsA(state, res); } break;  //CMP C
		case 0xba:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->d; ArithFlagsA(state, res); } break;  //CMP D
		case 0xbb:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->e; ArithFlagsA(state, res); } break;  //CMP E
		case 0xbc:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->h; ArithFlagsA(state, res); } break;  //CMP H
		case 0xbd:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->l; ArithFlagsA(state, res); } break;  //CMP L
		case 0xbe:  { uint16_t res = (uint16_t) state->a - (uint16_t) ReadFromHL(state); ArithFlagsA(state, res); } break;  //CMP L
		case 0xbf:  { uint16_t res = (uint16_t) state->a - (uint16_t) state->a; ArithFlagsA(state, res); } break;  //CMP A

		case 0xc0:						//RNZ
			if (state->flags.z == 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;

		case 0xc1: 						//POP    B
            Pop(state, &state->b, &state->c);
			break;
		case 0xc2: 						//JNZ address
			if (0 == state->flags.z)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xc3:						//JMP address
			state->pc = (opcode[2] << 8) | opcode[1];
			break;
		case 0xc4: 						//CNZ adr
			if (state->flags.z == 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;

		case 0xc5: 						//PUSH   B
            Push(state, state->b, state->c);
			break;
		case 0xc6: 						//ADI    byte
        {
			uint16_t x = (uint16_t) state->a + (uint16_t) opcode[1];
            FlagsZSP(state, x&0xff);
			state->flags.cy = (x > 0xff);
			state->a = x&0xff;
			state->pc++;
        }
			break;
		case 0xc7: 					//RST 0
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x0000;
        }
			break;
		case 0xc8: 					//RZ
			if (state->flags.z)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1] << 8);
				state->sp += 2;
			}
			break;
		case 0xc9: 						//RET
			state->pc = state->memory[state->sp] | (state->memory[state->sp+1] << 8);
			state->sp += 2;
			break;
		case 0xca: 						//JZ adr
			if (state->flags.z)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xcb: UnimplementedInstruction(state); break;
		case 0xcc: 						//CZ adr
			if (state->flags.z == 1)
			{
                uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
                state->sp = state->sp - 2;
                state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;
		case 0xcd: 						//CALL address
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = (opcode[2] << 8) | opcode[1];
        }
 			break;
		case 0xce:					//ACI  byte
        {
			uint16_t x = state->a + opcode[1] + state->flags.cy;
            FlagsZSP(state, x&0xff);
			state->flags.cy = (x > 0xff);
			state->a = x & 0xff;
			state->pc++;
        }
			break;
		case 0xcf: 					//RST 1
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x0008;
        }
			break;

		case 0xd0:					//RNC
			if (state->flags.cy == 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;
		case 0xd1: 						//POP    D
            Pop(state, &state->d, &state->e);
			break;
		case 0xd2:						//JNC
			if (state->flags.cy == 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xd3:                      //OUT d8
			state->pc++;
			break;
		case 0xd4:						//CNC adr
			if (state->flags.cy == 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;
		case 0xd5: 						//PUSH   D
            Push(state, state->d, state->e);
			break;
		case 0xd6: 						//SUI    byte
        {
			uint8_t x = state->a - opcode[1];
            FlagsZSP(state, x&0xff);
			state->flags.cy = (state->a < opcode[1]);
			state->a = x;
			state->pc++;
        }
			break;
		case 0xd7: 					//RST 2
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x10;
        }
			break;
		case 0xd8: 					//RC
			if (state->flags.cy != 0)
			{
                state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
                state->sp += 2;
			}
			break;
		case 0xd9: UnimplementedInstruction(state); break;
		case 0xda: 					//JC
			if (state->flags.cy != 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xdb: 					//IN d8
			state->pc++;
			break;
		case 0xdc: 					//CC adr
			if (state->flags.cy != 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;
		case 0xdd: UnimplementedInstruction(state); break;
		case 0xde: 					//SBI byte
        {
			uint16_t x = state->a - opcode[1] - state->flags.cy;
            FlagsZSP(state, x&0xff);
			state->flags.cy = (x > 0xff);
			state->a = x & 0xff;
			state->pc++;
        }
			break;
		case 0xdf: 					//RST 3
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x18;
        }
			break;

		case 0xe0:					//RPO
			if (state->flags.p == 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;
		case 0xe1: 					//POP    H
            Pop(state, &state->h, &state->l);
			break;
		case 0xe2: 						//JPO
			if (state->flags.p == 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xe3:						//XTHL
        {
            uint8_t h = state->h;
            uint8_t l = state->l;
            state->l = state->memory[state->sp];
            state->h = state->memory[state->sp+1];
            WriteMem(state, state->sp, l );
            WriteMem(state, state->sp+1, h );
        }
			break;
		case 0xe4: 						//CPO adr
			if (state->flags.p == 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;
		case 0xe5: 						//PUSH   H
            Push(state, state->h, state->l);
			break;
		case 0xe6: 						//ANI    byte
        {
			state->a = state->a & opcode[1];
			LogicFlagsA(state);
			state->pc++;
        }
			break;
		case 0xe7: 					//RST 4
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x20;
        }
			break;
		case 0xe8: 					//RPE
			if (state->flags.p != 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;
		case 0xe9: 						//PCHL
			state->pc = (state->h << 8) | state->l;
			break;
		case 0xea: 						//JPE
			if (state->flags.p != 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xeb: 					//XCHG
        {
            uint8_t save1 = state->d;
            uint8_t save2 = state->e;
            state->d = state->h;
            state->e = state->l;
            state->h = save1;
            state->l = save2;
        }
			break;
		case 0xec: 					//CPE adr
			if (state->flags.p != 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;
		case 0xed: UnimplementedInstruction(state); break;
		case 0xee: 					//XRI data
        {
			uint8_t x = state->a ^ opcode[1];
            FlagsZSP(state, x);
			state->flags.cy = 0;		//data book says clear cy
			state->a = x;
			state->pc++;
        }
			break;
		case 0xef: 					//RST 5
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x28;
        }
			break;

		case 0xf0: 					//RP
			if (state->flags.s == 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;
		case 0xf1: 					//POP PSW
            Pop(state, &state->a,(unsigned char*) &state->flags);
			break;
		case 0xf2:
			if (state->flags.s == 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xf3: state->int_enable = 0;  break;
		case 0xf4: 						//CP
			if (state->flags.s == 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;

		case 0xf5: 						//PUSH   PSW
            Push(state, state->a, *(unsigned char*)&state->flags);
			break;

		case 0xf6: 						//ORI    byte
        {
			//AC set if lower nibble of h was zero prior to dec
			uint8_t x = state->a | opcode[1];
            FlagsZSP(state, x);
			state->flags.cy = 0;
			state->a = x;
			state->pc++;
        }
			break;
		case 0xf7: 					//RST 6
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x30;
        }
			break;
		case 0xf8:					//RM
			if (state->flags.s != 0)
			{
				state->pc = state->memory[state->sp] | (state->memory[state->sp+1]<<8);
				state->sp += 2;
			}
			break;
		case 0xf9: 					//SPHL
			state->sp = state->l | (state->h << 8);
			break;
		case 0xfa: 					//JM
			if (state->flags.s != 0)
				state->pc = (opcode[2] << 8) | opcode[1];
			else
				state->pc += 2;
			break;
		case 0xfb: state->int_enable = 1;  break;
		case 0xfc: 					//CM
			if (state->flags.s != 0)
			{
				uint16_t	ret = state->pc+2;
                WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
                WriteMem(state, state->sp-2, (ret & 0xff));
				state->sp = state->sp - 2;
				state->pc = (opcode[2] << 8) | opcode[1];
			}
			else
				state->pc += 2;
			break;

		case 0xfd: UnimplementedInstruction(state); break;
		case 0xfe: 						//CPI  byte
        {
			uint8_t x = state->a - opcode[1];
            FlagsZSP(state, x);
			state->flags.cy = (state->a < opcode[1]);
			state->pc++;
        }
			break;
		case 0xff: 					//RST 7
        {
			uint16_t	ret = state->pc+2;
            WriteMem(state, state->sp-1, (ret >> 8) & 0xff);
            WriteMem(state, state->sp-2, (ret & 0xff));
			state->sp = state->sp - 2;
			state->pc = 0x38;
        }
			break;
			state->instructions++;
	//}
	}

#if PRINTOPS
	printf("\t");
	printf("%c", state->flags.z ? 'z' : '.');
	printf("%c", state->flags.s ? 's' : '.');
	printf("%c", state->flags.p ? 'p' : '.');
	printf("%c", state->flags.cy ? 'c' : '.');
	printf("%c  ", state->flags.ac ? 'a' : '.');
	printf("A $%02x B $%02x C $%02x D $%02x E $%02x H $%02x L $%02x SP %04x\n", state->a, state->b, state->c,
           state->d, state->e, state->h, state->l, state->sp);
#endif
	//return cycles8080[*opcode];
	//cpu_dump(state);
	return 0;
}
