#ifndef snake_h
#define snake_h

typedef struct snake_segment{

    SDL_Rect rect; 
    struct snake_segment *pNext;
}SnakeSegment;


typedef struct snake{
    float x, y, vy, vx;
    int length;
    int window_width,window_height;
    SnakeSegment *pHead;
    SnakeSegment *pTail;
    SDL_Renderer *pRenderer;
}Snake;

// typedef struct snake_segment SnakeSegment;
// typedef struct snake Snake;
// typedef struct board Board;

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y);
void updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);
void drawSnake(Snake *pSnake);
void accelerate(Snake *pSnake);
void turnRight(Snake *pSnake);
void turnLeft(Snake *pSnake);
void accelerateDown(Snake *pSnake);
void growSnake(Snake *pSnake);


#endif