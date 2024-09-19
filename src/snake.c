#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_error.h>
#include "snake.h"
#include <math.h>
#include <stdio.h>

static float distance(int x1, int y1, int x2, int y2);

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y)
{
    Snake *pSnake = malloc(sizeof(struct snake));
        if (!pSnake) {
        printf("Failed to allocate memory for snake\n");
        return NULL;
    }
    SnakeSegment *segment = malloc(sizeof(SnakeSegment));
    if (!segment) {
        printf("Failed to allocate memory for snake segment\n");
        free(pSnake);
        return NULL;
    }
    segment->rect.h = 20;
    segment->rect.w = 20;
    segment->rect.y = y;
    segment->rect.x = x;
    segment->pNext = NULL;

    pSnake->pHead = segment;
    pSnake->pTail = segment;
    pSnake->length = 1;

    

    pSnake->vx = 0;
    pSnake->vy = 0;
    pSnake->pRenderer = pRenderer;

    return pSnake;
}

void drawSnake(Snake *pSnake)
{
    SnakeSegment *current = pSnake->pHead;
    SDL_SetRenderDrawColor(pSnake->pRenderer, 0, 255, 0, 255);
    while(current != NULL){
        SDL_RenderFillRect(pSnake->pRenderer, &current->rect);
        current = current->pNext;
    }
}

void turnLeft(Snake *pSnake){   
    pSnake->vx = -5;
    pSnake->vy = 0;
}

void turnRight(Snake *pSnake){
    pSnake->vx = 5;
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

void updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y) {
    // Save the previous position of the head
    int prev_x = pSnake->pHead->rect.x;
    int prev_y = pSnake->pHead->rect.y;

    // Move the head according to its velocity
    pSnake->pHead->rect.x += pSnake->vx;
    pSnake->pHead->rect.y += pSnake->vy;

    // Loop through the rest of the snake body and move each segment to the previous segment's position
    SnakeSegment *current = pSnake->pHead->pNext;
    while (current != NULL) {
        int temp_x = current->rect.x;
        int temp_y = current->rect.y;
        
        current->rect.x = prev_x;
        current->rect.y = prev_y;
        
        prev_x = temp_x;
        prev_y = temp_y;
        
        current = current->pNext;
    }

        if (pSnake->pHead->rect.x<board_x+1 || pSnake->pHead->rect.x>board_width+25 || 
        pSnake->pHead->rect.y<board_y+1 || pSnake->pHead->rect.y>board_height+25){
        pSnake->vx=0;
        pSnake->vy=0;
        
    }

    // // Check for board boundaries
    // if (pSnake->pHead->rect.x < board_x || pSnake->pHead->rect.x >= board_x + board_width ||
    //     pSnake->pHead->rect.y < board_y || pSnake->pHead->rect.y >= board_y + board_height) {
    //     printf("Snake hit the boundary!\n");
    //     // Add game over logic or boundary handling here
    // }
}


void growSnake(Snake *pSnake) {
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment) {
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }
    // printf("growing snake\n");
    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    // x y for tail
    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    // link tail
    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;

    if (!pSnake->pTail){
        printf("Error creating tail: %s\n", SDL_GetError());
    }
    pSnake->length++;
}