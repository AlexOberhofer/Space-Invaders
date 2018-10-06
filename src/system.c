#include "i8080.h"
#include "disassembler.h"

#define H 256
#define W 224
#define F1 "res/roms/invaders.h"
#define F2 "res/roms/invaders.g"
#define F3 "res/roms/invaders.f"
#define F4 "res/roms/invaders.e"

void emu_init(){

}

int main(int argc, char* argv[]){

    FILE *f = fopen("./res/invaders.rom", "rb");
    if(f == NULL){
        printf("Error Could not open %s\n", argv[1]);
    }

    fseek (f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0l, SEEK_SET);

    int pc = 0x0;
    unsigned char *buffer = malloc(fsize);
    fread (buffer, fsize, 1, f);
    fclose (f);

    printf("FORMAT:  PC  OP  Disassembly\n");
    sleep(1);

    while(pc < fsize){
        pc += disassemble(buffer, pc);
        printf("\n");
    }

   return 0;
}
