#include "i8080.h"
#include "debug.h"
#include "memory.h"
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Space Invaders"
#define H 256
#define W 224
#define F1 "res/roms/invaders.h"
#define F2 "res/roms/invaders.g"
#define F3 "res/roms/invaders.f"
#define F4 "res/roms/invaders.e"

SDL_Surface *surface;
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Texture *texture;
SDL_Renderer *renderer;

void emu_init(){

      /*
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, 2*W, 2*H,SDL_WINDOW_RESIZABLE);

      if(!window){
          printf("Error initializing SDL Window. Exiting...\n");
          exit(0);
      }
      */
      if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
          printf("SDL_Init failed: %s\n", SDL_GetError());
          exit(1);
      } else {
          window = SDL_CreateWindow("WINDOW_TITLE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL);
          renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
          texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
    }

/*
      window_surface = SDL_GetWindowSurface(window);
      if(!window_surface){
          printf("Error accessing SDL window surface. Exiting...");
          exit(0);
      }
*/
      surface = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
}

void sdl_draw(cpu *c){
    uint32_t *pix = surface->pixels;
    int i = VRAM_START;
    for(int columns = 0; columns < W; columns++){
        for(int row = H; row > 0; row -=8){
            for(int j = 0; j < 8; j++){
                int idx = (row - j) * W + columns;

                if(c->memory[i] & 1 << j){
                    pix[idx] = 0xFFFFFF;
                } else {
                    pix[idx] = 0x000000;
                }
            }

            i++;
        }
    }

    SDL_UpdateTexture(texture, NULL, pix, 64*sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(2);
    //SDL_BlitScaled(surface, NULL, window_surface, NULL);

    SDL_UpdateWindowSurface(window);
}

int main(int argc, char* argv[]){

    //int run = 0;

    emu_init();

    cpu* c = init_8080();

    if(load_file_to_memory("./res/invaders.rom", c, 0x00) != 0) {
        printf("Could not load file into system memory.");
    }

    while(1) {
      emulate_cycle(c);
      sdl_draw(c);
      c->instructions++;
    }

   return 0;
}
