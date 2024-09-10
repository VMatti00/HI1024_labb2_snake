#ifndef snake_h
#define snake_h

typedef struct snake Snake;

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height);
void updateSnake(Snake *pSnake);

void drawSnake(Snake *pSnake);
void accelerate(Snake *pSnake);
void turnRight(Snake *pSnake);
void turnLeft(Snake *pSnake);


#endif