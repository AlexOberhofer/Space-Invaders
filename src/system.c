#include "i8080.h"
#include "debug.h"
#include "memory.h"
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Space Invaders"
#define H 256
#define W 224
#define FPS 60
#define CYCLES_PER_FRAME 2000000 / FPS // 2Mhz at 60 fps
#define HALF_CYCLES_PER_FRAME CYCLES_PER_FRAME / 2
#define F1 "res/roms/invaders.h"
#define F2 "res/roms/invaders.g"
#define F3 "res/roms/invaders.f"
#define F4 "res/roms/invaders.e"
#define ROM_SINGLE_FILE "res/roms/invaders.rom"

uint8_t shift0;
uint8_t shift1;
uint8_t shift_offset;

uint32_t timer = 0;


SDL_Surface *surface;
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Texture *texture;
SDL_Renderer *renderer;

void emu_init(){
    //TODO: call cpu init here

      if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
          printf("SDL_Init failed: %s\n", SDL_GetError());
          exit(1);
      } else {
          window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL);
          renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
          texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
          surface = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
    }
      
}

int process_keypress(SDL_Event *e){

  const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_ESCAPE])
      return  0;
    if(keys[SDL_SCANCODE_P]) {
       while(1){
         if(SDL_PollEvent(e)){
           if(keys[SDL_SCANCODE_ESCAPE]){
             return 0;
           } else if(keys[SDL_SCANCODE_R]){
             break;
           }
         }

       }
    }
    return 1;
}

void sdl_draw(cpu *c){
  
    uint32_t pix[W * H * 4];
    int i = VRAM_START;
    for(int columns = 0; columns < W; columns++){
        for(int row = H; row > 0; row -=8){
            for(int j = 0; j < 8; j++){
                
                int idx = (row - j) * W + columns;
                int res = c->memory[i] & 1 << j;
                
                if(res){
                   pix[idx] = 0xFFFFFF;
                } else {
                   pix[idx] = 0x000000;
                }
            }
            i++;
        }
    }
    

    SDL_UpdateTexture(texture, NULL, pix, W * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    //SDL_Delay(1);

    SDL_UpdateWindowSurface(window);
}

uint8_t machine_in(uint8_t port){
    uint8_t a = 0;
    switch(port){
        case 0:
            return 1;
        case 1:
            return 0;
        case 3:{
            uint16_t v = (shift1 << 8) | shift0;
            return ((v >> (8 - shift_offset)) & 0xff);
        }
                        
    }
    return a;
}

void machine_out(uint8_t port, uint8_t value){
    switch (port)
    {
        case 2: 
            shift_offset = value & 0x7;
            break;
        case 4:
            shift0 = shift1;
            shift1 = value;
            break;
    }
}

int main(int argc, char* argv[]){

    int run = 1;

    int int_flag = 0;

    SDL_Event event;

    emu_init();

    cpu* c = init_8080();

    if(load_file_to_memory("./res/invaders.rom", c, 0x00) != 0) {
        printf("Could not load file into system memory.");
    }

    timer = SDL_GetTicks();
 
    while(run) {

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
            exit(1);
        }

   if(SDL_GetTicks() - timer > (1 / FPS) * 1000){
      timer = SDL_GetTicks();
   
    if(c-> pc == 0x0aea){
      printf("");
    }

    //if(c-> pc == 0x0af3){
      if(c-> instructions == 42140){
      
      cpu_dump(c);
      printf("\n%d", c->instructions);
      //exit(0);
      if(c->pc == 0x0AE1 && c->instructions > 42000){
          
      }
      
    }
   if(c->int_enable == 1){
          if(!int_flag){
              do_interrupt(c, 1);
              //GenerateInterrupt(c, 1);
              int_flag = 1;
          } else {
              do_interrupt(c, 2);
              //GenerateInterrupt(c, 2);
              int_flag = 0;
          }
          
      }
    uint8_t *opcode = &c->memory[c->pc];

    if (*opcode == 0xdb) //machine specific handling for IN    
    {    
        uint8_t port = opcode[1];    
        c->a = machine_in(port);    
        c->pc+=2;    
    }    
    else if (*opcode == 0xd3)  //OUT    
    {    
        uint8_t port = opcode[1];    
        machine_out(port, c->a);    
        c->pc+=2;    
    }    
    else {
        //emulate_cycle(c);
        
        Emulate8080Op(c);
    }  
      
      sdl_draw(c);  
      //c->instructions++;
    }
      run = process_keypress(&event);
    }

   return 0;
}
