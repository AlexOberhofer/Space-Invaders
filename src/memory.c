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

#include "memory.h"
#include "i8080.h"

void stack_push(cpu *c, uint8_t high, uint8_t low){
    c->sp = c->sp - 2;
    c->memory[c->sp + 1] = high;
    c->memory[c->sp] = low;  
}

void stack_pop(cpu *c, uint8_t *high, uint8_t *low){
    *low = c->memory[c->sp];
    *high = c->memory[c->sp+1];
    c->sp += 2;
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
