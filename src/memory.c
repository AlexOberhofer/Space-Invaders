#include "memory.h"
#include "i8080.h"

void stack_push(cpu *c, int16_t address){
    
    c->memory[c->sp+1] = (address & 0xFF00) >> 8;
    c->memory[c->sp] = (address & 0xff);
    c->sp -= 2;
}

int load_file_to_memory(char *file_name, cpu *c, uint16_t address_start){
      FILE *f = fopen(file_name, "rb");

      if(f == NULL){
        printf("Error Could not open %s\n", file_name);
      }

      fseek (f, 0L, SEEK_END);
      int fsize = ftell(f);
      fseek(f, 0l, SEEK_SET);

      if(fread(&c->memory[address_start], 1, fsize, f) != fsize){
        return 1;
      }

      return 0;
}
