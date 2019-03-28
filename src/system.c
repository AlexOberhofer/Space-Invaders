#include "i8080.h"
#include "debug.h"
#include "memory.h"


#define H 256
#define W 224
#define F1 "res/roms/invaders.h"
#define F2 "res/roms/invaders.g"
#define F3 "res/roms/invaders.f"
#define F4 "res/roms/invaders.e"

void emu_init(){

}

int main(int argc, char* argv[]){

    int run = 0;

    FILE *f = fopen("./res/invaders.rom", "rb");
    if(f == NULL){
        printf("Error Could not open %s\n", argv[1]);
    }

    cpu* c = init_8080();

    if(load_file_to_memory("./res/invaders.rom", c, 0x00) != 0) {
        printf("Could not load file into system memory.");
    }
    int instr = 0;
    while(1) {
      emulate_cycle(c);
      c->instructions++;
    }

   return 0;
}
