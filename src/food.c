#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "food.h"
#include <time.h>




Food *createFood(SDL_Renderer *pRenderer, int board_width, int board_height, int board_x, int board_y){
    Food *pFood = malloc(sizeof(struct food));
    if (!pFood) {
        printf("Failed to allocate memory for food\n");
        return NULL;
    }
    pFood->rect.h = 20;
    pFood->rect.w = 20;
    pFood->pRenderer = pRenderer;
    repositionFood(pFood,  board_width,  board_height,  board_x,  board_y);
    return pFood;
}

void drawFood(Food *pFood){
    SDL_SetRenderDrawColor(pFood->pRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(pFood->pRenderer, &pFood->rect);
}

void repositionFood(Food *pFood, int board_width, int board_height, int board_x, int board_y){
    
    // pFood->rect.x = (rand() % (board_width+25) - board_x +1 ) + board_x;
    // pFood->rect.y = (rand() % (board_height+25) - board_y +1 ) + board_y;
    // pFood->rect.x = (rand() % (board_width +20) + board_x + 1);
    // pFood->rect.y = (rand() % (board_height - 20) + board_y + 1);

    pFood->rect.x = (rand() % (board_width - board_x + 1) + board_x );
    pFood->rect.y = (rand() % (board_height - board_y + 1) + board_y );

}

int foodX(Food *pFood){
    return pFood->rect.x;
}
int foodY(Food *pFood){
    return pFood->rect.y;
}