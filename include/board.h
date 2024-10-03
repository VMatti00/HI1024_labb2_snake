#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>



typedef struct board Board;

Board *createBoard(SDL_Renderer *pRenderer, int window_width, int window_height);
void drawBoard(Board *pBoard);
int getBoardWidth(Board *pBoard);
int getBoardHeight(Board *pBoard);
int getBoardX(Board *pBoard);
int getBoardY(Board *pBoard);

#endif