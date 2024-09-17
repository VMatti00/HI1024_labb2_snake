#ifndef food_h
#define food_h

typedef struct food
{
    SDL_Renderer *pRenderer;
    SDL_Rect rect;
}Food;

Food *createFood(SDL_Renderer *pRenderer, int window_width, int window_height);
void drawFood(Food *pFood);
void repositionFood(Food *pFood, int window_width, int window_height);




#endif