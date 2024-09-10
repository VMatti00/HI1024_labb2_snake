#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.h"
#include <math.h>


struct snake{
    float x, y, vy, vx;
    int angle;
    int window_width,window_height;
    SDL_Renderer *pRenderer;
    SDL_Rect rect;
    SDL_Surface *pSurface;
};

static float distance(int x1, int y1, int x2, int y2);

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height)
{
    Snake *pSnake = malloc(sizeof(struct snake));
    pSnake->pRenderer = pRenderer;
    pSnake->rect.h = 42;
    pSnake->rect.w = 42;
    pSnake->rect.y = (window_width/4);
    pSnake->rect.x = (window_height/4);
    
    pSnake->window_width = window_width;
    pSnake->window_height = window_height;
}

void drawSnake(Snake *pSnake)
{
    // SDL_RenderDrawRects(pSnake->pRenderer, & pSnake->rect, 3);
    SDL_RenderFillRect(pSnake->pRenderer, &pSnake->rect);
    // SDL_FillRects (pSnake->pSurface, &pSnake->rect,10 , 0x100);
}

void turnLeft(Snake *pSnake){
    pSnake->angle-=5;
}

void turnRight(Snake *pSnake){
    pSnake->angle+=5;
}

void accelerate(Snake *pSnake){
    pSnake->vx+=0.2*sin(pSnake->angle*2*M_PI/360);
    pSnake->vy-=0.2*cos(pSnake->angle*2*M_PI/360);
}

void updateSnake(Snake *pSnake){
    pSnake->x+=pSnake->vx;
    pSnake->y+=pSnake->vy;
    if(pSnake->x<0) pSnake->x+=pSnake->window_width;
    else if (pSnake->x>pSnake->window_width) pSnake->x-=pSnake->window_width;
    if(pSnake->y<0) pSnake->y+=pSnake->window_height;
    else if(pSnake->y>pSnake->window_height) pSnake->y-=pSnake->window_height;
    pSnake->rect.x=pSnake->x;
    pSnake->rect.y=pSnake->y;
}