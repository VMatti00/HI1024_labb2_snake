#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "food.h"
#include <time.h>




Food *createFood(SDL_Renderer *pRenderer, int window_width, int window_height){
    Food *pFood = malloc(sizeof(struct food));
    if (!pFood) {
        printf("Failed to allocate memory for food\n");
        return NULL;
    }
    pFood->rect.h = 20;
    pFood->rect.w = 20;
    pFood->pRenderer = pRenderer;
    repositionFood(pFood, window_width, window_height);
    return pFood;
}

void drawFood(Food *pFood){
    SDL_SetRenderDrawColor(pFood->pRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(pFood->pRenderer, &pFood->rect);
}

void repositionFood(Food *pFood, int window_width, int window_height){
    srand(time(NULL));
    pFood->rect.x = (rand() % (window_width - 200));
    pFood->rect.y = (rand() % (window_height - 200));
}