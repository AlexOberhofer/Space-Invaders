#include "i8080.h"

void fail(cpu *i8080){
      printf("Error: Instruction not implemented. Exiting...");
      exit(1);
}

int init_8080(cpu* c){
      c->memory = calloc(0x10000, 1);
      c-> sp &= 0x0;
      c-> pc &= 0x0;
      return 0;
}

void emulate_cycle(cpu *i8080){

}
