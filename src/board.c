#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "board.h"


struct board
{
    float x, y;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};


Board *createBoard(SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Board *pBoard = malloc(sizeof(struct board));
    pBoard->pRenderer = pRenderer;
    pBoard->rect.h = window_height-100;
    pBoard->rect.w = window_width-100;
    pBoard->rect.y = (window_width/2)-(pBoard->rect.w/2);
    pBoard->rect.x = (window_height/2)- (pBoard->rect.h/2);

}

void drawBoard(Board *pBoard)
{
    SDL_RenderDrawRect(pBoard->pRenderer, &pBoard->rect);
}

// getBord dimnesions w h x y
int getBoardWidth(Board *pBoard)
{
    return pBoard->rect.w;
}