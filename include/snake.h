#ifndef snake_h
#define snake_h



typedef struct snake_segment SnakeSegment;
typedef struct snake Snake;



Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y);
int updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);
void drawSnake(Snake *pSnake);
void accelerate(Snake *pSnake);
void turnRight(Snake *pSnake);
void turnLeft(Snake *pSnake);
void accelerateDown(Snake *pSnake);
void growSnake(Snake *pSnake);
void moveSnakeAI(Snake *pSnake, int food_x, int food_y, int board_width, int board_height, int board_x, int board_y);
void closeSnake(Snake *pSnake);
int avoidCollision(Snake *pSnake, int board_width, int board_height, int board_x, int board_y);





#endif