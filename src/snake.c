#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.h"
#include "board.h"
#include <math.h>





struct snake_segment{

    SDL_Rect rect;
    struct snake_segment *pNext;
};

struct snake{
    float x, y, vy, vx;
    // int angle;
    int window_width,window_height;
    SDL_Renderer *pRenderer;
    SDL_Rect rect;
    SDL_Surface *pSurface;
};

struct board
{
    float x, y;
    int window_width, window_height;
    SDL_Renderer *pRenderer;
    SDL_Texture *pTexture;
    SDL_Rect rect;
};


static float distance(int x1, int y1, int x2, int y2);

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y)
{
    Snake *pSnake = malloc(sizeof(struct snake));
    pSnake->pRenderer = pRenderer;
    pSnake->rect.h = 42;
    pSnake->rect.w = 42;
    // pSnake->angle = 0;

    pSnake->rect.y = (window_width/4);
    pSnake->rect.x = (window_height/4);
    
    pSnake->window_width = window_width;
    pSnake->window_height = window_height;
    pSnake->x=x;
    pSnake->y=y;

    pSnake->vx = 0;
    pSnake->vy = 0;
}

void drawSnake(Snake *pSnake)
{
    // SDL_RenderDrawRects(pSnake->pRenderer, & pSnake->rect, 3);
    SDL_RenderFillRect(pSnake->pRenderer, &pSnake->rect);
    // SDL_FillRects (pSnake->pSurface, &pSnake->rect,10 , 0x100);
}

void turnLeft(Snake *pSnake){
    // function to turn left the snake and continue to accelarate
    
    pSnake->vx = -5;
    pSnake->vy = 0;
    
    // pSnake->angle-=5;
}

void turnRight(Snake *pSnake){
    pSnake->vx =5;
    pSnake->vy = 0;
}

void accelerate(Snake *pSnake){
    pSnake->vy = -5;
    pSnake->vx = 0;
}
void accelerateDown(Snake *pSnake){
    pSnake->vy = 5;
    pSnake->vx = 0;
}

void updateSnake(Snake *pSnake, Board *pBoard){
    pSnake->x+=pSnake->vx;
    pSnake->y+=pSnake->vy;
    // if my snake goesout of the board it will just stop
    if (pSnake->x<pBoard->rect.x || pSnake->x>pBoard->rect.w || pSnake->y<pBoard->rect.y || pSnake->y>pBoard->rect.h){
        pSnake->vx=0;
        pSnake->vy=0;
    }
    
    printf("Snake Position: (%d, %d)\n", pSnake->x, pSnake->y);
    printf("Snake Velocity: (%d, %d)\n", pSnake->vx, pSnake->vy);
    // if (pSnake->x<pBoard->rect.x || pSnake->x>pBoard->rect.w || pSnake->y<pBoard->rect.y || pSnake->y>pBoard->rect.h){
    //     pSnake->vx=0;
    //     pSnake->vy=0;
    // }
    // if (pSnake->x<0 || pSnake->x>pSnake->window_width || pSnake->y<0 || pSnake->y>pSnake->window_height){
    //     pSnake->vx=0;
    //     pSnake->vy=0;
    // }
    // if(pSnake->x<0) pSnake->x+=pSnake->window_width;
    // else if (pSnake->x>pSnake->window_width) pSnake->x-=pSnake->window_width;
    // if(pSnake->y<0) pSnake->y+=pSnake->window_height;
    // else if(pSnake->y>pSnake->window_height) pSnake->y-=pSnake->window_height;
    pSnake->rect.x=pSnake->x;
    pSnake->rect.y=pSnake->y;
}