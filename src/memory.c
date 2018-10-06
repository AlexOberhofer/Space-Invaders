#include "memory.h"
#include "i8080.h"

int load_file_to_memory(char *file_name, cpu *c, uint16_t address_start){
      FILE *f = fopen(file_name, "rb");

      fseek (f, 0L, SEEK_END);
      int fsize = ftell(f);
      fseek(f, 0l, SEEK_SET);

      if(fread(&c->memory[address_start], 1, fsize, f) != fsize){
        return 1;
      }

      return 0;
}
