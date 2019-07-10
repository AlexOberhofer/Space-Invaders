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

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define PRINTOPS 0

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
      int instructions;
} cpu;

#define VRAM_START 0x2400

void fail(cpu *i8080);
void emulate_cycle(cpu *i8080);
cpu* init_8080(void);
int parity(int x, int size);
void logicFlagA(cpu* i8080);
void arithFlagA(cpu *c, uint16_t result);
int Emulate8080Op(cpu* state);
void do_interrupt(cpu *c, uint16_t adr);
void UnimplementedInstruction(cpu *c);
//static void FlagsZSP(cpu *state, uint8_t value);
//static void WriteToHL(cpu* state, uint8_t value);
//static uint8_t ReadFromHL(cpu* state);

//CPU OP Functions
void opLXIB(cpu* c, uint8_t bval, uint8_t cval);
void opLXID(cpu* c, uint8_t dval, uint8_t eval);
void opLXIH(cpu* c, uint8_t hval, uint8_t lval);
void opJMPadr(cpu* c, uint8_t* opcode);
void opJC(cpu *c, uint8_t *opcode);
void opJMPNZ(cpu* c, uint8_t *opcode);
void opJZadr(cpu *c, uint8_t *opcode);
void opJNCadr(cpu *c, uint8_t *opcode);
void opLXIsp(cpu* c, uint16_t stack_val);
void opMVIB(cpu* c, uint8_t val);
void opMVIC(cpu *c, uint8_t val);
void opMVIH(cpu *c, uint8_t val);
void opMVIM(cpu *c, uint8_t val);
void opMVIA(cpu *c, uint8_t val);
void opMVID(cpu *c, uint8_t val);
void opMVIL(cpu* c, uint8_t val);
void opANIA(cpu* c, uint8_t val);
void opADIA(cpu *c, uint8_t val);
void opMOVDM(cpu *c, uint16_t offset);
void opMOVBM(cpu *c, uint16_t offset);
void opMOVCM(cpu *c, uint16_t offset);
void opMOVHM(cpu *c, uint16_t offset);
void opMOVMA(cpu *c, uint16_t offset);
void opMOVEM(cpu *c, uint16_t offset);
void opMOVAM(cpu *c, uint16_t offset);
void opMOVAH(cpu *c);
void opMOVLA(cpu *c);
void opMOVLB(cpu *c);
void opMOVAD(cpu *c);
void opMOVAE(cpu *c);
void opMOVCA(cpu *c);
void opMOVDA(cpu *c);
void opMOVAC(cpu *c);
void opMOVEA(cpu *c);
void opMOVAB(cpu *c);
void opMOVAL(cpu *c);
void opMOVHC(cpu *c);
void opDADB(cpu *c);
void opDADD(cpu *c);
void opDADH(cpu *c);
void opCALLAdr(cpu *c, uint16_t return_adr, uint8_t *opcode);
void opCZ(cpu *c, uint8_t *opcode);
void opCNZ(cpu *c, uint8_t *opcode);
void opLDAXB(cpu *c);
void opLDAXD(cpu *c);
void opINXB(cpu *c);
void opINXH(cpu *c);
void opINXD(cpu *c);
void opINRB(cpu *c);
void opINRA(cpu *c);
void opDCRA(cpu *c);
void opDCRB(cpu *c);
void opDCRC(cpu *c);
void opDCRM(cpu *c);
void opDCRD(cpu *c);
void opDCXH(cpu *c);
void opRET(cpu *c);
void opRNZ(cpu *c);
void opRNC(cpu* c);
void opRC(cpu* c);
void opCPIA(cpu *c, uint8_t value);
void opPUSHD(cpu *c);
void opPUSHH(cpu *c);
void opPUSHB(cpu* c);
void opPUSHPSW(cpu *c);
void opPOPPSW(cpu *c);
void opPOPH(cpu *c);
void opPOPB(cpu *c);
void opPOPD(cpu *c);
void opXCHG(cpu *c);
void opOUT(cpu *c);
void opRRC(cpu *c);
void opLDAadr(cpu *c, uint16_t offset);
void opSTAadr(cpu *c, uint16_t offset);
void opXRAA(cpu *c);
void opXRAB(cpu *c);
void opEL(cpu *c);
void opANAA(cpu *c);
void opIN(cpu *c);
void opRZ(cpu *c);
void opDAA(cpu *c);
void opORAB(cpu *c);
void opORAH(cpu *c);
void opORAM(cpu *c);
void opXTHL(cpu *c);
void opPCHL(cpu* c);
void opSTC(cpu *c);
void opMOVHA(cpu *c);
void opRLC(cpu *c);
void opLHLD(cpu *c, uint16_t offset);
void opRAR(cpu *c);
void opORI(cpu *c, uint8_t value);
void opSHLD(cpu *c, uint16_t offset);
void opADDM(cpu *c);
void opJM(cpu *c, uint16_t offset);