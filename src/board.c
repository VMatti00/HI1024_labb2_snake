#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "board.h"

Board *createBoard(SDL_Renderer *pRenderer, int window_width, int window_height) {
    Board *pBoard = malloc(sizeof(Board));
    pBoard->pRenderer = pRenderer;
    pBoard->rect.h = window_height - 100;
    pBoard->rect.w = window_width - 100;
    pBoard->rect.y = (window_width / 2) - (pBoard->rect.w / 2);
    pBoard->rect.x = (window_height / 2) - (pBoard->rect.h / 2);
    return pBoard;
}

void drawBoard(Board *pBoard)
{
    SDL_SetRenderDrawColor(pBoard->pRenderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(pBoard->pRenderer, &pBoard->rect);
}


int getBoardWidth(Board *pBoard)
{
    return pBoard->rect.w;
}

int getBoardHeight(Board *pBoard)
{
    return pBoard->rect.h;
}
int getBoardX(Board *pBoard)
{
    return pBoard->rect.x;
}
int getBoardY(Board *pBoard)
{
    return pBoard->rect.y;
}