#ifndef food_h
#define food_h

typedef struct food
{
    SDL_Renderer *pRenderer;
    SDL_Rect rect;
}Food;

Food *createFood(SDL_Renderer *pRenderer, int board_width, int board_height, int board_x, int board_y);
void drawFood(Food *pFood);
void repositionFood(Food *pFood, int board_width, int board_height, int board_x, int board_y);
int foodX(Food *pFood);
int foodY(Food *pFood);




#endif