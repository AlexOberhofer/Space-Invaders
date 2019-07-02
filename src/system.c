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

uint8_t input_port = 0;

uint32_t timer = 0;

static uint8_t COLOR_FLAG = 1;


SDL_Surface *surface;
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Texture *texture;
SDL_Renderer *renderer;

void display_init(){

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
    SDL_Event event = * e;

    if (event.type == SDL_QUIT){
        exit(1);
    }
    
   /*
    if(keys[SDL_SCANCODE_P]) { // Pause
       while(1){
         if(SDL_PollEvent(e)){
           if(keys[SDL_SCANCODE_ESCAPE]){
             return 0;
           } else if(keys[SDL_SCANCODE_R]){
             return 1;
           }
         }

       }
    } 
    */

    if(event.type == SDL_KEYDOWN){

        uint32_t key = event.key.keysym.scancode;

        switch(key){
            case SDL_SCANCODE_C: input_port |= 0x01; break;
            case SDL_SCANCODE_X: input_port |= 0x04; break;
            case SDL_SCANCODE_A: input_port |= 0x20; break;
            case SDL_SCANCODE_S: input_port |= 0x10; break;
            case SDL_SCANCODE_D: input_port |= 0x40; break;
            case SDL_SCANCODE_ESCAPE: return 0;
        }

    } else if (event.type == SDL_KEYUP){
        
        uint32_t key = event.key.keysym.scancode;
        
        switch(key){
            case SDL_SCANCODE_C: input_port &= ~0x01; break;
            case SDL_SCANCODE_X: input_port &= ~0x04; break;
            case SDL_SCANCODE_A: input_port &= ~0x20; break;
            case SDL_SCANCODE_S: input_port &= ~0x10; break;
            case SDL_SCANCODE_D: input_port &= ~0x40; break;
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
                    if(row > 8 && row < 48 && COLOR_FLAG){ //RED TOP
                        pix[idx] = 0xFF0000;
                    } else if (row > 160 + 16 && row < 256 - 8 && COLOR_FLAG){ // GREEN MIDDLE
                        pix[idx] = 0x00F000;
                    }  else if (row > 256 - 16 && columns > 16 && columns < 256 - 122 && COLOR_FLAG){ // GREEN LOWER
                        pix[idx] = 0x00F000;
                    } else {
                        pix[idx] = 0xFFFFFF; //WHITE
                    }
                   
                } else {
                   pix[idx] = 0x000000; //BLACK
                }
            }
            i++;
        }
    }

    SDL_UpdateTexture(texture, NULL, pix, W * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
}

uint8_t machine_in(uint8_t port){
    uint8_t a = 0;
    switch(port){
        case 0:
            return 1;
        case 1:
            return input_port;
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

int main(int argc, char *argv[])
{

    int run = 1;

    int int_flag = 0;

    SDL_Event event;

    display_init();

    cpu *c = init_8080();

    //DEBUG
    cpu *c2 = init_8080();
    load_file_to_memory("./res/invaders.rom", c2, 0x00);
    

    if (load_file_to_memory("./res/invaders.rom", c, 0x00) != 0)
    {
        printf("Could not load file into system memory.");
    }

    

    while (run)
    {

        timer = SDL_GetTicks();

	    SDL_PollEvent(&event);


        if (SDL_GetTicks() - timer > (1 / FPS) * 1000)
        {
            //timer = SDL_GetTicks();

            if (c->int_enable == 1)
            {
                if (!int_flag)
                {
                    do_interrupt(c, 1);
                    //do_interrupt(c2, 1);
                    int_flag = 1;
                }
                else
                {
                    do_interrupt(c, 2);
                    //do_interrupt(c2, 2);
                    int_flag = 0;
                }
            }
            
            uint8_t *opcode2 = &c2->memory[c2->pc];

            for(int instrs = 0; instrs < 100; instrs++){

                process_keypress(&event);

                uint8_t *opcode = &c->memory[c->pc];

                if (*opcode == 0xdb) //machine specific handling for IN
                {
                    uint8_t port = opcode[1];
                    c->a = machine_in(port);
                    c->pc += 2;
                    //DEBUG
                    //c2->a = machine_in(port);
                    //c2->pc += 2;
                }
                else if (*opcode == 0xd3) //OUT
                {
                    uint8_t port = opcode[1];
                    machine_out(port, c->a);
                    c->pc += 2;
                    //machine_out(port, c2->a);
                    //c2->pc += 2;
                }
                else
                {
                    Emulate8080Op(c);
                    //printf(" Input Register : %02x \n", input_port);
                    //emulate_cycle(c);
                    //Emulate8080Op(c2);
                }
            }

            //if(compare(c, c2) == 0){
              // exit(1);
            //}
            sdl_draw(c);
        }
        run = process_keypress(&event);
    }

    free(c->memory);
    free(c2->memory);
    free(c);
    free(c2);

    return 0;
}
