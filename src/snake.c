#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_error.h>
#include "snake.h"
#include <math.h>
#include <stdio.h>
#include <windows.h>
#define SPEED 5

struct snake_segment{
    SDL_Rect rect; 
    struct snake_segment *pNext;
};

struct snake{
    float x, y, vy, vx;
    int length;
    int window_width,window_height;
    SnakeSegment *pHead;
    SnakeSegment *pTail;
    SDL_Renderer *pRenderer;
    int direction;
};

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
}Direction;

Snake *createSnake(SDL_Renderer *pRenderer, int window_width, int window_height, int x, int y){
    Snake *pSnake = malloc(sizeof(struct snake));
    if (!pSnake){
        printf("Failed to allocate memory for snake\n");
        return NULL;
    }
    SnakeSegment *segment = malloc(sizeof(SnakeSegment));
    if (!segment){
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
    Direction direction;
    pSnake->direction = direction;
    pSnake->pRenderer = pRenderer;



    return pSnake;
}

void drawSnake(Snake *pSnake){
    SnakeSegment *current = pSnake->pHead;
    SDL_SetRenderDrawColor(pSnake->pRenderer, 0, 255, 0, 255);
    while (current != NULL){
        SDL_RenderFillRect(pSnake->pRenderer, &current->rect);
        current = current->pNext;
    }

}

int updateSnake(Snake *pSnake, int board_width, int board_height, int board_x, int board_y){
    pSnake->pHead->rect.x += pSnake->vx;
    pSnake->pHead->rect.y += pSnake->vy;

    int prev_x = pSnake->pHead->rect.x;
    int prev_y = pSnake->pHead->rect.y;

    SnakeSegment *current = pSnake->pHead->pNext;
    while (current != NULL)
    {
        int temp_x = current->rect.x;
        int temp_y = current->rect.y;

        current->rect.x = prev_x;
        current->rect.y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;

        current = current->pNext;
    }
    if (pSnake->pHead->rect.x < board_x +1 || pSnake->pHead->rect.x > board_width + pSnake->pHead->rect.w ||
        pSnake->pHead->rect.y < board_y +1  || pSnake->pHead->rect.y > board_height + pSnake->pHead->rect.h)
    {

        return 0;
    }


    return 1;
}

void growSnake(Snake *pSnake){
    SnakeSegment *newSegment = malloc(sizeof(SnakeSegment));
    if (!newSegment){
        printf("Failed to allocate memory for new snake segment\n");
        return;
    }

    newSegment->rect.w = 20;
    newSegment->rect.h = 20;
    newSegment->pNext = NULL;

    newSegment->rect.x = pSnake->pTail->rect.x;
    newSegment->rect.y = pSnake->pTail->rect.y;

    pSnake->pTail->pNext = newSegment;
    pSnake->pTail = newSegment;

    if (!pSnake->pTail){
        printf("Error creating tail: %s\n", SDL_GetError());
    }
    pSnake->length++;
}

void closeSnake(Snake *pSnake){
    SnakeSegment *current = pSnake->pHead;
    while (current != NULL){
        SnakeSegment *temp = current;
        current = current->pNext;
        free(temp);
    }
    free(pSnake);
}
void moveSnakeAI(Snake *pSnake, int food_x, int food_y, int board_width, int board_height, int board_x, int board_y) {
    SDL_Rect head = pSnake->pHead->rect;

    int dist_x = food_x - head.x;
    int dist_y = food_y - head.y;

    if (abs(dist_x) > abs(dist_y)) {
        if (dist_x > 0) {
            if(pSnake->direction == LEFT){
                accelerate(pSnake);
                turnRight(pSnake);
                pSnake->vy = 0;

            }
            else{
                turnRight(pSnake);
                pSnake->vy = 0;
            }
            
        } else if (dist_x < 0) {
            if(pSnake->direction == RIGHT){
                accelerate(pSnake);
                turnLeft(pSnake);
                pSnake->vy = 0;
            }
            else{
                turnLeft(pSnake);
                pSnake->vy = 0;
            }

        }
    } else {
        if (dist_y > 0) {
            if(pSnake->direction == UP){
                turnLeft(pSnake);
                accelerateDown(pSnake);
                pSnake->vx = 0;
            }
            else{
                accelerateDown(pSnake);
                pSnake->vx = 0;
            }

        } else if (dist_y < 0) {
            if(pSnake->direction == DOWN){
                turnLeft(pSnake);
                accelerate(pSnake);
                pSnake->vx = 0;
            }
            else{
                accelerate(pSnake);
                pSnake->vx = 0;
            }
        }
    }

    avoidCollision(pSnake, board_width, board_height, board_x, board_y);


}

void turnLeft(Snake *pSnake){
    if(pSnake->vx == 0){
        pSnake->vy = 0;
        pSnake->vx = -SPEED;
        pSnake->direction = LEFT;
    }

}
void turnRight(Snake *pSnake){
    if(pSnake->vx == 0){
        pSnake->vy = 0;
        pSnake->vx = SPEED;
        pSnake->direction = RIGHT;
    }

}

void accelerate(Snake *pSnake){
    if(pSnake->vy == 0){
        pSnake->vx = 0;
        pSnake->vy = -SPEED;
        pSnake->direction = UP;
    }

}
void accelerateDown(Snake *pSnake){
    if(pSnake->vy == 0){
        pSnake->vx = 0;
        pSnake->vy = SPEED;
        pSnake->direction = DOWN;
    }

}


int avoidCollision(Snake *pSnake, int board_width, int board_height, int board_x, int board_y){
    if (pSnake->pHead->rect.x < board_x + 1 || pSnake->pHead->rect.x > board_width + 25 ||
        pSnake->pHead->rect.y < board_y + 1 || pSnake->pHead->rect.y > board_height + 25)
    {
        pSnake->vx = 0;
        pSnake->vy = 0;

        return 0;
    }
}
