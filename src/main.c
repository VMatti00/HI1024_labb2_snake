#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "snake.h"
#include "border.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 700


struct game{
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    Snake *pSnake;
    Border *pBorder;
};
typedef struct game Game;

int initiate(Game *pGame);
void close(Game *pGame);

int main(int argc, char *argv[]){

    Game g={0};
    if(!initiate(&g)) return 1;
    // run(&g);
    SDL_Delay(5000);
    close(&g);
    printf("SUCCESS!!");
    return 0;
}

int initiate(Game *pGame){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
        printf("Error initiateing VIDEO or TIMER %s\n", SDL_GetError());
        return 0;
    }
    pGame->pWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pGame->pWindow)   
    {
        printf("Error creating the window: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }
    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    pGame->pRenderer = SDL_CreateRenderer(pGame->pWindow, -1, renderer_flags); 
    if(!pGame->pRenderer){
        printf("Error creating renderer: %s\n", SDL_GetError());
        close(pGame);
        return 0;
    }   
}

void close(Game *pGame)
{
    if(pGame->pRenderer) SDL_DestroyRenderer(pGame->pRenderer);
    if(pGame->pWindow) SDL_DestroyWindow(pGame->pWindow);
    SDL_Quit();
}