#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Window *window;
SDL_Renderer *renderer;

void ready();
void update(); // responsible for main loop
void handle_event(SDL_Event event);

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    
    SDL_Event event;
    
    while(1) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        update();
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)) { handle_event(event); }
    }
}

void draw_square(float size, int x, int y) {
    for(int i = 0; i < size; i++) 
        SDL_RenderDrawLine(renderer, x, y+i, x+size, y+i);
}

void draw_board() {
    float square_sz = 480 / 8;
    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 8; x++) {
            if(y%2 != x%2) SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            else SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            draw_square(square_sz, ((640-480) * 0.5) + x * square_sz, y * square_sz);
        }
    }
}

void ready() {}

void update() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_board();
}

void handle_event(SDL_Event event) {
    switch(event.type) {
        case SDL_QUIT:
        SDL_Quit();
        exit(0);
    }
}