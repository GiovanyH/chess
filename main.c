#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

SDL_Window *window;
SDL_Renderer *renderer;

void ready();
void update(); // responsible for main loop
void handle_event(SDL_Event event);

const int WIDTH = 1360;
const int HEIGHT = 720;

SDL_Texture *tex = NULL;

char *pieces = "KQBNRP";
char chess_board[64][2];

int xMouse, yMouse;
char mouse_piece[2];

int chess_piece(char piece) {
    for(int i = 0; i < strlen(pieces); i++) {
        if(pieces[i] == piece) return i;
    }
    
    return -1;
}

void load(char *chess_cfg) {
    for(int i = 0; i < 8; i++) {
        chess_board[i][0] = chess_piece(chess_cfg[i])+'0'; chess_board[i][1] = '1';
        chess_board[i+8][0] = chess_piece('P')+'0'; chess_board[i+8][1] = '1';
        
        chess_board[64-i-1][0] = chess_piece(chess_cfg[i])+'0'; chess_board[64-i-1][1] = '0';
        chess_board[64-8-i-1][0] = chess_piece('P')+'0'; chess_board[64-8-i-1][1] = '0';
    }
}

int main() {
    char *chess_cfg = "RNBQKBNR";
    load(chess_cfg);
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    
    SDL_Surface *img = IMG_Load("pieces/pieces.png");
    tex = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    
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

int mouse_inside(int x, int y, int size) {
    if(xMouse > x && xMouse < x + size) {
        if(yMouse > y && yMouse < y + size) {
            return 1;
        }
    }
    return 0;
}

int mouse_square() {
    int square_sz = HEIGHT / 8;
    int x = (int)((xMouse - ((WIDTH-HEIGHT) * 0.5)) / square_sz);
    int y = (int)(yMouse / square_sz);
    
    return (y * 8) + x+1;
}

void draw_piece(int piece, int x, int y, int color) {
    int square_sz = HEIGHT / 8;
    SDL_Rect rect = {piece * (2000/6), color * 334, 2000/6, 334};
    SDL_Rect rect2 = {x, y, square_sz, square_sz};
    SDL_RenderCopy(renderer, tex, &rect, &rect2);
}

void draw_board() {
    float square_sz = HEIGHT / 8;
    for(int i = 0; i < 64; i++) {
        int x = (i - (floor(i/8) * 8));
        int y = (ceil(i/8));
        if(x%2 != y%2) SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        else SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        if(mouse_square() == i+1)
            SDL_SetRenderDrawColor(renderer, 20, 20, 200, 40);
        
        draw_square(square_sz, ((WIDTH-HEIGHT) * 0.5) + x * square_sz, y * square_sz);
        if(chess_board[i][0] != 0) {
            SDL_Rect rect = {(chess_board[i][0]-'0') * (2000/6), (chess_board[i][1]-'0') * 334, 2000/6, 334};
            SDL_Rect rect2 = {((WIDTH-HEIGHT) * 0.5) + x * square_sz, y * square_sz, square_sz, square_sz};
            SDL_RenderCopy(renderer, tex, &rect, &rect2);
        }
    }
}

void ready() {}

void update() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    draw_board();
    if(mouse_piece[0] != 0) draw_piece(mouse_piece[0]-'0', xMouse - (HEIGHT / 8) * 0.5, yMouse - (HEIGHT / 8) * 0.5, mouse_piece[1]-'0');
}

void handle_event(SDL_Event event) {
    switch(event.type) {
        case SDL_MOUSEBUTTONDOWN: {
            mouse_piece[0] = chess_board[mouse_square()-1][0];
            mouse_piece[1] = chess_board[mouse_square()-1][1];
            chess_board[mouse_square()-1][0] = 0;
            chess_board[mouse_square()-1][1] = 0;
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            chess_board[mouse_square()-1][0] = mouse_piece[0];
            chess_board[mouse_square()-1][1] = mouse_piece[1];
            mouse_piece[0] = 0; mouse_piece[1] = 0;
            break;
        }
        case SDL_MOUSEMOTION:
        SDL_GetMouseState(&xMouse, &yMouse);
        break;
        case SDL_QUIT:
        SDL_Quit();
        exit(0);
    }
}