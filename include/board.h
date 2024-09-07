#ifndef board_h
#define board_h

typedef struct board Board;

Board *createBoard(SDL_Renderer *pRenderer, int window_width, int window_height);
void drawBoard(Board *pBoard);

#endif