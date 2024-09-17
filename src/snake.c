#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "snake.h"
#include <math.h>
#include <stdio.h>






// typedef struct snake_segment Snake_segment;


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
    // pSnake->pRenderer = pRenderer;
    // pSnake->rect.h = 42;
    // pSnake->rect.w = 42;
    // // pSnake->angle = 0;

    // pSnake->rect.y = (window_width/4);
    // pSnake->rect.x = (window_height/4);
    
    // // pSnake->window_width = window_width;
    // // pSnake->window_height = window_height;
    // pSnake->x=x;
    // pSnake->y=y;

    // pSnake->vx = 0;
    // pSnake->vy = 0;
}

void drawSnake(Snake *pSnake)
{
    SnakeSegment *current = pSnake->pHead;
    SDL_SetRenderDrawColor(pSnake->pRenderer, 0, 255, 0, 255);
    while(current != NULL){
        SDL_RenderFillRect(pSnake->pRenderer, &current->rect);
        current = current->pNext;
    }

    // // SDL_RenderDrawRects(pSnake->pRenderer, & pSnake->rect, 3);
    // SDL_RenderFillRect(pSnake->pRenderer, &pSnake->rect);
    // // SDL_FillRects (pSnake->pSurface, &pSnake->rect,10 , 0x100);
}

void turnLeft(Snake *pSnake){
    // function to turn left the snake and continue to accelarate
    
    pSnake->vx = -5;
    pSnake->vy = 0;
    
    // pSnake->angle-=5;
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

void updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y){
    SnakeSegment *current = pSnake->pTail;
    while(current != pSnake->pHead){
        current->rect.x = current->pNext->rect.x;
        current->rect.y = current->pNext->rect.y;
        current = current->pNext;
    }
    pSnake->pHead->rect.x += pSnake->vx;
    pSnake->pHead->rect.y += pSnake->vy;
    
    if (pSnake->pHead->rect.x<board_x || pSnake->pHead->rect.x>board_width || pSnake->pHead->rect.y<board_y || pSnake->pHead->rect.y>board_height){
        pSnake->vx=0;
        pSnake->vy=0;
    }
    
    // pSnake->pHead->rect.x=pSnake->x;
    // pSnake->pHead->rect.y=pSnake->y;
}

void growSnake(Snake *pSnake) {
    // Check if pSnake is NULL
    if (pSnake == NULL) {
        fprintf(stderr, "Error: pSnake is NULL\n");
        return;
    }

    // Allocate memory for the new segment
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));

    // Segment *newSegment = (Segment *)malloc(sizeof(Segment));
    if (newSegment == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for new segment\n");
        return;
    }

    // Initialize the new segment (assuming Segment has a next pointer)
    newSegment->pNext = NULL;

    // Check if pTail is NULL (initial case)
    if (pSnake->pTail == NULL) {
        pSnake->pTail = newSegment;
        printf("Tail is NULL\n");
    } else {
        // Append the new segment to the tail
        pSnake->pTail->pNext = newSegment;
        pSnake->pTail = newSegment;
        printf("Tail is not NULL\n");
    }
}



/*

void growSnake(Snake *pSnake) {
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment) {
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }
    // printf("Growing snake\n");
    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    // Position the new segment at the tail's position
    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    // // Link the new segment to the current tail
    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;
    pSnake->length++;

    // // Debugging statements
    // printf("New segment added at position (%d, %d)\n", newSegment->rect.x, newSegment->rect.y);
    // printf("Snake length: %d\n", pSnake->length);
}


void growSnake(Snake *pSnake) {
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment) {
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }
    printf("Growing snake\n");
    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    // Position the new segment at the tail's position
    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;
    pSnake->length++;
}




void growSnake(Snake *pSnake) {
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment) {
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }
    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    // Position the new segment at the tail's position
    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    // Determine the direction of the tail segment to position the new segment correctly
    if (pSnake->pTail->rect.x == pSnake->pTail->pNext->rect.x) {
        // Tail is moving vertically
        if (pSnake->pTail->rect.y > pSnake->pTail->pNext->rect.y) {
            newSegment->rect.y += 20; // Moving down
        } else {
            newSegment->rect.y -= 20; // Moving up
        }
    } else {
        // Tail is moving horizontally
        if (pSnake->pTail->rect.x > pSnake->pTail->pNext->rect.x) {
            newSegment->rect.x += 20; // Moving right
        } else {
            newSegment->rect.x -= 20; // Moving left
        }
    }

    // Link the new segment to the current tail
    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;
    pSnake->length++;

    // Debugging statements
    printf("New segment added at position (%d, %d)\n", newSegment->rect.x, newSegment->rect.y);
    printf("Snake length: %d\n", pSnake->length);
}
*/