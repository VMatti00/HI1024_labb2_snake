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
int updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);
void drawSnake(Snake *pSnake);
void accelerate(Snake *pSnake);
void turnRight(Snake *pSnake);
void turnLeft(Snake *pSnake);
void accelerateDown(Snake *pSnake);
void growSnake(Snake *pSnake);
void moveSnakeAI(Snake *pSnake, int food_x, int food_y, int board_width, int board_height, int board_x, int board_y);
int checkCollision(SDL_Rect pSnake, SDL_Rect pFood);
void closeSnake(Snake *pSnake);
float distance(int x1, int y1, int x2, int y2);
void avoidCollisionAI(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);
void avoidCollision(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);






#endif